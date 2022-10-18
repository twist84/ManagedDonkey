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
		long __unknown4;
		long __unknown8;
		s_player_appearance player_appearance;
		c_static_wchar_string<48> __unknown66C;
		c_static_wchar_string<48> __unknown6CC;
		long __unknown72C;
		long __unknown730;
		bool __unknown734;
		long __unknown738;
		long __unknown73C;
		long __unknown740;
		c_static_wchar_string<48> __unknown744;
		c_static_wchar_string<48> __unknown7A4;
		c_static_wchar_string<48> __unknown804;
		bool __unknown864;
		bool __unknown865;
	};
	static_assert(sizeof(s_player_row) == 0x868);

protected:
	long m_current_scoreboard_mode;
	c_gui_scoreboard_data::s_player_row m_players[25];
	long m_player_count;
	c_enum<e_controller_index, long, k_number_of_controllers> m_controller_index;
};
static_assert(sizeof(c_gui_scoreboard_data) == 0xD340);
