#include "tag_files/tag_groups.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/string_ids.hpp"

#include <string.h>

void __cdecl tag_load_missing_tags_report()
{
	// nullsub
	INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

char const* __cdecl tag_name_strip_path(char const* path)
{
	//return INVOKE(0x0055AA90, tag_name_strip_path, path);

	char const* name = strrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

wchar_t const* __cdecl tag_name_strip_path(wchar_t const* path)
{
	wchar_t const* name = wcsrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

tag group_name_to_group_tag(char const* group_name)
{
	// string_id_retrieve
	for (long i = 0; i < global_tag_group_count; i++)
	{
		s_cache_file_tag_group const* group = &global_tag_groups[i];
		if (csstricmp(group_name, group->name.get_string()) == 0)
			return group->group_tags[0];
	}

	return NONE;
}

void* s_tag_reference::get_definition()
{
	return tag_get(group_tag, index);
}

char const* s_tag_reference::get_name()
{
	if (name)
		return name;

	if (!VALID_INDEX(index, g_cache_file_globals.header.debug_tag_name_count))
		return "<unknown>";

	if (char const* _name = tag_get_name_safe(index))
	{
		name = _name;
		name_length = csstrnlen(_name, _MAX_PATH);

		return _name;
	}

	return "<unknown>";
}

char const* s_tag_reference::get_group_name()
{
	if (group_tag != NONE)
		return tag_group_get_name(group_tag);

	if (!g_cache_file_globals.tag_instances || !g_cache_file_globals.tag_index_absolute_mapping)
		return "<unknown>";

	return g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[index]]->tag_group.name.get_string();
}

s_cache_file_tag_group const global_tag_groups[] =
{
	{
		.group_tags = { ACHIEVEMENTS_TAG },
		.name = STRING_ID(global, achievements)
	},
	{
		.group_tags = { AI_DIALOGUE_GLOBALS_TAG },
		.name = STRING_ID(global, ai_dialogue_globals)
	},
	{
		.group_tags = { AI_GLOBALS_TAG },
		.name = STRING_ID(global, ai_globals)
	},
	{
		.group_tags = { AI_MISSION_DIALOGUE_TAG },
		.name = STRING_ID(global, ai_mission_dialogue)
	},
	{
		.group_tags = { ANTENNA_TAG },
		.name = STRING_ID(global, antenna)
	},
	{
		.group_tags = { AREA_SCREEN_EFFECT_TAG },
		.name = STRING_ID(global, area_screen_effect)
	},
	{
		.group_tags = { ARMOR_SOUNDS_TAG },
		.name = STRING_ID(global, armor_sounds)
	},
	{
		.group_tags = { BEAM_SYSTEM_TAG },
		.name = STRING_ID(global, beam_system)
	},
	{
		.group_tags = { BINK_TAG },
		.name = STRING_ID(global, bink)
	},
	{
		.group_tags = { BIPED_TAG },
		.name = STRING_ID(global, biped)
	},
	{
		.group_tags = { BITMAP_TAG },
		.name = STRING_ID(global, bitmap)
	},
	{
		.group_tags = { BREAKABLE_SURFACE_TAG },
		.name = STRING_ID(global, breakable_surface)
	},
	{
		.group_tags = { CACHE_FILE_GLOBAL_TAGS_TAG },
		.name = STRING_ID(global, cache_file_global_tags)
	},
	{
		.group_tags = { CACHE_FILE_RESOURCE_GESTALT_TAG },
		.name = STRING_ID(global, cache_file_resource_gestalt)
	},
	{
		.group_tags = { CACHE_FILE_SOUND_TAG },
		.name = STRING_ID(global, cache_file_sound)
	},
	{
		.group_tags = { CAMERA_FX_SETTINGS_TAG },
		.name = STRING_ID(global, camera_fx_settings)
	},
	{
		.group_tags = { CAMERA_TRACK_TAG },
		.name = STRING_ID(global, camera_track)
	},
	{
		.group_tags = { CAMO_TAG },
		.name = STRING_ID(global, camo)
	},
	{
		.group_tags = { CELLULAR_AUTOMATA2D_TAG },
		.name = STRING_ID(global, cellular_automata2d)
	},
	{
		.group_tags = { CELLULAR_AUTOMATA_TAG },
		.name = STRING_ID(global, cellular_automata)
	},
	{
		.group_tags = { CHARACTER_TAG },
		.name = STRING_ID(global, character)
	},
	{
		.group_tags = { CHOCOLATE_MOUNTAIN_NEW_TAG },
		.name = STRING_ID(global, chocolate_mountain_new)
	},
	{
		.group_tags = { CHUD_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, chud_animation_definition)
	},
	{
		.group_tags = { CHUD_DEFINITION_TAG },
		.name = STRING_ID(global, chud_definition)
	},
	{
		.group_tags = { CHUD_GLOBALS_DEFINITION_TAG },
		.name = STRING_ID(global, chud_globals_definition)
	},
	{
		.group_tags = { CHUD_WIDGET_PARALLAX_DATA_TAG },
		.name = STRING_ID(global, chud_widget_parallax_data)
	},
	{
		.group_tags = { CINEMATIC_SCENE_TAG },
		.name = STRING_ID(global, cinematic_scene)
	},
	{
		.group_tags = { CINEMATIC_TAG },
		.name = STRING_ID(global, cinematic)
	},
	{
		.group_tags = { CLOTH_TAG },
		.name = STRING_ID(global, cloth)
	},
	{
		.group_tags = { COLLISION_DAMAGE_TAG },
		.name = STRING_ID(global, collision_damage)
	},
	{
		.group_tags = { COLLISION_MODEL_TAG },
		.name = STRING_ID(global, collision_model)
	},
	{
		.group_tags = { COLOR_TABLE_TAG },
		.name = STRING_ID(global, color_table)
	},
	{
		.group_tags = { CONTRAIL_SYSTEM_TAG },
		.name = STRING_ID(global, contrail_system)
	},
	{
		.group_tags = { CORTANA_EFFECT_DEFINITION_TAG },
		.name = STRING_ID(global, cortana_effect_definition)
	},
	{
		.group_tags = { CRATE_TAG },
		.name = STRING_ID(global, crate)
	},
	{
		.group_tags = { CREATURE_TAG },
		.name = STRING_ID(global, creature)
	},
	{
		.group_tags = { DAMAGE_EFFECT_TAG },
		.name = STRING_ID(global, damage_effect)
	},
	{
		.group_tags = { DAMAGE_RESPONSE_DEFINITION_TAG },
		.name = STRING_ID(global, damage_response_definition)
	},
	{
		.group_tags = { DECAL_SYSTEM_TAG },
		.name = STRING_ID(global, decal_system)
	},
	{
		.group_tags = { DECORATOR_SET_TAG },
		.name = STRING_ID(global, decorator_set)
	},
	{
		.group_tags = { DETAIL_OBJECT_COLLECTION_TAG },
		.name = STRING_ID(global, detail_object_collection)
	},
	{
		.group_tags = { DEVICE_ARG_DEVICE_TAG },
		.name = STRING_ID(global, device_arg_device)
	},
	{
		.group_tags = { DEVICE_CONTROL_TAG },
		.name = STRING_ID(global, device_control)
	},
	{
		.group_tags = { DEVICE_MACHINE_TAG },
		.name = STRING_ID(global, device_machine)
	},
	{
		.group_tags = { DEVICE_TAG },
		.name = STRING_ID(global, device)
	},
	{
		.group_tags = { DEVICE_TERMINAL_TAG },
		.name = STRING_ID(global, device_terminal)
	},
	{
		.group_tags = { DIALOGUE_TAG },
		.name = STRING_ID(global, dialogue)
	},
	{
		.group_tags = { EFFECT_GLOBALS_TAG },
		.name = STRING_ID(global, effect_globals)
	},
	{
		.group_tags = { EFFECT_SCENERY_TAG },
		.name = STRING_ID(global, effect_scenery)
	},
	{
		.group_tags = { EFFECT_TAG },
		.name = STRING_ID(global, effect)
	},
	{
		.group_tags = { EQUIPMENT_TAG },
		.name = STRING_ID(global, equipment)
	},
	{
		.group_tags = { FLOCK_TAG },
		.name = STRING_ID(global, flock)
	},
	{
		.group_tags = { FLUID_DYNAMICS_TAG },
		.name = STRING_ID(global, fluid_dynamics)
	},
	{
		.group_tags = { FORMATION_TAG },
		.name = STRING_ID(global, formation)
	},
	{
		.group_tags = { FRAGMENT_TAG },
		.name = STRING_ID(global, fragment)
	},
	{
		.group_tags = { GAME_ENGINE_SETTINGS_DEFINITION_TAG },
		.name = STRING_ID(global, game_engine_settings_definition)
	},
	{
		.group_tags = { GAME_PROGRESSION_TAG },
		.name = STRING_ID(global, game_progression)
	},
	{
		.group_tags = { GFX_TEXTURES_LIST_TAG },
		.name = STRING_ID(global, gfx_textures_list)
	},
	{
		.group_tags = { GIANT_TAG },
		.name = STRING_ID(global, giant)
	},
	{
		.group_tags = { GLOBALS_TAG },
		.name = STRING_ID(global, globals)
	},
	{
		.group_tags = { GLOBAL_PIXEL_SHADER_TAG },
		.name = STRING_ID(global, global_pixel_shader)
	},
	{
		.group_tags = { GLOBAL_VERTEX_SHADER_TAG },
		.name = STRING_ID(global, global_vertex_shader)
	},
	{
		.group_tags = { GUI_BITMAP_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_bitmap_widget_definition)
	},
	{
		.group_tags = { GUI_BUTTON_KEY_DEFINITION_TAG },
		.name = STRING_ID(global, gui_button_key_definition)
	},
	{
		.group_tags = { GUI_DATASOURCE_DEFINITION_TAG },
		.name = STRING_ID(global, gui_datasource_definition)
	},
	{
		.group_tags = { GUI_GROUP_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_group_widget_definition)
	},
	{
		.group_tags = { GUI_LIST_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_list_widget_definition)
	},
	{
		.group_tags = { GUI_MODEL_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_model_widget_definition)
	},
	{
		.group_tags = { GUI_SCREEN_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_screen_widget_definition)
	},
	{
		.group_tags = { GUI_SKIN_DEFINITION_TAG },
		.name = STRING_ID(global, gui_skin_definition)
	},
	{
		.group_tags = { GUI_TEXT_WIDGET_DEFINITION_TAG },
		.name = STRING_ID(global, gui_text_widget_definition)
	},
	{
		.group_tags = { GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_animation_collection_definition)
	},
	{
		.group_tags = { GUI_WIDGET_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_COLOR_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_color_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_FONT_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_font_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_POSITION_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_position_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_ROTATION_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_rotation_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_SCALE_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_scale_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_SPRITE_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_sprite_animation_definition)
	},
	{
		.group_tags = { GUI_WIDGET_TEXTURE_COORDINATE_ANIMATION_DEFINITION_TAG },
		.name = STRING_ID(global, gui_widget_texture_coordinate_animation_definition)
	},
	{
		.group_tags = { HLSL_INCLUDE_TAG },
		.name = STRING_ID(global, hlsl_include)
	},
	{
		.group_tags = { INPUT_GLOBALS_TAG },
		.name = STRING_ID(global, input_globals)
	},
	{
		.group_tags = { ITEM_COLLECTION_TAG },
		.name = STRING_ID(global, item_collection)
	},
	{
		.group_tags = { ITEM_TAG },
		.name = STRING_ID(global, item)
	},
	{
		.group_tags = { LEAF_SYSTEM_TAG },
		.name = STRING_ID(global, leaf_system)
	},
	{
		.group_tags = { LENS_FLARE_TAG },
		.name = STRING_ID(global, lens_flare)
	},
	{
		.group_tags = { LIGHT_TAG },
		.name = STRING_ID(global, light)
	},
	{
		.group_tags = { LIGHT_VOLUME_SYSTEM_TAG },
		.name = STRING_ID(global, light_volume_system)
	},
	{
		.group_tags = { MATERIAL_EFFECTS_TAG },
		.name = STRING_ID(global, material_effects)
	},
	{
		.group_tags = { METER_TAG },
		.name = STRING_ID(global, meter)
	},
	{
		.group_tags = { MODEL_ANIMATION_GRAPH_TAG },
		.name = STRING_ID(global, model_animation_graph)
	},
	{
		.group_tags = { MODEL_TAG },
		.name = STRING_ID(global, model)
	},
	{
		.group_tags = { MUFFIN_TAG },
		.name = STRING_ID(global, muffin)
	},
	{
		.group_tags = { MULTILINGUAL_UNICODE_STRING_LIST_TAG },
		.name = STRING_ID(global, multilingual_unicode_string_list)
	},
	{
		.group_tags = { MULTIPLAYER_GLOBALS_TAG },
		.name = STRING_ID(global, multiplayer_globals)
	},
	{
		.group_tags = { MULTIPLAYER_SCENARIO_DESCRIPTION_TAG },
		.name = STRING_ID(global, multiplayer_scenario_description)
	},
	{
		.group_tags = { MULTIPLAYER_VARIANT_SETTINGS_INTERFACE_DEFINITION_TAG },
		.name = STRING_ID(global, multiplayer_variant_settings_interface_definition)
	},
	{
		.group_tags = { NEW_CINEMATIC_LIGHTING_TAG },
		.name = STRING_ID(global, new_cinematic_lighting)
	},
	{
		.group_tags = { OBJECT_TAG },
		.name = STRING_ID(global, object)
	},
	{
		.group_tags = { PARTICLE_EMITTER_CUSTOM_POINTS_TAG },
		.name = STRING_ID(global, particle_emitter_custom_points)
	},
	{
		.group_tags = { PARTICLE_MODEL_TAG },
		.name = STRING_ID(global, particle_model)
	},
	{
		.group_tags = { PARTICLE_PHYSICS_TAG },
		.name = STRING_ID(global, particle_physics)
	},
	{
		.group_tags = { PARTICLE_TAG },
		.name = STRING_ID(global, particle)
	},
	{
		.group_tags = { PATCHY_FOG_TAG },
		.name = STRING_ID(global, patchy_fog)
	},
	{
		.group_tags = { PERFORMANCE_THROTTLES_TAG },
		.name = STRING_ID(global, performance_throttles)
	},
	{
		.group_tags = { PHYSICS_MODEL_TAG },
		.name = STRING_ID(global, physics_model)
	},
	{
		.group_tags = { PIXEL_SHADER_TAG },
		.name = STRING_ID(global, pixel_shader)
	},
	{
		.group_tags = { PLANAR_FOG_TAG },
		.name = STRING_ID(global, planar_fog)
	},
	{
		.group_tags = { PODIUM_SETTINGS_TAG },
		.name = STRING_ID(global, podium_settings)
	},
	{
		.group_tags = { POINT_PHYSICS_TAG },
		.name = STRING_ID(global, point_physics)
	},
	{
		.group_tags = { PROJECTILE_TAG },
		.name = STRING_ID(global, projectile)
	},
	{
		.group_tags = { RASTERIZER_CACHE_FILE_GLOBALS_TAG },
		.name = STRING_ID(global, rasterizer_cache_file_globals)
	},
	{
		.group_tags = { RASTERIZER_GLOBALS_TAG },
		.name = STRING_ID(global, rasterizer_globals)
	},
	{
		.group_tags = { RENDER_METHOD_DEFINITION_TAG },
		.name = STRING_ID(global, render_method_definition)
	},
	{
		.group_tags = { RENDER_METHOD_OPTION_TAG },
		.name = STRING_ID(global, render_method_option)
	},
	{
		.group_tags = { RENDER_METHOD_TAG },
		.name = STRING_ID(global, render_method)
	},
	{
		.group_tags = { RENDER_METHOD_TEMPLATE_TAG },
		.name = STRING_ID(global, render_method_template)
	},
	{
		.group_tags = { RENDER_MODEL_TAG },
		.name = STRING_ID(global, render_model)
	},
	{
		.group_tags = { RENDER_WATER_RIPPLE_TAG },
		.name = STRING_ID(global, render_water_ripple)
	},
	{
		.group_tags = { SANDBOX_TEXT_VALUE_PAIR_DEFINITION_TAG },
		.name = STRING_ID(global, sandbox_text_value_pair_definition)
	},
	{
		.group_tags = { SCENARIO_AI_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_ai_resource)
	},
	{
		.group_tags = { SCENARIO_BIPEDS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_bipeds_resource)
	},
	{
		.group_tags = { SCENARIO_CINEMATICS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_cinematics_resource)
	},
	{
		.group_tags = { SCENARIO_CLUSTER_DATA_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_cluster_data_resource)
	},
	{
		.group_tags = { SCENARIO_COMMENTS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_comments_resource)
	},
	{
		.group_tags = { SCENARIO_CREATURE_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_creature_resource)
	},
	{
		.group_tags = { SCENARIO_CUBEMAP_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_cubemap_resource)
	},
	{
		.group_tags = { SCENARIO_DECALS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_decals_resource)
	},
	{
		.group_tags = { SCENARIO_DECORATORS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_decorators_resource)
	},
	{
		.group_tags = { SCENARIO_DEVICES_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_devices_resource)
	},
	{
		.group_tags = { SCENARIO_EFFECT_SCENERY_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_effect_scenery_resource)
	},
	{
		.group_tags = { SCENARIO_EQUIPMENT_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_equipment_resource)
	},
	{
		.group_tags = { SCENARIO_FAUX_DATA_TAG },
		.name = STRING_ID(global, scenario_faux_data)
	},
	{
		.group_tags = { SCENARIO_HS_SOURCE_FILE_TAG },
		.name = STRING_ID(global, scenario_hs_source_file)
	},
	{
		.group_tags = { SCENARIO_LIGHTMAP_BSP_DATA_TAG },
		.name = STRING_ID(global, scenario_lightmap_bsp_data)
	},
	{
		.group_tags = { SCENARIO_LIGHTMAP_TAG },
		.name = STRING_ID(global, scenario_lightmap)
	},
	{
		.group_tags = { SCENARIO_LIGHTS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_lights_resource)
	},
	{
		.group_tags = { SCENARIO_PDA_TAG },
		.name = STRING_ID(global, scenario_pda)
	},
	{
		.group_tags = { SCENARIO_SCENERY_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_scenery_resource)
	},
	{
		.group_tags = { SCENARIO_SKY_REFERENCES_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_sky_references_resource)
	},
	{
		.group_tags = { SCENARIO_SOUND_SCENERY_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_sound_scenery_resource)
	},
	{
		.group_tags = { SCENARIO_STRUCTURE_BSP_TAG },
		.name = STRING_ID(global, scenario_structure_bsp)
	},
	{
		.group_tags = { SCENARIO_STRUCTURE_LIGHTING_INFO_TAG },
		.name = STRING_ID(global, scenario_structure_lighting_info)
	},
	{
		.group_tags = { SCENARIO_STRUCTURE_LIGHTING_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_structure_lighting_resource)
	},
	{
		.group_tags = { SCENARIO_TAG },
		.name = STRING_ID(global, scenario)
	},
	{
		.group_tags = { SCENARIO_TRIGGER_VOLUMES_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_trigger_volumes_resource)
	},
	{
		.group_tags = { SCENARIO_VEHICLES_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_vehicles_resource)
	},
	{
		.group_tags = { SCENARIO_WEAPONS_RESOURCE_TAG },
		.name = STRING_ID(global, scenario_weapons_resource)
	},
	{
		.group_tags = { SCENERY_TAG },
		.name = STRING_ID(global, scenery)
	},
	{
		.group_tags = { SCREEN_EFFECT_TAG },
		.name = STRING_ID(global, screen_effect)
	},
	{
		.group_tags = { SHADER_BEAM_TAG },
		.name = STRING_ID(global, shader_beam)
	},
	{
		.group_tags = { SHADER_BLACK_TAG },
		.name = STRING_ID(global, shader_black)
	},
	{
		.group_tags = { SHADER_CONTRAIL_TAG },
		.name = STRING_ID(global, shader_contrail)
	},
	{
		.group_tags = { SHADER_CORTANA_TAG },
		.name = STRING_ID(global, shader_cortana)
	},
	{
		.group_tags = { SHADER_CUSTOM_TAG },
		.name = STRING_ID(global, shader_custom)
	},
	{
		.group_tags = { SHADER_DECAL_TAG },
		.name = STRING_ID(global, shader_decal)
	},
	{
		.group_tags = { SHADER_FOLIAGE_TAG },
		.name = STRING_ID(global, shader_foliage)
	},
	{
		.group_tags = { SHADER_HALOGRAM_TAG },
		.name = STRING_ID(global, shader_halogram)
	},
	{
		.group_tags = { SHADER_LIGHT_VOLUME_TAG },
		.name = STRING_ID(global, shader_light_volume)
	},
	{
		.group_tags = { SHADER_PARTICLE_TAG },
		.name = STRING_ID(global, shader_particle)
	},
	{
		.group_tags = { SHADER_SCREEN_TAG },
		.name = STRING_ID(global, shader_screen)
	},
	{
		.group_tags = { SHADER_SKIN_TAG },
		.name = STRING_ID(global, shader_skin)
	},
	{
		.group_tags = { SHADER_TAG },
		.name = STRING_ID(global, shader)
	},
	{
		.group_tags = { SHADER_TERRAIN_TAG },
		.name = STRING_ID(global, shader_terrain)
	},
	{
		.group_tags = { SHADER_WATER_TAG },
		.name = STRING_ID(global, shader_water)
	},
	{
		.group_tags = { SHADER_ZONLY_TAG },
		.name = STRING_ID(global, shader_zonly)
	},
	{
		.group_tags = { SHIELD_IMPACT_TAG },
		.name = STRING_ID(global, shield_impact)
	},
	{
		.group_tags = { SKY_ATM_PARAMETERS_TAG },
		.name = STRING_ID(global, sky_atm_parameters)
	},
	{
		.group_tags = { SOUND_CACHE_FILE_GESTALT_TAG },
		.name = STRING_ID(global, sound_cache_file_gestalt)
	},
	{
		.group_tags = { SOUND_CLASSES_TAG },
		.name = STRING_ID(global, sound_classes)
	},
	{
		.group_tags = { SOUND_DIALOGUE_CONSTANTS_TAG },
		.name = STRING_ID(global, sound_dialogue_constants)
	},
	{
		.group_tags = { SOUND_EFFECT_COLLECTION_TAG },
		.name = STRING_ID(global, sound_effect_collection)
	},
	{
		.group_tags = { SOUND_EFFECT_TEMPLATE_TAG },
		.name = STRING_ID(global, sound_effect_template)
	},
	{
		.group_tags = { SOUND_ENVIRONMENT_TAG },
		.name = STRING_ID(global, sound_environment)
	},
	{
		.group_tags = { SOUND_GLOBAL_PROPAGATION_TAG },
		.name = STRING_ID(global, sound_global_propagation)
	},
	{
		.group_tags = { SOUND_LOOPING_TAG },
		.name = STRING_ID(global, sound_looping)
	},
	{
		.group_tags = { SOUND_MIX_TAG },
		.name = STRING_ID(global, sound_mix)
	},
	{
		.group_tags = { SOUND_SCENERY_TAG },
		.name = STRING_ID(global, sound_scenery)
	},
	{
		.group_tags = { SOUND_TAG },
		.name = STRING_ID(global, sound)
	},
	{
		.group_tags = { SOUND_UI_SOUNDS_TAG },
		.name = STRING_ID(global, sound_ui_sounds)
	},
	{
		.group_tags = { SQUAD_TEMPLATE_TAG },
		.name = STRING_ID(global, squad_template)
	},
	{
		.group_tags = { STEREO_SYSTEM_TAG },
		.name = STRING_ID(global, stereo_system)
	},
	{
		.group_tags = { STRUCTURE_DESIGN_TAG },
		.name = STRING_ID(global, structure_design)
	},
	{
		.group_tags = { STRUCTURE_SEAMS_TAG },
		.name = STRING_ID(global, structure_seams)
	},
	{
		.group_tags = { STYLE_TAG },
		.name = STRING_ID(global, style)
	},
	{
		.group_tags = { SURVIVAL_MODE_GLOBALS_TAG },
		.name = STRING_ID(global, survival_mode_globals)
	},
	{
		.group_tags = { TAG_TEMPLATE_UNIT_TEST_TAG },
		.name = STRING_ID(global, tag_template_unit_test)
	},
	{
		.group_tags = { TEST_TAG_TAG },
		.name = STRING_ID(global, test_tag)
	},
	{
		.group_tags = { TEXTURE_RENDER_LIST_TAG },
		.name = STRING_ID(global, texture_render_list)
	},
	{
		.group_tags = { TEXT_VALUE_PAIR_DEFINITION_TAG },
		.name = STRING_ID(global, text_value_pair_definition)
	},
	{
		.group_tags = { UNIT_TAG },
		.name = STRING_ID(global, unit)
	},
	{
		.group_tags = { USER_INTERFACE_FOURTH_WALL_TIMING_DEFINITION_TAG },
		.name = STRING_ID(global, user_interface_fourth_wall_timing_definition)
	},
	{
		.group_tags = { USER_INTERFACE_GLOBALS_DEFINITION_TAG },
		.name = STRING_ID(global, user_interface_globals_definition)
	},
	{
		.group_tags = { USER_INTERFACE_SHARED_GLOBALS_DEFINITION_TAG },
		.name = STRING_ID(global, user_interface_shared_globals_definition)
	},
	{
		.group_tags = { USER_INTERFACE_SOUNDS_DEFINITION_TAG },
		.name = STRING_ID(global, user_interface_sounds_definition)
	},
	{
		.group_tags = { VEHICLE_COLLECTION_TAG },
		.name = STRING_ID(global, vehicle_collection)
	},
	{
		.group_tags = { VEHICLE_TAG },
		.name = STRING_ID(global, vehicle)
	},
	{
		.group_tags = { VERTEX_SHADER_TAG },
		.name = STRING_ID(global, vertex_shader)
	},
	{
		.group_tags = { VFILES_LIST_TAG },
		.name = STRING_ID(global, vfiles_list)
	},
	{
		.group_tags = { VISION_MODE_TAG },
		.name = STRING_ID(global, vision_mode)
	},
	{
		.group_tags = { WEAPON_TAG },
		.name = STRING_ID(global, weapon)
	},
	{
		.group_tags = { WIND_TAG },
		.name = STRING_ID(global, wind)
	},
};
long const global_tag_group_count = NUMBEROF(global_tag_groups);

