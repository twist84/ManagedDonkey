#include "hs/hs_library_external.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/object_lists.hpp"
#include "interface/terminal.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "models/model_definitions.hpp"
#include "motor/actions.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "units/giants.hpp"

void __cdecl hs_evaluate_library_external(int16 script_index);

//HOOK_DECLARE(0x0096CEA0, hs_bitmap_predict);
//HOOK_DECLARE(0x0096CEC0, hs_damage_new);
//HOOK_DECLARE(0x0096CF50, hs_damage_object);
//HOOK_DECLARE(0x0096D070, hs_damage_object_effect);
//HOOK_DECLARE(0x0096D100, hs_damage_object_effect_list);
//HOOK_DECLARE(0x0096D1C0, hs_damage_object_list);
//HOOK_DECLARE(0x0096D320, hs_damage_players);
//HOOK_DECLARE(0x0096D3E0, hs_debug_variable);
//HOOK_DECLARE(0x0096D3F0, hs_debug_variable_all);
//HOOK_DECLARE(0x0096D400, hs_effect_new);
//HOOK_DECLARE(0x0096D530, hs_effect_new_at_ai_point);
//HOOK_DECLARE(0x0096D680, hs_effect_new_from_object_marker);
//HOOK_DECLARE(0x0096D6F0, hs_effect_new_on_ground);
//HOOK_DECLARE(0x0096D7D0, hs_effect_new_random);
HOOK_DECLARE(0x0096D850, hs_enable_debug_globals);
HOOK_DECLARE(0x0096D860, hs_enable_debug_scripting);
HOOK_DECLARE(0x0096D870, hs_evaluate_library_external);
HOOK_DECLARE(0x0096D8A0, hs_find_matching_commands);
HOOK_DECLARE(0x0096D8B0, hs_log_print);
HOOK_DECLARE(0x0096D8C0, hs_map_info);
HOOK_DECLARE(0x0096D8D0, hs_not);
//HOOK_DECLARE(0x0096D8E0, hs_object_buckling_magnitude_get);
HOOK_DECLARE(0x0096D940, hs_object_create);
HOOK_DECLARE(0x0096D970, hs_object_create_anew);
HOOK_DECLARE(0x0096D9D0, hs_object_create_anew_containing);
HOOK_DECLARE(0x0096D9F0, hs_object_create_anew_multiplayer_cinematic);
HOOK_DECLARE(0x0096DA50, hs_object_create_clone);
HOOK_DECLARE(0x0096DA80, hs_object_create_clone_containing);
HOOK_DECLARE(0x0096DAA0, hs_object_create_clone_multiplayer_cinematic);
HOOK_DECLARE(0x0096DAD0, hs_object_create_containing);
//HOOK_DECLARE(0x0096DAF0, hs_object_create_folder);
HOOK_DECLARE(0x0096DC50, hs_object_create_folder_anew);
//HOOK_DECLARE(0x0096DC70, hs_object_create_folder_internal);
HOOK_DECLARE(0x0096DCE0, hs_object_create_if_necessary);
HOOK_DECLARE(0x0096DD20, hs_object_create_multiplayer_cinematic);
HOOK_DECLARE(0x0096DD50, hs_object_delete_internal);
HOOK_DECLARE(0x0096DD70, hs_object_destroy);
HOOK_DECLARE(0x0096DDB0, hs_object_destroy_all);
HOOK_DECLARE(0x0096DDC0, hs_object_destroy_all_type_mask);
HOOK_DECLARE(0x0096DEA0, hs_object_destroy_by_name);
HOOK_DECLARE(0x0096DF00, hs_object_destroy_containing);
//HOOK_DECLARE(0x0096DF20, hs_object_destroy_folder);
//HOOK_DECLARE(0x0096E080, hs_object_destroy_folder_internal);
HOOK_DECLARE(0x0096E1F0, hs_object_destroy_multiplayer_cinematic);
HOOK_DECLARE(0x0096E230, hs_object_hide);
HOOK_DECLARE(0x0096E250, hs_object_iterate_names_containing);
//HOOK_DECLARE(0x0096E2B0, hs_object_list_get_element);
HOOK_DECLARE(0x0096E2F0, hs_object_new_by_name_internal);
//HOOK_DECLARE(0x0096E320, hs_object_orient);
HOOK_DECLARE(0x0096E4D0, hs_object_set_facing);
HOOK_DECLARE(0x0096E620, hs_object_set_permutation);
//HOOK_DECLARE(0x0096E640, hs_object_set_region_state);
HOOK_DECLARE(0x0096E6B0, hs_object_set_shadowless);
//HOOK_DECLARE(0x0096E6E0, hs_object_set_shield);
//HOOK_DECLARE(0x0096E740, hs_object_set_shield_normalized);
//HOOK_DECLARE(0x0096E7A0, hs_object_set_shield_stun);
//HOOK_DECLARE(0x0096E7D0, hs_object_set_shield_stun_infinite);
HOOK_DECLARE(0x0096E7F0, hs_object_set_variant);
HOOK_DECLARE(0x0096E810, hs_object_teleport);
HOOK_DECLARE(0x0096E870, hs_object_teleport_ai);
HOOK_DECLARE(0x0096E8B0, hs_objects_can_see_flag);
HOOK_DECLARE(0x0096E970, hs_objects_can_see_object);
HOOK_DECLARE(0x0096EA70, hs_objects_delete_by_definition);
HOOK_DECLARE(0x0096EAD0, hs_objects_distance_to_flag);
HOOK_DECLARE(0x0096EBE0, hs_objects_distance_to_object);
HOOK_DECLARE(0x0096ECE0, hs_objects_predict);
HOOK_DECLARE(0x0096ED30, hs_objects_predict_high);
HOOK_DECLARE(0x0096ED70, hs_objects_predict_low);
HOOK_DECLARE(0x0096EDB0, hs_objects_predict_old);
HOOK_DECLARE(0x0096EDF0, hs_pin);
//HOOK_DECLARE(0x0096EE40, hs_player_get);
//HOOK_DECLARE(0x0096EE90, hs_players);
HOOK_DECLARE(0x0096EF50, hs_position_predict);
HOOK_DECLARE(0x0096EF60, hs_print);
HOOK_DECLARE(0x0096EF70, hs_shader_predict);
HOOK_DECLARE(0x0096EF80, hs_tag_load_force_programmer_only);
HOOK_DECLARE(0x0096EF90, hs_tag_unload_force_programmer_only);
HOOK_DECLARE(0x0096EFA0, hs_teleport_players_not_in_trigger_volume);
HOOK_DECLARE(0x0096F080, hs_trigger_volume_test_objects);
HOOK_DECLARE(0x0096F0F0, hs_trigger_volume_test_objects_all);
HOOK_DECLARE(0x0096F150, hs_trigger_volume_test_objects_any);
HOOK_DECLARE(0x0096F1B0, hs_unit_can_see_flag);
HOOK_DECLARE(0x0096F210, hs_unit_can_see_object);
HOOK_DECLARE(0x0096F2A0, hs_user_interface_controller_get_last_level_played);

void __cdecl hs_evaluate_library_external(int16 script_index)
{
	hs_evaluate(script_index);
}

void __cdecl hs_bitmap_predict(int32 bitmap_group_index)
{
#if 1
	INVOKE(0x0096CEA0, hs_bitmap_predict, bitmap_group_index);
#else
	if (bitmap_group_index != NONE)
	{
		const bitmap_group* group = TAG_GET(BITMAP_TAG, const bitmap_group, bitmap_group_index);
		for (int32 bitmap_index = 0; bitmap_index < group->bitmaps.count; bitmap_index++)
		{
			bitmap_data* bitmap = TAG_BLOCK_GET_ELEMENT(&group->bitmaps, bitmap_index, bitmap_data);
			texture_cache_bitmap_precache(bitmap, 0.0f);
		}
	}
#endif
}

void __cdecl hs_damage_new(int32 definition_index, int16 flag_index)
{
#if 1
	INVOKE(0x0096CEC0, hs_damage_new, definition_index, flag_index);
#else
	if (!game_is_predicted() && definition_index != NONE && tag_get_group_tag(definition_index) == DAMAGE_EFFECT_TAG)
	{
		scenario_cutscene_flag* flag = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->cutscene_flags, flag_index, scenario_cutscene_flag);
		s_damage_data damage;
		damage_data_new(&damage, definition_index);
		damage.epicenter = flag->position;
		damage.origin = damage.epicenter;
		scenario_location_from_point(&damage.location, &flag->position);
		area_of_effect_cause_damage(&damage, NONE, 5);
	}
#endif
}

void __cdecl hs_damage_object(int32 object_index, int32 material_name, real32 damage_amount)
{
#if 1
	INVOKE(0x0096CF50, hs_damage_object, object_index, material_name, damage_amount);
#else
	if (!game_is_predicted())
	{
		const s_game_globals_falling_damage* falling_damage = TAG_BLOCK_GET_ELEMENT(&scenario_get_game_globals()->falling_damage, 0, const s_game_globals_falling_damage);
		if (falling_damage->hs_damage.index != NONE && object_index != NONE)
		{
			const object_datum* object = OBJECT_GET(const object_datum, object_index);
			const struct object_definition* object_definition = TAG_GET(OBJECT_TAG, const struct object_definition, object->definition_index);
			if (object_definition->object.model.index != NONE)
			{
				const s_model_definition* model_definition = TAG_GET(MODEL_TAG, const s_model_definition, object_definition->object.model.index);
				int32 material_count = model_definition->materials.count;

				int32 material_index;
				for (material_index = 0; material_index < material_count; material_index++)
				{
					s_model_material* model_material = TAG_BLOCK_GET_ELEMENT(&model_definition->materials, material_index, s_model_material);
					if (model_material->material_name.get_value() == material_name)
					{
						break;
					}
				}

				if (material_index < material_count)
				{
					s_damage_data damage;
					damage_data_new(&damage, falling_damage->hs_damage.index);

					object_get_origin(object_index, &damage.origin);
					damage.epicenter = damage.origin;
					damage.multiplier = damage_amount;
					scenario_location_from_point(&damage.location, &damage.origin);
					object_cause_damage(&damage, object_index, NONE, NONE, (int16)material_index, 5);
				}
			}
		}
	}
#endif
}

void __cdecl hs_damage_object_effect(int32 definition_index, int32 object_index)
{
#if 1
	INVOKE(0x0096D070, hs_damage_object_effect, definition_index, object_index);
#else
	if (!game_is_predicted() && object_index != NONE && definition_index != NONE && tag_get_group_tag(definition_index) == DAMAGE_EFFECT_TAG)
	{
		s_damage_data damage;
		damage_data_new(&damage, definition_index);
		object_get_origin(object_index, &damage.origin);
		damage.epicenter = damage.origin;
		scenario_location_from_point(&damage.location, &damage.origin);
		object_cause_damage_simple(&damage, NONE, 5);
	}
#endif
}

void __cdecl hs_damage_object_effect_list(int32 definition_index, int32 object_list_index)
{
#if 1
	INVOKE(0x0096D100, hs_damage_object_effect_list, definition_index, object_list_index);
#else
	int32 reference_index;
	for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
		object_index != NONE;
		object_index = object_list_get_next(object_list_index, &reference_index))
	{
		hs_damage_object_effect(definition_index, object_index);
	}
#endif
}

void __cdecl hs_damage_object_list(int32 object_list_index, int32 material_name, real32 damage_amount)
{
#if 1
	INVOKE(0x0096D1C0, hs_damage_object_list, object_list_index, material_name, damage_amount);
#else
	int32 reference_index;
	for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
		object_index != NONE;
		object_index = object_list_get_next(object_list_index, &reference_index))
	{
		hs_damage_object(object_index, material_name, damage_amount);
	}
#endif
}

void __cdecl hs_damage_players(int32 definition_index)
{
#if 1
	INVOKE(0x0096D320, hs_damage_players, definition_index);
#else
	if (!game_is_predicted())
	{
		c_player_with_unit_iterator iterator;
		iterator.begin();
		while (iterator.next())
		{
			const player_datum* player = iterator.get_datum();
			if (player->unit_index != NONE)
			{
				s_damage_data damage;
				damage_data_new(&damage, definition_index);
				object_get_origin(player->unit_index, &damage.origin);
				damage.epicenter = damage.origin;
				scenario_location_from_point(&damage.location, &damage.origin);
				object_cause_damage_simple(&damage, NONE, 5);
			}
		}
	}
#endif
}

void __cdecl hs_debug_variable(const char* s, bool debug)
{
	//INVOKE(0x0096D3E0, hs_debug_variable, s, debug);

	bool variable_not_enclosed = false;
	if (*s == '\'')
	{
		char global_name[1024]{};
	
		char* pos = global_name;
		char* end = global_name + sizeof(global_name);
		csstrnzcpy(global_name, s + 1, sizeof(global_name));
		if (global_name[0])
		{
			do
			{
				if (*pos == '\'')
				{
					break;
				}
	
				if (pos >= end)
				{
					break;
				}
	
				pos++;
	
			} while (*pos);
		}
	
		if (*pos && pos < end)
		{
			*pos = 0;
		}
		else
		{
			variable_not_enclosed = true;
		}
	
		int16 global_index = hs_find_global_by_name(global_name);
		if (global_index != NONE)
		{
			debug_globals_all = true;
			debug_global_variables[global_index] = debug;
		}
	}
	else
	{
		variable_not_enclosed = true;
	}
	
	if (variable_not_enclosed)
	{
		event(_event_error, "ai: please enclose the variable to be watched in single quotes");
	}
}

void __cdecl hs_debug_variable_all(bool enable)
{
	INVOKE(0x0096D3F0, hs_debug_variable_all, enable);
}

void __cdecl hs_effect_new(int32 definition_index, int16 flag_index)
{
	INVOKE(0x0096D400, hs_effect_new, definition_index, flag_index);
}

void __cdecl hs_effect_new_at_ai_point(int32 definition_index, int32 point_ref)
{
	INVOKE(0x0096D530, hs_effect_new_at_ai_point, definition_index, point_ref);
}

void __cdecl hs_effect_new_from_object_marker(int32 definition_index, int32 object_index, int32 marker_id)
{
#if 1
	INVOKE(0x0096D680, hs_effect_new_from_object_marker, definition_index, object_index, marker_id);
#else
	if (definition_index != NONE && object_index != NONE)
	{
		object_marker marker{};
		if (object_get_markers_by_string_id(object_index, marker_id, &marker, 1))
		{
			s_damage_reporting_info damage_reporting_info{};
			effect_new_attached_from_marker_name(definition_index, NULL, &damage_reporting_info, object_index, NONE, marker_id, 1.0f, 1.0f, NULL, NULL, 0, _effect_deterministic);
		}
		else
		{
			const object_datum* object = OBJECT_GET(const object_datum, object_index);
			const s_tag_block* object_names = &global_scenario_get()->object_names;
			scenario_object_name* object_name = TAG_BLOCK_GET_ELEMENT_SAFE(object_names, object->object.name_index, scenario_object_name);

			event(_event_warning, "object %s(%s) does not have marker '%s'",
				tag_name_strip_path(tag_get_name(object->definition_index)),
				object_name ? object_name->name.get_string() : "",
				string_id_get_string_const(marker_id));
		}
	}
#endif
}

void __cdecl hs_effect_new_on_ground(int32 definition_index, int32 object_index)
{
	INVOKE(0x0096D6F0, hs_effect_new_on_ground, definition_index, object_index);
}

void __cdecl hs_effect_new_random(int32 definition_index, int32 point_ref)
{
	INVOKE(0x0096D7D0, hs_effect_new_random, definition_index, point_ref);
}

void __cdecl hs_enable_debug_globals(bool enable)
{
	//INVOKE(0x0096D850, hs_enable_debug_globals, enable);

	debug_globals = enable;
}

void __cdecl hs_enable_debug_scripting(bool enable)
{
	//INVOKE(0x0096D860, hs_enable_debug_scripting, enable);

	debug_scripting = enable;
}

void __cdecl hs_evaluate(int16 script_index)
{
	//INVOKE(0x0096D870, hs_evaluate_library_external, script_index);

	hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script);
	if (script->parameters.count > 0)
	{
		event(_event_warning, "hs: tried to pass a script to 'evaluate' which has arguments");
	}
	else if (script->script_type != _hs_script_static)
	{
		event(_event_warning, "hs: tried to pass a script to 'evaluate' which isn't static");
	}
	else
	{
		hs_runtime_push_script(script_index);
	}
}

void __cdecl hs_find_matching_commands(const char* substring)
{
	//INVOKE(0x0096D8A0, hs_find_matching_commands, substring);

	for (int32 hs_function_index = 0; hs_function_index < hs_function_table_count; hs_function_index++)
	{
		const hs_function_definition* hs_function = hs_function_get((int16)hs_function_index);
		if (csstrstr(hs_function->name, substring) != 0)
		{
			console_printf("%s", hs_function->name);
		}
	}
	for (int32 hs_global_index = 0; hs_global_index < k_hs_external_global_count; hs_global_index++)
	{
		const hs_global_external* hs_global = hs_global_external_get((int16)hs_global_index);
		if (csstrstr(hs_global->name, substring) != 0)
		{
			console_printf("%s", hs_global->name);
		}
	}
}

void __cdecl hs_log_print(const char* s)
{
	//INVOKE(0x0096D8B0, hs_log_print, s);

	event(_event_warning, "hs: %s", s);
}

void __cdecl hs_map_info()
{
	//INVOKE(0x0096D8C0, hs_map_info);

	const scenario* scenario = global_scenario_get();
	if (scenario)
	{
		console_printf("map: '%s'",
			tag_get_name(global_scenario_index_get()));

		const int32 zone_set_index = scenario_zone_set_index_get();
		if (zone_set_index != NONE)
		{
			console_printf("zone set: '%s' (#%d)",
				TAG_BLOCK_GET_ELEMENT_SAFE(&scenario->zone_sets, zone_set_index, s_scenario_zone_set)->name.get_string(),
				zone_set_index);
		}
		else
		{
			console_printf("no zone set active");
		}

		const uns32 structure_bsp_mask = global_structure_bsp_active_mask_get();
		if (structure_bsp_mask)
		{
			char structure_names[1024]{};
			scenario_get_structure_bsp_string_from_mask(g_active_structure_bsp_mask, structure_names, NUMBEROF(structure_names));
			console_printf("active bsps: '%s' (0x%x)",
				structure_names,
				structure_bsp_mask);
		}
		else
		{
			console_printf("no bsps active");
		}

		const uns32 designer_zone_mask = game_get_active_designer_zone_mask();
		if (designer_zone_mask)
		{
			char designer_zones_names[1024]{};
			scenario_get_designer_zone_string_from_mask(designer_zone_mask, designer_zones_names, NUMBEROF(designer_zones_names));
			console_printf("active designer zones: '%s' (0x%x)",
				designer_zones_names,
				designer_zone_mask);
		}
		else
		{
			console_printf("no designer zones active");
		}

		const uns32 cinematic_zone_mask = game_get_active_cinematic_zone_mask();
		if (cinematic_zone_mask)
		{
			char cinematic_zones_names[1024]{};
			scenario_get_cinematic_zone_string_from_mask(cinematic_zone_mask, cinematic_zones_names, NUMBEROF(cinematic_zones_names));
			console_printf("active cinematic zones: '%s' (0x%x)",
				cinematic_zones_names,
				cinematic_zone_mask);
		}
		else
		{
			console_printf("no cinematic zones active");
		}

		const s_observer_result* observer = observer_try_and_get_camera(0);
		if (observer && cluster_reference_valid(&observer->location.cluster_reference))
		{
			console_printf("current bsp: '%s' (#%d)",
				scenario_get_structure_bsp_name(observer->location.cluster_reference.bsp_index),
				observer->location.cluster_reference.bsp_index);
		}
		else
		{
			console_printf("no current bsp");
		}
	}
	else
	{
		console_printf("no map loaded");
	}
}

bool __cdecl hs_not(bool value)
{
	//return INVOKE(0x0096D8D0, hs_not, value);

	return !value;
}

real32 __cdecl hs_object_buckling_magnitude_get(int32 object_index)
{
	//return INVOKE(0x0096D8E0, hs_object_buckling_magnitude_get, object_index);

	real32 magnitude = 0.0f;
	if (object_index != NONE && TEST_BIT(_object_mask_giant, object_get_type(object_index)))
	{
		magnitude = giant_buckling_magnitude_get(object_index);
	}
	return magnitude;
}

void __cdecl hs_object_create(int16 object_name_index)
{
	//INVOKE(0x0096D940, hs_object_create, object_name_index);

	if (object_name_index != NONE)
	{
		hs_object_new_by_name_internal(object_name_index, false, false);
	}
}

void __cdecl hs_object_create_anew(int16 object_name_index)
{
	//INVOKE(0x0096D970, hs_object_create_anew, object_name_index);

	if (object_name_index != NONE)
	{
		int32 old_object_index = object_index_from_name_index(object_name_index);
		if (old_object_index != NONE)
		{
			hs_object_destroy(old_object_index);
		}
		hs_object_new_by_name_internal(object_name_index, false, false);
	}
}

void __cdecl hs_object_create_anew_containing(const char* name_string)
{
	//INVOKE(0x0096D9D0, hs_object_create_anew_containing, name_string);

	hs_object_iterate_names_containing(name_string, hs_object_create_anew);
}

void __cdecl hs_object_create_anew_multiplayer_cinematic(int16 object_name_index)
{
	//INVOKE(0x0096D9F0, hs_object_create_anew_multiplayer_cinematic, object_name_index);

	if (object_name_index != NONE)
	{
		int32 old_object_index = object_index_from_name_index(object_name_index);
		if (old_object_index != NONE)
		{
			hs_object_destroy_multiplayer_cinematic(old_object_index);
		}
		hs_object_new_by_name_internal(object_name_index, false, false);
	}
}

void __cdecl hs_object_create_clone(int16 object_name_index)
{
	//INVOKE(0x0096DA50, hs_object_create_clone, object_name_index);

	if (object_name_index != NONE)
	{
		hs_object_new_by_name_internal(object_name_index, true, false);
	}
}

void __cdecl hs_object_create_clone_containing(const char* name_string)
{
	//INVOKE(0x0096DA80, hs_object_create_clone_containing, name_string);

	hs_object_iterate_names_containing(name_string, hs_object_create_clone);
}

void __cdecl hs_object_create_clone_multiplayer_cinematic(int16 object_name_index)
{
	//INVOKE(0x0096DAA0, hs_object_create_clone_multiplayer_cinematic, object_name_index);

	if (object_name_index != NONE)
	{
		hs_object_new_by_name_internal(object_name_index, true, true);
	}
}

void __cdecl hs_object_create_containing(const char* name_string)
{
	//INVOKE(0x0096DAD0, hs_object_create_containing, name_string);

	hs_object_iterate_names_containing(name_string, hs_object_create);
}

void __cdecl hs_object_create_folder(int32 folder_index)
{
	INVOKE(0x0096DAF0, hs_object_create_folder, folder_index);

	//if (folder_index != NONE)
	//{
	//	struct scenario* scenario = global_scenario_get();
	//	hs_object_create_folder_internal(folder_index, &scenario->crates, &scenario->crate_palette, sizeof(s_scenario_crate));
	//	hs_object_create_folder_internal(folder_index, &scenario->scenery, &scenario->scenery_palette, sizeof(s_scenario_scenery));
	//	hs_object_create_folder_internal(folder_index, &scenario->bipeds, &scenario->biped_palette, sizeof(s_scenario_biped));
	//	hs_object_create_folder_internal(folder_index, &scenario->vehicles, &scenario->vehicle_palette, sizeof(s_scenario_vehicle));
	//	hs_object_create_folder_internal(folder_index, &scenario->equipment, &scenario->equipment_palette, sizeof(s_scenario_equipment));
	//	hs_object_create_folder_internal(folder_index, &scenario->weapons, &scenario->weapon_palette, sizeof(s_scenario_weapon));
	//	hs_object_create_folder_internal(folder_index, &scenario->machines, &scenario->machine_palette, sizeof(s_scenario_machine));
	//	hs_object_create_folder_internal(folder_index, &scenario->terminals, &scenario->terminal_palette, sizeof(s_scenario_terminal));
	//	hs_object_create_folder_internal(folder_index, &scenario->controls, &scenario->control_palette, sizeof(s_scenario_control));
	//	hs_object_create_folder_internal(folder_index, &scenario->sound_scenery, &scenario->sound_scenery_palette, sizeof(s_scenario_sound_scenery));
	//	hs_object_create_folder_internal(folder_index, &scenario->giants, &scenario->giant_palette, sizeof(s_scenario_giant));
	//	hs_object_create_folder_internal(folder_index, &scenario->effect_scenery, &scenario->effect_scenery_palette, sizeof(s_scenario_effect_scenery));
	//	hs_object_create_folder_internal(folder_index, &scenario->lights, &scenario->lights_palette, sizeof(s_scenario_light));
	//}
}

void __cdecl hs_object_create_folder_anew(int32 folder_index)
{
	//INVOKE(0x0096DC50, hs_object_create_folder_anew, folder_index);

	if (folder_index != NONE)
	{
		hs_object_destroy_folder(folder_index);
		hs_object_create_folder(folder_index);
	}
}

void __cdecl hs_object_create_folder_internal(int32 folder_index, const s_tag_block* block, s_tag_block* palette, int32 size)
{
	INVOKE(0x0096DC70, hs_object_create_folder_internal, folder_index, block, palette, size);
}

void __cdecl hs_object_create_if_necessary(int16 object_name_index)
{
	//INVOKE(0x0096DCE0, hs_object_create_if_necessary, object_name_index);

	if (object_name_index != NONE && object_index_from_name_index(object_name_index) == NONE)
	{
		hs_object_new_by_name_internal(object_name_index, false, false);
	}
}

void __cdecl hs_object_create_multiplayer_cinematic(int16 object_name_index)
{
	//INVOKE(0x0096DD20, hs_object_create_multiplayer_cinematic, object_name_index);

	if (object_name_index != NONE)
	{
		hs_object_new_by_name_internal(object_name_index, false, true);
	}
}

void __cdecl hs_object_delete_internal(int32 object_index)
{
	//INVOKE(0x0096DD50, hs_object_delete_internal, object_index);

	object_placement_mark_object_on_delete(object_index);
	object_delete(object_index);
}

void __cdecl hs_object_destroy(int32 object_index)
{
	//INVOKE(0x0096DD70, hs_object_destroy, object_index);

	if (object_index != NONE)
	{
		if (object_is_or_contains_player(object_index))
		{
			event(_event_error, "a script tried to delete the player (or the horse he rode in on, or his six-shooter)");
		}
		else
		{
			if (object_is_multiplayer_cinematic_object(object_index))
			{
				event(_event_error, "attempting to delete a cinematic object with a a non-cinematic hs command");
			}
			hs_object_delete_internal(object_index);
		}
	}
}

void __cdecl hs_object_destroy_all()
{
	//INVOKE(0x0096DDB0, hs_object_destroy_all);

	hs_object_destroy_all_type_mask(_object_mask_all);
}

void __cdecl hs_object_destroy_all_type_mask(int32 mask)
{
	//INVOKE(0x0096DDC0, hs_object_destroy_all_type_mask, mask);

	if (TEST_MASK(mask, _object_mask_vehicle))
	{
		c_data_iterator<player_datum> player_iterator;
		player_iterator.begin(player_data);
		while (player_iterator.next())
		{
			const player_datum* player = player_iterator.get_datum();
			if (player->unit_index != NONE && object_get_ultimate_parent(player->unit_index) != player->unit_index)
			{
				action_submit(player->unit_index, _action_vehicle_exit_immediate);
			}
		}
	}

	c_object_iterator<object_datum> iterator;
	iterator.begin(mask, _object_mask_none);
	while (iterator.next())
	{
		const object_datum* object = iterator.get_datum();
		if (object->object.parent_object_index == NONE && !object_is_or_contains_player(iterator.get_index()))
		{
			if (object_is_multiplayer_cinematic_object(iterator.get_index()))
			{
				event(_event_error, "a script is deleting a cinematic object (in destroy_all()) with a non-cinematic hs command");
			}
			hs_object_delete_internal(iterator.get_index());
		}
	}
}

void __cdecl hs_object_destroy_by_name(int16 object_name_index)
{
	//INVOKE(0x0096DEA0, hs_object_destroy_by_name, object_name_index);

	if (object_name_index != NONE)
	{
		int32 object_index = object_index_from_name_index(object_name_index);
		if (object_index != NONE)
		{
			hs_object_destroy(object_index);
		}
	}
}

void __cdecl hs_object_destroy_containing(const char* name_string)
{
	//INVOKE(0x0096DF00, hs_object_destroy_containing, name_string);

	hs_object_iterate_names_containing(name_string, hs_object_destroy_by_name);
}

void __cdecl hs_object_destroy_folder(int32 folder_index)
{
	INVOKE(0x0096DF20, hs_object_destroy_folder, folder_index);

	//if (folder_index != NONE)
	//{
	//	struct scenario* scenario = global_scenario_get();
	//	hs_object_destroy_folder_internal(folder_index, &scenario->crates, &scenario->crate_palette, sizeof(s_scenario_crate));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->scenery, &scenario->scenery_palette, sizeof(s_scenario_scenery));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->bipeds, &scenario->biped_palette, sizeof(s_scenario_biped));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->vehicles, &scenario->vehicle_palette, sizeof(s_scenario_vehicle));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->equipment, &scenario->equipment_palette, sizeof(s_scenario_equipment));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->weapons, &scenario->weapon_palette, sizeof(s_scenario_weapon));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->machines, &scenario->machine_palette, sizeof(s_scenario_machine));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->terminals, &scenario->terminal_palette, sizeof(s_scenario_terminal));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->controls, &scenario->control_palette, sizeof(s_scenario_control));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->sound_scenery, &scenario->sound_scenery_palette, sizeof(s_scenario_sound_scenery));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->giants, &scenario->giant_palette, sizeof(s_scenario_giant));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->effect_scenery, &scenario->effect_scenery_palette, sizeof(s_scenario_effect_scenery));
	//	hs_object_destroy_folder_internal(folder_index, &scenario->lights, &scenario->lights_palette, sizeof(s_scenario_light));
	//}
}

void __cdecl hs_object_destroy_folder_internal(int32 folder_index, const s_tag_block* block, s_tag_block* palette, int32 size)
{
	INVOKE(0x0096E080, hs_object_destroy_folder_internal, folder_index, block, palette, size);
}

void __cdecl hs_object_destroy_multiplayer_cinematic(int32 object_index)
{
	//INVOKE(0x0096E1F0, hs_object_destroy_multiplayer_cinematic, object_index);

	if (object_index != NONE)
	{
		if (object_is_or_contains_player(object_index))
		{
			event(_event_error, "a cinematic script tried to delete the player (or the horse he rode in on, or his six-shooter)");
		}
		else
		{
			if (!object_is_multiplayer_cinematic_object(object_index))
			{
				event(_event_error, "a cinematic script deleting a non-cinematic object");
			}
			hs_object_delete_internal(object_index);
		}
	}
}

void __cdecl hs_object_hide(int32 object_index, bool hide)
{
	//INVOKE(0x0096E230, hs_object_hide, object_index, hide);

	if (object_index != NONE)
	{
		object_set_hidden(object_index, hide);
	}
}

void __cdecl hs_object_iterate_names_containing(const char* name_string, void(__cdecl* iterator)(int16))
{
	//INVOKE(0x0096E250, hs_object_iterate_names_containing, name_string, iterator);

	const struct scenario* scenario = global_scenario_get();
	for (int16 object_name_index = 0; object_name_index < global_scenario->object_names.count; object_name_index++)
	{
		scenario_object_name* object_name = TAG_BLOCK_GET_ELEMENT(&global_scenario->object_names, object_name_index, scenario_object_name);
		if (object_name->name.contains(name_string))
		{
			iterator(object_name_index);
		}
	}
}

int32 __cdecl hs_object_list_get_element(int32 object_list_index, int16 element_index)
{
	return INVOKE(0x0096E2B0, hs_object_list_get_element, object_list_index, element_index);

#if 0 // $REVIEW
	int32 reference_index;
	int32 object_index = object_list_get_first(object_list_index, &reference_index);
	while (element_index > 0)
	{
		if (object_index != NONE)
		{
			break;
		}

		object_index = object_list_get_next(object_list_index, &reference_index);
		element_index--;
	}
	return object_index;
#endif
}

void __cdecl hs_object_new_by_name_internal(int16 object_name_index, bool displace_previous_object, bool multiplayer_cinematic_object)
{
	//INVOKE(0x0096E2F0, hs_object_new_by_name_internal, object_name_index, displace_previous_object, multiplayer_cinematic_object);

	object_placement_mark_name_on_create(object_name_index);
	object_new_by_name(object_name_index, displace_previous_object, multiplayer_cinematic_object);
}

void __cdecl hs_object_orient(int32 object_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, bool set_position, bool set_facing)
{
	INVOKE(0x0096E320, hs_object_orient, object_index, position, forward, up, set_position, set_facing);

#if 0 // $REVIEW
	if (object_index != NONE)
	{
		const object_datum* object = OBJECT_GET(const object_datum, object_index);
		if (set_position)
		{
			if (object->object.parent_object_index != NONE)
			{
				object_detach(object_index);
			}
			else if (TEST_FLAG(object->object.flags, _object_in_limbo_bit))
			{
				object_set_in_limbo(object_index, false);
			}

			ASSERT(object->object.parent_object_index == NONE);
			ASSERT(!TEST_FLAG(object->object.flags, _object_in_limbo_bit));
		}

		object_reset(object_index);
		object_placement_reset_from_teleport(object_index);

		unit_datum* unit = UNIT_GET(object_index);
		if (unit)
		{
			real_vector3d relative_forward{};
			if (unit->object.parent_object_index == NONE)
			{
				relative_forward = *forward;
			}
			else
			{
				real_matrix4x3 inverted_matrix{};
				matrix4x3_inverse(object_get_node_matrix(unit->object.parent_object_index, unit->object.parent_node_index), &inverted_matrix);
				matrix4x3_transform_normal(&inverted_matrix, forward, &relative_forward);
			}

			if (set_facing)
			{
				unit->unit.desired_facing_vector = *forward;
				unit->unit.desired_aiming_vector = *forward;
				unit->unit.desired_looking_vector = *forward;
				simulation_action_object_update(object_index, _simulation_unit_update_desired_aiming_vector);
			}

			int32 player_index = player_index_from_unit_index(object_index);
			if (player_index != NONE)
			{
				if (set_position)
				{
					player_teleport(player_index, NONE, position);
					set_position = false;
				}

				if (set_facing)
				{
					player_set_facing(player_index, &relative_forward);
				}
			}
		}

		object_set_position(object_index, set_position ? position : NULL, set_facing ? forward : NULL, set_facing ? up : NULL, NULL);
	}
#endif
}

void __cdecl hs_object_set_facing(int32 object_index, int16 flag_index)
{
	//INVOKE(0x0096E4D0, hs_object_set_facing, object_index, flag_index);

	scenario_cutscene_flag* flag = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->cutscene_flags, flag_index, scenario_cutscene_flag);

	real_vector3d forward{};
	real_vector3d up_vector{};
	vector3d_from_euler_angles2d(&forward, &flag->facing);
	generate_up_vector3d(&forward, &up_vector);
	hs_object_orient(object_index, &flag->position, &forward, &up_vector, false, true);
}

void __cdecl hs_object_set_permutation(int32 object_index, int32 region_name_id, int32 permutation_name_id)
{
	//INVOKE(0x0096E620, hs_object_set_permutation, object_index, region_name_id, permutation_name_id);

	if (object_index != NONE)
	{
		object_set_region_permutation_direct(object_index, region_name_id, permutation_name_id);
	}
}

void __cdecl hs_object_set_region_state(int32 object_index, int32 region_name_id, int32 model_state)
{
	INVOKE(0x0096E640, hs_object_set_region_state, object_index, region_name_id, model_state);
}

void __cdecl hs_object_set_shadowless(int32 object_index, bool shadowless)
{
	//INVOKE(0x0096E6B0, hs_object_set_shadowless, object_index, shadowless);

	if (object_index != NONE)
	{
		object_set_shadowless(object_get_ultimate_parent(object_index), shadowless);
	}
}

void __cdecl hs_object_set_shield(int32 object_index, real32 shield)
{
	INVOKE(0x0096E6E0, hs_object_set_shield, object_index, shield);
}

void __cdecl hs_object_set_shield_normalized(int32 object_index, real32 shield)
{
	INVOKE(0x0096E740, hs_object_set_shield_normalized, object_index, shield);
}

void __cdecl hs_object_set_shield_stun(int32 object_index, real32 shield)
{
	INVOKE(0x0096E7A0, hs_object_set_shield_stun, object_index, shield);
}

void __cdecl hs_object_set_shield_stun_infinite(int32 object_index)
{
	INVOKE(0x0096E7D0, hs_object_set_shield_stun_infinite, object_index);
}

void __cdecl hs_object_set_variant(int32 object_index, int32 variant_name)
{
	//INVOKE(0x0096E7F0, hs_object_set_variant, object_index, variant_name);

	if (object_index != NONE)
	{
		object_set_variant_direct(object_index, variant_name);
	}
}

void __cdecl hs_object_teleport(int32 object_index, int16 flag_index)
{
	//INVOKE(0x0096E810, hs_object_teleport, object_index, flag_index);

	scenario_cutscene_flag* flag = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->cutscene_flags, flag_index, scenario_cutscene_flag);
	
	real_vector3d forward{};
	real_vector3d up_vector{};
	vector3d_from_euler_angles2d(&forward, &flag->facing);
	generate_up_vector3d(&forward, &up_vector);
	hs_object_orient(object_index, &flag->position, &forward, &up_vector, true, true);
}

void __cdecl hs_object_teleport_ai(int32 object_index, int32 ai_point_ref)
{
	//INVOKE(0x0096E870, hs_object_teleport_ai, object_index, ai_point_ref);

	real_point3d position{};
	real_vector3d forward{};
	real_vector3d up{};
	if (point_ref_get_position_and_orientation(ai_point_ref, &position, &forward, &up))
	{
		hs_object_orient(object_index, &position, &forward, &up, true, true);
	}
}

bool __cdecl hs_objects_can_see_flag(int32 object_list_index, int16 flag_index, real32 degrees)
{
	//return INVOKE(0x0096E8B0, hs_objects_can_see_flag, object_list_index, flag_index, degrees);

	bool visible = false;
	int32 reference_index;
	for (int32 unit_index = object_list_get_first(object_list_index, &reference_index);
		unit_index != NONE;
		unit_index = object_list_get_next(object_list_index, &reference_index))
	{
		if (hs_unit_can_see_flag(unit_index, flag_index, degrees))
		{
			visible = true;
			break;
		}
	}
	return visible;
}

bool __cdecl hs_objects_can_see_object(int32 object_list_index, int32 object_index, real32 degrees)
{
	//return INVOKE(0x0096E970, hs_objects_can_see_object, object_list_index, object_index, degrees);

	bool visible = false;
	int32 reference_index;
	for (int32 unit_index = object_list_get_first(object_list_index, &reference_index);
		unit_index != NONE;
		unit_index = object_list_get_next(object_list_index, &reference_index))
	{
		if (hs_unit_can_see_object(unit_index, object_index, degrees))
		{
			visible = true;
			break;
		}
	}
	return visible;
}

void __cdecl hs_objects_delete_by_definition(int32 definition_index)
{
	//INVOKE(0x0096EA70, hs_objects_delete_by_definition, definition_index);

	c_object_iterator<object_datum> iterator;
	iterator.begin(_object_mask_all, _object_mask_none);
	while (iterator.next())
	{
		const object_datum* object = iterator.get_datum();
		if (object->definition_index == definition_index)
		{
			hs_object_delete_internal(iterator.get_index());
		}
	}
	objects_memory_compact();
}

real32 __cdecl hs_objects_distance_to_flag(int32 object_list_index, int16 flag_index)
{
	//return INVOKE(0x0096EAD0, hs_objects_distance_to_flag, object_list_index, flag_index);

	real32 minimum_distance = k_real_max;
	if (VALID_INDEX(flag_index, global_scenario_get()->cutscene_flags.count))
	{
		const real_point3d* flag_position = &TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->cutscene_flags, flag_index, scenario_cutscene_flag)->position;

		int32 reference_index;
		for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
			object_index != NONE;
			object_index = object_list_get_next(object_list_index, &reference_index))
		{
			real_point3d object_origin{};
			object_get_origin(object_index, &object_origin);
			real32 distance = distance3d(flag_position, &object_origin);
			if (minimum_distance > distance)
			{
				minimum_distance = distance;
			}
		}
	}
	if (minimum_distance == k_real_max)
	{
		minimum_distance = -1.0;
	}
	return minimum_distance;
}

real32 __cdecl hs_objects_distance_to_object(int32 object_list_index, int32 target_object_index)
{
	//return INVOKE(0x0096EBE0, hs_objects_distance_to_object, object_list_index, target_object_index);

	real32 minimum_distance = k_real_max;
	if (target_object_index != NONE)
	{
		real_point3d target_object_origin{};
		object_get_origin(target_object_index, &target_object_origin);

		int32 reference_index;
		for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
			object_index != NONE;
			object_index = object_list_get_next(object_list_index, &reference_index))
		{
			real_point3d object_origin{};
			object_get_origin(object_index, &object_origin);
			real32 distance = distance3d(&target_object_origin, &object_origin);
			if (minimum_distance > distance)
			{
				minimum_distance = distance;
			}
		}
	}
	if (minimum_distance == k_real_max)
	{
		minimum_distance = -1.0;
	}
	return minimum_distance;
}

void __cdecl hs_objects_predict(int32 object_list_index, bool low_detail)
{
	//INVOKE(0x0096ECE0, hs_objects_predict, object_list_index, low_detail);

	int32 reference_index;
	for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
		object_index != NONE;
		object_index = object_list_get_next(object_list_index, &reference_index))
	{
		if (low_detail)
		{
			object_predict_low(object_index);
		}
		else
		{
			object_predict_all(object_index);
		}
	}
}

void __cdecl hs_objects_predict_high(int32 object_list_index)
{
	//INVOKE(0x0096ED30, hs_objects_predict_high, object_list_index);

	hs_objects_predict(object_list_index, false);
}

void __cdecl hs_objects_predict_low(int32 object_list_index)
{
	//INVOKE(0x0096ED70, hs_objects_predict_low, object_list_index);

	hs_objects_predict(object_list_index, true);
}

void __cdecl hs_objects_predict_old(int32 object_list_index)
{
	//INVOKE(0x0096EDB0, hs_objects_predict_old, object_list_index);

	hs_objects_predict_high(object_list_index);
}

real32 __cdecl hs_pin(real32 value, real32 min, real32 max)
{
	//return INVOKE(0x0096EDF0, hs_pin, value, min, max);

	return PIN(value, min, max);
}

#if 0
int32 player_get_player_index_from_contiguous_index(int32 contiguous_player_index)
{
	int32 result = NONE;
	{
		ASSERT(VALID_INDEX(contiguous_player_index, k_maximum_campaign_or_survival_players));

		c_data_iterator<player_datum> player_iterator;
		player_iterator.begin(player_data);
		for (int32 player_index = 0; player_iterator.next(); player_index++)
		{
			if (contiguous_player_index == player_index)
			{
				result = player_iterator.get_index();
				break;
			}
		}
	}
	return result;
}
#endif

int32 __cdecl hs_player_get(int32 contiguous_player_index)
{
	return INVOKE(0x0096EE40, hs_player_get, contiguous_player_index);

#if 0
	int32 unit_index = NONE;
	{
		int32 player_index = player_get_player_index_from_contiguous_index(contiguous_player_index);
		if (player_index != NONE)
		{
			const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
			unit_index = player->unit_index;
		}
	}
	return unit_index;
#endif
}

int32 __cdecl hs_players()
{
	return INVOKE(0x0096EE90, hs_players);

#if 0
	int32 player_list_index = object_list_new();
	if (player_list_index != NONE)
	{
		int32 unit_indices[k_maximum_players]{};

		int16 player_count = 0;
		for (int32 player_index = data_next_index(player_data, NONE);
			player_index != NONE;
			player_index = data_next_index(player_data, player_index))
		{
			const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
			if (player->unit_index != NONE)
			{
				ASSERT(player_count < k_maximum_players);
				unit_indices[player_count++] = player->unit_index;
			}
		}

		while (--player_count >= 0)
		{
			object_list_add(player_list_index, unit_indices[player_count]);
		}
	}
	return player_list_index;
#endif
}

void __cdecl hs_position_predict(real32 x, real32 y, real32 z)
{
	//INVOKE(0x0096EF50, hs_position_predict, x, y, z);
}

void __cdecl hs_print(const char* s)
{
	//INVOKE(0x0096EF60, hs_print, s);

	terminal_printf(global_real_argb_green, s);
}

void __cdecl hs_shader_predict(int32 shader_index)
{
	//INVOKE(0x0096EF70, hs_shader_predict, shader_index);
}

void __cdecl hs_tag_load_force_programmer_only(const char* tag_path_plus_extension)
{
	//INVOKE(0x0096EF80, hs_tag_load_force_programmer_only, tag_path_plus_extension);
}

void __cdecl hs_tag_unload_force_programmer_only(const char* tag_path_plus_extension)
{
	//INVOKE(0x0096EF90, hs_tag_unload_force_programmer_only, tag_path_plus_extension);
}

void __cdecl hs_teleport_players_not_in_trigger_volume(int16 trigger_volume_index, int16 flag_index)
{
	//INVOKE(0x0096EFA0, hs_teleport_players_not_in_trigger_volume, trigger_volume_index, flag_index);

	for (int32 player_index = data_next_index(player_data, NONE);
		player_index != NONE;
		player_index = data_next_index(player_data, player_index))
	{
		const player_datum* player = DATUM_GET(player_data, player_datum, player_index);
		if (player->unit_index != NONE && !scenario_trigger_volume_test_object(trigger_volume_index, player->unit_index))
		{
			hs_object_teleport(player->unit_index, flag_index);
		}
	}
}

bool __cdecl hs_trigger_volume_test_objects(int16 trigger_volume_index, int32 object_list_index, bool and_)
{
	//return INVOKE(0x0096F080, hs_trigger_volume_test_objects, trigger_volume_index, object_list_index, and_);

	bool test = and_;
	int32 reference_index;
	for (int32 object_index = object_list_get_first(object_list_index, &reference_index);
		object_index != NONE;
		object_index = object_list_get_next(object_list_index, &reference_index))
	{
		if (scenario_trigger_volume_test_object(trigger_volume_index, object_index))
		{
			if (!and_)
			{
				test = true;
				break;
			}
		}
		else if (and_)
		{
			test = false;
			break;
		}
	}

	hs_debug_data.activated_trigger_volumes.set(trigger_volume_index, test);

	return test;
}

bool __cdecl hs_trigger_volume_test_objects_all(int16 trigger_volume_index, int32 object_index)
{
	//return INVOKE(0x0096F0F0, hs_trigger_volume_test_objects_all, trigger_volume_index, object_index);

	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, true);
}

bool __cdecl hs_trigger_volume_test_objects_any(int16 trigger_volume_index, int32 object_index)
{
	//return INVOKE(0x0096F150, hs_trigger_volume_test_objects_any, trigger_volume_index, object_index);

	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, false);
}

bool __cdecl hs_unit_can_see_flag(int32 unit_index, int16 flag_index, real32 degrees)
{
	//return INVOKE(0x0096F1B0, hs_unit_can_see_flag, unit_index, flag_index, degrees);

	bool visible = false;
	if (VALID_INDEX(flag_index, global_scenario_get()->cutscene_flags.count))
	{
		if (unit_can_see_point(unit_index, &TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->cutscene_flags, flag_index, scenario_cutscene_flag)->position, degrees * DEG))
		{
			visible = true;
		}
	}
	return visible;
}

bool __cdecl hs_unit_can_see_object(int32 unit_index, int32 object_index, real32 degrees)
{
	//return INVOKE(0x0096F210, hs_unit_can_see_object, unit_index, object_index, degrees);

	bool visible = false;
	if (object_index != NONE)
	{
		real_point3d target_point{};
		if (UNIT_GET(object_index))
		{
			unit_get_head_position(object_index, &target_point);
		}
		else
		{
			target_point = OBJECT_GET(object_datum, object_index)->object.bounding_sphere_center;
		}

		if (unit_can_see_point(unit_index, &target_point, degrees * DEG))
		{
			visible = true;
		}
	}
	return visible;
}

int32 __cdecl hs_user_interface_controller_get_last_level_played(int16 controller)
{
	//return INVOKE(0x0096F2A0, hs_user_interface_controller_get_last_level_played, controller);

	event(_event_critical, "hs: user_interface_controller_get_last_level_played is not deterministic. if you need to use this, talk to Luke or Petar");
	return NONE;
}

