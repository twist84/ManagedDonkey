#pragma once

#include "cseries/cseries.hpp"

struct s_multiplayer_object_boundary_geometry_data
{
	long boundary_shape;
	long __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
	real __unknown14;
	real_matrix4x3 world_matrix;
	real boundary_geometry;
};
static_assert(sizeof(s_multiplayer_object_boundary_geometry_data) == 0x50);

