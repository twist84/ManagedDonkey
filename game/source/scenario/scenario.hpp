#pragma once

#include "scenario/scenario_definitions.hpp"

struct s_scenario_game_state
{
	uns32 scenario_designer_zones_active;
	uns32 zone_set_index;
	uns32 active_designer_zone_mask;
	uns32 active_structure_bsp_mask;
	uns32 touched_structure_bsp_mask;
	uns32 active_cinematic_zone_mask;
	uns32 touched_cinematic_zone_mask;
	uns32 loaded_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_game_state) == 0x20);

struct s_scenario_zone_state
{
	bool using_designer_zones;
	uns32 active_bsp_mask;
	uns32 touched_bsp_mask;
	uns32 active_designer_zone_mask;
	uns32 active_cinematic_zone_mask;
	uns32 pending_bsp_zone_mask;
	uns32 pending_designer_zone_mask;
	uns32 pending_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_state) == 0x20);

struct s_game_non_bsp_zone_set
{
	uns32 designer_zone_mask;
	uns32 cinematic_zone_mask;
};
static_assert(sizeof(s_game_non_bsp_zone_set) == 0x8);

struct s_scenario_zone_change
{
	bool any_cinematic_zone_changes() const;
	bool any_designer_zone_changes() const;
	bool any_zone_changes() const;
	uns32 get_activating_cinematic_zone_mask() const;
	uns32 get_activating_designer_zone_mask() const;
	uns32 get_deactivating_cinematic_zone_mask() const;
	uns32 get_deactivating_designer_zone_mask() const;
	uns32 pre_switch_cinematic_zone_mask() const;
	uns32 pre_switch_designer_zone_mask() const;

	uns32 original_designer_zone_mask;
	uns32 original_cinematic_zone_mask;
	uns32 new_designer_zone_mask;
	uns32 new_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_change) == 0x10);

struct s_game_globals;
struct structure_bsp;
struct s_structure_design;
struct s_cluster_reference;

extern int32& global_scenario_index;
extern int32& global_scenario_game_globals_index;

extern struct scenario*& global_scenario;

extern s_game_globals*& global_game_globals;
extern uns32& g_active_structure_bsp_mask;
extern uns32& g_touched_structure_bsp_mask;
extern uns32& g_active_designer_zone_mask;
extern uns32& g_active_cinematic_zone_mask;
extern uns32& g_touched_cinematic_zone_mask;

extern const s_cluster_reference* cluster_reference_set(s_cluster_reference* cluster_reference, int32 bsp_index, int32 cluster_index);
extern bool cluster_reference_valid(const s_cluster_reference* cluster_reference);

extern uns32 __cdecl global_cinematic_zone_active_mask_get();
extern uns32 __cdecl global_designer_zone_active_mask_get();
extern struct scenario* __cdecl global_scenario_get();
extern int32 __cdecl global_scenario_index_get();
extern struct scenario* __cdecl global_scenario_try_and_get();
extern uns32 __cdecl global_structure_bsp_active_mask_get();
extern int32 __cdecl global_structure_bsp_first_active_index_get();
extern structure_bsp* __cdecl global_structure_bsp_get(int32 structure_bsp_index);
extern bool __cdecl global_structure_bsp_is_active(int32 structure_bsp_index);
extern int32 __cdecl global_structure_bsp_next_active_index_get(int32 structure_bsp_index);
extern s_structure_design* global_structure_design_get(int32 structure_bsp_index);
extern bool __cdecl scenario_activate_initial_designer_zones(int32 zone_set_index);
extern bool __cdecl scenario_activate_initial_zone_set(int32 zone_set_index);
extern s_cluster_reference __cdecl scenario_cluster_reference_from_point(const real_point3d* point);
extern bool __cdecl scenario_cluster_reference_valid(const s_cluster_reference* cluster_reference);
extern bool __cdecl scenario_connect_game_to_new_bsps(uns32 game_structure_bsp_mask, uns32 new_structure_bsp_mask);
extern bool __cdecl scenario_connect_zone_set_resources(
	int32 new_zone_set_index,
	uns32 new_active_bsp_mask,
	uns32 new_touched_bsp_mask,
	uns32 new_designer_zone_mask,
	uns32 new_cinematic_zone_mask,
	uns32 new_touched_cinematic_zone_mask,
	bool unload_old_bsps);
extern void __cdecl scenario_disconnect_from_old_zone_set(uns32 loaded_structure_bsp_mask, uns32 new_structure_bsp_mask);
extern bool __cdecl scenario_disconnect_game_from_old_bsps(uns32 old_structure_bsp_mask, uns32 new_structure_bsp_mask);
extern void __cdecl scenario_dispose();
extern void __cdecl scenario_dispose_from_old_map();
extern void __cdecl scenario_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl scenario_frame_update(real32 game_seconds_elapsed);
extern void __cdecl scenario_game_state_grab_global_state(s_scenario_game_state* out_game_state);
extern const char* __cdecl scenario_get_cinematic_zone_string_from_mask(uns32 cinematic_zone_mask, char* cinematic_zone_string, uns32 cinematic_zone_string_size);
extern s_game_globals* __cdecl scenario_get_game_globals();
extern int32 __cdecl scenario_get_designer_zone_index_by_name(const struct scenario* scenario, const char* name);
extern const char* __cdecl scenario_get_designer_zone_string_from_mask(uns32 designer_zone_mask, char* designer_zone_string, uns32 designer_zone_string_size);
extern void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state);
extern const char* __cdecl scenario_get_structure_bsp_string_from_mask(uns32 structure_bsp_mask, char* structure_bsp_string, uns32 structure_bsp_string_size);
extern int32 __cdecl scenario_get_zone_set_index_by_name(const struct scenario* scenario, const char* name, bool strip_path);
extern void __cdecl scenario_initialize();
extern void __cdecl scenario_initialize_for_new_map();
extern void __cdecl scenario_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl scenario_initialize_game_state(s_scenario_game_state* out_game_state);
extern void __cdecl scenario_invalidate();
extern void __cdecl scenario_invalidate_zone_set_internal();
extern bool __cdecl scenario_language_pack_load();
extern void __cdecl scenario_language_pack_unload();
extern bool __cdecl scenario_load(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path);
extern bool __cdecl scenario_load_resources_blocking(bool include_pending_in_blockingness);
extern void __cdecl scenario_location_from_point(s_location* location, const real_point3d* point);
extern bool __cdecl scenario_modify_zone_activation_internal(int32 new_zone_set_index, uns32 old_structure_bsp_mask, uns32 new_structure_bsp_mask, uns32 new_touched_bsp_mask, const s_scenario_zone_change* non_bsp_zone_change, uns32 new_touched_cinematics_mask, bool unload_old_bsps);
extern int16 __cdecl scenario_object_name_index_from_string(struct scenario* scenario, const char* name);
extern bool __cdecl scenario_preload_initial_zone_set(int16 initial_zone_set_index);
extern void __cdecl scenario_prepare_for_map_reset(int16 initial_zone_set_index);
extern bool __cdecl scenario_prepare_to_switch_zone_set(int32 new_zone_set_index);
extern void __cdecl scenario_reset_zone_resources_from_main();
extern void __cdecl scenario_switch_to_null_zone_set();
extern bool __cdecl scenario_switch_zone_set(int32 zone_set_index);
extern bool __cdecl scenario_switch_zone_set_internal(int32 new_zone_set_index, bool unload_old_bsps);
extern const char* __cdecl scenario_tag_get_structure_bsp_name(int32 scenario_index, int32 structure_bsp_index);
extern bool __cdecl scenario_tags_match(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path);
extern void __cdecl scenario_tags_teardown();
extern bool __cdecl scenario_test_pvs(s_cluster_reference cluster_reference0, s_cluster_reference cluster_reference1);
extern s_game_globals* __cdecl scenario_try_and_get_game_globals();
extern void __cdecl scenario_unload();
extern void __cdecl scenario_zone_set_debug_status(const char* status, int32 zone_set_index);
extern int32 __cdecl scenario_zone_set_index_get();
extern int32 __cdecl scenario_zone_set_index_get_if_fully_activated();
extern bool __cdecl scenario_zone_set_is_fully_active(int32 zone_set_index);
extern int32 __cdecl scenario_zone_set_name_get();
extern const structure_bsp* __cdecl scenario_structure_bsp_get(const struct scenario* scenario, int32 structure_bsp_index);


