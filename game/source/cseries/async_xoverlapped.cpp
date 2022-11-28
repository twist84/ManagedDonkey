#include "cseries/async_xoverlapped.hpp"

c_overlapped_task* c_overlapped_task::constructor(char const* file, long line)
{
	return DECLFUNC(0x005A8C80, c_overlapped_task*, __thiscall, c_overlapped_task const*, char const*, long)(this, file, line);
}

bool c_overlapped_task::busy() const
{
	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}

void __cdecl overlapped_render()
{
	INVOKE(0x005A8E20, overlapped_render);
}

