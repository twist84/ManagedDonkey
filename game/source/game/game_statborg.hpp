#pragma once

#include "cseries/cseries.hpp"

struct c_game_statborg
{
	short __unknown0[2];
	short player_stats[16][26];
	short team_stats[8][13];
	bool stats_paused;
	byte __data415;
	byte __data416;
	byte __data417;
	dword dword418;
	dword dword41C;
	byte __data420[4];
};
static_assert(sizeof(c_game_statborg) == 0x424);

