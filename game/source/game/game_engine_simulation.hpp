#pragma once

#include "cseries/cseries.hpp"

struct s_territories_event_data
{
	int16 territory_index;
};
COMPILE_ASSERT(sizeof(s_territories_event_data) == sizeof(int16));

struct s_objective_game_role_change_event_data
{
	int32 objective_game_role_index;
};
COMPILE_ASSERT(sizeof(s_objective_game_role_change_event_data) == sizeof(int32));

struct s_game_engine_event_data
{
	int32 event_type;
	int32 event;
	int32 identifier;
	int32 audience_player_index;
	int32 cause_player_index;
	int32 cause_team_index;
	int32 effect_player_index;
	int32 effect_team_index;
	int32 event_quantity;

	union
	{
		s_territories_event_data territories_event_data;
		s_objective_game_role_change_event_data objective_game_role_event_data;
	};
};
COMPILE_ASSERT(sizeof(s_game_engine_event_data) == 0x28);

enum e_game_engine_queued_event_flags
{
	_game_engine_queued_event_valid_bit = 0,
	_game_engine_queued_event_played_bit,
	_game_engine_queued_event_long_delay_bit,

	k_game_engine_queued_event_flags_count
};

struct s_game_engine_queued_event
{
	uns32 received_game_time;
	c_flags<e_game_engine_queued_event_flags, uns8, k_game_engine_queued_event_flags_count> flags;
	s_game_engine_event_data event_data;
};
COMPILE_ASSERT(sizeof(s_game_engine_queued_event) == 0x30);

