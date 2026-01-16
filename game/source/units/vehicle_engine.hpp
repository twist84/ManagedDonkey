#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real32 cruising_to_loaded_transition;
	real32 engine_angular_velocity;
	real32 torque_from_wheels;
	int8 gear;
	int8 desired_gear;
	uns8 shift_counter;
	uns8 clutch_in_ticks;
	uns8 ticks_in_gear;
	uns8 ticks_above_shift_threshold;
	uns8 ticks_below_shift_threshold;

	// padding?
	byte __unknown13;
};
COMPILE_ASSERT(sizeof(s_vehicle_engine) == 0x14);

extern real32 __cdecl vehicle_engine_get_rpm_function_scale(const s_vehicle_engine* engine);

