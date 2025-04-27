#include "memory/read_write_lock.hpp"

c_read_write_lock::c_read_write_lock() :
	m_write_lock(k_invalid_critical_section),
	m_read_lock(k_invalid_semaphore),
	m_max_readers(NONE)
{
}

c_read_write_lock::~c_read_write_lock()
{
	m_write_lock = k_invalid_critical_section;
	m_read_lock = k_invalid_semaphore;
	m_max_readers = NONE;
}

void c_read_write_lock::read_lock()
{
	INVOKE_CLASS_MEMBER(0x006EF4D0, c_read_write_lock, read_lock);
}

void c_read_write_lock::read_unlock()
{
	INVOKE_CLASS_MEMBER(0x006EF4E0, c_read_write_lock, read_unlock);
}

void c_read_write_lock::setup(int32 critcal_section_index, int32 semaphore_index)
{
	INVOKE_CLASS_MEMBER(0x006EF4F0, c_read_write_lock, setup, critcal_section_index, semaphore_index);
}

void c_read_write_lock::write_lock()
{
	INVOKE_CLASS_MEMBER(0x006EF530, c_read_write_lock, write_lock);
}

void c_read_write_lock::write_unlock()
{
	INVOKE_CLASS_MEMBER(0x006EF570, c_read_write_lock, write_unlock);
}

