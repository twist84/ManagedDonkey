#pragma once

struct c_gui_screen_widget;
struct c_load_screen_message;
struct c_window_manager
{
	// #TODO: map this
	static void* this_ptr;

	static c_gui_screen_widget* __stdcall allocate_codeless_screen(long screen_name);
	static c_gui_screen_widget* __stdcall allocate_named_screen(long screen_name);
	static c_gui_screen_widget* load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index);
	static bool __cdecl named_screen_defined_in_code(long screen_name);
};

extern void __cdecl window_manager_reset_screens();