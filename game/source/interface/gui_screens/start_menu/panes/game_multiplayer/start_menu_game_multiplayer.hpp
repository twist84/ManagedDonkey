#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_game_multiplayer :
	public c_start_menu_pane_screen_widget
{
public:
	wchar_t m_gametype[32];
	wchar_t m_gametype_description[256];
};
static_assert(sizeof(c_start_menu_game_multiplayer) == 0x23A8);
static_assert(sizeof(c_start_menu_game_multiplayer) == sizeof(c_start_menu_pane_screen_widget) + 0x240);

