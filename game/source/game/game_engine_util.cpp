#include "game/game_engine_util.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "units/units.hpp"

//HOOK_DECLARE(0x005CE150, current_game_engine);

REFERENCE_DECLARE_ARRAY(0x0471A920, c_game_engine*, game_engines, k_game_engine_type_count);

REFERENCE_DECLARE(0x0190B890, c_game_engine*, ctf_engine);// = &internal_ctf_engine;
REFERENCE_DECLARE(0x0190B8D0, c_ctf_engine, internal_ctf_engine);// = c_ctf_engine();

REFERENCE_DECLARE(0x0190E1E8, c_game_engine*, slayer_engine);// = &internal_slayer_engine;
REFERENCE_DECLARE(0x0190E218, c_slayer_engine, internal_slayer_engine);// = c_slayer_engine();

REFERENCE_DECLARE(0x0190E23C, c_game_engine*, oddball_engine);// = &internal_oddball_engine;
REFERENCE_DECLARE(0x0190E26C, c_oddball_engine, internal_oddball_engine);// = c_oddball_engine();

REFERENCE_DECLARE(0x0190E290, c_game_engine*, king_engine);// = &internal_king_engine;
REFERENCE_DECLARE(0x0190E2C0, c_king_engine, internal_king_engine);// = c_king_engine();

REFERENCE_DECLARE(0x018A1564, c_game_engine*, sandbox_engine);// = &internal_sandbox_engine;
REFERENCE_DECLARE(0x018A15BC, c_sandbox_engine, internal_sandbox_engine);// = c_sandbox_engine();

REFERENCE_DECLARE(0x0190E3E8, c_game_engine*, vip_engine);// = &internal_vip_engine;
REFERENCE_DECLARE(0x0190E418, c_vip_engine, internal_vip_engine);// = c_vip_engine();

REFERENCE_DECLARE(0x0190E2E0, c_game_engine*, juggernaut_engine);// = &internal_juggernaut_engine;
REFERENCE_DECLARE(0x0190E310, c_juggernaut_engine, internal_juggernaut_engine);// = c_juggernaut_engine();

REFERENCE_DECLARE(0x0190E338, c_game_engine*, territories_engine);// = &internal_territories_engine;
REFERENCE_DECLARE(0x0190E368, c_territories_engine, internal_territories_engine);// = c_territories_engine();

REFERENCE_DECLARE(0x018EB5A8, c_game_engine*, assault_engine);// = &internal_assault_engine;
REFERENCE_DECLARE(0x018EB5E8, c_assault_engine, internal_assault_engine);// = c_assault_engine();

REFERENCE_DECLARE(0x0190E390, c_game_engine*, infection_engine);// = &internal_infection_engine;
REFERENCE_DECLARE(0x0190E3C0, c_infection_engine, internal_infection_engine);// = c_infection_engine();

const c_game_engine* __cdecl current_game_engine()
{
	//return INVOKE(0x005CE150, current_game_engine);

	const c_game_engine* game_engine = nullptr;
	if (game_engine_globals && IN_RANGE(game_engine_globals->game_engine_index, _game_engine_type_none, k_game_engine_type_count))
	{
		game_engine = game_engines[game_engine_globals->game_engine_index];
	}
	return game_engine;
}

const c_game_variant* __cdecl current_game_variant()
{
	//return INVOKE(0x005CE190, current_game_variant);

	const c_game_variant* variant = nullptr;
	game_options* options = game_options_get();
	if (options->game_mode == _game_mode_multiplayer)
	{
		variant = &options->multiplayer_variant;
	}
	return variant;
}

void __cdecl build_multiplayer_string(int32 player_index, const wchar_t* raw_string, const s_game_engine_event_data* event_data, int32 buffer_size, wchar_t* buffer)
{
	INVOKE(0x005CE070, build_multiplayer_string, player_index, raw_string, event_data, buffer_size, buffer);
}

int16 __cdecl game_engine_get_multiplayer_weapon_selection_absolute_index(int32 name)
{
	return INVOKE(0x005CE4A0, game_engine_get_multiplayer_weapon_selection_absolute_index, name);
}

int32 __cdecl game_engine_get_multiplayer_weapon_selection_name(int16 absolute_index)
{
	return INVOKE(0x005CE560, game_engine_get_multiplayer_weapon_selection_name, absolute_index);
}

int32 __cdecl game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(int16 absolute_index, e_weapon_set weapon_set)
{
	return INVOKE(0x005CECD0, game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index, absolute_index, weapon_set);
}

int32 __cdecl game_engine_add_starting_weapon_to_player(int32 unit_index, int32 weapon_index, int32 mode)
{
	return INVOKE(0x005CE210, game_engine_add_starting_weapon_to_player, unit_index, weapon_index, mode);
}

void __cdecl game_engine_initialize_event(int32 event_type, int32 event, s_game_engine_event_data* event_data)
{
	INVOKE(0x005CEA30, game_engine_initialize_event, event_type, event, event_data);
}

