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

struct s_game_globals;
struct structure_bsp;
struct s_structure_design;

extern long& global_scenario_index;
extern long& global_scenario_game_globals_index;

extern s_scenario*& global_scenario;

extern s_game_globals*& global_game_globals;
extern dword& g_active_structure_bsp_mask;
extern dword& g_touched_structure_bsp_mask;
extern dword& g_active_designer_zone_mask;
extern dword& g_active_cinematic_zone_mask;
extern dword& g_touched_cinematic_zone_mask;

extern s_scenario* global_scenario_get();
extern s_scenario* global_scenario_try_and_get();
extern s_game_globals* scenario_get_game_globals();
extern s_game_globals* scenario_try_and_get_game_globals();
extern long __cdecl global_structure_bsp_first_active_index_get();
extern structure_bsp* __cdecl global_structure_bsp_get(long structure_bsp_index);
extern bool __cdecl global_structure_bsp_is_active(long structure_bsp_index);
extern long __cdecl global_structure_bsp_next_active_index_get(long structure_bsp_index);
extern s_structure_design* global_structure_design_get(long structure_bsp_index);
extern bool __cdecl scenario_activate_initial_designer_zones(long zoneset_index);
extern bool __cdecl scenario_activate_initial_zone_set(long zoneset_index);
extern void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state);
extern void __cdecl scenario_invalidate();
extern bool __cdecl scenario_language_pack_load();
extern bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path);
extern bool __cdecl scenario_load_resources_blocking(bool a1);
extern bool __cdecl scenario_preload_initial_zone_set(short zone_set_index);
extern bool __cdecl scenario_switch_zone_set(long zoneset_index);
extern long __cdecl scenario_zone_set_index_get();
extern long global_scenario_index_get();
extern char const* scenario_get_structure_bsp_string_from_mask(dword mask, char* structure_bsp_string, dword structure_bsp_string_size);
extern bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path);
extern short scenario_object_name_index_from_string(s_scenario* scenario, char const* name);
extern long scenario_get_zone_set_index_by_name(s_scenario const* scenario, char const* name, bool strip_path);
extern long scenario_get_designer_zone_index_by_name(s_scenario const* scenario, char const* name);

