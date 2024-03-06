#pragma once

#include "cseries/cseries.hpp"

struct s_campaign_metagame_runtime_globals
{
	enum e_metagame_flags
	{
		_initialized_bit = 1
	};

	byte __data0[0x1A14C];

	dword_flags flags;

	byte __data[0x8];
};
static_assert(sizeof(s_campaign_metagame_runtime_globals) == 0x1A158);

