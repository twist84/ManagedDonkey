#pragma once

#include "cseries/async_xoverlapped.hpp"

class c_storage_device;
class c_storage_device_selector_overlapped_task :
	public c_overlapped_task
{
public:
	uns32 __thiscall start_(void* xenon_task_handle);

public:
	virtual ~c_storage_device_selector_overlapped_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;

public:
	c_storage_device_selector_overlapped_task(const char* file, int32 line);

//private:
	c_storage_device* m_storage_device;
	e_controller_index m_controller_index;
	uns32 m_minimum_required_bytes;
	uns32* m_device_id;
	bool m_always_show;
};
COMPILE_ASSERT(sizeof(c_storage_device_selector_overlapped_task) == 0x24);

class c_storage_device
{
public:
	void set_device_selection_cancelled(bool device_selection_cancelled);

	int32 m_flags;
	uns32 m_device_id;
	e_controller_index m_controller_index;
	c_storage_device_selector_overlapped_task m_storage_device_selector_task;
};
COMPILE_ASSERT(sizeof(c_storage_device) == 0x30);

