#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/http_stream.hpp"

#pragma pack(push, 1)
struct c_http_buffer_downloader
{
//public:
	virtual ~c_http_buffer_downloader();

	c_url_string m_url;

	bool __unknown114;
	s_network_http_request_hash m_hash;
	byte __pad129[3];

	c_http_post_source m_post_source;

	bool __unknown268;
	c_static_string<1024> m_extra_headers;
	byte __pad669[3];

	long m_internal_status;
	long __unknown670;
	long m_cache_task_token;

	char* m_buffer;
	int m_buffer_size;
	int m_buffer_size_downloaded;

	dword __unknown684;
	dword __unknown688;
	dword __unknown68C;
	dword __unknown690;
	dword __unknown694;
};
static_assert(sizeof(c_http_buffer_downloader) == 0x698);
#pragma pack(pop)

