#include "cseries/async_xoverlapped.hpp"

c_overlapped_task::c_overlapped_task(char const* file, long line) :
	m_task_flags(0),
	m_task_state(0)
{
	DECLFUNC(0x005A8C80, void, __thiscall, c_overlapped_task const*, char const*, long)(this, file, line);
	
	set_file(file);
	set_line(line);
}

bool c_overlapped_task::busy() const
{
	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}