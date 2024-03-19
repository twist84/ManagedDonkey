#pragma once

#include "cseries/cseries.hpp"

struct c_tag_resource_address_cache;
struct c_tag_resource_address_cache_control_interface
{
	c_tag_resource_address_cache* m_address_cache;
};
static_assert(sizeof(c_tag_resource_address_cache_control_interface) == 0x4);

struct c_tag_resource_cache_file_access_cache;
struct c_tag_resource_address_cache
{
	struct s_published_resources_state
	{
		c_tag_resource_cache_file_access_cache* m_access_cache;

		c_wrapped_flags __flags4;
		c_wrapped_flags __flagsC;
		c_wrapped_flags __flags14;

		bool __unknown1C;
	};
	static_assert(sizeof(s_published_resources_state) == 0x20);

	struct s_cached_resource_state
	{
		dword external_handle;
		void* required_address;
		void* optional_address;

		byte __dataC[0x8]; // ?
	};
	static_assert(sizeof(s_cached_resource_state) == 0x14);

	c_tag_resource_address_cache_control_interface m_control_interface;
	c_wrapped_array<s_cached_resource_state*> m_cached_access_datums;

	byte __dataC[0x8]; // ?

	c_wrapped_flags m_available_resource_flags;
	c_wrapped_flags __flags14;
	c_wrapped_flags m_demanded_flags;

	byte __data2C[0x8]; // ?

	c_wrapped_array<s_published_resources_state*> m_published_resources_states;
	void* m_allocation;
	c_allocation_base* m_allocator;
};
static_assert(sizeof(c_tag_resource_address_cache) == 0x44);

