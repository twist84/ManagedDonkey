#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real cruising_to_loaded_transition;

	// wheel_ground_speed = __unknown4 * engine_definition->gears[gear].gear_ratio
	real __unknown4; // velocity?

	real __unknown8;

	char gear;
	char desired_gear;

	// some calculation with `__unknownE` gives us `rpm_function_scale`
	byte __unknownE;

	byte clutch_in_ticks;

	byte __unknown10;

	byte time_to_upshift_ticks;
	byte time_to_downshift_ticks;

	byte __unknown13;
};
static_assert(sizeof(s_vehicle_engine) == 0x14);

extern real __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine);

