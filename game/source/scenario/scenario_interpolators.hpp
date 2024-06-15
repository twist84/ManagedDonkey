#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_interpolator_state
{
	real current_value;
	real start_value;
	real final_value;
	real __unknownC;  // reset time?
	real __unknown10; // start time?
	real __unknown14; // stop time?
	real __unknown18;
	byte_flags flags;
};
static_assert(sizeof(s_scenario_interpolator_state) == 0x20);

struct s_scenario_interpolator_globals
{
	c_string_id name;
	c_static_array<s_scenario_interpolator_state, 16> states;
};
static_assert(sizeof(s_scenario_interpolator_globals) == 0x204);

extern void __cdecl scenario_interpolators_frame_update(real game_seconds_elapsed);

