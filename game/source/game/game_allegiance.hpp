#pragma once

#include "cseries/cseries.hpp"

struct s_game_allegiance_globals
{
	byte __data[0x184];
};
static_assert(sizeof(s_game_allegiance_globals) == 0x184);

bool __cdecl game_allegiance_betrayal_exists();

