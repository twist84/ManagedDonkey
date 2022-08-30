#pragma once

#include "game/game_options.hpp"
#include "memory/data.hpp"
#include "render/render_visibility_collection.hpp"
#include "shell/shell.hpp"

extern s_game_cluster_bit_vectors* game_get_cluster_pvs();
extern s_game_cluster_bit_vectors* game_get_cluster_pvs_local();
extern s_game_cluster_bit_vectors* game_get_cluster_activation();
//extern bool game_test_cluster_activation(s_cluster_reference* cluster_reference);
extern void game_pvs_enable_scripted_camera_pvs();
extern void game_pvs_clear_scripted_camera_pvs();
extern void game_pvs_scripted_set_object(datum_index object_index);
//extern void game_pvs_scripted_set_camera_point(short camera_point_index);
extern void game_pvs_scripted_clear();
//extern void game_update_pvs();
//extern void game_won();
//extern bool game_is_won();
extern void game_lost(bool game_revert);
extern bool game_is_lost();
extern bool game_is_lost_immediate();
extern void game_finish();
extern void game_finish_immediate();
extern bool game_is_finished();
extern bool game_is_finished_immediate();
extern bool game_in_startup_phase();
extern bool game_in_progress();
//extern long game_create_lock_resources(e_game_create_mode);
//extern void game_create_unlock_resources(e_game_create_mode, long&);
//extern void game_start(enum e_game_create_mode);
extern bool game_options_valid();
extern bool game_options_verify(game_options const* options, char* error_string, long error_string_length);
extern void assert_game_options_verify(game_options const* options);
extern game_options* game_options_get();
extern void game_options_print_game_id();
//extern void game_options_setup_for_saved_film(e_game_playback_type playback_type);
extern void game_options_clear_game_playback();
//extern void game_options_game_engine_fixup();
extern e_campaign_difficulty_level game_difficulty_level_get();
extern e_campaign_difficulty_level game_difficulty_level_get_ignore_easy();
extern e_game_mode game_mode_get();
extern bool game_is_ui_shell();
extern bool game_is_multiplayer();
extern bool game_is_campaign();
extern bool game_is_survival();
extern bool game_is_campaign_or_survival();
extern void game_set_active_skulls(dword* active_primary_skulls, dword* active_secondary_skulls);
extern void game_set_difficulty(e_campaign_difficulty_level campaign_difficulty);
//extern bool game_is_cooperative();
//extern long game_coop_player_count();
extern bool game_is_playtest();
extern bool game_had_an_update_tick_this_frame();
extern e_game_simulation_type game_simulation_get();
extern bool game_is_playback();
extern e_game_playback_type game_playback_get();
//extern void game_playback_set(e_game_playback_type playback_type);
extern void game_simulation_set(e_game_simulation_type game_simulation);
extern bool game_is_synchronous_networking();
extern bool game_is_networked();
//extern bool game_is_in_progress_on_live();
extern bool game_is_server();
extern bool game_is_authoritative();
extern bool game_is_predicted();
extern bool game_is_distributed();
extern long game_tick_rate_get();
extern bool game_skull_is_active_primary(e_campaign_skulls_primary primary_skull);
extern bool game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull);
extern void game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable);
extern void game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable);
extern bool game_coop_allow_respawn();
//extern bool game_survival_allow_respawn(long);
extern e_language game_get_master_language();
//extern bool game_is_language_neutral();
extern void game_options_setup_default_players(long player_count, game_options* options);