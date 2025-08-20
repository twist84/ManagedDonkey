#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_settings :
	public c_start_menu_pane_screen_widget
{
public:
};
static_assert(sizeof(c_start_menu_settings) == 0x2168);
static_assert(sizeof(c_start_menu_settings) == sizeof(c_start_menu_pane_screen_widget));

