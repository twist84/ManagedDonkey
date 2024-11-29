#pragma once

struct firing_position_ref
{
	short firing_position_index;
	short source_index;
};
static_assert(sizeof(firing_position_ref) == 0x4);
