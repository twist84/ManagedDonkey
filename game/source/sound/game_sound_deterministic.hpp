#pragma once

#include "cseries/cseries.hpp"

struct s_game_sound_deterministic_globals
{
	byte __data[0x1300];
};
static_assert(sizeof(s_game_sound_deterministic_globals) == 0x1300);

extern void __cdecl game_sound_deterministic_update_timers();

