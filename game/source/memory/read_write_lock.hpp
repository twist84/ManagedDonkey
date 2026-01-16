#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronization.hpp"

class c_read_write_lock
{
public:
	c_read_write_lock();
	~c_read_write_lock();

	void read_lock();
	void read_unlock();
	void setup(int32 critcal_section_index, int32 semaphore_index);
	void write_lock();
	void write_unlock();

protected:
	e_critical_sections m_write_lock;
	e_synchronization_semaphore m_read_lock;
	int32 m_max_readers;
};
COMPILE_ASSERT(sizeof(c_read_write_lock) == 0xC);

