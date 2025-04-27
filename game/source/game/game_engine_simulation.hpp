#pragma once

#include "cseries/cseries.hpp"

enum e_multiplayer_event_type
{
	_multiplayer_event_type_general = 0,
	_multiplayer_event_type_flavor,
	_multiplayer_event_type_slayer,
	_multiplayer_event_type_capture_the_flag,
	_multiplayer_event_type_oddball,
	_multiplayer_event_type_dummy,
	_multiplayer_event_type_king_of_the_hill,
	_multiplayer_event_type_vip,
	_multiplayer_event_type_territories,
	_multiplayer_event_type_juggernaut,
	_multiplayer_event_type_assault,
	_multiplayer_event_type_infection,
	_multiplayer_event_type_survival,
	_multiplayer_event_type_earn_wp,

	k_multiplayer_event_type_count
};

struct s_territories_event_data
{
	short territory_index;
};
static_assert(sizeof(s_territories_event_data) == sizeof(short));

struct s_objective_game_role_change_event_data
{
	long objective_game_role_index;
};
static_assert(sizeof(s_objective_game_role_change_event_data) == sizeof(long));

struct s_game_engine_event_data
{
	long event_type;
	long event;
	long identifier;
	long audience_player_index;
	long cause_player_index;
	long cause_team_index;
	long effect_player_index;
	long effect_team_index;
	long event_quantity;

	union
	{
		s_territories_event_data territories_event_data;
		s_objective_game_role_change_event_data objective_game_role_event_data;
	};
};
static_assert(sizeof(s_game_engine_event_data) == 0x28);

enum e_game_engine_queued_event_flags
{
	_game_engine_queued_event_valid_bit = 0,
	_game_engine_queued_event_played_bit,
	_game_engine_queued_event_long_delay_bit,

	k_game_engine_queued_event_flags_count
};

struct s_game_engine_queued_event
{
	uint32 received_game_time;
	c_flags<e_game_engine_queued_event_flags, uint8, k_game_engine_queued_event_flags_count> flags;
	s_game_engine_event_data event_data;
};
static_assert(sizeof(s_game_engine_queued_event) == 0x30);

