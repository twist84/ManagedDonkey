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
	m_extra_headers.append_print("%s: \"%s\"\r\n", key, value);
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

bool c_http_get_stream::verify_nescessary_state_is_set()
{
	return m_url.length() != 0;
}

bool c_http_get_stream::read(char* buffer, long buffer_length, long* bytes_read)
{
	ASSERT(buffer);
	ASSERT(bytes_read);

	bool result = DECLFUNC(0x00432A10, bool, __thiscall, c_http_get_stream*, char*, long, long*)(this, buffer, buffer_length, bytes_read);

	return result;
	
	result = false;
	
	char* buf = buffer;
	long buf_len = buffer_length;
	
	if (verify_nescessary_state_is_set())
	{
		if (!m_position)
			build_headers();
	
		if (at_end())
		{
			*bytes_read = buf - buffer;
			result = true;
		}
	
		while (true)
		{
			if (at_end() || buf_len <= 0)
				break;
		}
	
		*bytes_read = buf - buffer;
	}
	
	return result;
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
	DECLFUNC(0x00432720, void, __thiscall, c_http_post_stream*)(this);
}

bool c_http_post_stream::verify_nescessary_state_is_set()
{
	return DECLFUNC(0x00433000, bool, __thiscall, c_http_post_stream*)(this);
}

bool c_http_post_stream::read(char* buffer, long buffer_length, long* bytes_read)
{
	ASSERT(buffer);
	ASSERT(bytes_read);

	return DECLFUNC(0x00432A10, bool, __thiscall, c_http_post_stream*, char*, long, long*)(this, buffer, buffer_length, bytes_read);
}

long c_http_post_stream::get_length()
{
	return DECLFUNC(0x004328F0, long, __thiscall, c_http_post_stream*)(this);
}

bool c_http_post_stream::at_end()
{
	return DECLFUNC(0x004326B0, bool, __thiscall, c_http_post_stream*)(this);
}

