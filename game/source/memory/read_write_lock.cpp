#include "memory/read_write_lock.hpp"

c_read_write_lock::c_read_write_lock() :
	m_critcal_section_index(NONE),
	m_semaphore_index(NONE),
	m_signal_count(NONE)
{
}

c_read_write_lock::~c_read_write_lock()
{
	m_critcal_section_index = NONE;
	m_semaphore_index = NONE;
	m_signal_count = NONE;
}

void __cdecl c_read_write_lock::read_lock()
{
	INVOKE_CLASS_MEMBER(0x006EF4D0, c_read_write_lock::read_lock);
}

void __cdecl c_read_write_lock::read_unlock()
{
	INVOKE_CLASS_MEMBER(0x006EF4E0, c_read_write_lock::read_unlock);
}


void __cdecl c_read_write_lock::setup(long critcal_section_index, long semaphore_index)
{
	INVOKE_CLASS_MEMBER(0x006EF4F0, c_read_write_lock::setup, critcal_section_index, semaphore_index);
}

void __cdecl c_read_write_lock::write_lock()
{
	INVOKE_CLASS_MEMBER(0x006EF530, c_read_write_lock::write_lock);
}

void __cdecl c_read_write_lock::write_unlock()
{
	INVOKE_CLASS_MEMBER(0x006EF570, c_read_write_lock::write_unlock);
}

