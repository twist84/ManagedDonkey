#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"

#pragma pack(push, 1)
struct c_network_http_request_description
{
	c_static_string<256> m_url;
	long m_request_type;

	bool __unknown104;
	s_network_http_request_hash m_file_hash;

	char m_headers[1024];
	byte __pad519[3];

	c_http_post_source m_post_source;
};
static_assert(sizeof(c_network_http_request_description) == 0x658);
#pragma pack(pop)

struct c_network_http_request_queue;
struct c_network_http_request_item
{
	long m_request_cookie;
	long m_request_state;
	c_network_http_request_queue* m_request_queue;
	c_network_http_request_description m_request_description;
	long m_client_usage_type;
	long m_service_type;
	char* m_fill_buffer;
	long m_fill_buffer_length;
	long m_fill_buffer_progress;
	long m_retries_remaining;
};
static_assert(sizeof(c_network_http_request_item) == 0x67C);

struct c_network_http_request_queue
{
	long m_request_queue_type;
	long __unknown4;
	long m_file_queue_count;
	c_network_http_request_item m_request_items[32];
	dword m_connection_token;
	dword m_ip_address;
	word m_port;
	c_http_client m_http_client;
	c_http_get_stream m_http_get_stream;
	c_http_post_stream m_http_post_stream;
};
static_assert(sizeof(c_network_http_request_queue) == 0xF8D8);

extern c_network_http_request_queue(&g_network_http_request_queue)[2];

