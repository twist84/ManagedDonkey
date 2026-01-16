#pragma once

#include "cseries/cseries.hpp"

class c_ring_buffer
{
public:
	int32 ring_size;
	int32 head_index;
	int32 element_count;
};
COMPILE_ASSERT(sizeof(c_ring_buffer) == 0xC);

template<typename t_type, int32 k_maximum_count>
struct t_static_ring_buffer :
	public c_ring_buffer
{
	t_type m_data_members[k_maximum_count];
};
COMPILE_ASSERT(sizeof(t_static_ring_buffer<int32, 16>) == 0x4C);

