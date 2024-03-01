#pragma once

#include "cseries/cseries.hpp"

struct s_cortana_globals
{
	long cortana_effect_definition_index;
	long __unknown4;
	long keyframe_index;
	long __unknownC;
};
static_assert(sizeof(s_cortana_globals) == 0x10);

extern real __cdecl sub_A8E700();
extern real __cdecl sub_ABEA20();

