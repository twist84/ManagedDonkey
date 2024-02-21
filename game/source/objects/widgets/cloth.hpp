#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct cloth_datum :
	s_datum_header
{
	byte __data[0x1702];
};
static_assert(sizeof(cloth_datum) == 0x1704);

