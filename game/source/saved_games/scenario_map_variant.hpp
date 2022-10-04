#pragma once

#include "cseries/cseries.hpp"

struct c_map_variant
{
public:
	c_map_variant();

private:
	byte __data[0xE090];
};
static_assert(sizeof(c_map_variant) == 0xE090);