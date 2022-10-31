#pragma once

#include "cseries/cseries.hpp"

struct s_file_reference;
struct c_http_post_source
{
	long m_source_type;
	c_static_string<32> m_content_type;
	c_static_string<260> m_filename;
	char const* m_contents_buffer;
	s_file_reference* m_contents_file;
	long m_contents_length;
	long m_start_position;
	bool __unknown138;
};
static_assert(sizeof(c_http_post_source) == 0x13C);

struct c_http_stream
{
protected:
	virtual void __cdecl build_headers();
	virtual bool __cdecl verify_nescessary_state_is_set();

public:
	virtual bool __cdecl read(char* buffer, long buffer_length, long* bytes_read);
	virtual long __cdecl get_length();
	virtual bool __cdecl at_end();
	virtual bool __cdecl reset();

	c_static_string<256> m_url;
	c_static_string<1536> m_headers;
	long __unknown704;
	c_static_string<1024> m_extra_headers;
	long m_position;
};
static_assert(sizeof(c_http_stream) == 0xB0C);

