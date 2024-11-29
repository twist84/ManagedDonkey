#pragma once

#include "cseries/cseries.hpp"

enum e_game_team;
struct s_game_engine_score_list
{
	long player_indices[16];
	e_game_team team_indices[8];

	char player_compound_ranks[16];
	char team_compound_ranks[8];

	short player_count;
	short team_count;
};
static_assert(sizeof(s_game_engine_score_list) == 0x7C);

