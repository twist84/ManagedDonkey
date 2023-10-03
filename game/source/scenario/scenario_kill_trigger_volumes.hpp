#pragma once

#include "cseries/cseries.hpp"
#include "scenario/scenario_trigger_volumes.hpp"

struct s_scenario_kill_trigger_volumes_state
{
	c_static_flags<MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO> disabled_kill_volumes;
	bool map_initialized;
};
static_assert(sizeof(s_scenario_kill_trigger_volumes_state) == 0x84);

