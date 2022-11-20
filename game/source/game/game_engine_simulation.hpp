#pragma once

#include "cseries/cseries.hpp"

struct s_game_engine_queued_event
{
	dword __time0;
	byte_flags flags;
	byte __data5[0x3];
	byte __data8[0x28];
};
static_assert(sizeof(s_game_engine_queued_event) == 0x30);

