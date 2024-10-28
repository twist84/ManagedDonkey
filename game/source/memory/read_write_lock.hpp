#pragma once

#include "cseries/cseries.hpp"

struct c_read_write_lock
{
public:
	c_read_write_lock();
	~c_read_write_lock();

	void read_lock();
	void read_unlock();
	void setup(long critcal_section_index, long semaphore_index);
	void write_lock();
	void write_unlock();

protected:
	long m_critcal_section_index;
	long m_semaphore_index;
	long m_signal_count;
};
static_assert(sizeof(c_read_write_lock) == 0xC);

