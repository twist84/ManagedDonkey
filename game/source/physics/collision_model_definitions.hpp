#pragma once

#include "cseries/cseries.hpp"

enum e_pathfinding_sphere_flags
{
	_pathfinding_sphere_flag_remains_when_open_bit = 0,
	_pathfinding_sphere_flag_vehicle_only_bit,
	_pathfinding_sphere_flag_with_sectors_bit,

	k_pathfinding_sphere_flags
};

struct collision_model_pathfinding_sphere
{
	short node;
	c_flags<e_pathfinding_sphere_flags, short, k_pathfinding_sphere_flags> flags;
	real_point3d center;
	real radius;
};
static_assert(sizeof(collision_model_pathfinding_sphere) == 0x14);

