#pragma once

#include "cseries/cseries.hpp"
#include "units/units.hpp"

struct c_simulation_world;
struct c_simulation_actor
{
	int32 __unknown0;
	int32 __unknown4;
	c_simulation_world* m_world;
	int32 __unknownC;
	uint32 m_time;
	unit_control_data m_actor_control;
};
static_assert(sizeof(c_simulation_actor) == 0x94);

