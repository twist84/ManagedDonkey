#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_breakable_surface_globals
{
	byte __data[0x3CE18];
};
static_assert(sizeof(s_breakable_surface_globals) == 0x3CE18);

struct s_breakable_surface_set_broken_event :
	s_datum_header
{
	byte __data[0x462];
};
static_assert(sizeof(s_breakable_surface_set_broken_event) == 0x464);

