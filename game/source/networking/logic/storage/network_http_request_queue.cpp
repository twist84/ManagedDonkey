#include "networking/logic/storage/network_http_request_queue.hpp"

#include "config/version.hpp"
#include "cseries/headers.hpp"
#include "memory/module.hpp"

#define STORAGE_URL_PREFIX "donkey/"

HOOK_DECLARE(0x004A3630, network_http_request_queue_initialize);
HOOK_DECLARE(0x004A3690, network_http_request_queue_update);
HOOK_DECLARE(0x004A36A0, network_storage_set_storage_subdirectory);
HOOK_DECLARE(0x004A36C0, network_storage_set_storage_user);

REFERENCE_DECLARE_ARRAY(0x0224A4C8, c_network_http_request_queue, g_network_http_request_queue, k_network_http_request_queue_type_count);
REFERENCE_DECLARE(0x02269678, c_static_string<256>, g_storage_url_subdirectory);

c_network_http_request_description::c_network_http_request_description(const char* url, e_http_request_type request_type)
{
	DECLFUNC(0x004A2CE0, void, __thiscall, c_network_http_request_description*, const char*, e_http_request_type)(this, url, request_type);
}

c_network_http_request_description::c_network_http_request_description(const char* url, e_http_request_type request_type, const s_network_http_request_hash* request_hash)
{
	DECLFUNC(0x004A2D20, void, __thiscall, c_network_http_request_description*, const char*, e_http_request_type, const s_network_http_request_hash*)(this, url, request_type, request_hash);
}

c_network_http_request_description::c_network_http_request_description()
{
	DECLFUNC(0x004A2D90, void, __thiscall, c_network_http_request_description*)(this);
}

c_network_http_request_item::c_network_http_request_item(int32 request_cookie, c_network_http_request_queue* request_queue, e_online_lsp_service_type service_type, const c_network_http_request_description* request_description)
{
	DECLFUNC(0x004A2DC0, void, __thiscall, c_network_http_request_item*, int32, c_network_http_request_queue*, e_online_lsp_service_type, const c_network_http_request_description*)(this, request_cookie, request_queue, service_type, request_description);
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

void c_network_http_request_queue::cancel_request(int32 request_cookie)
{
	INVOKE_CLASS_MEMBER(0x004A3050, c_network_http_request_queue, cancel_request, request_cookie);
}

//.text:004A3140 ; void c_network_http_request_item::failed(bool failed)
//.text:004A3170 ; c_network_http_request_item  c_network_http_request_queue::find_http_request_item_from_cookie(int32 request_cookie)
//.text:004A31C0 ; int32 c_network_http_request_queue::find_http_request_item_index(const c_network_http_request_item* request_item) const 

c_network_http_request_queue* c_network_http_request_queue::get(e_network_http_request_queue_type queue_type)
{
	//return INVOKE(0x004A3220, c_network_http_request_queue::get, queue_type);

	ASSERT(VALID_INDEX(queue_type, k_network_http_request_queue_type_count));

	return &g_network_http_request_queue[queue_type];
}

//.text:004A3240 ; const c_network_http_request_description* c_network_http_request_item::get_description() const
//.text:004A3250 ; char* c_network_http_request_item::get_fill_buffer() const
//.text:004A3260 ; int32 c_network_http_request_item::get_fill_buffer_length() const
//.text:004A3270 ; int32 c_network_http_request_item::get_fill_buffer_progress() const
//.text:004A3280 ; const char* c_network_http_request_description::get_headers() const
//.text:004A3290 ; const c_http_post_source* c_network_http_request_description::get_post_source() const
//.text:004A32A0 ; 
//.text:004A32B0 ; 
//.text:004A32C0 ; e_http_request_type c_network_http_request_description::get_request_type() const
//.text:004A32D0 ; int32 c_network_http_request_item::get_retries_remaining() const
//.text:004A32E0 ; e_online_lsp_service_type c_network_http_request_item::get_service_type() const
//.text:004A32F0 ; 

bool c_network_http_request_queue::has_file_changed(const c_network_http_request_description* request_description) const
{
	return DECLFUNC(0x004A3300, bool, __cdecl, const c_network_http_request_description*)(request_description);

	//return network_http_request_cache_has_file_changed(request_description);
}

e_network_http_request_result c_network_http_request_queue::is_fill_buffer_complete(int32 request_cookie, int32* bytes_read, s_network_http_request_hash* request_hash, e_network_http_request_queue_failure_reason* out_failure_reason)
{
	return INVOKE_CLASS_MEMBER(0x004A3350, c_network_http_request_queue, is_fill_buffer_complete, request_cookie, bytes_read, request_hash, out_failure_reason);
}

void __cdecl make_url(const c_static_string<256>* url, c_static_string<256>* out_url)
{
	INVOKE(0x004A34B0, make_url, url, out_url);
}

void __cdecl network_http_request_queue_dispose()
{
	//INVOKE(0x004A3620, network_http_request_queue_dispose);
}

void __cdecl network_http_request_queue_initialize()
{
	//INVOKE(0x004A3630, network_http_request_queue_initialize);

	if (k_tracked_build)
	{
		int32 build_number = version_get_build_number();
		g_storage_url_subdirectory.print(STORAGE_URL_PREFIX"tracked%c%05u", '/', build_number);
	}
	else
	{
		network_storage_set_storage_user(g_username);
	}
}

void __cdecl network_http_request_queue_update()
{
	//INVOKE(0x004A3690, network_http_request_queue_update);
}

void network_storage_set_storage_subdirectory(const char* url)
{
	//INVOKE(0x004A36A0, network_storage_set_storage_subdirectory, url);

	g_storage_url_subdirectory.set(url);
}

void network_storage_set_storage_user(const char* user)
{
	//INVOKE(0x004A36C0, network_storage_set_storage_user, user);
	g_storage_url_subdirectory.print(STORAGE_URL_PREFIX"untracked%c%s%c%s", '/', user, '/', "main");
}

e_network_http_request_result c_network_http_request_queue::read_bytes(int32 request_cookie, char* buffer, int32 buffer_length, int32* out_bytes_read, e_network_http_request_queue_failure_reason* out_failure_reason)
{
	return INVOKE_CLASS_MEMBER(0x004A3730, c_network_http_request_queue, read_bytes, request_cookie, buffer, buffer_length, out_bytes_read, out_failure_reason);
}

//.text:004A37E0 ; e_network_http_request_result c_network_http_request_queue::read_bytes_internal(c_network_http_request_item* request_item, char* buffer, int32 buffer_length, int32* out_bytes_read, bool* out_item_was_removed, e_network_http_request_queue_failure_reason* out_failure_reason)
//.text:004A3AD0 ; void c_network_http_request_queue::remove_first_item_from_queue()
//.text:004A3B00 ; void c_network_http_request_queue::request_failed(c_network_http_request_item* request_item, bool, bool )
//.text:004A3B60 ; bool c_network_http_request_queue::run_request(char* buffer, int32 buffer_length, int32* out_bytes_read, bool* upload_complete, int32* http_response_code)
//.text:004A3C00 ; void c_network_http_request_item::set_client_usage_type(e_client_usage_type client_usage_type)
//.text:004A3C10 ; void c_network_http_request_item::set_fill_buffer(char* fill_buffer)
//.text:004A3C20 ; void c_network_http_request_item::set_fill_buffer_length(int32 fill_buffer_length)
//.text:004A3C30 ; void c_network_http_request_item::set_fill_buffer_progress(int32 fill_buffer_progress)
//.text:004A3C40 ; void c_network_http_request_item::set_request_state(e_network_http_request_state request_state)

int32 c_network_http_request_queue::start_request(e_online_lsp_service_type service_type, const c_network_http_request_description* request_description)
{
	return INVOKE_CLASS_MEMBER(0x004A3C50, c_network_http_request_queue, start_request, service_type, request_description);
}

int32 c_network_http_request_queue::start_request_into_buffer(e_online_lsp_service_type service_type, const c_network_http_request_description* request_description, char* buffer, int32 buffer_length)
{
	return INVOKE_CLASS_MEMBER(0x004A3D80, c_network_http_request_queue, start_request_into_buffer, service_type, request_description, buffer, buffer_length);
}

