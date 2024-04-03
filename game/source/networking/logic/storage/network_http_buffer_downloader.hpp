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

struct c_http_buffer_downloader
{
	enum e_internal_status
	{
		_internal_status_none = 0,
		_internal_status_cache_retrieving,
		_internal_status_unknown2,
		_internal_status_unknown3,
		_internal_status_cache_submitting,
		_internal_status_succeeded,
		_internal_status_failed,
		_internal_status_failed_file_not_found,

		k_internal_status_count
	};

public:
	virtual ~c_http_buffer_downloader();

	e_download_status __thiscall get_download_status();
	e_download_status __thiscall get_data(char const** buffer, long* buffer_size);
	static e_download_status __cdecl get_download_status_from_internal_status(e_internal_status internal_status);
	void __thiscall update();


protected:
	c_url_string m_url;

#pragma pack(push, 1)
	bool m_hash_is_set;
	s_network_http_request_hash m_hash;
	byte __pad129[3];
#pragma pack(pop)

	c_http_post_source m_post_source;

#pragma pack(push, 1)
	bool __unknown268;
	c_static_string<1024> m_extra_headers;
	byte __pad669[3];
#pragma pack(pop)

	c_enum<e_internal_status, long, _internal_status_none, k_internal_status_count> m_internal_status;
	dword m_request_cookie;
	long m_cache_task_token;

	char* m_buffer;
	long m_buffer_max_size;
	long m_buffer_size;

	dword __unknown684;
	dword __unknown688;
	dword __unknown68C;
	long m_attempt_index;
	dword __unknown694;
};
static_assert(sizeof(c_http_buffer_downloader) == 0x698);

template<long k_buffer_size>
struct c_http_stored_buffer_downloader :
	public c_http_buffer_downloader
{
	char m_stored_buffer[k_buffer_size];
};
//static_assert(sizeof(c_http_stored_buffer_downloader<4>) == sizeof(c_http_buffer_downloader) + 4);

template<typename t_blf_type>
struct c_http_blf_simple_downloader :
	public c_http_stored_buffer_downloader<sizeof(t_blf_type)>
{
public:
	e_download_status __thiscall get_data(t_blf_type const** data, long* data_size)
	{
		// override this function for hooks
	}

protected:
	long __unknown_index_or_count;
	char const* m_chunk_buffer;
	long m_chunk_buffer_size;
};

//struct s_some_data
//{
//	long some_count;
//	long many_things[256];
//};
//using t_some_data_downloader = c_http_blf_simple_downloader<s_some_data>;
//e_download_status __thiscall t_some_data_downloader::get_data(s_some_data const** data, long* data_size)
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

