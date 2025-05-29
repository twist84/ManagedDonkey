#include "interface/user_interface_window_manager_debug.hpp"

#include "interface/user_interface_window_manager.hpp"

void gui_debug_bitmap_animation_state(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_bitmap, name, c_window_manager::_debug_animation_state, activate, false);
}

void gui_debug_bitmap_bounds(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_bitmap, name, c_window_manager::_debug_bounds, activate, false);
}

void gui_debug_bitmap_display_name(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_bitmap, name, c_window_manager::_debug_name, activate, false);
}

void gui_debug_bitmap_rotation(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_bitmap, name, c_window_manager::_debug_rotation_origin, activate, false);
}

void gui_debug_group_animation_state(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_group, name, c_window_manager::_debug_animation_state, activate, include_children);
}

void gui_debug_group_bounds(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_group, name, c_window_manager::_debug_bounds, activate, include_children);
}

void gui_debug_group_display_name(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_group, name, c_window_manager::_debug_name, activate, include_children);
}

void gui_debug_group_rotation(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_group, name, c_window_manager::_debug_rotation_origin, activate, include_children);
}

void gui_debug_list_animation_state(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list, name, c_window_manager::_debug_animation_state, activate, include_children);
}

void gui_debug_list_bounds(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list, name, c_window_manager::_debug_bounds, activate, include_children);
}

void gui_debug_list_display_name(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list, name, c_window_manager::_debug_name, activate, include_children);
}

void gui_debug_list_item_animation_state(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list_item, name, c_window_manager::_debug_rotation_origin, activate, include_children);
}

void gui_debug_list_item_bounds(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list_item, name, c_window_manager::_debug_animation_state, activate, include_children);
}

void gui_debug_list_item_display_name(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list_item, name, c_window_manager::_debug_bounds, activate, include_children);
}

void gui_debug_list_item_rotation(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list_item, name, c_window_manager::_debug_name, activate, include_children);
}

void gui_debug_list_rotation(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_list, name, c_window_manager::_debug_rotation_origin, activate, include_children);
}

void gui_debug_screen_animation_state(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_screen, name, c_window_manager::_debug_animation_state, activate, include_children);
}

void gui_debug_screen_bounds(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_screen, name, c_window_manager::_debug_bounds, activate, include_children);
}

void gui_debug_screen_display_name(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_screen, name, c_window_manager::_debug_name, activate, include_children);
}

void gui_debug_screen_rotation(int32 name, bool activate, bool include_children)
{
	window_manager_get()->debug_widget_state(_gui_screen, name, c_window_manager::_debug_rotation_origin, activate, include_children);
}

void gui_debug_text_animation_state(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_text, name, c_window_manager::_debug_animation_state, activate, false);
}

void gui_debug_text_bounds(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_text, name, c_window_manager::_debug_bounds, activate, false);
}

void gui_debug_text_display_name(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_text, name, c_window_manager::_debug_name, activate, false);
}

void gui_debug_text_rotation(int32 name, bool activate)
{
	window_manager_get()->debug_widget_state(_gui_text, name, c_window_manager::_debug_rotation_origin, activate, false);
}

void gui_print_active_screen_strings_tag_name()
{
	window_manager_get()->print_active_screen_strings_tag_name();
}

void gui_print_active_screens()
{
	window_manager_get()->print_active_screens();
}

