#pragma once

#include "cseries/cseries.hpp"

struct s_game_engine_score_list
{
	int32 player_indices[16];
	e_game_team team_indices[8];

	char player_compound_ranks[16];
	char team_compound_ranks[8];

	int16 player_count;
	int16 team_count;
};
static_assert(sizeof(s_game_engine_score_list) == 0x7C);

