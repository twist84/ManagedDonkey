#pragma once

#include "cseries/cseries.hpp"

struct s_player_effect_globals
{
	byte __data[0x3A0];
};
COMPILE_ASSERT(sizeof(s_player_effect_globals) == 0x3A0);


extern void __cdecl player_effect_dispose();
extern void __cdecl player_effect_dispose_from_old_map();
extern void __cdecl player_effect_frame_update(real32 game_seconds_elapsed);
extern void __cdecl player_effect_initialize();
extern void __cdecl player_effect_initialize_for_new_map();
extern void __cdecl player_effect_render(int32 user_index);
extern void __cdecl player_effect_update();

