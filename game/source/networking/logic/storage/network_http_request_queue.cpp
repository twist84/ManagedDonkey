#include "networking/logic/storage/network_http_request_queue.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE_ARRAY(0x0224A4C8, c_network_http_request_queue, g_network_http_request_queue, k_network_http_request_queue_type_count);
REFERENCE_DECLARE(0x02269678, c_static_string<256>, g_storage_url_subdirectory);

c_network_http_request_description::c_network_http_request_description(char const* url, e_http_request_type request_type)
{
	DECLFUNC(0x004A2CE0, void, __thiscall, c_network_http_request_description*, char const*, e_http_request_type)(this, url, request_type);
}

c_network_http_request_description::c_network_http_request_description(char const* url, e_http_request_type request_type, s_network_http_request_hash const* request_hash)
{
	DECLFUNC(0x004A2D20, void, __thiscall, c_network_http_request_description*, char const*, e_http_request_type, s_network_http_request_hash const*)(this, url, request_type, request_hash);
}

c_network_http_request_description::c_network_http_request_description()
{
	DECLFUNC(0x004A2D90, void, __thiscall, c_network_http_request_description*)(this);
}

c_network_http_request_item::c_network_http_request_item(long request_cookie, c_network_http_request_queue* request_queue, e_online_lsp_service_type service_type, c_network_http_request_description const* request_description)
{
	DECLFUNC(0x004A2DC0, void, __thiscall, c_network_http_request_item*, long, c_network_http_request_queue*, e_online_lsp_service_type, c_network_http_request_description const*)(this, request_cookie, request_queue, service_type, request_description);
}

c_network_http_request_item::c_network_http_request_item()
{
	DECLFUNC(0x004A2E20, void, __thiscall, c_network_http_request_item*)(this);
}

c_network_http_request_queue::c_network_http_request_queue(e_network_http_request_queue_type queue_type)
{
	DECLFUNC(0x004A2E80, void, __thiscall, c_network_http_request_queue*, e_network_http_request_queue_type)(this, queue_type);
}

//.text:004A2F30 ; e_network_http_request_result c_network_http_request_queue::acquire_server(c_network_http_request_item* request_item)

void c_network_http_request_queue::cancel_request(long request_cookie)
{
	DECLFUNC(0x004A3050, void, __thiscall, c_network_http_request_queue*, long)(this, request_cookie);
}

//.text:004A3140 ; void c_network_http_request_item::failed(bool failed)
//.text:004A3170 ; c_network_http_request_item  c_network_http_request_queue::find_http_request_item_from_cookie(long request_cookie)
//.text:004A31C0 ; long c_network_http_request_queue::find_http_request_item_index(c_network_http_request_item const* request_item) const 

c_network_http_request_queue* c_network_http_request_queue::get(e_network_http_request_queue_type queue_type)
{
	//return DECLFUNC(0x004A3220, c_network_http_request_queue*, __thiscall, c_network_http_request_queue*, e_network_http_request_queue_type)(this, queue_type);

	ASSERT(VALID_INDEX(queue_type, k_network_http_request_queue_type_count));

	return &g_network_http_request_queue[queue_type];
}

//.text:004A3240 ; c_network_http_request_description const* c_network_http_request_item::get_description() const
//.text:004A3250 ; char* c_network_http_request_item::get_fill_buffer() const
//.text:004A3260 ; long c_network_http_request_item::get_fill_buffer_length() const
//.text:004A3270 ; long c_network_http_request_item::get_fill_buffer_progress() const
//.text:004A3280 ; char const* c_network_http_request_description::get_headers() const
//.text:004A3290 ; c_http_post_source const* c_network_http_request_description::get_post_source() const
//.text:004A32A0 ; 
//.text:004A32B0 ; 
//.text:004A32C0 ; e_http_request_type c_network_http_request_description::get_request_type() const
//.text:004A32D0 ; long c_network_http_request_item::get_retries_remaining() const
//.text:004A32E0 ; e_online_lsp_service_type c_network_http_request_item::get_service_type() const
//.text:004A32F0 ; 
//.text:004A3300 ; 
//.text:004A3350 ; e_network_http_request_result c_network_http_request_queue::is_fill_buffer_complete(long request_cookie, long* bytes_read, s_network_http_request_hash* request_hash, e_network_http_request_queue_failure_reason* out_failure_reason)

void __cdecl make_url(c_static_string<256> const* url, c_static_string<256>* out_url)
{
	//INVOKE(0x004A34B0, make_url, url, out_url);
 
	ASSERT(out_url);

	c_static_string<256> temp = *url;
	c_static_string<256> title_storage = "/storage/title/";

	if (temp.starts_with(title_storage.get_string()))
	{
		c_static_string<256> temp_substring;
		temp.substring(title_storage.length(), temp.length() - title_storage.length(), temp_substring);
		temp.print("%s%s/%s", title_storage.get_string(), g_storage_url_subdirectory.get_string(), temp_substring.get_string());
	}

	out_url->set(temp.get_string());
}
HOOK_DECLARE(0x004A34B0, make_url);

void __cdecl network_http_request_queue_dispose()
{
	INVOKE(0x004A3620, network_http_request_queue_dispose);
}

void __cdecl network_http_request_queue_initialize()
{
	INVOKE(0x004A3630, network_http_request_queue_initialize);
}

//.text:004A36A0 ; void network_storage_set_storage_subdirectory(char const* storage_subdirectory)
//.text:004A36C0 ; void network_storage_set_storage_user(char const* storage_user)

e_network_http_request_result c_network_http_request_queue::read_bytes(long request_cookie, char* buffer, long buffer_length, long* out_bytes_read, e_network_http_request_queue_failure_reason* out_failure_reason)
{
	return DECLFUNC(0x004A3730, e_network_http_request_result, __thiscall, c_network_http_request_queue*, long, char*, long, long*, e_network_http_request_queue_failure_reason*)(this, request_cookie, buffer, buffer_length, out_bytes_read, out_failure_reason);
}

//.text:004A37E0 ; e_network_http_request_result c_network_http_request_queue::read_bytes_internal(c_network_http_request_item* request_item, char* buffer, long buffer_length, long* out_bytes_read, bool* out_item_was_removed, e_network_http_request_queue_failure_reason* out_failure_reason)
//.text:004A3AD0 ; void c_network_http_request_queue::remove_first_item_from_queue()
//.text:004A3B00 ; void c_network_http_request_queue::request_failed(c_network_http_request_item* request_item, bool, bool )
//.text:004A3B60 ; bool c_network_http_request_queue::run_request(char* buffer, long buffer_length, long* out_bytes_read, bool* upload_complete, long* http_response_code)
//.text:004A3C00 ; void c_network_http_request_item::set_client_usage_type(e_client_usage_type client_usage_type)
//.text:004A3C10 ; void c_network_http_request_item::set_fill_buffer(char* fill_buffer)
//.text:004A3C20 ; void c_network_http_request_item::set_fill_buffer_length(long fill_buffer_length)
//.text:004A3C30 ; void c_network_http_request_item::set_fill_buffer_progress(long fill_buffer_progress)
//.text:004A3C40 ; void c_network_http_request_item::set_request_state(e_network_http_request_state request_state)

long c_network_http_request_queue::start_request(e_online_lsp_service_type service_type, c_network_http_request_description const* request_description)
{
	return DECLFUNC(0x004A3C50, long, __thiscall, c_network_http_request_queue*, e_online_lsp_service_type, c_network_http_request_description const*)(this, service_type, request_description);
}

long c_network_http_request_queue::start_request_into_buffer(e_online_lsp_service_type service_type, c_network_http_request_description const* request_description, char* buffer, long buffer_length)
{
	return DECLFUNC(0x004A3D80, long, __thiscall, c_network_http_request_queue*, e_online_lsp_service_type, c_network_http_request_description const*, char*, long)(this, service_type, request_description, buffer, buffer_length);
}

