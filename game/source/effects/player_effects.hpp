#pragma once

#include "cseries/cseries.hpp"

struct s_player_effect_globals
{
	byte __data[0x3A0];
};
static_assert(sizeof(s_player_effect_globals) == 0x3A0);

enum e_output_user_index;

extern void __cdecl player_effect_dispose();
extern void __cdecl player_effect_dispose_from_old_map();
extern void __cdecl player_effect_frame_update(real game_seconds_elapsed);
extern void __cdecl player_effect_initialize();
extern void __cdecl player_effect_initialize_for_new_map();
extern void __cdecl player_effect_render(e_output_user_index output_user_index);
extern void __cdecl player_effect_update();

