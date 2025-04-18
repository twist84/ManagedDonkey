#pragma once

#define BOT_CLIENT(true_false) if (game_is_bot_client() == true_false)

enum
{
	k_maximum_scenario_zone_set_count = 48,
};

enum
{
	_campaign_player0 = 0,
	_campaign_player1,
	_campaign_player2,
	_campaign_player3,

	k_maximum_campaign_players,


	k_maximum_multiplayer_players = 16,

	k_maximum_game_tick_rate = 60,
	k_game_tick_rate_bits = 6,

	k_maximum_players = 16,
	k_maximum_player_index = 15,

	k_player_index_bits = 4,
	k_player_count_bits = 5,

	k_maximum_machines = 17,
	k_machine_count_bits = 5,
	k_maximum_machine_index = 15,
	k_machine_index_bits = 4,

	k_maximum_rounds = 32,
	k_round_count_bits = 6,
	k_maximum_round_index = 31,
	k_round_index_bits = 5,
};

enum e_campaign_id
{
	_campaign_id_default = 1,
	_campaign_id_none = -1,
};

enum e_map_id
{
	_map_id_first = 3005,
	_map_id_tutorial = 3010,
	_map_id_first_playable = 3010,

	_map_id_none = -1,
	_map_id_use_scenario_path = -2,

	_map_id_mainmenu = 0x10231971,
};

enum e_gui_campaign_difficulty_setup_mode
{
	_campaign_difficulty_setup_mode_start_new_campaign = 0,
	_campaign_difficulty_setup_mode_replay_level,
	_campaign_difficulty_setup_mode_lobby
};

enum e_gui_campaign_level_setup_mode
{
	_campaign_level_setup_mode_replay_level = 0,
	_campaign_level_setup_mode_lobby
};

enum e_team_scoring_method
{
	_team_scoring_method_first = 0,

	_team_scoring_method_sum = _team_scoring_method_first,
	_team_scoring_method_minimum,
	_team_scoring_method_maximum,
	_team_scoring_method_custom,

	_team_scoring_method_last = _team_scoring_method_custom,

	k_number_of_team_scoring_methods,
	k_team_scoring_method_default = _team_scoring_method_sum,
};

enum e_game_engine_type
{
	_game_engine_type_none = 0,
	_game_engine_type_ctf,
	_game_engine_type_slayer,
	_game_engine_type_oddball,
	_game_engine_type_king,
	_game_engine_type_sandbox,
	_game_engine_type_vip,
	_game_engine_type_juggernaut,
	_game_engine_type_territories,
	_game_engine_type_assault,
	_game_engine_type_infection,

	k_game_engine_type_count,
	k_game_engine_type_default = _game_engine_type_none
};

enum e_simulation_world_type
{
	_simulation_world_type_none = 0,
	_simulation_world_type_local,
	_simulation_world_type_local_playback,
	_simulation_world_type_synchronous_game_server,
	_simulation_world_type_synchronous_game_client,
	_simulation_world_type_synchronous_film_server,
	_simulation_world_type_synchronous_film_client,
	_simulation_world_type_distributed_server,
	_simulation_world_type_distributed_client,

	k_simulation_world_type_count
};

enum e_simulation_world_state
{
	_simulation_world_state_none = 0,
	_simulation_world_state_dead,
	_simulation_world_state_disconnected,
	_simulation_world_state_joining,
	_simulation_world_state_active,
	_simulation_world_state_handoff,
	_simulation_world_state_leaving,

	k_simulation_world_state_count
};

enum e_update_queue_state
{
	_update_queue_state_normal = 0,
	_update_queue_state_peaking,
	_update_queue_state_throttling,
	_update_queue_state_cooling,

	k_update_queue_state_count
};

enum e_simulation_update_flags
{
	_simulation_update_simulation_in_progress_bit = 0,
	_simulation_update_flush_gamestate_bit,
	_simulation_update_gamestate_flushed_outside_game_tick_bit,
	_simulation_update_game_simulation_queue_requires_application_bit,

	k_simulation_update_flags_count
};

enum e_simulation_update_metadata_flags
{
	_simulation_update_from_local_simulation_bit = 0,
	_simulation_update_from_synchronous_update_bit,
	_simulation_update_from_saved_film_bit,

	k_simulation_update_metadata_flags_count
};

enum e_update_queue_node
{
	_update_queue_node_update = 0,
	_update_queue_node_playback_event,

	k_update_queue_node_count,
	k_update_queue_node_invalid = -1
};

enum e_simulation_playback_event
{
	_simulation_playback_event_revert = 0,

	k_simulation_playback_event_count
};

enum e_scenario_camera_type
{
	_scenario_camera_type_target_relative = 0,
	_scenario_camera_type_ignore_target_orientation,
	_scenario_camera_type_dolly,
	_scenario_camera_type_ignore_target_updates,

	k_number_of_scenario_camera_types,
	_scenario_camera_type_default = _scenario_camera_type_target_relative
};

enum e_level_flags
{
	_level_from_dlc_bit = 0,
	_level_unlockable_bit,
	_level_visible_in_ui,
	_level_allows_saved_films,
	_level_is_main_menu_bit,
	_level_is_campaign_bit,
	_level_is_multiplayer_bit,
	_level_in_dlc_directory_bit,
	_level_in_test_directory_bit,
	_level_in_temp_directory_bit,

	k_number_of_level_flags
};

enum e_text_justification
{
	_text_justification_left = 0,
	_text_justification_right,
	_text_justification_center,

	k_text_justification_count
};

enum e_text_vertical_justification
{
	_text_vertical_justification_default = 0,
	_text_vertical_justification_top,
	_text_vertical_justification_center,
	_text_vertical_justification_bottom,

	k_text_vertical_justification_count
};

enum e_text_style
{
	_text_style_plain = -1,
	_text_style_bold,
	_text_style_italic,
	_text_style_condense,
	_text_style_underline,

	k_text_style_count
};

enum e_font_id
{
	_font_id_fallback = -2,
	_invalid_font = -1,

	_terminal_font = 0,
	_body_text_font,
	_title_font,
	_super_large_font,
	_large_body_text_font,
	_split_screen_hud_message_font,
	_full_screen_hud_message_font,
	_english_body_text_font,
	_hud_number_font,
	_subtitle_font,
	_main_menu_font,

	k_public_font_id_count
};

enum e_session_creation_flags
{
	_session_creation_unknown_bit0 = 0,
	_session_creation_unknown_bit1,
	_session_creation_unknown_bit2,
	_session_creation_unknown_bit3,

	k_number_of_session_creation_flags
};

enum
{
	k_number_of_users = 4
};

enum e_network_game_type
{
	_network_game_type_none = 0,
	_network_game_type_custom_game,
	_network_game_type_matchmaking,
	_network_game_type_film,

	k_network_game_type_count
};

enum e_desired_multiplayer_mode
{
	_desired_multiplayer_mode_none = -1,
	_desired_multiplayer_mode_campaign,
	_desired_multiplayer_mode_custom_game,
	_desired_multiplayer_mode_matchmaking,
	_desired_multiplayer_mode_film,

	k_number_of_desired_multiplayer_modes
};

enum e_session_game_mode
{
	_session_game_mode_none = 0,
	_session_game_mode_matchmaking,
	_session_game_mode_pregame,
	_session_game_mode_ingame,
	_session_game_mode_postgame,

	k_session_game_mode_count
};

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
	_game_mode_ui_shell,
	_game_mode_tool,

	k_game_mode_count
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_number_of_campaign_difficulty_levels
};

// e_game_skulls
enum e_primary_skulls
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

	k_number_of_primary_skulls
};

enum e_secondary_skulls
{
	_campaign_skull_assassin = 0,
	_campaign_skull_blind,
	_campaign_skull_superman,
	_campaign_skull_birthday_party,
	_campaign_skull_daddy,
	_campaign_skull_third_person,
	_campaign_skull_directors_cut,

	k_number_of_secondary_skulls
};

// $TODO: find a home
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

enum e_simulation_abort_reason
{
	_simulation_abort_reason_exiting_in_game = 0,
	_simulation_abort_reason_exiting_in_match,
	_simulation_abort_reason_failed_to_find_next_map,
	_simulation_abort_reason_playing_film,
	_simulation_abort_reason_reset_failed,
	_simulation_abort_reason_fatal_error,
	_simulation_abort_reason_lost_connection,
	_simulation_abort_reason_out_of_sync,
	_simulation_abort_reason_film_ended,
	_simulation_abort_reason_film_playback_error,

	k_simulation_abort_reason_count
};

enum e_simulation_view_reason
{
	_simulation_view_reason_none = 0,
	_simulation_view_reason_disconnected,
	_simulation_view_reason_out_of_sync,
	_simulation_view_reason_failed_to_join,
	_simulation_view_reason_blocking,
	_simulation_view_reason_catchup_fail,
	_simulation_view_reason_ended,
	_simulation_view_reason_mode_error,
	_simulation_view_reason_player_error,
	_simulation_view_reason_replication_entity,
	_simulation_view_reason_replication_event,
	_simulation_view_reason_replication_game_results,
	_simulation_view_reason_no_longer_authority,
	_simulation_view_reason_signature_invalid,

	k_simulation_view_reason_count
};

enum e_simulation_view_type
{
	_simulation_view_type_none = 0,
	_simulation_view_type_synchronous_to_remote_authority,
	_simulation_view_type_synchronous_to_remote_client,
	_simulation_view_type_distributed_to_remote_authority,
	_simulation_view_type_distributed_to_remote_client,

	k_simulation_view_type_count
};

// $TODO: find a home
enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_film,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count
};

enum e_content_item_type
{

};

enum e_player_model_choice
{
	_player_model_choice_spartan,
	_player_model_choice_elite,

	k_number_of_player_model_choices
};

#define VALID_CONTROLLER(CONTROLLER) ((CONTROLLER) >= _controller0 && (CONTROLLER) < k_number_of_controllers)

enum e_controller_index
{
	_controller0 = 0,
	_controller1,
	_controller2,
	_controller3,

	k_number_of_controllers,

	k_any_controller = 0xFF,
	k_no_controller = -1,
};

static inline e_controller_index operator-(e_controller_index& value, int increment)
{
	return e_controller_index((int)value - increment);
}

static inline e_controller_index operator+(e_controller_index& value, int increment)
{
	return e_controller_index((int)value + increment);
}

static inline e_controller_index operator-=(e_controller_index& value, int increment)
{
	return value = e_controller_index((int)value - increment);
}

static inline e_controller_index operator+=(e_controller_index& value, int increment)
{
	return value = e_controller_index((int)value + increment);
}

static inline e_controller_index operator--(e_controller_index& value, int increment)
{
	return value = e_controller_index((int)value - 1);
}

static inline e_controller_index operator++(e_controller_index& value, int increment)
{
	return value = e_controller_index((int)value + 1);
}

enum e_window_index
{
	_player_window_first = 0,

	_window0 = 0,
	_window1,
	_window2,
	_window3,
	k_number_of_player_windows,

	_console_window = 4,
	k_number_of_render_windows,

	k_no_window = -1,
	k_any_window = 0xFF,
};

enum e_network_game_start_mode
{
	_network_game_start_mode_campaign = 0,
	_network_game_start_mode_custom_game,
	_network_game_start_mode_matchmaking,
	_network_game_start_mode_film,

	k_number_of_network_game_start_modes
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
	_join_destination_target = 0,
	_join_destination_group,

	k_join_destination_count
};

enum e_transport_platform
{
	_transport_platform_xenon = 0,
	_transport_platform_windows,

	k_transport_platform_count
};

enum e_network_channel_closure_reason
{
	_network_channel_reason_none = 0,
	_network_channel_reason_link_destroyed,
	_network_channel_reason_link_refused_listening,
	_network_channel_reason_channel_deleted,
	_network_channel_reason_connect_request_timeout,
	_network_channel_reason_connect_refuse,
	_network_channel_reason_connect_reinitiated,
	_network_channel_reason_establish_timeout,
	_network_channel_reason_address_change,
	_network_channel_reason_destination_unreachable,
	_network_channel_reason_remote_closure,
	_network_channel_reason_connection_overflowed,
	_network_channel_reason_message_queue_overflowed,
	_network_channel_reason_observer_security_lost,
	_network_channel_reason_observer_owners_released,
	_network_channel_reason_observer_fatal_refusal,
	_network_channel_reason_observer_timeout,
	_network_channel_reason_observer_reset,
	_network_channel_reason_observer_reset_security,
	_network_channel_reason_test,

	k_network_channel_reason_count
};

enum e_network_join_refuse_reason
{
	_network_join_refuse_reason_none = 0,
	_network_join_refuse_reason_tried_to_join_self,
	_network_join_refuse_reason_unable_to_connect,
	_network_join_refuse_reason_join_timed_out,
	_network_join_refuse_reason_session_not_found,
	_network_join_refuse_reason_session_privacy,
	_network_join_refuse_reason_session_not_joinable,
	_network_join_refuse_reason_session_full,
	_network_join_refuse_reason_no_split_screen_on_alpha,
	_network_join_refuse_reason_session_disbanded,
	_network_join_refuse_reason_session_boot,
	_network_join_refuse_reason_address_invalid,
	_network_join_refuse_reason_address_decode_failed,
	_network_join_refuse_reason_too_many_observers,
	_network_join_refuse_reason_abort_successful,
	_network_join_refuse_reason_abort_ignored,
	_network_join_refuse_reason_wrong_payload_type,
	_network_join_refuse_reason_no_reservation,
	_network_join_refuse_reason_in_matchmaking,
	_network_join_refuse_reason_player_count_zero,
	_network_join_refuse_reason_player_not_online_enabled,
	_network_join_refuse_reason_player_add_pending,
	_network_join_refuse_reason_player_add_failed,
	_network_join_refuse_reason_host_timed_out,
	_network_join_refuse_reason_rejected,
	_network_join_refuse_reason_peer_version_too_low,
	_network_join_refuse_reason_host_version_too_low,
	_network_join_refuse_reason_holding_in_queue,
	_network_join_refuse_reason_film_in_progress,
	_network_join_refuse_reason_campaign_in_progress,
	_network_join_refuse_reason_user_content_not_permitted,
	_network_join_refuse_reason_survival_in_progress,
	_network_join_refuse_reason_executable_type_mismatch,

	k_network_join_refuse_reason_count
};

enum e_network_session_boot_reason
{
	_network_session_boot_user_request_in_game = 0,
	_network_session_boot_join_timeout,
	_network_session_boot_failed_to_load_map,
	_network_session_boot_blocking_stats_write,
	_network_session_boot_from_ui,
	_network_session_boot_quick_disconnect,
	_network_session_boot_for_join,
	_network_session_boot_for_join_abort,
	_network_session_boot_simulation_aborted,
	_network_session_boot_unknown9,
	_network_session_boot_unknown10,
	_network_session_boot_unknown11,
	_network_session_boot_unknown12,

	k_network_session_boot_reason_count
};

enum e_life_cycle_join_result
{
	_life_cycle_join_result_none = 0,
	_life_cycle_join_result_in_progress,
	_life_cycle_join_result_generic_error,
	_life_cycle_join_result_failed_to_find_session,
	_life_cycle_join_result_not_enough_space,
	_life_cycle_join_result_game_not_open,
	_life_cycle_join_result_target_is_matchmaking,
	_life_cycle_join_result_host_timed_out,
	_life_cycle_join_result_peer_version_too_low,
	_life_cycle_join_result_host_version_too_low,
	_life_cycle_join_result_unable_to_connect_open_nat,
	_life_cycle_join_result_unable_to_connect_moderate_nat,
	_life_cycle_join_result_unable_to_connect_strict_nat,
	_life_cycle_join_result_unable_to_connect_party_open_nat,
	_life_cycle_join_result_unable_to_connect_party_moderate_nat,
	_life_cycle_join_result_unable_to_connect_party_strict_nat,
	_life_cycle_join_result_player_not_online_enabled,
	_life_cycle_join_result_target_has_film_in_progress,
	_life_cycle_join_result_target_has_campaign,
	_life_cycle_join_result_user_content_not_permitted,
	_life_cycle_join_result_survival_in_progress,
	_life_cycle_join_result_invalid_executable_type,

	k_life_cycle_join_result_count
};

enum e_network_session_type
{
	_network_session_type_none = 0,
	_network_session_type_squad,
	_network_session_type_group,

	k_network_session_type_count
};

enum e_network_session_class
{
	_network_session_class_offline = 0,
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
	_matchmaking_search_preference_none = 0,
	_matchmaking_search_preference_skill,
	_matchmaking_search_preference_good_connection,
	_matchmaking_search_preference_language,

	k_matchmaking_search_preference_count
};

enum e_life_cycle_state
{
	_life_cycle_state_none = 0,
	_life_cycle_state_pre_game,
	_life_cycle_state_start_game,
	_life_cycle_state_in_game,
	_life_cycle_state_end_game_write_stats,
	_life_cycle_state_leaving,
	_life_cycle_state_joining,
	_life_cycle_state_matchmaking_start,
	_life_cycle_state_matchmaking_find_match_client,
	_life_cycle_state_matchmaking_find_match,
	_life_cycle_state_matchmaking_find_and_assemble_match,
	_life_cycle_state_matchmaking_assemble_match,
	_life_cycle_state_matchmaking_arbitration,
	_life_cycle_state_matchmaking_select_host,
	_life_cycle_state_matchmaking_prepare_map,
	_life_cycle_state_in_match,
	_life_cycle_state_end_match_write_stats,
	_life_cycle_state_post_match,

	k_life_cycle_state_count
};

enum e_life_cycle_matchmaking_progress_type
{
	_life_cycle_matchmaking_progress_none = 0,
	_life_cycle_matchmaking_progress_searching_for_match,
	_life_cycle_matchmaking_progress_assembling_match,
	_life_cycle_matchmaking_progress_setting_up_match,
	_life_cycle_matchmaking_progress_configuring_match,
	_life_cycle_matchmaking_progress_post_match,

	k_life_cycle_matchmaking_progress_type_count
};

enum e_network_rough_quality
{
	_network_rough_quality_none = 0,
	_network_rough_quality_good,
	_network_rough_quality_moderate,
	_network_rough_quality_bad,

	k_network_rough_quality_count
};

enum e_session_game_start_status
{
	_session_game_start_status_none = 0,
	_session_game_start_status_not_pregame,
	_session_game_start_status_not_leader,
	_session_game_start_status_join_in_progress,
	_session_game_start_status_loading,
	_session_game_start_status_ready,
	_session_game_start_status_accelerate_countdown,
	_session_game_start_status_countdown,
	_session_game_start_status_error,

	k_session_game_start_status_count
};

enum e_session_game_start_error
{
	_session_game_start_error_none = 0,
	_session_game_start_error_network_configuration_unavailable,
	_session_game_start_match_error_banhammer_unavailable,
	_session_game_start_match_error_player_stats_unavailable,
	_session_game_start_match_error_master_hopper_file_unavailable,
	_session_game_start_match_error_hopper_specific_file_unavailable,
	_session_game_start_error_network_configuration_pending,
	_session_game_start_match_error_banhammer_pending,
	_session_game_start_match_error_player_stats_pending,
	_session_game_start_match_error_master_hopper_file_pending,
	_session_game_start_match_error_hopper_specific_file_pending,
	_session_game_start_error_no_map_selected,
	_session_game_start_error_map_load_failure,
	_session_game_start_error_map_load_precaching,
	_session_game_start_error_invalid_film_selected,
	_session_game_start_error_no_film_selected,
	_session_game_start_error_too_many_teams,
	_session_game_start_error_all_players_are_observers,
	_session_game_start_error_too_many_local_players_for_coop,
	_session_game_start_error_too_many_players_for_network_coop,
	_session_game_start_error_players_incompatible_for_network_coop,
	_session_game_start_error_account_not_online_enabled,
	_session_game_start_match_error_must_have_online_enabled_profiles_for_match,
	_session_game_start_match_error_must_connect_to_live_for_match,
	_session_game_start_match_error_must_be_in_live_lobby_for_match,
	_session_game_start_match_error_invalid_hopper,
	_session_game_start_match_error_squad_too_large,
	_session_game_start_match_error_squad_too_small,
	_session_game_start_match_error_games_played_too_low,
	_session_game_start_match_error_games_played_too_high,
	_session_game_start_match_error_experience_too_low,
	_session_game_start_match_error_experience_too_high,
	_session_game_start_match_error_access_bit_not_set,
	_session_game_start_match_error_unpaid_in_paid_hopper,
	_session_game_start_match_error_paid_in_unpaid_hopper,
	_session_game_start_match_error_player_not_in_required_region,
	_session_game_start_match_error_player_not_in_required_country_code,
	_session_game_start_match_error_player_not_in_required_language,
	_session_game_start_match_error_player_not_in_required_gamerzone,
	_session_game_start_match_error_guest_player_when_not_allowed,
	_session_game_start_match_error_not_beta_player_in_beta_hopper,
	_session_game_start_match_error_player_missing_content,
	_session_game_start_match_error_player_missing_files,
	_session_game_start_match_error_player_missing_required_maps,
	_session_game_start_match_error_player_banned_from_matchmaking,
	_session_game_start_match_error_not_yet_start_time,
	_session_game_start_match_error_end_time_has_passed,
	_session_game_start_error_custom_games_are_disabled_for_alpha,
	_session_game_start_error_maximum_multiplayer_split_screen_exceeded,
	_session_game_start_error_must_be_connected_to_live_in_live_lobby,
	_session_game_start_error_must_be_connected_to_live_for_alpha,
	_session_game_start_error_only_one_player_allowed_in_theater_for_alpha,
	_session_game_start_error_theater_too_many_players,
	_session_game_start_error_theater_must_have_hard_drive,
	_session_game_start_error_theater_leader_must_be_host,
	_session_game_start_error_theater_all_players_not_compatible_with_film,
	_session_game_start_error_forge_too_many_players,
	_session_game_start_error_user_content_not_permitted,
	_session_game_start_error_coop_player_missing_hdd,
	_session_game_start_error_coop_player_hdd_mismatch,
	_session_game_start_error_coop_player_language_mismatch,
	_session_game_start_error_invalid_film_selected_language,
	_session_game_start_error_user_controller_not_attached,

	k_session_game_start_error_count
};

enum e_network_session_map_status
{
	_network_session_map_status_none = 0,
	_network_session_map_status_failed,
	_network_session_map_status_precaching,
	_network_session_map_status_precached,
	_network_session_map_status_loaded,

	k_network_session_map_status_count
};

enum e_network_session_peer_state
{
	_network_session_peer_state_none = 0,
	_network_session_peer_state_rejoining,
	_network_session_peer_state_reserved,
	_network_session_peer_state_disconnected,
	_network_session_peer_state_connected,
	_network_session_peer_state_joining,
	_network_session_peer_state_joined,
	_network_session_peer_state_waiting,
	_network_session_peer_state_established,

	k_network_session_peer_state_count
};

enum e_gui_network_session_advertisement_mode
{
	_gui_network_session_advertisement_mode_invalid = -1,
	_gui_network_session_advertisement_mode_open_to_public,
	_gui_network_session_advertisement_mode_open_to_friends,
	_gui_network_session_advertisement_mode_invite_only,
	_gui_network_session_advertisement_mode_system_link,
	_gui_network_session_advertisement_mode_offline,
	_gui_network_session_advertisement_mode_full,

	k_gui_network_session_advertisement_mode_count
};

enum e_user_interface_session_leave_type
{
	_user_interface_session_leave_to_pre_game_lobby = 0,
	_user_interface_session_leave_to_main_menu,

	k_user_interface_session_leave_type_count
};

enum e_user_interface_session_leave_reason
{
	_user_interface_session_leaving_all_controllers_signed_out = 0,
	_user_interface_session_leaving_booted,
	_user_interface_session_leaving_failed_to_start,
	_user_interface_session_leaving_game,
	_user_interface_session_leaving_backing_out_of_searching,
	_user_interface_session_leaving_backing_out_of_post_game,
	_user_interface_session_leaving_backing_out_of_pre_game,

	// Halo: Reach
	//_user_interface_session_leaving_whitelist_enforcement,
	//_user_interface_session_leaving_leaving_game_on_resume,

	// MCC
	//_user_interface_session_leaving_banned,

	k_user_interface_session_leave_reason_count
};

enum e_network_session_peer_properties_status_flags
{
	_network_session_peer_properties_status_game_stats_written_bit = 0,
	_network_session_peer_properties_status_match_ready_to_start_bit,
	_network_session_peer_properties_status_match_arbitration_succeeded_bit,
	_network_session_peer_properties_status_match_arbitration_failed_bit,
	_network_session_peer_properties_status_match_teams_selected_bit,
	_network_session_peer_properties_status_match_repeated_play_set_bit,
	_network_session_peer_properties_status_match_started_bit,
	_network_session_peer_properties_status_match_start_failed_bit,
	_network_session_peer_properties_status_match_initial_stats_written_bit,
	_network_session_peer_properties_status_match_initial_stats_write_failed_bit,
	_network_session_peer_properties_status_match_stats_written_bit,
	_network_session_peer_properties_status_match_host_selection_complete_bit,
	_network_session_peer_properties_status_match_post_match_countdown_bit,
	_network_session_peer_properties_status_match_has_idle_controller_bit,
	_network_session_peer_properties_status_match_ready_for_next_match_bit,
	_network_session_peer_properties_status_match_simulation_aborted_bit,
	_network_session_peer_properties_status_match_acknowledge_sync_bit,

	k_network_session_peer_properties_status_flags
};

enum e_ui_message_type
{
	_message_type_error = 0,
	_message_type_controller_input,
	_message_type_xenon,
	_message_type_load_screen,
	_message_type_change_display_group,
	_message_type_screen_custom,
	_message_type_dialog_result,
	_message_type_unknown7,
};

enum e_browser_type
{
	// names from string ids used with `<browser-title` game tag parser data function

	_browse_system_link = 0,
	_browse_friends,
	_browse_xbox_live,

	k_number_of_game_browser_types
};

enum e_game_progression_level
{
	_game_progression_level_none = 0,
	_game_progression_level_normal,
	_game_progression_level_hub_and_level_is_hub,
	_game_progression_level_spoke_and_level_is_spoke,

	k_game_progression_level_count
};

enum e_achievement
{
	_achievement_beat_sc100 = 0,
	_achievement_beat_sc110,
	_achievement_beat_sc120,
	_achievement_beat_sc130,
	_achievement_beat_sc140,
	_achievement_beat_sc150,
	_achievement_beat_l200,
	_achievement_beat_l300,
	_achievement_beat_campaign_normal,
	_achievement_beat_campaign_heroic,
	_achievement_beat_campaign_legendary,
	_achievement_wraith_killer,
	_achievement_naughty_naughty,
	_achievement_good_samaritan,
	_achievement_dome_inspector,
	_achievement_laser_blaster,
	_achievement_both_tubes,
	_achievement_i_like_fire,
	_achievement_my_clothes,
	_achievement_pink_and_deadly,
	_achievement_dark_times,
	_achievement_trading_down,
	_achievement_headcase,
	_achievement_boom_headshot,
	_achievement_ewww_sticky,
	_achievement_junior_detective,
	_achievement_gumshoe,
	_achievement_super_sleuth,
	_achievement_metagame_points_in_sc100,
	_achievement_metagame_points_in_sc110,
	_achievement_metagame_points_in_sc120,
	_achievement_metagame_points_in_sc130a,
	_achievement_metagame_points_in_sc130b,
	_achievement_metagame_points_in_sc140,
	_achievement_metagame_points_in_l200,
	_achievement_metagame_points_in_l300,
	_achievement_be_like_marty,
	_achievement_find_all_audio_logs,
	_achievement_find_01_audio_logs,
	_achievement_find_03_audio_logs,
	_achievement_find_15_audio_logs,
	_achievement_vidmaster_challenge_deja_vu,
	_achievement_vidmaster_challenge_endure,
	_achievement_vidmaster_challenge_classic,
	_achievement_heal_up,
	_achievement_stunning,
	_achievement_tourist,

	k_achievement_count
};

enum e_damage_reporting_type
{
	_damage_reporting_type_unknown = 0,
	_damage_reporting_type_guardians,
	_damage_reporting_type_falling_damage,
	_damage_reporting_type_generic_collision_damage,
	_damage_reporting_type_armor_lock_crush,
	_damage_reporting_type_generic_melee_damage,
	_damage_reporting_type_generic_explosion,
	_damage_reporting_type_magnum_pistol,
	_damage_reporting_type_plasma_pistol,
	_damage_reporting_type_needler,
	_damage_reporting_type_excavator,
	_damage_reporting_type_smg,
	_damage_reporting_type_plasma_rifle,
	_damage_reporting_type_battle_rifle,
	_damage_reporting_type_carbine,
	_damage_reporting_type_shotgun,
	_damage_reporting_type_sniper_rifle,
	_damage_reporting_type_beam_rifle,
	_damage_reporting_type_assault_rifle,
	_damage_reporting_type_spike_rifle,
	_damage_reporting_type_flak_cannon,
	_damage_reporting_type_missile_launcher,
	_damage_reporting_type_rocket_launcher,
	_damage_reporting_type_spartan_laser,
	_damage_reporting_type_brute_shot,
	_damage_reporting_type_flame_thrower,
	_damage_reporting_type_sentinal_gun,
	_damage_reporting_type_energy_sword,
	_damage_reporting_type_gravity_hammer,
	_damage_reporting_type_frag_grenade,
	_damage_reporting_type_plasma_grenade,
	_damage_reporting_type_claymore_grenade,
	_damage_reporting_type_firebomb_grenade,
	_damage_reporting_type_flag_melee_damage,
	_damage_reporting_type_bomb_melee_damage,
	_damage_reporting_type_bomb_explosion_damage,
	_damage_reporting_type_ball_melee_damage,
	_damage_reporting_type_human_turret,
	_damage_reporting_type_plasma_cannon,
	_damage_reporting_type_plasma_mortar,
	_damage_reporting_type_plasma_turret,
	_damage_reporting_type_shade_turret,
	_damage_reporting_type_banshee,
	_damage_reporting_type_ghost,
	_damage_reporting_type_mongoose,
	_damage_reporting_type_scorpion,
	_damage_reporting_type_scorpion_gunner,
	_damage_reporting_type_spectre_driver,
	_damage_reporting_type_spectre_gunner,
	_damage_reporting_type_warthog_driver,
	_damage_reporting_type_warthog_gunner,
	_damage_reporting_type_warthog_gunner_gauss,
	_damage_reporting_type_wraith,
	_damage_reporting_type_wraith_anti_infantry,
	_damage_reporting_type_tank,
	_damage_reporting_type_chopper,
	_damage_reporting_type_hornet,
	_damage_reporting_type_mantis,
	_damage_reporting_type_mauler,
	_damage_reporting_type_sentinel_beam,
	_damage_reporting_type_sentinel_rpg,
	_damage_reporting_type_teleporter,
	_damage_reporting_type_prox_mine,
	_damage_reporting_type_marksman_rifle,

	k_damage_reporting_type_count
};

// find an actual name for this and also place it in the correct location
enum e_damage_aftermath_flags
{
	_damage_aftermath_body_depleted_bit = 0,
	_damage_aftermath_shield_depleted_bit,
	_damage_aftermath_area_of_effect_bit,
	_damage_aftermath_any_damage_dealt_bit,
	_damage_aftermath_damaged_by_friendly_bit,
	_damage_aftermath_silent_bit,
	_damage_aftermath_killed_instantly_bit,
	_damage_aftermath_primary_recipient_bit,
	_damage_aftermath_outside_aoe_dmg_range_bit,
	_damage_aftermath_object_destroyed_bit,
	_damage_aftermath_emp_bit,

	k_damage_aftermath_flags
};

enum e_game_engine_category
{
	_game_variant_miscellaneous = 0,
	_game_variant_respawn,
	_game_variant_social,
	_game_variant_map_override,

	_base_player_traits,
	_base_player_traits_shields,
	_base_player_traits_weapons,
	_base_player_traits_movement,
	_base_player_traits_sensors,
	_base_player_traits_appearance,
	_base_player_templated,

	_respawn_traits,
	_respawn_traits_shields,
	_respawn_traits_weapons,
	_respawn_traits_movement,
	_respawn_traits_sensors,
	_respawn_traits_appearance,
	_respawn_templated,

	_red_powerup_traits,
	_red_powerup_traits_shields,
	_red_powerup_traits_weapons,
	_red_powerup_traits_movement,
	_red_powerup_traits_sensors,
	_red_powerup_traits_appearance,
	_red_powerup_templated,

	_blue_powerup_traits,
	_blue_powerup_traits_shields,
	_blue_powerup_traits_weapons,
	_blue_powerup_traits_movement,
	_blue_powerup_traits_sensors,
	_blue_powerup_traits_appearance,
	_blue_powerup_templated,

	_yellow_powerup_traits,
	_yellow_powerup_traits_shields,
	_yellow_powerup_traits_weapons,
	_yellow_powerup_traits_movement,
	_yellow_powerup_traits_sensors,
	_yellow_powerup_traits_appearance,
	_yellow_powerup_templated,

	_powerup_duration,

	_slayer_leader_traits,
	_slayer_leader_traits_shields,
	_slayer_leader_traits_weapons,
	_slayer_leader_traits_movement,
	_slayer_leader_traits_sensors,
	_slayer_leader_traits_appearance,
	_slayer_leader_templated,

	_oddball_carrier_traits,
	_oddball_carrier_traits_shields,
	_oddball_carrier_traits_weapons,
	_oddball_carrier_traits_movement,
	_oddball_carrier_traits_sensors,
	_oddball_carrier_traits_appearance,
	_oddball_carrier_templated,

	_assult_carrier_traits,
	_assult_carrier_traits_shields,
	_assult_carrier_traits_weapons,
	_assult_carrier_traits_movement,
	_assult_carrier_traits_sensors,
	_assult_carrier_traits_appearance,
	_assult_carrier_templated,

	_ctf_carrier_traits,
	_ctf_carrier_traits_shields,
	_ctf_carrier_traits_weapons,
	_ctf_carrier_traits_movement,
	_ctf_carrier_traits_sensors,
	_ctf_carrier_traits_appearance,
	_ctf_carrier_templated,

	_juggernaut_traits,
	_juggernaut_traits_shields,
	_juggernaut_traits_weapons,
	_juggernaut_traits_movement,
	_juggernaut_traits_sensors,
	_juggernaut_traits_appearance,
	_juggernaut_templated,

	_king_hill_traits,
	_king_hill_traits_shields,
	_king_hill_traits_weapons,
	_king_hill_traits_movement,
	_king_hill_traits_sensors,
	_king_hill_traits_appearance,
	_king_hill_templated,

	_zombie_traits,
	_zombie_traits_shields,
	_zombie_traits_weapons,
	_zombie_traits_movement,
	_zombie_traits_sensors,
	_zombie_traits_appearance,
	_zombie_templated,

	_first_zombie_traits,
	_first_zombie_traits_shields,
	_first_zombie_traits_weapons,
	_first_zombie_traits_movement,
	_first_zombie_traits_sensors,
	_first_zombie_traits_appearance,
	_first_zombie_templated,

	_safe_haven_traits,
	_safe_haven_traits_shields,
	_safe_haven_traits_weapons,
	_safe_haven_traits_movement,
	_safe_haven_traits_sensors,
	_safe_haven_traits_appearance,
	_safe_haven_templated,

	_infection_last_human_traits,
	_infection_last_human_traits_shields,
	_infection_last_human_traits_weapons,
	_infection_last_human_traits_movement,
	_infection_last_human_traits_sensors,
	_infection_last_human_traits_appearance,
	_infection_last_human_templated,

	_target_dummy_traits,
	_target_dummy_traits_shields,
	_target_dummy_traits_weapons,
	_target_dummy_traits_movement,
	_target_dummy_traits_sensors,
	_target_dummy_traits_appearance,
	_target_dummy_templated,

	_territory_defender_traits,
	_territory_defender_traits_shields,
	_territory_defender_traits_weapons,
	_territory_defender_traits_movement,
	_territory_defender_traits_sensors,
	_territory_defender_traits_appearance,
	_territory_defender_templated,

	_territory_attacker_traits,
	_territory_attacker_traits_shields,
	_territory_attacker_traits_weapons,
	_territory_attacker_traits_movement,
	_territory_attacker_traits_sensors,
	_territory_attacker_traits_appearance,
	_territory_attacker_templated,

	_vip_team_traits,
	_vip_team_traits_shields,
	_vip_team_traits_weapons,
	_vip_team_traits_movement,
	_vip_team_traits_sensors,
	_vip_team_traits_appearance,
	_vip_team_templated,

	_vip_influence_traits,
	_vip_influence_traits_shields,
	_vip_influence_traits_weapons,
	_vip_influence_traits_movement,
	_vip_influence_traits_sensors,
	_vip_influence_traits_appearance,
	_vip_influence_templated,

	_vip_traits,
	_vip_traits_shields,
	_vip_traits_weapons,
	_vip_traits_movement,
	_vip_traits_sensors,
	_vip_traits_appearance,
	_vip_templated,

	_sandbox_editor_traits,
	_sandbox_editor_traits_shields,
	_sandbox_editor_traits_weapons,
	_sandbox_editor_traits_movement,
	_sandbox_editor_traits_sensors,
	_sandbox_editor_traits_appearance,
	_sandbox_editor_templated,

	_game_engine_ctf_top_level_options,
	_game_engine_slayer_top_level_options,
	_game_engine_oddball_top_level_options,
	_game_engine_king_top_level_options,
	_game_engine_sandbox_top_level_options,
	_game_engine_juggernaut_top_level_options,
	_game_engine_territories_top_level_options,
	_game_engine_assault_top_level_options,
	_game_engine_infection_top_level_options,
	_game_engine_vip_top_level_options,
	_game_engine_training_top_level_options,
	_game_engine_map_override_advanced_options,
	_game_engine_respawn_advanced_options,
	_game_engine_respawn_modifiers,

	_slayer_primary_options,
	_slayer_advanced_options,
	_slayer_scoring_options,

	_oddball_primary_options,
	_oddball_advanced_options,

	_assault_primary_options,
	_assault_advanced_options,

	_ctf_primary_options,
	_ctf_advanced_options,

	_juggernaut_primary_options,
	_juggernaut_advanced_options,

	_king_primary_options,
	_king_advanced_options,

	_infection_primary_options,
	_infection_advanced_options,
	_infection_scoring_options,

	_target_training_primary_options,
	_target_training_advanced_options,

	_target_01_setup_options,
	_target_02_setup_options,
	_target_03_setup_options,
	_target_04_setup_options,
	_target_05_setup_options,
	_target_06_setup_options,
	_target_07_setup_options,
	_target_08_setup_options,
	_target_09_setup_options,
	_target_10_setup_options,

	_vip_primary_options,
	_vip_advanced_options,

	_territories_primary_options,
	_territories_advanced_options,

	_player_template_traits,
	_player_template_traits_shields,
	_player_template_traits_weapons,
	_player_template_traits_movement,
	_player_template_traits_sensors,
	_player_template_traits_appearance,

	_player_alternate_template_traits,
	_player_alternate_template_traits_shields,
	_player_alternate_template_traits_weapons,
	_player_alternate_template_traits_movement,
	_player_alternate_template_traits_sensors,
	_player_alternate_template_traits_appearance,

	k_number_of_editable_game_engine_category_groups
};

enum e_game_variant_parameter
{
	_game_variant_base_miscellaneous_teams,
	_game_variant_base_miscellaneous_director_allow_player_switching,
	_game_variant_base_miscellaneous_round_reset_map,
	_game_variant_base_miscellaneous_round_time_limit,
	_game_variant_base_miscellaneous_round_limit,
	_game_variant_base_miscellaneous_early_victory_win_count,

	_game_variant_respawn_inherit_respawn_timer,
	_game_variant_respawn_with_teammate,
	_game_variant_respawn_at_location,
	_game_variant_respawn_upon_kill,
	_game_variant_respawn_lives_per_round,
	_game_variant_respawn_team_lives_per_round,
	_game_variant_respawn_time,
	_game_variant_respawn_suicide_penalty,
	_game_variant_respawn_betrayal_penalty,
	_game_variant_respawn_growth,
	_game_variant_respawn_traits_duration,

	_respawn_shld_dmg_resistance,
	_respawn_shld_multiplier,
	_respawn_shld_recharge_rate,
	_respawn_headshot_immunity,
	_respawn_vampirism,
	_respawn_dmg_modifier_percentage,
	_respawn_initial_primary_weapon,
	_respawn_initial_secondary_weapon,
	_respawn_initial_grenade_count,
	_respawn_recharging_grenades,
	_respawn_infinite_ammo,
	_respawn_weapon_pickup,
	_respawn_walking_speed,
	_respawn_personal_gravity,
	_respawn_vehicle_use,
	_respawn_motion_tracker,
	_respawn_motion_tracker_range,
	_respawn_waypoint,
	_respawn_active_camo,
	_respawn_visual,
	_respawn_forced_change_colors,

	_game_variant_social_observers,
	_game_variant_social_team_changing,
	_game_variant_social_friendly_fire,
	_game_variant_social_betrayal_booting,
	_game_variant_social_enemy_voice,
	_game_variant_social_open_channel_voice,
	_game_variant_social_dead_player_voice,

	_game_variant_map_override_grenades,
	_game_variant_map_override_indestructible_vehicles,

	_player_shld_dmg_resistance,
	_player_shld_multiplier,
	_player_shld_recharge_rate,
	_player_headshot_immunity,
	_player_vampirism,
	_player_dmg_modifier_percentage,
	_player_initial_primary_weapon,
	_player_initial_secondary_weapon,
	_player_initial_grenade_count,
	_player_recharging_grenades,
	_player_infinite_ammo,
	_player_weapon_pickup,
	_player_walking_speed,
	_player_personal_gravity,
	_player_vehicle_use,
	_player_motion_tracker,
	_player_motion_tracker_range,
	_player_waypoint,
	_player_active_camo,
	_player_visual,
	_player_forced_change_colors,

	_game_variant_map_override_weapon_set,
	_game_variant_map_override_vehicle_set,

	_red_powerup_shld_dmg_resistance,
	_red_powerup_shld_multiplier,
	_red_powerup_shld_recharge_rate,
	_red_powerup_headshot_immunity,
	_red_powerup_vampirism,
	_red_powerup_dmg_modifier_percentage,
	_red_powerup_initial_primary_weapon,
	_red_powerup_initial_secondary_weapon,
	_red_powerup_initial_grenade_count,
	_red_powerup_recharging_grenades,
	_red_powerup_infinite_ammo,
	_red_powerup_weapon_pickup,
	_red_powerup_walking_speed,
	_red_powerup_personal_gravity,
	_red_powerup_vehicle_use,
	_red_powerup_motion_tracker,
	_red_powerup_motion_tracker_range,
	_red_powerup_waypoint,
	_red_powerup_active_camo,
	_red_powerup_visual,
	_red_powerup_forced_change_colors,

	_blue_powerup_shld_dmg_resistance,
	_blue_powerup_shld_multiplier,
	_blue_powerup_shld_recharge_rate,
	_blue_powerup_headshot_immunity,
	_blue_powerup_vampirism,
	_blue_powerup_dmg_modifier_percentage,
	_blue_powerup_initial_primary_weapon,
	_blue_powerup_initial_secondary_weapon,
	_blue_powerup_initial_grenade_count,
	_blue_powerup_recharging_grenades,
	_blue_powerup_infinite_ammo,
	_blue_powerup_weapon_pickup,
	_blue_powerup_walking_speed,
	_blue_powerup_personal_gravity,
	_blue_powerup_vehicle_use,
	_blue_powerup_motion_tracker,
	_blue_powerup_motion_tracker_range,
	_blue_powerup_waypoint,
	_blue_powerup_active_camo,
	_blue_powerup_visual,
	_blue_powerup_forced_change_colors,

	_yellow_powerup_shld_dmg_resistance,
	_yellow_powerup_shld_multiplier,
	_yellow_powerup_shld_recharge_rate,
	_yellow_powerup_headshot_immunity,
	_yellow_powerup_vampirism,
	_yellow_powerup_dmg_modifier_percentage,
	_yellow_powerup_initial_primary_weapon,
	_yellow_powerup_initial_secondary_weapon,
	_yellow_powerup_initial_grenade_count,
	_yellow_powerup_recharging_grenades,
	_yellow_powerup_infinite_ammo,
	_yellow_powerup_weapon_pickup,
	_yellow_powerup_walking_speed,
	_yellow_powerup_personal_gravity,
	_yellow_powerup_vehicle_use,
	_yellow_powerup_motion_tracker,
	_yellow_powerup_motion_tracker_range,
	_yellow_powerup_waypoint,
	_yellow_powerup_active_camo,
	_yellow_powerup_visual,
	_yellow_powerup_forced_change_colors,

	_game_variant_map_override_red_powerup_traits_duration,
	_game_variant_map_override_blue_powerup_traits_duration,
	_game_variant_map_override_yellow_powerup_traits_duration,

	_slayer_team_scoring,
	_slayer_score_to_win,
	_slayer_kill_points,
	_slayer_assist_points,
	_slayer_death_points,
	_slayer_suicide_points,
	_slayer_betrayal_points,
	_slayer_leader_killed_points,
	_slayer_elimination_points,
	_slayer_assassination_points,
	_slayer_headshot_points,
	_slayer_melee_points,
	_slayer_sticky_points,
	_slayer_splatter_points,
	_slayer_killing_spree_points,

	_slayer_leader_shld_dmg_resistance,
	_slayer_leader_shld_multiplier,
	_slayer_leader_shld_recharge_rate,
	_slayer_leader_headshot_immunity,
	_slayer_leader_vampirism,
	_slayer_leader_dmg_modifier_percentage,
	_slayer_leader_initial_primary_weapon,
	_slayer_leader_initial_secondary_weapon,
	_slayer_leader_initial_grenade_count,
	_slayer_leader_recharging_grenades,
	_slayer_leader_infinite_ammo,
	_slayer_leader_weapon_pickup,
	_slayer_leader_walking_speed,
	_slayer_leader_personal_gravity,
	_slayer_leader_vehicle_use,
	_slayer_leader_motion_tracker,
	_slayer_leader_motion_tracker_range,
	_slayer_leader_waypoint,
	_slayer_leader_active_camo,
	_slayer_leader_visual,
	_slayer_leader_forced_change_colors,

	_oddball_auto_ball_pickup,
	_oddball_ball_effect,
	_oddball_team_scoring,
	_oddball_waypoints,
	_oddball_score_to_win,
	_oddball_carrying_points,
	_oddball_kill_points,
	_oddball_ball_kill_points,
	_oddball_carrier_kill_points,
	_oddball_ball_count,
	_oddball_ball_spawn_delay,
	_oddball_inactive_respawn_delay,

	_ball_carrier_shld_dmg_resistance,
	_ball_carrier_shld_multiplier,
	_ball_carrier_shld_recharge_rate,
	_ball_carrier_headshot_immunity,
	_ball_carrier_vampirism,
	_ball_carrier_dmg_modifier_percentage,
	_ball_carrier_initial_primary_weapon,
	_ball_carrier_initial_secondary_weapon,
	_ball_carrier_initial_grenade_count,
	_ball_carrier_recharging_grenades,
	_ball_carrier_infinite_ammo,
	_ball_carrier_weapon_pickup,
	_ball_carrier_walking_speed,
	_ball_carrier_personal_gravity,
	_ball_carrier_vehicle_use,
	_ball_carrier_motion_tracker,
	_ball_carrier_motion_tracker_range,
	_ball_carrier_waypoint,
	_ball_carrier_active_camo,
	_ball_carrier_visual,
	_ball_carrier_forced_change_colors,

	_ctf_touch_return_timeout,
	_ctf_flag_at_home_to_score,
	_ctf_bomb_sticky_arming,
	_ctf_home_flag_waypoint,
	_ctf_enemy_bomb_waypoint,
	_ctf_game_type,
	_ctf_respawn,
	_ctf_score_to_win,
	_ctf_sudden_death_time,
	_ctf_flag_reset_time,
	_ctf_bomb_arming_time,
	_ctf_bomb_fuse_time,

	_flag_carrier_shld_dmg_resistance,
	_flag_carrier_shld_multiplier,
	_flag_carrier_shld_recharge_rate,
	_flag_carrier_headshot_immunity,
	_flag_carrier_vampirism,
	_flag_carrier_dmg_modifier_percentage,
	_flag_carrier_initial_primary_weapon,
	_flag_carrier_initial_secondary_weapon,
	_flag_carrier_initial_grenade_count,
	_flag_carrier_recharging_grenades,
	_flag_carrier_infinite_ammo,
	_flag_carrier_weapon_pickup,
	_flag_carrier_walking_speed,
	_flag_carrier_personal_gravity,
	_flag_carrier_vehicle_use,
	_flag_carrier_motion_tracker,
	_flag_carrier_motion_tracker_range,
	_flag_carrier_waypoint,
	_flag_carrier_active_camo,
	_flag_carrier_visual,
	_flag_carrier_forced_change_colors,

	_infection_respawn_on_haven_move,
	_infection_safe_haven_setting,
	_infection_next_zombie,
	_infection_initial_zombie_count,
	_infection_safe_haven_movement_time,
	_infection_zombie_kill_points,
	_infection_points,
	_infection_safe_haven_arrival_points,
	_infection_suicide_points,
	_infection_betrayal_points,
	_infection_last_man_standing_bonus_points,

	_zombie_shld_dmg_resistance,
	_zombie_shld_multiplier,
	_zombie_shld_recharge_rate,
	_zombie_headshot_immunity,
	_zombie_vampirism,
	_zombie_dmg_modifier_percentage,
	_zombie_initial_primary_weapon,
	_zombie_initial_secondary_weapon,
	_zombie_initial_grenade_count,
	_zombie_recharging_grenades,
	_zombie_infinite_ammo,
	_zombie_weapon_pickup,
	_zombie_walking_speed,
	_zombie_personal_gravity,
	_zombie_vehicle_use,
	_zombie_motion_tracker,
	_zombie_motion_tracker_range,
	_zombie_waypoint,
	_zombie_active_camo,
	_zombie_visual,
	_zombie_forced_change_colors,

	_first_zombie_shld_dmg_resistance,
	_first_zombie_shld_multiplier,
	_first_zombie_shld_recharge_rate,
	_first_zombie_headshot_immunity,
	_first_zombie_vampirism,
	_first_zombie_dmg_modifier_percentage,
	_first_zombie_initial_primary_weapon,
	_first_zombie_initial_secondary_weapon,
	_first_zombie_initial_grenade_count,
	_first_zombie_recharging_grenades,
	_first_zombie_infinite_ammo,
	_first_zombie_weapon_pickup,
	_first_zombie_walking_speed,
	_first_zombie_personal_gravity,
	_first_zombie_vehicle_use,
	_first_zombie_motion_tracker,
	_first_zombie_motion_tracker_range,
	_first_zombie_waypoint,
	_first_zombie_active_camo,
	_first_zombie_visual,
	_first_zombie_forced_change_colors,

	_haven_defender_shld_dmg_resistance,
	_haven_defender_shld_multiplier,
	_haven_defender_shld_recharge_rate,
	_haven_defender_headshot_immunity,
	_haven_defender_vampirism,
	_haven_defender_dmg_modifier_percentage,
	_haven_defender_initial_primary_weapon,
	_haven_defender_initial_secondary_weapon,
	_haven_defender_initial_grenade_count,
	_haven_defender_recharging_grenades,
	_haven_defender_infinite_ammo,
	_haven_defender_weapon_pickup,
	_haven_defender_walking_speed,
	_haven_defender_personal_gravity,
	_haven_defender_vehicle_use,
	_haven_defender_motion_tracker,
	_haven_defender_motion_tracker_range,
	_haven_defender_waypoint,
	_haven_defender_active_camo,
	_haven_defender_visual,
	_haven_defender_forced_change_colors,

	_last_human_shld_dmg_resistance,
	_last_human_shld_multiplier,
	_last_human_shld_recharge_rate,
	_last_human_headshot_immunity,
	_last_human_vampirism,
	_last_human_dmg_modifier_percentage,
	_last_human_initial_primary_weapon,
	_last_human_initial_secondary_weapon,
	_last_human_initial_grenade_count,
	_last_human_recharging_grenades,
	_last_human_infinite_ammo,
	_last_human_weapon_pickup,
	_last_human_walking_speed,
	_last_human_personal_gravity,
	_last_human_vehicle_use,
	_last_human_motion_tracker,
	_last_human_motion_tracker_range,
	_last_human_waypoint,
	_last_human_active_camo,
	_last_human_visual,
	_last_human_forced_change_colors,

	_king_opaque_hill,
	_king_score_to_win,
	_king_team_scoring,
	_king_moving_hill,
	_king_moving_hill_order,
	_king_inside_hill_points,
	_king_outside_hill_points,
	_king_uncontested_hill_points,
	_king_kill_points,

	_on_hill_shld_dmg_resistance,
	_on_hill_shld_multiplier,
	_on_hill_shld_recharge_rate,
	_on_hill_headshot_immunity,
	_on_hill_vampirism,
	_on_hill_dmg_modifier_percentage,
	_on_hill_initial_primary_weapon,
	_on_hill_initial_secondary_weapon,
	_on_hill_initial_grenade_count,
	_on_hill_recharging_grenades,
	_on_hill_infinite_ammo,
	_on_hill_weapon_pickup,
	_on_hill_walking_speed,
	_on_hill_personal_gravity,
	_on_hill_vehicle_use,
	_on_hill_motion_tracker,
	_on_hill_motion_tracker_range,
	_on_hill_waypoint,
	_on_hill_active_camo,
	_on_hill_visual,
	_on_hill_forced_change_colors,

	_territories_capture_time,
	_territories_one_sided,
	_territories_lock_after_capture,
	_territories_sudden_death,
	_territories_respawn_on_capture,

	_territory_defender_shld_dmg_resistance,
	_territory_defender_shld_multiplier,
	_territory_defender_shld_recharge_rate,
	_territory_defender_headshot_immunity,
	_territory_defender_vampirism,
	_territory_defender_dmg_modifier_percentage,
	_territory_defender_initial_primary_weapon,
	_territory_defender_initial_secondary_weapon,
	_territory_defender_initial_grenade_count,
	_territory_defender_recharging_grenades,
	_territory_defender_infinite_ammo,
	_territory_defender_weapon_pickup,
	_territory_defender_walking_speed,
	_territory_defender_personal_gravity,
	_territory_defender_vehicle_use,
	_territory_defender_motion_tracker,
	_territory_defender_motion_tracker_range,
	_territory_defender_waypoint,
	_territory_defender_active_camo,
	_territory_defender_visual,
	_territory_defender_forced_change_colors,

	_territory_attacker_shld_dmg_resistance,
	_territory_attacker_shld_multiplier,
	_territory_attacker_shld_recharge_rate,
	_territory_attacker_headshot_immunity,
	_territory_attacker_vampirism,
	_territory_attacker_dmg_modifier_percentage,
	_territory_attacker_initial_primary_weapon,
	_territory_attacker_initial_secondary_weapon,
	_territory_attacker_initial_grenade_count,
	_territory_attacker_recharging_grenades,
	_territory_attacker_infinite_ammo,
	_territory_attacker_weapon_pickup,
	_territory_attacker_walking_speed,
	_territory_attacker_personal_gravity,
	_territory_attacker_vehicle_use,
	_territory_attacker_motion_tracker,
	_territory_attacker_motion_tracker_range,
	_territory_attacker_waypoint,
	_territory_attacker_active_camo,
	_territory_attacker_visual,
	_territory_attacker_forced_change_colors,

	_vip_score_to_win,
	_vip_single_vip,
	_vip_destination_zones_enabled,
	_vip_end_round_on_vip_death,
	_vip_kill_points,
	_vip_takedown_points,
	_vip_kill_as_vip_points,
	_vip_death_points,
	_vip_zone_arrival_points,
	_vip_suicide_points,
	_vip_vip_friendly_death_points,
	_vip_betrayal_points,
	_vip_selection,
	_vip_zone_movement,
	_vip_zone_order,
	_vip_influence_radius,

	_vip_team_shld_dmg_resistance,
	_vip_team_shld_multiplier,
	_vip_team_shld_recharge_rate,
	_vip_team_headshot_immunity,
	_vip_team_vampirism,
	_vip_team_dmg_modifier_percentage,
	_vip_team_initial_primary_weapon,
	_vip_team_initial_secondary_weapon,
	_vip_team_initial_grenade_count,
	_vip_team_recharging_grenades,
	_vip_team_infinite_ammo,
	_vip_team_weapon_pickup,
	_vip_team_walking_speed,
	_vip_team_personal_gravity,
	_vip_team_vehicle_use,
	_vip_team_motion_tracker,
	_vip_team_motion_tracker_range,
	_vip_team_waypoint,
	_vip_team_active_camo,
	_vip_team_visual,
	_vip_team_forced_change_colors,

	_vip_influence_shld_dmg_resistance,
	_vip_influence_shld_multiplier,
	_vip_influence_shld_recharge_rate,
	_vip_influence_headshot_immunity,
	_vip_influence_vampirism,
	_vip_influence_dmg_modifier_percentage,
	_vip_influence_initial_primary_weapon,
	_vip_influence_initial_secondary_weapon,
	_vip_influence_initial_grenade_count,
	_vip_influence_recharging_grenades,
	_vip_influence_infinite_ammo,
	_vip_influence_weapon_pickup,
	_vip_influence_walking_speed,
	_vip_influence_personal_gravity,
	_vip_influence_vehicle_use,
	_vip_influence_motion_tracker,
	_vip_influence_motion_tracker_range,
	_vip_influence_waypoint,
	_vip_influence_active_camo,
	_vip_influence_visual,
	_vip_influence_forced_change_colors,

	_vip_shld_dmg_resistance,
	_vip_shld_multiplier,
	_vip_shld_recharge_rate,
	_vip_headshot_immunity,
	_vip_vampirism,
	_vip_dmg_modifier_percentage,
	_vip_initial_primary_weapon,
	_vip_initial_secondary_weapon,
	_vip_initial_grenade_count,
	_vip_recharging_grenades,
	_vip_infinite_ammo,
	_vip_weapon_pickup,
	_vip_walking_speed,
	_vip_personal_gravity,
	_vip_vehicle_use,
	_vip_motion_tracker,
	_vip_motion_tracker_range,
	_vip_waypoint,
	_vip_active_camo,
	_vip_visual,
	_vip_forced_change_colors,

	_juggernaut_score_to_win,
	_juggernaut_kill_points,
	_juggernaut_juggernaut_kill_points,
	_juggernaut_kill_as_juggernaut_points,
	_juggernaut_zone_arrival_points,
	_juggernaut_suicide_points,
	_juggernaut_betrayal_points,
	_juggernaut_initial_juggernaut,
	_juggernaut_next_juggernaut,
	_juggernaut_allied_against_juggernaut,
	_juggernaut_delay,
	_juggernaut_zones_enabled,
	_juggernaut_zone_movement,
	_juggernaut_zone_order,
	_juggernaut_respawn_on_lone_juggernaut,
	_juggernaut_shld_dmg_resistance,
	_juggernaut_shld_multiplier,
	_juggernaut_shld_recharge_rate,
	_juggernaut_headshot_immunity,
	_juggernaut_vampirism,
	_juggernaut_dmg_modifier_percentage,
	_juggernaut_initial_primary_weapon,
	_juggernaut_initial_secondary_weapon,
	_juggernaut_initial_grenade_count,
	_juggernaut_recharging_grenades,
	_juggernaut_infinite_ammo,
	_juggernaut_weapon_pickup,
	_juggernaut_walking_speed,
	_juggernaut_personal_gravity,
	_juggernaut_vehicle_use,
	_juggernaut_motion_tracker,
	_juggernaut_motion_tracker_range,
	_juggernaut_waypoint,
	_juggernaut_active_camo,
	_juggernaut_visual,
	_juggernaut_forced_change_colors,

	_assault_arming_shld_dmg_resistance,
	_assault_arming_shld_multiplier,
	_assault_arming_shld_recharge_rate,
	_assault_arming_headshot_immunity,
	_assault_arming_vampirism,
	_assault_arming_dmg_modifier_percentage,
	_assault_arming_initial_primary_weapon,
	_assault_arming_initial_secondary_weapon,
	_assault_arming_initial_grenade_count,
	_assault_arming_recharging_grenades,
	_assault_arming_infinite_ammo,
	_assault_arming_weapon_pickup,
	_assault_arming_walking_speed,
	_assault_arming_personal_gravity,
	_assault_arming_vehicle_use,
	_assault_arming_motion_tracker,
	_assault_arming_motion_tracker_range,
	_assault_arming_waypoint,
	_assault_arming_active_camo,
	_assault_arming_visual,
	_assault_arming_forced_change_colors,

	_assault_bomb_arming_time,
	_assault_bomb_disarming_time,
	_assault_bomb_fuse_time,
	_assault_bomb_reset_time,

	_assault_carrier_shld_dmg_resistance,
	_assault_carrier_shld_multiplier,
	_assault_carrier_shld_recharge_rate,
	_assault_carrier_headshot_immunity,
	_assault_carrier_vampirism,
	_assault_carrier_dmg_modifier_percentage,
	_assault_carrier_initial_primary_weapon,
	_assault_carrier_initial_secondary_weapon,
	_assault_carrier_initial_grenade_count,
	_assault_carrier_recharging_grenades,
	_assault_carrier_infinite_ammo,
	_assault_carrier_weapon_pickup,
	_assault_carrier_walking_speed,
	_assault_carrier_personal_gravity,
	_assault_carrier_vehicle_use,
	_assault_carrier_motion_tracker,
	_assault_carrier_motion_tracker_range,
	_assault_carrier_waypoint,
	_assault_carrier_active_camo,
	_assault_carrier_visual,
	_assault_carrier_forced_change_colors,
	_assault_enemy_bomb_waypoint,
	_assault_game_type,
	_assault_reset_bomb_on_disarm,
	_assault_siege_mode,
	_assault_respawn,
	_assault_score_to_win,
	_assault_unknown577,
	_assault_unknown578,
	_assault_unknown579,
	_assault_sudden_death_time,

	_sandbox_open_channel_voice,
	_sandbox_enter_editor_mode,
	_sandbox_respawn_time,
	_sandbox_editor_shld_dmg_resistance,
	_sandbox_editor_shld_multiplier,
	_sandbox_editor_shld_recharge_rate,
	_sandbox_editor_headshot_immunity,
	_sandbox_editor_vampirism,
	_sandbox_editor_dmg_modifier_percentage,
	_sandbox_editor_initial_primary_weapon,
	_sandbox_editor_initial_secondary_weapon,
	_sandbox_editor_initial_grenade_count,
	_sandbox_editor_recharging_grenades,
	_sandbox_editor_infinite_ammo,
	_sandbox_editor_weapon_pickup,
	_sandbox_editor_walking_speed,
	_sandbox_editor_personal_gravity,
	_sandbox_editor_vehicle_use,
	_sandbox_editor_motion_tracker,
	_sandbox_editor_motion_tracker_range,
	_sandbox_editor_waypoint,
	_sandbox_editor_active_camo,
	_sandbox_editor_visual,
	_sandbox_editor_forced_change_colors,

	_template_shld_dmg_resistance,
	_template_shld_multiplier,
	_template_shld_recharge_rate,
	_template_headshot_immunity,
	_template_vampirism,
	_template_dmg_modifier_percentage,
	_template_initial_primary_weapon,
	_template_initial_secondary_weapon,
	_template_initial_grenade_count,
	_template_recharging_grenades,
	_template_infinite_ammo,
	_template_weapon_pickup,
	_template_walking_speed,
	_template_personal_gravity,
	_template_vehicle_use,
	_template_motion_tracker,
	_template_motion_tracker_range,
	_template_waypoint,
	_template_active_camo,
	_template_visual,
	_template_forced_change_colors,

	k_game_variant_parameter_count,
	k_game_variant_parameter_none = -1,
};

enum e_text_value_pair_parameter_type
{
	_integer_value = 0,
	_string_id_value,
	_real_range_value,

	k_number_of_text_value_pair_parameter_types,

	_parameter_type_invalid = -1
};

enum e_game_content_type
{
	_game_content_dlc = 0,
	_game_content_map_variant,
	_game_content_personal_files,
	_game_content_game_variant_ctf,
	_game_content_game_variant_slayer,
	_game_content_game_variant_oddball,
	_game_content_game_variant_king,
	_game_content_game_variant_juggernaut,
	_game_content_game_variant_territories,
	_game_content_game_variant_assault,
	_game_content_game_variant_vip,
	_game_content_game_variant_infection,
	_game_content_saved_film,
	_game_content_saved_film_clip,
	_game_content_saved_screen_shot,

	k_number_of_game_content_types,

	_game_content_type_unknown = -1,
	k_first_user_created_content_type = _game_content_map_variant,
	k_last_user_created_content_type = _game_content_saved_screen_shot,
};

enum e_quality_setting
{
	_quality_setting_low = 0,
	_quality_setting_medium,
	_quality_setting_high,

	k_quality_setting_count
};

enum e_subtitle_setting
{
	_subtitle_setting_automatic = 0,
	_subtitle_setting_enabled,
	_subtitle_setting_disabled,

	k_number_of_subtitle_settings
};

enum e_color_type
{
	_color_type_primary = 0,
	_color_type_secondary,
	_color_type_visor,
	_color_type_lights,
	_color_type_holo,

	k_color_type_count
};

enum e_armor_type
{
	_armor_type_helmet = 0,
	_armor_type_chest,
	_armor_type_shoulders,
	_armor_type_arms,
	_armor_type_legs,

	_armor_type_acc,
	_armor_type_pelvis,

	_armor_type_unknown7,
	_armor_type_unknown8,
	_armor_type_unknown9,

	k_armor_type_count
};

enum e_grenade_type
{
	_grenade_type_human_fragmentation = 0,
	_grenade_type_covenant_plasma,
	_grenade_type_brute_claymore,
	_grenade_type_firebomb,

	k_grenade_type_count
};

// `0069C3D0`, gameplay_modifier_get_name
// `0069C730`, gameplay_modifier_get_index
enum e_gameplay_modifier
{
	_gameplay_modifier_safety_booster = 0,
	_gameplay_modifier_explosives_damage,
	_gameplay_modifier_explosives_aoe_size_boost,
	_gameplay_modifier_grenade_scavenger,
	_gameplay_modifier_stamina_restore_near_death,
	_gameplay_modifier_cooldown_reset,
	_gameplay_modifier_reflex_boost,
	_gameplay_modifier_zoom_radar,
	_gameplay_modifier_cooldown_boost,
	_gameplay_modifier_enable_nemesis_mechanics,
	_gameplay_modifier_silent_sprint,
	_gameplay_modifier_vehicle_shield_regen_speed,
	_gameplay_modifier_vehicle_shield_regen_delay,
	_gameplay_modifier_scavenger_aura,
	_gameplay_modifier_armor_to_vehicle,
	_gameplay_modifier_armor_to_player_in_vehicle,
	_gameplay_modifier_armor_to_player_in_vehicle_transferred,
	_gameplay_modifier_sprint_speed,
	_gameplay_modifier_sprint_stamina,
	_gameplay_modifier_reload_speed,
	_gameplay_modifier_weapon_switch_speed,
	_gameplay_modifier_ammo_max,
	_gameplay_modifier_melee_damage,
	_gameplay_modifier_melee_resist,
	_gameplay_modifier_revenge_shield_boost,
	_gameplay_modifier_respawn_modifier,
	_gameplay_modifier_grenades_max,
	_gameplay_modifier_motion_sensor_range,
	_gameplay_modifier_shield_regen_rate,
	_gameplay_modifier_shield_regen_delay,
	_gameplay_modifier_teflon_armor,
	_gameplay_modifier_explosion_resist,
	_gameplay_modifier_vehicle_ram_damage,
	_gameplay_modifier_turret_damage,
	_gameplay_modifier_machine_gun_rof_acceleration_time,
	_gameplay_modifier_grenade_warning,
	_gameplay_modifier_consumable_duration,
	_gameplay_modifier_energy_increase_max,
	_gameplay_modifier_energy_increase_count_on_spawn,
	_gameplay_modifier_energy_regeneration_rate,
	_gameplay_modifier_assault_rifle_rounds_per_second,
	_gameplay_modifier_assault_rifle_damage,
	_gameplay_modifier_assault_rifle_ammo_capacity,
	_gameplay_modifier_assault_rifle_reload_speed,
	_gameplay_modifier_battle_rifle_rounds_per_second,
	_gameplay_modifier_battle_rifle_fire_recovery_time,
	_gameplay_modifier_battle_rifle_damage,
	_gameplay_modifier_battle_rifle_ammo_capacity,
	_gameplay_modifier_battle_rifle_reload_speed,
	_gameplay_modifier_shotgun_spread,
	_gameplay_modifier_shotgun_damage,
	_gameplay_modifier_shotgun_ammo_capacity,
	_gameplay_modifier_shotgun_reload_speed,
	_gameplay_modifier_sniper_rifle_zoom_protection,
	_gameplay_modifier_sniper_rifle_damage,
	_gameplay_modifier_sniper_rifle_ammo_capacity,
	_gameplay_modifier_sniper_rifle_reload_speed,
	_gameplay_modifier_dmr_zoom_protection,
	_gameplay_modifier_dmr_damage,
	_gameplay_modifier_dmr_ammo_capacity,
	_gameplay_modifier_dmr_reload_speed,
	_gameplay_modifier_magnum_rounds_per_second,
	_gameplay_modifier_magnum_penetration,
	_gameplay_modifier_magnum_accuracy,
	_gameplay_modifier_magnum_damage,
	_gameplay_modifier_magnum_ammo_capacity,
	_gameplay_modifier_magnum_reload_speed,
	_gameplay_modifier_plasma_pistol_charge_up_time,
	_gameplay_modifier_plasma_pistol_damage,
	_gameplay_modifier_plasma_pistol_ammo_capacity,
	_gameplay_modifier_plasma_pistol_heat_per_round,
	_gameplay_modifier_frag_grenade_aoe_size,
	_gameplay_modifier_frag_grenade_damage,
	_gameplay_modifier_frag_grenade_ammo_capacity,
	_gameplay_modifier_plasma_grenade_aoe_size,
	_gameplay_modifier_plasma_grenade_damage,
	_gameplay_modifier_plasma_grenade_ammo_capacity,
	_gameplay_modifier_mauler_damage,
	_gameplay_modifier_spiker_damage,
	_gameplay_modifier_brute_shot_damage,
	_gameplay_modifier_plasma_rifle_heat_per_round,
	_gameplay_modifier_detonate_on_player_cdt,
	_gameplay_modifier_detonate_on_vehicle_cdt,
	_gameplay_modifier_enemies_always_on_radar,
	_gameplay_modifier_plant_plasma_on_death,
	_gameplay_modifier_shield_regen_rate_near_killed_enemies,
	_gameplay_modifier_shield_recharge_on_melee_kill,
	_gameplay_modifier_frag_grenades_max,
	_gameplay_modifier_plasma_grenades_max,
	_gameplay_modifier_claymore_grenade_aoe_size,
	_gameplay_modifier_smg_damage,
	_gameplay_modifier_smg_reload_speed,
	_gameplay_modifier_vehicle_autoflip,

	k_gameplay_modifier_count
};

enum e_player_respawn_failure
{
	_player_respawn_failure_none = 0,
	_player_respawn_failure_in_editor,
	_player_respawn_failure_cinematic_playing,
	_player_respawn_failure_scripting_says_it_isnt_safe,
	_player_respawn_failure_waiting_on_respawn_timer,
	_player_respawn_failure_friend_in_combat,
	_player_respawn_failure_nearby_enemies_pursuing_friend,
	_player_respawn_failure_friend_airborne_or_moving,
	_player_respawn_failure_friend_in_vehicle,
	_player_respawn_failure_everybody_is_dead,
	_player_respawn_failure_waiting_for_checkpoint,
	_player_respawn_failure_can_not_find_valid_location,

	k_player_respawn_failure_count
};

enum e_multiplayer_powerup_flavor
{
	_powerup_flavor_red = 0,
	_powerup_flavor_blue,
	_powerup_flavor_yellow,

	k_multiplayer_powerup_flavor_count,
	_powerup_flavor_none = -1
};

enum e_shell_application_type
{
	_shell_application_tool = 0,
	_shell_application_editor,
	_shell_application_game,

	k_shell_application_count
};

enum e_shell_tool_type
{
	_shell_tool_invalid = 0,
	_shell_tool_interactive,
	_shell_tool_command_line,

	k_shell_tool_count
};

enum e_init_file_type
{
	_init_file_type_editor = 0,
	_init_file_type_bonobo,
};


enum e_game_team
{
	_game_team_none = -1,

	_campaign_team_default = 0,
	_campaign_team_player,
	_campaign_team_human,
	_campaign_team_covenant,
	_campaign_team_flood,
	_campaign_team_sentinel,
	_campaign_team_heretic,
	_campaign_team_prophet,
	_campaign_team_guilty,
	_campaign_team_unused9,
	_campaign_team_unused10,
	_campaign_team_unused11,
	_campaign_team_unused12,
	_campaign_team_unused13,
	_campaign_team_unused14,
	_campaign_team_unused15,

	k_campaign_team_count,

	_multiplayer_team_first = 0,
	_multiplayer_team_red = _multiplayer_team_first,
	_multiplayer_team_blue,
	_multiplayer_team_green,
	_multiplayer_team_yellow,
	_multiplayer_team_purple,
	_multiplayer_team_orange,
	_multiplayer_team_brown,
	_multiplayer_team_grey,

	k_multiplayer_team_count,

	_multiplayer_team_last = _multiplayer_team_grey,
	_multiplayer_team_none = -1,

	k_maximum_teams = 16
};

enum e_button_preset
{
	_button_preset_standard = 0,
	_button_preset_south_paw,
	_button_preset_boxer,
	_button_preset_green_thumb,
	_button_preset_professional,
	_button_preset_walkie_talkie,

	k_number_of_button_presets
};

enum e_joystick_preset
{
	_joystick_preset_standard = 0,
	_joystick_preset_south_paw,
	_joystick_preset_legacy,
	_joystick_preset_legacy_south_paw,

	k_number_of_joystick_presets
};

enum e_voice_output_setting
{
	_voice_output_setting_default = 0,
	_voice_output_setting_headset,

	k_number_of_voice_output_settings
};

enum e_voice_mask
{
	_voice_mask_none = 0,
	_voice_mask_anonymous,

	k_number_of_voice_masks
};

enum e_actor_type
{
	_actor_elite = 0,
	_actor_jackal,
	_actor_grunt,
	_actor_hunter,
	_actor_engineer,
	_actor_assassin,
	_actor_player,
	_actor_marine,
	_actor_crew,
	_actor_combat_form,
	_actor_infection_form,
	_actor_carrier_form,
	_actor_monitor,
	_actor_sentinel,
	_actor_none,
	_actor_mounted_weapon,
	_actor_brute,
	_actor_prophet,
	_actor_bugger,
	_actor_juggernaut,
	_actor_pure_form_stealth,
	_actor_pure_form_tank,
	_actor_pure_form_ranged,
	_actor_scarab,
	_actor_guardian,

	k_number_of_actor_types
};

enum e_model_state
{
	_model_state_standard = 0,
	_model_state_minor_damage,
	_model_state_medium_damage,
	_model_state_major_damage,
	_model_state_destroyed,

	k_number_of_model_states
};

enum e_character_physics_override
{
	_character_physics_default = 0,
	_character_physics_player,
	_character_physics_biped,

	k_total_character_physics_overrides
};

enum e_ai_sound_volume
{
	_ai_sound_volume_silent = 0,
	_ai_sound_volume_medium,
	_ai_sound_volume_loud,
	_ai_sound_volume_shout,
	_ai_sound_volume_quiet,

	k_ai_sound_volume_count
};

enum e_game_engine_status_flags
{
	_game_engine_status_flags_unused_bit = 0,

	k_game_engine_status_flags
};

enum e_game_engine_status
{
	_game_engine_status_waiting_for_space_to_clear = 0,
	_game_engine_status_observing,
	_game_engine_status_respawning_soon,
	_game_engine_status_sitting_out,
	_game_engine_status_out_of_lives,
	_game_engine_status_playing_winning,
	_game_engine_status_playing_tied,
	_game_engine_status_playing_losing,
	_game_engine_status_game_over_won,
	_game_engine_status_game_over_tied,
	_game_engine_status_game_over_lost,
	_game_engine_status_game_over_lost_tied_game,
	_game_engine_status_you_have_flag,
	_game_engine_status_enemy_has_your_flag,
	_game_engine_status_flag_away_from_home,
	_game_engine_status_carrying_oddball,
	_game_engine_status_you_are_juggernaut,
	_game_engine_status_you_control_hill,
	_game_engine_status_switching_sides_soon,
	_game_engine_status_recently_started,
	_game_engine_status_you_have_bomb,
	_game_engine_status_flag_contested,
	_game_engine_status_bomb_contested,
	_game_engine_limited_lives_left_multiple,
	_game_engine_limited_lives_left_single,
	_game_engine_limited_lives_left_final,
	_game_engine_status_playing_winning_unlimited,
	_game_engine_status_playing_tied_unlimited,
	_game_engine_status_playing_losing_unlimited,

	// HO?
	_game_engine_status_in_lobby,

	k_game_engine_status_count
};

enum e_odst_recon_variant
{
	_odst_recon_variant_none = 0,
	_odst_recon_variant_rookie,
	_odst_recon_variant_buck,
	_odst_recon_variant_dare,
	_odst_recon_variant_dutch,
	_odst_recon_variant_johnson,
	_odst_recon_variant_mickey,
	_odst_recon_variant_romeo,

	k_odst_recon_variant_count
};

enum e_user_interface_sound_effect
{
	_ui_global_sound_effect_failure = 0,
	_ui_global_sound_effect_cursor_up_down,
	_ui_global_sound_effect_cursor_left_right,
	_ui_global_sound_effect_button_a,
	_ui_global_sound_effect_button_b,
	_ui_global_sound_effect_button_x,
	_ui_global_sound_effect_button_y,
	_ui_global_sound_effect_button_start,
	_ui_global_sound_effect_button_back,
	_ui_global_sound_effect_button_left_shoulder,
	_ui_global_sound_effect_button_right_shoulder,
	_ui_global_sound_effect_button_left_trigger,
	_ui_global_sound_effect_button_right_trigger,
	_ui_global_sound_effect_pregame_countdown_timer,
	_ui_global_sound_effect_pregame_countdown_timer_zero,
	_ui_global_sound_effect_alternate_countdown_timer,
	_ui_global_sound_effect_alternate_countdown_timer_zero,
	_ui_global_sound_effect_matchmaking,
	_ui_global_sound_effect_rank_up,
	_ui_global_sound_effect_matchmaking_party_up,

	k_number_of_global_ui_sound_effects
};

enum e_hopper_type
{
	_hopper_type_unranked_ffa = 0,
	_hopper_type_ranked_ffa,
	_hopper_type_unranked_teams,
	_hopper_type_ranked_teams,

	k_hopper_type_count
};

enum e_hopper_load_status
{
	_hopper_load_none = 0,
	_hopper_load_pending,
	_hopper_load_complete,
	_hopper_load_failed, // is this correct?

	k_hopper_load_count
};

enum e_network_file_load_status
{
	_network_file_load_none = 0,
	_network_file_load_in_progress,
	_network_files_load_complete,
	_network_files_load_error,

	k_network_file_load_count
};

enum e_player_color_index
{
	_player_color_00 = 0,
	_player_color_01,
	_player_color_02,
	_player_color_03,
	_player_color_04,
	_player_color_05,
	_player_color_06,
	_player_color_07,
	_player_color_08,
	_player_color_09,
	_player_color_10,
	_player_color_11,
	_player_color_12,
	_player_color_13,
	_player_color_14,
	_player_color_15,
	_player_color_16,
	_player_color_17,
	_player_color_18,
	_player_color_19,
	_player_color_20,
	_player_color_21,
	_player_color_22,
	_player_color_23,
	_player_color_24,
	_player_color_25,
	_player_color_26,
	_player_color_27,
	_player_color_28,
	_player_color_29,
	_player_color_30,
	_player_color_31,

	k_player_color_index_count,

	_player_color_none = -1,
	k_default_primary_color = _player_color_13,
	k_default_secondary_color = _player_color_13,
	k_default_tertiary_color = _player_color_13,
	k_default_emblem_icon_color1 = _player_color_27,
	k_default_emblem_icon_color2 = _player_color_10,
	k_default_emblem_background_color = _player_color_02,
};

enum e_network_interface_user_state
{
	_network_session_interface_user_state_none = 0,
	_network_session_interface_user_state_exists,
	_network_session_interface_user_state_zombie,
	_network_session_interface_user_state_error_online_permissions,
	_network_session_interface_user_state_error_user_content_not_permitted,
	_network_session_interface_user_state_error_generic,
	_network_session_interface_user_state_error_game_in_matchmaking,
	_network_session_interface_user_state_error_game_in_theater,
	_network_session_interface_user_state_error_game_in_campaign,
	_network_session_interface_user_state_error_game_full,
	_network_session_interface_user_state_error_no_split_screen_on_alpha,
	_network_session_interface_user_state_error_everybody_signed_out,
	_network_session_interface_user_state_error_not_signed_in,
	_network_session_interface_user_state_error_confirm_temporary,
	_network_session_interface_user_state_error_survival_in_progress,
};

enum e_session_network_message
{
	_network_message_network_interface_guide_opened = 0,
	_network_message_network_interface_guide_closed,
	_network_message_network_interface_connected,
	_network_message_network_interface_connection_lost,
	_network_message_connected_to_live,
	_network_message_lost_connection_to_live,
	_network_message_all_players_signed_out,
};

enum e_game_time_holiday
{
	_game_time_holiday_none = 0,
	_game_time_holiday_new_years,
	_game_time_holiday_earth_day,
	_game_time_cinco_de_mayo,
	_game_time_holiday_american_independence,
	_game_time_holiday_bungie_day,
	_game_time_holiday_american_halloween,
	_game_time_holiday_veterans_day,
	_game_time_holiday_kenyan_independence,
	_game_time_holiday_christmas,

	k_game_time_holiday_count
};

enum e_render_data_size
{
	k_render_data_size_small = 0x1400,
	k_render_data_size_normal = 0x7800,
	k_render_data_size_large = 0xF000,
	k_render_data_maximum_size = 0xF000,
};

extern bool __cdecl shell_application_is_paused();
extern void __cdecl shell_application_pause(bool pause);
extern e_shell_application_type __cdecl shell_application_type();
extern bool __cdecl shell_build_number_is_compatible(long build_number);
extern bool __cdecl shell_build_string_is_compatible(char const* build_string);
extern void __cdecl shell_dispose();
extern char const* __cdecl shell_get_target();
extern char const* __cdecl shell_get_target_variant();
extern void __cdecl shell_halt_on_pure_virtual_call();
extern void __cdecl shell_halt_with_message(char const* message);
extern bool __cdecl shell_initialize(bool windowed);
extern bool __cdecl game_is_bot_client();
extern bool __cdecl game_is_dedicated_server();
extern bool __cdecl game_is_client();
extern bool __cdecl game_is_guerilla();
extern bool __cdecl game_is_halo3();
extern bool __cdecl game_is_sapien();
extern bool __cdecl game_is_tool();
extern e_shell_tool_type __cdecl shell_tool_type();

extern char const* quality_setting_get_name(e_quality_setting quality_setting);
extern e_quality_setting quality_setting_from_string(char const* str);
extern char const* subtitle_setting_get_name(e_subtitle_setting subtitle_setting);
extern e_subtitle_setting subtitle_setting_from_string(char const* str);
extern char const* campaign_difficulty_level_get_name(e_campaign_difficulty_level difficulty);
extern e_campaign_difficulty_level campaign_difficulty_level_from_string(char const* str);
extern char const* network_session_mode_get_name(long session_mode);
extern char const* ui_game_mode_get_name(long ui_game_mode);
extern char const* gui_network_session_advertisement_mode_get_name(long advertisement_mode);
extern char const* game_variant_parameter_get_name(long parameter);

extern char const* const k_network_session_mode_names[k_network_session_mode_count];
extern char const* const k_ui_game_mode_names[k_ui_game_mode_count];
extern char const* const k_gui_network_session_advertisement_mode_names[k_gui_network_session_advertisement_mode_count];
extern char const* const k_game_variant_parameter_names[k_game_variant_parameter_count];
extern char const* const global_campaign_difficulty_level_names[k_number_of_campaign_difficulty_levels];
extern char const* const global_campaign_team_names[k_campaign_team_count];
extern char const* const global_multiplayer_team_names[k_multiplayer_team_count];
extern char const* const global_controller_index_names[k_number_of_controllers];
extern char const* const global_button_preset_names[k_number_of_button_presets];
extern char const* const global_joystick_preset_names[k_number_of_joystick_presets];
extern char const* const global_player_model_choice_names[k_number_of_player_model_choices];
extern char const* const global_voice_output_setting_names[k_number_of_voice_output_settings];
extern char const* const global_voice_mask_names[k_number_of_voice_masks];
extern char const* const global_subtitle_setting_names[k_number_of_subtitle_settings];
extern char const* const global_actor_type_names[k_number_of_actor_types];
extern char const* const global_model_state_names[k_number_of_model_states];
extern char const* const global_character_physics_override_names[k_total_character_physics_overrides];
extern char const* const global_primary_skull_names[k_number_of_primary_skulls];
extern char const* const global_secondary_skull_names[k_number_of_secondary_skulls];

