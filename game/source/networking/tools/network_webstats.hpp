#pragma once

#include "cseries/cseries.hpp"

struct s_game_matchmaking_options
{
	word hopper_identifier;
	byte xlast_index;
	bool is_ranked;
	bool team_game;
	byte : 8;
	wchar_t hopper_name[32];
	byte : 8;
	byte : 8;
	long draw_probability;
	real beta;
	real tau;
	long experience_base_increment;
	long experience_penalty_decrement;
};
static_assert(sizeof(s_game_matchmaking_options) == 0x5C);

extern void __cdecl network_webstats_dispose();
extern void __cdecl network_webstats_force_reset();
extern void __cdecl network_webstats_initialize();
extern void __cdecl network_webstats_update();

