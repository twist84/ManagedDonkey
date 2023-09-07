#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_leaf_system : s_datum_header
{
	byte __data[0x94A];
};
static_assert(sizeof(c_leaf_system) == 0x94C);

