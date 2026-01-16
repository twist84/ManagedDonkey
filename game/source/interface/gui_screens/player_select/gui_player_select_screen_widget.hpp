#pragma once

#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"

class c_gui_player_select_screen_widget :
	public c_gui_screen_widget
{
public:
	void __thiscall initialize_();

	bool __thiscall try_and_get_render_data_emblem_info_(c_gui_bitmap_widget* bitmap_widget, s_emblem_info* emblem_info);

	c_gui_player_select_screen_widget(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00B07790, void, __thiscall, c_gui_player_select_screen_widget*, int32)(this, name);
	}

//protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
	s_player_configuration m_target_player_configuration;

	bool m_target_player_configuration_valid;
	bool m_full_details;
	byte __data30D2[0x2];
	
	c_string_id m_biggest_kill_name;

	byte __data30D8[0x4];

	int32 m_implement_of_destruction;

	int32 m_killed_most_player_index;
	int16 m_killed_count;
	int32 m_killed_most_by_player_index;
	int16 m_killed_by_count;
};
COMPILE_ASSERT(sizeof(c_gui_player_select_screen_widget) == sizeof(c_gui_screen_widget) + 0x1650);

