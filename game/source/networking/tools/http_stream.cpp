#include "networking/tools/http_stream.hpp"

#include "tag_files/files_windows.hpp"

#include <string>

c_http_post_source::c_http_post_source() :
	m_source_type(_contents_source_none),
	m_content_type("text/plain"),
	m_filename("blob"),
	m_contents_buffer(NULL),
	m_contents_file(NULL),
	m_contents_length(0),
	m_start_position(0),
	m_is_json(false)
{
}

void c_http_post_source::set_source_as_buffer(const char* contents_buffer, int32 contents_buffer_length)
{
	ASSERT(contents_buffer);

	m_source_type = _contents_source_as_buffer;
	m_contents_buffer = contents_buffer;
	m_contents_length = static_cast<uns32>(contents_buffer_length);
}

bool c_http_post_source::set_source_as_file(s_file_reference* contents_file)
{
	ASSERT(contents_file);

	m_source_type = _contents_source_as_file;
	m_contents_file = contents_file;
	if (file_get_size(contents_file, &m_contents_length))
	{
		VASSERT(m_contents_length >= 0, "We shouldn't be uploading files larger than 2 GB anyway.");

		return true;
	}
	return false;
}

void c_http_post_source::set_content_type(const char* content_type)
{
	ASSERT(content_type);
	ASSERT(strlen_debug(content_type) < k_content_type_length);

	m_content_type.set(content_type);
}

void c_http_post_source::set_filename(const char* filename)
{
	ASSERT(filename);
	ASSERT(strlen_debug(filename) < FILENAME_MAX);

	m_filename.set(filename);
}

void c_http_post_source::set_start_position(int32 start_position)
{
	ASSERT(start_position >= 0);

	m_start_position = start_position;
}

c_http_post_source& c_http_post_source::operator=(const c_http_post_source& other)
{
	m_source_type = other.m_source_type;
	m_content_type = other.m_content_type;
	m_filename = other.m_filename;
	m_contents_buffer = other.m_contents_buffer;
	m_contents_file = other.m_contents_file;
	m_contents_length = other.m_contents_length;
	m_start_position = other.m_start_position;
	m_is_json = other.m_is_json;

	return *this;
}

const char* c_http_post_source::get_content_type() const
{
	return m_content_type.get_string();
}

const char* c_http_post_source::get_filename() const
{
	return m_filename.get_string();
}

int32 c_http_post_source::get_contents_length() const
{
	return m_contents_length;
}

c_http_post_source::e_contents_source c_http_post_source::get_source_type() const
{
	return m_source_type;
}

const char* c_http_post_source::get_contents_buffer() const
{
	return m_contents_buffer;
}

s_file_reference* c_http_post_source::get_contents_file() const
{
	return m_contents_file;
}

int32 c_http_post_source::get_start_position() const
{
	return m_start_position;
}

bool c_http_post_source::is_json() const
{
	return m_is_json;
}

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

void c_http_stream::add_header(const char* key, const char* value)
{
	m_extra_headers.append_print("%s: %s\r\n", key, value);
}

void c_http_stream::clear_headers()
{
	m_extra_headers.clear();
}

void c_http_stream::set_headers(const char* headers)
{
	m_extra_headers.set(headers);
}

void c_http_stream::set_position(int32 position)
{
	m_position = position;
}

int32 c_http_stream::get_position()
{
	return m_position;
}

void c_http_stream::set_url(const char* url)
{
	ASSERT(url);
	ASSERT(strlen_debug(url) < k_url_size);

	m_url.set(url);
}

const char* c_http_stream::get_url() const
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

bool c_http_get_stream::read(char* buffer, int32 buffer_length, int32* bytes_read)
{
	//bool success = INVOKE_CLASS_MEMBER(0x00432A10, c_http_get_stream, read, buffer, buffer_length, bytes_read);

	ASSERT(buffer);
	ASSERT(bytes_read);

	char* dest_buffer = buffer;
	int32 dest_buffer_length = buffer_length;
	bool success = false;

	if (verify_nescessary_state_is_set())
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

				int32 position = m_position;
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

int32 c_http_get_stream::get_length()
{
	return m_headers_length;
}

bool c_http_get_stream::at_end()
{
	return m_position == m_headers_length;
}

c_http_post_stream::c_http_post_stream() :
	c_http_stream(),
	m_mime_header(),
	m_mime_footer(),
	m_extra_headers_length(),
	m_mime_header_length(),
	m_mime_footer_length()
{
}

void c_http_post_stream::set_source(const c_http_post_source* source)
{
	//INVOKE_CLASS_MEMBER(0x00432D90, c_http_post_stream, set_source, source);

	m_post_source = *source;
	reset();
}

void c_http_post_stream::build_headers()
{
	//INVOKE_CLASS_MEMBER(0x00432720, c_http_post_stream, build_headers);

	// added for HO
	if (m_post_source.is_json())
	{
		m_mime_footer.set("\r\n");
		m_mime_footer_length = m_mime_footer.length();

		m_mime_header.set("\r\n");
		m_mime_header_length = m_mime_header.length();

		m_headers.print("POST %s HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: %d\r\n%s\r\n",
			m_url.get_string(),
			m_post_source.get_contents_length() - m_post_source.get_start_position() + m_mime_footer_length + 2,
			m_extra_headers.get_string());
		m_extra_headers_length = m_headers.length();
	}
	else
	{
		m_mime_footer.set("\r\n--BUNGIEr0x0rz--\r\n");
		m_mime_footer_length = m_mime_footer.length();

		m_mime_header.print("--BUNGIEr0x0rz\r\nContent-Disposition: form-data; name=\"upload\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n",
			m_post_source.get_filename(),
			m_post_source.get_content_type());
		m_mime_header_length = m_mime_header.length();

		m_headers.print("POST %s HTTP/1.0\r\nContent-type: multipart/form-data; boundary=BUNGIEr0x0rz\r\nContent-Length: %d\r\n%s\r\n",
			m_url.get_string(),
			m_mime_header_length + m_mime_footer_length + m_post_source.get_contents_length() - m_post_source.get_start_position(),
			m_extra_headers.get_string());
		m_extra_headers_length = m_headers.length();
	}
}

bool c_http_post_stream::verify_nescessary_state_is_set()
{
	return INVOKE_CLASS_MEMBER(0x00433000, c_http_post_stream, verify_nescessary_state_is_set);

	//uns8 necessary_state_is_set = m_post_source.get_source_type() == c_http_post_source::_contents_source_as_file
	//	|| m_post_source.get_source_type() == c_http_post_source::_contents_source_as_buffer;
	//return necessary_state_is_set & (m_url.length() != 0);
}

bool c_http_post_stream::read(char* buffer, int32 buffer_length, int32* bytes_read)
{
	return INVOKE_CLASS_MEMBER(0x00432AD0, c_http_post_stream, read, buffer, buffer_length, bytes_read);

	//ASSERT(buffer);
	//ASSERT(bytes_read);
	//
	//char* buf = buffer;
	//int32 len = buffer_length;
	//
	//bool success = false;
	//
	//if (verify_nescessary_state_is_set())
	//{
	//	if (!get_position())
	//		build_headers();
	//
	//	success = true;
	//	while (!at_end() && len > 0)
	//	{
	//		int32 position = get_position();
	//		if (position >= m_extra_headers_length)
	//		{
	//			position -= m_extra_headers_length;
	//
	//			if (position >= m_mime_header_length)
	//			{
	//				position -= m_mime_header_length;
	//
	//				if (position >= m_post_source.get_contents_length())
	//				{
	//					position -= m_post_source.get_contents_length();
	//
	//					if (position >= m_mime_footer_length)
	//					{
	//						VASSERT(at_end(), "c_http_stream::read: Read past the maximum length of the stream.");
	//
	//						success = false;
	//						break;
	//					}
	//
	//					if (len > m_mime_footer_length - position)
	//						len = m_mime_footer_length - position;
	//
	//					csmemcpy(buf, &m_mime_footer.get_string()[position], len);
	//				}
	//				else
	//				{
	//					if (len > m_post_source.get_contents_length() - position)
	//						len = m_post_source.get_contents_length() - position;
	//
	//					switch (m_post_source.get_source_type())
	//					{
	//					case c_http_post_source::_contents_source_as_buffer:
	//					{
	//						csmemcpy(buf, &m_post_source.get_contents_buffer()[m_post_source.get_start_position()], len);
	//					}
	//					break;
	//					case c_http_post_source::_contents_source_as_file:
	//					{
	//						position += m_post_source.get_start_position();
	//						if (!file_read_from_position(m_post_source.get_contents_file(), position, len, false, buf))
	//							success = false;
	//					}
	//					break;
	//					}
	//				}
	//			}
	//			else
	//			{
	//				if (len > m_mime_header_length - position)
	//					len = m_mime_header_length - position;
	//				csmemcpy(buf, &m_mime_header.get_string()[position], len);
	//			}
	//		}
	//		else
	//		{
	//			if (len > m_extra_headers_length - position)
	//				len = m_extra_headers_length - position;
	//			csmemcpy(buf, &m_headers.get_string()[position], len);
	//		}
	//
	//		buf += len;
	//		m_position += len;
	//		len = buffer_length - (buf - buffer);
	//	}
	//
	//	*bytes_read = buf - buffer;
	//}
	//
	//return success;
}

int32 c_http_post_stream::get_length()
{
	//return INVOKE_CLASS_MEMBER(0x004328F0, c_http_post_stream, get_length);

	return m_extra_headers_length
		+ m_mime_header_length
		+ m_mime_footer_length
		+ m_post_source.get_contents_length() - m_post_source.get_start_position();
}

bool c_http_post_stream::at_end()
{
	return INVOKE_CLASS_MEMBER(0x004326B0, c_http_post_stream, at_end);

	return m_position == get_length();
}

