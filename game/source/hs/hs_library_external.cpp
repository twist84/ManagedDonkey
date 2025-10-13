#include "hs/hs_library_external.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "hs/hs.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/object_lists.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"

void __cdecl hs_evaluate_library_external(int16 script_index);

HOOK_DECLARE(0x0096D3E0, hs_debug_variable);
HOOK_DECLARE(0x0096D870, hs_evaluate_library_external);
//HOOK_DECLARE(0x0096D8B0, hs_log_print);
//HOOK_DECLARE(0x0096EF60, hs_print);
HOOK_DECLARE(0x0096F0F0, hs_trigger_volume_test_objects_all);
HOOK_DECLARE(0x0096F150, hs_trigger_volume_test_objects_any);

void __cdecl hs_evaluate_library_external(int16 script_index)
{
	hs_evaluate(script_index);
}

void __cdecl hs_debug_variable(const char* s, bool debug)
{
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

//.text:0096D8A0 ; 

void __cdecl hs_log_print(const char* s)
{
	//INVOKE(0x0096D8B0, hs_log_print, s);

	event(_event_warning, "hs: %s", s);
}

//.text:0096D8C0 ; void __cdecl hs_map_info()

bool __cdecl hs_not(bool value)
{
	return INVOKE(0x0096D8D0, hs_not, value);
}

//.text:0096D8E0 ; real32 __cdecl hs_object_buckling_magnitude_get(int32 object_index)

void __cdecl hs_object_create(int16 object_name_index)
{
	INVOKE(0x0096D940, hs_object_create, object_name_index);
}

void __cdecl hs_object_create_anew(int16 object_name_index)
{
	INVOKE(0x0096D970, hs_object_create_anew, object_name_index);
}

void __cdecl hs_object_create_anew_containing(const char* name_string)
{
	INVOKE(0x0096D9D0, hs_object_create_anew_containing, name_string);
}

void __cdecl hs_object_create_anew_multiplayer_cinematic(int16 object_name_index)
{
	INVOKE(0x0096D9F0, hs_object_create_anew_multiplayer_cinematic, object_name_index);
}

void __cdecl hs_object_create_clone(int16 object_name_index)
{
	INVOKE(0x0096DA50, hs_object_create_clone, object_name_index);
}

void __cdecl hs_object_create_clone_containing(const char* name_string)
{
	INVOKE(0x0096DA80, hs_object_create_clone_containing, name_string);
}

void __cdecl hs_object_create_clone_multiplayer_cinematic(int16 object_name_index)
{
	INVOKE(0x0096DAA0, hs_object_create_clone_multiplayer_cinematic, object_name_index);
}

void __cdecl hs_object_create_containing(const char* name_string)
{
	INVOKE(0x0096DAD0, hs_object_create_containing, name_string);
}

void __cdecl hs_object_create_folder(int32 folder_index)
{
	INVOKE(0x0096DAF0, hs_object_create_folder, folder_index);
}

void __cdecl hs_object_create_folder_anew(int32 folder_index)
{
	INVOKE(0x0096DC50, hs_object_create_folder_anew, folder_index);
}

void __cdecl hs_object_create_folder_internal(int32 folder_index, const s_tag_block* block, s_tag_block* palette, int32 size)
{
	INVOKE(0x0096DC70, hs_object_create_folder_internal, folder_index, block, palette, size);
}

void __cdecl hs_object_create_if_necessary(int16 object_name_index)
{
	INVOKE(0x0096DCE0, hs_object_create_if_necessary, object_name_index);
}

void __cdecl hs_object_create_multiplayer_cinematic(int16 object_name_index)
{
	INVOKE(0x0096DD20, hs_object_create_multiplayer_cinematic, object_name_index);
}

void __cdecl hs_object_delete_internal(int32 object_index)
{
	INVOKE(0x0096DD50, hs_object_delete_internal, object_index);
}

void __cdecl hs_object_destroy(int32 object_index)
{
	INVOKE(0x0096DD70, hs_object_destroy, object_index);
}

void __cdecl hs_object_destroy_all()
{
	INVOKE(0x0096DDB0, hs_object_destroy_all);
}

void __cdecl hs_object_destroy_all_type_mask(int32 mask)
{
	INVOKE(0x0096DDC0, hs_object_destroy_all_type_mask, mask);
}

void __cdecl hs_object_destroy_by_name(int16 object_name_index)
{
	INVOKE(0x0096DEA0, hs_object_destroy_by_name, object_name_index);
}

void __cdecl hs_object_destroy_containing(const char* name_string)
{
	INVOKE(0x0096DF00, hs_object_destroy_containing, name_string);
}

void __cdecl hs_object_destroy_folder(int32 folder_index)
{
	INVOKE(0x0096DF20, hs_object_destroy_folder, folder_index);
}

void __cdecl hs_object_destroy_folder_internal(int32 folder_index, const s_tag_block* block, s_tag_block* palette, int32 size)
{
	INVOKE(0x0096E080, hs_object_destroy_folder_internal, folder_index, block, palette, size);
}

void __cdecl hs_object_destroy_multiplayer_cinematic(int32 object_index)
{
	INVOKE(0x0096E1F0, hs_object_destroy_multiplayer_cinematic, object_index);
}

void __cdecl hs_object_hide(int32 object_index, bool hide)
{
	INVOKE(0x0096E230, hs_object_hide, object_index, hide);
}

void __cdecl hs_object_iterate_names_containing(const char* name_string, void(__cdecl* iterator)(int16))
{
	INVOKE(0x0096E250, hs_object_iterate_names_containing, name_string, iterator);
}

int32 __cdecl hs_object_list_get_element(int32 object_list_index, int16 element_index)
{
	return INVOKE(0x0096E2B0, hs_object_list_get_element, object_list_index, element_index);
}

void __cdecl hs_object_new_by_name_internal(int16 object_name_index, bool displace_previous_object, bool multiplayer_cinematic_object)
{
	INVOKE(0x0096E2F0, hs_object_new_by_name_internal, object_name_index, displace_previous_object, multiplayer_cinematic_object);
}

void __cdecl hs_object_orient(int32 object_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, bool set_position, bool set_facing)
{
	INVOKE(0x0096E320, hs_object_orient, object_index, position, forward, up, set_position, set_facing);
}

void __cdecl hs_object_set_facing(int32 object_index, int16 flag_index)
{
	INVOKE(0x0096E4D0, hs_object_set_facing, object_index, flag_index);
}

void __cdecl hs_object_set_permutation(int32 object_index, int32 region_name_id, int32 permutation_name_id)
{
	INVOKE(0x0096E620, hs_object_set_permutation, object_index, region_name_id, permutation_name_id);
}

void __cdecl hs_object_set_region_state(int32 object_index, int32 region_name_id, int32 model_state)
{
	INVOKE(0x0096E640, hs_object_set_region_state, object_index, region_name_id, model_state);
}

void __cdecl hs_object_set_shadowless(int32 object_index, bool shadowless)
{
	INVOKE(0x0096E6B0, hs_object_set_shadowless, object_index, shadowless);
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
	INVOKE(0x0096E7F0, hs_object_set_variant, object_index, variant_name);
}

void __cdecl hs_object_teleport(int32 object_index, int32 flag_index)
{
	INVOKE(0x0096E810, hs_object_teleport, object_index, flag_index);
}

void __cdecl hs_object_teleport_ai(int32 object_index, int32 ai_point_ref)
{
	INVOKE(0x0096E870, hs_object_teleport_ai, object_index, ai_point_ref);
}

bool __cdecl hs_objects_can_see_flag(int32 object_list_index, int16 flag_index, real32 degrees)
{
	return INVOKE(0x0096E8B0, hs_objects_can_see_flag, object_list_index, flag_index, degrees);
}

bool __cdecl hs_objects_can_see_object(int32 object_list_index, int32 object_index, real32 degrees)
{
	return INVOKE(0x0096E970, hs_objects_can_see_object, object_list_index, object_index, degrees);
}

void __cdecl hs_objects_delete_by_definition(int32 definition_index)
{
	INVOKE(0x0096EA70, hs_objects_delete_by_definition, definition_index);
}

//.text:0096EAD0 ; real32 __cdecl hs_objects_distance_to_flag(int32 object_list_index, int16 flag_index)
//.text:0096EBE0 ; real32 __cdecl hs_objects_distance_to_object(int32 object_list_index, int32 target_object_index)

void __cdecl hs_objects_predict(int32 object_list_index, bool low_detail)
{
	INVOKE(0x0096ECE0, hs_objects_predict, object_list_index, low_detail);
}

void __cdecl hs_objects_predict_high(int32 object_list_index)
{
	INVOKE(0x0096ED30, hs_objects_predict_high, object_list_index);
}

void __cdecl hs_objects_predict_low(int32 object_list_index)
{
	INVOKE(0x0096ED70, hs_objects_predict_low, object_list_index);
}

void __cdecl hs_objects_predict_old(int32 object_list_index)
{
	INVOKE(0x0096EDB0, hs_objects_predict_old, object_list_index);
}

//.text:0096EDF0 ; real32 __cdecl hs_pin(real32 value, real32 min, real32 max)
//.text:0096EE40 ; int32 __cdecl hs_player_get(int32)

int32 __cdecl hs_players()
{
	return INVOKE(0x0096EE90, hs_players);
}

void __cdecl hs_position_predict(real32 x, real32 y, real32 z)
{
	INVOKE(0x0096EF50, hs_position_predict, x, y, z);
}

void __cdecl hs_print(const char* s)
{
	//INVOKE(0x0096EF60, hs_print, s);

	terminal_printf(global_real_argb_green, s);
}

void __cdecl hs_shader_predict(int32 shader_index)
{
	INVOKE(0x0096EF70, hs_shader_predict, shader_index);
}

void __cdecl hs_tag_load_force_programmer_only(const char* tag_path_plus_extension)
{
	INVOKE(0x0096EF80, hs_tag_load_force_programmer_only, tag_path_plus_extension);
}

void __cdecl hs_tag_unload_force_programmer_only(const char* tag_path_plus_extension)
{
	INVOKE(0x0096EF90, hs_tag_unload_force_programmer_only, tag_path_plus_extension);
}

void __cdecl hs_teleport_players_not_in_trigger_volume(int16 trigger_volume_index, int16 flag_index)
{
	INVOKE(0x0096EFA0, hs_teleport_players_not_in_trigger_volume, trigger_volume_index, flag_index);
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
	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, true);
}

bool __cdecl hs_trigger_volume_test_objects_any(int16 trigger_volume_index, int32 object_index)
{
	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, false);
}

bool __cdecl hs_unit_can_see_flag(int32 unit_index, int16 flag_index, real32 degrees)
{
	return INVOKE(0x0096F1B0, hs_unit_can_see_flag, unit_index, flag_index, degrees);
}

bool __cdecl hs_unit_can_see_object(int32 unit_index, int32 object_index, real32 degrees)
{
	return INVOKE(0x0096F210, hs_unit_can_see_object, unit_index, object_index, degrees);
}

int32 __cdecl hs_user_interface_controller_get_last_level_played(int16 controller)
{
	return INVOKE(0x0096F2A0, hs_user_interface_controller_get_last_level_played, controller);
}

