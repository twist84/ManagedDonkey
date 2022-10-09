#pragma once

struct c_window_manager
{
	// #TODO: map this
	static void* this_ptr;

	static c_gui_screen_widget* __stdcall allocate_codeless_screen(long screen_name);
	static c_gui_screen_widget* __stdcall allocate_named_screen(long screen_name);
};

extern void __cdecl window_manager_reset_screens();