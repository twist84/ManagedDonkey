#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_light_volume_gpu : s_datum_header
{
	struct s_row : s_datum_header
	{
		dword __unknown4;
		dword __unknown8;
	};
	static_assert(sizeof(s_row) == 0xC);

	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(c_light_volume_gpu) == 0x10);

