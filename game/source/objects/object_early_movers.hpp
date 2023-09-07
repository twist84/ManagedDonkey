#pragma once

#include "cseries/cseries.hpp"

struct s_object_early_mover
{
	byte __data[0x130];
};
static_assert(sizeof(s_object_early_mover) == 0x130);

struct s_object_early_movers_globals
{
	s_object_early_mover early_movers[32];
	long object_indices[32];
	long object_index_count;
	bool map_initialized;
};
static_assert(sizeof(s_object_early_movers_globals) == 0x2688);

struct s_object_early_mover_obb
{
	real_rectangle3d bounds;
	real_matrix4x3 matrix;
};
static_assert(sizeof(s_object_early_mover_obb) == 0x4C);

extern bool __cdecl object_early_mover_get_obb(long object_index_array_index, s_object_early_mover_obb* obb);
extern void __cdecl object_early_mover_render_debug();

