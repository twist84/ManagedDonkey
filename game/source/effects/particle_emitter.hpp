#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_emitter :
	s_datum_header
{
	byte __data[0x8E];
};
static_assert(sizeof(c_particle_emitter) == 0x90);

