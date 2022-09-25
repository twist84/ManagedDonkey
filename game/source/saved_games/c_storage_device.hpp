#pragma once

#include "cseries/async_xoverlapped.hpp"

struct c_storage_device;
struct c_storage_device_selector_overlapped_task : public c_overlapped_task
{
	c_storage_device* m_storage_device;
	long m_controller_index;
	dword_flags m_flags;
	long* m_device_id;
	byte __data[0x4];
};
static_assert(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

struct c_storage_device
{
	dword_flags m_flags;
	long m_device_id;
	long m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
static_assert(sizeof(c_storage_device) == 0x30);