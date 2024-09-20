#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

struct c_start_menu_settings_appearance_colors :
	public c_start_menu_pane_screen_widget
{
	void set_color_focused_list_item(long name, e_player_color_index player_color_index);
	void __thiscall set_color_values_from_profile();
};
static_assert(sizeof(c_start_menu_settings_appearance_colors) == sizeof(c_start_menu_pane_screen_widget));

