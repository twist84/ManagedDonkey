#include "scenario/scenario_definitions.hpp"

void s_scenario::update_reference_names()
{
	structure_seams.get_name();
	custom_object_names.get_name();
	chapter_title_text.get_name();
	sound_effect_collection.get_name();
	subtitles.get_name();
	game_engine_strings.get_name();
	objectives.get_name();
	camera_effects.get_name();
	global_screen_effect.get_name();
	atmospheric.get_name();
	chocalate_mountain.get_name();
	new_lightmaps.get_name();
	performance_throttles.get_name();
	tchou_shader_effect.get_name();
	vision_mode_override.get_name();

	UPDATE_REFERENCE_NAMES(structure_bsps);
	UPDATE_REFERENCE_NAMES(sky_references);
	UPDATE_REFERENCE_NAMES(scenery_palette);
	UPDATE_REFERENCE_NAMES(biped_palette);
	UPDATE_REFERENCE_NAMES(vehicle_palette);
	UPDATE_REFERENCE_NAMES(equipment_palette);
	UPDATE_REFERENCE_NAMES(weapon_palette);
	UPDATE_REFERENCE_NAMES(machine_palette);
	UPDATE_REFERENCE_NAMES(terminal_palette);
	UPDATE_REFERENCE_NAMES(arg_device_palette);
	UPDATE_REFERENCE_NAMES(control_palette);
	UPDATE_REFERENCE_NAMES(sound_scenery_palette);
	UPDATE_REFERENCE_NAMES(giant_palette);
	UPDATE_REFERENCE_NAMES(effect_scenery_palette);
	UPDATE_REFERENCE_NAMES(light_volume_palette);
	UPDATE_REFERENCE_NAMES(map_variant_vehicle_palette);
	UPDATE_REFERENCE_NAMES(map_variant_weapon_palette);
	UPDATE_REFERENCE_NAMES(map_variant_equipment_palette);
	UPDATE_REFERENCE_NAMES(map_variant_scenery_palette);
	UPDATE_REFERENCE_NAMES(map_variant_teleporters_palette);
	UPDATE_REFERENCE_NAMES(map_variant_goals_palette);
	UPDATE_REFERENCE_NAMES(map_variant_spawners_palette);
	UPDATE_REFERENCE_NAMES(player_starting_profile);
	UPDATE_REFERENCE_NAMES(weapon_spawn_influencers);
	UPDATE_REFERENCE_NAMES(vehicle_spawn_influencers);
	UPDATE_REFERENCE_NAMES(projectile_spawn_influencers);
	UPDATE_REFERENCE_NAMES(equipment_spawn_influencers);
	UPDATE_REFERENCE_NAMES(references);
	UPDATE_REFERENCE_NAMES(crate_palette);
	UPDATE_REFERENCE_NAMES(creature_palette);
	UPDATE_REFERENCE_NAMES(background_bitmap_references);
}

void scenario_structure_bsp_reference::update_reference_names()
{
	structure_bsp.get_name();
	structure_design.get_name();
	structure_lighting_info.get_name();
	cubemap_bitmap_group_reference.get_name();
	wind.get_name();
}

void s_scenario_sky_reference::update_reference_names()
{
	sky.get_name();
}

void scenario_starting_profile::update_reference_names()
{
	for (long i = 0; i < NUMBEROF(starting_weapons); i++)
		starting_weapons[i].weapon.get_name();
}

void s_background_bitmap_reference_definition::update_reference_names()
{
	background_bitmap_group_reference.get_name();
}
