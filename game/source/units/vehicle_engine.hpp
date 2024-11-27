#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real cruising_to_loaded_transition;
	real engine_angular_velocity;
	real torque_from_wheels;
	char gear;
	char desired_gear;
	byte shift_counter;
	byte clutch_in_ticks;
	byte ticks_in_gear;
	byte ticks_above_shift_threshold;
	byte ticks_below_shift_threshold;

	// padding?
	byte __unknown13;
};
static_assert(sizeof(s_vehicle_engine) == 0x14);

extern real __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine);

