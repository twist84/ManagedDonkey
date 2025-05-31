#pragma once

#include "cseries/cseries.hpp"

struct collision_bsp;
struct large_collision_bsp;
real_plane3d* bsp3d_get_plane_from_designator_internal(const collision_bsp* bsp, int16 index, real_plane3d* out_plane);
real_plane3d* bsp3d_get_plane_from_designator_internal(const large_collision_bsp* bsp, int32 index, real_plane3d* out_plane);

