#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_light_volume_gpu :
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

	long m_light_volume_index;
	long m_row_head;
	word_flags m_flags;
};
static_assert(sizeof(c_light_volume_gpu) == 0x10);

