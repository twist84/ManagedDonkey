#pragma once

#include "multithreading/synchronized_value.hpp"

template<typename t_type, long k_maximum_count, long k_storage_count = ((k_maximum_count - 1) >> 5) + 1>
struct t_message_queue
{
	c_synchronized_long m_count;

	//c_synchronized_array<k_maximum_count >> 5> m_added_entries;
	c_synchronized_array<k_storage_count> m_added_entries;

	t_type m_entries[k_maximum_count];
};
static_assert(sizeof(t_message_queue<long, 1024>) == 0x1084);

