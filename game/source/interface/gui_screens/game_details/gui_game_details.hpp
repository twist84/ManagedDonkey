#pragma once

#include "interface/gui_screens/game_info_screen/game_info_screen.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"

class c_gui_game_details :
	public c_gui_game_info_screen
{
public:
	c_gui_game_details(int32 name) :
		c_gui_game_info_screen(name)
	{
		DECLFUNC(0x00B0AB70, void, __thiscall, c_gui_game_info_screen*, int32)(this, name);
	}

//protected:
	s_network_squad_status_data m_status_data;
};
COMPILE_ASSERT(sizeof(c_gui_game_details) == 0x17F60);