#pragma once

#include "cseries/cseries.hpp"

struct c_tag_resource_address_cache;
struct c_tag_resource_address_cache_control_interface
{
	c_tag_resource_address_cache* m_address_cache;
};
static_assert(sizeof(c_tag_resource_address_cache_control_interface) == 0x4);

struct s_tag_resource_location_handle_struct;
struct c_tag_resource_cache_file_access_cache;
struct c_tag_resource_address_cache
{
	struct s_published_resources_state
	{
		c_tag_resource_cache_file_access_cache* access_cache;
		c_wrapped_flags locked_resources;
		c_wrapped_flags published_required_resources;
		c_wrapped_flags locked_pages;
		bool active;
	};
	static_assert(sizeof(s_published_resources_state) == 0x20);

	struct s_cached_resource_state
	{
		long external_handle;
		void* required_address;
		void* optional_address;
		s_tag_resource_location_handle_struct* required_page_handle;
		s_tag_resource_location_handle_struct* optional_page_handle;
	};
	static_assert(sizeof(s_cached_resource_state) == 0x14);

	c_tag_resource_address_cache_control_interface m_control_interface;
	c_wrapped_array<s_cached_resource_state*> m_cached_access_datums;
	c_wrapped_array<uint64> m_cached_resource_snapshots;
	c_wrapped_flags m_ready_resource_addresses;
	c_wrapped_flags m_required_resource_addresses;
	c_wrapped_flags m_demanded_external_resources;
	c_wrapped_flags m_demanded_pages;
	c_wrapped_array<s_published_resources_state*> m_published_resources_states;
	void* m_allocation_start;
	c_allocation_base* m_allocator;
};
static_assert(sizeof(c_tag_resource_address_cache) == 0x44);

