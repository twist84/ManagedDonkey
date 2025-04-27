#pragma once

#include "cseries/cseries.hpp"

struct c_particle_emitter_gpu :
	s_datum_header
{
public:
	static void __cdecl shell_initialize();

	struct s_row :
		s_datum_header
	{
		long m_prev;
		long m_next;
		uint16 m_row;
		uint8 m_used_count;
		real32 m_lifespan;
	};
	static_assert(sizeof(s_row) == 0x14);

	long m_system_index;
	long m_emitter_def_index;
	long m_emitter_index;
	long m_row_head;
	uint16 m_particle_count;
};
static_assert(sizeof(c_particle_emitter_gpu) == 0x18);

