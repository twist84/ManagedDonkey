#pragma once

#include "cseries/cseries.hpp"

extern dword __cdecl scenario_zone_set_bsp_active_mask_get(long zone_set_index);
extern dword __cdecl scenario_zone_set_cinematic_zone_required_mask_get(long zone_set_index);
extern dword __cdecl scenario_zone_set_designer_zone_forbidden_mask_get(long zone_set_index);
extern dword __cdecl scenario_zone_set_designer_zone_required_mask_get(long zone_set_index);
extern char const* scenario_get_designer_zone_name(struct scenario* scenario, long designer_zone_index);
extern char const* scenario_get_cinematic_zone_name(struct scenario* scenario, long cinematic_zone_index);

