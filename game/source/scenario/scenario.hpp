#pragma once

#include "scenario/scenario_definitions.hpp"

struct s_scenario_game_state
{
	uint32 scenario_designer_zones_active;
	uint32 zone_set_index;
	uint32 active_designer_zone_mask;
	uint32 active_structure_bsp_mask;
	uint32 touched_structure_bsp_mask;
	uint32 active_cinematic_zone_mask;
	uint32 touched_cinematic_zone_mask;
	uint32 loaded_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_game_state) == 0x20);

struct s_scenario_zone_state
{
	bool using_designer_zones;
	uint32 active_bsp_mask;
	uint32 touched_bsp_mask;
	uint32 active_designer_zone_mask;
	uint32 active_cinematic_zone_mask;
	uint32 pending_bsp_zone_mask;
	uint32 pending_designer_zone_mask;
	uint32 pending_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_state) == 0x20);

struct s_game_non_bsp_zone_set
{
	uint32 designer_zone_mask;
	uint32 cinematic_zone_mask;
};
static_assert(sizeof(s_game_non_bsp_zone_set) == 0x8);

struct s_scenario_zone_change
{
	bool any_cinematic_zone_changes() const;
	bool any_designer_zone_changes() const;
	bool any_zone_changes() const;
	uint32 get_activating_cinematic_zone_mask() const;
	uint32 get_activating_designer_zone_mask() const;
	uint32 get_deactivating_cinematic_zone_mask() const;
	uint32 get_deactivating_designer_zone_mask() const;
	uint32 pre_switch_cinematic_zone_mask() const;
	uint32 pre_switch_designer_zone_mask() const;

	uint32 original_designer_zone_mask;
	uint32 original_cinematic_zone_mask;
	uint32 new_designer_zone_mask;
	uint32 new_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_change) == 0x10);

enum e_campaign_id;
enum e_map_id;

struct s_game_globals;
struct structure_bsp;
struct s_structure_design;
struct s_cluster_reference;

extern long& global_scenario_index;
extern long& global_scenario_game_globals_index;

extern struct scenario*& global_scenario;

extern s_game_globals*& global_game_globals;
extern uint32& g_active_structure_bsp_mask;
extern uint32& g_touched_structure_bsp_mask;
extern uint32& g_active_designer_zone_mask;
extern uint32& g_active_cinematic_zone_mask;
extern uint32& g_touched_cinematic_zone_mask;

extern uint32 __cdecl global_cinematic_zone_active_mask_get();
extern uint32 __cdecl global_designer_zone_active_mask_get();
extern struct scenario* __cdecl global_scenario_get();
extern long __cdecl global_scenario_index_get();
extern struct scenario* __cdecl global_scenario_try_and_get();
extern uint32 __cdecl global_structure_bsp_active_mask_get();
extern long __cdecl global_structure_bsp_first_active_index_get();
extern structure_bsp* __cdecl global_structure_bsp_get(long structure_bsp_index);
extern bool __cdecl global_structure_bsp_is_active(long structure_bsp_index);
extern long __cdecl global_structure_bsp_next_active_index_get(long structure_bsp_index);
extern s_structure_design* global_structure_design_get(long structure_bsp_index);
extern bool __cdecl scenario_activate_initial_designer_zones(long zone_set_index);
extern bool __cdecl scenario_activate_initial_zone_set(long zone_set_index);
extern s_cluster_reference __cdecl scenario_cluster_reference_from_point(real_point3d const* point);
extern bool __cdecl scenario_connect_game_to_new_bsps(uint32 game_structure_bsp_mask, uint32 new_structure_bsp_mask);
extern bool __cdecl scenario_connect_zone_set_resources(
	long new_zone_set_index,
	uint32 new_active_bsp_mask,
	uint32 new_touched_bsp_mask,
	uint32 new_designer_zone_mask,
	uint32 new_cinematic_zone_mask,
	uint32 new_touched_cinematic_zone_mask,
	bool unload_old_bsps);
extern void __cdecl scenario_disconnect_from_old_zone_set(uint32 loaded_structure_bsp_mask, uint32 new_structure_bsp_mask);
extern bool __cdecl scenario_disconnect_game_from_old_bsps(uint32 old_structure_bsp_mask, uint32 new_structure_bsp_mask);
extern void __cdecl scenario_dispose();
extern void __cdecl scenario_dispose_from_old_map();
extern void __cdecl scenario_dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask);
extern void __cdecl scenario_frame_update(real32 game_seconds_elapsed);
extern char const* __cdecl scenario_get_cinematic_zone_string_from_mask(uint32 cinematic_zone_mask, char* cinematic_zone_string, uint32 cinematic_zone_string_size);
extern s_game_globals* __cdecl scenario_get_game_globals();
extern long __cdecl scenario_get_designer_zone_index_by_name(struct scenario const* scenario, char const* name);
extern char const* __cdecl scenario_get_designer_zone_string_from_mask(uint32 designer_zone_mask, char* designer_zone_string, uint32 designer_zone_string_size);
extern void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state);
extern char const* __cdecl scenario_get_structure_bsp_string_from_mask(uint32 structure_bsp_mask, char* structure_bsp_string, uint32 structure_bsp_string_size);
extern long __cdecl scenario_get_zone_set_index_by_name(struct scenario const* scenario, char const* name, bool strip_path);
extern void __cdecl scenario_initialize();
extern void __cdecl scenario_initialize_for_new_map();
extern void __cdecl scenario_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask);
extern void __cdecl scenario_invalidate();
extern void __cdecl scenario_invalidate_zone_set_internal();
extern bool __cdecl scenario_language_pack_load();
extern void __cdecl scenario_language_pack_unload();
extern bool __cdecl scenario_load(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path);
extern bool __cdecl scenario_load_resources_blocking(bool include_pending_in_blockingness);
extern void __cdecl scenario_location_from_point(s_location* location, real_point3d const* point);
extern bool __cdecl scenario_modify_zone_activation_internal(long new_zone_set_index, uint32 old_structure_bsp_mask, uint32 new_structure_bsp_mask, uint32 new_touched_bsp_mask, s_scenario_zone_change const* non_bsp_zone_change, uint32 new_touched_cinematics_mask, bool unload_old_bsps);
extern short __cdecl scenario_object_name_index_from_string(struct scenario* scenario, char const* name);
extern bool __cdecl scenario_preload_initial_zone_set(short initial_zone_set_index);
extern void __cdecl scenario_prepare_for_map_reset(short initial_zone_set_index);
extern bool __cdecl scenario_prepare_to_switch_zone_set(long new_zone_set_index);
extern void __cdecl scenario_reset_zone_resources_from_main();
extern void __cdecl scenario_switch_to_null_zone_set();
extern bool __cdecl scenario_switch_zone_set(long zone_set_index);
extern bool __cdecl scenario_switch_zone_set_internal(long new_zone_set_index, bool unload_old_bsps);
extern char const* __cdecl scenario_tag_get_structure_bsp_name(long scenario_index, long structure_bsp_index);
extern bool __cdecl scenario_tags_match(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path);
extern void __cdecl scenario_tags_teardown();
extern bool __cdecl scenario_test_pvs(s_cluster_reference cluster_reference0, s_cluster_reference cluster_reference1);
extern s_game_globals* __cdecl scenario_try_and_get_game_globals();
extern void __cdecl scenario_unload();
extern void __cdecl scenario_zone_set_debug_status(char const* status, long zone_set_index);
extern long __cdecl scenario_zone_set_index_get();
extern long __cdecl scenario_zone_set_index_get_if_fully_activated();
extern bool __cdecl scenario_zone_set_is_fully_active(long zone_set_index);
extern long __cdecl scenario_zone_set_name_get();
extern structure_bsp const* __cdecl scenario_structure_bsp_get(struct scenario const* scenario, long structure_bsp_index);


