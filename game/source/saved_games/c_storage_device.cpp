#include "saved_games/c_storage_device.hpp"

#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <winerror.h>

HOOK_DECLARE_CLASS(0x0074D690, c_storage_device_selector_overlapped_task, start_);

dword online_guide_show_device_selector_ui(e_controller_index controller_index, dword bytes_requested, dword* device_id, void* xenon_task_handle)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(device_id != NULL);
	ASSERT(xenon_task_handle != NULL);

	if (controller_get(controller_index)->is_signed_in_to_machine())
		return ERROR_IO_PENDING; //XShowDeviceSelectorUI(controller_index, 1, 0, bytes_requested, device_id, xenon_task_handle);

	return 0x80004005;
}

dword __fastcall c_storage_device_selector_overlapped_task::start_(c_storage_device_selector_overlapped_task* _this, void* unused, void* xenon_task_handle)
{
	ASSERT(VALID_INDEX(_this->m_controller_index, k_number_of_controllers));
	ASSERT(_this->m_storage_device != NULL);
	ASSERT(_this->m_device_id != NULL);

	_this->m_storage_device->set_device_selection_cancelled(false);

	return online_guide_show_device_selector_ui(_this->m_controller_index, _this->m_bytes_requested, _this->m_device_id, xenon_task_handle);
}

void c_storage_device::set_device_selection_cancelled(bool device_selection_cancelled)
{
	m_flags.set(1, device_selection_cancelled);
}

