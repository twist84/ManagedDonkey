#pragma once

#include "cseries/cseries.hpp"

struct s_game_matchmaking_options
{
	uint16 hopper_identifier;
	uint8 xlast_index;
	bool is_ranked;
	bool team_game;
	uint8 : 8;
	wchar_t hopper_name[32];
	uint8 : 8;
	uint8 : 8;
	int32 draw_probability;
	real32 beta;
	real32 tau;
	int32 experience_base_increment;
	int32 experience_penalty_decrement;
};
static_assert(sizeof(s_game_matchmaking_options) == 0x5C);

extern void __cdecl network_webstats_dispose();
extern void __cdecl network_webstats_force_reset();
extern void __cdecl network_webstats_initialize();
extern void __cdecl network_webstats_update();

