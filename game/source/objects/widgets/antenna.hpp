#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct antenna_datum :
	s_datum_header
{
	byte __data[0x62];
};
static_assert(sizeof(antenna_datum) == 0x64);

