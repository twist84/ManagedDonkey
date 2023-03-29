#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct c_storage_device;
struct c_storage_device_selector_overlapped_task : public c_overlapped_task
{
	static dword __fastcall start_(c_storage_device_selector_overlapped_task* _this, void* unused, void* xenon_task_handle);

	c_storage_device* m_storage_device;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;
	dword m_bytes_requested;
	dword* m_device_id;
	bool __unknown20;
};
static_assert(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

struct c_storage_device
{
	void set_device_selection_cancelled(bool device_selection_cancelled);

	c_flags<long, dword_flags, 8> m_flags;
	long m_device_id;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
static_assert(sizeof(c_storage_device) == 0x30);

