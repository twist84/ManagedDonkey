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
	s_player_configuration m_target_player_configuration;

	bool m_target_player_configuration_valid;
	bool m_full_details;
	byte __data30D2[0x2];
	
	c_string_id m_biggest_kill_name;

	byte __data30D8[0x4];

	long m_implement_of_destruction;

	long m_killed_most_player_index;
	short m_killed_count;
	long m_killed_most_by_player_index;
	short m_killed_by_count;
};
static_assert(sizeof(c_gui_player_select_screen_widget) == sizeof(c_gui_screen_widget) + 0x1650);

