#pragma once

#include "cseries/cseries.hpp"

class c_gui_widget;
class c_gui_list_item_widget;
class c_gui_list_widget;
class c_gui_screen_widget;

struct s_user_interface_mouse_globals
{
	e_controller_index controller_index;
	real_rectangle2d window_bounds;
	real_point2d window_cursor_position;
	real_point2d last_window_cursor_position;
	long_point2d update_cursor_position;
	uns8 mouse_move_ticks;
	uns8 left_button_frames_down;
	uns8 right_button_frames_down;
	int16 mouse_wheel_delta;
	int16 mouse_hwheel_delta;
	void* last_focused_widget;
	bool list_item_selected_dirty;
};
COMPILE_ASSERT(sizeof(s_user_interface_mouse_globals) == 0x3C);

extern s_user_interface_mouse_globals user_interface_mouse_globals;

extern void user_interface_mouse_update();
extern void user_interface_mouse_update_tracking();
extern void user_interface_mouse_update_internal();
extern void user_interface_mouse_compute_widget_bounds(c_gui_widget* widget, real_rectangle2d* accumulated_bounds, bool(*child_filter)(const c_gui_widget*));
extern void user_interface_mouse_compute_list_item_bounds(c_gui_list_item_widget* list_item_widget, real_rectangle2d* bounds);
extern bool user_interface_mouse_handle_spinner_list_widget_focus(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget);
extern void user_interface_mouse_handle_scroll_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget, int32 scroll_amount);
extern bool user_interface_mouse_handle_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget);
extern bool user_interface_mouse_handle_list_widgets(c_gui_screen_widget* screen_widget, c_gui_widget* parent_widget);
extern bool user_interface_mouse_handle_screen_widget(c_gui_screen_widget* screen_widget);

