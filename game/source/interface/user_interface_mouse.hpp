#pragma once

#include "cseries/cseries.hpp"

enum e_controller_index;

struct s_user_interface_mouse_globals
{
	e_controller_index controller_index;
	real_rectangle2d window_bounds;
	real_point2d window_cursor_position;
	real_point2d last_window_mouse_coords;
	byte mouse_move_ticks;
	byte left_button_down_ticks;
	byte right_button_down_ticks;
	short mouse_wheel_delta;
	void* last_focused_widget;
	c_string_id last_screen_widget_name;
	bool list_item_selected_dirty;
};
static_assert(sizeof(s_user_interface_mouse_globals) == 0x38);

extern s_user_interface_mouse_globals user_interface_mouse_globals;
extern point2d global_cursor_position;

extern void __cdecl user_interface_mouse_update();

