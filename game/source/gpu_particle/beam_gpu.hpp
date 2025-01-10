#pragma once

#include "cseries/cseries.hpp"

struct c_beam_gpu :
	s_datum_header
{
public:
	static void __cdecl shell_initialize();

	struct s_row :
		s_datum_header
	{
		word m_row;
		long m_prev;
		long m_next;
	};
	static_assert(sizeof(s_row) == 0xC);

	long m_beam_index;
	long m_row_head;
	word_flags m_flags;
};
static_assert(sizeof(c_beam_gpu) == 0x10);

