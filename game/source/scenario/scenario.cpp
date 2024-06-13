#include "scenario/scenario.hpp"

#include "cache/cache_file_io_failure.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "hf2p/hf2p.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "profiler/profiler_stopwatch.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "structures/structure_bsp_definitions.hpp"
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
REFERENCE_DECLARE(0x022AAED1, bool, byte_22AAED1);

c_stop_watch scenario_load_resources_blocking_watch(true);

s_scenario* global_scenario_get()
{
	// halo 3
	ASSERT(global_scenario);
	return global_scenario;

	// halo online
	//return static_cast<s_scenario*>(tag_get(SCENARIO_TAG, global_scenario_index));
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

dword global_structure_bsp_active_mask_get()
{
	return g_active_structure_bsp_mask;
}

long __cdecl global_structure_bsp_first_active_index_get()
{
	return INVOKE(0x004E96A0, global_structure_bsp_first_active_index_get);
}

structure_bsp* __cdecl global_structure_bsp_get(long structure_bsp_index)
{
	return INVOKE(0x004E96D0, global_structure_bsp_get, structure_bsp_index);
}

bool __cdecl global_structure_bsp_is_active(long structure_bsp_index)
{
	return INVOKE(0x004E9700, global_structure_bsp_is_active, structure_bsp_index);
}

long __cdecl global_structure_bsp_next_active_index_get(long structure_bsp_index)
{
	return INVOKE(0x004E9730, global_structure_bsp_next_active_index_get, structure_bsp_index);
}

s_structure_design* global_structure_design_get(long structure_bsp_index)
{
	return INVOKE(0x004E97D0, global_structure_design_get, structure_bsp_index);
}

bool __cdecl scenario_activate_initial_designer_zones(long zoneset_index)
{
	return INVOKE(0x004E9950, scenario_activate_initial_designer_zones, zoneset_index);
}

bool __cdecl scenario_activate_initial_zone_set(long zoneset_index)
{
	return INVOKE(0x004E9990, scenario_activate_initial_zone_set, zoneset_index);
}

s_cluster_reference __cdecl scenario_cluster_reference_from_point(real_point3d const* point)
{
	return INVOKE(0x004E9BD0, scenario_cluster_reference_from_point, point);
}

void __cdecl scenario_get_global_zone_state(s_scenario_zone_state* global_zone_state)
{
	INVOKE(0x004EA1F0, scenario_get_global_zone_state, global_zone_state);
}

void __cdecl scenario_invalidate()
{
	return INVOKE(0x004EA3E0, scenario_invalidate);
}

bool __cdecl scenario_language_pack_load()
{
	return INVOKE(0x004EA440, scenario_language_pack_load);
}

void __cdecl scenario_language_pack_unload()
{
	INVOKE(0x004EA4B0, scenario_language_pack_unload);
}

void on_scenario_loaded();

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
	//return INVOKE(0x004EA5E0, scenario_load, campaign_id, map_id, scenario_path);

	//bool result = false;
	//HOOK_INVOKE(result =, scenario_load, campaign_id, map_id, scenario_path);
	//return result;

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

// halo online only loads pending resources
// ~~we add back the if statement for pending or required~~
// ~~if we ever crash from this decision replace the if statment~~
// ~~with just a call to `cache_file_tag_resources_load_pending_resources_blocking`~~
bool __cdecl scenario_load_resources_blocking(bool pending)
{
	//return INVOKE(0x004EA730, scenario_load_resources_blocking, pending);

	scenario_load_resources_blocking_watch.start();

	// some kind of recursion lock that is never checked in release?
	byte_22AAED1 = true;
	
	bool succeeded = false;
	long failure_count = 0;
	while (!succeeded && failure_count < 3)
	{
		c_simple_io_result io_result;
		//if (pending)
			cache_file_tag_resources_load_pending_resources_blocking(&io_result);
		//else
		//	cache_file_tag_resources_load_required_resources_blocking(&io_result);
	
		if (io_result.check_success())
		{
			succeeded = true;
		}
		else if (++failure_count >= 3)
		{
			io_result.handle_failure();
		}
	}
	
	byte_22AAED1 = false;

	if (__int64 blocking_cycles = scenario_load_resources_blocking_watch.stop())
		status_printf("scenario_load_resources_blocking time: %.2f ms", 1000.0f * c_stop_watch::cycles_to_seconds(blocking_cycles));
	
	return succeeded;
}

bool __cdecl scenario_preload_initial_zone_set(short zone_set_index)
{
	//return INVOKE(0x004EB260, scenario_preload_initial_zone_set, zone_set_index);

	s_scenario* scenario = global_scenario_get();
	if (VALID_INDEX(zone_set_index, scenario->zone_sets.count()))
	{
		s_scenario_zone_state zone_state{};
		scenario_get_global_zone_state(&zone_state);

		s_scenario_zone_set& zone_set = scenario->zone_sets[zone_set_index];
		zone_state.pending_bsp_zone_mask |= zone_set.bsp_zone_flags;
		zone_state.pending_cinematic_zone_mask |= zone_set.cinematic_zones;
		zone_state.pending_designer_zone_mask |= zone_set.required_designer_zones;

		string_id zone_set_name = _string_id_empty_string;
		if (global_scenario_index_get() != NONE)
		{
			s_scenario* global_scenario = (s_scenario*)tag_get(SCENARIO_TAG, global_scenario_index_get());
			if (VALID_INDEX(scenario_zone_set_index_get(), global_scenario->zone_sets.count()))
				zone_set_name = global_scenario->zone_sets[scenario_zone_set_index_get()].name.get_value();
		}

		cache_file_tag_resources_set_zone_state(global_scenario_index_get(), zone_set_name, &zone_state);
		return scenario_load_resources_blocking(true);
	}

	return true;
}

bool __cdecl scenario_switch_zone_set(long zoneset_index)
{
	return INVOKE(0x004EB620, scenario_switch_zone_set, zoneset_index);
}

long __cdecl scenario_zone_set_index_get()
{
	return INVOKE(0x004EBA20, scenario_zone_set_index_get);
}

char const* scenario_tag_get_structure_bsp_name(long scenario_index, long structure_bsp_index)
{
	scenario_structure_bsp_reference& structure_bsp_reference = static_cast<s_scenario*>(tag_get('scnr', scenario_index))->structure_bsps[structure_bsp_index];

	char const* structure_bsp_name = structure_bsp_reference.structure_bsp.get_name();
	if (structure_bsp_name)
	{
		for (char const* i = csstrstr(structure_bsp_name, "\\"); i; i = csstrstr(i + 1, "\\"))
			structure_bsp_name = i + 1;
	}
	return structure_bsp_name;
}

long global_scenario_index_get()
{
	return global_scenario_index;
}

char const* scenario_get_structure_bsp_name(long structure_bsp_index)
{
	return scenario_tag_get_structure_bsp_name(global_scenario_index_get(), structure_bsp_index);
}

char const* scenario_get_structure_bsp_string_from_mask(dword mask, char* structure_bsp_string, dword structure_bsp_string_size)
{
	csnzprintf(structure_bsp_string, structure_bsp_string_size, "");

	bool first_structure_bsp = true;
	for (long i = 0; i < global_scenario_get()->structure_bsps.count(); i++)
	{
		if (TEST_BIT(mask, i))
			continue;

		if (char const* structure_bsp_name = scenario_get_structure_bsp_name(i))
		{
			if (!first_structure_bsp)
				csstrnzcat(structure_bsp_string, ", ", structure_bsp_string_size);
			csstrnzcat(structure_bsp_string, structure_bsp_name, structure_bsp_string_size);
			first_structure_bsp = false;
		}
	}

	return structure_bsp_string;
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

void __cdecl scenario_unload()
{
	INVOKE(0x004EB950, scenario_unload);
}

structure_bsp const* __cdecl scenario_structure_bsp_get(s_scenario const* scenario, long structure_bsp_index)
{
	return INVOKE(0x00766280, scenario_structure_bsp_get, scenario, structure_bsp_index);
}

short scenario_object_name_index_from_string(s_scenario* scenario, char const* name)
{
	for (short object_name_index = 0; object_name_index < static_cast<short>(global_scenario_get()->object_names.count()); object_name_index++)
	{
		scenario_object_name& object_name = global_scenario_get()->object_names[object_name_index];
		if (object_name.name.is_equal(name))
			return object_name_index;
	}

	return NONE;
}

long scenario_get_zone_set_index_by_name(s_scenario const* scenario, char const* name, bool strip_path)
{
	for (long zone_set_index = 0; zone_set_index < scenario->zone_sets.count(); zone_set_index++)
	{
		s_scenario_zone_set& zone_set = scenario->zone_sets[zone_set_index];

		char const* zone_set_name = zone_set.name.get_string();
		if (strip_path)
			zone_set_name = tag_name_strip_path(zone_set_name);

		if (csstrcmp(zone_set_name, name) == 0)
			return zone_set_index;
	}

	return NONE;
}

long scenario_get_designer_zone_index_by_name(s_scenario const* scenario, char const* name)
{
	string_id retrieved_string_id = string_id_retrieve(name);
	if (retrieved_string_id != NONE)
	{
		for (long designer_zone_index = 0; designer_zone_index < scenario->designer_zones.count(); designer_zone_index++)
		{
			s_scenario_designer_zone& designer_zone = scenario->designer_zones[designer_zone_index];
			if (designer_zone.name.get_value() == retrieved_string_id)
				return designer_zone_index;
		}
	}

	return NONE;
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
		char const* name = object_name.name.get_string();\
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
		char const* name = profile.name.get_string();\
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
		char const* script_name = script.name.get_string();\
		if (script_name && *script_name)\
			c_console::write_line("        name: %s", script_name);\
		if (script.parameters.count())\
		{\
			c_console::write_line("            parameters: %d", script.parameters.count());\
			for (auto parameter : script.parameters)\
			{\
				char const* parameter_name = parameter.name.get_string();\
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
		char const* name = global.name.get_string();\
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

