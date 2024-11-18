#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_data.hpp"
#include "shell/shell.hpp"

struct c_gui_roster_data :
	c_gui_ordered_data
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
		c_enum<e_player_row_type, long, _player_row_type_player, k_player_row_type_count> player_row_type;
		long session_player_index;

		s_player_identifier player_identifier;
		bool player_identifier_valid;

		// pad
		long __unknown14;

		s_player_configuration player_configuration;
		bool player_configuration_valid;

		struct
		{
			long skill_level;
			long experience;
		} configuration;

		long machine_index;
		long squad_join_sequence_number;
		long __unknown164C;

		qword squad_nonce;

		long leader_team;
		c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> local_controller_index;
		long voice_state;
		long party_bar_length;
		bool is_leader;
		bool show_teams;
		long special_status;
		
		//bool in_matchmaking;
		struct
		{
			long highest_party_skill;
			long highest_party_experience;
		} calculated_for_group;
	};
	static_assert(sizeof(s_player_row) == 0x1678);

public:
	bool __thiscall _get_integer_value(long element_handle, long value_name, long* value);
	bool __thiscall _get_text_value(long element_handle, long value_name, c_static_wchar_string<1024>* value);

protected:
	long m_matchmaking_last_known_good_extra_slots_searching;
	long m_matchmaking_last_known_good_extra_slots_found;
	c_static_array<c_gui_roster_data::s_player_row, 16> m_players;
	long m_player_count;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_driving_controller;
	bool m_pause_updating;
};
static_assert(sizeof(c_gui_roster_data) == 0x168A8);

struct c_gui_active_roster_data :
	c_gui_roster_data
{
};
static_assert(sizeof(c_gui_active_roster_data) == sizeof(c_gui_roster_data));

struct c_gui_static_roster_data :
	c_gui_roster_data
{
};
static_assert(sizeof(c_gui_static_roster_data) == sizeof(c_gui_roster_data));

