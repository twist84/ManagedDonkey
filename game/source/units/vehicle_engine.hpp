#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real32 cruising_to_loaded_transition;
	real32 engine_angular_velocity;
	real32 torque_from_wheels;
	char gear;
	char desired_gear;
	uint8 shift_counter;
	uint8 clutch_in_ticks;
	uint8 ticks_in_gear;
	uint8 ticks_above_shift_threshold;
	uint8 ticks_below_shift_threshold;

	// padding?
	uint8 __unknown13;
};
static_assert(sizeof(s_vehicle_engine) == 0x14);

extern real32 __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine);

