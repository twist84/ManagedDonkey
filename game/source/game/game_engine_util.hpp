#pragma once

#include "game/game_engine_default.hpp"

class c_game_engine;
class c_game_variant;
struct s_game_engine_event_data;

extern c_game_engine* (&game_engines)[k_game_engine_type_count];

extern const c_game_engine* __cdecl current_game_engine();
extern const c_game_variant* __cdecl current_game_variant();
extern void __cdecl build_multiplayer_string(int32 player_index, const wchar_t* raw_string, const s_game_engine_event_data* event_data, int32 buffer_size, wchar_t* buffer);
extern int16 __cdecl game_engine_get_multiplayer_weapon_selection_absolute_index(int32 name);
extern int32 __cdecl game_engine_get_multiplayer_weapon_selection_name(int16 absolute_index);
extern int32 __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(int16 absolute_index, e_weapon_set weapon_set);
extern int32 __cdecl game_engine_add_starting_weapon_to_player(int32 unit_index, int32 weapon_index, int32 mode);
extern void __cdecl game_engine_initialize_event(int32 event_type, int32 event, s_game_engine_event_data* event_data);

