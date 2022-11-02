#include "interface/user_interface_window_manager.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);

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

c_gui_screen_widget* __cdecl c_window_manager::load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index)
{
	FUNCTION_BEGIN(true);

	return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, long, bool, c_load_screen_message const*, long)(&g_window_manager, controller_index, unused, screen_message, window_index);
}

bool __cdecl c_window_manager::named_screen_defined_in_code(long screen_name)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00AABFD0, named_screen_defined_in_code, screen_name);
}

void __cdecl window_manager_reset_screens()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00AADAC0, window_manager_reset_screens);
}