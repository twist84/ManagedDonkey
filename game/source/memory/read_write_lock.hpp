#pragma once

#include "cseries/cseries.hpp"

struct c_read_write_lock
{
public:
	c_read_write_lock();
	~c_read_write_lock();

	void __cdecl read_lock();
	void __cdecl read_unlock();
	void __cdecl setup(long critcal_section_index, long semaphore_index);
	void __cdecl write_lock();
	void __cdecl write_unlock();

protected:
	long m_critcal_section_index;
	long m_semaphore_index;
	long m_signal_count;
};
static_assert(sizeof(c_read_write_lock) == 0xC);

