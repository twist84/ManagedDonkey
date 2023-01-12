#pragma once

#include "cseries/cseries.hpp"

enum e_statborg_entry
{
	_statborg_entry_score = 0,
	_statborg_entry_total_score,
	_statborg_entry_kills,
	_statborg_entry_deaths,
	_statborg_entry_suicides,
	_statborg_entry_betrayals,
	_statborg_entry_assists,
	_statborg_entry_unknown7,
	_statborg_entry_unknown8,
	_statborg_entry_unknown9,
	_statborg_entry_unknown10,
	_statborg_entry_medals,
	_statborg_entry_consecutive_kills,
	_statborg_entry_highest_spree,
	_statborg_entry_unknown14,
	_statborg_entry_unknown15,
	_statborg_entry_unknown16,
	_statborg_entry_unknown17,
	_statborg_entry_unknown18,
	_statborg_entry_unknown19,
	_statborg_entry_unknown20,
	_statborg_entry_unknown21,
	_statborg_entry_unknown22,
	_statborg_entry_unknown23,
	_statborg_entry_unknown24,
	_statborg_entry_unknown25
};

struct c_game_statborg
{
	short __unknown0[2];
	c_static_array<c_static_array<word, 26>, 16> players_statistics;
	c_static_array<c_static_array<word, 13>, 8>  teams_statistics;
	bool stats_paused;
	dword __unknown418;
	dword __unknown41C;
	bool __unknown420;
};
static_assert(sizeof(c_game_statborg) == 0x424);

