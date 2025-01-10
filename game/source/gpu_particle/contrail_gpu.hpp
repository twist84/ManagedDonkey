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
		long m_prev;
		long m_next;
		word m_row;
		byte m_used_count;
		real m_lifespan;
	};
	static_assert(sizeof(s_row) == 0x14);

	long m_contrail_index;
	long m_row_head;
	word m_profile_count;
	word_flags m_flags;
};
static_assert(sizeof(c_contrail_gpu) == 0x10);

