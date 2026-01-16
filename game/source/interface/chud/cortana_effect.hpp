#pragma once

#include "cseries/cseries.hpp"

struct s_cortana_globals
{
	int32 cortana_effect_index;
	int32 cortana_effect_start_game_tick;
	int32 cortana_effect_tick;
	int32 cortana_effect_length;
};
COMPILE_ASSERT(sizeof(s_cortana_globals) == 0x10);

extern real32 __cdecl sub_A8E700();
extern real32 __cdecl cortana_effect_get_fov_scale();

