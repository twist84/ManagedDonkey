#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle :
	s_datum_header
{
	word_flags m_flags;
	long __unknown4;
	long m_particle_emitter_index;
	dword __unknownC;
	real __unknown10;
	byte __data14[0x18];
	real_point3d m_position;
	vector3d m_velocity;
	real __unknown44;
	real __unknown48;
	byte __data4C[0x24];
	vector3d __vector70;
};
static_assert(sizeof(c_particle) == 0x7C);

