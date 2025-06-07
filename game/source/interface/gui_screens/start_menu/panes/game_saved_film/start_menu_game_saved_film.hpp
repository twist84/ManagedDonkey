#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_game_saved_film :
	public c_start_menu_pane_screen_widget
{
public:
	e_controller_index m_controller_index;
};
static_assert(sizeof(c_start_menu_game_saved_film) == 0x2170);
static_assert(sizeof(c_start_menu_game_saved_film) == sizeof(c_start_menu_pane_screen_widget) + 0x8);

