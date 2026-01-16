#pragma once

#include "cseries/cseries.hpp"

struct s_game_sound_deterministic_globals
{
	byte __data[0x1300];
};
COMPILE_ASSERT(sizeof(s_game_sound_deterministic_globals) == 0x1300);

extern void __cdecl game_sound_deterministic_dispose();
extern void __cdecl game_sound_deterministic_dispose_from_old_map();
extern void __cdecl game_sound_deterministic_initialize();
extern void __cdecl game_sound_deterministic_initialize_for_new_map();
extern void __cdecl game_sound_deterministic_update_timers();

