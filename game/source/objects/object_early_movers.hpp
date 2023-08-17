#pragma once

#include "cseries/cseries.hpp"

struct s_object_early_mover_obb
{
	real_rectangle3d bounds;
	real_matrix4x3 matrix;
};
static_assert(sizeof(s_object_early_mover_obb) == 0x4C);

extern bool __cdecl object_early_mover_get_obb(long object_index_array_index, s_object_early_mover_obb* obb);
extern void __cdecl object_early_mover_render_debug();

