#pragma once

#include "cseries/cseries.hpp"

struct c_global_material_type
{
	c_global_material_type() :
		collision_flags(-1)
	{
	}

	word collision_flags;
};
static_assert(sizeof(c_global_material_type) == 0x2);

