#include "scenario/scenario_definitions.hpp"

int32 __cdecl scenario_cinematic_lighting_palette_entry_get_by_name(const struct scenario* scenario, string_id name)
{
	for (int32 cinematic_lighting_palette_entry = 0; cinematic_lighting_palette_entry < scenario->cinematic_lighting_palette.count; cinematic_lighting_palette_entry++)
	{
		if (scenario->cinematic_lighting_palette[cinematic_lighting_palette_entry].name.get_value() == name)
			return cinematic_lighting_palette_entry;
	}

	return NONE;
}

void scenario::update_reference_names()
{
	UPDATE_REFERENCE_NAME(structure_seams);
	UPDATE_REFERENCE_NAME(custom_object_names);
	UPDATE_REFERENCE_NAME(chapter_title_text);
	UPDATE_REFERENCE_NAME(sound_effect_collection);
	UPDATE_REFERENCE_NAME(subtitles);
	UPDATE_REFERENCE_NAME(game_engine_strings);
	UPDATE_REFERENCE_NAME(objectives);
	UPDATE_REFERENCE_NAME(camera_effects);
	UPDATE_REFERENCE_NAME(podium_override);
	UPDATE_REFERENCE_NAME(global_screen_effect);
	UPDATE_REFERENCE_NAME(atmospheric);
	UPDATE_REFERENCE_NAME(chocalate_mountain);
	UPDATE_REFERENCE_NAME(new_lightmaps);
	UPDATE_REFERENCE_NAME(performance_throttles);
	UPDATE_REFERENCE_NAME(tchou_shader_effect);
	UPDATE_REFERENCE_NAME(vision_mode_override);

	UPDATE_BLOCK_REFERENCE_NAMES(structure_bsp_references);
	UPDATE_BLOCK_REFERENCE_NAMES(sky_references);
	UPDATE_BLOCK_REFERENCE_NAMES(scenery_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(biped_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(vehicle_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(equipment_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(weapon_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(machine_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(terminal_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(arg_device_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(control_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(sound_scenery_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(giant_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(effect_scenery_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(light_volume_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_vehicle_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_weapon_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_equipment_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_scenery_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_teleporters_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_goals_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(map_variant_spawners_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(player_starting_profile);
	UPDATE_BLOCK_REFERENCE_NAMES(weapon_spawn_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(vehicle_spawn_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(projectile_spawn_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(equipment_spawn_influencers);
	UPDATE_BLOCK_REFERENCE_NAMES(hs_references);
	UPDATE_BLOCK_REFERENCE_NAMES(crate_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(creature_palette);
	UPDATE_BLOCK_REFERENCE_NAMES(background_bitmap_references);
}

void scenario_structure_bsp_reference::update_reference_names()
{
	UPDATE_REFERENCE_NAME(structure_bsp);
	UPDATE_REFERENCE_NAME(structure_design);
	UPDATE_REFERENCE_NAME(structure_lighting_info);
	UPDATE_REFERENCE_NAME(cubemap_bitmap_group_reference);
	UPDATE_REFERENCE_NAME(wind);
}

void s_scenario_sky_reference::update_reference_names()
{
	UPDATE_REFERENCE_NAME(sky);
}

void scenario_starting_profile::update_reference_names()
{
	UPDATE_REFERENCE_NAME(starting_weapons[_unit_weapon_set_primary].weapon);
	UPDATE_REFERENCE_NAME(starting_weapons[_unit_weapon_set_secondary].weapon);
}

void s_background_bitmap_reference_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(background_bitmap_group_reference);
}

