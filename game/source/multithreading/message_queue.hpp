#pragma once

#include "multithreading/synchronized_value.hpp"

template<typename t_type, int32 k_maximum_count>
struct t_message_queue
{
	static int32 const k_max_per_frame = k_maximum_count;

	c_synchronized_long m_message_count;
	c_synchronized_bitvector<k_max_per_frame> m_bitflags;
	t_type m_message[k_max_per_frame];
};
COMPILE_ASSERT(sizeof(t_message_queue<int32, 1024>) == 0x1084);

