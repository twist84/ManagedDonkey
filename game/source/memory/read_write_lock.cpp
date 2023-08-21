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
	DECLFUNC(0x006EF4D0, void, __thiscall, c_read_write_lock*)(this);
}

void __cdecl c_read_write_lock::read_unlock()
{
	DECLFUNC(0x006EF4E0, void, __thiscall, c_read_write_lock*)(this);
}


void __cdecl c_read_write_lock::setup(long critcal_section_index, long semaphore_index)
{
	DECLFUNC(0x006EF4F0, void, __thiscall, c_read_write_lock*, long, long)(this, critcal_section_index, semaphore_index);
}

void __cdecl c_read_write_lock::write_lock()
{
	DECLFUNC(0x006EF530, void, __thiscall, c_read_write_lock*)(this);
}

void __cdecl c_read_write_lock::write_unlock()
{
	DECLFUNC(0x006EF570, void, __thiscall, c_read_write_lock*)(this);
}

