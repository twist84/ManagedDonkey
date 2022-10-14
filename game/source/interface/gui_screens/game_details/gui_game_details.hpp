#pragma once

#include "interface/gui_screens/game_info_screen/game_info_screen.hpp"
#include "networking/logic/network_session_interface.hpp"

struct c_gui_game_details : c_gui_game_info_screen
{
	s_network_squad_status_data m_status_data;
};
static_assert(sizeof(c_gui_game_details) == 0x17F60);