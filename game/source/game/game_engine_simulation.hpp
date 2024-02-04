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

struct s_game_engine_event_data
{
	c_enum<e_multiplayer_event_type, long, _multiplayer_event_type_general, k_multiplayer_event_type_count> event_type;
	c_string_id type;
	long identifier;
	long audience_player_index;
	long cause_player_index;
	long cause_team_index;
	long effect_player_index;
	long effect_team_index;
	long event_quantity;

	union
	{
		short dummy;
		short territories;
	};
};
static_assert(sizeof(s_game_engine_event_data) == 0x28);

enum e_game_engine_queued_event_flags
{
	_game_engine_queued_event_valid_bit = 0,
	_game_engine_queued_event_played_bit,

	// game_engine_events_update
	// bool long_delay = flags.test(_game_engine_queued_event_unknown_bit2);
	// delay = game_seconds_to_ticks_real(long_delay ? 2.0f : 0.15f);
	_game_engine_queued_event_unknown_bit2,

	// any more?

	k_game_engine_queued_event_flags
};

struct s_game_engine_queued_event
{
	dword __time0;
	c_flags<e_game_engine_queued_event_flags, byte, k_game_engine_queued_event_flags> flags;
	s_game_engine_event_data event_data;
};
static_assert(sizeof(s_game_engine_queued_event) == 0x30);

