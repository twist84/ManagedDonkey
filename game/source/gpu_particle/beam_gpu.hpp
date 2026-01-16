#pragma once

#include "cseries/cseries.hpp"

class c_beam_gpu :
	public s_datum_header
{
public:
	static void __cdecl shell_initialize();

	struct s_row :
		s_datum_header
	{
		uns16 m_row;
		int32 m_prev;
		int32 m_next;
	};
	COMPILE_ASSERT(sizeof(s_row) == 0xC);

	int32 m_beam_index;
	int32 m_row_head;
	uns16 m_flags;
};
COMPILE_ASSERT(sizeof(c_beam_gpu) == 0x10);

