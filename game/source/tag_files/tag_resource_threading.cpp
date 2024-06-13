#include "tag_files/tag_resource_threading.hpp"

#include "rasterizer/rasterizer_main.hpp"
#include "tag_files/tag_resource_cache_access_cache.hpp"

bool __cdecl c_tag_resource_thread_access::resource_available(long resource_handle)
{
	c_tag_resource_cache_file_access_cache* access_cache = get_current_thread_access_cache();
	return access_cache && access_cache->cached_resource_available(resource_handle);
}

void* __cdecl c_tag_resource_thread_access::get_resource_data(long resource_handle)
{
	c_tag_resource_cache_file_access_cache* resource_access_cache = get_current_thread_access_cache();
	ASSERT(resource_access_cache);

	return resource_access_cache->get_cached_resource_data(resource_handle);
}

c_tag_resource_cache_file_access_cache* __cdecl c_tag_resource_thread_access::get_current_thread_access_cache()
{
	// I don't want to synchronize access to resources except for the main and render threads!
	long thread_index = get_current_thread_index();
	ASSERT(m_per_thread_acquired_access_cache[thread_index]);

	return m_per_thread_access_cache[thread_index];
}

void c_thread_safeish_tag_resource_cache::load_pending_data_only_blocking(c_io_result* io_result)
{
	DECLFUNC(0x006F1690, void, __thiscall, c_thread_safeish_tag_resource_cache*, c_io_result*)(this, io_result);
}

void c_thread_safeish_tag_resource_cache::load_required_data_only_blocking(c_io_result* io_result)
{
	DECLFUNC(0x006F16A0, void, __thiscall, c_thread_safeish_tag_resource_cache*, c_io_result*)(this, io_result);
}

