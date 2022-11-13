#include "players.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "memory/module.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE(0x00536680, player_get_weapon_loadout);

long __cdecl players_first_active_user()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00589A30, players_first_active_user);
}

bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position)
{
	return INVOKE(0x0053F550, player_teleport, player_index, object_index, position);
}

long get_equipment_index_by_name(char const* name)
{
	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	s_multiplayer_globals_definition* multiplayer_globals = static_cast<s_multiplayer_globals_definition*>(tag_get(game_globals->multiplayer_globals.group_tag, game_globals->multiplayer_globals.index));

	s_multiplayer_universal_globals_definition* universal_data = multiplayer_globals->universal.elements;
	if (!universal_data)
		return 0;

	for (long equipment_index = 0; equipment_index < universal_data->equipment.count; equipment_index++)
	{
		s_multiplayer_equipment& equipment = universal_data->equipment.elements[equipment_index];
		char const* equipment_name = equipment.name.get_string();
		if (equipment_name && csstricmp(name, equipment_name) == 0)
			return equipment_index;
	}

	return 0;
}

// "empty"
// "jammer"
// "powerdrain"
// "invisibility"
// "invisibility_vehicle"
// "bubbleshield"
// "superflare"
// "regenerator"
// "tripmine"
// "auto_turret"
// "deployable_cover"
// "forced_reload"
// "concussive_blast"
// "tank_mode"
// "mag_pulse"
// "hologram"
// "reactive_armor"
// "bomb_run"
// "armor_lock"
// "adrenaline"
// "lightning_strike"
// "scrambler"
// "weapon_jammer"
// "ammo_pack"
// "consumable_vision"
// "bubbleshield_tutorial"
// "consumable_vision_tutorial"

s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon_configuration.loadouts[player->active_weapon_loadout];

	// allow player traits override
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	loadout.grenade_index = _grenade_type_firebomb;

	// #TODO: pull these from a config file
	loadout.consumables[0] = static_cast<char>(get_equipment_index_by_name("reactive_armor"));
	loadout.consumables[1] = static_cast<char>(get_equipment_index_by_name("consumable_vision"));
	loadout.consumables[2] = static_cast<char>(get_equipment_index_by_name("bubbleshield_tutorial"));
	loadout.consumables[3] = static_cast<char>(get_equipment_index_by_name("consumable_vision_tutorial"));

	return &loadout;
}