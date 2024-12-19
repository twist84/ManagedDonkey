#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_resource_cache_address_cache.hpp"
#include "tag_files/tag_resource_cache_control.hpp"
#include "tag_files/tag_resource_cache_inverse_page_table.hpp"
#include "tag_files/tag_resource_cache_paging.hpp"
#include "tag_files/tag_resource_cache_thread_lock.hpp"

struct c_tag_resource_cache_new;
struct c_tag_resource_cache_published_location_interface
{
	c_tag_resource_cache_new* m_resource_cache;
};
static_assert(sizeof(c_tag_resource_cache_published_location_interface) == sizeof(c_tag_resource_cache_new*));

struct c_tag_resource_cache_file_datum_handler;
struct c_tag_resource_runtime_active_set;
struct c_tag_resource_cache_file_prefetch_set;
struct c_tag_resource_cache_new
{
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

	dword m_idle_signal_flags;
	dword m_do_not_allocate_beyond_size;

	byte __data364[0x1C];
};
static_assert(sizeof(c_tag_resource_cache_new) == 0x380);
static_assert(0x000 == OFFSETOF(c_tag_resource_cache_new, m_published_resource_interface));
static_assert(0x004 == OFFSETOF(c_tag_resource_cache_new, m_page_io_listener));
static_assert(0x008 == OFFSETOF(c_tag_resource_cache_new, m_page_table));
static_assert(0x06C == OFFSETOF(c_tag_resource_cache_new, m_cache_controller));
static_assert(0x0CC == OFFSETOF(c_tag_resource_cache_new, m_address_cache));
static_assert(0x110 == OFFSETOF(c_tag_resource_cache_new, m_inverse_page_table));
static_assert(0x128 == OFFSETOF(c_tag_resource_cache_new, m_resource_datum_handler));
static_assert(0x12C == OFFSETOF(c_tag_resource_cache_new, m_runtime_active_set));
static_assert(0x130 == OFFSETOF(c_tag_resource_cache_new, m_cache_file_prefetch_set));
static_assert(0x134 == OFFSETOF(c_tag_resource_cache_new, __data134));
static_assert(0x140 == OFFSETOF(c_tag_resource_cache_new, m_thread_access_lock));
static_assert(0x300 == OFFSETOF(c_tag_resource_cache_new, m_load_optional_resources_in_idle));
static_assert(0x301 == OFFSETOF(c_tag_resource_cache_new, m_can_eat_up_memory));
static_assert(0x307 == OFFSETOF(c_tag_resource_cache_new, __data307));
static_assert(0x35C == OFFSETOF(c_tag_resource_cache_new, m_idle_signal_flags));
static_assert(0x360 == OFFSETOF(c_tag_resource_cache_new, m_do_not_allocate_beyond_size));
static_assert(0x364 == OFFSETOF(c_tag_resource_cache_new, __data364));

