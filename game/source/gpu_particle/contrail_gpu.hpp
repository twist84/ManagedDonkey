#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_contrail_gpu :
	s_datum_header
{
	struct s_row :
		s_datum_header
	{
		long __unknown4;
		long __unknown8;
		word __unknownC;
		byte m_used_count;
		real __unknown10;
	};
	static_assert(sizeof(s_row) == 0x14);

	long m_contrail_index;
	long m_row_index;
	word m_alive;
	word_flags m_flags;
};
static_assert(sizeof(c_contrail_gpu) == 0x10);

