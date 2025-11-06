#include "networking/tools/http_client.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"

#include <winsock.h>

HOOK_DECLARE_CLASS_MEMBER(0x00433760, c_http_client, receive_data);

real32 g_http_client_test_failure_ratio = 0.0f;

c_http_client::c_http_client() :
	m_address(),
	m_ip_address_string(),
	m_endpoint(),
	m_endpoint_ptr(&m_endpoint),
	m_http_stream(),
	m_response_buffer(),
	m_response_buffer_count(),
	m_bytes_remaining(),
	m_socket_count(),
	m_started(),
	m_start_time(0),
	m_previous_time(0),
	m_upstream_quota(NONE),
	m_current_state(_upload_state_none)
{
	transport_register_transition_functions(nullptr, transport_shutdown, nullptr, this);
}

bool c_http_client::do_work(
	bool* upload_complete,
	char* out_response_content_buffer,
	int32* out_response_content_buffer_count,
	int32* out_http_response_code
)
{
	ASSERT(upload_complete);

	bool result = false;

	*upload_complete = false;
	bool is_connected = false;

	m_previous_time = m_start_time;
	m_start_time = __rdtsc();

	switch (m_current_state)
	{
	case _upload_state_connecting:
	{
		if (out_response_content_buffer_count)
		{
			*out_response_content_buffer_count = 0;
		}

		bool is_connected = false;
		if (transport_endpoint_async_is_connected(m_endpoint_ptr, &is_connected))
		{
			result = true;

			if (is_connected)
				m_current_state = _upload_state_sending;
		}
	}
	break;
	case _upload_state_sending:
	{
		if (out_response_content_buffer_count)
		{
			*out_response_content_buffer_count = 0;
		}

		if (send_data())
		{
			result = true;
		}
	}
	break;
	case _upload_state_receiving_header:
	case _upload_state_receiving_content:
	{
		if (receive_data(
			upload_complete,
			out_response_content_buffer,
			out_response_content_buffer_count,
			out_http_response_code))
		{
			result = true;
		}
	}
	break;
	default:
	{
		if (m_current_state)
		{
			VASSERT(0, "c_http_client::do_work: not in a valid state");
		}
		else
		{
			event(_event_status, "networking:http_client: do_work called when http client was stopped");
		}
	}
	break;
	}

	if (!result || *upload_complete)
	{
		stop();

		if (g_http_client_test_failure_ratio > 0.0f)
		{
			if (rand() < g_http_client_test_failure_ratio * 32768.0f)
			{
				result = false;
				*upload_complete = false;
			}
		}

		const char* stream_url = m_http_stream->get_url();
		if (*upload_complete)
		{
			event(_event_message, "networking:http_client: request completed successfully to '%s'", stream_url);
		}
		else
		{
			event(_event_message, "networking:http_client: request failed to '%s'", stream_url);
		}
	}

	// ASSERT
	if (*upload_complete && !result)
	{
		VASSERT(0, "upload_complete should only be set on success.");
	}

	return result;
}

uns32 c_http_client::get_ip_address()
{
	if (m_current_state)
		return htonl(m_address.ipv4_address);

	return 0;
}

void c_http_client::get_ip_address_string(int32 ipv4_address, c_static_string<16>* out_string)
{
	union
	{
		int32 value;
		byte bytes[4];
	};

	value = ipv4_address;

	out_string->print("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

int32 c_http_client::get_upload_length()
{
	return m_http_stream->get_length();
}

int32 c_http_client::get_upload_position()
{
	return m_http_stream->get_position();
}

int32 c_http_client::get_upstream_quota()
{
	return m_upstream_quota;
}

bool c_http_client::is_connected()
{
	return m_current_state == _upload_state_sending || m_current_state == _upload_state_receiving_header || m_current_state == _upload_state_receiving_content;
}

bool c_http_client::is_started()
{
	return m_current_state != _upload_state_none;
}

bool c_http_client::parse_http_response(
	const char* buffer,
	int32 buffer_length,
	bool* out_completed_successfully,
	int32* http_header_size,
	int32* http_response_code,
	int32* content_length
)
{
	ASSERT(buffer);
	ASSERT(out_completed_successfully);
	ASSERT(m_current_state == _upload_state_receiving_header);
	ASSERT(http_header_size);
	ASSERT(http_response_code);
	ASSERT(content_length);

	c_static_string<4096> http_response;
	http_response.set_bounded(buffer, buffer_length);

	*out_completed_successfully = false;
	*http_header_size = NONE;
	*content_length = NONE;

	if (http_response.length() > 9)
	{
		if (!http_response.starts_with("HTTP/1.0 ") && !http_response.starts_with("HTTP/1.1 "))
		{
			return false;
		}

		*http_response_code = atoi(http_response.get_string() + 9);

		int32 next_index = 0;
		for (int32 index = http_response.index_of("\r\n") + 2;; index = next_index + 2)
		{
			if (index >= http_response.length())
			{
				break;
			}

			next_index = http_response.next_index_of("\r\n", index);
			if (index == next_index)
			{
				*http_header_size = next_index + 2;
				break;
			}

			c_static_string<4096> contents;
			if (http_response.substring(index, next_index - index, contents))
			{
				if (contents.starts_with("Content-Length: ") || contents.starts_with("content-length: "))
				{
					*content_length = atoi(contents.get_string() + strlen_debug("Content-Length: "));
				}
			}
		}

		if (*http_header_size > 0)
		{
			if (*content_length < 0)
			{
				return false;
			}
			else
			{
				*out_completed_successfully = true;
			}
		}
	}

	return true;
}

bool __thiscall c_http_client::receive_data(
	bool* out_completed_successfully,
	char* out_response_content_buffer,
	int32* out_response_content_buffer_count,
	int32* out_http_response_code
)
{
	bool result = false;

	ASSERT(out_completed_successfully);
	ASSERT(m_current_state == _upload_state_receiving_header || m_current_state == _upload_state_receiving_content);

	int16 bytes_read = 0;
	int32 input_buffer_size = 0;
	*out_completed_successfully = false;

	if (out_response_content_buffer_count)
	{
		ASSERT(*out_response_content_buffer_count > 0);
		input_buffer_size = *out_response_content_buffer_count;
		*out_response_content_buffer_count = 0;
	}

	if (4096 - m_response_buffer_count > 0 && (m_current_state == _upload_state_receiving_header || m_bytes_remaining > 0))
	{
		bytes_read = transport_endpoint_read(m_endpoint_ptr, m_response_buffer + m_response_buffer_count, static_cast<int16>(4096 - m_response_buffer_count));
		if (bytes_read <= 0)
		{
			if (bytes_read == -2)
			{
				result = true;
				bytes_read = 0;
			}
			else if (bytes_read)
			{
				event(_event_message, "networking:http_client: transport_endpoint_read() failed to %s with error code %d.",
					m_ip_address_string.get_string(),
					bytes_read);
			}
			else
			{
				event(_event_message, "networking:http_client: transport_endpoint_read() because %s closed the socket.",
					m_ip_address_string.get_string());
			}
		}
		else
		{
			m_response_buffer_count += bytes_read;
			result = true;
		}
	}
	else
	{
		result = true;
	}

	if (result)
	{
		ASSERT(bytes_read >= 0);

		result = false;
		if (m_current_state == _upload_state_receiving_header)
		{
			bool completed_successfully = false;
			int32 http_header_size = 0;
			int32 content_length = 0;
			int32 http_response_code = 0;

			if (parse_http_response(m_response_buffer, m_response_buffer_count, &completed_successfully, &http_header_size, &http_response_code, &content_length))
			{
				if (completed_successfully)
				{
					if (out_http_response_code)
					{
						*out_http_response_code = http_response_code;
					}

					if (http_response_code == 200)
					{
						if (out_response_content_buffer && out_response_content_buffer_count)
						{
							ASSERT(m_response_buffer_count >= http_header_size);

							int32 bytes_of_content_ready = m_response_buffer_count - http_header_size;
							for (int32 i = 0; i < bytes_of_content_ready; ++i)
								m_response_buffer[i] = m_response_buffer[i + http_header_size];

							m_response_buffer_count = bytes_of_content_ready;
							m_bytes_remaining = content_length - bytes_of_content_ready;
							m_current_state = _upload_state_receiving_content;

							result = true;
						}
						else
						{
							result = true;
							*out_completed_successfully = true;
						}
					}
					else
					{
						event(http_response_code == 404 ? _event_message : _event_warning,
							"networking:http_client: Received an unexpected '%d' response from %s.",
							http_response_code,
							m_ip_address_string.get_string());
					}
				}
				else
				{
					result = true;
				}
			}
			else if (m_response_buffer_count < 4096)
			{
				result = true;
			}
			else
			{
				event(_event_error, "networking:http_client: The response header from '%s' is too big to fit in the buffer.  This is probably a misconfiguration on the server.",
					m_ip_address_string.get_string());
			}
		}
		else
		{
			m_bytes_remaining -= bytes_read;
			result = true;
		}
	}

	if (result && m_current_state == _upload_state_receiving_content && out_response_content_buffer && out_response_content_buffer_count)
	{
		ASSERT(input_buffer_size > 0);
		ASSERT(m_response_buffer_count >= 0);

		if (input_buffer_size > m_response_buffer_count)
		{
			input_buffer_size = m_response_buffer_count;
		}

		csmemcpy(out_response_content_buffer, m_response_buffer, input_buffer_size);
		*out_response_content_buffer_count = input_buffer_size;

		for (int32 i = 0; i < m_response_buffer_count - input_buffer_size; ++i)
		{
			m_response_buffer[i] = m_response_buffer[i + input_buffer_size];
		}

		m_response_buffer_count -= input_buffer_size;

		if (!m_bytes_remaining && !m_response_buffer_count)
		{
			*out_completed_successfully = true;
		}
	}

	return result;
}

bool c_http_client::send_data()
{
	ASSERT(m_current_state == _upload_state_sending);

	char buffer[4096]{};
	int32 buffer_length = 4096;

	int32 upstream_quota = NONE;
	if (m_upstream_quota != NONE)
	{
		upstream_quota = static_cast<int32>((m_upstream_quota * (m_start_time - m_previous_time)) / 1000);
	}

	bool result = true;
	while (result && (upstream_quota == NONE || upstream_quota > 0))
	{
		int32 position = m_http_stream->get_position();
		int32 bytes_read = 0;

		result = false;
		if (!transport_endpoint_writeable(m_endpoint_ptr))
		{
			result = true;
			break;
		}

		if (m_http_stream->read(buffer, buffer_length, &bytes_read))
		{
			uns16 bytes_written = 0;
			ASSERT(IN_RANGE_INCLUSIVE(bytes_read, 0, SHRT_MAX - 1));

			if (bytes_read)
			{
				bytes_written = transport_endpoint_write(m_endpoint_ptr, buffer, static_cast<int16>(bytes_read));
			}
			else
			{
				bytes_written = 0;
			}

			if (bytes_written <= 0)
			{
				if (bytes_written == 0xFFFE)
				{
					m_http_stream->set_position(position);
					result = true;
					break;
				}

				if (bytes_written)
				{
					event(_event_warning, "networking:http_client: transport_endpoint_write() failed with error code %d in async_upload_block()", bytes_written);
				}
				else
				{
					event(_event_message, "networking:http_client: transport_endpoint_write() failed because the endpoint was closed");
				}
			}
			else
			{
				result = true;

				if (upstream_quota != NONE)
				{
					upstream_quota -= bytes_written;
				}

				if (bytes_written < bytes_read)
				{
					m_http_stream->set_position(position);
					break;
				}

				if (m_http_stream->at_end())
				{
					m_response_buffer_count = 0;
					m_current_state = _upload_state_receiving_header;
					break;
				}
			}
		}
		else
		{
			event(_event_warning, "networking:http_client: m_upload_stream::read failed from %s", m_ip_address_string);
		}
	}

	return result;
}

void c_http_client::set_upstream_quota(int32 upstream_quota)
{
	m_upstream_quota = upstream_quota;
}

bool c_http_client::start(c_http_stream* stream, int32 ip_address, uns16 port, const char* url, bool endpoint_is_alpha)
{
	ASSERT(stream);
	ASSERT(url);
	ASSERT(m_current_state == _upload_state_none);
	ASSERT(m_socket_count == 0);
	ASSERT(ip_address != 0);
	ASSERT(port != 0);

	m_http_stream = stream;
	m_address.address_length = sizeof(uns32);
	m_address.ipv4_address = ip_address;
	m_address.port = port;
	get_ip_address_string(ip_address, &m_ip_address_string);

	if (!transport_address_valid(&m_address))
	{
		return false;
	}

	transport_endpoint_setup(m_endpoint_ptr, _transport_type_tcp);

	if (endpoint_is_alpha)
	{
		m_endpoint_ptr->flags |= FLAG(6);
	}

	if (!start_connect())
	{
		return false;
	}

	event(_event_message, "networking:http_client: request started to '%s'", url);
	m_http_stream->set_url(url);

	return true;
}


bool c_http_client::start_connect()
{
	ASSERT(m_current_state == _upload_state_none);
	ASSERT(m_address.address_length == sizeof(uns32));
	ASSERT(m_address.ipv4_address != 0);
	ASSERT(m_address.port != 0);
	ASSERT(m_socket_count == 0);

	if (transport_endpoint_async_connect(m_endpoint_ptr, &m_address))
	{
		m_socket_count++;
		m_current_state = _upload_state_connecting;

		return true;
	}

	transport_endpoint_disconnect(m_endpoint_ptr);
	event(_event_message, "networking:http_client: transport_endpoint_async_connect() failed to %s.", m_ip_address_string);

	return false;
}

bool c_http_client::stop()
{
	if (m_current_state == _upload_state_none)
	{
		return true;
	}

	bool result = m_http_stream->reset();
	ASSERT(m_socket_count == 1);

	transport_endpoint_disconnect(m_endpoint_ptr);
	m_socket_count--;
	m_current_state = _upload_state_none;

	return result;
}

void c_http_client::transport_shutdown(void* client)
{
	ASSERT(client);

	static_cast<c_http_client*>(client)->stop();
}

