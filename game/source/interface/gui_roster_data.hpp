#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_data.hpp"
#include "shell/shell.hpp"

struct c_gui_roster_data : c_gui_ordered_data
{
	enum e_player_row_type
	{
		_player_row_type_player,
		_player_row_type_player_found,
		_player_row_type_looking_for_player,
		_player_row_type_press_a_to_join,

		k_player_row_type_count
	};

	enum e_voice_talking_state
	{
	};

	struct s_player_row
	{
		c_enum<e_player_row_type, long, k_player_row_type_count> player_row_type;
		long player_index;
		s_player_identifier player_identifier;
		bool __unknown10;
		long __unknown14;
		s_player_configuration player_data;
		bool __unknown1638;
		long skill_level;
		long __unknown1640;
		long peer_index;
		long peer_user_index;
		long __unknown164C;
		qword party_nonce;
		long leader_team_index;
		c_enum<e_controller_index, long, k_number_of_controllers> controller_index;
		long voice_talking_state;
		long party_bar_length;
		bool is_leader;
		bool show_teams;
		long special_status;
		byte __data[0x8];
	};
	static_assert(sizeof(s_player_row) == 0x1678);

public:
	static bool __fastcall get_text_value(c_gui_roster_data* _this, void* unused, long player_row_index, long name, c_static_wchar_string<1024>* text_value);

protected:
	long __unknown10C;
	long __unknown110;
	byte __data114[4];
	c_gui_roster_data::s_player_row m_players[16];
	long m_player_count;
	c_enum<e_controller_index, long, k_number_of_controllers> m_controller_index;
	bool __unknown168A0;
	byte __data168A1[0x7];
};
static_assert(sizeof(c_gui_roster_data) == 0x168A8);

struct c_gui_active_roster_data : c_gui_roster_data
{
};
static_assert(sizeof(c_gui_active_roster_data) == sizeof(c_gui_roster_data));

struct c_gui_static_roster_data : c_gui_roster_data
{
};
static_assert(sizeof(c_gui_static_roster_data) == sizeof(c_gui_roster_data));

