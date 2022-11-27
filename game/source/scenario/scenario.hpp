#pragma once

#include "scenario/scenario_definitions.hpp"

extern long& global_scenario_index;
extern long& global_scenario_game_globals_index;

extern s_scenario*& global_scenario;

struct s_game_globals;
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

extern bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path);
extern void __cdecl scenario_invalidate();
extern bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path);