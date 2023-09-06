#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_location : s_datum_header
{
	byte __data[0x5E];
};
static_assert(sizeof(c_particle_location) == 0x60);

