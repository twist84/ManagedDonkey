#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

// player 1
// player 2
// player 3
// player 4
// console

struct s_data_array;
struct c_gui_screen_widget;
struct c_load_screen_message;
struct s_event_record;
enum e_window_index;
struct c_window_manager
{
	s_data_array* m_active_screens;
	dword __time4;
	c_gui_screen_widget* m_screen_array[5][10];
	c_synchronized_long m_current_channel_count[5];
	short_rectangle2d m_window_bounds[5];
	long __unknown10C[4];
	long __unknown11C;
	long __unknown120;
	long __unknown124;

	c_gui_screen_widget* allocate_codeless_screen(long screen_name);
	c_gui_screen_widget* allocate_named_screen(long screen_name);
	c_gui_screen_widget* get_screen_above(e_window_index window_index, c_gui_screen_widget* screen);
	c_gui_screen_widget* get_topmost_screen(e_window_index window_index);
	void handle_global_controller_event(s_event_record* event_record);
	c_gui_screen_widget* load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index);
	bool named_screen_defined_in_code(long screen_name);
	void update(dword milliseconds);
};
static_assert(sizeof(c_window_manager) == 0x128);

extern c_window_manager& g_window_manager;

extern c_window_manager* __cdecl window_manager_get();
extern void __cdecl window_manager_load_screen_hs(long screen_name);
extern void __cdecl window_manager_reset_screens();
