#pragma once

#include "cseries/cseries.hpp"

struct s_cortana_globals
{
	long cortana_effect_index;
	long cortana_effect_start_game_tick;
	long cortana_effect_tick;
	long cortana_effect_length;
};
static_assert(sizeof(s_cortana_globals) == 0x10);

extern real __cdecl sub_A8E700();
extern real __cdecl sub_ABEA20();

