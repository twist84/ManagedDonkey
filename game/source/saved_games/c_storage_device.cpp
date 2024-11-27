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

//.text:005A4BF0 ; public: __cdecl c_storage_device::~c_storage_device()
//.text:0074D3A0 ; public: __cdecl c_storage_device::c_storage_device()
//.text:0074D3E0 ; public: __cdecl c_storage_device_selector_overlapped_task::c_storage_device_selector_overlapped_task(char const*, long)
//.text:0074D400 ; public: virtual void* __cdecl c_storage_device_selector_overlapped_task::`scalar deleting destructor'(unsigned int)
//.text:0074D430 ; public: virtual void __cdecl c_storage_device_selector_overlapped_task::failure(dword, dword, dword)
//.text:0074D520 ; 
//.text:0074D530 ; public: virtual char const* __cdecl c_storage_device_selector_overlapped_task::get_context_string() const
//.text:0074D540 ; public: char const* __cdecl c_overlapped_task::get_file() const
//.text:0074D550 ; public: e_controller_index __cdecl c_storage_device_selector_overlapped_task::get_controller_index() const
//.text:0074D560 ; public: static bool __cdecl c_storage_device::get_device_data(dword, e_storage_device_type*, qword*, qword*, wchar_t*, long)
//.text:0074D570 ; public: dword __cdecl c_storage_device::get_device_id() const
//.text:0074D580 ; 
//.text:0074D590 ; 
//.text:0074D5A0 ; 
//.text:0074D5B0 ; public: qword __cdecl c_storage_device::get_total_free_bytes() const
//.text:0074D5D0 ; public: void __cdecl c_storage_device_selector_overlapped_task::initialize(c_storage_device*, e_controller_index, dword, dword*)
//.text:0074D620 ; 
//.text:0074D630 ; public: void __cdecl c_storage_device::release()
//.text:0074D640 ; public: void __cdecl c_storage_device::select_device(dword)

void c_storage_device::set_device_selection_cancelled(bool device_selection_cancelled)
{
	//INVOKE_CLASS_MEMBER(0x0074D670, c_storage_device, set_device_selection_cancelled, device_selection_cancelled);

	m_flags.set(1, device_selection_cancelled);
}

dword __thiscall c_storage_device_selector_overlapped_task::_start(void* xenon_task_handle)
{
	//INVOKE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, start, xenon_task_handle);

	ASSERT(VALID_INDEX(m_controller_index, k_number_of_controllers));
	ASSERT(m_storage_device != NULL);
	ASSERT(m_device_id != NULL);

	m_storage_device->set_device_selection_cancelled(false);

	return online_guide_show_device_selector_ui(m_controller_index, m_bytes_requested, m_device_id, xenon_task_handle);
}

//.text:0074D6A0 ; 
//.text:0074D6C0 ; public: void __cdecl c_storage_device::storage_device_selection_failed()
//.text:0074D6F0 ; public: void __cdecl c_storage_device::storage_device_selection_succeeded(dword)
//.text:0074D780 ; public: virtual void __cdecl c_storage_device_selector_overlapped_task::success(dword)
//.text:0074D7A0 ; 


