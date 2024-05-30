#include "saved_games/c_storage_device.hpp"

#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <winerror.h>

HOOK_DECLARE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, _start);

dword online_guide_show_device_selector_ui(e_controller_index controller_index, dword bytes_requested, dword* device_id, void* xenon_task_handle)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(device_id != NULL);
	ASSERT(xenon_task_handle != NULL);

	if (controller_get(controller_index)->is_signed_in_to_machine())
		return ERROR_IO_PENDING; //XShowDeviceSelectorUI(controller_index, 1, 0, bytes_requested, device_id, xenon_task_handle);

	return E_FAIL;
}

dword __thiscall c_storage_device_selector_overlapped_task::_start(void* xenon_task_handle)
{
	ASSERT(VALID_INDEX(m_controller_index, k_number_of_controllers));
	ASSERT(m_storage_device != NULL);
	ASSERT(m_device_id != NULL);

	m_storage_device->set_device_selection_cancelled(false);

	return online_guide_show_device_selector_ui(m_controller_index, m_bytes_requested, m_device_id, xenon_task_handle);
}

void c_storage_device::set_device_selection_cancelled(bool device_selection_cancelled)
{
	m_flags.set(1, device_selection_cancelled);
}

