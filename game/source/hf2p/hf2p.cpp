#include "hf2p/hf2p.hpp"

#include "cache/cache_files.hpp"
#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "game/multiplayer_definitions.hpp"
#include "game/players.hpp"
#include "main/console.hpp"
#include "main/global_preferences.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "objects/objects.hpp"
#include "units/bipeds.hpp"
#include "units/units.hpp"

#include <stdlib.h>

//#define ALLOW_LOADING_CONSUMABLES_FROM_FILE

REFERENCE_DECLARE(0x04FE67A0, int32, mainmenu_spartan_unit_index);
REFERENCE_DECLARE(0x04FE67A4, int32, mainmenu_elite_unit_index);

HOOK_DECLARE(0x00600600, hf2p_handle_deleted_object);
HOOK_DECLARE(0x00600620, hf2p_initialize_for_new_map);
HOOK_DECLARE(0x00600630, hf2p_initialize);
HOOK_DECLARE(0x006006F0, hf2p_game_initialize);
HOOK_DECLARE(0x00600750, hf2p_scenario_tags_load_finished);
HOOK_DECLARE(0x00600770, hf2p_scenario_load);
HOOK_DECLARE(0x00600790, hf2p_game_dispose);
HOOK_DECLARE(0x00600830, hf2p_dispose_from_old_map);
HOOK_DECLARE(0x00600850, hf2p_game_update);
HOOK_DECLARE(0x006008F0, hf2p_idle);
HOOK_DECLARE(0x00600900, hf2p_render);
HOOK_DECLARE(0x00600910, hf2p_bot_client_initialize);
HOOK_DECLARE(0x00600960, hf2p_dedicated_server_initialize);
HOOK_DECLARE(0x00600990, hf2p_client_initialize);
HOOK_DECLARE(0x00600A20, hf2p_security_initialize);
HOOK_DECLARE(0x00600A80, hf2p_editor_initialize);
HOOK_DECLARE(0x00600A90, hf2p_main_initialize);
HOOK_DECLARE(0x00600C70, hf2p_config_initialize);
HOOK_DECLARE(0x00600D40, hf2p_bot_client_dispose);
HOOK_DECLARE(0x00600D80, hf2p_dedicated_server_dispose);
HOOK_DECLARE(0x00600DB0, hf2p_client_dispose);
HOOK_DECLARE(0x00600E00, hf2p_main_dispose);
HOOK_DECLARE(0x00600E80, hf2p_bot_client_update);
HOOK_DECLARE(0x00600EB0, hf2p_dedicated_server_update);
HOOK_DECLARE(0x00600ED0, hf2p_client_update);

HOOK_DECLARE(0x007B8810, hf2p_backend_register);
HOOK_DECLARE(0x007B8830, hf2p_backend_unregister);
HOOK_DECLARE(0x007B8870, hf2p_backend_update);

void __cdecl hf2p_handle_deleted_object(int32 object_index)
{
}

void __cdecl hf2p_initialize_for_new_map()
{
}

void __cdecl hf2p_initialize()
{
}

void __cdecl hf2p_game_initialize()
{
}

void __cdecl hf2p_scenario_tags_load_finished()
{
}

void __cdecl hf2p_scenario_load()
{
}

void __cdecl hf2p_game_dispose()
{
}

void __cdecl hf2p_dispose_from_old_map()
{
}

int32& mainmenu_unit_index = mainmenu_spartan_unit_index;

void __cdecl sub_7B7940()
{
	mainmenu_spartan_unit_index = NONE;
	mainmenu_elite_unit_index = NONE;

	if (game_is_ui_shell())
	{
		c_object_iterator<biped_datum> biped_iterator;
		biped_iterator.begin(_object_mask_biped, 0);
		while (biped_iterator.next())
		{
			int32 player_representation_index = player_unit_get_representation_index(biped_iterator.get_index());
			switch (player_representation_index)
			{
			case 6: // ui_spartan
				mainmenu_spartan_unit_index = biped_iterator.get_index();
				break;
			case 7: // ui_elite
				mainmenu_elite_unit_index = biped_iterator.get_index();
				break;
			}
		}
	}
}

void __cdecl hf2p_game_update()
{
	// this function runs in `main_loop_body`

	sub_7B7940();

	if (mainmenu_unit_index != NONE)
	{
		static bool first_run = true;
		if (first_run && g_cache_file_globals.tags_loaded)
		{
			first_run = false;

			console_process_command("load_customization_from_file customization.txt", true);
		}

		{
			// only allow one instance of this
			s_s3d_player_armor_configuration_loadout& loadout = get_armor_loadout();

			DECLFUNC(0x005A4430, void, __cdecl, s_s3d_player_armor_configuration_loadout*, uns32)(&loadout, mainmenu_unit_index);

			// $TODO when we have full control over player profile and player customization maybe update and use this
			//c_player_profile_interface* player_profile = controller_get(_controller0)->get_player_profile_interface();
			//e_player_color_index primary_change_color = player_profile->get_primary_change_color();
			//e_player_color_index secondary_change_color = player_profile->get_secondary_change_color();
			//
			//if (VALID_INDEX(primary_change_color, k_player_color_index_count))
			//	loadout.colors[_color_type_primary] = s_game_globals::profile_colors[primary_change_color];
			//
			//if (VALID_INDEX(secondary_change_color, k_player_color_index_count))
			//	loadout.colors[_color_type_secondary] = s_game_globals::profile_colors[secondary_change_color];

			for (int32 color_index = 0; color_index < k_color_type_count; color_index++)
			{
				rgb_color& color = loadout.colors[color_index];
				real_rgb_color real_color;
				pixel32_to_real_rgb_color(color, &real_color);
				object_set_base_change_color_by_index(mainmenu_unit_index, color_index, &real_color);
			}
			DECLFUNC(0x005A2FA0, void, __cdecl, uns32)(mainmenu_unit_index);
		}

		int32 primary_weapon_index = game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(/* random */ int16(0xFFFD), _unit_weapon_set_primary);
		game_engine_add_starting_weapon_to_player(mainmenu_unit_index, primary_weapon_index, 1);
	}
}

void __cdecl hf2p_idle()
{
}

void __cdecl hf2p_render()
{
}

void __cdecl hf2p_bot_client_initialize()
{
}

void __cdecl hf2p_dedicated_server_initialize()
{
}

void __cdecl hf2p_client_initialize()
{
}

void __cdecl hf2p_security_initialize()
{
}

void __cdecl hf2p_editor_initialize()
{

}

void __cdecl hf2p_main_initialize()
{
}

void __cdecl hf2p_config_initialize()
{
}

void __cdecl hf2p_bot_client_dispose()
{
}

void __cdecl hf2p_dedicated_server_dispose()
{
}

void __cdecl hf2p_client_dispose()
{
}

void __cdecl hf2p_main_dispose()
{
}

void __cdecl hf2p_bot_client_update()
{
}

void __cdecl hf2p_dedicated_server_update()
{
}

void __cdecl hf2p_client_update()
{
}

void __cdecl hf2p_backend_register(void* backend)
{
}

void __cdecl hf2p_backend_unregister(void* backend)
{
}

void __cdecl hf2p_backend_update()
{
}

s_s3d_player_armor_configuration_loadout& get_armor_loadout()
{
	static s_s3d_player_armor_configuration_loadout loadout{};

	int32 user_index = 0;
	e_controller_index controller_index = _controller0;
	s_player_configuration player_data{};
	uns32 player_voice_settings = 0;

	if (network_session_interface_get_local_user_properties(user_index, &controller_index, &player_data, &player_voice_settings))
	{
		player_data.host.armor.loadouts[player_data.host.armor.loadout_index] = loadout;
		network_session_interface_set_local_user_properties(user_index, controller_index, &player_data, player_voice_settings);
	}

	return loadout;
}

s_s3d_player_weapon_configuration_loadout& get_weapon_loadout()
{
	static s_s3d_player_weapon_configuration_loadout loadout;

	int32 user_index = 0;
	e_controller_index controller_index = _controller0;
	s_player_configuration player_data{};
	uns32 player_voice_settings = 0;

	// $TODO pull this from tags
	//loadout.grenade_index = _grenade_type_firebomb;

	// $TODO pull this from a config file
#if defined(_DEBUG)
	loadout.bungienet_user.set(_bungienet_user_bungie, true);
#else
	loadout.bungienet_user.set(_bungienet_user_seventh_column, true);
#endif // _DEBUG

	if (network_session_interface_get_local_user_properties(user_index, &controller_index, &player_data, &player_voice_settings))
	{
		player_data.host.weapon.loadouts[player_data.host.weapon.loadout_index] = loadout;
		network_session_interface_set_local_user_properties(user_index, controller_index, &player_data, player_voice_settings);
	}

	return loadout;
}

c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& get_armor_regions(e_player_model_choice player_model_choice, bool* cache_file_has_halo3_armors)
{
	static c_static_array<c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>, k_player_model_choice_count> armor_regions;

	if (g_cache_file_globals.tags_loaded)
	{
		s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();

		for (s_multiplayer_customized_model_character& customized_spartan_character : universal_data->customized_spartan_characters)
		{
			const char* armor_region = customized_spartan_character.armor_region.get_string();

			bool ignore_requirements = false;
			c_static_array<c_static_string<64>, 100>* armor_type = NULL;
			switch (string_hash(armor_region))
			{
			case "helmet"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_helmet];
			}
			break;
			case "chest"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_chest];
			}
			break;
			case "shoulders"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_shoulders];
			}
			break;
			case "arms"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_arms];
			}
			break;
			case "legs"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_legs];
			}
			break;
			case "acc"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_acc];
			}
			break;
			case "pelvis"_hash:
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_pelvis];
			}
			break;
			case "rightshoulder"_hash:
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_shoulders];

				if (cache_file_has_halo3_armors)
				{
					*cache_file_has_halo3_armors = true;
				}
			}
			break;
			case "leftshoulder"_hash:
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_arms];

				if (cache_file_has_halo3_armors)
				{
					*cache_file_has_halo3_armors = true;
				}
			}
			break;
			default:
				continue;
			}
			

			for (int32 i = 0; i < customized_spartan_character.customized_selection.count; i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_spartan_character.customized_selection[i];

				// you can only see the arms in first person
				if (ignore_requirements || customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}

		for (s_multiplayer_customized_model_character& customized_elite_character : universal_data->customized_elite_characters)
		{
			const char* armor_region = customized_elite_character.armor_region.get_string();

			c_static_array<c_static_string<64>, 100>* armor_type = NULL;

			switch (string_hash(armor_region))
			{
			case "helmet"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_helmet];
			}
			break;
			case "chest"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_chest];
			}
			break;
			case "shoulders"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_shoulders];
			}
			break;
			case "arms"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_arms];
			}
			break;
			case "legs"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_legs];
			}
			break;
			case "acc"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_acc];
			}
			break;
			case "pelvis"_hash:
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_pelvis];
			}
			break;
			case "upper_body"_hash: // there's an upper body armor type??
			{
				continue;
			}
			break;
			default:
				continue;
			}

			for (int32 i = 0; i < customized_elite_character.customized_selection.count; i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_elite_character.customized_selection[i];

				// you can only see the arms in first person
				if (customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}
	}

	return armor_regions[player_model_choice];
}

bool load_preference(const char* name, const char* value)
{
	// special case
	if (csstricmp(name, "screen_resolution") == 0)
	{
		char width[8]{};
		char height[8]{};
		sscanf_s(value, "%[^x]x%s", width, sizeof(width), height, sizeof(height));

		return global_preference_set(name, atol(width), atol(height));
	}
	else
	{
		if (csstricmp(value, "true") == 0)
		{
			return global_preference_set(name, true);
		}

		if (csstricmp(value, "false") == 0)
		{
			return global_preference_set(name, false);
		}

		if (csstrstr(value, "."))
		{
			return global_preference_set(name, real32(atof(value)));
		}

		return global_preference_set(name, atol(value));
	}

	return false;
}

void load_preferences_from_file_hs(const char* filename)
{
	FILE* file = NULL;
	if (fopen_s(&file, filename, "r") == 0 && file)
	{
		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");
			char name[128]{};
			char value[32]{};

			sscanf_s(buffer, "%[^:]: %s", name, sizeof(name), value, sizeof(value));
			if (*name && *value)
			{
				load_preference(name, value);
			}
		}
		fclose(file);
	}
}

void load_customization_from_file_hs(const char* filename)
{
	bool cache_file_has_halo3_armors = false;
	c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& armor_regions = get_armor_regions(_player_model_choice_spartan, &cache_file_has_halo3_armors);

	FILE* file = NULL;
	if (fopen_s(&file, filename, "r") == 0 && file)
	{
		s_s3d_player_armor_configuration_loadout& armor_loadout = get_armor_loadout();
		s_s3d_player_weapon_configuration_loadout& weapon_loadout = get_weapon_loadout();

		char buffer[200]{};
		while (fgets(buffer, NUMBEROF(buffer), file))
		{
			string_terminate_at_first_delimiter(buffer, "\r\n");

#if defined(ALLOW_LOADING_CONSUMABLES_FROM_FILE)
			{
				int32 consumable_index = NONE;
				char consumable_name[32]{};
				if (sscanf_s(buffer, "consumables[%d]: %s", &consumable_index, consumable_name, sizeof(consumable_name)) && (consumable_index != NONE && *consumable_name))
				{
					weapon_loadout.consumables[consumable_index] = static_cast<int8>(multiplayer_universal_data_get_absolute_equipment_block_index(consumable_name));
					continue;
				}
			}
#endif

			// colors
			{
				char color_type_name[128]{};
				uns32 rgb_value = NONE;
				if (sscanf_s(buffer, "colors[%[^]]]: #%08X", color_type_name, sizeof(color_type_name), &rgb_value) && (*color_type_name && rgb_value != NONE))
				{
					int32 index = NONE;
					switch (string_hash(color_type_name))
					{
					case "primary"_hash:
						index = _color_type_primary;
						break;
					case "secondary"_hash:
						index = _color_type_secondary;
						break;
					case "visor"_hash:
						index = _color_type_visor;
						break;
					case "lights"_hash:
						index = _color_type_lights;
						break;
					case "holo"_hash:
						if (!cache_file_has_halo3_armors)
						{
							index = _color_type_holo;
						}
						break;
					}
					if (VALID_INDEX(index, k_color_type_count))
					{
						armor_loadout.colors[index].value = rgb_value;
					}

					continue;
				}
			}

			// armors
			{
				char armor_region_name[128]{};
				char armor_name[32]{};
				if (sscanf_s(buffer, "armors[%[^]]]: %s", armor_region_name, sizeof(armor_region_name), armor_name, sizeof(armor_name)) && (*armor_region_name && *armor_name))
				{
					int32 armor_region_count = cache_file_has_halo3_armors ? _armor_type_arms + 1 : armor_loadout.armors.get_count();
					for (int32 armor_region_index = 0; armor_region_index < armor_region_count; armor_region_index++)
					{
						const char* armor_region = NULL;
						switch (armor_region_index)
						{
						case _armor_type_helmet:
							armor_region = "helmet";
							break;
						case _armor_type_chest:
							armor_region = "chest";
							break;
						case _armor_type_shoulders:
							armor_region = cache_file_has_halo3_armors ? "rightshoulder" : "shoulders";
							break;
						case _armor_type_arms:
							armor_region = cache_file_has_halo3_armors ? "leftshoulder" : "arms";
							break;
						case _armor_type_legs:
							armor_region = "legs";
							break;
						case _armor_type_acc:
							armor_region = "acc";
							break;
						case _armor_type_pelvis:
							armor_region = "pelvis";
							break;
						}

						if (armor_region && csstricmp(armor_region, armor_region_name) == 0)
						{
							armor_loadout.armors[armor_region_index] = static_cast<uns8>(multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(armor_region, armor_name));
						}
					}
				}
			}
		}

		fclose(file);
	}
	else
	{
		FILE* customization_info_file = NULL;
		if (fopen_s(&customization_info_file, "customization_info.txt", "w") == 0 && customization_info_file)
		{
			fprintf_s(customization_info_file, "This file serves as a reference to what a customization file contains\n\n");

			fprintf_s(customization_info_file, "Example usage:\n\n");

#if defined(ALLOW_LOADING_CONSUMABLES_FROM_FILE)
			fprintf_s(customization_info_file, "consumables[<consumable_index(0 - 3)>]: <consumable_name>\n");
			fprintf_s(customization_info_file, "consumables[0]: empty\n");
			fprintf_s(customization_info_file, "consumables[1]: empty\n");
			fprintf_s(customization_info_file, "consumables[2]: empty\n");
			fprintf_s(customization_info_file, "consumables[3]: empty\n\n");
#endif

			fprintf_s(customization_info_file, "colors[<color_type_name>]: #RGB\n");
			fprintf_s(customization_info_file, "colors[primary]: #FF0000\n");
			fprintf_s(customization_info_file, "colors[secondary]: #0000FF\n");
			fprintf_s(customization_info_file, "colors[visor]: #0000FF\n");
			fprintf_s(customization_info_file, "colors[lights]: #00F00F\n");

			if (!cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "colors[holo]: #0F0F00\n\n");
			}

			if (cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "armors[<armor_region_name>]: <armor_name>\n");
				fprintf_s(customization_info_file, "armors[helmet]: base\n");
				fprintf_s(customization_info_file, "armors[chest]: base\n");
				fprintf_s(customization_info_file, "armors[rightshoulder]: base\n");
				fprintf_s(customization_info_file, "armors[leftshoulder]: base\n");
			}
			else
			{
				fprintf_s(customization_info_file, "armors[<armor_region_name>]: <armor_name>\n");
				fprintf_s(customization_info_file, "armors[helmet]: base\n");
				fprintf_s(customization_info_file, "armors[chest]: base\n");
				fprintf_s(customization_info_file, "armors[shoulders]: base\n");
				fprintf_s(customization_info_file, "armors[arms]: base\n");
				fprintf_s(customization_info_file, "armors[legs]: base\n");
				fprintf_s(customization_info_file, "armors[acc]: flashlight\n");
				fprintf_s(customization_info_file, "armors[pelvis]: base\n\n");
			}

			if (!cache_file_has_halo3_armors)
			{
				fprintf_s(customization_info_file, "consumable:\n");
				fprintf_s(customization_info_file, "\tempty\n");
				fprintf_s(customization_info_file, "\tjammer\n");
				fprintf_s(customization_info_file, "\tpowerdrain\n");
				fprintf_s(customization_info_file, "\tinvisibility\n");
				fprintf_s(customization_info_file, "\tinvisibility_vehicle\n");
				fprintf_s(customization_info_file, "\tbubbleshield\n");
				fprintf_s(customization_info_file, "\tsuperflare\n");
				fprintf_s(customization_info_file, "\tregenerator\n");
				fprintf_s(customization_info_file, "\ttripmine\n");
				fprintf_s(customization_info_file, "\tauto_turret\n");
				fprintf_s(customization_info_file, "\tdeployable_cover\n");
				fprintf_s(customization_info_file, "\tforced_reload\n");
				fprintf_s(customization_info_file, "\tconcussive_blast\n");
				fprintf_s(customization_info_file, "\ttank_mode\n");
				fprintf_s(customization_info_file, "\tmag_pulse\n");
				fprintf_s(customization_info_file, "\thologram\n");
				fprintf_s(customization_info_file, "\treactive_armor\n");
				fprintf_s(customization_info_file, "\tbomb_run\n");
				fprintf_s(customization_info_file, "\tarmor_lock\n");
				fprintf_s(customization_info_file, "\tadrenaline\n");
				fprintf_s(customization_info_file, "\tlightning_strike\n");
				fprintf_s(customization_info_file, "\tscrambler\n");
				fprintf_s(customization_info_file, "\tweapon_jammer\n");
				fprintf_s(customization_info_file, "\tammo_pack\n");
				fprintf_s(customization_info_file, "\tconsumable_vision\n");
				fprintf_s(customization_info_file, "\tbubbleshield_tutorial\n");
				fprintf_s(customization_info_file, "\tconsumable_vision_tutorial\n\n");
			}

			int32 armor_region_count = cache_file_has_halo3_armors ? _armor_type_arms + 1 : k_armor_type_count;
			for (int32 armor_region_index = 0; armor_region_index < armor_region_count; armor_region_index++)
			{
				c_static_array<c_static_string<64>, 100>& armor_types = armor_regions[armor_region_index];

				const char* armor_region = NULL;
				switch (armor_region_index)
				{
				case _armor_type_helmet:
					armor_region = "helmet";
					break;
				case _armor_type_chest:
					armor_region = "chest";
					break;
				case _armor_type_shoulders:
					armor_region = cache_file_has_halo3_armors ? "rightshoulder" : "shoulders";
					break;
				case _armor_type_arms:
					armor_region = cache_file_has_halo3_armors ? "leftshoulder" : "arms";
					break;
				case _armor_type_legs:
					armor_region = "legs";
					break;
				case _armor_type_acc:
					armor_region = "acc";
					break;
				case _armor_type_pelvis:
					armor_region = "pelvis";
					break;
				}

				if (armor_region)
				{
					fprintf_s(customization_info_file, "%s:\n", armor_region);
					for (int32 armor_type_index = 0; armor_type_index < armor_types.get_count(); armor_type_index++)
					{
						const char* value = armor_types[armor_type_index].get_string();
						if (*value)
						{
							fprintf_s(customization_info_file, "\t%s\n", value);
						}
					}
					fprintf_s(customization_info_file, "\n");
				}
			}

			fclose(customization_info_file);
		}
	}
}

