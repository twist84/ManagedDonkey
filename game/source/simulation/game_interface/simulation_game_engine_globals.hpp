#pragma once

#include "cseries/cseries.hpp"

enum e_multiplayer_event_type
{
	_event_type_general = 0,
	_event_type_flavor,
	_event_type_slayer,
	_event_type_capture_the_flag,
	_event_type_oddball,
	_event_type_dummy,
	_event_type_king_of_the_hill,
	_event_type_vip,
	_event_type_territories,
	_event_type_juggernaut,
	_event_type_assault,
	_event_type_infection,
	_event_type_survival,
	_event_type_earn_wp,

	k_event_type_count
};

struct s_game_engine_event_data
{
	c_enum<e_multiplayer_event_type, long, _event_type_general, k_event_type_count> event_type;
	c_string_id type;
	long identifier;
	long audience_player_index;
	long cause_player_index;
	long cause_team_index;
	long effect_player_index;
	long effect_team_index;
	long event_quantity;
	short dummy_or_territories;
};

static_assert(sizeof(s_game_engine_event_data) == 0x28);

struct s_game_engine_request_boot_player_event_data
{
	long booted_player_index;
	long booting_player_index;
};
static_assert(sizeof(s_game_engine_request_boot_player_event_data) == 0x8);

