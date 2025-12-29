#include "game/game_engine_scoring.hpp"

#include "cseries/cseries.hpp"

void __cdecl game_engine_scoring_build_score_list(s_game_engine_score_list* out_list, int32 scoring_type, bool used_for_display)
{
	INVOKE(0x00555020, game_engine_scoring_build_score_list, out_list, scoring_type, used_for_display);
}

//.text:00555C80 ; int32 __cdecl game_engine_scoring_get_player_compound_rank(int32, e_game_engine_scoring_type)
//.text:00555CD0 ; int32 __cdecl game_engine_scoring_get_player_compound_rank_from_score_list(int32, const s_game_engine_player_score_list*)
//.text:00555D10 ; int32 __cdecl game_engine_scoring_get_team_compound_rank(e_game_team, e_game_engine_scoring_type)
//.text:00555D60 ; int32 __cdecl game_engine_scoring_get_team_compound_rank_from_score_list(e_game_team, const s_game_engine_score_list*)
//.text:00555DA0 ; void __cdecl game_engine_scoring_initialize_for_new_round()
//.text:00555DD0 ; void __cdecl game_engine_scoring_notify_statborg_reset()
//.text:00555DF0 ; bool __cdecl game_engine_scoring_sort_players_compare_function(int32, int32, const void*)
//.text:00555E30 ; bool __cdecl game_engine_scoring_sort_teams_compare_function(e_game_team, e_game_team, const void*)
//.text:00555E60 ; void __cdecl game_engine_scoring_update_leaders()
//.text:00555EE0 ; void __cdecl game_engine_scoring_update_leaders_internal(const s_game_engine_score_list*, const s_game_engine_score_list*)
//.text:00556190 ; bool __cdecl game_engine_scoring_update_leaders_is_dirty()
//.text:005561B0 ; void __cdecl game_engine_scoring_update_leaders_make_dirty()

