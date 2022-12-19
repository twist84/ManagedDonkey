#include "cseries/async_xoverlapped.hpp"

#include "memory/module.hpp"

#include <assert.h>

//HOOK_DECLARE(0x005A8F40, overlapped_task_start_internal);
HOOK_DECLARE_CALL(0x00AF8495, overlapped_task_start_internal); // c_start_menu_settings_appearance_emblem::handle_list_item_chosen

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

bool __stdcall overlapped_task_start_internal(c_overlapped_task* task, const char* file, int line)
{
	assert(task);
	task->set_file(file);
	task->set_line(line);

	dword result = task->start(get_donkey_module());

	if (result == 0x80004005 || result == 0xFFFFFFFF)
		return false;

	task->m_task_state = _overlapped_task_state_succeeded;

	return true;
}

