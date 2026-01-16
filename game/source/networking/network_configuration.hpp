#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

#pragma region config_download

struct s_network_file_download_configuration
{
	int32 master_catalog_file_download_interval_msec;
	int32 required_file_invalidation_check_interval_msec;
	int32 required_file_download_retry_interval_msec;
};
COMPILE_ASSERT(sizeof(s_network_file_download_configuration) == 0xC);

#pragma endregion

#pragma region bandwidth

struct s_bandwidth_configuration_host_preference_table
{
	int32 connectivity_multiplier;
	int32 host_bonus;
	int32 desired_hostable_peers_multiplier;
	int32 maximum_hostable_peers_multiplier;
	int32 unbounded_hostable_peers_multiplier;
	int32 latency_table_multiplier;
	int32 gamestate_bonus;
	int32 open_nat_bonus;
	int32 hard_drive_bonus;
	int32 local_user_table_multiplier;
	int32 host_rating_multiplier;
	int32 connectivity_rating_multiplier;
	int32 synchronous_hostable_peers_multiplier;
};
COMPILE_ASSERT(sizeof(s_bandwidth_configuration_host_preference_table) == 0x34);

struct s_bandwidth_configuration
{
	real32 bandwidth_outlier_discard_fraction;
	int32 bandwidth_minimum_measurement_count;
	int32 bandwidth_safety_margin_bps;
	int32 bandwidth_minimum_known_good_bps;
	int32 bandwidth_tracking_minimum_duration_msec;
	real32 bandwidth_tracking_maximum_satiation;
	int32 bandwidth_dispute_minimum_count;
	int32 bandwidth_dispute_threshold_bps;
	int32 bandwidth_dispute_increase_bps;
	int32 host_speculative_migration_check_interval_msec;
	int32 host_speculative_migration_check_interval_custom_msec;
	int32 host_speculative_migration_check_interval_matchmaking_msec;
	int32 host_speculative_migration_remigrate_interval_msec;
	int32 host_speculative_migration_required_lobby_peer_connectivity_difference;
	int32 host_speculative_migration_required_match_host_rating_difference;
	int32 host_speculative_migration_required_match_host_bandwidth_difference;
	int32 host_speculative_migration_required_custom_host_rating_difference;
	int32 host_speculative_migration_required_custom_host_bandwidth_difference;
	uns8 host_preference_latency_table[20];
	uns8 host_preference_local_user_table[5];
	s_bandwidth_configuration_host_preference_table host_preferences[4];
	int32 host_preference_connectivity_rankings_spread;
	int32 host_preference_host_rankings_spread;
	int32 upstream_bandwidth_absolute_minimum_bps[18];
	int32 upstream_bandwidth_requirement_bps[18];
	int32 upstream_bandwidth_desired_bps[18];
	int32 minimum_player_restricted_count;
	int32 minimum_host_upstream_bandwidth_bps;
	int32 minimum_host_downstream_bandwidth_bps;
	int32 minimum_host_delegation_advantage_bps;
	int32 good_host_upstream_bandwidth_bps;
	int32 good_host_downstream_bandwidth_bps;
	int32 minimum_voice_repeater_upstream_bandwidth_bps;
	int32 minimum_voice_repeater_downstream_bandwidth_bps;
	int32 voice_channel_bandwidth_bps;
};
COMPILE_ASSERT(sizeof(s_bandwidth_configuration) == 0x238);

#pragma endregion

#pragma region life_cycle

struct s_life_cycle_handler_joining_configuration
{
	int32 network_session_migration_wait_timeout_msec;
	int32 network_session_migration_disband_timeout_msec;
	int32 join_remote_squad_player_reservation_timeout_msec;
	int32 request_migration_start_rety_interval_msec;
	int32 request_migration_abort_rety_interval_msec;
	int32 joining_search_qos_bps;
	int32 join_timeout_msec;
	int32 desperation_wait_time_seconds;
};
COMPILE_ASSERT(sizeof(s_life_cycle_handler_joining_configuration) == 0x20);

struct s_life_cycle_handler_matchmaking_configuration
{
	bool perform_nat_check;
	real32 matchmaking_strict_nat_host_percentage;
	int32 matchmaking_start_failure_wait_time_ms;
	int32 matchmaking_find_match_joinable_session_threshold;
	int32 matchmaking_find_match_join_wait_time_ms;
	int32 matchmaking_find_match_search_results_stale_ms;
	int32 matchmaking_gather_join_wait_time_ms;
	int32 matchmaking_search_give_up_time_seconds;
	int32 join_request_wait_time_ms;
	bool prepare_map_display_map_during_loading;
	int32 prepare_map_veto_timer_seconds;
	int32 prepare_map_minimum_load_time_seconds;
	int32 prepare_map_countdown_timer_seconds;
	int32 prepare_map_vetoed_countdown_timer_seconds;
	int32 prepare_map_veto_failed_countdown_timer_seconds;
	int32 end_match_write_stats_boot_threshold_seconds;
	int32 arbitration_wait_for_establishment_and_connectivity_threshold_seconds;
	int32 arbitration_wait_for_completion_threshold_seconds;
	bool post_match_return_to_pre_game_lobby;
	int32 post_match_stats_refresh_time;
	int32 warning_toast_minimum_time_seconds;
};
COMPILE_ASSERT(sizeof(s_life_cycle_handler_matchmaking_configuration) == 0x54);

struct s_life_cycle_handler_in_game_configuration
{
	int32 simulation_aborted_host_delay_ms;
	int32 simulation_aborted_peer_delay_ms;
};
COMPILE_ASSERT(sizeof(s_life_cycle_handler_in_game_configuration) == 0x8);

struct s_life_cycle_configuration
{
	s_life_cycle_handler_joining_configuration joining;
	s_life_cycle_handler_matchmaking_configuration matchmaking;
	s_life_cycle_handler_in_game_configuration in_game;
};
COMPILE_ASSERT(sizeof(s_life_cycle_configuration) == 0x7C);

#pragma endregion

#pragma region logic

struct s_logic_session_tracker_configuration
{
	int32 unsuitable_session_cache_count;
	int32 qos_bps;
	int32 default_qos_refresh_interval_msec;
	int32 full_qos_timeout_msec;
	int32 maximum_target_sessions_per_default_qos_task;
	int32 maximum_qos_tasks;
};
COMPILE_ASSERT(sizeof(s_logic_session_tracker_configuration) == 0x18);

struct s_logic_matchmaking_desirability
{
	int32 add_to_match_desirability_bonus;
	int32 combined_player_count_per_player_desirability_bonus;
	int32 fill_out_match_desirability_bonus;
	int32 needed_party_size_desirability_bonus;
	int32 gather_time_bonus_threshold_seconds;
	int32 gather_time_desirability_bonus;
	int32 search_time_bonus_threshold_seconds;
	int32 search_time_desirability_bonus;
	int32 average_skill_range_for_desirability_bonus;
	int32 average_skill_desirability_bonus_factor;
	real32 average_mu_range_for_desirability_bonus;
	int32 average_mu_desirability_bonus_factor;
	int32 average_experience_rank_range_for_desirability_bonus;
	int32 average_experience_rank_desirability_bonus_factor;
	int32 good_host_desirability_bonus;
	int32 language_match_desirability_bonus;
	int32 gamer_region_match_desirability_bonus;
	int32 gamer_zone_match_desirability_bonus;
	int32 quitter_match_desirability_bonus;
	int32 dlc_match_desirability_bonus;
	int32 maximum_ping_for_desirability_bonus;
	int32 ping_desirability_bonus_interval;
	int32 desirability_bonus_per_ping_interval;
};
COMPILE_ASSERT(sizeof(s_logic_matchmaking_desirability) == 0x5C);

struct s_logic_matchmaking_seeker_configuration
{
	int32 session_search_wait_time_ms;
	int32 failed_session_search_wait_time_ms;
	int32 final_session_search_query_minimum_time_ms;
	int32 search_stage_strict_skill_round_limit;
	real32 search_stage_strict_skill_initial_mu_range;
	real32 search_stage_strict_skill_final_mu_range;
	int32 search_stage_strict_skill_initial_desired_skill_range;
	int32 search_stage_strict_skill_final_desired_skill_range;
	int32 search_stage_strict_skill_initial_average_skill_range;
	int32 search_stage_strict_skill_final_average_skill_range;
	int32 search_stage_strict_skill_initial_average_experience_rank_range;
	int32 search_stage_strict_skill_connection_threshold_ms;
	int32 search_stage_skill_round_limit;
	int32 search_stage_skill_connection_threshold_ms;
	int32 search_stage_any_round_limit;
	int32 search_stage_any_final_connection_threshold_ms;
	real32 search_stage_any_final_mu_range;
	int32 search_stage_any_final_average_skill_range;
	int32 search_stage_any_final_desired_skill_range;
};
COMPILE_ASSERT(sizeof(s_logic_matchmaking_seeker_configuration) == 0x4C);

struct s_logic_leaderboard_configuration
{
	int32 consecutive_failed_download_threshold;
	int32 milliseconds_between_failed_downloads;
	int32 milliseconds_between_consecutive_failed_downloads;
	int32 refresh_request_wait_time_ms;
};
COMPILE_ASSERT(sizeof(s_logic_leaderboard_configuration) == 0x10);

struct s_session_interface_configuration
{
	int32 peer_properties_interval_msec;
	int32 user_addition_interval_msec;
	int32 user_removal_interval_msec;
	int32 user_properties_interval_msec;
	int32 local_specific_parameter_propagation_msec;
	int32 ready_for_next_match_wait_time_milliseconds;
	int32 stat_replication_wait_threshold_milliseconds;
};
COMPILE_ASSERT(sizeof(s_session_interface_configuration) == 0x1C);

struct s_qos_reply_block_configuration
{
	int32 qos_listener_update_interval;
	int32 disabled_qos_reply_bandwidth_bps;
};
COMPILE_ASSERT(sizeof(s_qos_reply_block_configuration) == 0x8);

struct s_session_qos_reply_block_configuration
{
	int32 out_of_game_qos_reply_bandwidth_bps;
	int32 in_game_qos_reply_bandwidth_bps;
};
COMPILE_ASSERT(sizeof(s_session_qos_reply_block_configuration) == 0x8);

struct s_logic_configuration
{
	s_logic_session_tracker_configuration session_tracker;
	s_logic_matchmaking_desirability desirability;
	s_logic_matchmaking_seeker_configuration seeker;
	s_logic_leaderboard_configuration leaderboard;
	s_session_interface_configuration session_interface;
	s_qos_reply_block_configuration base_qos_reply_block;
	s_session_qos_reply_block_configuration squad_qos_reply_block;
	s_session_qos_reply_block_configuration group_qos_reply_block;
	int32 maximum_players_in_coop;
	int32 maximum_players_in_forge;
};
COMPILE_ASSERT(sizeof(s_logic_configuration) == 0x10C);

#pragma endregion

#pragma region banhammer

struct s_banhammer_configuration
{
	int32 machine_file_refresh_seconds;
	int32 machine_file_refresh_threshold_seconds;
	int32 user_file_refresh_seconds;
	int32 user_file_refresh_threshold_seconds;
	int32 host_chance_reduction_percentage;
	int32 idle_controller_timeout_seconds;
};
COMPILE_ASSERT(sizeof(s_banhammer_configuration) == 0x18);

#pragma endregion

#pragma region simulation

struct s_simulation_zoom_relevance
{
	real32 zoom_0_tolerance;
	real32 zoom_1_tolerance;
	real32 zoom_0_relevance_bonus;
	real32 zoom_1_relevance_bonus;
};
COMPILE_ASSERT(sizeof(s_simulation_zoom_relevance) == 0x10);

struct s_simulation_control_relevance
{
	real32 zero_relevance_distance;
	real32 max_relevance;
	real32 min_relevance;
	int32 min_period;
	int32 max_period;
};
COMPILE_ASSERT(sizeof(s_simulation_control_relevance) == 0x14);

struct s_simulation_position_relevance
{
	real32 distance_to_player_threshold;
	real32 aiming_vector_high_tolerance;
	real32 aiming_vector_medium_tolerance;
	real32 distance_to_player_medium_tolerance;
};
COMPILE_ASSERT(sizeof(s_simulation_position_relevance) == 0x10);

struct s_simulation_netdebug_configuration
{
	int32 bar_maximum_count;
	int32 axis_bounds[4][2];
};
COMPILE_ASSERT(sizeof(s_simulation_netdebug_configuration) == 0x24);

struct s_simulation_view_configuration
{
	int32 game_results_update_interval_msec;
	int32 synchronous_client_block_duration_msec;
};
COMPILE_ASSERT(sizeof(s_simulation_view_configuration) == 0x8);

struct s_simulation_shared_configuration
{
	real32 action_persist_time;
	real32 simulation_event_projectile_supercombine_request_fraction;
};
COMPILE_ASSERT(sizeof(s_simulation_shared_configuration) == 0x8);

struct s_simulation_world_configuration
{
	int32 maximum_catchup_views;
	int32 join_timeout;
	int32 host_join_minimum_wait_time;
	int32 host_join_timeout;
	int32 join_total_wait_timeout;
	real32 pause_game_required_machines_fraction;
	real32 join_activation_blocking_machines_fraction;
	int32 maximum_catchup_attempts;
	int32 catchup_failure_timeout;
	int32 client_join_failure_count;
	int32 client_activation_failure_timeout;
	real32 game_simulation_queue_danger_allocation_size_percentage;
	real32 game_simulation_queue_danger_allocation_count_percentage;
};
COMPILE_ASSERT(sizeof(s_simulation_world_configuration) == 0x34);

struct s_simulation_event_configuration
{
	real32 constant_priority;
	int32 cancel_timer_milliseconds;
	real32 zero_relevance_distance;
	real32 minimum_priority;
	real32 maximum_priority;
};
COMPILE_ASSERT(sizeof(s_simulation_event_configuration) == 0x14);

struct s_simulation_entity_creation_configuration
{
	real32 constant_priority;
	real32 creation_zero_relevance_distance;
	real32 creation_minimum_priority;
	real32 creation_maximum_priority;
};
COMPILE_ASSERT(sizeof(s_simulation_entity_creation_configuration) == 0x10);

struct s_simulation_entity_update_configuration
{
	real32 constant_priority;
	real32 zero_relevance_distance;
	real32 minimum_relevance;
	real32 maximum_relevance;
	int32 minimum_period;
	int32 maximum_period;
	real32 normal_minimum_priority;
	real32 normal_maximum_priority;
	int32 delayed_time_threshold;
	real32 delayed_minimum_priority;
	real32 delayed_maximum_priority;
	real32 maximum_priority;
	real32 player_priority;
	real32 dead_priority;
	real32 in_motion_by_unit;
};
COMPILE_ASSERT(sizeof(s_simulation_entity_update_configuration) == 0x3C);

struct s_simulation_entity_configuration
{
	s_simulation_entity_creation_configuration creation_configuration;
	s_simulation_entity_update_configuration update_configuration;
};
COMPILE_ASSERT(sizeof(s_simulation_entity_configuration) == 0x4C);

struct s_simulation_warping_configuration
{
	real32 simulation_position_update_object_corrective_accept_tolerance;
	real32 simulation_position_update_object_predicted_accept_tolerance;
	real32 simulation_position_update_vehicle_corrective_accept_tolerance;
	real32 simulation_position_update_vehicle_predicted_accept_tolerance;
	real32 position_update_recent_seconds;
	real32 position_update_minimum_distance;
};
COMPILE_ASSERT(sizeof(s_simulation_warping_configuration) == 0x18);

struct s_simulation_weapon_configuration
{
	real32 trigger_recent_spew_time;
	real32 prediction_delay_timer;
	real32 predicted_fire_allow_ratio;
	real32 predicted_fire_always_allow_threshold;
};
COMPILE_ASSERT(sizeof(s_simulation_weapon_configuration) == 0x10);

struct s_simulation_configuration
{
	s_simulation_zoom_relevance zoom_relevance;
	s_simulation_control_relevance control_relevance;
	s_simulation_position_relevance position_relevance;
	s_simulation_netdebug_configuration netdebug;
	s_simulation_view_configuration view;
	s_simulation_shared_configuration shared;

	// odst?
	real32 __unknown68;

	s_simulation_world_configuration world;
	c_static_array<s_simulation_event_configuration, 39> simulation_event_configurations;
	c_static_array<s_simulation_entity_configuration, 22> simulation_entity_configurations;
	s_simulation_warping_configuration warping;
	s_simulation_weapon_configuration weapon;
};
COMPILE_ASSERT(sizeof(s_simulation_configuration) == 0xA5C);

#pragma endregion

#pragma region replication

struct s_event_manager_view_configuration
{
	int32 replication_event_maximum_blocked_time;
};
COMPILE_ASSERT(sizeof(s_event_manager_view_configuration) == 0x4);

struct s_replication_control_view
{
	real32 base_non_player_motion_priority;
	int32 max_priority_threshold;
	real32 max_priority;
	real32 medium_priority_base;
	real32 medium_priority_relevance_scale;
	real32 min_priority_base;
	real32 min_priority_relevance_scale;
};
COMPILE_ASSERT(sizeof(s_replication_control_view) == 0x1C);

struct s_replication_configuration
{
	s_event_manager_view_configuration event_manager_view;
	s_replication_control_view replication_control_view;
	int32 maximum_requests_to_send_in_one_frame;
};
COMPILE_ASSERT(sizeof(s_replication_configuration) == 0x24);

#pragma endregion

#pragma region session

struct s_session_configuration
{
	int32 session_recreate_timeout_msec;
	int32 join_request_interval_msec;
	int32 join_secure_connection_timeout_msec;
	int32 join_initial_update_timeout_msec;
	int32 join_time_to_hold_in_join_queue_msec;
	int32 join_notify_client_join_in_queue_interval_msec;
	int32 join_abort_interval_msec;
	int32 join_abort_timeout_msec;
	int32 host_rejoin_accept_timeout_msec;
	int32 leave_timeout_msec;
	int32 leave_request_interval_msec;
	int32 host_handoff_initiate_timeout_msec;
	int32 host_handoff_selection_delay_msec;
	int32 host_handoff_selection_timeout_msec;
	int32 host_transition_timeout_msec;
	int32 host_reestablish_timeout_msec;
	int32 host_reestablish_maximum_send_to_original_host_delay_msec;
	int32 election_failsafe_timeout_msec;
	int32 election_peer_timeout_msec;
	int32 election_ignore_dissension_msec;
	int32 guaranteed_election_send_interval_msec;
	int32 time_synchronization_interval_msec;
	int32 time_synchronization_retry_msec;
	int32 minimum_election_send_interval_msec;
	int32 allow_third_party_host_elections;
};
COMPILE_ASSERT(sizeof(s_session_configuration) == 0x64);

#pragma endregion

#pragma region observer

struct s_observer_configuration
{
	int32 secure_connect_attempts;
	int32 secure_connect_intervals[8];
	int32 connect_attempts;
	int32 connect_intervals[8];
	int32 non_simulation_reconnect_attempts;
	int32 non_simulation_reconnect_intervals[8];
	int32 reconnect_attempts;
	int32 reconnect_intervals[8];
	int32 death_recovery_time;
	int32 heartbeat_send_timeout;
	int32 connection_active_send_timeout;
	int32 connection_alive_send_timeout;
	int32 connection_alive_receive_timeout;
	int32 connection_drop_minimum_active_time;
	int32 connection_drop_receive_timeout;
	int32 synchronous_connection_drop_minimum_active_time;
	int32 synchronous_connection_drop_receive_timeout;
	int32 minimum_undesired_connection_timeout_msec;
	int32 maximum_undesired_connection_timeout_msec;
	real32 safety_window_threshold;
	real32 safety_packet_interval;
	real32 safety_packet_maximum_interval;
	int32 packet_rate_multiplier_count;
	real32 packet_rate_multipliers[16];
	int32 packet_window_minimum_bytes;
	real32 voice_fraction_authority;
	real32 voice_fraction_client;
	real32 voice_fraction_non_simulation;
	real32 voice_maximum_packet_rate;
	int32 voice_receive_fast_acknowledge_time;
	int32 bandwidth_interval;
	int32 loss_detection_window_size;
	int32 flood_max_aperture_msec;
	real32 flood_packet_fraction;
	int32 desired_payload_bytes_out_of_game;
	int32 desired_payload_bytes_simulation;
	real32 desired_minimum_packet_rate_multiplier;
	int32 minimum_bandwidth_bps;
	int32 bandwidth_adjust_interval;
	real32 bandwidth_flood_channel_fraction;
	int32 bandwidth_flood_check_threshold;
	int32 bandwidth_flood_backoff_kbps;
	int32 bandwidth_flood_backoff_repeat_interval;
	int32 bandwidth_clear_check_threshold;
	int32 bandwidth_check_threshold_maximum;
	int32 bandwidth_initial_backoff_kbps;
	int32 bandwidth_known_good_minimum_kbps;
	int32 bandwidth_increment_known_bad_threshold_kbps;
	int32 bandwidth_increment_kbps;
	bool generate_stream_events;
	int32 observer_stream_expansion_interval_msec;
	int32 stream_minimum_bps;
	int32 stream_maximum_bps;
	int32 stream_initial_startup_msec;
	int32 stream_initial_bps;
	int32 stream_initial_total_bps_unreliable;
	int32 stream_initial_total_bps_reliable;
	int32 stream_initial_minimum_bps_per_stream;
	int32 stream_minimum_great_bandwidth_bps;
	int32 stream_out_of_game_upstream_bandwidth_bps;
	int32 stream_distributed_client_upstream_bandwidth_bps;
	int32 stream_distributed_host_upstream_bandwidth_bps;
	int32 stream_synchronous_client_initial_upstream_bandwidth_bps;
	int32 stream_synchronous_host_initial_upstream_bandwidth_bps;
	int32 stream_synchronous_joining_host_initial_upstream_bandwidth_bps;
	int32 stream_synchronous_client_upstream_bandwidth_bps;
	int32 stream_synchronous_host_upstream_bandwidth_bps;
	int32 stream_synchronous_joining_host_upstream_bandwidth_bps;
	int32 stream_synchronous_non_joining_host_upstream_bandwidth_bps;
	int32 stream_synchronous_client_minimum_upstream_bandwidth_bps;
	int32 stream_synchronous_host_minimum_upstream_bandwidth_bps;
	int32 stream_rtt_noise_msec;
	int32 stream_minimum_rtt_msec;
	int32 stream_rtt_average_gain_bits;
	int32 stream_loss_window_size;
	real32 stream_loss_throttle_fraction;
	int32 stream_throttle_rtt_multiplier;
	real32 stream_throttle_reduce_multiplier;
	int32 stream_throttle_cookie_event_count;
	int32 stream_throttle_cookie_backoff_threshold;
	int32 stream_bandwidth_step_bps;
	real32 stream_bandwidth_step_max_fraction;
	int32 stream_bandwidth_backoff_bps;
	real32 stream_bandwidth_backoff_max_fraction;
	int32 stream_period_maximum_msec;
	int32 stream_growth_period_minimum_msec;
	int32 stream_throughput_recording_period_minimum_msec;
	int32 stream_throughput_satiated_bandwidth_bps;
	real32 stream_throughput_satiated_stream_fraction;
	int32 stream_satiation_timeout_msec;
	int32 stream_congestion_bandwidth_average_gain_bits;
	int32 stream_congestion_offender_bandwidth_increment_bps;
	int32 stream_congestion_offender_timeout_msec;
	int32 stream_growth_maximum_count;
	real32 stream_growth_maximum_fraction;
	real32 stream_growth_desire_maximum;
	int32 stream_growth_desire_delay_multiplier;
	real32 stream_growth_desire_penalty_bandwidth_multiplier;
	int32 stream_growth_out_of_game_maximum_rtt_increase_msec;
	int32 stream_growth_simulation_maximum_rtt_increase_msec;
	int32 stream_non_growth_simulation_maximum_rtt_increase_msec;
	int32 stream_growth_backoff_rtt_event_threshold;
	int32 stream_growth_incremental_rtt_event_threshold;
	int32 stream_growth_incremental_rtt_increase_msec;
	int32 stream_cautious_expansion_limited_periods_count;
	int32 stream_cautious_expansion_instability_timeout_msec;
	int32 bandwidth_monitor_period_count;
	int32 bandwidth_monitor_limitation_bps;
	real32 bandwidth_monitor_constriction_fraction;
	int32 bandwidth_monitor_constriction_threshold_bps;
	int32 client_badness_rating_threshold;
	int32 bad_bandwidth_throughput_threshold;
	bool disable_bad_client_anticheating;
	bool disable_bad_connectivity_anticheating;
	bool disable_bad_bandwidth_anticheating;
	int32 initial_timeout;
	int32 mini_period_minimum_duration;
	int32 mini_period_minimum_rtt_deviation;
	int32 stream_wants_more_bandwidth_fudge_factor;
	int32 stream_wants_more_bandwidth_fudge_factor_small;
	int32 stream_wants_more_allocation_fudge_factor;
	int32 stream_wants_more_allocation_fudge_factor_small;
	int32 stream_maximum_instability_value;
	int32 stream_probe_failure_limit;
	int32 stream_rebalance_interval_msec;
	int32 packet_loss_rate_gain_bits;
	int32 packet_loss_rate_deviation_gain_bits;
	int32 maximum_rtt_increase_msec;
	int32 maximum_lost_packet_rate_increase;
	int32 minimum_packet_loss_deviation;
	int32 minimum_probe_packet_loss_deviation;
	int32 stream_maximum_bandwidth_maximum_delta;
	int32 stream_maximum_bandwidth_skip_max;
	int32 packet_loss_deviation_adjustment;
	int32 packet_rate_deviation_adjustment;
	int32 consecutive_rate_failures_before_badness;
	int32 consecutive_latency_failures_before_badness;
	int32 consecutive_packet_loss_failures_before_badness;
	int32 consecutive_packet_loss_spike_failures_before_badness;
	int32 badness_minimum_host_to_client_bandwidth;
	int32 badness_minimum_allocated_host_to_client_bandwidth;
	int32 badness_minimum_client_to_host_bandwidth;
	int32 badness_minimum_allocated_client_to_host_bandwidth;
	int32 badness_maximum_latency_msec;
	int32 badness_maximum_packet_loss_rate;
	int32 badness_maximum_packet_loss_spike_count;
	int32 stream_congestion_rtt_multiplier;
	int32 remote_client_downstream_usage_multiplier;
	int32 remote_host_downstream_usage_multiplier;
	int32 throttle_congested_stream_bandwidth_multiplier;
	int32 throttle_noncongested_stream_bandwidth_multiplier;
	int32 bandwidth_estimate_multiplier_reliable;
	int32 bandwidth_estimate_multiplier_unreliable;
	int32 bandwidth_estimate_multiplier_qos;
	int32 qos_to_live_interval_msec;
	int32 synchronous_joining_maximum_stream_growth_shift;
	int32 maximum_aggressive_total_growth_bandwidth_bps;
	int32 maximum_cautious_total_growth_bandwidth_bps;
	int32 client_probe_base_delay_msec;
	int32 client_probe_additional_client_delay_msec;
	int32 client_probe_variability;
	int32 stream_stability_deviation_multiplier;
	int32 stream_growth_base_upstream_shift;
	int32 stream_growth_base_upstream_shift_max;
	int32 stream_fail_all_failed_related_probes;
	int32 maximum_consecutive_probe_successes;
	int32 minimum_packet_rate_for_automatic_congestion;
	int32 maximum_rtt_for_automatic_congestion;
	bool do_collateral_last_resort_throttle;
	bool release_throttle_lock_on_first_congest;
	bool round_robin_probes;
	bool fail_all_member_probes_together;
	bool mark_throttled_stream_with_one_failure;
	bool check_single_stream_overprobe;
	bool use_deviation_only_for_related_rtt_timeout;
	bool fast_probe_failed_streams;
	int32 packet_loss_spike_threshold;
	int32 packet_loss_spike_minimum_packet_count;
	int32 packet_loss_spike_skip_averaging_any_spike;
	int32 stable_probe_start_stream_growth_base;
	int32 stable_probe_start_stream_growth_shift_shift;
	int32 stable_probe_maximum_stream_growth_shift;
	int32 stable_probe_query_time_msec;
	int32 stable_probe_query_time_minimum_msec;
	int32 stable_probe_settle_time_msec;
	real32 stable_probe_settle_bandwidth_multiplier;
	int32 stable_probe_throttle_minimum_rollback;
	int32 stable_probe_recover_minimum_time_msec;
	int32 stable_probe_overprobe_minimum_bps;
	int32 stable_probe_overprobe_maximum_bps;
	int32 stable_probe_overprobe_transmit_ratio;
	int32 initial_probe_start_stream_growth_base;
	int32 initial_probe_start_stream_growth_shift_shift;
	int32 initial_probe_maximum_stream_growth_shift;
	int32 initial_probe_query_time_msec;
	int32 initial_probe_query_time_minimum_msec;
	int32 initial_probe_settle_time_msec;
	real32 initial_probe_settle_bandwidth_multiplier;
	int32 initial_probe_throttle_minimum_rollback;
	int32 initial_probe_recover_minimum_time_msec;
	int32 initial_probe_overprobe_minimum_bps;
	int32 initial_probe_overprobe_maximum_bps;
	int32 initial_probe_overprobe_transmit_ratio;
	int32 fast_probe_start_stream_growth_base;
	int32 fast_probe_start_stream_growth_shift_shift;
	int32 fast_probe_maximum_stream_growth_shift;
	int32 fast_probe_query_time_msec;
	int32 fast_probe_query_time_minimum_msec;
	int32 fast_probe_settle_time_msec;
	real32 fast_probe_settle_bandwidth_multiplier;
	int32 fast_probe_throttle_minimum_rollback;
	int32 fast_probe_recover_minimum_time_msec;
	int32 fast_probe_overprobe_minimum_bps;
	int32 fast_probe_overprobe_maximum_bps;
	int32 fast_probe_overprobe_transmit_ratio;
	int32 slow_probe_start_stream_growth_base;
	int32 slow_probe_start_stream_growth_shift_shift;
	int32 slow_probe_maximum_stream_growth_shift;
	int32 slow_probe_query_time_msec;
	int32 slow_probe_query_time_minimum_msec;
	int32 slow_probe_settle_time_msec;
	real32 slow_probe_settle_bandwidth_multiplier;
	int32 slow_probe_throttle_minimum_rollback;
	int32 slow_probe_recover_minimum_time_msec;
	int32 slow_probe_overprobe_minimum_bps;
	int32 slow_probe_overprobe_maximum_bps;
	int32 slow_probe_overprobe_transmit_ratio;
};
COMPILE_ASSERT(sizeof(s_observer_configuration) == 0x418);

struct s_channel_configuration
{
	int32 connect_request_interval_msec;
	int32 connect_request_count;
	int32 connect_request_timeout_msec;
	int32 establish_timeout_msec;
	int32 packet_statistics_interval;
};
COMPILE_ASSERT(sizeof(s_channel_configuration) == 0x14);

struct s_connection_configuration
{
	int32 sequence_advance_rate;
	int32 discard_ancient_reply_sequence_threshold;
	int32 packet_skipped_outoforder_threshold;
	int32 retain_lost_packets_msec;
	int32 latency_average_gain_bits;
	int32 latency_deviation_gain_bits;
	int32 timeout_deviations;
	int32 timeout_minimum_tolerance_msec;
	int32 timeout_minimum_msec;
	int32 inactive_timeout_deviations;
	int32 inactive_timeout_minimum_tolerance_msec;
	int32 inactive_timeout_minimum_msec;
	int32 initial_latency_minimum_msec;
	int32 initial_latency_average_msec;
	int32 initial_latency_deviation_msec;
	int32 initial_timeout_msec;
	int32 backoff_increment_msec;
	int32 backoff_max_msec;
	int32 backoff_decrement_msec;
	real32 bandwidth_warning_latency_multiplier;
	int32 bandwidth_warning_latency_minimum_increase;
};
COMPILE_ASSERT(sizeof(s_connection_configuration) == 0x54);

#pragma endregion

#pragma region delivery

struct s_delivery_configuration
{
	s_channel_configuration channel_configuration;
	s_connection_configuration connection;
};
COMPILE_ASSERT(sizeof(s_delivery_configuration) == 0x68);

#pragma endregion

#pragma region transport

struct s_transport_configuration
{
	int32 address_resolution_timeout_msec;
	int32 qos_probe_count[2];
	int32 qos_probe_bps[2];
	bool qos_upstream_cap_enabled;
	int32 qos_upstream_cap_upstream_modifier_percentage;
	int32 qos_upstream_cap_correction_modifier;
};
COMPILE_ASSERT(sizeof(s_transport_configuration) == 0x20);

#pragma endregion

#pragma region voice

struct s_voice_configuration
{
	real32 push_to_talk_inactivity_threshold_seconds;
	real32 maximum_push_to_talk_time_seconds;
	bool clients_can_be_preferred_consumers_of_voice_repeater;
	int32 open_channel_player_count;
};
COMPILE_ASSERT(sizeof(s_voice_configuration) == 0x10);

#pragma endregion

#pragma region data_mine

struct s_data_mine_configuration
{
	int32 ui_upload_record_threshold;
	int32 ui_upload_time_threshold;
	bool record_uploads_prevent_game_from_starting;
};
COMPILE_ASSERT(sizeof(s_data_mine_configuration) == 0xC);

#pragma endregion

#pragma region griefer_config

struct s_griefer_configuration
{
	uns32 betrayal_decrement_time;
	uns32 eject_decrement_time;
	real32 betrayal_cutoff;
	int32 ejection_cutoff;
	real32 friendly_assist_amount; // UNUSED
};
COMPILE_ASSERT(sizeof(s_griefer_configuration) == 0x14);

#pragma endregion

#pragma region memory

struct s_network_memory_configuration
{
	int32 network_heap_large_size;
	int32 network_heap_small_size;
};
COMPILE_ASSERT(sizeof(s_network_memory_configuration) == 0x8);

#pragma endregion

#pragma region user_interface

struct s_user_interface
{
	int32 allow_matchmaking_abort_interval_msec;
	int32 networked_playback_maximum_player_count;
	int32 basic_training_completion_minimum_games_completed;
	int32 basic_training_completion_minimum_experience;
};
COMPILE_ASSERT(sizeof(s_user_interface) == 0x10);

#pragma endregion

#pragma region skill_level_configuration

struct s_skill_level_configuration
{
	int32 bonus_skill_level_wins_required[50];
};
COMPILE_ASSERT(sizeof(s_skill_level_configuration) == 0xC8);

#pragma endregion

#pragma region experience_configuration

struct s_experience_configuration
{
	int32 experience_for_rank[56];
	int32 skill_for_rank[14];
};
COMPILE_ASSERT(sizeof(s_experience_configuration) == 0x118);

#pragma endregion

#pragma region alpha_configuration

struct s_alpha_configuration
{
	bool disable_game;
	bool disable_custom_games;
	int32 ui_level;
	int32 maximum_multiplayer_split_screen;
};
COMPILE_ASSERT(sizeof(s_alpha_configuration) == 0xC);

#pragma endregion

#pragma region crash_handling_configuration

struct s_crash_handling_configuration
{
	bool display_crash_handling_ui;
	int32 minidump_generation;
};
COMPILE_ASSERT(sizeof(s_crash_handling_configuration) == 0x8);

#pragma endregion

#pragma region lsp_configuration

struct s_lsp_configuration
{
	int32 port_range_start;
	int32 port_range_count;
	int32 server_retry_count;
	int32 search_results_fresh_milliseconds;
	int32 recent_activity_milliseconds;
};
COMPILE_ASSERT(sizeof(s_lsp_configuration) == 0x14);

#pragma endregion

#pragma region map_configuration

struct s_map_information
{
	e_map_id map_id;
	int32 map_status;
};
COMPILE_ASSERT(sizeof(s_map_information) == 0x8);

struct s_map_configuration
{
	c_static_array<s_map_information, 32> map_list;
};

#pragma endregion

#pragma region chicken_switches

struct s_chicken_switches
{
	bool allow_no_hdd_network_coop;
	bool allow_matched_hdd_network_coop;
	bool disallow_cross_language_coop;
	bool disable_prefer_good_connection_changes;
};
COMPILE_ASSERT(sizeof(s_chicken_switches) == 0x4);

#pragma endregion

#pragma region determinism_configuration

struct s_determinism_configuration
{
	int32 determinism_version;
	int32 determinism_compatible_version;
};
COMPILE_ASSERT(sizeof(s_determinism_configuration) == 0x8);

#pragma endregion

struct s_network_configuration
{
	s_network_file_download_configuration config_download;
	s_bandwidth_configuration bandwidth;
	s_life_cycle_configuration life_cycle;
	s_logic_configuration logic;
	s_banhammer_configuration banhammer;
	s_simulation_configuration simulation;
	s_replication_configuration replication;
	s_session_configuration session;
	s_observer_configuration observer;
	s_delivery_configuration delivery;
	s_transport_configuration transport;
	s_voice_configuration voice;
	s_data_mine_configuration data_mine;
	s_griefer_configuration griefer_config;
	s_network_memory_configuration memory;
	s_user_interface user_interface;
	s_skill_level_configuration skill_level_configuration;
	s_experience_configuration experience_configuration;
	s_alpha_configuration alpha_configuration;
	s_crash_handling_configuration crash_handling_configuration;
	s_lsp_configuration lsp_configuration;
	s_map_configuration map_configuration;
	s_chicken_switches chicken_switches;
	s_determinism_configuration determinism_configuration;
};
COMPILE_ASSERT(sizeof(s_network_configuration) == 0x16C4);

class c_network_configuration_globals
{
public:
	c_http_blf_simple_downloader<s_network_configuration, 8033> downloader;
	bool last_download_failed;
	bool has_downloaded_network_configuration;
	int32 attempt_index;
};
COMPILE_ASSERT(sizeof(c_network_configuration_globals) == 0x2610);

extern bool& g_network_configuration_initialized;
extern s_network_configuration& g_network_configuration;
extern c_network_configuration_globals& g_network_configuration_globals;

extern int32 __cdecl network_configuration_calculate_next_retry_interval(int32 current_interval, int32 original_interval);
extern int32 __cdecl network_configuration_client_badness_rating_threshold();
extern bool __cdecl network_configuration_custom_games_enabled();
extern bool __cdecl network_configuration_disable_bad_bandwidth_anticheating();
extern bool __cdecl network_configuration_disable_bad_client_anticheating();
extern bool __cdecl network_configuration_disable_bad_connectivity_anticheating();
extern void __cdecl network_configuration_dispose();
extern int32 __cdecl network_configuration_get_map_index_from_map_id(e_map_id map_id);
extern int32 __cdecl network_configuration_get_minidump_generation_type();
extern int32 __cdecl network_configuration_get_state();
extern void __cdecl network_configuration_initialize(bool building_for_network_file_output);
extern void __cdecl network_configuration_initialize_observer_constants();
extern void __cdecl network_configuration_initialize_private();
extern void __cdecl network_configuration_initialize_simulation_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_creation_constants();
extern void __cdecl network_configuration_initialize_simulation_entity_update_constants();
extern void __cdecl network_configuration_initialize_simulation_event_constants();
extern bool __cdecl network_configuration_is_crash_ui_enabled();
extern int32 __cdecl network_configuration_maximum_multiplayer_split_screen();
extern void __cdecl network_configuration_update();

extern void network_build_network_configuration();

