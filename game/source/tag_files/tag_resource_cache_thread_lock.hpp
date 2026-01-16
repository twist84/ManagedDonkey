#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/event_queue.hpp"
#include "tag_files/tag_resource_cache_access_cache.hpp"

struct s_access_cache_available_event :
	s_synchronized_list_entry
{
	byte __data0[0xC];
};
COMPILE_ASSERT(sizeof(s_access_cache_available_event) == 0x10);
//COMPILE_ASSERT(sizeof(t_event_queue<s_access_cache_available_event, 3>) == 0x50);

class c_tag_resource_cache_dpc_event
{
public:
	c_interlocked_int64 m_dpc_event_plus_flag;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_dpc_event) == 0x8);

class c_tag_resource_cache_thread_lock_lock_freeish
{
public:
	class c_synchronized_access_cache :
		public c_tag_resource_cache_file_access_cache
	{
	public:
		c_tag_resource_cache_dpc_event m_dpc_event;
		c_interlocked_long m_reference_count;
		int32 : 32;
	};
	COMPILE_ASSERT(sizeof(c_synchronized_access_cache) == 0x38);
	COMPILE_ASSERT(0x28 == OFFSETOF(c_synchronized_access_cache, m_dpc_event));
	COMPILE_ASSERT(0x30 == OFFSETOF(c_synchronized_access_cache, m_reference_count));

	void* volatile m_active_access_cache;
	t_event_queue<s_access_cache_available_event, 3> m_available_cache_list;
	t_event_queue<s_access_cache_available_event, 3> m_released_cache_list;
	c_static_array<c_synchronized_access_cache, 3> m_access_cache_storage; // c_tag_resource_cache_thread_lock_lock_freeish::c_synchronized_access_cache
	c_reference_count<int32> m_published_cache_count;
	c_read_write_lock m_read_write_lock;
	c_interlocked_int64 m_dpc_event_plus_flag;
	bool m_blocking_update;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_thread_lock_lock_freeish) == 0x1C0);
COMPILE_ASSERT(0x020 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_available_cache_list));
COMPILE_ASSERT(0x080 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_released_cache_list));
COMPILE_ASSERT(0x0E0 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_access_cache_storage));
COMPILE_ASSERT(0x188 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_published_cache_count));
COMPILE_ASSERT(0x18C == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_read_write_lock));
COMPILE_ASSERT(0x198 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_dpc_event_plus_flag));
COMPILE_ASSERT(0x1A0 == OFFSETOF(c_tag_resource_cache_thread_lock_lock_freeish, m_blocking_update));

