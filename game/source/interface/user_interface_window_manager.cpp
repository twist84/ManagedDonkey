#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);

c_gui_screen_widget* __stdcall c_window_manager::allocate_codeless_screen(long screen_name)
{
	return INVOKE(0x00AA8D90, allocate_codeless_screen, screen_name);
}

c_gui_screen_widget* __stdcall c_window_manager::allocate_named_screen(long screen_name)
{
	return INVOKE(0x00AA8E00, allocate_named_screen, screen_name);
}

void __cdecl c_window_manager::handle_global_controller_event(s_event_record* event_record)
{
	DECLFUNC(0x00AAB7D0, void, __thiscall, c_window_manager*, s_event_record*)(this, event_record);
}

c_gui_screen_widget* __cdecl c_window_manager::load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index)
{
	return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, long, bool, c_load_screen_message const*, long)(&g_window_manager, controller_index, unused, screen_message, window_index);
}

bool __cdecl c_window_manager::named_screen_defined_in_code(long screen_name)
{
	return INVOKE(0x00AABFD0, named_screen_defined_in_code, screen_name);
}

c_gui_screen_widget* __cdecl c_window_manager::get_screen_above(e_window_index window_index, c_gui_screen_widget* screen)
{
	return DECLFUNC(0x00AAB490, c_gui_screen_widget*, __thiscall, c_window_manager*, e_window_index, c_gui_screen_widget*)(this, window_index, screen);
}

c_window_manager* __cdecl window_manager_get()
{
	return &g_window_manager;
}

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}