#pragma once

#include "units/vehicle_definitions.hpp"

extern bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index);
extern e_vehicle_type __cdecl vehicle_get_type(long vehicle_index);
extern bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_render_debug(long vehicle_index);

