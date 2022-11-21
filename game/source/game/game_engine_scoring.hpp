#pragma once

#include "cseries/cseries.hpp"

struct s_game_engine_score_list
{
	c_static_array<long, 16> player_indices;
	c_static_array<long, 8> team_indices;

	c_static_array<byte, 16> player_compound_ranks;
	c_static_array<byte, 8> team_compound_ranks;

	// VALID_COUNT(list->player_count, NUMBEROF(list->player_compound_ranks))
	short player_count;

	// VALID_COUNT(list->team_count, NUMBEROF(list->team_compound_ranks))
	short team_count;
};
static_assert(sizeof(s_game_engine_score_list) == 0x7C);

