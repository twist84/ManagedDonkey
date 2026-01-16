#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_vehicle_trick_event_data
{
	int32 vehicle_trick_type;
};
COMPILE_ASSERT(sizeof(s_simulation_vehicle_trick_event_data) == 0x4);

