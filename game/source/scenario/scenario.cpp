#include "scenario/scenario.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "hf2p/hf2p.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "tag_files/tag_groups.hpp"

#include <assert.h>

HOOK_DECLARE(0x004EA5E0, scenario_load);

REFERENCE_DECLARE(0x0189CCF8, long, global_scenario_index);
REFERENCE_DECLARE(0x0189CCFC, long, global_scenario_game_globals_index);
REFERENCE_DECLARE(0x022AAEB4, s_scenario*, global_scenario);
REFERENCE_DECLARE(0x022AAEB8, s_game_globals*, global_game_globals);

s_scenario* global_scenario_get()
{
	FUNCTION_BEGIN(true);

	// halo 3
	assert(global_scenario);
	return global_scenario;

	// halo online
	//return tag_get<s_scenario>('scnr', global_scenario_index);
}

s_scenario* global_scenario_try_and_get()
{
	FUNCTION_BEGIN(true);

	if (global_scenario)
		return global_scenario_get();

	return nullptr;
}

s_game_globals* scenario_get_game_globals()
{
	FUNCTION_BEGIN(true);

	assert(global_game_globals);
	return global_game_globals;
}

s_game_globals* scenario_try_and_get_game_globals()
{
	FUNCTION_BEGIN(true);

	if (global_game_globals)
		return scenario_get_game_globals();

	return nullptr;
}

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path)
{
	FUNCTION_BEGIN(true);

	//return INVOKE(0x004EB820, scenario_tags_match, campaign_id, map_id, scenario_path);

	assert(scenario_path != 0);

	s_scenario* scenario = global_scenario_get();
	if (levels_map_id_is_fake(map_id))
		return true;

	return (scenario->campaign_id == campaign_id || campaign_id == -1) && (scenario->map_id == map_id || map_id == -1);
}

void __cdecl scenario_invalidate()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x004EA3E0, scenario_invalidate);
}

void on_scenario_loaded();

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
	FUNCTION_BEGIN(true);

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

			on_scenario_loaded();
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

#define PRINT_ZONE_SETS()\
if (scenario->zone_sets.count)\
{\
	c_console::write_line("zone sets: %d", scenario->zone_sets.count);\
	for (long i = 0; i < scenario->zone_sets.count; i++)\
	{\
		auto zone_set = scenario->zone_sets.elements + i;\
		assert(zone_set);\
		char const* name = zone_set->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_LIGHTING_ZONE_SETS()\
if (scenario->lighting_zone_sets.count)\
{\
	c_console::write_line("lighting zone sets: %d", scenario->lighting_zone_sets.count);\
	for (long i = 0; i < scenario->lighting_zone_sets.count; i++)\
	{\
		auto lighting_zone_set = scenario->lighting_zone_sets.elements + i;\
		assert(lighting_zone_set);\
		char const* name = lighting_zone_set->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_CAMPAIN_PLAYERS()\
if (scenario->campaign_players.count)\
{\
	c_console::write_line("campaign players: %d", scenario->campaign_players.count);\
	for (long i = 0; i < scenario->campaign_players.count; i++)\
	{\
		auto campaign_player = scenario->campaign_players.elements + i;\
		assert(campaign_player);\
		char const* name = campaign_player->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_OBJECT_NAMES()\
if (scenario->object_names.count)\
{\
	c_console::write_line("object names: %d", scenario->object_names.count);\
	for (long i = 0; i < scenario->object_names.count; i++)\
	{\
		auto object_name = scenario->object_names.elements + i;\
		assert(object_name);\
		char const* name = object_name->name;\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_MAP_VARIANT_PALETTES(NAME)\
if (scenario->map_variant_##NAME##_palette.count)\
{\
	c_console::write_line("%s palettes: %d", #NAME, scenario->map_variant_##NAME##_palette.count);\
	for (long i = 0; i < scenario->map_variant_##NAME##_palette.count; i++)\
	{\
		auto palette = scenario->map_variant_##NAME##_palette.elements + i;\
		assert(palette);\
		char const* display_name = palette->display_name.get_string();\
		if (display_name && *display_name)\
			c_console::write_line("    %s", display_name);\
	}\
	c_console::write_line("");\
}

#define PRINT_SOFT_CEILINGS()\
if (scenario->soft_ceilings.count)\
{\
	c_console::write_line("soft ceilings: %d", scenario->soft_ceilings.count);\
	for (long i = 0; i < scenario->soft_ceilings.count; i++)\
	{\
		auto soft_ceiling = scenario->soft_ceilings.elements + i;\
		assert(soft_ceiling);\
		char const* name = soft_ceiling->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_PLAYER_STARTING_PROFILES()\
if (scenario->player_starting_profile.count)\
{\
	c_console::write_line("player starting profiles: %d", scenario->player_starting_profile.count);\
	for (long i = 0; i < scenario->player_starting_profile.count; i++)\
	{\
		auto profile = scenario->player_starting_profile.elements + i;\
		assert(profile);\
		char const* name = profile->name;\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_TRIGGER_VOLUMES()\
if (scenario->trigger_volumes.count)\
{\
	c_console::write_line("trigger volumes: %d", scenario->trigger_volumes.count);\
	for (long i = 0; i < scenario->trigger_volumes.count; i++)\
	{\
		auto trigger_volume = scenario->trigger_volumes.elements + i;\
		assert(trigger_volume);\
		char const* name = trigger_volume->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_SCRIPTS()\
if (scenario->scripts.count)\
{\
	c_console::write_line("scripts: %d", scenario->scripts.count);\
	for (long i = 0; i < scenario->scripts.count; i++)\
	{\
		auto script = scenario->scripts.elements + i;\
		assert(script);\
		char const* script_name = script->name;\
		if (script_name && *script_name)\
			c_console::write_line("    %s", script_name);\
		if (script->parameters.count)\
		{\
			c_console::write_line("        parameters: %d", script->parameters.count);\
			for (long i = 0; i < script->parameters.count; i++)\
			{\
				auto parameter = script->parameters.elements + i;\
				assert(parameter);\
				char const* parameter_name = parameter->name;\
				if (parameter_name && *parameter_name)\
					c_console::write_line("            %s", parameter_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

#define PRINT_GLOBALS()\
if (scenario->globals.count)\
{\
	c_console::write_line("globals: %d", scenario->globals.count);\
	for (long i = 0; i < scenario->globals.count; i++)\
	{\
		auto global = scenario->globals.elements + i;\
		assert(global);\
		char const* name = global->name;\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_EQUIPMENT()\
if (universal_data->equipment.count)\
{\
	c_console::write_line("equipment: %d", universal_data->equipment.count);\
	for (long i = 0; i < universal_data->equipment.count; i++)\
	{\
		auto equipment = universal_data->equipment.elements + i;\
			assert(equipment);\
		char const* name = equipment->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name); \
	}\
	c_console::write_line("");\
}

#define PRINT_SELECTIONS(NAME)\
if (universal_data->NAME##_selections.count)\
{\
	c_console::write_line("%s selections: %d", #NAME, universal_data->NAME##_selections.count);\
	for (long i = 0; i < universal_data->NAME##_selections.count; i++)\
	{\
		auto selection = universal_data->NAME##_selections.elements + i;\
		assert(selection);\
		char const* name = selection->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
	}\
	c_console::write_line("");\
}

#define PRINT_SETS(NAME)\
if (universal_data->NAME##_sets.count)\
{\
	c_console::write_line("%s sets: %d", #NAME, universal_data->NAME##_sets.count);\
	for (long i = 0; i < universal_data->NAME##_sets.count; i++)\
	{\
		auto set = universal_data->NAME##_sets.elements + i;\
		assert(set);\
		char const* name = set->name.get_string();\
		if (name && *name)\
			c_console::write_line("    %s", name);\
		if (set->remap_table.count)\
		{\
			c_console::write_line("        remap_table: %d", set->remap_table.count);\
			for (long i = 0; i < set->remap_table.count; i++)\
			{\
				auto remap_entry = set->remap_table.elements + i;\
				assert(remap_entry);\
				char const* placed_object_name = remap_entry->placed_object_name.get_string();\
				char const* remapped_object_name = remap_entry->remapped_object_name.get_string();\
				if (placed_object_name && *placed_object_name)\
					c_console::write_line("            %s", placed_object_name);\
				if (remapped_object_name && *remapped_object_name)\
					c_console::write_line("            %s", remapped_object_name);\
			}\
			c_console::write_line("");\
		}\
	}\
	c_console::write_line("");\
}

void on_scenario_loaded()
{
	s_scenario* scenario = global_scenario_try_and_get();

	PRINT_ZONE_SETS();
	PRINT_LIGHTING_ZONE_SETS();
	PRINT_CAMPAIN_PLAYERS();
	PRINT_OBJECT_NAMES();
	PRINT_MAP_VARIANT_PALETTES(vehicle);
	PRINT_MAP_VARIANT_PALETTES(weapon);
	PRINT_MAP_VARIANT_PALETTES(equipment);
	PRINT_MAP_VARIANT_PALETTES(scenery);
	PRINT_MAP_VARIANT_PALETTES(teleporters);
	PRINT_MAP_VARIANT_PALETTES(goals);
	PRINT_MAP_VARIANT_PALETTES(spawners);
	PRINT_SOFT_CEILINGS();
	PRINT_PLAYER_STARTING_PROFILES();
	PRINT_TRIGGER_VOLUMES();
	PRINT_SCRIPTS();
	PRINT_GLOBALS();

	s_game_globals* game_globals = scenario_try_and_get_game_globals();
	s_multiplayer_globals_definition* multiplayer_globals = static_cast<s_multiplayer_globals_definition*>(tag_get(game_globals->multiplayer_globals.group_tag, game_globals->multiplayer_globals.index));

	s_multiplayer_universal_globals_definition* universal_data = multiplayer_globals->universal.elements;
	if (!universal_data)
		return;

	PRINT_EQUIPMENT();
	PRINT_SELECTIONS(weapon);
	PRINT_SELECTIONS(vehicle);
	PRINT_SELECTIONS(grenade);
	PRINT_SETS(weapon);
	PRINT_SETS(vehicle);

	printf("");
}

#undef PRINT_SETS
#undef PRINT_SELECTIONS
#undef PRINT_EQUIPMENT
#undef PRINT_GLOBALS
#undef PRINT_SCRIPTS
#undef PRINT_TRIGGER_VOLUMES
#undef PRINT_PLAYER_STARTING_PROFILES
#undef PRINT_SOFT_CEILINGS
#undef PRINT_MAP_VARIANT_PALETTES
#undef PRINT_OBJECT_NAMES
#undef PRINT_CAMPAIN_PLAYERS
#undef PRINT_LIGHTING_ZONE_SETS
#undef PRINT_ZONE_SETS