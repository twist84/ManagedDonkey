#pragma once

#include "cseries/cseries.hpp"

int32 const k_url_size = 256;

struct s_file_reference;

class c_http_post_source
{
private:
	static int32 const k_content_type_length = 32;

public: 
	enum e_contents_source
	{
		_contents_source_none,
		_contents_source_as_buffer,
		_contents_source_as_file,

		k_contents_source_count
	};

	c_http_post_source();

	void set_source_as_buffer(const char* contents_buffer, int32 contents_buffer_length);
	bool set_source_as_file(s_file_reference* contents_file);
	void set_content_type(const char* content_type);
	void set_filename(const char* filename);
	void set_start_position(int32 start_position);

	c_http_post_source& operator=(const c_http_post_source& other);

	const char* get_content_type() const;
	const char* get_filename() const;
	int32 get_contents_length() const;
	e_contents_source get_source_type() const;
	const char* get_contents_buffer() const;
	s_file_reference* get_contents_file() const;
	int32 get_start_position() const;
	bool is_json() const;

protected:
	c_enum<e_contents_source, int32, _contents_source_none, k_contents_source_count> m_source_type;
	c_static_string<k_content_type_length> m_content_type;
	c_static_string<260> m_filename;
	const char* m_contents_buffer;
	s_file_reference* m_contents_file;
	uns32 m_contents_length;
	int32 m_start_position;

	// Halo Online
	bool m_is_json;
};
COMPILE_ASSERT(sizeof(c_http_post_source) == 0x13C);

class c_http_stream
{
protected:
	static int32 const k_http_header_size = 1536;
	static int32 const k_extra_headers_size = 1024;

public:
	c_http_stream();

protected:
	virtual void build_headers() = 0;
	virtual bool verify_nescessary_state_is_set() = 0;

public:
	virtual bool read(char* buffer, int32 buffer_length, int32* bytes_read) = 0;
	virtual int32 get_length() = 0;
	virtual bool at_end() = 0;
	virtual bool reset();

	void add_header(const char* key, const char* value);
	void clear_headers();
	void set_headers(const char* headers);
	void set_position(int32 position);
	int32 get_position();
	void set_url(const char* url);
	const char* get_url() const;

protected:
	c_static_string<k_url_size> m_url;
	c_static_string<k_http_header_size> m_http_header;
	int32 m_http_header_length;
	c_static_string<k_extra_headers_size> m_extra_headers;
	int32 m_overall_position;
};
COMPILE_ASSERT(sizeof(c_http_stream) == 0xB0C);

class c_http_get_stream :
	public c_http_stream
{
public:
	c_http_get_stream();

protected:
	virtual void build_headers() override;
	virtual bool verify_nescessary_state_is_set() override;

public:
	virtual bool read(char* buffer, int32 buffer_length, int32* bytes_read) override;
	virtual int32 get_length() override;
	virtual bool at_end() override;
};
COMPILE_ASSERT(sizeof(c_http_get_stream) == sizeof(c_http_stream));

class c_http_post_stream :
	public c_http_stream
{
private:
	static int32 const k_mime_header_size = 256;
	static int32 const k_mime_footer_size = 128;

public:
	c_http_post_stream();

	void set_source(const c_http_post_source* source);

protected:
	virtual void build_headers() override;
	virtual bool verify_nescessary_state_is_set() override;

public:
	virtual bool read(char* buffer, int32 buffer_max_length, int32* bytes_read) override;
	virtual int32 get_length() override;
	virtual bool at_end() override;

protected:
	c_http_post_source m_http_post_source;
	c_static_string<k_mime_header_size> m_mime_header;
	c_static_string<k_mime_footer_size> m_mime_footer;
	int32 m_http_header_length;
	int32 m_mime_header_length;
	int32 m_mime_footer_length;
};
COMPILE_ASSERT(sizeof(c_http_post_stream) == 0xDD4);

