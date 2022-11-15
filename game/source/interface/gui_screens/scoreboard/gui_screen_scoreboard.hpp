#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct c_gui_screen_scoreboard : c_gui_screen_widget
{
	enum e_scoreboard_mode;

protected:
	long m_current_scoreboard_mode;
	bool m_is_interactive;
};

struct c_gui_scoreboard_data : c_gui_ordered_data
{
	enum e_player_row_type
	{
		_player_row_type_player = 0,
		_player_row_type_team_bar,

		k_player_row_type_count
	};

	struct s_player_row
	{
		c_enum<e_player_row_type, long, k_player_row_type_count> player_row_type;
		long player_index;
		long network_player_index;
		s_player_appearance player_appearance;
		c_static_wchar_string<48> player_name;
		c_static_wchar_string<48> service_tag;
		long base_color;
		long team_color;
		bool team;
		long controller_index;
		long voice_output;
		long connectivity_rating;
		c_static_wchar_string<48> place;
		c_static_wchar_string<48> score;
		c_static_wchar_string<48> round_score;
		bool dead;
		bool left;
	};
	static_assert(sizeof(s_player_row) == 0x868);

protected:
	long m_current_scoreboard_mode;
	c_static_array<s_player_row, 25> m_player_rows;
	long m_player_row_count;
	c_enum<e_controller_index, long, k_number_of_controllers> m_controller_index;
};
static_assert(sizeof(c_gui_scoreboard_data) == 0xD340);
