#include "networking/tools/http_stream.hpp"

#include <string>

c_http_stream::c_http_stream() :
	m_url(),
	m_headers(),
	m_headers_length(),
	m_extra_headers(),
	m_position()
{
}

bool c_http_stream::reset()
{
	m_position = 0;
	return true;
}

void c_http_stream::add_header(char const* key, char const* value)
{
	m_extra_headers.append_print("%s: %s\r\n", key, value);
}

void c_http_stream::clear_headers()
{
	m_extra_headers.clear();
}

void c_http_stream::set_headers(char const* headers)
{
	m_extra_headers.set(headers);
}

void c_http_stream::set_position(long position)
{
	m_position = position;
}

long c_http_stream::get_position()
{
	return m_position;
}

void c_http_stream::set_url(char const* url)
{
	ASSERT(url);
	ASSERT(strlen(url) < k_url_size);

	m_url.set(url);
}

char const* c_http_stream::get_url() const
{
	return m_url.get_string();
}

c_http_get_stream::c_http_get_stream() :
	c_http_stream()
{
}

void c_http_get_stream::build_headers()
{
	m_headers.print("GET %s HTTP/1.0\r\n%s\r\n", m_url.get_string(), m_extra_headers.get_string());
	m_headers_length = m_headers.length();
}

bool c_http_get_stream::verify_necessary_state_is_set()
{
	return m_url.length() != 0;
}

bool c_http_get_stream::read(char* buffer, long buffer_length, long* bytes_read)
{
	//return INVOKE_CLASS_MEMBER(0x00432A10, c_http_get_stream::read, buffer, buffer_length, bytes_read);

	ASSERT(buffer);
	ASSERT(bytes_read);

	char* dest_buffer = buffer;
	long dest_buffer_length = buffer_length;
	bool success = false;

	if (verify_necessary_state_is_set())
	{
		if (!m_position)
			build_headers();

		success = true;

		if (!at_end())
		{
			while (true)
			{
				if (at_end() || dest_buffer_length <= 0)
					break;

				long position = m_position;
				if (position >= m_headers_length)
				{
					//ASSERT(some_func(), "c_http_stream::read: Read past the maximum length of the stream.");
					success = false;
					break;
				}

				if (dest_buffer_length > m_headers_length - position)
					dest_buffer_length = m_headers_length - position;

				memmove(dest_buffer, &m_headers.get_string()[position], dest_buffer_length);

				dest_buffer += dest_buffer_length;
				m_position += dest_buffer_length;
				dest_buffer_length = buffer_length - (dest_buffer - buffer);

			}
		}

		*bytes_read = dest_buffer - buffer;
	}

	return success;
}

long c_http_get_stream::get_length()
{
	return m_headers_length;
}

bool c_http_get_stream::at_end()
{
	return m_position == m_headers_length;
}

c_http_post_stream::c_http_post_stream() :
	c_http_stream(),
	__stringC48(),
	__stringD48(),
	m_extra_headers_length(),
	__stringC48_length(),
	__stringD48_length()
{
}

void c_http_post_stream::build_headers()
{
	//INVOKE_CLASS_MEMBER(0x00432720, c_http_post_stream::build_headers);

	if (m_post_source.__unknown138)
	{
		__stringD48.set("\r\n");
		__stringD48_length = 2;

		__stringC48.set("\r\n");
		__stringC48_length = 2;

		m_headers.print("POST %s HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: %d\r\n%s\r\n",
			m_url.get_string(),
			m_post_source.m_contents_length - m_post_source.m_start_position + __stringD48_length + 2,
			m_extra_headers.get_string());

		m_extra_headers_length = m_headers.length();
	}
	else
	{
		__stringD48.set("\r\n--BUNGIEr0x0rz--\r\n");
		__stringD48_length = __stringD48.length();

		__stringC48.print("--BUNGIEr0x0rz\r\nContent-Disposition: form-data; name=\"upload\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n", m_post_source.m_filename.get_string(), m_post_source.m_content_type.get_string());
		__stringC48_length = __stringC48.length();

		m_headers.print("POST %s HTTP/1.0\r\nContent-type: multipart/form-data; boundary=BUNGIEr0x0rz\r\nContent-Length: %d\r\n%s\r\n",
			m_url.get_string(),
			__stringC48_length + __stringD48_length + m_post_source.m_contents_length - m_post_source.m_start_position,
			m_extra_headers.get_string());

		m_extra_headers_length = m_headers.length();
	}
}

bool c_http_post_stream::verify_necessary_state_is_set()
{
	return INVOKE_CLASS_MEMBER(0x00433000, c_http_post_stream::verify_necessary_state_is_set);
}

bool c_http_post_stream::read(char* buffer, long buffer_length, long* bytes_read)
{
	ASSERT(buffer);
	ASSERT(bytes_read);

	return INVOKE_CLASS_MEMBER(0x00432AD0, c_http_post_stream::read, buffer, buffer_length, bytes_read);
}

long c_http_post_stream::get_length()
{
	//return INVOKE_CLASS_MEMBER(0x004328F0, c_http_post_stream::get_length);

	return m_extra_headers_length
		+ __stringC48_length
		+ __stringD48_length
		+ m_post_source.m_contents_length - m_post_source.m_start_position;
}

bool c_http_post_stream::at_end()
{
	//return INVOKE_CLASS_MEMBER(0x004326B0, c_http_post_stream::at_end);

	return m_position == get_length();
}

