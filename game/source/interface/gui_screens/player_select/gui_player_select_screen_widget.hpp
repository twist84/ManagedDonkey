#pragma once

#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"

struct c_gui_player_select_screen_widget :
	public c_gui_screen_widget
{
public:
	void __thiscall initialize_();

//protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
	s_player_configuration m_player_data;

	bool __unknown30D0;
	bool __unknown30D1;
	byte __data30D2[0x2];
	
	c_string_id m_biggest_kill_name;

	byte __data30D8[0x4];

	long m_damage_reporting_type;

	long __unknown30E0;
	byte __data30E4[0x4];
	long __unknown30E8;
	byte __data30EC[0x4];
};
static_assert(sizeof(c_gui_player_select_screen_widget) == sizeof(c_gui_screen_widget) + 0x1650);

