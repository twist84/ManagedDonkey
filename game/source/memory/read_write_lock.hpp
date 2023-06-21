#pragma once

#include "cseries/cseries.hpp"

struct c_read_write_lock
{
	dword m_critical_section_id;
	long m_semaphore_id;
	long m_max_signal_count;
};
static_assert(sizeof(c_read_write_lock) == 0xC);

