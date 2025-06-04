#pragma once

#include "cseries/async_xoverlapped.hpp"

class c_storage_device;
class c_storage_device_selector_overlapped_task :
	public c_overlapped_task
{
public:
	uns32 __thiscall start_(void* xenon_task_handle);

	c_storage_device* m_storage_device;
	e_controller_index m_controller_index;
	uns32 m_minimum_required_bytes;
	uns32* m_device_id;
	bool m_always_show;
};
static_assert(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

class c_storage_device
{
public:
	void set_device_selection_cancelled(bool device_selection_cancelled);

	int32 m_flags;
	uns32 m_device_id;
	e_controller_index m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
static_assert(sizeof(c_storage_device) == 0x30);

