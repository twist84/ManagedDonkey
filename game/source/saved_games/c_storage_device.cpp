#include "saved_games/c_storage_device.hpp"

#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <winerror.h>

HOOK_DECLARE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, start_);

//.text:005A4BF0 ; public: __cdecl c_storage_device::~c_storage_device()
//.text:0074D3A0 ; public: __cdecl c_storage_device::c_storage_device()
//.text:0074D3E0 ; public: __cdecl c_storage_device_selector_overlapped_task::c_storage_device_selector_overlapped_task(char const*, int32)
//.text:0074D400 ; public: virtual void* __cdecl c_storage_device_selector_overlapped_task::`scalar deleting destructor'(unsigned int)
//.text:0074D430 ; public: virtual void __cdecl c_storage_device_selector_overlapped_task::failure(uns32, uns32, uns32)
//.text:0074D520 ; public: bool __cdecl c_controller_interface::get_always_show_device_selector_ui() const
//.text:0074D530 ; public: virtual char const* __cdecl c_storage_device_selector_overlapped_task::get_context_string() const
//.text:0074D540 ; public: char const* __cdecl c_overlapped_task::get_file() const
//.text:0074D550 ; public: e_controller_index __cdecl c_storage_device_selector_overlapped_task::get_controller_index() const
//.text:0074D560 ; public: static bool __cdecl c_storage_device::get_device_data(uns32, e_storage_device_type*, uns64*, uns64*, wchar_t*, int32)
//.text:0074D570 ; public: uns32 __cdecl c_storage_device::get_device_id() const
//.text:0074D580 ; public: e_storage_device_type __cdecl c_storage_device::get_device_type() const
//.text:0074D5A0 ; public: bool __cdecl c_controller_interface::get_storage_device_selection_performed()
//.text:0074D5B0 ; public: uns64 __cdecl c_storage_device::get_total_free_bytes() const
//.text:0074D5D0 ; public: void __cdecl c_storage_device_selector_overlapped_task::initialize(c_storage_device*, e_controller_index, uns32, uns32*)
//.text:0074D620 ; 
//.text:0074D630 ; public: void __cdecl c_storage_device::release()
//.text:0074D640 ; public: void __cdecl c_storage_device::select_device(uns32)

void c_storage_device::set_device_selection_cancelled(bool device_selection_cancelled)
{
	//INVOKE_CLASS_MEMBER(0x0074D670, c_storage_device, set_device_selection_cancelled, device_selection_cancelled);

	SET_BIT(m_flags, 1, device_selection_cancelled);
}

uns32 __thiscall c_storage_device_selector_overlapped_task::start_(void* xenon_task_handle)
{
	//INVOKE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, start, xenon_task_handle);

	ASSERT(VALID_INDEX(m_controller_index, k_number_of_controllers));
	ASSERT(m_storage_device != NULL);
	ASSERT(m_device_id != NULL);

	m_storage_device->set_device_selection_cancelled(false);

	return online_guide_show_device_selector_ui(m_controller_index, m_minimum_required_bytes, false, m_device_id, xenon_task_handle);
}

//.text:0074D6A0 ; public: void __cdecl c_storage_device::storage_device_selection_cancelled()
//.text:0074D6C0 ; public: void __cdecl c_storage_device::storage_device_selection_failed()
//.text:0074D6F0 ; public: void __cdecl c_storage_device::storage_device_selection_succeeded(uns32)
//.text:0074D780 ; public: virtual void __cdecl c_storage_device_selector_overlapped_task::success(uns32)
//.text:0074D7A0 ; 


