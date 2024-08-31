#pragma once

#include "cseries/cseries.hpp"
#include "game/game_statborg.hpp"

struct s_survival_mode_globals
{
	long lives;
	short set_index;
	short round_index;
	short wave_index;

	short __unknownA;

	real set_multiplier;
	real bonus_multiplier;
	short waves_per_round;
	short rounds_per_set;

	long __unknown18;
	long __unknown1C;

	long wave_in_set_index;
	dword primary_skulls;
	dword secondary_skulls;

	long __unknown2C;

	c_game_statborg statborg;

	struct
	{
		word __unknown0; // wave begin seconds?
		word __unknown2; // active skulls?
	} waves_in_set[225];

	byte __data7D8[0x8];
};
static_assert(sizeof(s_survival_mode_globals) == 0x7E0);

extern void __cdecl survival_mode_dispose();
extern void __cdecl survival_mode_dispose_from_old_map();
extern void __cdecl survival_mode_initialize();
extern void __cdecl survival_mode_initialize_for_new_map();

