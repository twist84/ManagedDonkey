#pragma once

#include "multithreading/synchronized_value.hpp"

template<typename t_type, long k_maximum_count, long k_storage_count = ((k_maximum_count - 1) >> 5) + 1>
struct t_message_queue
{
	static long const k_max_per_frame = k_maximum_count;

	c_synchronized_long m_message_count;

	//c_synchronized_array<k_max_per_frame >> 5> m_messages_added;
	c_synchronized_array<k_storage_count> m_messages_added;

	t_type m_messages[k_max_per_frame];
};
static_assert(sizeof(t_message_queue<long, 1024>) == 0x1084);

