#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_impact_globals
{
	byte __unknown0[0x8C];
};
static_assert(sizeof(s_impact_globals) == 0x8C);

struct c_impact :
	s_datum_header
{
	byte __unknown2[0xB2];
};
static_assert(sizeof(c_impact) == 0xB4);

struct impact_array_datum :
	s_datum_header
{
	short impact_indexes_count;
	long impact_indexes[33];
};
static_assert(sizeof(impact_array_datum) == 0x88);

extern void __cdecl impacts_update();

