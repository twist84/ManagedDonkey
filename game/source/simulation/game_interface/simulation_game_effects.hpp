#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_effect_on_pos_event_data
{
	long effect_definition_index;
	real_point3d position;
};
static_assert(sizeof(s_simulation_effect_on_pos_event_data) == 0x10);

