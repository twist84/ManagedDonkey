#pragma once

#include "cseries/cseries.hpp"

struct s_player_effect_globals
{
	byte __data[0x3A0];
};
static_assert(sizeof(s_player_effect_globals) == 0x3A0);

extern void __cdecl player_effect_frame_update(real game_seconds_elapsed);
extern void __cdecl player_effect_update();

