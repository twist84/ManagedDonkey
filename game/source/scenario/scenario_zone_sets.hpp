#pragma once

typedef struct scenario s_scenario;

extern uns32 __cdecl scenario_zone_set_bsp_active_mask_get(int32 zone_set_index);
extern uns32 __cdecl scenario_zone_set_cinematic_zone_required_mask_get(int32 zone_set_index);
extern uns32 __cdecl scenario_zone_set_compute_new_designer_zone_mask(uns32 a1, uns32 a2, uns32 a3);
extern uns32 __cdecl scenario_zone_set_designer_zone_forbidden_mask_get(int32 zone_set_index);
extern uns32 __cdecl scenario_zone_set_designer_zone_required_mask_get(int32 zone_set_index);
extern const char* scenario_get_designer_zone_name(s_scenario* scenario, int32 designer_zone_index);
extern const char* scenario_get_cinematic_zone_name(s_scenario* scenario, int32 cinematic_zone_index);

