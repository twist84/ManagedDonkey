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
		uint16 m_row;
		int32 m_prev;
		int32 m_next;
	};
	static_assert(sizeof(s_row) == 0xC);

	int32 m_beam_index;
	int32 m_row_head;
	uint16 m_flags;
};
static_assert(sizeof(c_beam_gpu) == 0x10);

