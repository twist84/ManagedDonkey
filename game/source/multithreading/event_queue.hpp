#pragma once

#include "multithreading/primitives/synchronized_list_windows.hpp"

template<typename t_type, long k_count>
struct t_event_queue
{
	t_type m_events[k_count];
	s_synchronized_list_header m_header0;
	s_synchronized_list_header m_header1;
	bool m_initialized;
	long m_limit_count;
};
static_assert(sizeof(t_event_queue<long, 6>) == 0x30);

