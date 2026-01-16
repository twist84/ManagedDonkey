#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/http_stream.hpp"

enum e_download_status
{
	_http_download_status_none = 0,
	_http_download_status_pending,
	_http_download_status_succeeded,
	_http_download_status_failed,
	_http_download_status_failed_file_not_found,

	k_download_status_count
};

class c_http_buffer_downloader
{
public:
	enum e_internal_status
	{
		_internal_status_none = 0,
		_internal_status_cache_retrieving,
		_internal_status_download_started,
		_internal_status_download_pending,
		_internal_status_cache_submitting,
		_internal_status_succeeded,
		_internal_status_failed,
		_internal_status_failed_file_not_found,

		k_internal_status_count
	};

public:
	virtual ~c_http_buffer_downloader() = default;

	e_download_status __thiscall get_download_status();
	e_download_status __thiscall get_data(const char** out_data, int32* out_data_length);
	static e_download_status __cdecl get_download_status_from_internal_status(e_internal_status internal_status);
	void __thiscall update();

	bool is_submitting_to_cache() const
	{
		return m_internal_status == _internal_status_cache_submitting;
	}

	void set_url(const c_url_string* url)
	{
		m_url.set(url);
	}

	int32 get_attempt_index() const
	{
		return m_attempt_index;
	}

protected:
	c_url_string m_url;
	bool m_use_specified_hash;
	s_network_http_request_hash m_hash;
	c_http_post_source m_http_post_source;
	bool m_http_post_source_set;
	c_static_string<1024> m_extra_headers;
	e_internal_status m_internal_status;
	int32 m_request_cookie;
	int32 m_cache_task_token;
	char* m_download_buffer;
	int32 m_download_buffer_length;
	int32 m_download_buffer_count;
	int32 m_automatic_retry_mode;
	int32 m_next_retry_backoff_milliseconds;
	int32 m_next_retry_milliseconds;
	int32 m_attempt_index;
};
COMPILE_ASSERT(sizeof(c_http_buffer_downloader) == 0x694);

template<int32 k_buffer_size>
class c_http_stored_buffer_downloader :
	public c_http_buffer_downloader
{
public:
	char m_stored_buffer[ALIGN_UP(k_buffer_size, 3)];
};

template<typename t_blf_type, int32 k_buffer_size = sizeof(t_blf_type)>
class c_http_blf_simple_downloader
{
public:
	e_download_status __thiscall get_data_no_update(const t_blf_type** data, int32* data_size)
	{
		// override this function for hooks
	}

	e_download_status get_download_status()
	{
		return m_downloader.get_download_status();
	}

	bool is_submitting_to_cache() const
	{
		return m_downloader.is_submitting_to_cache();
	}

	void set_url(const c_url_string* url)
	{
		m_downloader.set_url(url);
	}

	void update()
	{
		m_downloader.update();
	}

protected:
	c_http_stored_buffer_downloader<k_buffer_size> m_downloader;
	int32 m_last_attempt_index;
	const t_blf_type* m_last_chunk_data;
	int32 m_last_data_length;
};

//struct s_some_data
//{
//	int32 some_count;
//	int32 many_things[256];
//};
//using t_some_data_downloader = c_http_blf_simple_downloader<s_some_data>;
//e_download_status __thiscall t_some_data_downloader::get_data(const s_some_data** data, int32* data_size)
//{
//	static s_some_data static_data{};
//	if (!static_data.some_count)
//	{
//		static_data.many_things[static_data.some_count++] = 0xDEADBEEF;
//	}
//
//	if (*data)
//		*data = &static_data;
//
//	if (data_size)
//		*data_size = sizeof(static_data);
//
//	return _http_download_status_succeeded;
//}
//HOOK_DECLARE_CLASS_MEMBER(0 /* some address */, t_some_data_downloader, get_data);

