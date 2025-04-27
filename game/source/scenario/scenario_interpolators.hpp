#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_interpolator_state
{
	real32 current_value;
	real32 start_value;
	real32 final_value;
	real32 __unknownC;  // reset time?
	real32 __unknown10; // start time?
	real32 __unknown14; // stop time?
	real32 __unknown18;
	uint8 flags;
};
static_assert(sizeof(s_scenario_interpolator_state) == 0x20);

struct s_scenario_interpolator_globals
{
	c_string_id name;
	c_static_array<s_scenario_interpolator_state, 16> states;
};
static_assert(sizeof(s_scenario_interpolator_globals) == 0x204);

extern void __cdecl scenario_interpolators_dispose();
extern void __cdecl scenario_interpolators_dispose_from_old_map();
extern void __cdecl scenario_interpolators_frame_update(real32 game_seconds_elapsed);
extern void __cdecl scenario_interpolators_initialize();
extern void __cdecl scenario_interpolators_initialize_for_new_map();

