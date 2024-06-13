#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"
#include "multithreading/threads.hpp"
#include "tag_files/tag_resource_cache_external_access.hpp"

struct c_tag_resource_cache_file_access_cache;
struct c_tag_resource_cache_thread_lock_lock_freeish;
struct c_tag_resource_thread_access
{
public:
	bool __cdecl resource_available(long resource_handle);
	void* __cdecl get_resource_data(long resource_handle);

private:
	c_tag_resource_cache_file_access_cache* __cdecl get_current_thread_access_cache();

protected:
	long m_per_thread_acquired_access_cache_storage[k_registered_thread_count];
	c_tag_resource_cache_file_access_cache* m_per_thread_access_cache_storage[k_registered_thread_count];
	c_wrapped_array<long> m_per_thread_acquired_access_cache;
	c_wrapped_array<c_tag_resource_cache_file_access_cache*> m_per_thread_access_cache;
	c_read_write_lock m_read_write_lock;
	c_tag_resource_cache_thread_lock_lock_freeish* m_resource_thread_access_lock;
	byte __data[0x10];
};
static_assert(sizeof(c_tag_resource_thread_access) == 0x80);

struct c_io_result;
struct c_thread_safeish_tag_resource_cache
{
public:
	void* __cdecl get_resource_data(long resource_handle)
	{
		if (!m_resource_thread_access.resource_available(resource_handle))
			report_unavailable_resource(resource_handle);

		return get_thread_access()->get_resource_data(resource_handle);
	}

	//private:
	void __cdecl report_unavailable_resource(long resource_handle)
	{
	}

	c_tag_resource_thread_access* __cdecl get_thread_access()
	{
		return &m_resource_thread_access;
	}

	void load_pending_data_only_blocking(c_io_result* io_result);
	void load_required_data_only_blocking(c_io_result* io_result);

protected:
	c_tag_resource_cache_new m_resource_cache_new;
	c_tag_resource_thread_access m_resource_thread_access;
};
static_assert(sizeof(c_thread_safeish_tag_resource_cache) == 0x400);

