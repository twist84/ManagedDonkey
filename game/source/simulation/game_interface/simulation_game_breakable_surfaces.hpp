#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_breakable_surface_damage_event_data
{
	long instanced_geometry_instance_index;
	long seed_surface_index;
	s_cluster_reference cluster_reference;
	real_point3d position;
};
static_assert(sizeof(s_simulation_breakable_surface_damage_event_data) == 0x18);

