#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_emitter_gpu :
	s_datum_header
{
public:
	static void __cdecl shell_initialize();

	struct s_row :
		s_datum_header
	{
		byte __data[0x12];
	};
	static_assert(sizeof(s_row) == 0x14);

	byte __data2[0x2];
	long m_particle_system_index;
	byte __data8[0x4];
	long m_particle_emitter_index;
	long __unknown10;
	word m_particle_count;
	byte __data16[0x2];
};
static_assert(sizeof(c_particle_emitter_gpu) == 0x18);

