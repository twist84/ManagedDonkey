#pragma once

#include "game/game_engine_default.hpp"

enum e_weapon_set;

extern c_game_engine const* __cdecl current_game_engine();
extern long __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(short absolute_weapons_selection_block_index, e_weapon_set weapon_set);

