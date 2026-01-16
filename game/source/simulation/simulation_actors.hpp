#pragma once

#include "cseries/cseries.hpp"
#include "units/units.hpp"

class c_simulation_world;
class c_simulation_actor
{
public:
	int32 m_simulation_actor_index;
	int32 m_unit_index;
	c_simulation_world* m_world;
	bool m_exists;
	int32 m_current_control_time;
	unit_control_data m_current_control;
};
COMPILE_ASSERT(sizeof(c_simulation_actor) == 0x94);

