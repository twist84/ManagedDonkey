#pragma once

#include "cseries/cseries.hpp"

const long k_url_size = 256;

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
	byte __pad139[3];
};
static_assert(sizeof(c_http_post_source) == 0x13C);

struct c_http_stream
{
protected:
	static long const k_http_header_size = 1024;

public:
	c_http_stream();

protected:
	virtual void build_headers() = 0;
	virtual bool verify_necessary_state_is_set() = 0;

public:
	virtual bool read(char* buffer, long buffer_length, long* bytes_read) = 0;
	virtual long get_length() = 0;
	virtual bool at_end() = 0;
	virtual bool reset();

	void add_header(char const* key, char const* value);
	void clear_headers();
	void set_headers(char const* headers);
	void set_position(long position);
	long get_position();
	void set_url(char const* url);
	char const* get_url() const;

protected:
	c_static_string<k_url_size> m_url;
	c_static_string<1536> m_headers;
	long m_headers_length;
	c_static_string<1024> m_extra_headers;
	long m_position;
};
static_assert(sizeof(c_http_stream) == 0xB0C);

struct c_http_get_stream : c_http_stream
{
public:
	c_http_get_stream();

protected:
	virtual void build_headers() override;
	virtual bool verify_necessary_state_is_set() override;

public:
	virtual bool read(char* buffer, long buffer_length, long* bytes_read) override;
	virtual long get_length() override;
	virtual bool at_end() override;
};
static_assert(sizeof(c_http_get_stream) == sizeof(c_http_stream));

struct c_http_post_stream : c_http_stream
{
public:
	c_http_post_stream();

protected:
	virtual void build_headers() override;
	virtual bool verify_necessary_state_is_set() override;

public:
	virtual bool read(char* buffer, long buffer_length, long* bytes_read) override;
	virtual long get_length() override;
	virtual bool at_end() override;

protected:
	c_http_post_source m_post_source;
	c_static_string<256> __stringC48;
	c_static_string<128> __stringD48;
	long m_extra_headers_length;
	long __stringC48_length;
	long __stringD48_length;
};
static_assert(sizeof(c_http_post_stream) == 0xDD4);

