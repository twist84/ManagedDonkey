#pragma once

#include "cseries/cseries.hpp"

struct s_vehicle_engine
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	char current_gear;

	// ...
};

extern real __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine);

