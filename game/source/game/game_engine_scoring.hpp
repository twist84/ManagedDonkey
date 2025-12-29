#pragma once

enum
{
	_game_engine_scoring_type_in_round = 0,
	_game_engine_scoring_type_end_game,

	k_game_engine_scoring_type_count,
};

struct s_game_engine_score_list
{
	int32 player_indices[16];
	e_game_team team_indices[8];

	int8 player_compound_ranks[16];
	int8 team_compound_ranks[8];

	int16 player_count;
	int16 team_count;
};
static_assert(sizeof(s_game_engine_score_list) == 0x7C);

extern void __cdecl game_engine_scoring_build_score_list(s_game_engine_score_list* out_list, int32 scoring_type, bool used_for_display);

