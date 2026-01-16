#pragma once

#include "cseries/cseries.hpp"
#include "game/game_statborg.hpp"

struct s_survival_mode_globals
{
	int32 lives;
	int16 set_index;
	int16 round_index;
	int16 wave_index;

	int16 __unknownA;

	real32 set_multiplier;
	real32 bonus_multiplier;
	int16 waves_per_round;
	int16 rounds_per_set;

	int32 __unknown18;
	int32 __unknown1C;

	int32 wave_in_set_index;
	uns32 primary_skulls;
	uns32 secondary_skulls;

	int32 __unknown2C;

	c_game_statborg statborg;

	struct
	{
		uns16 __unknown0; // wave begin seconds?
		uns16 __unknown2; // active skulls?
	} waves_in_set[225];

	byte __data7D8[0x8];
};
COMPILE_ASSERT(sizeof(s_survival_mode_globals) == 0x7E0);

extern void __cdecl survival_mode_dispose();
extern void __cdecl survival_mode_dispose_from_old_map();
extern void __cdecl survival_mode_initialize();
extern void __cdecl survival_mode_initialize_for_new_map();

