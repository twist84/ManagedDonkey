#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_effect_on_pos_event_data
{
	int32 effect_definition_index;
	real_point3d position;
};
COMPILE_ASSERT(sizeof(s_simulation_effect_on_pos_event_data) == 0x10);

