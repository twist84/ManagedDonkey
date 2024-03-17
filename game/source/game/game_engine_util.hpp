#pragma once

#include "game/game_engine_default.hpp"

enum e_weapon_set;

struct c_game_engine;
struct c_game_variant;

extern c_game_engine* (&game_engines)[k_game_engine_type_count];

extern c_game_engine const* __cdecl current_game_engine();
extern c_game_variant const* __cdecl current_game_variant();
extern short __cdecl game_engine_get_multiplayer_weapon_selection_absolute_index(long name);
extern long __cdecl game_engine_get_multiplayer_weapon_selection_name(short absolute_index);
extern long __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(short absolute_weapons_selection_block_index, e_weapon_set weapon_set);
extern long __cdecl game_engine_add_starting_weapon_to_player(long unit_index, long definition_index, long method);

