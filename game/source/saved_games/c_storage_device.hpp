#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

enum e_controller_index;
struct c_storage_device;
struct c_storage_device_selector_overlapped_task :
	public c_overlapped_task
{
	dword __thiscall start_(void* xenon_task_handle);

	c_storage_device* m_storage_device;
	e_controller_index m_controller_index;
	dword m_minimum_required_bytes;
	dword* m_device_id;
	bool m_always_show;
};
static_assert(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

struct c_storage_device
{
	void set_device_selection_cancelled(bool device_selection_cancelled);

	long m_flags;
	dword m_device_id;
	e_controller_index m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
static_assert(sizeof(c_storage_device) == 0x30);

