#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_emitter_gpu :
	s_datum_header
{
	struct s_row :
		s_datum_header
	{
		byte __data[0x12];
	};
	static_assert(sizeof(s_row) == 0x14);

	byte __data[0x16];
};
static_assert(sizeof(c_particle_emitter_gpu) == 0x18);

