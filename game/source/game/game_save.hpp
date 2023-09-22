#pragma once

#include "cseries/cseries.hpp"

struct s_game_save_globals
{
	byte __data[0x18];
};
static_assert(sizeof(s_game_save_globals) == 0x18);

