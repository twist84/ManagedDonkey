#pragma once

#include "cseries/language.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_globals.hpp"
#include "game/game_options.hpp"
#include "game/game_state.hpp"
#include "game/game_time.hpp"
#include "game/player_control.hpp"
#include "game/player_mapping.hpp"
#include "game/players.hpp"
#include "game/survival_mode.hpp"
#include "memory/data.hpp"
#include "render/render_visibility_collection.hpp"
#include "shell/shell.hpp"

extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs();
extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs_local();
extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_activation();
//extern bool __cdecl game_test_cluster_activation(s_cluster_reference* cluster_reference);
extern void __cdecl game_pvs_enable_scripted_camera_pvs();
extern void __cdecl game_pvs_clear_scripted_camera_pvs();
extern void __cdecl game_pvs_scripted_set_object(datum_index object_index);
//extern void __cdecl game_pvs_scripted_set_camera_point(short camera_point_index);
extern void __cdecl game_pvs_scripted_clear();
extern void __cdecl game_tick();
extern void __cdecl game_update(long tick_count, real* game_seconds_elapsed);
//extern void __cdecl game_update_pvs();
extern void __cdecl game_won();
//extern bool __cdecl game_is_won();
extern void __cdecl game_lost(bool game_revert);
extern bool __cdecl game_is_lost();
extern bool __cdecl game_is_lost_immediate();
extern void __cdecl game_finish();
extern void __cdecl game_finish_immediate();
extern bool __cdecl game_is_finished();
extern bool __cdecl game_is_finished_immediate();
extern bool __cdecl game_in_startup_phase();
extern void __cdecl game_initialize();
extern bool __cdecl game_in_progress();
//extern long __cdecl game_create_lock_resources(e_game_create_mode);
//extern void __cdecl game_create_unlock_resources(e_game_create_mode, long&);
//extern void __cdecl game_start(enum e_game_create_mode);
extern bool __cdecl game_options_valid();
extern bool __cdecl game_options_verify(game_options const* options, char* error_string, long error_string_length);
extern void __cdecl assert_game_options_verify(game_options const* options);
extern game_options* __cdecl game_options_get();
extern void __cdecl game_options_print_game_id();
//extern void __cdecl game_options_setup_for_saved_film(e_game_playback_type playback_type);
extern void __cdecl game_options_clear_game_playback();
//extern void __cdecl game_options_game_engine_fixup();
extern e_campaign_difficulty_level __cdecl game_difficulty_level_get();
extern e_campaign_difficulty_level __cdecl game_difficulty_level_get_ignore_easy();
extern void __cdecl game_dispose();
extern e_game_mode __cdecl game_mode_get();
extern bool __cdecl game_is_ui_shell();
extern bool __cdecl game_is_multiplayer();
extern bool __cdecl game_is_campaign();
extern bool __cdecl game_is_survival();
extern bool __cdecl game_is_campaign_or_survival();
extern void __cdecl game_set_active_skulls(dword* active_primary_skulls, dword* active_secondary_skulls);
extern void __cdecl game_set_difficulty(e_campaign_difficulty_level campaign_difficulty);
//extern bool __cdecl game_is_cooperative();
//extern long __cdecl game_coop_player_count();
extern bool __cdecl game_is_playtest();
extern bool __cdecl game_had_an_update_tick_this_frame();
extern e_game_simulation_type __cdecl game_simulation_get();
extern bool __cdecl game_is_playback();
extern e_game_playback_type __cdecl game_playback_get();
//extern void __cdecl game_playback_set(e_game_playback_type playback_type);
extern void __cdecl game_simulation_set(e_game_simulation_type game_simulation);
extern bool __cdecl game_is_multithreaded();
extern bool __cdecl game_is_synchronous_networking();
extern bool __cdecl game_is_networked();
//extern bool __cdecl game_is_in_progress_on_live();
extern bool __cdecl game_is_server();
extern bool __cdecl game_is_authoritative();
extern bool __cdecl game_is_authoritative_playback();
extern bool __cdecl game_is_predicted();
extern bool __cdecl game_is_distributed();
extern long __cdecl game_tick_rate_get();
extern bool __cdecl game_skull_is_active_primary(e_campaign_skulls_primary primary_skull);
extern bool __cdecl game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull);
extern void __cdecl game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable);
extern void __cdecl game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable);
extern bool __cdecl game_coop_allow_respawn();
//extern bool __cdecl game_survival_allow_respawn(long);
extern e_language __cdecl game_get_master_language();
//extern bool __cdecl game_is_language_neutral();
extern void __cdecl game_options_setup_default_players(long player_count, game_options* options);
extern void __cdecl game_options_new(game_options* options);
extern dword __cdecl game_get_active_structure_bsp_mask();
extern bool __cdecl game_launch_has_initial_script(char const* script_name);
extern bool __cdecl game_options_get_launch_settings(game_options*, bool change_in_progress);
extern void __cdecl game_pvs_debug_render();

