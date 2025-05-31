#include "networking/logic/network_leaderboard.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0226B508, s_network_leaderboard_globals, network_leaderboard_globals);

HOOK_DECLARE(0x004D60B0, network_leaderboard_get_write_status);
HOOK_DECLARE(0x004D62D0, network_leaderboard_player_stats_valid);

//.text:004D53A0 ; void __cdecl network_leaderboard_add_unused_parameter_to_stat_write(s_online_stat_write* stat_write)
//.text:004D53D0 ; void __cdecl network_leaderboard_add_users()
//.text:004D5510 ; void __cdecl network_leaderboard_build_global_arbitrated_stats(const s_network_leaderboard_game_results* game_results, const s_network_leaderboard_player_game_results* player_game_results, s_online_stat_write* stat_write)
//.text:004D5580 ; void __cdecl network_leaderboard_build_global_matchmade_stats(const s_network_leaderboard_game_results* game_results, s_online_stat_write* stat_write)
//.text:004D5650 ; void __cdecl network_leaderboard_build_global_unarbitrated_stats(const s_network_leaderboard_game_results* game_results, const s_network_leaderboard_player_game_results* player_game_results, s_online_stat_write* stat_write)
//.text:004D56C0 ; void __cdecl network_leaderboard_build_hopper_stats(const s_network_leaderboard_game_results* game_results, const s_network_leaderboard_player_game_results* player_game_results, s_online_stat_write* stat_write)
//.text:004D5730 ; void __cdecl network_leaderboard_build_leaderboard_game_results_from_game_results(const c_game_results* game_results, s_network_leaderboard_game_results* game_results_out)
//.text:004D5830 ; void __cdecl network_leaderboard_build_leaderboard_game_results_from_initial(uns16 hopper_identifier, bool is_ranked, bool team_game, int32 draw_probability, real32 beta, real32 tau, int32 experience_penalty_decrement, uns32 start_time, s_network_leaderboard_game_results* game_results)
//.text:004D58A0 ; void __cdecl network_leaderboard_build_leaderboard_player_game_results_from_game_results(int32 player_index, const s_network_leaderboard_game_results* leaderboard_game_results, const c_game_results* game_results, int32 relative_score, s_network_leaderboard_player_game_results* player_game_results_out)
//.text:004D59B0 ; void __cdecl network_leaderboard_build_leaderboard_player_game_results_from_initial(const game_player_options* player_options, s_network_leaderboard_player_game_results* player_game_results)
//.text:004D5A10 ; void __cdecl network_leaderboard_build_property_from_statistic_and_results(e_leaderboard_statistic leaderboard_statistic, const s_network_leaderboard_game_results* game_results, const s_network_leaderboard_player_game_results* player_game_results, s_online_property* property)
//.text:004D5B90 ; void __cdecl network_leaderboard_build_query_from_statistic_list(s_online_stat_query* stat_query, e_online_leaderboard_id leaderboard_id, int32 leaderboard_statistics_count, const e_leaderboard_statistic* leaderboard_statistics)
//.text:004D5C40 ; void __cdecl network_leaderboard_build_skill_stats(const s_network_leaderboard_player_game_results* player_game_results, s_online_stat_write* stat_write)
//.text:004D5C90 ; void __cdecl network_leaderboard_clear_query_stats(e_controller_index controller_index)
//.text:004D5CC0 ; void __cdecl network_leaderboard_clear_user_stats(e_controller_index controller_index, e_network_leaderboard_player_stats_clear_type stats_clear_type)
//.text:004D5D70 ; bool __cdecl network_leaderboard_controller_get_highest_skill(e_controller_index, int32*)

void __cdecl network_leaderboard_destory()
{
	INVOKE(0x004D5DF0, network_leaderboard_destory);
}

//.text:004D5E10 ; e_online_leaderboard_column_id __cdecl network_leaderboard_get_column_id_by_statistic(e_leaderboard_statistic leaderboard_statistic, e_online_leaderboard_id leaderboard_id)
//.text:004D5E80 ; e_online_property_id __cdecl network_leaderboard_get_property_id_by_statistic(e_leaderboard_statistic leaderboard_statistic)
//.text:004D5F50 ; e_hopper_load_status __cdecl network_leaderboard_get_query_status()
//.text:004D5FA0 ; void __cdecl network_leaderboard_get_results(e_controller_index controller_index)
//.text:004D5FF0 ; e_leaderboard_statistic __cdecl network_leaderboard_get_statistic_by_column_id(e_online_leaderboard_column_id column_id, e_online_leaderboard_id leaderboard_id)
//.text:004D6070 ; int32 network_leaderboard_get_user_by_xuid(uns64 const)

e_network_leaderboard_write_status __cdecl network_leaderboard_get_write_status()
{
	//return INVOKE(0x004D60B0, network_leaderboard_get_write_status);

	return _network_leaderboard_write_completed;
}

//.text:004D6130 ; bool __cdecl network_leaderboard_id_valid_for_query(e_online_leaderboard_id leaderboard_id)

bool __cdecl network_leaderboard_initialize()
{
	return INVOKE(0x004D6170, network_leaderboard_initialize);
}

//.text:004D6200 ; void __cdecl network_leaderboard_log_global_stat_write(const s_online_stat_write*, bool)
//.text:004D6210 ; void __cdecl network_leaderboard_log_player_stat_write(int32, const s_network_leaderboard_player_game_results*, const s_online_stat_write*, bool)
//.text:004D6220 ; void __cdecl network_leaderboard_log_user_stats_read(int32, const c_static_array<s_network_leaderboard_statistic, 21>*)
//.text:004D6230 ; void __cdecl network_leaderboard_notify_user_stats_updated(int32 user_index)

bool __cdecl network_leaderboard_player_stats_valid(const s_network_session_player* player)
{
	//return INVOKE(0x004D62D0, network_leaderboard_player_stats_valid, player);

	return true;
}

//.text:004D62E0 ; void __cdecl network_leaderboard_refresh()
//.text:004D6300 ; void __cdecl network_leaderboard_remove_users()
//.text:004D6390 ; void __cdecl network_leaderboard_submit_query_for_user(e_controller_index controller_index)

void __cdecl network_leaderboard_update()
{
	INVOKE(0x004D64B0, network_leaderboard_update);
}

//.text:004D64E0 ; void __cdecl network_leaderboard_update_highest_skill()
//.text:004D6720 ; void __cdecl network_leaderboard_update_leaderboards()
//.text:004D6820 ; void __cdecl network_leaderboard_update_queries()
//.text:004D6840 ; void __cdecl network_leaderboard_update_queries_in_progress()
//.text:004D68B0 ; void __cdecl network_leaderboard_update_queries_initiate()
//.text:004D6960 ; void __cdecl network_leaderboard_update_queries_refresh()
//.text:004D6A10 ; void __cdecl network_leaderboard_update_queries_request()
//.text:004D6A80 ; void __cdecl network_leaderboard_update_users()
//.text:004D6A90 ; void __cdecl network_leaderboard_update_writes()
//.text:004D6AA0 ; void __cdecl network_leaderboard_update_writes_in_progress()
//.text:004D6AF0 ; void __cdecl network_leaderboard_user_query_failed(e_controller_index controller_index)
//.text:004D6B30 ; void __cdecl network_leaderboard_write_initial_stats(int32 managed_session_index, uns16 hopper_identifier, bool is_ranked, bool team_game, int32 draw_probability, real32 beta, real32 tau, int32 experience_penalty_decrement, uns32 selection_time, const game_player_options* player_options)
//.text:004D6CB0 ; void __cdecl network_leaderboard_write_stats(int32 managed_session_index, const c_game_results* game_results)
//.text:004D6DC0 ; void __cdecl network_leaderboard_write_stats_internal(int32 managed_session_index, const s_network_leaderboard_game_results* game_results, const s_network_leaderboard_player_game_results* player_game_results, bool a4)

