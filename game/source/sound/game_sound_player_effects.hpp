#pragma once

#include "cseries/cseries.hpp"

struct s_game_sound_player_effects_globals
{
	uint8 __data[0x28];
};
static_assert(sizeof(s_game_sound_player_effects_globals) == 0x28);

extern void __cdecl game_sound_player_effects_dispose();
extern void __cdecl game_sound_player_effects_dispose_from_old_map();
extern void __cdecl game_sound_player_effects_initialize();
extern void __cdecl game_sound_player_effects_initialize_for_new_map();

