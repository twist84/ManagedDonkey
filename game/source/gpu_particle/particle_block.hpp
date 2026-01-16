#pragma once

#include "cseries/cseries.hpp"

class c_particle_emitter_gpu :
	public s_datum_header
{
public:
	static void __cdecl shell_initialize();

	struct s_row :
		s_datum_header
	{
		int32 m_prev;
		int32 m_next;
		uns16 m_row;
		uns8 m_used_count;
		real32 m_lifespan;
	};
	COMPILE_ASSERT(sizeof(s_row) == 0x14);

	int32 m_system_index;
	int32 m_emitter_def_index;
	int32 m_emitter_index;
	int32 m_row_head;
	uns16 m_particle_count;
};
COMPILE_ASSERT(sizeof(c_particle_emitter_gpu) == 0x18);

