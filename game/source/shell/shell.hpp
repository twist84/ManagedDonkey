#pragma once

enum e_gui_game_mode
{
	_ui_game_mode_none = -1,
	_ui_game_mode_campaign,
	_ui_game_mode_matchmaking,
	_ui_game_mode_multiplayer,
	_ui_game_mode_map_editor,
	_ui_game_mode_theater,
	_ui_game_mode_survival,

	k_ui_game_mode_count
};

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mainmenu,
	_game_mode_shared,

	k_game_mode_count
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_campaign_difficulty_levels_count
};

// e_game_skulls
enum e_campaign_skulls_primary
{
	_campaign_skull_iron = 0,
	_campaign_skull_black_eye,
	_campaign_skull_tough_luck,
	_campaign_skull_catch,
	_campaign_skull_fog,
	_campaign_skull_famine,
	_campaign_skull_thunderstorm,
	_campaign_skull_tilt,
	_campaign_skull_mythic,

	k_campaign_skulls_primary_count
};

enum e_campaign_skulls_secondary
{
	_campaign_skull_assassin = 0,
	_campaign_skull_blind,
	_campaign_skull_superman,
	_campaign_skull_birthday_party,
	_campaign_skull_daddy,
	_campaign_skull_third_person,
	_campaign_skull_directors_cut,

	k_campaign_skulls_secondary_count
};

// #TODO: find a home
enum e_game_simulation_type
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_synchronous_client,
	_game_simulation_synchronous_server,
	_game_simulation_distributed_client,
	_game_simulation_distributed_server,

	k_game_simulation_count
};

enum e_network_game_simulation_protocol
{
	_network_game_simulation_none = 0,
	_network_game_simulation_local,
	_network_game_simulation_sync_client,
	_network_game_simulation_sync_server,
	_network_game_simulation_dist_client,
	_network_game_simulation_dist_server,

	k_network_game_simulation_count
};

// #TODO: find a home
enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count
};

enum e_content_item_type
{

};

enum e_player_model_choice
{

};

enum e_controller_index
{
	_controller_index0 = 0,
	_controller_index1,
	_controller_index2,
	_controller_index3,

	k_number_of_controllers,

	k_any_controller = 0xFF,
	k_no_controller = 0xFFFFFFFF,
};

enum e_window_index
{
	_window_index0 = 0,
	_window_index1,
	_window_index2,
	_window_index3,
	_window_index4,

	k_number_of_windows
};

enum e_network_game_start_mode
{
};

enum e_network_join_queue_mode
{
	_network_join_closed_to_all_joins = 0,
	_network_join_open_to_join_squad,
	_network_join_queue_joins_to_group,
	_network_join_process_queued_group_joins,
	_network_join_queue_closed_while_in_match,

	k_network_join_queue_mode_count
};

enum e_join_local_state
{
	_join_local_state_none = 0,
	_join_local_state_start_join_squad,
	_join_local_state_joining_squad,
	_join_local_state_leaving_old_squad,
	_join_local_state_processing_join_to_us,

	k_join_local_state_count
};

enum e_join_remote_state
{
	_join_remote_state_none = 0,
	_join_remote_state_party_start_join,
	_join_remote_state_party_join_host,
	_join_remote_state_party_join_clients,
	_join_remote_state_party_leave_old_squad,
	_join_remote_state_party_join_complete,
	_join_remote_state_leave_group,

	k_join_remote_state_count
};

enum e_join_type
{
	_join_type_squad = 0,
	_join_type_group,

	k_join_type_count
};

enum e_networking_join_destination_squad
{
	k_join_destination_count = 1
};

enum e_transport_platform
{
	k_transport_platform_count = 2
};

enum e_network_join_refuse_reason
{
	_network_join_refuse_reason_none = 0,
	_network_join_refuse_reason_tried_to_join_self,
	_network_join_refuse_reason_could_not_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_not_found,
	_network_join_refuse_reason_privacy_mode,
	_network_join_refuse_reason_not_joinable,
	_network_join_refuse_reason_session_full,
	_network_join_refuse_reason_alpha_split_screen,
	_network_join_refuse_reason_session_disband,
	_network_join_refuse_reason_session_booted,
	_network_join_refuse_reason_address_invalid,
	_network_join_refuse_reason_address_failed,
	_network_join_refuse_reason_too_many_observers,
	_network_join_refuse_reason_aborted,
	_network_join_refuse_reason_abort_ignored,
	_network_join_refuse_reason_wrong_payload_type,
	_network_join_refuse_reason_no_reservation,
	_network_join_refuse_reason_in_matchmaking,
	_network_join_refuse_reason_player_count_zero,
	_network_join_refuse_reason_player_not_online_enabled,
	_network_join_refuse_reason_player_add_pending,
	_network_join_refuse_reason_player_add_failed,
	_network_join_refuse_reason_host_time_out,
	_network_join_refuse_reason_rejected_by_host,
	_network_join_refuse_reason_peer_version_too_low,
	_network_join_refuse_reason_host_version_too_low,
	_network_join_refuse_reason_holding_in_queue,
	_network_join_refuse_reason_film_in_progress,
	_network_join_refuse_reason_campaign_in_progress,
	_network_join_refuse_reason_user_content_not_allowed,
	_network_join_refuse_reason_survival_in_progress,
	_network_join_refuse_reason_executable_type_mismatch,

	k_network_join_refuse_reason_count,
};

enum e_life_cycle_join_result
{
	_life_cycle_join_result_none = 0,
	_life_cycle_join_result_join_in_progress,
	_life_cycle_join_result_join_failed_generic_error,
	_life_cycle_join_result_join_failed_to_find_session,
	_life_cycle_join_result_join_failed_not_enough_space,
	_life_cycle_join_result_join_failed_game_not_open,
	_life_cycle_join_result_join_failed_target_is_matchmaking,
	_life_cycle_join_result_join_failed_host_timed_out,
	_life_cycle_join_result_join_failed_peer_version_too_low,
	_life_cycle_join_result_join_failed_host_version_too_low,
	_life_cycle_join_result_join_failed_unable_to_connect_open_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_moderate_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_closed_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_open_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_moderate_nat,
	_life_cycle_join_result_join_failed_unable_to_connect_party_strict_nat,
	_life_cycle_join_result_join_failed_party_member_not_online_enabled,
	_life_cycle_join_result_join_failed_film_in_progress,
	_life_cycle_join_result_join_failed_campaign_in_progress,
	_life_cycle_join_result_join_failed_user_content_not_permitted,
	_life_cycle_join_result_join_failed_survival_in_progress,
	_life_cycle_join_result_join_failed_invalid_executable_type,

	k_life_cycle_join_result_count
};

enum e_network_session_class
{
	_network_session_class_none = 0,
	_network_session_class_offline,
	_network_session_class_system_link,
	_network_session_class_xbox_live,

	k_network_session_class_count
};

enum e_network_session_mode
{
	_network_session_mode_none = 0,
	_network_session_mode_idle,
	_network_session_mode_setup,
	_network_session_mode_in_game,
	_network_session_mode_end_game,
	_network_session_mode_post_game,
	_network_session_mode_matchmaking_start,
	_network_session_mode_matchmaking_searching,
	_network_session_mode_matchmaking_gathering,
	_network_session_mode_matchmaking_slave,
	_network_session_mode_matchmaking_disbanding,
	_network_session_mode_matchmaking_arbitrating,
	_network_session_mode_matchmaking_choosing_game,

	k_network_session_mode_count
};

enum e_matchmaking_search_preference
{
};

enum e_life_cycle_matchmaking_progress_type
{
	_life_cycle_matchmaking_progress_type_unknown0 = 0,
	_life_cycle_matchmaking_progress_type_searching_for_match,
	_life_cycle_matchmaking_progress_type_assembling_match,
	_life_cycle_matchmaking_progress_type_unknown3,
	_life_cycle_matchmaking_progress_type_configuring_match,
	_life_cycle_matchmaking_progress_type_post_match,

	k_life_cycle_matchmaking_progress_type_count
};

enum e_network_rough_quality
{
};

extern bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len);