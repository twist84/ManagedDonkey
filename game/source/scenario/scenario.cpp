#include "scenario/scenario.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "hf2p/hf2p.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "tag_files/tag_groups.hpp"


REFERENCE_DECLARE(0x0189CCF8, long, global_scenario_index);
REFERENCE_DECLARE(0x0189CCFC, long, global_scenario_game_globals_index);
REFERENCE_DECLARE(0x022AAEB4, s_scenario*, global_scenario);
REFERENCE_DECLARE(0x022AAEB8, s_game_globals*, global_game_globals);
REFERENCE_DECLARE(0x022AAEBC, dword, g_active_structure_bsp_mask);
REFERENCE_DECLARE(0x022AAEC0, dword, g_touched_structure_bsp_mask);
REFERENCE_DECLARE(0x022AAEC4, dword, g_active_designer_zone_mask);
REFERENCE_DECLARE(0x022AAEC8, dword, g_active_cinematic_zone_mask);
REFERENCE_DECLARE(0x022AAECC, dword, g_touched_cinematic_zone_mask);

HOOK_DECLARE(0x004EA5E0, scenario_load);

s_scenario* global_scenario_get()
{
	// halo 3
	ASSERT(global_scenario);
	return global_scenario;

	// halo online
	//return tag_get<s_scenario>('scnr', global_scenario_index);
}

s_scenario* global_scenario_try_and_get()
{
	if (global_scenario)
		return global_scenario_get();

	return nullptr;
}

s_game_globals* scenario_get_game_globals()
{
	ASSERT(global_game_globals);
	return global_game_globals;
}

s_game_globals* scenario_try_and_get_game_globals()
{
	if (global_game_globals)
		return scenario_get_game_globals();

	return nullptr;
}

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path)
{
	//return INVOKE(0x004EB820, scenario_tags_match, campaign_id, map_id, scenario_path);

	ASSERT(scenario_path != 0);

	s_scenario* scenario = global_scenario_get();
	if (levels_map_id_is_fake(map_id))
		return true;

	return (scenario->campaign_id == campaign_id || campaign_id == -1) && (scenario->map_id == map_id || map_id == -1);
}

void __cdecl scenario_invalidate()
{
	return INVOKE(0x004EA3E0, scenario_invalidate);
}

void on_scenario_loaded();

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
	//bool result = false;
	//HOOK_INVOKE(result =, scenario_load, campaign_id, map_id, scenario_path);
	//return result;

	// saber function, crashes in `hf2p_game_client_cache_release.exe!sub_A28EC0` if not called
	hf2p_scenario_load();

	if (scenario_tags_load(scenario_path))
	{
		if (scenario_tags_match(campaign_id, map_id, scenario_path))
		{
			scenario_tags_fixup();
			scenario_tags_load_finished();
			hf2p_scenario_tags_load_finished();

			//on_scenario_loaded();
			return true;
		}
		else
		{
			scenario_tags_unload();
			scenario_invalidate();

			return false;
		}
	}

	tag_load_missing_tags_report();
	return false;
}

#define SCENARIO_PRINT_ZONE_SETS()\
if (scenario->zone_sets.count())\
{\
	c_console::write_line("    zone sets: %d", scenario->zone_sets.count());\
	for (auto zone_set : scenario->zone_sets)\
	{\
		char const* name = zone_set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_LIGHTING_ZONE_SETS()\
if (scenario->lighting_zone_sets.count())\
{\
	c_console::write_line("    lighting zone sets: %d", scenario->lighting_zone_sets.count());\
	for (auto lighting_zone_set : scenario->lighting_zone_sets)\
	{\
		char const* name = lighting_zone_set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_CAMPAIN_PLAYERS()\
if (scenario->campaign_players.count())\
{\
	c_console::write_line("    campaign players: %d", scenario->campaign_players.count());\
	for (auto campaign_player : scenario->campaign_players)\
	{\
		char const* name = campaign_player.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_OBJECT_NAMES()\
if (scenario->object_names.count())\
{\
	c_console::write_line("    object names: %d", scenario->object_names.count());\
	for (auto object_name : scenario->object_names)\
	{\
		char const* name = object_name.name;\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_MAP_VARIANT_PALETTES(NAME)\
if (scenario->map_variant_##NAME##_palette.count())\
{\
	c_console::write_line("    %s palettes: %d", #NAME, scenario->map_variant_##NAME##_palette.count());\
	for (auto palette : scenario->map_variant_##NAME##_palette)\
	{\
		char const* display_name = palette.display_name.get_string();\
		if (display_name && *display_name)\
			c_console::write_line("        display name: %s", display_name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_SOFT_CEILINGS()\
if (scenario->soft_ceilings.count())\
{\
	c_console::write_line("    soft ceilings: %d", scenario->soft_ceilings.count());\
	for (auto soft_ceiling : scenario->soft_ceilings)\
	{\
		char const* name = soft_ceiling.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_PLAYER_STARTING_PROFILES()\
if (scenario->player_starting_profile.count())\
{\
	c_console::write_line("    player starting profiles: %d", scenario->player_starting_profile.count());\
	for (auto profile : scenario->player_starting_profile)\
	{\
		char const* name = profile.name;\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_TRIGGER_VOLUMES()\
if (scenario->trigger_volumes.count())\
{\
	c_console::write_line("    trigger volumes: %d", scenario->trigger_volumes.count());\
	for (auto trigger_volume : scenario->trigger_volumes)\
	{\
		char const* name = trigger_volume.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_SCRIPTS()\
if (scenario->scripts.count())\
{\
	c_console::write_line("    scripts: %d", scenario->scripts.count());\
	for (auto script : scenario->scripts)\
	{\
		char const* script_name = script.name;\
		if (script_name && *script_name)\
			c_console::write_line("        name: %s", script_name);\
		if (script.parameters.count())\
		{\
			c_console::write_line("            parameters: %d", script.parameters.count());\
			for (auto parameter : script.parameters)\
			{\
				char const* parameter_name = parameter.name;\
				if (parameter_name && *parameter_name)\
					c_console::write_line("                name: %s", parameter_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

#define SCENARIO_PRINT_GLOBALS()\
if (scenario->globals.count())\
{\
	c_console::write_line("    globals: %d", scenario->globals.count());\
	for (auto global : scenario->globals)\
	{\
		char const* name = global.name;\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
	}\
	c_console::write_line("");\
}

#define GLOBALS_PRINT_PLAYER_REPRESENTATION()\
if (game_globals->player_representation.count())\
{\
	c_console::write_line("    player representation: %d", game_globals->player_representation.count());\
	for (auto player_representation : game_globals->player_representation)\
	{\
		char const* name = player_representation.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
		char const* third_person_variant = player_representation.third_person_variant.get_string();\
		if (third_person_variant && *third_person_variant)\
			c_console::write_line("        third person variant: %s", third_person_variant);\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_EQUIPMENT()\
if (universal_data->equipment.count())\
{\
	c_console::write_line("    equipment: %d", universal_data->equipment.count());\
	for (auto equipment : universal_data->equipment)\
	{\
		char const* name = equipment.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name); \
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_SELECTIONS(NAME)\
if (universal_data->NAME##_selections.count())\
{\
	c_console::write_line("    %s selections: %d", #NAME, universal_data->NAME##_selections.count());\
	for (auto selection : universal_data->NAME##_selections)\
	{\
		char const* name = selection.name.get_string();\
		if (name && *name)\
		{\
			c_console::write_line("        name: %s, 0x%08X", name, selection.NAME##_tag.index);\
		}\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_SETS(NAME)\
if (universal_data->NAME##_sets.count())\
{\
	c_console::write_line("    %s sets: %d", #NAME, universal_data->NAME##_sets.count());\
	for (auto set : universal_data->NAME##_sets)\
	{\
		char const* name = set.name.get_string();\
		if (name && *name)\
			c_console::write_line("        name: %s", name);\
		if (set.remap_table.count())\
		{\
			c_console::write_line("            remap table: %d", set.remap_table.count());\
			for (auto remap_entry : set.remap_table)\
			{\
				char const* placed_object_name = remap_entry.placed_object_name.get_string();\
				char const* remapped_object_name = remap_entry.remapped_object_name.get_string();\
				if (placed_object_name && *placed_object_name)\
					c_console::write_line("                  placed object name: %s", placed_object_name);\
				if (remapped_object_name && *remapped_object_name)\
					c_console::write_line("                remapped object name: %s", remapped_object_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

#define UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(NAME)\
if (universal_data->customized_##NAME##_characters.count())\
{\
	c_console::write_line("    customized %s characters: %d", #NAME, universal_data->customized_##NAME##_characters.count());\
	for (auto customized_character : universal_data->customized_##NAME##_characters)\
	{\
		char const* armor_region = customized_character.armor_region.get_string();\
		if (armor_region && *armor_region)\
			c_console::write_line("        armor region: %s", armor_region);\
		char const* biped_region = customized_character.biped_region.get_string();\
		if (biped_region && *biped_region)\
			c_console::write_line("        biped region: %s", biped_region);\
		if (customized_character.customized_areas.count())\
		{\
			c_console::write_line("            customized areas: %d", customized_character.customized_areas.count());\
			for (auto customized_area : customized_character.customized_areas)\
			{\
				char const* selection_name = customized_area.selection_name.get_string();\
				if (selection_name && *selection_name)\
					c_console::write_line("                selection name: %s", selection_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

void on_scenario_loaded()
{
	s_scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
		return;

	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	if (!game_globals)
		return;

	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return;

	s_multiplayer_runtime_globals_definition* runtime_data = scenario_multiplayer_globals_try_and_get_runtime_data();
	if (!runtime_data)
		return;

	c_console::write_line("scenario:");
	SCENARIO_PRINT_ZONE_SETS();
	SCENARIO_PRINT_LIGHTING_ZONE_SETS();
	SCENARIO_PRINT_CAMPAIN_PLAYERS();
	SCENARIO_PRINT_OBJECT_NAMES();
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(vehicle);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(weapon);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(equipment);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(scenery);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(teleporters);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(goals);
	SCENARIO_PRINT_MAP_VARIANT_PALETTES(spawners);
	SCENARIO_PRINT_SOFT_CEILINGS();
	SCENARIO_PRINT_PLAYER_STARTING_PROFILES();
	SCENARIO_PRINT_TRIGGER_VOLUMES();
	SCENARIO_PRINT_SCRIPTS();
	SCENARIO_PRINT_GLOBALS();

	c_console::write_line("globals:");
	GLOBALS_PRINT_PLAYER_REPRESENTATION();

	c_console::write_line("multiplayer globals universal data:");
	UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(spartan);
	UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS(elite);
	UNIVERSAL_DATA_PRINT_EQUIPMENT();
	UNIVERSAL_DATA_PRINT_SELECTIONS(weapon);
	UNIVERSAL_DATA_PRINT_SELECTIONS(vehicle);
	UNIVERSAL_DATA_PRINT_SELECTIONS(grenade);
	UNIVERSAL_DATA_PRINT_SETS(weapon);
	UNIVERSAL_DATA_PRINT_SETS(vehicle);

	printf("");
}

#undef UNIVERSAL_DATA_PRINT_CUSTOMIZED_CHARACTERS
#undef UNIVERSAL_DATA_PRINT_SETS
#undef UNIVERSAL_DATA_PRINT_SELECTIONS
#undef UNIVERSAL_DATA_PRINT_EQUIPMENT
#undef GLOBALS_PRINT_PLAYER_REPRESENTATION
#undef SCENARIO_PRINT_GLOBALS
#undef SCENARIO_PRINT_SCRIPTS
#undef SCENARIO_PRINT_TRIGGER_VOLUMES
#undef SCENARIO_PRINT_PLAYER_STARTING_PROFILES
#undef SCENARIO_PRINT_SOFT_CEILINGS
#undef SCENARIO_PRINT_MAP_VARIANT_PALETTES
#undef SCENARIO_PRINT_OBJECT_NAMES
#undef SCENARIO_PRINT_CAMPAIN_PLAYERS
#undef SCENARIO_PRINT_LIGHTING_ZONE_SETS
#undef SCENARIO_PRINT_ZONE_SETS

