#pragma once

#include "cseries/cseries.hpp"

struct s_object_placement_globals
{
	byte __data[0x320];
};
static_assert(sizeof(s_object_placement_globals) == 0x320);

