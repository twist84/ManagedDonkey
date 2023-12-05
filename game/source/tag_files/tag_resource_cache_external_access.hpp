#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_resource_cache_address_cache.hpp"
#include "tag_files/tag_resource_cache_control.hpp"
#include "tag_files/tag_resource_cache_inverse_page_table.hpp"
#include "tag_files/tag_resource_cache_paging.hpp"
#include "tag_files/tag_resource_cache_thread_lock.hpp"

struct c_tag_resource_runtime_active_set;
struct c_tag_resource_cache_file_prefetch_set;
struct c_tag_resource_cache_new
{
	void* __unknown0;

	c_tag_resource_page_table_io_listener m_page_io_listener;
	c_tag_resource_page_table m_page_table;
	c_tag_resource_cache_controller m_cache_controller;
	c_tag_resource_address_cache m_address_cache;
	c_tag_resource_inverse_page_table m_inverse_page_table;

	// vtable?
	void* __unknown128;

	c_tag_resource_runtime_active_set* m_runtime_active_set;
	c_tag_resource_cache_file_prefetch_set* m_cache_file_prefetch_set;

	byte __data134[0xC];

	c_tag_resource_cache_thread_lock_lock_freeish m_thread_access_lock;
};
static_assert(sizeof(c_tag_resource_cache_new) == 0x380);

