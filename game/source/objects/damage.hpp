#pragma once

#include "cseries/cseries.hpp"

extern bool debug_damage_radius;
extern bool debug_damage_this_event;
extern bool debug_damage_verbose;
extern bool debug_player_damage;
extern bool debug_damage;

extern void render_debug_object_damage();
extern real __cdecl compute_total_damage(void* damage_data, void* damage_effect_definition, void const* damage_definition, long object_index, bool* a5);

