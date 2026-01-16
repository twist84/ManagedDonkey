#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"
#include "multithreading/threads.hpp"
#include "tag_files/tag_resource_cache_external_access.hpp"

class c_tag_resource_cache_file_access_cache;
class c_tag_resource_cache_thread_lock_lock_freeish;
class c_tag_resource_thread_access
{
public:
	bool resource_available(int32 resource_handle);
	void* get_resource_data(int32 resource_handle);

private:
	c_tag_resource_cache_file_access_cache* get_current_thread_access_cache();

protected:
	int32 m_per_thread_acquired_access_cache_storage[k_registered_thread_count];
	c_tag_resource_cache_file_access_cache* m_per_thread_access_cache_storage[k_registered_thread_count];
	c_wrapped_array<int32> m_per_thread_acquired_access_cache;
	c_wrapped_array<c_tag_resource_cache_file_access_cache*> m_per_thread_access_cache;
	c_read_write_lock m_read_write_lock;
	c_tag_resource_cache_thread_lock_lock_freeish* m_resource_thread_access_lock;
	byte __data[0x10];
};
COMPILE_ASSERT(sizeof(c_tag_resource_thread_access) == 0x80);

class c_io_result;
class c_thread_safeish_tag_resource_cache :
	public c_tag_resource_cache_new//, c_tag_resource_thread_access
{
public:
	void* get_resource_data(int32 resource_handle)
	{
		if (!m_resource_thread_access.resource_available(resource_handle))
			report_unavailable_resource(resource_handle);

		return get_thread_access()->get_resource_data(resource_handle);
	}

	//private:
	void report_unavailable_resource(int32 resource_handle)
	{
	}

	c_tag_resource_thread_access* get_thread_access()
	{
		return &m_resource_thread_access;
	}

	void load_pending_data_only_blocking(c_io_result* io_result);
	void load_required_data_only_blocking(c_io_result* io_result);

protected:
	c_tag_resource_thread_access m_resource_thread_access;
};
COMPILE_ASSERT(sizeof(c_thread_safeish_tag_resource_cache) == 0x400);

