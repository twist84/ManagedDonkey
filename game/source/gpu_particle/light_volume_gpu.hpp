#pragma once

#include "cseries/cseries.hpp"

class c_light_volume_gpu :
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
	static_assert(sizeof(s_row) == 0xC);

	int32 m_light_volume_index;
	int32 m_row_head;
	uns16 m_flags;
};
static_assert(sizeof(c_light_volume_gpu) == 0x10);

