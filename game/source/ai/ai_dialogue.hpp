#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct vocalization_records :
	s_datum_header
{
	byte __data[0x5A];
};
static_assert(sizeof(vocalization_records) == 0x5C);

struct vocalization_timers
{
	byte __data[0xFB8];
};
static_assert(sizeof(vocalization_timers) == 0xFB8);

