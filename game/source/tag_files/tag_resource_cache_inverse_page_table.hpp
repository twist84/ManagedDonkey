#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_tag_resource_inverse_page_table
{
	struct s_page_resource_reference_range : s_datum_header
	{
		word __unknown2;
		word __unknown4;
		word __unknown6;
	};
	static_assert(sizeof(s_page_resource_reference_range) == 0x8);

	c_allocation_base* m_allocator;
	void* m_allocation;
	dword __unknown8;
	dword __unknownC;
	c_smart_data_array<s_page_resource_reference_range> m_page_entries;
	byte_flags flags;
	word __unknown16;
};
static_assert(sizeof(c_tag_resource_inverse_page_table) == 0x18);

