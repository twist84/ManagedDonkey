#pragma once

#include "cseries/location.hpp"
#include "math/real_math.hpp"

struct s_structure_cluster_cubemap_info
{
	real_point3d cubemap_position;
	int16 scenario_cubemap_index;
	int16 cubemap_bitmap_index;
};
COMPILE_ASSERT(sizeof(s_structure_cluster_cubemap_info) == 0x10);

extern s_structure_cluster_cubemap_info* structure_cluster_try_and_get_cubemap_info(s_cluster_reference cluster_reference, int32 cubemap_index);

