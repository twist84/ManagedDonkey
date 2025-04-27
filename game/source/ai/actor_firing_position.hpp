#pragma once

struct firing_position_ref
{
	int16 firing_position_index;
	int16 source_index;
};
static_assert(sizeof(firing_position_ref) == 0x4);
