#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct dynamic_firing_set_datum : s_datum_header
{
	byte __data[0x582];
};
static_assert(sizeof(dynamic_firing_set_datum) == 0x584);

