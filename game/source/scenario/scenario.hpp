#pragma once

#include "scenario/scenario_definitions.hpp"

extern long& global_scenario_index;
extern scenario*& global_scenario;

extern scenario* global_scenario_get();

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
extern bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path);

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
extern bool scenario_tags_match(long campaign_id, long map_id, char const* scenario_path);

extern bool __cdecl scenario_tags_load(char const* scenario_path);