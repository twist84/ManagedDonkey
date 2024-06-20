#pragma once

#include "cseries/cseries.hpp"

struct object_placement_data;

extern void __cdecl simulation_action_game_engine_globals_update(c_flags<long, qword, 64>& flags);
extern void __cdecl simulation_action_game_engine_player_update(long player_index, c_flags<long, qword, 64>& flags);
extern void __cdecl simulation_action_object_create(long object_index);
extern bool __cdecl simulation_query_object_is_predicted(long object_index);
extern bool __cdecl simulation_query_object_placement(object_placement_data const* data);

