#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"

enum e_online_lsp_service_type;

enum e_network_http_request_queue_type
{
	// REQ LSP
	_network_http_request_queue_type_unknown0 = 0,

	// OPT LSP
	_network_http_request_queue_type_unknown1,

	k_network_http_request_queue_type_count
};

enum e_network_http_request_result
{

};

enum e_network_http_request_queue_failure_reason
{

};

enum e_http_request_type
{
	// GET?
	_http_request_type_get = 0,

	// POST?
	_http_request_type_post,

	// are there any others supported?

	k_http_request_type_count
};

enum e_client_usage_type
{
	_client_usage_type_stream = 0,
	_client_usage_type_fill_buffer,

	k_client_usage_type_count
};

#pragma pack(push, 1)
struct c_network_http_request_description
{
	c_network_http_request_description(char const* url, e_http_request_type request_type);
	c_network_http_request_description(char const* url, e_http_request_type request_type, s_network_http_request_hash const* request_hash);
	c_network_http_request_description();

	c_static_string<256> m_url;
	long m_request_type;

	bool __unknown104;

	s_network_http_request_hash m_file_hash;
	c_static_string<1024> m_headers;
	byte __pad519[0x3];
	c_http_post_source m_post_source;
};
static_assert(sizeof(c_network_http_request_description) == 0x658);
#pragma pack(pop)

struct c_network_http_request_queue;
struct c_network_http_request_item
{
	c_network_http_request_item(long request_cookie, c_network_http_request_queue* request_queue, e_online_lsp_service_type service_type, c_network_http_request_description const* request_description);
	c_network_http_request_item();

	long m_request_cookie;
	long m_request_state;
	c_network_http_request_queue* m_request_queue;
	c_network_http_request_description m_request_description;

	// e_client_usage_type
	long m_client_usage_type;

	// e_online_lsp_service_type
	long m_service_type;

	char* m_fill_buffer;
	long m_fill_buffer_length;
	long m_fill_buffer_progress;

	long m_retries_remaining;
};
static_assert(sizeof(c_network_http_request_item) == 0x67C);

struct c_network_http_request_queue
{
	c_network_http_request_queue(e_network_http_request_queue_type queue_type);

	void cancel_request(long request_cookie);
	static c_network_http_request_queue* get(e_network_http_request_queue_type queue_type);
	e_network_http_request_result read_bytes(long request_cookie, char* buffer, long buffer_length, long* out_bytes_read, e_network_http_request_queue_failure_reason* out_failure_reason);
	long start_request(e_online_lsp_service_type service_type, c_network_http_request_description const* request_description);
	long start_request_into_buffer(e_online_lsp_service_type service_type, c_network_http_request_description const* request_description, char* buffer, long buffer_length);
	bool has_file_changed(c_network_http_request_description const* request_description) const;
	e_network_http_request_result is_fill_buffer_complete(long request_cookie, long* bytes_read, s_network_http_request_hash* request_hash, e_network_http_request_queue_failure_reason* out_failure_reason);

	long m_request_queue_type;
	long m_request_cookie;
	long m_file_queue_count;
	c_network_http_request_item m_request_items[32];
	long m_connection_token;
	dword m_ip_address;
	word m_port;
	c_http_client m_http_client;
	c_http_get_stream m_http_get_stream;
	c_http_post_stream m_http_post_stream;
};
static_assert(sizeof(c_network_http_request_queue) == 0xF8D8);

extern c_network_http_request_queue(&g_network_http_request_queue)[k_network_http_request_queue_type_count];
extern c_static_string<256>& g_storage_url_subdirectory;

extern void __cdecl network_http_request_queue_dispose();
extern void __cdecl network_http_request_queue_initialize();
extern void __cdecl network_http_request_queue_update();

