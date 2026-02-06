#include "saved_games/c_storage_device.hpp"

#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <winerror.h>

HOOK_DECLARE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, start_);

uns32 __thiscall c_storage_device_selector_overlapped_task::start_(void* platform_handle)
{
	return c_storage_device_selector_overlapped_task::start(platform_handle);
}

//.text:005A4BF0 ; public: __cdecl c_storage_device::~c_storage_device()
//.text:0074D3A0 ; public: __cdecl c_storage_device::c_storage_device()

c_storage_device_selector_overlapped_task::c_storage_device_selector_overlapped_task(const char* file, int32 line) :
	c_overlapped_task(file, line),
	m_storage_device(nullptr),
	m_controller_index(k_no_controller),
	m_minimum_required_bytes(0),
	m_device_id(nullptr),
	m_always_show(false)
{
	//DECLFUNC(0x0074D3E0, void, __thiscall, c_storage_device_selector_overlapped_task*, const char*, int32)(this, file, line);
}

//.text:0074D400 ; public: virtual void* __cdecl c_storage_device_selector_overlapped_task::`scalar deleting destructor'(unsigned int)

void c_storage_device_selector_overlapped_task::failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	INVOKE_CLASS_MEMBER(0x0074D430, c_storage_device_selector_overlapped_task, failure, calling_result, overlapped_error, overlapped_extended_error);
}

//.text:0074D520 ; public: bool __cdecl c_controller_interface::get_always_show_device_selector_ui() const

const char* c_storage_device_selector_overlapped_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x0074D530, c_storage_device_selector_overlapped_task, get_context_string);

	return "XShowDeviceSelectorUI";
}

//.text:0074D540 ; public: const char* __cdecl c_overlapped_task::get_file() const
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

uns32 c_storage_device_selector_overlapped_task::start(void* platform_handle)
{
	//INVOKE_CLASS_MEMBER(0x0074D690, c_storage_device_selector_overlapped_task, start, platform_handle);

	ASSERT(VALID_INDEX(m_controller_index, k_number_of_controllers));
	ASSERT(m_storage_device != nullptr);
	ASSERT(m_device_id != nullptr);

	m_storage_device->set_device_selection_cancelled(false);

	return online_guide_show_device_selector_ui(m_controller_index, m_minimum_required_bytes, false, m_device_id, platform_handle);
}

//.text:0074D6A0 ; public: void __cdecl c_storage_device::storage_device_selection_cancelled()
//.text:0074D6C0 ; public: void __cdecl c_storage_device::storage_device_selection_failed()
//.text:0074D6F0 ; public: void __cdecl c_storage_device::storage_device_selection_succeeded(uns32)

void c_storage_device_selector_overlapped_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x0074D780, c_storage_device_selector_overlapped_task, success, return_result);
}

//.text:0074D7A0 ; 


