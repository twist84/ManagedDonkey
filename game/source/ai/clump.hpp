#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct clump_datum :
	s_datum_header
{
	byte __data[0x106];
};
static_assert(sizeof(clump_datum) == 0x108);

