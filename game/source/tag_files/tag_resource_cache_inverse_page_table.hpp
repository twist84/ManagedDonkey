#pragma once

#include "cseries/cseries.hpp"

struct s_data_array;
class c_tag_resource_inverse_page_table
{
public:
	struct s_page_resource_reference_range :
		s_datum_header
	{
		uns16 first_resource_handle_index;
		uns16 used_resource_handle_count;
		uns16 resource_handle_count;
	};
	COMPILE_ASSERT(sizeof(s_page_resource_reference_range) == 0x8);

	c_allocation_base* m_allocator;
	void* m_allocation;
	c_wrapped_array<int32*> m_resource_handle_storage;
	s_data_array* m_page_entries;
	uns16 flags;
	uns16 m_next_resource_handle_index;
};
COMPILE_ASSERT(sizeof(c_tag_resource_inverse_page_table) == 0x18);

