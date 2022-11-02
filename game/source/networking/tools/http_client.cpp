#include "networking/tools/http_client.hpp"

#include "cseries/console.hpp"
#include "networking/transport/transport.hpp"

#include <assert.h>

real g_http_client_test_failure_ratio = 0.0f;

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
	__time1048(0),
	__time1050(0),
	m_upstream_quota(-1),
	m_current_state()
{
	transport_register_transition_functions(nullptr, transport_shutdown, nullptr, this);
}

bool c_http_client::do_work(
	bool* upload_complete,
	char* out_response_content_buffer,
	long* out_response_content_buffer_count,
	long* out_http_response_code
)
{
	assert(upload_complete);

	bool result = false;

	*upload_complete = false;

	__time1050 = __time1048;
	__time1048 = __rdtsc();

	switch (m_current_state)
	{
	case _upload_state_connecting:
	{
		if (out_response_content_buffer_count)
			*out_response_content_buffer_count = 0;

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
			*out_response_content_buffer_count = 0;

		if (send_data())
			result = true;
	}
	break;
	case _upload_state_receiving_header:
	case _upload_state_receiving_content:
	{
		if (receive_data(
			upload_complete,
			out_response_content_buffer,
			out_response_content_buffer_count,
			out_http_response_code)
			)
		{
			result = true;
		}
	}
	default:
	{
		if (m_current_state)
			c_console::write_line("c_http_client::do_work: not in a valid state");
		else
			c_console::write_line("networking:http_client: do_work called when http client was stopped");
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

		char const* stream_url = m_http_stream->get_url();
		if (*upload_complete)
			c_console::write_line("networking:http_client: request completed successfully to '%s'", stream_url);
		else
			c_console::write_line("networking:http_client: request failed to '%s'", stream_url);
	}

	// assert
	if (*upload_complete && result)
		c_console::write_line("upload_complete should only be set on success.");

	return result;
}

dword c_http_client::get_ip_address()
{
	if (m_current_state)
		return htonl(m_address.ipv4_address);

	return 0;
}

void c_http_client::get_ip_address_string(long ipv4_address, c_static_string<16>* out_string)
{
	union
	{
		long value;
		byte bytes[4];
	};

	value = ipv4_address;

	out_string->print("%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
}

long c_http_client::get_upload_length()
{
	return m_http_stream->get_length();
}

long c_http_client::get_upload_position()
{
	return m_http_stream->get_position();
}

long c_http_client::get_upstream_quota()
{
	return m_upstream_quota;
}

bool c_http_client::is_connected()
{
	return m_current_state > _upload_state_connecting && m_current_state <= _upload_state_receiving_content;
}

bool c_http_client::is_started()
{
	return m_current_state != _upload_state_none;
}

bool c_http_client::parse_http_response(
	char const* buffer,
	long buffer_length,
	bool* out_completed_successfully,
	long* http_header_size,
	long* http_response_code,
	long* content_length
)
{
	assert(buffer);
	assert(out_completed_successfully);
	assert(m_current_state == _upload_state_receiving_header);
	assert(http_header_size);
	assert(http_response_code);
	assert(content_length);

	c_static_string<4096> http_response;
	c_static_string<4096> contents;

	http_response.set_bounded(buffer, buffer_length);

	bool result = true;

	if (http_response.length() > 9)
	{
		if (!http_response.starts_with("HTTP/1.0 ") && !http_response.starts_with("HTTP/1.1 "))
			return false;

		*http_response_code = atoi(http_response.get_string() + 9);

		for (long index = http_response.index_of("\r\n") + 2;;)
		{
			if (index >= http_response.length())
				break;

			long next_index = http_response.next_index_of("\r\n", index);
			if (index == next_index)
			{
				*http_header_size = next_index + 2;
				break;
			}

			if (http_response.substring(index, next_index - index, contents))
			{
				if (contents.starts_with("Content-Length: "))
					*content_length = atoi(contents.get_string() + strlen("Content-Length: "));
			}

			if (*http_header_size > 0)
			{
				if (*content_length < 0)
					result = false;
				else
					*out_completed_successfully = true;
			}

			index = next_index + 2;
		}
	}

	return result;
}

bool c_http_client::receive_data(
	bool* out_completed_successfully,
	char* destination,
	long* out_response_content_buffer_count,
	long* out_http_response_code
)
{
	// #TODO: implement code
	bool result = DECLFUNC(0x00433760, bool, __thiscall, c_http_client*, bool*, char*, long*, long*)(
		this,
		out_completed_successfully,
		destination,
		out_response_content_buffer_count,
		out_http_response_code);

	return result;
}

bool c_http_client::send_data()
{
	// #TODO: implement code
	bool result = DECLFUNC(0x004339C0, bool, __thiscall, c_http_client*)(this);

	return result;
}

void c_http_client::set_upstream_quota(long upstream_quota)
{
	m_upstream_quota = upstream_quota;
}

bool c_http_client::start(c_http_stream* stream, long ip_address, word port, char const* url, bool endpoint_is_alpha)
{
	assert(stream);
	assert(url);
	assert(m_current_state == _upload_state_none);
	assert(m_socket_count == 0);
	assert(ip_address != 0);
	assert(port != 0);

	m_http_stream = stream;
	m_address.address_length = sizeof(dword);
	m_address.ipv4_address = ip_address;
	m_address.port = port;
	get_ip_address_string(ip_address, &m_ip_address_string);

	if (transport_address_valid(&m_address))
	{
		transport_endpoint_setup(m_endpoint_ptr, _transport_type_tcp);

		if (endpoint_is_alpha)
			m_endpoint_ptr->flags |= FLAG(6);

		if (start_connect())
		{
			c_console::write_line("networking:http_client: request started to '%s'", url);
			m_http_stream->set_url(url);

			return true;
		}
	}

	return false;
}


bool c_http_client::start_connect()
{
	assert(m_current_state == _upload_state_none);
	assert(m_address.address_length == sizeof(unsigned long));
	assert(m_address.ipv4_address != 0);
	assert(m_address.port != 0);
	assert(m_socket_count == 0);

	if (transport_endpoint_async_connect(m_endpoint_ptr, &m_address))
	{
		m_socket_count++;
		m_current_state = _upload_state_connecting;

		return true;
	}

	transport_endpoint_disconnect(m_endpoint_ptr);
	c_console::write_line("networking:http_client: transport_endpoint_async_connect() failed to %s.", m_ip_address_string.get_string());

	return false;
}

bool c_http_client::stop()
{
	bool result = true;

	if (m_current_state)
	{
		if (!m_http_stream->reset())
			result = false;

		assert(m_socket_count == 1);

		transport_endpoint_disconnect(m_endpoint_ptr);
		m_socket_count--;
		m_current_state = _upload_state_none;
	}

	return result;
}

void c_http_client::transport_shutdown(void* client)
{
	assert(client);

	static_cast<c_http_client*>(client)->stop();
}

