#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_interpolator_state
{
	real32 value_current;
	real32 value_bounds[2];
	real32 time_current;
	real32 time_bounds[2];
	real32 phase;
	uns8 flags;
	byte pad[0x3];
};
COMPILE_ASSERT(sizeof(s_scenario_interpolator_state) == 0x20);

struct s_scenario_interpolator_globals
{
	c_string_id last_interpolator_name_id;
	s_scenario_interpolator_state states[16];
};
COMPILE_ASSERT(sizeof(s_scenario_interpolator_globals) == 0x204);

extern void __cdecl scenario_interpolators_dispose();
extern void __cdecl scenario_interpolators_dispose_from_old_map();
extern void __cdecl scenario_interpolators_frame_update(real32 game_seconds_elapsed);
extern void __cdecl scenario_interpolators_initialize();
extern void __cdecl scenario_interpolators_initialize_for_new_map();

