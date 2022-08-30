#pragma once

#include "cseries/cseries.hpp"

struct c_map_variant
{
	byte __data[0xE090];
};
static_assert(sizeof(c_map_variant) == 0xE090);