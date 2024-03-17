#include "game/game_engine_util.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "units/units.hpp"

//HOOK_DECLARE(0x005CE150, current_game_engine);

REFERENCE_DECLARE_ARRAY(0x0471A920, c_game_engine*, game_engines, k_game_engine_type_count);

REFERENCE_DECLARE(0x0190B890, c_game_engine*, ctf_engine);
REFERENCE_DECLARE(0x0190B8D0, c_ctf_engine, internal_ctf_engine);

REFERENCE_DECLARE(0x0190E1E8, c_game_engine*, slayer_engine);
REFERENCE_DECLARE(0x0190E218, c_slayer_engine, internal_slayer_engine);

REFERENCE_DECLARE(0x0190E23C, c_game_engine*, oddball_engine);
REFERENCE_DECLARE(0x0190E26C, c_oddball_engine, internal_oddball_engine);

REFERENCE_DECLARE(0x0190E290, c_game_engine*, king_engine);
REFERENCE_DECLARE(0x0190E2C0, c_king_engine, internal_king_engine);

REFERENCE_DECLARE(0x018A1564, c_game_engine*, sandbox_engine);
REFERENCE_DECLARE(0x018A15BC, c_sandbox_engine, internal_sandbox_engine);

REFERENCE_DECLARE(0x0190E3E8, c_game_engine*, vip_engine);
REFERENCE_DECLARE(0x0190E418, c_vip_engine, internal_vip_engine);

REFERENCE_DECLARE(0x0190E2E0, c_game_engine*, juggernaut_engine);
REFERENCE_DECLARE(0x0190E310, c_juggernaut_engine, internal_juggernaut_engine);

REFERENCE_DECLARE(0x0190E338, c_game_engine*, territories_engine);
REFERENCE_DECLARE(0x0190E368, c_territories_engine, internal_territories_engine);

REFERENCE_DECLARE(0x018EB5A8, c_game_engine*, assault_engine);
REFERENCE_DECLARE(0x018EB5E8, c_assault_engine, internal_assault_engine);

REFERENCE_DECLARE(0x0190E390, c_game_engine*, infection_engine);
REFERENCE_DECLARE(0x0190E3C0, c_infection_engine, internal_infection_engine);

c_game_engine const* __cdecl current_game_engine()
{
	c_game_engine const* result = nullptr;
	//HOOK_INVOKE(result =, current_game_engine);

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	if (game_engine_globals && (game_engine_globals->game_engine_index > _game_engine_type_none && game_engine_globals->game_engine_index < k_game_engine_type_count))
		return game_engines[game_engine_globals->game_engine_index.get()];

	return result;
}

c_game_variant const* __cdecl current_game_variant()
{
	game_options* options = game_options_get();
	if (options->game_mode == _game_mode_multiplayer)
		return &options->game_variant;

	return nullptr;
}

short __cdecl game_engine_get_multiplayer_weapon_selection_absolute_index(long name)
{
	return INVOKE(0x005CE4A0, game_engine_get_multiplayer_weapon_selection_absolute_index, name);
}

long __cdecl game_engine_get_multiplayer_weapon_selection_name(short absolute_index)
{
	return INVOKE(0x005CE560, game_engine_get_multiplayer_weapon_selection_name, absolute_index);
}

long __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(short absolute_weapons_selection_block_index, e_weapon_set weapon_set)
{
	return INVOKE(0x005CECD0, game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index, absolute_weapons_selection_block_index, weapon_set);
}

long __cdecl game_engine_add_starting_weapon_to_player(long unit_index, long definition_index, long method)
{
	return INVOKE(0x005CE210, game_engine_add_starting_weapon_to_player, unit_index, definition_index, method);
}

