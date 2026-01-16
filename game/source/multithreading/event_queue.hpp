#pragma once

#include "multithreading/primitives/synchronized_list_windows.hpp"

template<typename t_type, int32 k_count>
struct __declspec(align(32)) t_event_queue
{
	t_type m_entries[k_count];
	s_synchronized_list_header m_pending_list_header;
	s_synchronized_list_header m_free_list_header;
	bool m_initialized;
	int32 m_entry_limit;
	const char* m_name;
};
COMPILE_ASSERT(sizeof(t_event_queue<int32, 6>) == 0x40);

