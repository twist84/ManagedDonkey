#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"

class c_network_http_request_description
{
public:
	c_network_http_request_description(const char* url, e_http_request_type request_type);
	c_network_http_request_description(const char* url, e_http_request_type request_type, const s_network_http_request_hash* request_hash);
	c_network_http_request_description();

	c_static_string<256> m_url;
	int32 m_request_type;
	bool m_has_file_hash;
	s_network_http_request_hash m_file_hash;
	c_static_string<1024> m_headers;
	c_http_post_source m_post_source;
};
static_assert(sizeof(c_network_http_request_description) == 0x658);
static_assert(0x000 == OFFSETOF(c_network_http_request_description, m_url));
static_assert(0x100 == OFFSETOF(c_network_http_request_description, m_request_type));
static_assert(0x104 == OFFSETOF(c_network_http_request_description, m_has_file_hash));
static_assert(0x105 == OFFSETOF(c_network_http_request_description, m_file_hash));
static_assert(0x119 == OFFSETOF(c_network_http_request_description, m_headers));
static_assert(0x51C == OFFSETOF(c_network_http_request_description, m_post_source));

class c_network_http_request_queue;
class c_network_http_request_item
{
public:
	c_network_http_request_item(int32 request_cookie, c_network_http_request_queue* request_queue, e_online_lsp_service_type service_type, const c_network_http_request_description* request_description);
	c_network_http_request_item();

	int32 m_request_cookie;
	int32 m_request_state;
	c_network_http_request_queue* m_request_queue;
	c_network_http_request_description m_request_description;

	// e_client_usage_type
	int32 m_client_usage_type;

	// e_online_lsp_service_type
	int32 m_service_type;

	char* m_fill_buffer;
	int32 m_fill_buffer_length;
	int32 m_fill_buffer_progress;

	int32 m_retries_remaining;
};
static_assert(sizeof(c_network_http_request_item) == 0x67C);

class c_network_http_request_queue
{
public:
	c_network_http_request_queue(e_network_http_request_queue_type queue_type);

	void cancel_request(int32 request_cookie);
	static c_network_http_request_queue* get(e_network_http_request_queue_type queue_type);
	e_network_http_request_result read_bytes(int32 request_cookie, char* buffer, int32 buffer_length, int32* out_bytes_read, e_network_http_request_queue_failure_reason* out_failure_reason);
	int32 start_request(e_online_lsp_service_type service_type, const c_network_http_request_description* request_description);
	int32 start_request_into_buffer(e_online_lsp_service_type service_type, const c_network_http_request_description* request_description, char* buffer, int32 buffer_length);
	bool has_file_changed(const c_network_http_request_description* request_description) const;
	e_network_http_request_result is_fill_buffer_complete(int32 request_cookie, int32* bytes_read, s_network_http_request_hash* request_hash, e_network_http_request_queue_failure_reason* out_failure_reason);

	int32 m_request_queue_type;
	int32 m_request_cookie;
	int32 m_file_queue_count;
	c_network_http_request_item m_request_items[32];
	int32 m_connection_token;
	uns32 m_ip_address;
	uns16 m_port;
	c_http_client m_http_client;
	c_http_get_stream m_http_get_stream;
	c_http_post_stream m_http_post_stream;
};
static_assert(sizeof(c_network_http_request_queue) == 0xF8D8);

extern c_network_http_request_queue(&g_network_http_request_queue)[k_network_http_request_queue_type_count];
extern c_static_string<256>& g_storage_url_subdirectory;

extern void __cdecl make_url(const c_static_string<256>* url, c_static_string<256>* out_url);
extern void __cdecl network_http_request_queue_dispose();
extern void __cdecl network_http_request_queue_initialize();
extern void __cdecl network_http_request_queue_update();
extern void network_storage_set_storage_subdirectory(const char* url);
extern void network_storage_set_storage_user(const char* user);

