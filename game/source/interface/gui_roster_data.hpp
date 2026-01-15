#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/user_interface_data.hpp"

class c_gui_roster_data :
	public c_gui_ordered_data
{
public:
	enum e_player_row_type
	{
		_player_row_type_player,
		_player_row_type_found,
		_player_row_type_searching,
		_player_row_type_press_a_to_join,

		k_player_row_type_count
	};

	enum e_voice_talking_state
	{
		_voice_state_none = 0,
		_voice_state_has_voice,
		_voice_state_talking,
		_voice_state_away_in_private_chat,
		_voice_state_muted
	};

	enum e_special_status
	{
		_special_status_none = 0,
		_special_status_error,
		_special_status_party_up_undecided_leader,
		_special_status_party_up_chosen_leader,
		_special_status_party_up_chosen_member
	};

	struct s_player_row
	{
		s_player_row();

		e_player_row_type player_row_type;
		int32 session_player_index;
		s_player_identifier player_identifier;
		bool player_identifier_valid;
		int32 pad0;
		s_player_configuration player_configuration;
		bool player_configuration_valid;

		struct
		{
			int32 skill_level;
			int32 experience;
		} configuration;

		int32 machine_index;
		int32 squad_join_sequence_number;
		int32 pad1;
		uns64 squad_nonce;
		int32 leader_team;
		e_controller_index local_controller_index;
		int32 voice_state;
		int32 party_bar_length;
		bool is_leader;
		bool show_teams;
		int32 special_status;
		
		//bool in_matchmaking;
		struct
		{
			int32 highest_party_skill;
			int32 highest_party_experience;
		} calculated_for_group;
	};
	static_assert(sizeof(s_player_row) == 0x1678);

public:
	bool __thiscall get_integer_value_(int32 element_handle, string_id value_name, int32* value);
	bool __thiscall get_text_value_(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* value);

public:
	virtual ~c_gui_roster_data() = default;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual bool get_player_appearance(int32 element_handle, s_player_appearance* appearance) override;

protected:
	c_gui_roster_data(e_controller_index driving_controller);
	virtual int32 get_current_item_count_internal() override;

public:
	virtual bool get_player_identifier_value(int32 element_handle, s_player_identifier* player_identifier);
	virtual bool get_player_configuration_value(int32 element_handle, s_player_configuration* player_configuration);

protected:
	int32 m_matchmaking_last_known_good_extra_slots_searching;
	int32 m_matchmaking_last_known_good_extra_slots_found;
	c_static_array<s_player_row, k_maximum_players> m_players;
	int32 m_player_count;
	e_controller_index m_driving_controller;
	bool m_pause_updating;
};
static_assert(sizeof(c_gui_roster_data) == 0x168A8);
static_assert(0x0000 == OFFSETOF(c_gui_roster_data::s_player_row, player_row_type));
static_assert(0x0004 == OFFSETOF(c_gui_roster_data::s_player_row, session_player_index));
static_assert(0x0008 == OFFSETOF(c_gui_roster_data::s_player_row, player_identifier));
static_assert(0x0010 == OFFSETOF(c_gui_roster_data::s_player_row, player_identifier_valid));
static_assert(0x0018 == OFFSETOF(c_gui_roster_data::s_player_row, player_configuration));
static_assert(0x1638 == OFFSETOF(c_gui_roster_data::s_player_row, player_configuration_valid));
static_assert(0x163C == OFFSETOF(c_gui_roster_data::s_player_row, configuration));
static_assert(0x163C == OFFSETOF(c_gui_roster_data::s_player_row, configuration.skill_level));
static_assert(0x1640 == OFFSETOF(c_gui_roster_data::s_player_row, configuration.experience));
static_assert(0x1644 == OFFSETOF(c_gui_roster_data::s_player_row, machine_index));
static_assert(0x1648 == OFFSETOF(c_gui_roster_data::s_player_row, squad_join_sequence_number));
static_assert(0x1650 == OFFSETOF(c_gui_roster_data::s_player_row, squad_nonce));
static_assert(0x1658 == OFFSETOF(c_gui_roster_data::s_player_row, leader_team));
static_assert(0x165C == OFFSETOF(c_gui_roster_data::s_player_row, local_controller_index));
static_assert(0x1660 == OFFSETOF(c_gui_roster_data::s_player_row, voice_state));
static_assert(0x1664 == OFFSETOF(c_gui_roster_data::s_player_row, party_bar_length));
static_assert(0x1668 == OFFSETOF(c_gui_roster_data::s_player_row, is_leader));
static_assert(0x1669 == OFFSETOF(c_gui_roster_data::s_player_row, show_teams));
static_assert(0x166C == OFFSETOF(c_gui_roster_data::s_player_row, special_status));
static_assert(0x1670 == OFFSETOF(c_gui_roster_data::s_player_row, calculated_for_group));
static_assert(0x1670 == OFFSETOF(c_gui_roster_data::s_player_row, calculated_for_group.highest_party_skill));
static_assert(0x1674 == OFFSETOF(c_gui_roster_data::s_player_row, calculated_for_group.highest_party_experience));

class c_gui_active_roster_data :
	public c_gui_roster_data
{
public:
	void __thiscall update_();

public:
	virtual ~c_gui_active_roster_data() = default;
	virtual void update() override;

public:
	c_gui_active_roster_data(e_controller_index driving_controller);
};
static_assert(sizeof(c_gui_active_roster_data) == sizeof(c_gui_roster_data));

class c_gui_static_roster_data :
	public c_gui_roster_data
{
public:
	virtual ~c_gui_static_roster_data() = default;

public:
	c_gui_static_roster_data();
};
static_assert(sizeof(c_gui_static_roster_data) == sizeof(c_gui_roster_data));

