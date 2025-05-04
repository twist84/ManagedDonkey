#pragma once

#include "cseries/cseries.hpp"

struct c_sector_ref
{
	int16 structure_bsp_index;
	uns16 sector_index;
};
static_assert(sizeof(c_sector_ref) == 0x4);

struct pathfinding_data;

extern pathfinding_data const* __cdecl pathfinding_data_get(int16 structure_bsp_index);

void sector_link_render_debug(int32 link_index, pathfinding_data const* pf_data, real_argb_color const* color, bool a4);

