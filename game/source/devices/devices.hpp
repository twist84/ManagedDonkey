#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct device_group_datum :
	s_datum_header
{
	word_flags flags;
	real __unknown4; // desired value, actual value
	real __unknown8; // desired value, actual value
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);

extern long __cdecl device_group_get_from_scenario_index(short device_index);

