#pragma once

#include "cseries/language.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_globals.hpp"
#include "game/game_options.hpp"
#include "game/game_time.hpp"
#include "game/player_control.hpp"
#include "game/player_mapping.hpp"
#include "game/players.hpp"
#include "game/survival_mode.hpp"
#include "render/render_visibility_collection.hpp"
#include "saved_games/game_state.hpp"
#include "structures/structures.hpp"

enum
{
	_cluster_activation_reason_none = 0,
	_cluster_activation_reason_player_pvs,
	_cluster_activation_reason_ai,
	_cluster_activation_reason_scripted_object,
	_cluster_activation_reason_scripted_camera,
	_cluster_activation_reason_player_pvs_inactive,

	k_cluster_activation_reason_count,
};

struct s_game_non_bsp_zone_set;
class c_scenario_resource_registry;

struct s_game_system
{
	const char* name;

	void(__cdecl* initialize_proc)();
	void(__cdecl* dispose_proc)();
	void(__cdecl* initialize_for_new_map_proc)();
	void(__cdecl* dispose_from_old_map_proc)();
	void(__cdecl* prepare_for_new_zone_set_proc)(uns32 old_structure_bsp_mask, uns32 new_structure_bsp_mask);
	void(__cdecl* initialize_for_new_structure_bsp_proc)(uns32 new_structure_bsp_mask);
	void(__cdecl* dispose_from_old_structure_bsp_proc)(uns32 old_structure_bsp_mask);
	void(__cdecl* change_pvs_proc)(const s_game_cluster_bit_vectors*, const s_game_cluster_bit_vectors*, bool);
	void(__cdecl* activation_proc)(const s_game_cluster_bit_vectors*, const s_game_cluster_bit_vectors*);
	void(__cdecl* prepare_for_non_bsp_zone_set_switch_proc)(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
	void(__cdecl* initialize_for_new_non_bsp_zone_set_proc)(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
	void(__cdecl* dispose_from_old_non_bsp_zone_set_proc)(const s_game_non_bsp_zone_set* old_non_bsp_zone_set);
};
static_assert(sizeof(s_game_system) == sizeof(void*) * 13);

enum e_game_create_mode
{
	_game_create_from_main = 0,
	_game_create_inside_game_tick,

	k_number_of_game_create_mode_count
};

struct s_game_options_launch_settings
{
	// 106708
	int32 build_number;

	char scenario_path[256];

	e_campaign_difficulty_level campaign_difficulty;

	e_game_mode game_mode;
	e_game_engine_type game_engine_index;

	int32 player_count;

	int16 insertion_point;
	int16 zone_set_index;
	uns32 launch_file_flags; // bit 1, delete after read

	//char insertion_point_name[128]; // name speculation, never actually saw this used
	//char zone_set_name[128];
};
//static_assert(sizeof(s_game_options_launch_settings) == 0x210);
static_assert(sizeof(s_game_options_launch_settings) == 0x11C);

struct s_date_and_time
{
	int32 year;
	int32 month;
	int32 day;
	int32 hour;
	int32 minute;
	int32 second;
};
static_assert(sizeof(s_date_and_time) == 0x18);

extern s_game_system const g_game_systems[];
extern int32 const g_game_system_count;

extern bool g_debug_survival_mode;
extern bool debug_pvs_editor_mode;
extern const char* const k_game_simulation_names[k_game_simulation_count];
extern const char* const k_game_playback_names[k_game_playback_count];
extern t_cluster_activation_reason g_cluster_activation_reason;

extern bool __cdecl game_is_multithreaded();
//extern void __cdecl __tls_set_g_game_globals_allocator(void*);
extern void __cdecl assert_game_options_verify(const game_options* options);
//.text:00530580 ; game_globals_get_primary_skulls;
//.text:005305D0 ; game_globals_get_secondary_skulls;
//extern void __cdecl game_react_to_level_completion();
extern void __cdecl game_clear_structure_pvs(s_game_cluster_bit_vectors* pvs, uns32 structure_mask);
extern void __cdecl game_clusters_and(const s_game_cluster_bit_vectors* clusters0, const s_game_cluster_bit_vectors* clusters1, s_game_cluster_bit_vectors* clusters_destination);
extern void __cdecl game_clusters_fill(s_game_cluster_bit_vectors* clusters, bool value);
extern void __cdecl game_clusters_or(const s_game_cluster_bit_vectors* clusters0, const s_game_cluster_bit_vectors* clusters1, s_game_cluster_bit_vectors* clusters_destination);
extern void __cdecl game_compute_pvs(s_game_cluster_bit_vectors* pvs, bool local_only, t_cluster_activation_reason* activation_reason);
extern bool __cdecl game_clusters_test(const s_game_cluster_bit_vectors* game_cluster_bit_vectors, struct s_cluster_reference structure_reference);
//extern bool __cdecl game_coop_allow_respawn();
//extern int32 __cdecl game_coop_player_count();
extern void __cdecl game_create_ai(e_game_create_mode mode);
extern int32 __cdecl game_create_lock_resources(e_game_create_mode mode);
extern void __cdecl game_create_missing_objects(e_game_create_mode mode);
extern void __cdecl game_create_objects(e_game_create_mode mode);
extern void __cdecl game_create_players();
extern void __cdecl game_create_unlock_resources(e_game_create_mode mode, int32&);
//extern bool __cdecl game_determinism_version_compatible(int32 determinism_version);
extern e_campaign_difficulty_level __cdecl game_difficulty_level_get();
extern e_campaign_difficulty_level __cdecl game_difficulty_level_get_ignore_easy();
extern void __cdecl game_dispose();
extern void __cdecl game_dispose_from_old_map();
extern void __cdecl game_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set);
extern void __cdecl game_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl game_finish();
//.text:00530FF0 ; 
extern void __cdecl game_finished_update();
extern void __cdecl game_frame(real32 game_seconds_elapsed);
extern uns32 __cdecl game_get_active_cinematic_zone_mask();
extern uns32 __cdecl game_get_active_designer_zone_mask();
extern uns32 __cdecl game_get_active_structure_bsp_mask();
extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_activation();
extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs();
extern s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs_local();
//extern void __cdecl game_get_determinism_versions(int32* determinism_version, int32* determinism_compatible_version);
//.text:00531200 ; 
extern e_language __cdecl game_get_master_language();
//extern int32 __cdecl game_get_recently_spawned_grace_period_seconds();
extern void __cdecl game_globals_dispose_from_old_map();
extern void __cdecl game_globals_initialize_for_new_map(const game_options* options);
extern bool __cdecl game_had_an_update_tick_this_frame();
extern bool __cdecl game_has_nonlocal_players();
extern bool __cdecl game_in_progress();
extern bool __cdecl game_in_startup_phase();
extern void __cdecl game_initialize();
extern void __cdecl game_initialize_for_new_map(const game_options* options);
extern void __cdecl game_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
extern void __cdecl game_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
//extern int16 __cdecl game_insertion_point_get();
//extern void __cdecl game_insertion_point_lock(int16);
//extern void __cdecl game_insertion_point_unlock(int16);
extern bool __cdecl game_is_authoritative();
extern bool __cdecl game_is_authoritative_playback();
extern bool __cdecl game_is_available();
extern bool __cdecl game_is_campaign_or_survival();
extern bool __cdecl game_is_campaign();
extern bool __cdecl game_is_cooperative();
//extern bool __cdecl game_is_cross_language();
extern bool __cdecl game_is_distributed();
extern bool __cdecl game_is_finished();
extern bool __cdecl game_is_finished_immediate();
//extern bool __cdecl game_is_in_progress_on_live();
extern bool __cdecl game_is_lost();
extern bool __cdecl game_is_lost_immediate();
extern bool __cdecl game_is_multiplayer();
extern bool __cdecl game_is_networked();
extern bool __cdecl game_is_or_was_cooperative();
extern bool __cdecl game_is_playback();
extern bool __cdecl game_is_playtest();
extern bool __cdecl game_is_predicted();
extern bool __cdecl game_is_server();
extern bool __cdecl game_is_splitscreen_deterministic();
extern bool __cdecl game_is_survival();
extern bool __cdecl game_is_synchronous_networking();
extern bool __cdecl game_is_ui_shell();
extern void __cdecl game_launch_initial_script();
//.text:00531EE0 ; game_level_advance;
//.text:00531FF0 ; prepare_game_level;
//.text:00532050 ; game_level_prepare;
extern void __cdecl game_loss_update();
//extern void __cdecl game_lost_for_scripting(bool);
extern void __cdecl game_lost(bool lost);
extern e_game_mode __cdecl game_mode_get();
extern void __cdecl game_options_clear_game_playback();
extern void __cdecl game_options_game_engine_fixup();
extern game_options* __cdecl game_options_get();
extern void __cdecl game_options_print_game_id();
//extern  bool __cdecl game_options_match(const game_options*, const game_options*);
extern void __cdecl game_options_new(game_options* options);
extern void __cdecl game_options_setup_default_players(int32 player_count, game_options* options);
//extern void __cdecl game_options_setup_for_playback(e_game_playback_type);
//void __cdecl game_options_setup_for_saved_film(e_game_playback_type);
extern bool __cdecl game_options_valid();
extern void __cdecl game_options_validate(game_options* options);
//extern void __cdecl game_options_validate_for_saved_game(int32);
extern bool __cdecl game_options_verify(const game_options* options, char* error_string, int32 error_string_length);
extern e_game_playback_type __cdecl game_playback_get();
//extern void __cdecl game_playback_set(e_game_playback_type playback_type);
extern void __cdecl game_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* currently_active_tags_registry);
extern void __cdecl game_prepare_to_switch_structure_bsp(uns32 old_structure_bsp_mask, uns32 new_structure_bsp_mask);
//extern void __cdecl skull_primary_enable;
//extern game_get_game_progression;
extern void __cdecl game_pvs_clear_scripted_camera_pvs();
extern void __cdecl game_pvs_debug_render();
extern void __cdecl game_pvs_enable_scripted_camera_pvs();
extern void __cdecl game_pvs_scripted_clear();
extern s_cluster_reference __cdecl game_pvs_scripted_get_cluster_reference();
//extern void __cdecl game_pvs_scripted_set_camera_point(int16 camera_point_index);
extern void __cdecl game_pvs_scripted_set_object(int32 object_index);
//extern void __cdecl update_controller_game_progression();
//.text: ; skull_secondary_enable;
extern void __cdecl game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable);
//.text:00532F20 ; game_set_active_primary_skulls;
//.text:00532F50 ; game_set_active_secondary_skulls;
extern bool __cdecl game_skull_is_active_primary(e_campaign_skulls_primary primary_skull);
extern bool __cdecl game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull);
extern void __cdecl game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable);
extern void __cdecl game_set_active_skulls(uns32* active_primary_skulls, uns32* active_secondary_skulls);
extern void __cdecl game_set_difficulty(int16 campaign_difficulty);
extern e_game_simulation_type __cdecl game_simulation_get();
extern void __cdecl game_simulation_set(e_game_simulation_type game_simulation);
extern void __cdecl game_start(e_game_create_mode mode);
extern bool __cdecl game_survival_allow_respawn();
//extern bool __cdecl game_test_cluster_activation(const s_cluster_reference*);
extern void __cdecl game_tick();
extern void __cdecl game_tick_pulse_random_seed_deterministic(const struct simulation_update* update);
extern int32 __cdecl game_tick_rate_get();
extern void __cdecl game_time_get_date_and_time(s_date_and_time* date_and_time);
//extern e_game_time_holiday __cdecl game_time_get_holiday();
//.text:005335E0 game_progression_get_previous_map_block_index;
//.text:00533640 ; 
//.text:00533690 ; game_prepare_for_progression;
extern void __cdecl game_update(int32 tick_count, real32* game_seconds_elapsed);
//.text:005339C0 ; 
extern void __cdecl game_update_pvs();
extern void __cdecl game_won();
//.text:00533EF0 ; 
//extern const char* __cdecl get_game_difficulty_name(int16);
extern void __cdecl game_finish_immediate();
extern bool __cdecl game_options_read_launch_settings_from_string(const char* buffer, s_game_options_launch_settings* out_launch_settings);
extern bool __cdecl game_launch_get_settings(s_game_options_launch_settings* out_launch_settings);
extern bool __cdecl game_launch_get_initial_script_name(const char* script_name);
extern bool __cdecl game_options_get_launch_settings(game_options* options, bool change_in_progress);
extern void game_bsp_debug_status(const char* status, uns32 structure_bsp_mask);
extern void game_designer_zone_set_debug_status(const char* status, uns32 designer_zone_mask);
extern void game_cinematic_zone_set_debug_status(const char* status, uns32 cinematic_zone_mask);

