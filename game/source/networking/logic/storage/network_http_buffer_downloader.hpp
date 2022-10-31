#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/http_stream.hpp"

enum e_download_status
{
	_http_download_status_none = 0,
	_http_download_status_unknown1,
	_http_download_status_succeeded,
	_http_download_status_failed,
	_http_download_status_failed_file_not_found,

	k_download_status_count
};

#pragma pack(push, 1)
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

//public:
	virtual ~c_http_buffer_downloader();

	static e_download_status __fastcall get_download_status(c_http_buffer_downloader* _this);
	static e_download_status __fastcall get_data(c_http_buffer_downloader* _this, void* unused, char const** buffer, long* buffer_size);
	static e_download_status __cdecl get_download_status_from_internal_status(e_internal_status internal_status);

	c_url_string m_url;

	bool __unknown114;
	s_network_http_request_hash m_hash;
	byte __pad129[3];

	c_http_post_source m_post_source;

	bool __unknown268;
	c_static_string<1024> m_extra_headers;
	byte __pad669[3];

	c_enum<e_internal_status, long, k_internal_status_count> m_internal_status;
	dword m_request_cookie;
	long m_cache_task_token;

	char* m_buffer;
	int m_buffer_max_size;
	int m_buffer_size;

	dword __unknown684;
	dword __unknown688;
	dword __unknown68C;
	dword __unknown690;
	dword __unknown694;
};
static_assert(sizeof(c_http_buffer_downloader) == 0x698);
#pragma pack(pop)

