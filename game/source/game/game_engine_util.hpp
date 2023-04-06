#pragma once

#include "game/game_engine_default.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_slayer.hpp"
#include "game/game_engine_oddball.hpp"
#include "game/game_engine_king.hpp"
#include "game/game_engine_sandbox.hpp"
#include "game/game_engine_vip.hpp"
#include "game/game_engine_juggernaut.hpp"
#include "game/game_engine_territories.hpp"
#include "game/game_engine_assault.hpp"
#include "game/game_engine_infection.hpp"

enum e_weapon_set;

extern c_game_engine const* __cdecl current_game_engine();
extern c_game_variant const* __cdecl current_game_variant();
extern short __cdecl game_engine_get_multiplayer_weapon_selection_absolute_index(long name);
extern long __cdecl game_engine_get_multiplayer_weapon_selection_name(short absolute_index);
extern long __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(short absolute_weapons_selection_block_index, e_weapon_set weapon_set);
extern long __cdecl game_engine_add_starting_weapon_to_player(long unit_index, long definition_index, long method);

