#include "players.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
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

long multiplayer_universal_data_get_equipment_absolute_index_from_name(char const* name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	long result = 0;
	for (long equipment_absolute_index = 0; equipment_absolute_index < universal_data->equipment.count; equipment_absolute_index++)
	{
		s_multiplayer_equipment& equipment = universal_data->equipment.elements[equipment_absolute_index];
		char const* equipment_name = equipment.name.get_string();
		if (equipment_name && csstricmp(name, equipment_name) == 0)
		{
			result = equipment_absolute_index;
			break;
		}
	}

	return result;
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

// this function gets called in `player_spawn` -> `game_engine_add_starting_equipment`
// #TODO: hook `game_engine_add_starting_equipment` and reimplement the original functionality
s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon_configuration.loadouts[player->active_weapon_loadout];

	// allow player traits override
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	loadout.grenade_index = _grenade_type_firebomb;

	// #TODO: pull these from a config file
	loadout.consumables[0] = static_cast<char>(multiplayer_universal_data_get_equipment_absolute_index_from_name("reactive_armor"));
	loadout.consumables[1] = static_cast<char>(multiplayer_universal_data_get_equipment_absolute_index_from_name("consumable_vision"));
	loadout.consumables[2] = static_cast<char>(multiplayer_universal_data_get_equipment_absolute_index_from_name("bubbleshield_tutorial"));
	loadout.consumables[3] = static_cast<char>(multiplayer_universal_data_get_equipment_absolute_index_from_name("consumable_vision_tutorial"));

	return &loadout;
}