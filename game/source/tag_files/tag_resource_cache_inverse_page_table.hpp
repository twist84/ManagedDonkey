#pragma once

#include "cseries/cseries.hpp"

struct s_data_array;
struct c_tag_resource_inverse_page_table
{
	struct s_page_resource_reference_range :
		s_datum_header
	{
		uint16 first_resource_handle_index;
		uint16 used_resource_handle_count;
		uint16 resource_handle_count;
	};
	static_assert(sizeof(s_page_resource_reference_range) == 0x8);

	c_allocation_base* m_allocator;
	void* m_allocation;
	c_wrapped_array<int32*> m_resource_handle_storage;
	s_data_array* m_page_entries;
	uint16 flags;
	uint16 m_next_resource_handle_index;
};
static_assert(sizeof(c_tag_resource_inverse_page_table) == 0x18);

