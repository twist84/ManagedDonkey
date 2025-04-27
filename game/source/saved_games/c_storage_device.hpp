#pragma once

#include "cseries/async_xoverlapped.hpp"

struct c_storage_device;
struct c_storage_device_selector_overlapped_task :
	public c_overlapped_task
{
	uint32 __thiscall start_(void* xenon_task_handle);

	c_storage_device* m_storage_device;
	e_controller_index m_controller_index;
	uint32 m_minimum_required_bytes;
	uint32* m_device_id;
	bool m_always_show;
};
static_assert(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

struct c_storage_device
{
	void set_device_selection_cancelled(bool device_selection_cancelled);

	int32 m_flags;
	uint32 m_device_id;
	e_controller_index m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
static_assert(sizeof(c_storage_device) == 0x30);

