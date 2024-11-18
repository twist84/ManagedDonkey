#pragma once

#include "multithreading/primitives/synchronized_list_windows.hpp"

template<typename t_type, long k_count>
struct __declspec(align(32)) t_event_queue
{
	t_type m_entries[k_count];
	s_synchronized_list_header m_pending_list_header;
	s_synchronized_list_header m_free_list_header;
	bool m_initialized;
	long m_entry_limit;
	char const* m_name;
};
static_assert(sizeof(t_event_queue<long, 6>) == 0x40);

