#pragma once

#include "interface/c_gui_screen_widget.hpp"

struct c_gui_game_info_screen :
	c_gui_screen_widget
{
public:
	c_gui_game_info_screen(long name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00ADE090, c_gui_game_info_screen*, __thiscall, c_gui_game_info_screen*, long)(this, name);
	}
};
static_assert(sizeof(c_gui_game_info_screen) == sizeof(c_gui_screen_widget));