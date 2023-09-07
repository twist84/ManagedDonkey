#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct device_group_datum : s_datum_header
{
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);

