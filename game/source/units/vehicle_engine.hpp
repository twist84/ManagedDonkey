#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real cruising_to_loaded_transition;

	real __unknown4;
	real __unknown8;

	char gear;
	char desired_gear;

	byte __unknownE;

	byte clutch_in_ticks;

	byte __unknown10;
	byte __unknown11;
	byte __unknown12;
	byte __unknown13;
};
static_assert(sizeof(s_vehicle_engine) == 0x14);

extern real __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine);

