#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct formation_datum : s_datum_header
{
	byte __data[0x292];
};
static_assert(sizeof(formation_datum) == 0x294);

