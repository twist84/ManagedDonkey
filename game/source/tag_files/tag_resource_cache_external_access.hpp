#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_resource_cache_address_cache.hpp"
#include "tag_files/tag_resource_cache_control.hpp"
#include "tag_files/tag_resource_cache_inverse_page_table.hpp"
#include "tag_files/tag_resource_cache_paging.hpp"
#include "tag_files/tag_resource_cache_thread_lock.hpp"

class c_tag_resource_cache_new;
class c_tag_resource_cache_published_location_interface
{
public:
	c_tag_resource_cache_new* m_resource_cache;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_published_location_interface) == sizeof(c_tag_resource_cache_new*));

class c_tag_resource_cache_file_datum_handler;
class c_tag_resource_runtime_active_set;
class c_tag_resource_cache_file_prefetch_set;
class c_tag_resource_cache_new
{
public:
	c_tag_resource_cache_published_location_interface m_published_resource_interface;
	c_tag_resource_page_table_io_listener m_page_io_listener;
	c_tag_resource_page_table m_page_table;
	c_tag_resource_cache_controller m_cache_controller;
	c_tag_resource_address_cache m_address_cache;
	c_tag_resource_inverse_page_table m_inverse_page_table;
	c_tag_resource_cache_file_datum_handler* m_resource_datum_handler;
	c_tag_resource_runtime_active_set* m_runtime_active_set;
	c_tag_resource_cache_file_prefetch_set* m_cache_file_prefetch_set;

	byte __data134[0xC];

	c_tag_resource_cache_thread_lock_lock_freeish m_thread_access_lock;

	bool m_load_optional_resources_in_idle;
	bool m_can_eat_up_memory;
	bool m_local_resource_state_changed;
	bool m_thread_access_state_locked;
	bool m_last_resize_result;
	bool m_can_load_all_optional_resources;
	bool m_enable_incremental_publish;

	byte __data307[0x55];

	uns32 m_idle_signal_flags;
	uns32 m_do_not_allocate_beyond_size;

	byte __data364[0x1C];
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_new) == 0x380);
COMPILE_ASSERT(0x000 == OFFSETOF(c_tag_resource_cache_new, m_published_resource_interface));
COMPILE_ASSERT(0x004 == OFFSETOF(c_tag_resource_cache_new, m_page_io_listener));
COMPILE_ASSERT(0x008 == OFFSETOF(c_tag_resource_cache_new, m_page_table));
COMPILE_ASSERT(0x06C == OFFSETOF(c_tag_resource_cache_new, m_cache_controller));
COMPILE_ASSERT(0x0CC == OFFSETOF(c_tag_resource_cache_new, m_address_cache));
COMPILE_ASSERT(0x110 == OFFSETOF(c_tag_resource_cache_new, m_inverse_page_table));
COMPILE_ASSERT(0x128 == OFFSETOF(c_tag_resource_cache_new, m_resource_datum_handler));
COMPILE_ASSERT(0x12C == OFFSETOF(c_tag_resource_cache_new, m_runtime_active_set));
COMPILE_ASSERT(0x130 == OFFSETOF(c_tag_resource_cache_new, m_cache_file_prefetch_set));
COMPILE_ASSERT(0x134 == OFFSETOF(c_tag_resource_cache_new, __data134));
COMPILE_ASSERT(0x140 == OFFSETOF(c_tag_resource_cache_new, m_thread_access_lock));
COMPILE_ASSERT(0x300 == OFFSETOF(c_tag_resource_cache_new, m_load_optional_resources_in_idle));
COMPILE_ASSERT(0x301 == OFFSETOF(c_tag_resource_cache_new, m_can_eat_up_memory));
COMPILE_ASSERT(0x307 == OFFSETOF(c_tag_resource_cache_new, __data307));
COMPILE_ASSERT(0x35C == OFFSETOF(c_tag_resource_cache_new, m_idle_signal_flags));
COMPILE_ASSERT(0x360 == OFFSETOF(c_tag_resource_cache_new, m_do_not_allocate_beyond_size));
COMPILE_ASSERT(0x364 == OFFSETOF(c_tag_resource_cache_new, __data364));

