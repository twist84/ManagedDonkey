#pragma once

#include "scenario/scenario_definitions.hpp"

extern long& global_scenario_index;
extern scenario*& global_scenario;

extern scenario* global_scenario_get();

extern bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path);
extern void __cdecl scenario_invalidate();
extern bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path);