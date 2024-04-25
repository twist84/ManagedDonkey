#pragma once

struct c_sector_ref
{
	short structure_bsp_index;
	short __unknown2;
};
static_assert(sizeof(c_sector_ref) == 0x4);

