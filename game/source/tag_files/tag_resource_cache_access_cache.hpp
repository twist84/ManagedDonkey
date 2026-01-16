#pragma once

#include "cseries/cseries.hpp"

struct s_tag_resource_access_datum
{
	int32 resource_handle;
	void* resource_data;
};
COMPILE_ASSERT(sizeof(s_tag_resource_access_datum) == 0x8);

enum e_tag_resource_access_state_bit
{
	_tag_resource_access_state_bit0 = 0,

	k_tag_resource_access_state_bits,
};

class c_tag_resource_cache_file_access_cache
{
public:
	bool __cdecl cached_resource_available(int32 resource_handle)
	{
		if (resource_handle == NONE)
			return false;

		s_tag_resource_access_datum* cached_resource = m_cached_resources[resource_handle];
		if (!cached_resource)
			return false;

		//ASSERT(cached_resource->resource_handle == resource_handle || cached_resource->resource_handle == NONE);

		return cached_resource->resource_handle != NONE && cached_resource->resource_data;
	}

	void* __cdecl get_cached_resource_data(int32 resource_handle)
	{
		ASSERT(resource_handle != NONE);

		s_tag_resource_access_datum* cached_resource = m_cached_resources[DATUM_INDEX_TO_ABSOLUTE_INDEX(resource_handle)];
		//ASSERT(cached_resource->resource_handle == resource_handle);
		//ASSERT(cached_resource->resource_data);

		return cached_resource;
	}

	c_wrapped_array<uns64> m_cached_resource_snapshots;
	c_wrapped_array<s_tag_resource_access_datum*> m_cached_resources;
	c_wrapped_array<uns32*> m_cached_resource_handles;
	c_wrapped_array<c_flags<e_tag_resource_access_state_bit, uns32, k_tag_resource_access_state_bits>*> m_resource_access_flags;
	c_allocation_base* m_allocator;
	void* m_allocation_base;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_file_access_cache) == 0x28);

