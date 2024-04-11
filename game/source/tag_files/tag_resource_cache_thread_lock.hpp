#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"
#include "multithreading/event_queue.hpp"
#include "tag_files/tag_resource_cache_access_cache.hpp"

struct s_access_cache_available_event :
	s_synchronized_list_entry
{
	byte __data0[0xC];
};
static_assert(sizeof(s_access_cache_available_event) == 0x10);

struct c_tag_resource_cache_thread_lock_lock_freeish
{
	byte __data0[0x20];

	t_event_queue<s_access_cache_available_event, 3> __unknown20;
	byte __data68[0x18];
	t_event_queue<s_access_cache_available_event, 3> __unknown80;
	byte __dataC8[0x18];

	c_static_array<c_tag_resource_cache_file_access_cache, 3> m_access_cache_storage; // c_tag_resource_cache_thread_lock_lock_freeish::c_synchronized_access_cache
	c_reference_count<long> m_published_cache_count;
	c_read_write_lock m_read_write_lock;
	byte __data198[0x8];
	bool m_blocking_update;
	byte __data1A1[0x1F];
	bool __unknown1C0;
	bool __unknown1C1;
	bool __unknown1C2;
	bool m_thread_access_state_locked;
	bool __unknown1C4;
	bool m_can_load_all_desired_resources;
	bool __unknown1C6;
	bool __unknown1C7;
	bool __unknown1C8;
	bool __unknown1C9;
	bool __unknown1CA;
	bool __unknown1CB;
	bool __unknown1CC;
	dword __unknown1D0;
	byte __data1CC[0x6C];
};
static_assert(sizeof(c_tag_resource_cache_thread_lock_lock_freeish) == 0x240);

