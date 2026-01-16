#pragma once

#include "cseries/cseries.hpp"

// there's a mapping in engine between `e_game_engine_type`, `e_statborg_entry` and `e_game_results_statistic`
enum e_statborg_entry
{
	_statborg_entry_in_round_score = 0,
	_statborg_entry_in_game_total_score,
	_statborg_entry_kills,
	_statborg_entry_deaths,
	_statborg_entry_suicides,
	_statborg_entry_betrayals,
	_statborg_entry_assists,
	_statborg_entry_rounds_won,

	_statborg_entry_infection_time_as_human,
	_statborg_entry_infection_infections,
	_statborg_entry_infection_zombie_kills,

	_statborg_entry_territories_owned = _statborg_entry_infection_time_as_human,
	_statborg_entry_territories_captures,
	_statborg_entry_territories_ousts,

	_statborg_entry_total_wp,
	_statborg_entry_kills_in_a_row,
	_statborg_entry_multiple_kills,
	_statborg_entry_total_kill_damage_dealt,

	_statborg_entry_unknown15,

	_statborg_entry_team_kills_in_a_row,

	_statborg_entry_unknown17,

	_statborg_entry_sniper_kills_in_a_row,
	_statborg_entry_shotgun_kills_in_a_row,
	_statborg_entry_sword_kills_in_a_row,
	_statborg_entry_vehicle_kills_in_a_row,
	_statborg_entry_team_kills_team,

	_statborg_entry_unknown23,

	_statborg_entry_kill_damage_type,

	k_statborg_indices_count
};

struct s_game_statborg_player
{
	c_static_array<uns16, k_statborg_indices_count> statistics;
	bool __unknown32;
};
COMPILE_ASSERT(sizeof(s_game_statborg_player) == 0x34);

struct s_game_statborg_team
{
	c_static_array<uns16, 12> statistics;
	bool finalised;
};
COMPILE_ASSERT(sizeof(s_game_statborg_team) == 0x1A);

class c_game_statborg
{
public:
	uns16 __unknown0;
	uns16 __unknown2;
	c_static_array<s_game_statborg_player, 16> player;
	c_static_array<s_game_statborg_team, 8>  team;
	bool stats_started;
	c_static_flags<64> update_mask;
	bool finalized;
};
COMPILE_ASSERT(sizeof(c_game_statborg) == 0x424);

