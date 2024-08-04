#pragma once

#include "scenario/scenario_definitions.hpp"

struct s_scenario_game_state
{
	dword scenario_designer_zones_active;
	dword zone_set_index;
	dword active_designer_zone_mask;
	dword active_structure_bsp_mask;
	dword touched_structure_bsp_mask;
	dword active_cinematic_zone_mask;
	dword touched_cinematic_zone_mask;
	dword loaded_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_game_state) == 0x20);

struct s_scenario_zone_state
{
	bool using_designer_zones;
	dword active_bsp_mask;
	dword touched_bsp_mask;
	dword active_designer_zone_mask;
	dword active_cinematic_zone_mask;
	dword pending_bsp_zone_mask;
	dword pending_designer_zone_mask;
	dword pending_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_state) == 0x20);

struct s_scenario_zone_change
{
	dword get_deactivating_designer_zone_mask() const;
	dword get_deactivating_cinematic_zone_mask() const;
	dword pre_switch_designer_zone_mask() const;
	dword pre_switch_cinematic_zone_mask() const;
	dword get_activating_designer_zone_mask() const;
	dword get_activating_cinematic_zone_mask() const;
	bool any_designer_zone_changes() const;
	bool any_cinematic_zone_changes() const;
	bool any_zone_changes() const;

	dword active_designer_zone_mask;
	dword active_cinematic_zone_mask;
	dword pending_designer_zone_mask;
	dword pending_cinematic_zone_mask;
};

struct s_game_non_bsp_zone_set : s_scenario_zone_change
{
};

struct s_game_globals;
struct structure_bsp;
struct s_structure_design;
struct s_cluster_reference;

extern long& global_scenario_index;
extern long& global_scenario_game_globals_index;

extern struct scenario*& global_scenario;

extern s_game_globals*& global_game_globals;
extern dword& g_active_structure_bsp_mask;
extern dword& g_touched_structure_bsp_mask;
extern dword& g_active_designer_zone_mask;
extern dword& g_active_cinematic_zone_mask;
extern dword& g_touched_cinematic_zone_mask;

extern struct scenario* global_scenario_get();
extern struct scenario* global_scenario_try_and_get();
extern s_game_globals* scenario_get_game_globals();
extern s_game_globals* scenario_try_and_get_game_globals();
extern dword global_structure_bsp_active_mask_get();
extern long __cdecl global_structure_bsp_first_active_index_get();
extern structure_bsp* __cdecl global_structure_bsp_get(long structure_bsp_index);
extern bool __cdecl global_structure_bsp_is_active(long structure_bsp_index);
extern long __cdecl global_structure_bsp_next_active_index_get(long structure_bsp_index);
extern s_structure_design* global_structure_design_get(long structure_bsp_index);
extern bool __cdecl scenario_activate_initial_designer_zones(long zoneset_index);
extern bool __cdecl scenario_activate_initial_zone_set(long zoneset_index);
extern s_cluster_reference __cdecl scenario_cluster_reference_from_point(real_point3d const* point);
extern void __cdecl scenario_frame_update(real game_seconds_elapsed);
extern void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state);
extern void __cdecl scenario_invalidate();
extern bool __cdecl scenario_language_pack_load();
extern void __cdecl scenario_language_pack_unload();
extern bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path);
extern bool __cdecl scenario_load_resources_blocking(bool a1);
extern bool __cdecl scenario_preload_initial_zone_set(short zone_set_index);
extern bool __cdecl scenario_switch_zone_set(long zoneset_index);
extern long __cdecl scenario_zone_set_index_get();
extern long global_scenario_index_get();
extern char const* scenario_get_structure_bsp_string_from_mask(dword mask, char* structure_bsp_string, dword structure_bsp_string_size);
extern bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path);
extern void __cdecl scenario_unload();
extern structure_bsp const* __cdecl scenario_structure_bsp_get(struct scenario const* scenario, long structure_bsp_index);
extern short scenario_object_name_index_from_string(struct scenario* scenario, char const* name);
extern long scenario_get_zone_set_index_by_name(struct scenario const* scenario, char const* name, bool strip_path);
extern long scenario_get_designer_zone_index_by_name(struct scenario const* scenario, char const* name);

