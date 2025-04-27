#pragma once

#include "cseries/cseries.hpp"

long const k_url_size = 256;

struct s_file_reference;

struct c_http_post_source
{
private:
	static long const k_content_type_length = 32;

public: 
	enum e_contents_source
	{
		_contents_source_none,
		_contents_source_as_buffer,
		_contents_source_as_file,

		k_contents_source_count
	};

	c_http_post_source();

	void set_source_as_buffer(char const* contents_buffer, long contents_buffer_length);
	bool set_source_as_file(s_file_reference* contents_file);
	void set_content_type(char const* content_type);
	void set_filename(char const* filename);
	void set_start_position(long start_position);

	c_http_post_source& operator=(c_http_post_source const& other);

	char const* get_content_type() const;
	char const* get_filename() const;
	long get_contents_length() const;
	e_contents_source get_source_type() const;
	char const* get_contents_buffer() const;
	s_file_reference* get_contents_file() const;
	long get_start_position() const;
	bool is_json() const;

protected:
	c_enum<e_contents_source, long, _contents_source_none, k_contents_source_count> m_source_type;
	c_static_string<k_content_type_length> m_content_type;
	c_static_string<260> m_filename;
	char const* m_contents_buffer;
	s_file_reference* m_contents_file;
	uint32 m_contents_length;
	long m_start_position;

	// Halo Online
	bool m_is_json;
	uint8 __pad139[0x3];
};
static_assert(sizeof(c_http_post_source) == 0x13C);

struct c_http_stream
{
protected:
	static long const k_http_header_size = 1536;
	static long const k_extra_headers_size = 1024;

public:
	c_http_stream();

protected:
	virtual void build_headers() = 0;
	virtual bool verify_nescessary_state_is_set() = 0;

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
	c_static_string<k_http_header_size> m_headers;
	long m_headers_length;
	c_static_string<k_extra_headers_size> m_extra_headers;
	long m_position;
};
static_assert(sizeof(c_http_stream) == 0xB0C);

struct c_http_get_stream :
	public c_http_stream
{
public:
	c_http_get_stream();

protected:
	virtual void build_headers() override;
	virtual bool verify_nescessary_state_is_set() override;

public:
	virtual bool read(char* buffer, long buffer_length, long* bytes_read) override;
	virtual long get_length() override;
	virtual bool at_end() override;
};
static_assert(sizeof(c_http_get_stream) == sizeof(c_http_stream));

struct c_http_post_stream :
	public c_http_stream
{
private:
	static long const k_mime_header_size = 256;
	static long const k_mime_footer_size = 128;

public:
	c_http_post_stream();

	void set_source(c_http_post_source const* source);

protected:
	virtual void build_headers() override;
	virtual bool verify_nescessary_state_is_set() override;

public:
	virtual bool read(char* buffer, long buffer_length, long* bytes_read) override;
	virtual long get_length() override;
	virtual bool at_end() override;

protected:
	c_http_post_source m_post_source;
	c_static_string<k_mime_header_size> m_mime_header;
	c_static_string<k_mime_footer_size> m_mime_footer;
	long m_extra_headers_length;
	long m_mime_header_length;
	long m_mime_footer_length;
};
static_assert(sizeof(c_http_post_stream) == 0xDD4);

