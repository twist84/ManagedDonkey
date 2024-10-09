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
	real __unknown14;
	real __unknown18;
	byte __data1C[0x10];
	real_point3d m_position;
	vector3d m_velocity;
	real __unknown44;
	real m_starting_rotation;
	real_argb_color m_tint;
	byte __data5C[0x10];
	real m_animation_rate;
	vector3d __vector70;
};
static_assert(sizeof(c_particle) == 0x7C);

