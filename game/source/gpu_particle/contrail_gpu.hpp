#pragma once

#include "cseries/cseries.hpp"

struct c_contrail_gpu :
	s_datum_header
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
	static_assert(sizeof(s_row) == 0x14);

	int32 m_contrail_index;
	int32 m_row_head;
	uns16 m_profile_count;
	uns16 m_flags;
};
static_assert(sizeof(c_contrail_gpu) == 0x10);

