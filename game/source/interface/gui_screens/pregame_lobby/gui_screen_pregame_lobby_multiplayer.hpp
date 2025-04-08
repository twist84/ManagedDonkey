#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

struct c_gui_screen_pregame_lobby_multiplayer :
	public c_gui_screen_pregame_lobby
{
public:
	bool __thiscall handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);

	c_gui_screen_pregame_lobby_multiplayer(long name) :
		c_gui_screen_pregame_lobby(name)
	{
		DECLFUNC(0x00B01130, c_gui_screen_pregame_lobby_multiplayer*, __thiscall, c_gui_screen_pregame_lobby_multiplayer*, long)(this, name);
	}
};
static_assert(sizeof(c_gui_screen_pregame_lobby_multiplayer) == sizeof(c_gui_screen_pregame_lobby));

