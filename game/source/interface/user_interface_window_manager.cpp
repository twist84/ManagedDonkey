#include "interface/user_interface_window_manager.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void* c_window_manager::this_ptr = reinterpret_cast<void*>(0x05260F34);

c_gui_screen_widget* __stdcall c_window_manager::allocate_codeless_screen(long screen_name)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00AA8D90, allocate_codeless_screen, screen_name);
}

c_gui_screen_widget* __stdcall c_window_manager::allocate_named_screen(long screen_name)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00AA8E00, allocate_named_screen, screen_name);
}

void __cdecl window_manager_reset_screens()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00AADAC0, window_manager_reset_screens);
}