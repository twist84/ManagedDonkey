#pragma once

// `get_current_time_in_seconds`
enum
{
	_one_minute_in_seconds = 60,
	_one_hour_in_seconds = _one_minute_in_seconds * 60,
	_one_day_in_seconds = _one_hour_in_seconds * 24,
	_one_week_in_seconds = _one_day_in_seconds * 7,
	_one_year_in_seconds = _one_week_in_seconds * 52,
};

enum : tag
{
	_tag_none = 0xFFFFFFFF,
};

enum e_none_sentinel
{
	NONE = -1,
};

enum
{
	k_maximum_scenario_zone_set_count = 48,
};

enum
{
	k_invalid_storage_device_id = 0xCACACACA,
};

enum
{
	k_network_maximum_machines_per_session = 17,
	k_network_maximum_players_per_session = 16,
};

enum
{
	_campaign_player0 = 0,
	_campaign_player1,
	_campaign_player2,
	_campaign_player3,

	k_maximum_campaign_players,
	k_maximum_campaign_or_survival_players = k_maximum_campaign_players,

	k_network_maximum_sessions = 3,

	k_maximum_multiplayer_players = 16,

	k_maximum_game_tick_rate = 60,
	k_game_tick_rate_bits = 6,

	k_maximum_players = 16,
	k_maximum_player_index = 15,

	k_player_index_bits = 4,
	k_player_count_bits = 5,

	k_maximum_machines = 17,
	k_machine_count_bits = 5,
	k_maximum_machine_index = k_maximum_machines - 1,
	k_machine_index_bits = 4,

	k_maximum_rounds = 32,
	k_round_count_bits = 6,
	k_maximum_round_index = 31,
	k_round_index_bits = 5,
};

enum
{
	k_content_catalog_dlc_map_count = 50,
	k_content_catalog_maximum_number_of_core_files_per_user = 1,
	k_content_catalog_maximum_number_of_general_user_content = 100,
	k_content_catalog_maximum_number_of_user_screenshots = 50,
	k_content_catalog_count_per_box = 50,
	k_content_catalog_count_per_user = 151,
	k_content_catalog_enumeration_count = 151,

	k_maximum_thumbnail_png_data_size = 0x4000,
	k_personal_storage_content_item_required_size_bytes = 0xA00000,

	k_content_item_display_name_maximum_length = 128,
};

enum e_campaign_id
{
	_campaign_id_default = 1,
	_campaign_id_none = NONE,
};

enum e_map_id
{
	_map_id_first = 3005,
	_map_id_tutorial = 3010,
	_map_id_first_playable = 3010,

	_map_id_none = NONE,
	_map_id_use_scenario_path = -2,

	_map_id_mainmenu = 0x10231971,
};

enum e_gui_campaign_difficulty_setup_mode
{
	_campaign_difficulty_setup_mode_start_new_campaign = 0,
	_campaign_difficulty_setup_mode_replay_level,
	_campaign_difficulty_setup_mode_lobby,
};

enum e_gui_campaign_level_setup_mode
{
	_campaign_level_setup_mode_replay_level = 0,
	_campaign_level_setup_mode_lobby,
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
	k_game_engine_type_default = _game_engine_type_none,
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

	k_simulation_world_type_count,
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

	k_simulation_world_state_count,
};

enum e_update_queue_state
{
	_update_queue_state_normal = 0,
	_update_queue_state_peaking,
	_update_queue_state_throttling,
	_update_queue_state_cooling,

	k_update_queue_state_count,
};

enum e_simulation_update_flags
{
	_simulation_update_simulation_in_progress_bit = 0,
	_simulation_update_flush_gamestate_bit,
	_simulation_update_gamestate_flushed_outside_game_tick_bit,
	_simulation_update_game_simulation_queue_requires_application_bit,

	k_simulation_update_flags_count,
};

enum e_simulation_update_metadata_flags
{
	_simulation_update_from_local_simulation_bit = 0,
	_simulation_update_from_synchronous_update_bit,
	_simulation_update_from_saved_film_bit,

	k_simulation_update_metadata_flags_count,
};

enum e_update_queue_node
{
	_update_queue_node_update = 0,
	_update_queue_node_playback_event,

	k_update_queue_node_count,
	k_update_queue_node_invalid = -1,
};

enum e_simulation_playback_event
{
	_simulation_playback_event_revert = 0,

	k_simulation_playback_event_count,
};

enum e_scenario_camera_type
{
	_scenario_camera_type_target_relative = 0,
	_scenario_camera_type_ignore_target_orientation,
	_scenario_camera_type_dolly,
	_scenario_camera_type_ignore_target_updates,

	k_number_of_scenario_camera_types,
	_scenario_camera_type_default = _scenario_camera_type_target_relative,
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

	k_number_of_level_flags,
};

enum e_text_justification
{
	_text_justification_left = 0,
	_text_justification_right,
	_text_justification_center,

	k_text_justification_count,
};

enum e_text_vertical_justification
{
	_text_vertical_justification_default = 0,
	_text_vertical_justification_top,
	_text_vertical_justification_center,
	_text_vertical_justification_bottom,

	k_text_vertical_justification_count,
};

enum e_text_style
{
	_text_style_plain = -1,
	_text_style_bold,
	_text_style_italic,
	_text_style_condense,
	_text_style_underline,

	k_text_style_count,
};

enum e_text_drop_shadow_style
{
	_shadow_none = 0,
	_shadow_drop,
	_shadow_outline,

	k_number_of_text_drop_shadow_styles,
	k_default_drop_shadow_style = _shadow_drop,
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

	k_public_font_id_count,
};

enum e_session_creation_flags
{
	_session_creation_master_bit = 0,
	_session_creation_uses_presence_bit,
	_session_creation_uses_matchmaking_bit,
	_session_creation_uses_arbitration_bit,

	k_session_creation_flags_count,
};

enum
{
	k_number_of_users = 4,
};

enum e_network_game_type
{
	_network_game_type_none = 0,
	_network_game_type_custom_game,
	_network_game_type_matchmaking,
	_network_game_type_film,

	k_network_game_type_count,
};
//using c_session_creation_flags = c_flags<e_session_creation_flags, uns8, k_network_game_type_count>;

enum e_desired_multiplayer_mode
{
	_desired_multiplayer_mode_none = NONE,
	_desired_multiplayer_mode_coop,
	_desired_multiplayer_mode_custom,
	_desired_multiplayer_mode_matchmaking,
	_desired_multiplayer_mode_film,

	k_number_of_desired_multiplayer_modes,
};

enum e_session_game_mode
{
	_session_game_mode_none = 0,
	_session_game_mode_matchmaking,
	_session_game_mode_pregame,
	_session_game_mode_ingame,
	_session_game_mode_postgame,

	k_number_of_session_game_modes,
	k_number_of_session_game_bits = 3,
};

enum e_gui_game_mode
{
	_ui_game_mode_none = NONE,
	_ui_game_mode_campaign,
	_ui_game_mode_matchmaking,
	_ui_game_mode_multiplayer,
	_ui_game_mode_map_editor,
	_ui_game_mode_theater,
	_ui_game_mode_survival,

	k_gui_game_setup_mode_count,
	k_gui_game_setup_mode_bits = 3,
};

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_ui_shell,
	_game_mode_tool,

	k_game_mode_count,
	k_game_mode_bits = 3,
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_impossible,

	k_campaign_difficulty_levels_count,
	k_campaign_difficulty_level_bits = 2,
	k_campaign_difficulty_level_none = NONE,
};

enum e_campaign_metagame_scoring
{
	_metagame_off = 0,
	_metagame_on_group_scoring,
	_metagame_on,

	k_metagame_scoring_count,
	k_metagame_scoring_bits = 2,
};

enum e_saved_film_category
{
	_saved_film_category_none = 0,
	_saved_film_category_recent_films,
	_saved_film_category_film_clips,
	_saved_film_category_campaign,
	_saved_film_category_multiplayer,
	_saved_film_category_editor,

	// ODST
	_saved_film_category_survival,

	_saved_film_category_invalid,

	k_saved_film_category_count,
	k_saved_film_category_bits = 4,
};

enum e_metadata_file_type
{
	_metadata_file_type_film = 0,
	_metadata_file_type_film_clip,
	_metadata_file_type_map_variant,
	_metadata_file_type_screenshot,
	_metadata_file_type_game_variant,
};

enum e_button_presets
{
	_button_preset_first = 0,
	_button_preset_standard = _button_preset_first,
	_button_preset_south_paw,
	_button_preset_boxer,
	_button_preset_green_thumb,
	_button_preset_professional,
	_button_preset_walkie_talkie,

	k_button_presets_count,
};

enum e_camera_attached_horizontal
{
	_camera_attached_horizontal_normal = 0,
	_camera_attached_horizontal_inverted,

	k_number_of_camera_attached_horizontal_settings,
	k_default_camera_attached_horizontal_setting = _camera_attached_horizontal_normal,
};

enum e_camera_flying_movement
{
	_camera_flying_movement_slow = 0,
	_camera_flying_movement_normal,
	_camera_flying_movement_fast,
	_camera_flying_movement_faster,

	k_number_of_camera_flying_movement_settings,
	k_default_camera_flying_movement_setting = _camera_flying_movement_normal,
};

enum e_camera_flying_thrust
{
	_camera_flying_thrust_slow = 0,
	_camera_flying_thrust_normal,
	_camera_flying_thrust_fast,

	k_number_of_camera_flying_thrust_settings,
	k_default_camera_flying_thrust_setting = _camera_flying_thrust_normal,
};

enum e_camera_look_sensitivity
{
	_camera_look_sensitivity_slow = 0,
	_camera_look_sensitivity_normal,
	_camera_look_sensitivity_fast,
	_camera_look_sensitivity_very_fast,

	k_number_of_camera_look_sensitivity_settings,
	k_default_camera_look_sensitivity_setting = _camera_look_sensitivity_normal,
};

enum e_camera_panning
{
	_camera_panning_hold_to_pan = 0,
	_camera_panning_toggle,

	k_number_of_camera_panning_settings,
	k_default_camera_panning_setting = _camera_panning_hold_to_pan,
};

enum e_display_brightness
{
	_display_brightness_lower = 0,
	_display_brightness_low,
	_display_brightness_normal,
	_display_brightness_high,
	_display_brightness_higher,

	k_number_of_display_brightness_settings,
	k_default_display_brightness_setting = _display_brightness_normal,
};

enum e_guide_voice_through_speakers
{
	_guide_voice_through_speakers_off = 0,
	_guide_voice_through_speakers_on,
	_guide_voice_through_speakers_both,

	k_guide_voice_through_speakers_count,
};

enum e_joystick_presets
{
	_joystick_preset_first = 0,
	_joystick_preset_standard = _joystick_preset_first,
	_joystick_preset_south_paw,
	_joystick_preset_legacy,
	_joystick_preset_legacy_south_paw,

	k_joystick_presets_count,
};

enum e_look_sensitivity
{
	_look_sensitivity0 = 0,
	_look_sensitivity1,
	_look_sensitivity2,
	_look_sensitivity3,
	_look_sensitivity4,
	_look_sensitivity5,
	_look_sensitivity6,
	_look_sensitivity7,
	_look_sensitivity8,
	_look_sensitivity9,

	k_number_of_look_sensitivity_settings,
	k_default_look_sensitivity = _look_sensitivity2,
};

enum e_voice_matchmaking_setting
{
	_voice_matchmaking_setting_everyone = 0,
	_voice_matchmaking_setting_team_only,

	k_voice_matchmaking_setting_count,
	k_voice_matchmaking_setting_default = _voice_matchmaking_setting_everyone,
};

enum e_voice_mute_setting
{
	_voice_mute_setting_off = 0,
	_voice_mute_setting_on,

	k_voice_mute_setting_count,
	k_voice_mute_setting_default = _voice_mute_setting_off,
};

enum e_campaign_skulls_primary
{
	_campaign_skull_primary_iron = 0,
	_campaign_skull_primary_black_eye,
	_campaign_skull_primary_tough_luck,
	_campaign_skull_primary_catch,
	_campaign_skull_primary_fog,
	_campaign_skull_primary_famine,
	_campaign_skull_primary_thunderstorm,
	_campaign_skull_primary_tilt,
	_campaign_skull_primary_mythic,

	k_campaign_skull_primary_count,
};

enum e_campaign_skulls_secondary
{
	_campaign_skull_secondary_assassin = 0,
	_campaign_skull_secondary_blind,
	_campaign_skull_secondary_superman,
	_campaign_skull_secondary_grunt_birthday_party,
	_campaign_skull_secondary_IWHBYD,

	// ODST
	_campaign_skull_secondary_third_person,
	_campaign_skull_secondary_directors_cut,

	k_campaign_skull_secondary_count,
};

enum e_game_simulation_type
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_synchronous_client,
	_game_simulation_synchronous_server,
	_game_simulation_distributed_client,
	_game_simulation_distributed_server,

	k_game_simulation_count,
	k_game_simulation_bits = 3,
};

enum e_network_game_simulation_protocol
{
	_network_game_simulation_none = 0,
	_network_game_simulation_local,
	_network_game_simulation_sync_client,
	_network_game_simulation_sync_server,
	_network_game_simulation_dist_client,
	_network_game_simulation_dist_server,

	k_network_game_simulation_count,
};

enum e_simulation_abort_reason
{
	_simulation_abort_reason_exiting_in_game = 0,
	_simulation_abort_reason_exiting_in_match,
	_simulation_abort_reason_failed_to_find_next_level,
	_simulation_abort_reason_preparing_to_play_film,
	_simulation_abort_reason_reset_failed,
	_simulation_abort_reason_fatal_error,
	_simulation_abort_reason_lost_connection,
	_simulation_abort_reason_out_of_sync,
	_simulation_abort_reason_film_ended,
	_simulation_abort_reason_film_playback_error,

	k_simulation_abort_reason_count,
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

	k_simulation_view_reason_count,
};

enum e_simulation_view_type
{
	_simulation_view_type_none = 0,
	_simulation_view_type_synchronous_to_remote_authority,
	_simulation_view_type_synchronous_to_remote_client,
	_simulation_view_type_distributed_to_remote_authority,
	_simulation_view_type_distributed_to_remote_client,

	k_simulation_view_type_count,
};

enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_film,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count,
	k_game_playback_bits = 2,
};

enum e_content_item_type
{

};

enum e_player_model_choice
{
	_player_model_choice_first = 0,

	_player_model_choice_spartan = _player_model_choice_first,
	_player_model_choice_elite,

	k_player_model_choice_count
};

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

	_window0 = _player_window_first,
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
	_network_game_start_mode_none = 0,
	_network_game_start_mode_custom_game,
	_network_game_start_mode_matchmaking,
	_network_game_start_mode_film,

	k_network_game_start_mode_count,
	k_network_game_start_mode_bits = 2,
};

enum e_network_join_queue_mode
{
	_network_join_closed_to_all_joins = 0,
	_network_join_open_to_join_squad,
	_network_join_queue_joins_to_group,
	_network_join_process_queued_group_joins,
	_network_join_queue_closed_while_in_match,

	k_network_join_queue_mode_count,
};

enum e_networking_join_local_state
{
	_join_local_state_none = 0,
	_join_local_state_start_join_squad,
	_join_local_state_joining_squad,
	_join_local_state_leaving_old_squad,
	_join_local_state_processing_join_to_us,

	k_join_local_state_count,
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

	k_join_remote_state_count,
};

enum e_join_type
{
	_join_type_squad = 0,
	_join_type_group,

	k_join_type_count,
};

enum e_networking_join_destination_squad
{
	_join_destination_target = 0,
	_join_destination_group,

	k_join_destination_count,
	k_join_destination_bits = 1,
};

enum e_transport_platform
{
	_transport_platform_xenon = 0,
	_transport_platform_windows,

	k_transport_platform_count,
	k_transport_platform_bits = 2,
	k_transport_platform_none = NONE,
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

	k_network_channel_reason_count,
	k_network_channel_reason_bits = 5,
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

	k_network_join_refuse_reason_count,
	k_network_join_refuse_reason_bits = 6,
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

	k_network_session_boot_reason_count,
};

enum e_network_session_state
{
	_network_session_state_none = 0,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,

	//_network_session_state_host_handoff,
	//_network_session_state_host_reestablish,
	//_network_session_state_election,

	k_network_session_state_count,
	k_network_session_state_none = NONE,
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

	k_life_cycle_join_result_count,
	k_life_cycle_join_result_bits = 5,
};

enum e_network_session_type
{
	_network_session_type_none = 0,
	_network_session_type_squad,
	_network_session_type_group,

	k_network_session_type_count,
	k_network_session_type_bits = 2,
};

enum e_network_session_class
{
	_network_session_class_offline = 0,
	_network_session_class_system_link,
	_network_session_class_xbox_live,

	k_network_session_class_count,
	k_network_session_class_bits = 2,
	k_network_session_class_none = NONE,
};

enum e_network_observer_owner
{
	_network_observer_owner_first_session = 0,
	// 
	_network_observer_owner_last_session = 2,
	_network_observer_owner_simulation,
	k_network_observer_owner_count,

	_network_observer_owner_none = NONE,
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

	k_network_session_mode_count,
	k_network_session_mode_bits = 5,
};

enum e_matchmaking_search_preference
{
	_matchmaking_search_preference_none = 0,
	_matchmaking_search_preference_skill,
	_matchmaking_search_preference_good_connection,
	_matchmaking_search_preference_language,

	k_matchmaking_search_preference_count,
	k_matchmaking_search_preference_bits = 2,
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

	k_life_cycle_state_count,
	k_life_cycle_state_bits = 5,
};

enum e_life_cycle_matchmaking_progress_type
{
	_life_cycle_matchmaking_progress_none = 0,
	_life_cycle_matchmaking_progress_searching_for_match,
	_life_cycle_matchmaking_progress_assembling_match,
	_life_cycle_matchmaking_progress_setting_up_match,
	_life_cycle_matchmaking_progress_configuring_match,
	_life_cycle_matchmaking_progress_post_match,

	k_life_cycle_matchmaking_progress_type_count,
	k_life_cycle_matchmaking_progress_type_bits = 3,
};

enum e_network_rough_quality
{
	_network_rough_quality_none = 0,
	_network_rough_quality_good,
	_network_rough_quality_moderate,
	_network_rough_quality_bad,

	k_network_rough_quality_count,
	k_network_rough_quality_bits = 2,
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

	k_session_game_start_status_count,
	k_session_game_start_status_bits = 4,
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

	k_session_game_start_error_count,
	k_session_game_start_error_bits = 6,
};

enum e_network_session_map_status
{
	_network_session_map_status_none = 0,
	_network_session_map_status_failed,
	_network_session_map_status_precaching,
	_network_session_map_status_precached,
	_network_session_map_status_loaded,

	k_network_session_map_status_count,
	k_network_session_map_status_bits = 3,
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

	k_network_session_peer_state_count,
	k_network_session_peer_state_bits = 4,
};

enum e_gui_network_session_advertisement_mode
{
	_network_advertise_xbox_live_public = 0,
	_network_advertise_xbox_live_friends_only,
	_network_advertise_xbox_live_invite_only,
	_network_advertise_system_link,
	_network_advertise_nothing,

	k_network_session_advertisement_mode_count,
};

enum e_user_interface_session_leave_type
{
	_user_interface_session_leave_to_pre_game_lobby = 0,
	_user_interface_session_leave_to_main_menu,

	k_user_interface_session_leave_type_count,
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

enum e_peer_property_flag_test_type
{
	_peer_property_flag_test_all_peers = 0,
	_peer_property_flag_test_host,
	_peer_property_flag_test_all_peers_but_host,
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

	k_network_session_peer_properties_status_flag_count,
	k_network_session_peer_properties_status_flags_bits = 5
};
//using c_network_session_peer_properties_status_flags = c_flags<e_network_session_peer_properties_status_flags, uns32, k_network_session_peer_properties_status_flag_count>;

enum e_ui_message_type
{
	_message_type_invalid = -1,
	_message_type_error,
	_message_type_controller_input,
	_message_type_xenon,
	_message_type_load_screen,
	_message_type_change_display_group,
	_message_type_screen_custom,
	_message_type_dialog_result,
	_message_type_unknown7,

	k_number_of_ui_message_types,
};

enum e_browser_type
{
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

	_game_variant_ctf_top_category_options,
	_game_variant_slayer_top_category_options,
	_game_variant_oddball_top_category_options,
	_game_variant_king_top_category_options,
	_game_variant_sandbox_top_category_options,
	_game_variant_juggernaut_top_category_options,
	_game_variant_territories_top_category_options,
	_game_variant_assault_top_category_options,
	_game_variant_infection_top_category_options,
	_game_variant_vip_top_category_options,
	_game_variant_target_training_top_category_options,
	_game_variant_map_override_advanced_options,
	_game_variant_respawn_advanced_options,
	_game_variant_respawn_modifiers,

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

	k_number_of_editable_game_engine_category_groups,
	k_invalid_category = NONE,
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

	k_number_of_editable_game_variant_parameters,
	k_invalid_parameter = NONE,
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
	_game_team_none = NONE,

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
	_multiplayer_team_none = NONE,

	k_maximum_teams = 16
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

	_player_color_none = NONE,

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
enum
{
	k_max_activity_variants = 10
};

enum e_activity
{
	_activity_none = 0,
	_activity_patrol,
	_activity_stand,
	_activity_crouch,
	_activity_stand_drawn,
	_activity_crouch_drawn,
	_activity_corner,
	_activity_corner_open,
	_activity_bunker,
	_activity_bunker_open,
	_activity_combat,
	_activity_backup,
	_activity_guard,
	_activity_guard_crouch,
	_activity_guard_wall,
	_activity_typing,
	_activity_kneel,
	_activity_gaze,
	_activity_poke,
	_activity_sniff,
	_activity_track,
	_activity_watch,
	_activity_examine,
	_activity_sleep,
	_activity_at_ease,
	_activity_cower,
	_activity_tai_chi,
	_activity_pee,
	_activity_doze,
	_activity_eat,
	_activity_medic,
	_activity_work,
	_activity_cheering,
	_activity_injured,
	_activity_captured,

	k_activity_count
};

enum e_ai_posture
{
	_ai_posture_none = 0,
	_ai_posture_corner_cover_left,
	_ai_posture_corner_cover_right,
	_ai_posture_corner_open_left,
	_ai_posture_corner_open_right,
	_ai_posture_bunker_cover,
	_ai_posture_bunker_open,
	_ai_posture_act_combat,
	_ai_posture_act_backup_left,
	_ai_posture_act_backup_right,
	_ai_posture_act_asleep_1,
	_ai_posture_act_guard_1,
	_ai_posture_act_guard_2,
	_ai_posture_act_guard_3,
	_ai_posture_act_guard_4,
	_ai_posture_act_guard_crouch,
	_ai_posture_act_guard_crouch_1,
	_ai_posture_act_guard_wall_1,
	_ai_posture_act_typing,
	_ai_posture_act_kneel_1,
	_ai_posture_act_kneel_2,
	_ai_posture_act_gaze_1,
	_ai_posture_act_gaze_2,
	_ai_posture_act_poke_1,
	_ai_posture_act_sniff_1,
	_ai_posture_act_track_1,
	_ai_posture_act_watch_1,
	_ai_posture_act_examine_1,
	_ai_posture_asleep,
	_ai_posture_act_smoking,
	_ai_posture_act_cards,
	_ai_posture_act_cower_1,
	_ai_posture_act_cower_2,
	_ai_posture_act_tai_chi,
	_ai_posture_act_pee_1,
	_ai_posture_act_doze_sit_1,
	_ai_posture_act_doze_stand_1,
	_ai_posture_act_eat_1,
	_ai_posture_act_cheering_1,
	_ai_posture_act_cheering_2,
	_ai_posture_act_cheering_3,
	_ai_posture_act_cheering_4,
	_ai_posture_act_cheering_5,
	_ai_posture_act_injured_ankle,
	_ai_posture_act_injured_sit_wall,
	_ai_posture_act_injured_shoulder,
	_ai_posture_act_injured_gut,
	_ai_posture_act_laser,
	_ai_posture_act_laser1,
	_ai_posture_act_laser2,
	_ai_posture_act_laser3,
	_ai_posture_act_laser4,
	_ai_posture_act_laser5,
	_ai_posture_act_medic,
	_ai_posture_act_work,
	_ai_posture_act_work_stand,
	_ai_posture_act_captured_sit_wall,
	_ai_posture_act_captured_cuffed_wall,
	_ai_posture_act_captured_kneel,
	_ai_posture_act_bugger_lunch,

	k_ai_posture_count
};

enum e_posture_category
{
	_posture_category_none = 0,
	_posture_category_open,
	_posture_category_closed,

	k_posture_category_count
};

enum e_map_memory_configuration
{
	_map_memory_configuration_none = 0,
	_map_memory_configuration_main_menu,
	_map_memory_configuration_multiplayer,
	_map_memory_configuration_multiplayer_saved_film,
	_map_memory_configuration_campaign,
	_map_memory_configuration_campaign_saved_film,
	_map_memory_configuration_campaign_epilogue,

	k_map_memory_configuration_count
};

enum e_game_loaded_status
{
	_game_loaded_status_none = 0,
	_game_loaded_status_map_loading,
	_game_loaded_status_map_loaded,
	_game_loaded_status_map_loaded_failure,
	_game_loaded_status_map_unloading,
	_game_loaded_status_map_reloading,
	_game_loaded_status_pregame,

	k_game_loaded_status_count
};

enum e_cache_file_shared_file_type
{
	_cache_file_shared_file_type_ui = 0,
	_cache_file_shared_file_type_resources,
	_cache_file_shared_file_type_textures,
	_cache_file_shared_file_type_textures_b,
	_cache_file_shared_file_type_audio,
	_cache_file_shared_file_type_video,

	k_number_of_cache_file_shared_file_types
};

enum e_cache_file_section
{
	_cache_file_debug_section = 0,
	_cache_file_resource_section,
	_cache_file_tag_section,
	_cache_file_language_pack_section,

	k_number_of_cache_file_sections
};

enum e_map_file_index
{
	_map_file_index_shared_ui = 0,
	_map_file_index_shared_resources,
	_map_file_index_shared_textures,
	_map_file_index_shared_textures_b,
	_map_file_index_shared_audio,
	_map_file_index_shared_video,

	k_cached_map_file_shared_count = 6,
	k_total_tracked_cached_map_files_count = 15,
	k_no_cached_map_file_index = -1,
};

enum e_cache_file_load_action
{

};

enum e_cache_copy_state
{
	_cache_copy_state_idle = 0,
	_cache_copy_state_mount_dlc,
	_cache_copy_state_create_source_file,
	_cache_copy_state_verify_create_source_file,
	_cache_copy_state_read_dvd_header,
	_cache_copy_state_verify_read_dvd_header,
	_cache_copy_state_get_dvd_file_size,
	_cache_copy_state_verify_get_dvd_file_size,
	_cache_copy_state_find_free_map_and_clear_header,
	_cache_copy_state_verify_find_free_map_and_clear_header,
	_cache_copy_state_flush_clear_header,
	_cache_copy_state_start_copying_map_data,
	_cache_copy_state_copy_map_data,
	_cache_copy_state_verify_copy_map_data,
	_cache_copy_state_prepare_copy_thread,
	_cache_copy_state_kick_off_copy_thread,
	_cache_copy_state_finish_copy_from_thread,
	_cache_copy_state_flush_after_copy,
	_cache_copy_state_write_header,
	_cache_copy_state_verify_write_header,
	_cache_copy_state_mark_file_as_loaded,
	_cache_copy_state_halt,
	_cache_copy_state_close_source_file,
	_cache_copy_state_finish,

	k_number_of_cache_copy_states
};

enum e_cache_copy_finish_reason
{
	_cache_copy_finish_reason_invalid = 0,
	_cache_copy_finish_reason_copied,
	_cache_copy_finish_reason_canceled,
	_cache_copy_finish_reason_failed
};

enum e_cache_file_compression_codec
{
	_cache_file_compression_codec_none = NONE,
	_cache_file_compression_codec_lz,

	// custom compression codecs
	_cache_file_compression_codec_runtime_resource,
	_cache_file_compression_codec_runtime_tag_resource,

	k_cache_file_compression_codec_count
};

enum e_cache_file_resource_type
{
	_cache_file_resource_type_structure_bsp_cache_file_tag_resources = 0,
	_cache_file_resource_type_bitmap_texture_interop_resource,
	_cache_file_resource_type_bitmap_texture_interleaved_interop_resource,
	_cache_file_resource_type_sound_resource_definition,
	_cache_file_resource_type_model_animation_tag_resource,
	_cache_file_resource_type_render_geometry_api_resource_definition,
	_cache_file_resource_type_bink_resource,
	_cache_file_resource_type_structure_bsp_tag_resources,

	k_cache_file_resource_type_count
};

enum e_cache_file_resource_data_flags
{
	_cache_file_resource_data_flags_has_pageable_data = 0,
	_cache_file_resource_data_flags_has_optional_data,

	k_cache_file_resource_data_flags
};

enum
{
	k_maximum_async_task_data_size = 0x220
};

enum e_async_category
{
	_async_category_none = 0,
	_async_category_background_copy,
	_async_category_cache_file_misc,
	_async_category_cache_file_texture,
	_async_category_cache_file_geometry,
	_async_category_cache_file_sound,
	_async_category_cache_file_animation,
	_async_category_text,
	_async_category_saved_games,
	_async_category_online_files,

	k_async_category_count
};

enum e_async_priority
{
	_async_priority_background_idle_loading = 0,
	_async_priority_background_user_task,
	_async_priority_background_important_loading,
	_async_priority_background_important_user_task,
	_async_priority_predicted,
	_async_priority_important_non_blocking,
	_async_priority_blocking_below_sound,
	_async_priority_sound,
	_async_priority_very_important_non_blocking,
	_async_priority_blocking_generic,
	_async_priority_blocking_render,
	_async_priority_blocking_animation,

	k_async_priorities_count
};

enum e_async_completion
{
	_async_completion_retry = 0,
	_async_completion_done,
	_async_completion_thread_exit,

	k_async_completion_count
};

enum e_yield_reason
{
	_yield_for_unknown = 0,
	_yield_for_sound_cache,
	_yield_for_texture_cache,
	_yield_for_geometry_cache,
	_yield_for_font_cache
};

enum e_camera_mode
{
	_camera_mode_following = 0,
	_camera_mode_orbiting,
	_camera_mode_flying,
	_camera_mode_first_person,
	_camera_mode_dead,
	_camera_mode_static,
	_camera_mode_scripted,
	_camera_mode_authored,

	k_number_of_camera_modes,
	k_camera_mode_null = -1,
};

enum e_director_mode
{
	_director_mode_game = 0,
	_director_mode_saved_film,
	_director_mode_observer,
	_director_mode_debug,
	_director_mode_unused,
	_director_mode_editor,

	k_number_of_director_modes
};

enum e_director_perspective
{
	_director_perspective_first_person = 0,
	_director_perspective_third_person,
	_director_perspective_scripted,
	_director_perspective_neutral,

	k_number_of_director_perspectives,
};

enum e_write_position_flags
{
	_write_position_flush_file_bit = 0,

	k_write_position_flags
};

enum e_dvd_find_files_stage
{
	_dvd_find_files_start_stage = 0,
	_dvd_find_next_file_stage,

	k_dvd_find_files_stage_count
};

enum e_dlc_find_files_stage
{
	_dlc_begin_next_content_catalogue_stage = 0,
	_dlc_find_next_content_item,
	_dlc_find_files_start_stage,
	_dlc_find_next_file_stage,
	_dlc_find_next_content_catalogue_stage,

	k_dlc_find_files_stage_count
};

enum e_overlapped_task_state
{
	_overlapped_task_state_none = 0,
	_overlapped_task_state_starting,
	_overlapped_task_state_pending,
	_overlapped_task_state_completing,
	_overlapped_task_state_succeeded,
	_overlapped_task_state_failed,

	k_overlapped_task_state_count
};

enum e_effect_pass
{
	_effect_pass_opaque = 0,
	_effect_pass_transparents,
	_effect_pass_distortion,
	_effect_pass_first_person,

	k_effect_pass_count
};

enum
{
	_unit_estimate_none = 0,
	_unit_estimate_head_standing,
	_unit_estimate_head_crouching,
	_unit_estimate_gun_position,
	_unit_estimate_aiming_standing,
	_unit_estimate_aiming_crouching,

	k_unit_estimate_position_modes_count
};

enum e_unit_drop_type
{
	_unit_drop_type_default = 0,
	_unit_drop_type_delete,
	_unit_drop_type_dual_primary,
	_unit_drop_type_dual_secondary,
	_unit_drop_type_response_to_deletion,

	k_unit_drop_type_count,

	k_unit_drop_type_none = NONE
};

enum e_weapon_set
{
	_unit_weapon_set_primary = 0,
	_unit_weapon_set_secondary,

	k_unit_weapon_set_count
};

enum e_game_engine_symmetric_placement
{
	_game_engine_disregard_symmetry_for_placement = 0,
	_game_engine_symmetric_placement,
	_game_engine_asymmetric_placement,

	k_number_of_game_engine_symmetric_placement_settings
};

enum e_scenario_game_engine
{
	_scenario_game_engine_type_ctf = 0,
	_scenario_game_engine_type_slayer,
	_scenario_game_engine_type_oddball,
	_scenario_game_engine_type_king,
	_scenario_game_engine_type_juggernaut,
	_scenario_game_engine_type_territories,
	_scenario_game_engine_type_assault,
	_scenario_game_engine_type_vip,
	_scenario_game_engine_type_infection,
	_scenario_game_engine_type_target_training,

	k_scenario_game_engine_type_count
};

enum e_multiplayer_team_designator
{
	_multiplayer_team_designator_defender = 0,
	_multiplayer_team_designator_attacker,
	_multiplayer_team_designator_third_party,
	_multiplayer_team_designator_fourth_party,
	_multiplayer_team_designator_fifth_party,
	_multiplayer_team_designator_sixth_party,
	_multiplayer_team_designator_seventh_party,
	_multiplayer_team_designator_eigth_party,
	_multiplayer_team_designator_neutral,

	k_multiplayer_team_designator_count
};

enum e_teleporter_channel
{
	_channel_first = 0,
	_channel_alpha = _channel_first,
	_channel_bravo,
	_channel_charlie,
	_channel_delta,
	_channel_echo,
	_channel_foxtrot,
	_channel_golf,
	_channel_hotel,
	_channel_india,
	_channel_juliet,
	_channel_kilo,
	_channel_lima,
	_channel_mike,
	_channel_november,
	_channel_oscar,
	_channel_papa,
	_channel_quebec,
	_channel_romeo,
	_channel_sierra,
	_channel_tango,
	_channel_uniform,
	_channel_victor,
	_channel_whiskey,
	_channel_xray,
	_channel_yankee,
	_channel_zulu,

	k_teleporter_channel_count,
	k_channel_default = _channel_first,
};
enum e_game_results_medal
{
	_game_results_medal_extermination = 0,
	_game_results_medal_perfection,
	_game_results_medal_multiple_kill_2,
	_game_results_medal_multiple_kill_3,
	_game_results_medal_multiple_kill_4,
	_game_results_medal_multiple_kill_5,
	_game_results_medal_multiple_kill_6,
	_game_results_medal_multiple_kill_7,
	_game_results_medal_multiple_kill_8,
	_game_results_medal_multiple_kill_9,
	_game_results_medal_multiple_kill_10,
	_game_results_medal_5_kills_in_a_row,
	_game_results_medal_10_kills_in_a_row,
	_game_results_medal_15_kills_in_a_row,
	_game_results_medal_20_kills_in_a_row,
	_game_results_medal_25_kills_in_a_row,
	_game_results_medal_30_kills_in_a_row,
	_game_results_medal_sniper_kill_5,
	_game_results_medal_sniper_kill_10,
	_game_results_medal_shotgun_kill_5,
	_game_results_medal_shotgun_kill_10,
	_game_results_medal_collision_kill_5,
	_game_results_medal_collision_kill_10,
	_game_results_medal_sword_kill_5,
	_game_results_medal_sword_kill_10,
	_game_results_medal_juggernaut_kill_5,
	_game_results_medal_juggernaut_kill_10,
	_game_results_medal_zombie_kill_5,
	_game_results_medal_zombie_kill_10,
	_game_results_medal_human_kill_5,
	_game_results_medal_human_kill_10,
	_game_results_medal_human_kill_15,
	_game_results_medal_koth_kill_5,
	_game_results_medal_shotgun_kill_sword,
	_game_results_medal_vehicle_impact_kill,
	_game_results_medal_vehicle_hijack,
	_game_results_medal_aircraft_hijack,
	_game_results_medal_deadplayer_kill,
	_game_results_medal_player_kill_spreeplayer,
	_game_results_medal_spartanlaser_kill,
	_game_results_medal_stickygrenade_kill,
	_game_results_medal_sniper_kill,
	_game_results_medal_bashbehind_kill,
	_game_results_medal_bash_kill,
	_game_results_medal_flame_kill,
	_game_results_medal_driver_assist_gunner,
	_game_results_medal_assault_bomb_planted,
	_game_results_medal_assault_player_kill_carrier,
	_game_results_medal_vip_player_kill_vip,
	_game_results_medal_juggernaut_player_kill_juggernaut,
	_game_results_medal_oddball_carrier_kill_player,
	_game_results_medal_ctf_flag_captured,
	_game_results_medal_ctf_flag_player_kill_carrier,
	_game_results_medal_ctf_flag_carrier_kill_player,
	_game_results_medal_infection_survive,
	_game_results_medal_nemesis,
	_game_results_medal_avenger,
	_game_results_medal_unused3,

	k_game_results_medal_count
};

enum e_game_results_damage_statistic
{
	_game_results_damage_statistic_damage_kills = 0,
	_game_results_damage_statistic_damage_deaths,
	_game_results_damage_statistic_damage_betrayals,
	_game_results_damage_statistic_damage_suicides,
	_game_results_damage_statistic_damage_headshots,

	k_game_results_damage_statistic_count
};

enum e_game_results_statistic
{
	_game_results_statistic_games_played = 0,
	_game_results_statistic_games_completed,
	_game_results_statistic_games_won,
	_game_results_statistic_games_tied,
	_game_results_statistic_rounds_completed,
	_game_results_statistic_rounds_won,
	_game_results_statistic_in_round_score,
	_game_results_statistic_in_game_total_score,
	_game_results_statistic_kills,
	_game_results_statistic_assists,
	_game_results_statistic_deaths,
	_game_results_statistic_betrayals,
	_game_results_statistic_suicides,
	_game_results_statistic_most_kills_in_a_row,
	_game_results_statistic_seconds_alive,
	_game_results_statistic_ctf_flag_scores,
	_game_results_statistic_ctf_flag_grabs,
	_game_results_statistic_ctf_flag_carrier_kills,
	_game_results_statistic_ctf_flag_returns,
	_game_results_statistic_assault_bomb_arms,
	_game_results_statistic_assault_bomb_grabs,
	_game_results_statistic_assault_bomb_disarms,
	_game_results_statistic_assault_bomb_detonations,
	_game_results_statistic_oddball_time_with_ball,
	_game_results_statistic_oddball_unused,
	_game_results_statistic_oddball_kills_as_carrier,
	_game_results_statistic_oddball_ball_carrier_kills,
	_game_results_statistic_king_time_on_hill,
	_game_results_statistic_king_total_control_time,
	_game_results_statistic_king_unused0,
	_game_results_statistic_king_unused1,
	_game_results_statistic_unused0,
	_game_results_statistic_unused1,
	_game_results_statistic_unused2,
	_game_results_statistic_vip_takedowns,
	_game_results_statistic_vip_kills_as_vip,
	_game_results_statistic_vip_guard_time,
	_game_results_statistic_vip_time_as_vip,
	_game_results_statistic_vip_lives_as_vip,
	_game_results_statistic_juggernaut_kills,
	_game_results_statistic_juggernaut_kills_as_juggernaut,
	_game_results_statistic_juggernaut_total_control_time,
	_game_results_statistic_total_wp,
	_game_results_statistic_juggernaut_unused,
	_game_results_statistic_territories_owned,
	_game_results_statistic_territories_captures,
	_game_results_statistic_territories_ousts,
	_game_results_statistic_territories_time_in_territory,
	_game_results_statistic_infection_zombie_kills,
	_game_results_statistic_infection_infections,
	_game_results_statistic_infection_time_as_human,

	k_game_results_statistic_count
};

enum e_game_results_event_type
{
	_game_results_event_type_none = 0,
	_game_results_event_type_kill,
	_game_results_event_type_carry,
	_game_results_event_type_score,

	k_game_results_event_type_count
};

enum e_simulation_event_type
{
	_simulation_event_type_damage_aftermath = 0,
	_simulation_event_type_damage_section_response,
	_simulation_event_type_breakable_surface_damage,
	_simulation_event_type_projectile_attached,
	_simulation_event_type_projectile_detonate,
	_simulation_event_type_projectile_impact_effect,
	_simulation_event_type_projectile_object_impact_effect,
	_simulation_event_type_effect_on_pos,
	_simulation_event_type_game_engine,
	_simulation_event_type_unit_board_vehicle,
	_simulation_event_type_unit_pickup,
	_simulation_event_type_weapon_effect,
	_simulation_event_type_weapon_empty_click,
	_simulation_event_type_hit_marker,
	_simulation_event_type_unit_exit_vehicle,
	_simulation_event_type_unit_assassinate,
	_simulation_event_type_player_taunt_request,
	_simulation_event_type_weapon_fire,
	_simulation_event_type_unit_equipment_use,
	_simulation_event_type_weapon_reload,
	_simulation_event_type_unit_throw_initiate,
	_simulation_event_type_unit_melee_initiate,
	_simulation_event_type_weapon_pickup,
	_simulation_event_type_weapon_put_away,
	_simulation_event_type_weapon_drop,
	_simulation_event_type_vehicle_flip,
	_simulation_event_type_vehicle_trick,
	_simulation_event_type_device_touch,
	_simulation_event_type_unit_throw_release,
	_simulation_event_type_unit_melee_damage,
	_simulation_event_type_unit_melee_clang,
	_simulation_event_type_unit_enter_vehicle,
	_simulation_event_type_game_engine_request_boot_player,
	_simulation_event_type_player_respawn_request,
	_simulation_event_type_player_force_base_respawn,
	_simulation_event_type_unit_equipment_pickup,
	_simulation_event_type_projectile_supercombine_request,
	_simulation_event_type_object_refresh,
	_simulation_event_type_player_editor_request,

	k_simulation_event_type_count,

	_simulation_event_type_hs_script_wake = k_simulation_event_type_count,

	k_simulation_event_type_count_plus_additional,

	k_simulation_event_type_none = NONE,
	k_simulation_event_type_maximum_count = 64
};

enum e_simulation_entity_type
{
	_simulation_entity_type_slayer = 0,
	_simulation_entity_type_ctf,
	_simulation_entity_type_assault,
	_simulation_entity_type_oddball,
	_simulation_entity_type_king,
	_simulation_entity_type_territories,
	_simulation_entity_type_juggernaut,
	_simulation_entity_type_sandbox,
	_simulation_entity_type_infection,
	_simulation_entity_type_vip,
	_simulation_entity_type_game_engine_player,
	_simulation_entity_type_game_statborg,
	_simulation_entity_type_breakable_surface_group,
	_simulation_entity_type_map_variant,
	_simulation_entity_type_unit,
	_simulation_entity_type_item,
	_simulation_entity_type_generic,
	_simulation_entity_type_generic_garbage,
	_simulation_entity_type_vehicle,
	_simulation_entity_type_projectile,
	_simulation_entity_type_weapon,
	_simulation_entity_type_device,

	k_simulation_entity_type_count,

	k_simulation_entity_type_none = NONE,
	k_simulation_entity_type_maximum_count = 32
};

enum e_object_type
{
	_object_type_biped = 0,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_equipment,
	_object_type_arg_device,
	_object_type_terminal,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_sound_scenery,
	_object_type_crate,
	_object_type_creature,
	_object_type_giant,
	_object_type_effect_scenery,

	k_object_type_count,

	k_object_type_none = -1
};

enum
{
	k_error_message_buffer_maximum_size = 2048,
	k_spamming_event_display_timeout = 3000,
};

enum e_event_level
{
	_event_verbose = 0,
	_event_status,
	_event_message,
	_event_warning,
	_event_error,
	_event_critical,

	k_event_level_count,

	k_event_level_none = NONE,
};

enum e_category_properties_flags
{
	_category_properties_display_level_bit = 0,
	_category_properties_force_display_level_bit,
	_category_properties_log_level_bit,
	_category_properties_remote_log_level_bit, // datamine
	_category_properties_debugger_break_level_bit,
	_category_properties_halt_level_bit,
	_category_properties_event_listener_bit,

	k_category_properties_flags_count
};

enum e_reference_type
{
	_hs_reference_type_global = 0,
	_hs_reference_type_script
};

enum
{
	k_maximum_number_of_references = 10240,
};

enum e_hs_type
{
	_hs_unparsed = 0,
	_hs_special_form,
	_hs_function_name,
	_hs_passthrough,

	// actual types start
	_hs_type_void,
	_hs_type_boolean,
	_hs_type_real,
	_hs_type_short_integer,
	_hs_type_long_integer,
	_hs_type_string,
	_hs_type_script,
	_hs_type_string_id,
	_hs_type_unit_seat_mapping,
	_hs_type_trigger_volume,
	_hs_type_cutscene_flag,
	_hs_type_cutscene_camera_point,
	_hs_type_cutscene_title,
	_hs_type_cutscene_recording,
	_hs_type_device_group,
	_hs_type_ai,
	_hs_type_ai_command_list,
	_hs_type_ai_command_script,
	_hs_type_ai_behavior,
	_hs_type_ai_orders,
	_hs_type_ai_line,
	_hs_type_starting_profile,
	_hs_type_conversation,
	_hs_type_zone_set,
	_hs_type_designer_zone,
	_hs_type_point_ref,
	_hs_type_style,
	_hs_type_object_list,
	_hs_type_folder,

	// tag reference type
	_hs_type_sound,
	_hs_type_effect,
	_hs_type_damage,
	_hs_type_looping_sound,
	_hs_type_animation_graph,
	_hs_type_damage_effect,
	_hs_type_object_definition,
	_hs_type_bitmap,
	_hs_type_shader,
	_hs_type_render_model_definition,
	_hs_type_structure_bsp_definition,
	_hs_type_structure_lightmap_definition,
	_hs_type_cinematic_definition,
	_hs_type_cinematic_scene_definition,
	_hs_type_bink_definition,
	_hs_type_any_tag,
	_hs_type_any_tag_not_resolving,

	// enum
	_hs_type_enum_game_difficulty,
	_hs_type_enum_team,
	_hs_type_enum_mp_team,
	_hs_type_enum_controller,
	_hs_type_enum_button_preset,
	_hs_type_enum_joystick_preset,
	//_hs_type_enum_player_color,
	_hs_type_enum_player_character_type,
	_hs_type_enum_voice_output_setting,
	_hs_type_enum_voice_mask,
	_hs_type_enum_subtitle_setting,
	_hs_type_enum_actor_type,
	_hs_type_enum_model_state,
	_hs_type_enum_event,
	_hs_type_enum_character_physics_override,
	_hs_type_enum_primary_skull,
	_hs_type_enum_secondary_skull,

	// object
	_hs_type_object,
	_hs_type_unit,
	_hs_type_vehicle,
	_hs_type_weapon,
	_hs_type_device,
	_hs_type_scenery,
	_hs_type_effect_scenery,

	// object name
	_hs_type_object_name,
	_hs_type_unit_name,
	_hs_type_vehicle_name,
	_hs_type_weapon_name,
	_hs_type_device_name,
	_hs_type_scenery_name,
	_hs_type_effect_scenery_name,

	_hs_type_cinematic_lightprobe,

	// budget reference
	_hs_type_budget_reference_animation_graph,
	_hs_type_budget_reference_looping_sound,
	_hs_type_budget_reference_sound,

	NUMBER_OF_HS_NODE_TYPES,

	FIRST_HS_TYPE = _hs_type_void,

	FIRST_HS_TAG_REFERENCE_TYPE = _hs_type_sound,
	LAST_HS_TAG_REFERENCE_TYPE = _hs_type_any_tag,
	NUMBER_OF_HS_TAG_REFERENCE_TYPES = LAST_HS_TAG_REFERENCE_TYPE - FIRST_HS_TAG_REFERENCE_TYPE + 1,

	FIRST_HS_ENUM_TYPE = _hs_type_enum_game_difficulty,
	LAST_HS_ENUM_TYPE = _hs_type_enum_secondary_skull,
	NUMBER_OF_HS_ENUM_TYPES = LAST_HS_ENUM_TYPE - FIRST_HS_ENUM_TYPE + 1,

	FIRST_HS_OBJECT_TYPE = _hs_type_object,
	LAST_HS_OBJECT_TYPE = _hs_type_effect_scenery,
	NUMBER_OF_HS_OBJECT_TYPES = LAST_HS_OBJECT_TYPE - FIRST_HS_OBJECT_TYPE + 1,

	FIRST_HS_OBJECT_NAME_TYPE = _hs_type_object_name,
	LAST_HS_OBJECT_NAME_TYPE = _hs_type_effect_scenery_name,

	FIRST_HS_BUDGET_REFERENCE_TYPE = _hs_type_budget_reference_animation_graph,
	LAST_HS_BUDGET_REFERENCE_TYPE = _hs_type_budget_reference_sound,
	NUMBER_OF_HS_BUDGET_REFERENCE_TYPES = LAST_HS_BUDGET_REFERENCE_TYPE - FIRST_HS_BUDGET_REFERENCE_TYPE + 1,
};

enum e_hs_script_type
{
	_hs_script_startup = 0,
	_hs_script_dormant,
	_hs_script_continuous,
	_hs_script_static,
	_hs_script_command_script,
	_hs_script_stub,

	NUMBER_OF_HS_SCRIPT_TYPES
};

enum e_core_widget_definition_flags
{
	_core_widget_definition_flag_do_not_apply_old_content_upscaling_bit = 0,
	_core_widget_definition_flag_override_template_flags_bit,
	_core_widget_definition_flag_enable_animation_debugging_bit,

	k_core_widget_definition_flags
};

enum e_widget_anchoring
{
	_anchor_UNUSED = 0,
	_anchor_center,
	_anchor_top,
	_anchor_bottom,
	_anchor_left,
	_anchor_right,
	_anchor_top_left,
	_anchor_top_right,
	_anchor_bottom_right,
	_anchor_bottom_left,

	k_number_of_widget_anchor_settings
};

enum e_gui_widget_type
{
	_gui_text = 0,
	_gui_bitmap,
	_gui_model,
	_gui_group,
	_gui_button_key,
	_gui_list_item,
	_gui_slider,
	_gui_list,
	_gui_screen,

	k_number_of_gui_element_types,
	k_gui_widget_type_invalid = -1
};

enum e_animation_state
{
	_first_animation_state = 0,
	_focused_ambient = _first_animation_state,
	_unfocused_ambient,
	_focused_disabled_ambient,
	_unfocused_disabled_ambient,
	_alternate_focused_ambient,
	_alternate_unfocused_ambient,
	_transition_from_screen,
	_transition_to_screen,
	_transition_back_from_screen,
	_transition_back_to_screen,
	_cycle_in_previous_screen,
	_cycle_in_next_screen,
	_cycle_out_previous_screen,
	_cycle_out_next_screen,
	_list_display_group_transition_in,
	_list_display_group_transition_out,
	_control_received_focus,
	_control_lost_focus,
	_indicator_ambient_additional_items,
	_indicator_ambient_no_additional_items,
	_indicator_activated_additional_items,
	_indicator_activated_no_additional_items,
	_load_submenu_focused,
	_load_submenu_unfocused,
	_unload_submenu_focused,
	_unload_submenu_unfocused,
	_load_as_submenu,
	_unload_as_submenu,
	_child_submenu_ambient_focused,
	_child_submenu_ambient_unfocused,
	_custom_animation0,
	_custom_animation1,
	_custom_screen_transition_in0,
	_custom_screen_transition_out0,
	_custom_screen_transition_in1,
	_custom_screen_transition_out1,

	// ODST
	_custom_display_group_in,
	_custom_display_group_out,
	_transition_to_overlaid,
	_transition_from_overlaid,

	// H3MCC
	//_mouse_enter,
	//_mouse_leave,
	//_mouse_hover_ambient,

	k_number_of_ui_animation_states,
	k_invalid_animation_state = NONE,
};

enum e_vehicle_trick_type
{
	_vehicle_trick_type_none = 0,
	_vehicle_trick_type_left_barrel_roll,
	_vehicle_trick_type_right_barrel_roll,
	_vehicle_trick_type_back_loop,

	k_vehicle_trick_type_count
};

enum
{
	k_latest_font_package_header_version = 0xC0000003,

	k_font_package_file_size = 0x8000,
	k_font_package_entry_count = 8,
	k_font_count = 16,

	k_latest_font_header_version = 0xF0000005,
	k_font_header_kerning_pair_index_count = 256,
};

enum e_font_index
{
	_font_index_none = NONE,
	_font_index_fallback = -2,

	k_maximum_font_index_count = 10
};

enum
{
	_package_cache_status_line_package_status = 0,
	_package_cache_status_line_load_stats,

	k_package_cache_status_line_count
};

enum e_font_package_status
{
	_font_package_unavailable = 0,
	_font_package_delayed_loading,
	_font_package_loading,
	_font_package_ready,

	k_font_package_status_count
};

enum e_character_status
{
	_character_status_invalid = 0,
	_character_status_loading,
	_character_status_ready,

	k_character_status_count
};

enum e_font_cache_flags
{
	_font_cache_block_bit = 0,
	_font_cache_load_bit,
	_font_cache_predicted_bit,

	k_font_cache_flag_count
};

enum
{
	_network_message_type_variable_length_bit = 0,
	_network_message_type_message_without_body_bit,

	k_network_message_type_flags_count
};

enum e_network_synchronous_playback_control
{
	_network_synchronous_playback_control_revert = 0,
	_network_synchronous_playback_control_end_playback,

	k_network_synchronous_playback_control_count
};

enum e_simulation_view_establishment_mode
{
	_simulation_view_establishment_mode_none = 0,
	_simulation_view_establishment_mode_unknown1,
	_simulation_view_establishment_mode_unknown2,
	_simulation_view_establishment_mode_unknown3,
	_simulation_view_establishment_mode_joining,
	_simulation_view_establishment_mode_unknown5,

	k_simulation_view_establishment_mode_count
};

enum e_simulation_view_synchronous_catchup_stage
{
	_synchronous_catchup_not_in_progress = 0,
	_synchronous_catchup_intiate,
	_synchronous_catchup_preparing_for_gamestate,
	_synchronous_catchup_in_progress,
	_synchronous_catchup_finish,
	_synchronous_catchup_client_decompressing_gamestate,

	k_synchronous_catchup_stage_count
};

enum e_synchronous_gamestate_message_type
{
	_synchronous_gamestate_message_initiate_join = 0,
	_synchronous_gamestate_message_gamestate_chunk,
	_synchronous_gamestate_message_checksums,

	k_number_of_synchronous_gamestate_message_types
};

enum
{
	k_max_decoded_event_data_reference_count = 2,
	k_max_decoded_event_data_payload_size = 1024
};

enum e_simulation_object_update_flag
{
	_simulation_object_update_dead = 0,
	_simulation_object_update_position,
	_simulation_object_update_forward_and_up,
	_simulation_object_update_scale,
	_simulation_object_update_translational_velocity,
	_simulation_object_update_angular_velocity,
	_simulation_object_update_body_vitality, // vitality & stun ticks
	_simulation_object_update_shield_vitality,
	_simulation_object_update_region_state,
	_simulation_object_update_constraint_state,
	_simulation_object_update_owner_team_index,
	_simulation_object_update_parent_state,
	_simulation_object_update_multiplayer_properties,
	_simulation_object_update_map_variant_index,

	k_simulation_object_update_flag_count
};

enum e_simulation_unit_update_flag
{
	_simulation_unit_update_control = k_simulation_object_update_flag_count,
	_simulation_unit_update_parent_vehicle,
	_simulation_unit_update_desired_aiming_vector,
	_simulation_unit_update_desired_weapon_set,
	_simulation_unit_update_weapon1_type,
	_simulation_unit_update_weapon2_type,
	_simulation_unit_update_weapon3_type,
	_simulation_unit_update_weapon4_type,
	_simulation_unit_update_weapon1_state,
	_simulation_unit_update_weapon2_state,
	_simulation_unit_update_weapon3_state,
	_simulation_unit_update_weapon4_state,
	_simulation_unit_update_grenade_counts,
	_simulation_unit_update_active_camo,
	_simulation_unit_update_equipment,
	_simulation_unit_update_equipment_charges,
	_simulation_unit_update_consumable_energy,
	_simulation_unit_update_control_context,
	_simulation_unit_update_assassination_data,

	k_simulation_unit_update_flag_count
};

enum e_simulation_item_update_flag
{
	_simulation_item_update_set_at_rest = k_simulation_object_update_flag_count,
	_simulation_item_update_equipment_begin_animation_state,
	_simulation_item_update_equipment_creation_time,
	_simulation_item_update_equipment_owner,

	k_simulation_item_update_flag_count
};

enum e_simulation_projectile_update_flag
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,

	k_simulation_projectile_update_flag_count
};

enum e_simulation_generic_update_flag
{
	_simulation_generic_update_rigid_body = k_simulation_object_update_flag_count,

	k_simulation_generic_update_flag_count
};

enum e_simulation_weapon_update_flag
{
	_simulation_weapon_update_weapon_flags = k_simulation_item_update_flag_count,
	_simulation_weapon_update_multiplayer_weapon_registration,
	_simulation_weapon_update_ammo,

	k_simulation_weapon_update_flag_count
};

enum e_simulation_device_update_flag
{
	_simulation_device_update_position = k_simulation_object_update_flag_count,
	_simulation_device_update_position_group,
	_simulation_device_update_power,
	_simulation_device_update_power_group,

	k_simulation_device_update_flag_count
};

enum e_simulation_vehicle_update_flag
{
	_simulation_vehicle_update_auto_turret = k_simulation_object_update_flag_count,
	_simulation_vehicle_update_auto_turret_tracking,
	_simulation_vehicle_update_seat_power,
	_simulation_vehicle_update_active_camo,

	k_simulation_vehicle_update_flag_count
};

enum e_saved_game_flags
{
	_last_game_film_saved_by_user_bit = 0,

	k_saved_game_flags_count
};

enum e_critical_sections
{
	// CS:Event Logs
	k_crit_section_event_logs = 0,

	// CS:Data Mine
	k_crit_section_data_mine,

	// CS:D3D_Device
	k_crit_section_rasterizer_device,

	// CS:Terminal
	k_crit_section_terminal,

	// CS:Network Debug
	k_crit_section_network_debug,

	// CS:Render Toggle
	k_crit_section_render_thread_toggle_lock,

	// CS:Event RW Lock
	k_crit_section_event_rw_lock,

	// CS:Header Region
	k_crit_section_header_region,

	// CS:Header Section
	k_crit_section_header_subsection,

	// CS:Update Region
	k_crit_section_update_region,

	// CS:Update Section
	k_crit_section_update_subsection,

	// CS:Render Region
	k_crit_section_render_region,

	// CS:Render Section
	k_crit_section_render_subsection,

	// CS:Shared Region
	k_crit_section_shared_region,

	// CS:Shared Section
	k_crit_section_shared_subsection,

	// CS:Shared Mirror0
	k_crit_section_shared_subsection_mirror0,

	// CS:Shared Mirror1
	k_crit_section_shared_subsection_mirror1,

	// CS:Shared Mirror2
	k_crit_section_shared_subsection_mirror2,

	// CS:UI widgets
	k_crit_section_ui_widgets_lock,

	// CS:UI memory
	k_crit_section_ui_memory_lock,

	// CS:UI custom bitmaps
	k_crit_section_ui_custom_bitmaps_lock,

	// CS:Saved Game
	k_crit_section_saved_game,

	// CS:Levels
	k_crit_section_levels,

	// CS:Autosave Queue
	k_crit_section_autosave_queue,

	// CS:XOverlapped
	k_crit_section_async_xoverlapped,

	// CS:XOverlapped Memory
	k_crit_section_async_xoverlapped_memory,

	// CS:Game Engine
	k_crit_section_game_engine,

	// CS:Http Request Queue
	k_crit_section_http_request_queue,

	// CS:LSP Manager
	k_crit_section_lsp_manager,

	// CS:Online files
	k_crit_section_online_files,

	// CS:Telnet console
	k_crit_section_telnet_console,

	// CS:G-Render Region
	k_crit_section_global_render_region,

	// CS:G-Render Section
	k_crit_section_global_render_subsection,

	// CS:Bink Prediction
	k_crit_section_bink_prediction,

	// CS:Bink Texture 0
	k_crit_section_bink_texture0,

	// CS:Bink Texture 1
	k_crit_section_bink_texture1,

	// CS:Bink Memory
	k_crit_section_bink_memory,

	// CS:Font Cache
	k_crit_section_font_cache,

	// CS:Sound Cache
	k_crit_section_sound_cache,

	// CS:Tag Cache
	k_crit_section_tag_cache,

	// CS:Partition Cache
	k_crit_section_partition_cache,

	// CS:Disk Cache
	k_crit_section_disk_cache,

	// CS:Test Cache
	k_crit_section_test_cache,

	// CS:Prof Display
	k_crit_section_profiler_display,

	// CS:Prof Session
	k_crit_section_profiler_datamine_update,

	// CS:Resource RW Lock
	k_crit_section_resource_threading_access_rw_lock,

	// CS:Simple Resource Access RW Lock
	k_crit_section_simple_resource_cache_access_rw_lock,

	// CS:Resource Publish
	k_crit_section_resource_cache_publish,

	// CS:Remote Console
	k_crit_section_remote_console_commands,

	// CS:Observer Updates
	k_crit_section_observer_updates,

	// CS:Web Cache
	k_crit_section_web_cache,

	// CS:Prof-Gather
	k_crit_section_performance_data_gathering,

	// CS:Prof-Register
	k_crit_section_performance_data_registration,

	// CS:Optional Cache
	k_crit_section_optional_cache,

	// CS:Net Block Detection
	k_crit_section_network_block_detection,

	// CS:Net Statistics
	k_crit_section_network_statistics,

	// CS:DSP-Radio
	k_crit_section_dsp_radio_effect,

	// CS:Global Prefs
	k_crit_section_global_preferences,

	// CS:Texture Render
	k_crit_section_texture_render,

	k_total_critical_sections,

	k_invalid_critical_section = NONE
};

enum e_synchronization_mutexes
{
	// MTX:AsyncFree
	k_mutex_async_free_list = 0,

	// MTX:AsyncWork
	k_mutex_async_work_list,

	// MTX:DataMine
	k_mutex_data_mining,

	k_total_synchronization_mutexes
};

enum e_synchronization_events
{
	// EV:CSeriesEvent
	k_event_cseries_event_log = 0,

	// EV:UploadDebug
	k_event_upload_debug,

	// EV:NetworkDebug
	k_event_render_network_debug_exit,

	// EV:VBlankUpdate
	k_event_main_time_update_vblank,

	// EV:VBlankThrottle
	k_event_main_time_throttle_vblank,

	// EV:SoundRenderDeferredStart
	k_event_sound_render_deferred_start,

	// EV:SoundRenderDeferredComplete
	k_event_sound_render_deferred_complete,

	// EV:UpdateThreadComplete
	k_event_update_thread_complete,

	// EV:UpdateThreadBegin
	k_event_update_thread_begin,

	// EV:UpdateThreadEnd
	k_event_update_thread_end,

	// EV:BinkFree
	k_event_bink_free,

	// EV:MirrorsChanged
	k_event_mirrors_changed,

	k_total_synchronization_events
};

enum e_synchronization_semaphore
{
	// S4:AsyncWork
	k_semaphore_async_work = 0,

	// S4:EventRW
	k_semaphore_event_rw_lock,

	// S4:ResourceRW
	k_semaphore_resource_threading_access_rw_lock,

	// S4:SRPublishRW
	k_semaphore_simple_resource_cache_publish_rw_lock,

	// S4:Bink Memory
	k_semaphore_bink_memory,

	k_total_synchronization_semaphores,

	k_invalid_semaphore = NONE
};

enum e_saved_game_file_type
{
	_saved_game_file_type_first = 0,
	_saved_game_campaign = _saved_game_file_type_first,

	_saved_game_file_type_first_game_variant,

	_saved_game_variant_ctf = _saved_game_file_type_first_game_variant,
	_saved_game_variant_slayer,
	_saved_game_variant_oddball,
	_saved_game_variant_king,
	_saved_game_variant_juggernaut,
	_saved_game_variant_territories,
	_saved_game_variant_assault,
	_saved_game_variant_infection,
	_saved_game_variant_vip,

	_saved_game_file_type_last_game_variant = _saved_game_variant_vip,

	_saved_game_map_variant,
	_saved_game_film,
	_saved_game_film_clip,
	_saved_game_screen_shot,

	k_saved_game_file_type_count,
	k_saved_game_file_type_any = k_saved_game_file_type_count,
	k_saved_game_file_type_any_game_variant,
	k_saved_game_file_type_none = NONE,
	k_saved_game_file_type_bits = 5,
};

enum e_vertex_type
{
	_vertex_type_world = 0,
	_vertex_type_rigid,
	_vertex_type_skinned,
	_vertex_type_particle_model,
	_vertex_type_flat_world,
	_vertex_type_flat_rigid,
	_vertex_type_flat_skinned,
	_vertex_type_screen,
	_vertex_type_debug,
	_vertex_type_transparent,
	_vertex_type_particle,
	_vertex_type_contrail,
	_vertex_type_light_volume,
	_vertex_type_simple_chud,
	_vertex_type_fancy_chud,
	_vertex_type_decorator,
	_vertex_type_tiny_position,
	_vertex_type_patchy_fog,
	_vertex_type_water,
	_vertex_type_ripple,
	_vertex_type_implicit,
	_vertex_type_beam,
	_vertex_type_dual_quat,

	k_number_of_vertex_types
};

enum e_lighting_vertex_types
{
	// default
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_none, _transfer_vertex_none)
	// _entry_point_static_lighting_prt_ambient
	// _entry_point_static_lighting_prt_linear
	// _entry_point_static_lighting_prt_quadratic
	// _entry_point_active_camo
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_none, transfer_vertex_type)
	_lighting_vertex_none = 0,

	// _entry_point_vertex_color_lighting
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_vert_color, _transfer_vertex_none)
	_lighting_vertex_vert_color,

	// _entry_point_static_lighting_per_pixel
	// _entry_point_lightmap_debug_mode
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_lightmap_uv, _transfer_vertex_none)
	_lighting_vertex_lightmap_uv,

	// _entry_point_static_lighting_per_vertex
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_per_vertex_lightprobe, _transfer_vertex_none)
	_lighting_vertex_per_vertex_lightprobe,

	k_number_of_lighting_vertex_types
};

enum e_transfer_vector_vertex_types
{
	_transfer_vertex_none = 0,
	_transfer_prt_ambient_only,
	_transfer_prt_linear,
	_transfer_prt_quadratic,

	k_number_of_transfer_vertex_types
};

enum e_entry_point
{
	_entry_point_default = 0,
	_entry_point_albedo,
	_entry_point_static_lighting_default,
	_entry_point_static_lighting_per_pixel,
	_entry_point_static_lighting_per_vertex,
	_entry_point_static_lighting_sh,
	_entry_point_static_lighting_prt_ambient,
	_entry_point_static_lighting_prt_linear,
	_entry_point_static_lighting_prt_quadratic,
	_entry_point_dynamic_light,
	_entry_point_shadow_generate,
	_entry_point_shadow_apply,
	_entry_point_active_camo,
	_entry_point_lightmap_debug_mode,
	_entry_point_vertex_color_lighting,
	_entry_point_water_tessellation,
	_entry_point_water_shading,
	_entry_point_dynamic_lighting_cinematic,
	_entry_point_z_only,
	_entry_point_sfx_distort,

	k_number_of_entry_points
};

enum e_rasterizer_constant_table_type
{
	_rasterizer_constant_table_type_vertex = 0,
	_rasterizer_constant_table_type_pixel,

	// MCC
	//_rasterizer_constant_table_type_compute,

	k_rasterizer_constant_table_type_count
};

enum e_transport_qos_type
{
	// probe only
	_transport_qos_type_probe_only = 0,

	// default (full)
	_transport_qos_type_default,

	k_transport_qos_type_count,
	k_transport_qos_type_none = NONE
};

enum e_network_session_qos_status_data_type
{
	_network_session_qos_status_data_none = 0,
	_network_session_qos_status_data_lobby_or_game,
	_network_session_qos_status_data_matchmaking,

	k_network_session_qos_status_data_type_count,
	k_network_session_qos_status_data_type_bits = 2
};

enum e_session_tracker_flags
{
	_session_tracker_initialized_bit = 0,
	_session_tracker_sort_required_bit,

	k_session_tracker_flags_count
};

enum e_network_session_tracker_sort_method
{
	_network_session_tracker_sort_none = 0,
	_network_session_tracker_sort_desirability,

	k_network_session_tracker_sort_method_count,
};

enum e_session_tracker_session_flags
{
	_session_tracker_session_valid_bit = 0,
	_session_tracker_session_refresh_bit,
	_session_tracker_session_unsuitable_bit,
	_session_tracker_session_undesireable_bit,

	k_session_tracker_session_flags_count
};

enum e_network_session_tracker_session_status
{
	_tracked_session_status_none = 0,
	_tracked_session_qos_needed,
	_tracked_session_completed_probe_only_qos,
	_tracked_session_completed_full_qos,
	_tracked_session_undesireable,
	_tracked_session_unsuitable,

	k_tracked_session_status_count,
};

enum e_network_session_tracker_session_undesirable_reason
{
	_session_undesirable_reason_none = 0,
	_session_undesirable_latency,

	k_network_session_tracker_session_undesirable_reason_count,
};

enum e_network_session_tracker_session_unsuitable_reason
{
	_session_unsuitable_reason_none = 0,
	_session_unsuitable_qos_refused,
	_session_unsuitable_qos_unreachable,
	_session_unsuitable_qos_failed_to_get_result,
	_session_unsuitable_qos_payload_empty,
	_session_unsuitable_qos_payload_failed_to_decode,
	_session_unsuitable_qos_payload_wrong_type,
	_session_unsuitable_qos_timeout,
	_session_unsuitable_qos_incompatible_version,
	_session_unsuitable_previously_marked_unsuitable,
	_session_unsuitable_join_failed,
	_session_unsuitable_failed_to_get_session_data,

	k_network_session_tracker_session_unsuitable_reason_count,
};

enum e_transport_qos_target_status
{
	_transport_qos_target_status_pending = 0,
	_transport_qos_target_status_in_progress_wait,
	_transport_qos_target_status_in_progress_partial,
	_transport_qos_target_status_unreachable,
	_transport_qos_target_status_refused,
	_transport_qos_target_status_succeeded,

	k_transport_qos_target_status_count,
	k_transport_qos_target_status_none = NONE,
};

enum e_parameter_change_request_access
{
	_parameter_change_request_access_default = 0,
	_parameter_change_request_access_leader,
	_parameter_change_request_access_peer,
	k_parameter_change_request_access_count,

	k_parameter_change_request_access_none = NONE,
};

enum e_network_session_parameter_flags
{
	_network_session_parameter_valid_bit = 0,
	_network_session_parameter_desires_change_request_bit,

	k_network_session_parameter_flags_count
};

enum e_parameter_update_priority
{
	_parameter_update_priority_default = 0,
	_parameter_update_priority_high,
	k_parameter_update_priority_count,

	k_parameter_update_priority_none = NONE,
};

enum e_network_session_parameter_type
{
	_network_session_parameter_type_session_size = 0,
	_network_session_parameter_type_session_mode,
	_network_session_parameter_type_privacy_mode,
	_network_session_parameter_type_ui_game_mode,
	_network_session_parameter_type_dedicated_server_session_state,
	_network_session_parameter_type_start_mode,
	_network_session_parameter_type_ui_state,
	_network_session_parameter_type_end_game,
	_network_session_parameter_type_map,
	_network_session_parameter_type_initial_participants,
	_network_session_parameter_type_game_instance,
	_network_session_parameter_type_random_seed,
	_network_session_parameter_type_language,
	_network_session_parameter_type_determinism_version,
	_network_session_parameter_type_simulation_protocol,
	_network_session_parameter_type_campaign_difficulty,
	_network_session_parameter_type_campaign_ative_primary_skulls,
	_network_session_parameter_type_campaign_ative_secondary_skulls,
	_network_session_parameter_type_campaign_metagame_scoring,
	_network_session_parameter_type_campaign_insertion_point,
	_network_session_parameter_type_game_variant,
	_network_session_parameter_type_map_variant,
	_network_session_parameter_type_saved_film,
	_network_session_parameter_type_saved_film_game_options,
	_network_session_parameter_type_game_start_status,
	_network_session_parameter_type_countdown_timer,
	_network_session_parameter_type_voice_repeater,
	_network_session_parameter_type_requested_remote_join_data,
	_network_session_parameter_type_remote_join_data,
	_network_session_parameter_type_matchmaking_arbitration_nonce,
	_network_session_parameter_type_matchmaking_hopper_list,
	_network_session_parameter_type_matchmaking_game_list,
	_network_session_parameter_type_matchmaking_requested_hopper,
	_network_session_parameter_type_matchmaking_hopper,
	_network_session_parameter_type_matchmaking_abort_requested,
	_network_session_parameter_type_matchmaking_search_preference,
	_network_session_parameter_type_matchmaking_progress,
	_network_session_parameter_type_matchmaking_progress_search_criteria,
	_network_session_parameter_type_matchmaking_peer_evict_mask,
	_network_session_parameter_type_matchmaking_rematch_data,
	_network_session_parameter_type_matchmaking_hopper_statistics,
	_network_session_parameter_type_matchmaking_tip,
	_network_session_parameter_type_matchmaking_suppress_progress,
	_network_session_parameter_type_synchronous_out_of_sync,
	_network_session_parameter_type_matchmaking_messaging,
	_network_session_parameter_type_host_frame_quality,
	_network_session_parameter_type_request_campaign_quit,
	_network_session_parameter_type_leader_properties,
	_network_session_parameter_type_lobby_vote_set,

	k_network_session_parameter_type_count,
	k_network_session_parameter_type_none = NONE,
};

enum e_network_message_type
{
	_network_message_ping = 0,
	_network_message_pong,
	_network_message_broadcast_search,
	_network_message_broadcast_reply,
	_network_message_connect_request,
	_network_message_connect_refuse,
	_network_message_connect_establish,
	_network_message_connect_closed,
	_network_message_join_request,
	_network_message_peer_connect,
	_network_message_join_abort,
	_network_message_join_refuse,
	_network_message_leave_session,
	_network_message_leave_acknowledge,
	_network_message_session_disband,
	_network_message_session_boot,
	_network_message_host_decline,
	_network_message_peer_establish,
	_network_message_time_synchronize,
	_network_message_membership_update,
	_network_message_peer_properties,
	_network_message_delegate_leadership,
	_network_message_boot_machine,
	_network_message_player_add,
	_network_message_player_refuse,
	_network_message_player_remove,
	_network_message_player_properties,
	_network_message_parameters_update,
	_network_message_parameters_request,
	_network_message_view_establishment,
	_network_message_player_acknowledge,
	_network_message_synchronous_update,
	_network_message_synchronous_playback_control,
	_network_message_synchronous_actions,
	_network_message_synchronous_acknowledge,
	_network_message_synchronous_gamestate,
	_network_message_distributed_game_results,
	_network_message_synchronous_client_ready,
	_network_message_test,

	k_old_network_message_type_count,

	_custom_network_message_text_chat = k_old_network_message_type_count,
	_custom_network_message_directed_search,

	k_network_message_type_count,

	k_custom_network_message_count = (k_network_message_type_count - k_old_network_message_type_count)
};

enum e_online_managed_session_flags
{
	_online_managed_session_allocated_bit = 0,
	_online_managed_session_master_bit,
	_online_managed_session_pending_retry_bit,
	_online_managed_session_force_create_offline_bit,
	_online_managed_session_created_bit,
	_online_managed_session_creation_failed_bit,
	_online_managed_session_host_migration_in_process_bit,
	_online_managed_session_host_migration_session_created_bit,
	_online_managed_session_host_migration_must_delete_session_bit,
	_online_managed_session_host_migration_using_new_session_bit,
	_online_managed_session_players_add_pending_bit,
	_online_managed_session_players_add_succeeded_bit,
	_online_managed_session_players_add_failed_bit,
	_online_managed_session_game_started_bit,
	_online_managed_session_game_start_failed_bit,
	_online_managed_session_game_ended_bit,
	_online_managed_session_game_end_failed_bit,
	_online_managed_session_locking_flags_bit,
	_online_managed_session_flags_locked_bit,

	k_online_managed_session_flags_count
};

enum e_online_managed_session_operation_flags
{
	_online_managed_session_delete_bit = 0,
	_online_managed_session_free_bit,
	_online_managed_session_create_bit,
	_online_managed_session_delete_host_migration_bit,
	_online_managed_session_create_host_migration_bit,
	_online_managed_session_recreate_players_bit,
	_online_managed_session_players_remove_bit,
	_online_managed_session_modify_session_bit,
	_online_managed_session_players_add_bit,
	_online_managed_session_game_end_bit,
	_online_managed_session_game_start_bit,

	k_online_managed_session_operation_flags_count
};

enum e_network_http_request_queue_type
{
	// REQ LSP
	_network_http_request_queue_type_required = 0,

	// OPT LSP
	_network_http_request_queue_type_optional,

	k_network_http_request_queue_type_count
};

enum e_network_http_request_result
{
	_network_http_request_result_none = 0,
	_network_http_request_result_success,
	_network_http_request_result_failure,
	_network_http_request_result_reset,

	k_network_http_request_result_count
};

enum e_network_http_request_queue_failure_reason
{
	_network_http_request_queue_failure_reason_unknown = 0,
	_network_http_request_queue_failure_reason_file_not_found,
	_network_http_request_queue_failure_reason_bad_request,
	_network_http_request_queue_failure_reason_internal_server_error
};

enum e_http_request_type
{
	// GET
	_http_request_type_get = 0,

	// POST
	_http_request_type_post
};

enum e_client_usage_type
{
	_client_usage_type_stream = 0,
	_client_usage_type_fill_buffer
};

enum e_online_lsp_service_type
{
	// H3
	/* ttl */ _online_lsp_service_type_title_files = 0,
	/* usr */ _online_lsp_service_type_user_files,
	/* shr */ _online_lsp_service_type_mass_storage,
	/* dbg */ _online_lsp_service_type_debug,
	/* web */ _online_lsp_service_type_webstats,

	// HO
	/* mmk */ _online_lsp_service_type_mmk, // matchmaking?
	/* amm */ _online_lsp_service_type_amm, // ?
	/* prs */ _online_lsp_service_type_prs, // presence from reach?
	/* ofr */ _online_lsp_service_type_ofr, // offers?

	k_online_lsp_service_type_count
};

enum e_online_lsp_state
{
	_online_lsp_state_search_start = 0,
	_online_lsp_state_searching,
	_online_lsp_state_search_succeeded,
	_online_lsp_state_search_failed,

	k_online_lsp_state_count
};

enum
{
	k_title_id = 0x4D530877
};

enum e_online_session_player_flags
{
	_online_session_player_valid_bit = 0,
	_online_session_player_occupies_private_slot_bit,
	//_online_session_player_is_online_enabled_bit,

	k_online_session_player_flags_count
};

enum e_online_nat_type
{
	_online_nat_type_unknown = 0,
	_online_nat_type_open,
	_online_nat_type_moderate,
	_online_nat_type_strict,

	k_online_nat_type_count
};

enum e_network_channel_state
{
	_network_channel_state_none = 0,
	_network_channel_state_empty,
	_network_channel_state_closed,
	_network_channel_state_connecting,
	_network_channel_state_established,
	_network_channel_state_connected,

	k_network_channel_state_count
};

enum e_main_reset_events_reason
{
	_main_reset_events_map_change = 0,
	_main_reset_events_pending_xsync,

	k_number_of_main_reset_event_reasons
};

enum e_game_state_revert_bit
{
	_game_state_revert_user_bit = 0,
	_game_state_revert_by_scripting_bit,
	_game_state_revert_keep_playing_cinematic_outros_bit,

	k_number_of_game_state_revert_bits
};

enum e_game_state_proc_flags
{
	_flush_physics_environment_bit = 0,
	_flush_networking_bit,
	_rebuild_players_bit,
	_doesnt_effect_location_after_death_bit,
	_restored_by_scripting_bit,
	_keep_playing_cinematic_outros_bit,
	_dont_cancel_pending_game_save_bit,
	_dont_rebuild_havok_bit,
	_use_insecure_signature_bit,
	_defer_writing_signature_bit,
	_disable_campaign_metagame_bit,
	_dont_save_gpu_effect_data_bit,

	k_game_state_proc_flags_count,
};

enum e_single_threaded_request_flags
{
	_single_thread_for_user_request = 0,
	_single_thread_for_physics_debug,
	_single_thread_for_actor_debug,
	_single_thread_for_ai_profile,
	_single_thread_for_hs_debug,
	_single_thread_for_unknown5,
	_single_thread_for_unknown6,
	_single_thread_for_unknown7,
	_single_thread_for_render_device_reset
};

enum e_render_thread_mode
{
	_render_thread_mode_disabled = 0,
	_render_thread_mode_enabled,
	_render_thread_mode_loading_screen,

	k_render_thread_mode_count
};

enum e_main_pregame_frame
{
	_main_pregame_frame_none = 0,
	_main_pregame_frame_normal,
	_main_pregame_frame_loading_debug,
	_main_pregame_frame_cache_loading,
	_main_pregame_frame_crash_uploading,
	_main_pregame_frame_crash_done,
	_main_pregame_frame_upload,
	_main_pregame_frame_notify_out_of_sync,
	_main_pregame_frame_loading_screen,

	k_main_pregame_frame_count
};

enum e_map_load_status
{
	_map_load_status_loaded = 3,
};

enum e_basic_loading_phase
{
	_basic_loading_phase_none = NONE,

	k_basic_loading_phase_count = 3
};

enum
{
	k_max_campaign_insertion_points = 9
};

enum e_campaign_flags
{
	_campaign_from_dlc_bit = 0,
	_campaign_unlockable_bit,

	k_number_of_campaign_flags
};

enum e_status_line_flags
{
	_status_line_blink_bit,
	_status_line_inhibit_drawing_bit,
	_status_line_left_justify_bit,
	_status_line_draw_once_bit,

	k_status_line_count
};

enum e_gui_selected_item_location
{
	_gui_stored_item_location_none = NONE,
	_gui_stored_item_location_built_in,
	_gui_stored_item_location_saved_game_file,
	_gui_stored_item_location_autosave_queue,

	k_gui_stored_item_location_count
};

enum e_gui_selected_item_type
{
	_gui_selection_type_none = NONE,
	_gui_selection_type_level,
	_gui_selection_type_difficulty,
	_gui_selection_type_map,
	_gui_selection_type_game_variant,
	_gui_selection_type_hopper,
	_gui_selection_type_film,
	_gui_selection_type_screenshot,
	_gui_selection_type_network_mode,

	k_gui_selected_item_type_count
};

enum e_gui_alert_flags
{
	_gui_alert_flag_allow_auto_dismissal_bit = 0,
	_gui_alert_flag_show_spinner_bit,

	k_gui_alert_flags
};

enum e_gui_error_category
{
	_gui_error_category_default = 0,
	_gui_error_category_networking,
	_gui_error_category_storage,
	_gui_error_category_controller,

	k_gui_error_category_count
};

enum e_gui_error_icon
{
	_gui_error_icon_default_alert = 0,
	_gui_error_icon_downloading,
	_gui_error_icon_paused,
	_gui_error_icon_uploading,
	_gui_error_icon_completed,

	k_gui_error_icon_count
};

enum e_online_leaderboard_id
{
	_online_leaderboard_id_skill = 0,
	_online_leaderboard_id_global_arbitrated,
	_online_leaderboard_id_global_unarbitrated,

#define DEFINE_HOPPER(ID) \
_online_leaderboard_id_hopper_##ID, \
_online_leaderboard_id_hopper_##ID##_ranked_skill, \
_online_leaderboard_id_hopper_##ID##_social_skill

	DEFINE_HOPPER(0),
	DEFINE_HOPPER(1),
	DEFINE_HOPPER(2),
	DEFINE_HOPPER(3),
	DEFINE_HOPPER(4),
	DEFINE_HOPPER(5),
	DEFINE_HOPPER(6),
	DEFINE_HOPPER(7),
	DEFINE_HOPPER(8),
	DEFINE_HOPPER(9),
	DEFINE_HOPPER(10),
	DEFINE_HOPPER(11),
	DEFINE_HOPPER(12),
	DEFINE_HOPPER(13),
	DEFINE_HOPPER(14),
	DEFINE_HOPPER(15),
	DEFINE_HOPPER(16),
	DEFINE_HOPPER(17),
	DEFINE_HOPPER(18),
	DEFINE_HOPPER(19),
	DEFINE_HOPPER(20),
	DEFINE_HOPPER(21),
	DEFINE_HOPPER(22),
	DEFINE_HOPPER(23),
	DEFINE_HOPPER(24),
	DEFINE_HOPPER(25),
	DEFINE_HOPPER(26),
	DEFINE_HOPPER(27),
	DEFINE_HOPPER(28),
	DEFINE_HOPPER(29),
	DEFINE_HOPPER(30),
	DEFINE_HOPPER(31),

#undef DEFINE_HOPPER

	k_online_leaderboard_id_count, // 99
	k_online_leaderboard_id_none = NONE,
};

enum e_online_leaderboard_column_id
{
	_online_leaderboard_column_id_skill_mu = 0,
	_online_leaderboard_column_id_skill_sigma,
	_online_leaderboard_column_id_global_unarbitrated_custom_games_completed,
	_online_leaderboard_column_id_global_unarbitrated_custom_games_won,
	_online_leaderboard_column_id_global_arbitrated_experience_base,
	_online_leaderboard_column_id_global_arbitrated_experience_penalty,
	_online_leaderboard_column_id_global_arbitrated_highest_skill_level_attained,
	_online_leaderboard_column_id_global_arbitrated_matchmade_ranked_games_completed,
	_online_leaderboard_column_id_global_arbitrated_matchmade_ranked_games_played,
	_online_leaderboard_column_id_global_arbitrated_matchmade_ranked_games_won,
	_online_leaderboard_column_id_global_arbitrated_matchmade_unranked_games_completed,
	_online_leaderboard_column_id_global_arbitrated_matchmade_unranked_games_played,
	_online_leaderboard_column_id_global_arbitrated_matchmade_unranked_games_won,
	_online_leaderboard_column_id_global_arbitrated_first_game_played_date,
	_online_leaderboard_column_id_global_arbitrated_last_game_played_date,

#define DEFINE_HOPPER(ID) \
_online_leaderboard_column_id_hopper_##ID##_skill, \
_online_leaderboard_column_id_hopper_##ID##_games_completed, \
_online_leaderboard_column_id_hopper_##ID##_games_played, \
_online_leaderboard_column_id_hopper_##ID##_games_won, \
_online_leaderboard_column_id_hopper_##ID##_experience_base, \
_online_leaderboard_column_id_hopper_##ID##_experience_penalty

	DEFINE_HOPPER(0),
	DEFINE_HOPPER(1),
	DEFINE_HOPPER(2),
	DEFINE_HOPPER(3),
	DEFINE_HOPPER(4),
	DEFINE_HOPPER(5),
	DEFINE_HOPPER(6),
	DEFINE_HOPPER(7),
	DEFINE_HOPPER(8),
	DEFINE_HOPPER(9),
	DEFINE_HOPPER(10),
	DEFINE_HOPPER(11),
	DEFINE_HOPPER(12),
	DEFINE_HOPPER(13),
	DEFINE_HOPPER(14),
	DEFINE_HOPPER(15),
	DEFINE_HOPPER(16),
	DEFINE_HOPPER(17),
	DEFINE_HOPPER(18),
	DEFINE_HOPPER(19),
	DEFINE_HOPPER(20),
	DEFINE_HOPPER(21),
	DEFINE_HOPPER(22),
	DEFINE_HOPPER(23),
	DEFINE_HOPPER(24),
	DEFINE_HOPPER(25),
	DEFINE_HOPPER(26),
	DEFINE_HOPPER(27),
	DEFINE_HOPPER(28),
	DEFINE_HOPPER(29),
	DEFINE_HOPPER(30),
	DEFINE_HOPPER(31),

#undef DEFINE_HOPPER

	k_online_leaderboard_id_column_count, // 207
	k_online_leaderboard_id_column_none = NONE,
};

enum e_network_leaderboard_user_highest_skill_update_state
{
	_network_leaderboard_user_highest_skill_update_none = 0,
	_network_leaderboard_user_highest_skill_update_ready,
	_network_leaderboard_user_highest_skill_update_uploading,
};

enum e_network_leaderboard_statistics_write_state
{
	_network_leaderboard_statistic_write_none = 0,
	_network_leaderboard_statistic_write_in_progress,
	_network_leaderboard_statistic_written,

	k_network_leaderboard_statistic_write_state_count,
};

enum e_leaderboard_statistic
{
	_leaderboard_statistic_hopper_skill_mu = 0,
	_leaderboard_statistic_hopper_skill_sigma,
	_leaderboard_statistic_hopper_skill,

	_leaderboard_statistic_hopper_games_played,
	_leaderboard_statistic_hopper_games_completed,
	_leaderboard_statistic_hopper_games_won,

	//_leaderboard_statistic_hopper_experience_base,
	//_leaderboard_statistic_hopper_experience_penalty,

	_leaderboard_statistic_global_experience_base,
	_leaderboard_statistic_global_experience_penalty,

	_leaderboard_statistic_global_highest_skill_level_attained,

	_leaderboard_statistic_global_matchmade_ranked_games_played,
	_leaderboard_statistic_global_matchmade_ranked_games_completed,
	_leaderboard_statistic_global_matchmade_ranked_games_won,

	_leaderboard_statistic_global_matchmade_unranked_games_played,
	_leaderboard_statistic_global_matchmade_unranked_games_completed,
	_leaderboard_statistic_global_matchmade_unranked_games_won,

	_leaderboard_statistic_global_first_game_played_date,
	_leaderboard_statistic_global_last_game_played_date,

	_leaderboard_statistic_global_custom_games_completed,
	_leaderboard_statistic_global_custom_games_won,

	k_leaderboard_statistic_count,
	k_leaderboard_statistic_none = NONE,
};

enum e_online_property_id
{
	_online_property_id_gamer_language = 0,
	_online_property_id_gamer_zone,
	_online_property_id_gamer_hostname,
	_online_property_id_affiliate_score,
	_online_property_id_skill_draw_probability,
	_online_property_id_skill_beta,
	_online_property_id_skill_tau,
	_online_property_id_relative_score,
	_online_property_id_session_team,
	_online_property_id_skill_hill_climbing_factor,
	_online_property_id_hopper_skill,
	_online_property_id_hopper_games_played,
	_online_property_id_hopper_games_completed,
	_online_property_id_hopper_games_won,
	//_online_property_id_hopper_experience_base,
	//_online_property_id_hopper_experience_penalty,
	_online_property_id_global_experience_base,
	_online_property_id_global_experience_penalty,
	_online_property_id_global_highest_skill_level_attained,
	_online_property_id_global_matchmade_ranked_games_played,
	_online_property_id_global_matchmade_ranked_games_completed,
	_online_property_id_global_matchmade_ranked_games_won,
	_online_property_id_global_matchmade_unranked_games_played,
	_online_property_id_global_matchmade_unranked_games_completed,
	_online_property_id_global_matchmade_unranked_games_won,
	_online_property_id_global_custom_games_played,
	_online_property_id_global_custom_games_completed,
	_online_property_id_global_custom_games_won,
	_online_property_id_global_first_game_played_date,
	_online_property_id_global_last_game_played_date,
	_online_property_id_hopper_identifier,
	_online_property_id_lowest_skill_level,
	_online_property_id_highest_skill_level,
	_online_property_id_average_skill_level,
	_online_property_id_average_experience_rating,
	_online_property_id_has_all_dlc_maps,
	_online_property_id_session_of_quitters,
	_online_property_id_language_override,
	_online_property_id_gamerzone_override,
	_online_property_id_country_code_override,
	_online_property_id_mixed_skill_restricted,
	_online_property_id_joinable_party_size_1,
	_online_property_id_joinable_party_size_2,
	_online_property_id_joinable_party_size_3,
	_online_property_id_joinable_party_size_4,
	_online_property_id_joinable_party_size_5,
	_online_property_id_joinable_party_size_6,
	_online_property_id_joinable_party_size_7,
	_online_property_id_joinable_party_size_8,
	_online_property_id_joinable_party_size_9,
	_online_property_id_nat_type,
	_online_property_id_average_mu,
	_online_property_id_min_average_skill_level,
	_online_property_id_max_average_skill_level,
	_online_property_id_min_average_experience_rating,
	_online_property_id_max_average_experience_rating,
	_online_property_id_min_average_mu,
	_online_property_id_max_average_mu,
	_online_property_id_min_skill_level,
	_online_property_id_max_skill_level,
	_online_property_unused,
	k_online_property_id_count,

	k_online_property_id_none = NONE,
};

enum e_online_data_type
{
	_online_data_context = 0,
	_online_data_integer,
	_online_data_qword,
	_online_data_double,
	_online_data_unicode,
	_online_data_float,
	_online_data_binary,
	_online_data_date_time,

	_online_data_null = 0xFF,
};

enum e_network_session_closed_status
{
	_network_session_closed_none = NONE,
	_network_session_closed_not_closed,
	_network_session_closed_campaign,
	_network_session_closed_theater,
	_network_session_closed_matchmaking,

	k_network_session_closed_count,
	k_network_session_closed_bits = 2,
};

enum e_custom_bitmap_desired_aspect_ratio
{
	_custom_bitmap_desired_aspect_ratio_stretch_to_fit = 0,
	_custom_bitmap_desired_aspect_ratio_16x9,
	_custom_bitmap_desired_aspect_ratio_4x3,

	k_custom_bitmap_desired_aspect_ratio_count,
};

enum e_gui_location
{
	_gui_location_none = NONE,
	_gui_location_main_menu,
	_gui_location_pregame_lobby,
	_gui_location_matchmaking_searching,
	_gui_location_matchmaking_match_found,
	_gui_location_in_game,
	_gui_location_postgame_lobby,

	k_gui_location_count,
};

enum e_event_type
{
	_event_type_none = 0,
	_event_type_tab_up,
	_event_type_tab_left,
	_event_type_tab_down,
	_event_type_tab_right,
	_event_type_alt_stick_up,
	_event_type_alt_stick_left,
	_event_type_alt_stick_down,
	_event_type_alt_stick_right,
	_event_type_alt_tab_up,
	_event_type_alt_tab_left,
	_event_type_alt_tab_down,
	_event_type_alt_tab_right,
	_event_type_button_press,
	//_event_type_mouse_click,

	k_event_type_count
};

enum e_controller_component
{
	_controller_component_button_a = 0,
	_controller_component_button_b,
	_controller_component_button_x,
	_controller_component_button_y,

	_controller_component_button_left_shoulder,
	_controller_component_button_right_shoulder,

	_controller_component_button_left_trigger,
	_controller_component_button_right_trigger,

	_controller_component_button_dpad_up,
	_controller_component_button_dpad_down,
	_controller_component_button_dpad_left,
	_controller_component_button_dpad_right,

	_controller_component_button_start,
	_controller_component_button_back,

	_controller_component_button_left_thumb,
	_controller_component_button_right_thumb,

	_controller_component_stick_left_thumb,
	_controller_component_stick_right_thumb,

	_controller_component_left_stick_left,
	_controller_component_left_stick_right,
	_controller_component_left_stick_up,
	_controller_component_left_stick_down,

	_controller_component_right_stick_left,
	_controller_component_right_stick_right,
	_controller_component_right_stick_up,
	_controller_component_right_stick_down,

	_controller_component_left_stick_x,
	_controller_component_left_stick_y,

	_controller_component_right_stick_x,
	_controller_component_right_stick_y,
};

enum e_screen_transition_type
{
	_screen_transition_type_none = 0,
	_screen_transition_type_normal,
	_screen_transition_type_backwards,
	_screen_transition_type_cycle_previous,
	_screen_transition_type_cycle_next,
	_screen_transition_type_custom0,
	_screen_transition_type_custom1,
};

enum e_transition_out_type
{
	_transition_out_normal = 0,
	_transition_out_back_out,
};

enum e_network_leaderboard_write_status
{
	_network_leaderboard_write_status_none = 0,
	_network_leaderboard_write_in_progress,
	_network_leaderboard_write_completed,

	k_network_leaderboard_write_status_count,
};

enum e_gui_dialog_choice
{
	_gui_dialog_choice_first = 0,
	_gui_dialog_choice_second,
	_gui_dialog_choice_third,
	_gui_dialog_choice_fourth,

	k_gui_dialog_choice_count,
	k_gui_dialog_choice_ok = _gui_dialog_choice_first,
	k_gui_dialog_choice_cancel = _gui_dialog_choice_second,
};

enum
{
	_pvs_activation_none = 0,
	_pvs_activation_object,
	_pvs_activation_cluster,

	k_pvs_activation_count,
};

