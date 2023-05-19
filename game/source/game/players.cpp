#include "players.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_console.hpp"
#include "cseries/cseries_windows.hpp"
#include "game/multiplayer_definitions.hpp"
#include "input/input_abstraction.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE(0x00536020, player_get_armor_loadout);
HOOK_DECLARE(0x00536680, player_get_weapon_loadout);
HOOK_DECLARE(0x0053F220, player_suppress_action);

void __cdecl player_set_unit_index(long player_index, long unit_index)
{
	INVOKE(0x0053CA80, player_set_unit_index, player_index, unit_index);
}

//void __cdecl player_suppress_action(long, enum e_player_suppress_action_type_enum)
void __cdecl player_suppress_action(long player_index, long player_suppress_action_type)
{
	//INVOKE(0x0053F220, player_suppress_action, player_index, player_suppress_action_type);

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);

	long input_user = player_mapping_get_input_user(player_index);
	switch (player_suppress_action_type)
	{
	case 0:
	{
		if (!TEST_BIT(player->flags, 4))
		{
			if (global_game_globals->input_globals.index != NONE)
			{
				s_input_globals_definition* input_globals = global_game_globals->input_globals.cast_to<s_input_globals_definition>();

				player->flags |= FLAG(4);
				player->__unknown2CD4 = game_seconds_to_ticks_round(input_globals->__unknown30 / 1000.0f);
				break;
			}

			player->__unknown2CD4 = 0;
		}

		player->flags |= FLAG(4);
	}
	break;
	case 1:
	{
		player->flags |= FLAG(5);
		if (input_user != NONE)
			player_control_suppress_rotate_weapons(input_user);
	}
	break;
	case 2:
	{
		player->flags |= FLAG(6);
	}
	break;
	}
}

bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position)
{
	return INVOKE(0x0053F550, player_teleport, player_index, object_index, position);
}

long __cdecl players_first_active_user()
{
	return INVOKE(0x00589A30, players_first_active_user);
}

s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player)
{
	s_s3d_player_armor_configuration_loadout& loadout = player->configuration.host.armor.loadouts[player->armor_loadout_index];

	if (!loadout.armor_is_set)
	{
		// #TODO: pull these from a config file
		loadout.armors[_armor_type_helmet] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("helmet", "tankmode_human"));
		loadout.armors[_armor_type_chest] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("chest", "tankmode_human"));
		loadout.armors[_armor_type_shoulders] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("shoulders", "tankmode_human"));
		loadout.armors[_armor_type_arms] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("arms", "tankmode_human"));
		loadout.armors[_armor_type_legs] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("legs", "tankmode_human"));
		loadout.armors[_armor_type_acc] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("acc", "bullet_shield"));
		loadout.armors[_armor_type_pelvis] = static_cast<byte>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index("pelvis", "tankmode_human"));

		for (long color_index = 0; color_index < k_color_type_count; color_index++)
		{
			rgb_color& color = loadout.colors[color_index];
			color.red = byte(~127);
			color.green = byte(~0);
			color.blue = byte(~127);
		}

		loadout.armor_is_set = true;
	}

	return &loadout;
}

// this function gets called in `player_spawn` -> `game_engine_add_starting_equipment`
// #TODO: hook `game_engine_add_starting_equipment` and reimplement the original functionality
s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon.loadouts[player->weapon_loadout_index];

	// allow player traits override
	// #TODO: pull these from tags
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	// #TODO: pull this from tags
	loadout.grenade_index = _grenade_type_firebomb;

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

	// #TODO: pull these from a config file
	loadout.consumables[0] = static_cast<char>(multiplayer_universal_data_get_absolute_equipment_block_index("adrenaline"));
	loadout.consumables[1] = static_cast<char>(multiplayer_universal_data_get_absolute_equipment_block_index("reactive_armor"));
	loadout.consumables[2] = static_cast<char>(multiplayer_universal_data_get_absolute_equipment_block_index("armor_lock"));
	loadout.consumables[3] = static_cast<char>(multiplayer_universal_data_get_absolute_equipment_block_index("tank_mode"));

	return &loadout;
}

// find a better name?
bool customized_area_selection_from_name(s_multiplayer_customized_model_selection& selection, char const* selection_name)
{
	char const* selection_name_str = selection.selection_name.get_string();

	if (selection.third_person_armor_object.index != 0xFFFFFFFF /*&& selection.first_person_armor_object.index != 0xFFFFFFFF*/)
	{
		if (selection_name_str && csstricmp(selection_name, selection_name_str) == 0)
			return true;
	}

	return false;
}

// find a better name?
long customized_spartan_character_from_name(s_multiplayer_customized_model_character& character, char const* region_or_biped_name, char const* selection_name)
{
	char const* armor_region = character.armor_region.get_string();
	char const* biped_region = character.biped_region.get_string();

	if (armor_region && csstricmp(region_or_biped_name, armor_region) == 0)
	{
		for (long selection_absolute_index = 0; selection_absolute_index < character.customized_areas.count(); selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_areas[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	if (biped_region && csstricmp(region_or_biped_name, biped_region) == 0)
	{
		for (long selection_absolute_index = 0; selection_absolute_index < character.customized_areas.count(); selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_areas[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	return 0;
}

long multiplayer_universal_data_get_absolute_equipment_block_index(char const* name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	long result = 0;
	for (long equipment_absolute_index = 0; equipment_absolute_index < universal_data->equipment.count(); equipment_absolute_index++)
	{
		s_multiplayer_equipment& equipment = universal_data->equipment[equipment_absolute_index];
		char const* equipment_name = equipment.name.get_string();
		if (equipment_name && csstricmp(name, equipment_name) == 0)
		{
			result = equipment_absolute_index;
			break;
		}
	}

	return result;
}

// find a better name?
long multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(char const* region_or_biped_name, char const* selection_name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	long result = 0;
	for (s_multiplayer_customized_model_character& customized_model_character : universal_data->customized_spartan_characters)
	{
		result = customized_spartan_character_from_name(customized_model_character, region_or_biped_name, selection_name);
		if (result != 0)
			break;
	}

	return result;
}

// find a better name?
short multiplayer_universal_data_get_absolute_weapons_selection_block_index(char const* selection_name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return short(0xFFFD);

	if (universal_data->weapon_selections.count())
	{
		for (short weapon_selection_index = 0; weapon_selection_index < universal_data->weapon_selections.count(); weapon_selection_index++)
		{
			s_multiplayer_weapon_selection& weapon_selection = universal_data->weapon_selections[weapon_selection_index];

			char const* name = weapon_selection.name.get_string();
			if (name && csstricmp(selection_name, name) == 0)
				return weapon_selection_index;
		}
	}

	return short(0xFFFD);
}

