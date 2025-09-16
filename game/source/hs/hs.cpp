#include "hs/hs.hpp"

#include "ai/ai_orders.hpp"
#include "ai/behavior.hpp"
#include "ai/cl_engine.hpp"
#include "algorithms/qsort.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "memory/data.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "shell/shell.hpp"

//REFERENCE_DECLARE(0x0166D5E8, bool const, _hs_type_boolean_default);
//REFERENCE_DECLARE(0x0166D5EC, real32 const, _hs_type_real_default);
//REFERENCE_DECLARE(0x0166D5F0, int16 const, _hs_type_short_integer_default);
//REFERENCE_DECLARE(0x0166D5F4, int32 const, _hs_type_long_integer_default);
//REFERENCE_DECLARE(0x0166D5F8, int16 const, _hs_type_script_default);
//REFERENCE_DECLARE(0x0166D5FC, int32 const, _hs_type_string_id_default);
//REFERENCE_DECLARE(0x0166D600, int32 const, _hs_type_unit_seat_mapping_default);
//REFERENCE_DECLARE(0x0166D604, int16 const, _hs_type_trigger_volume_default);
//REFERENCE_DECLARE(0x0166D608, int16 const, _hs_type_cutscene_flag_default);
//REFERENCE_DECLARE(0x0166D60C, int16 const, _hs_type_cutscene_camera_point_default);
//REFERENCE_DECLARE(0x0166D610, int16 const, _hs_type_cutscene_title_default);
//REFERENCE_DECLARE(0x0166D614, int16 const, _hs_type_cutscene_recording_default);
//REFERENCE_DECLARE(0x0166D618, int32 const, _hs_type_device_group_default);
//REFERENCE_DECLARE(0x0166D61C, int32 const, _hs_type_ai_default);
//REFERENCE_DECLARE(0x0166D620, int16 const, _hs_type_ai_command_list_default);
//REFERENCE_DECLARE(0x0166D624, int16 const, _hs_type_ai_command_script_default);
//REFERENCE_DECLARE(0x0166D628, int16 const, _hs_type_ai_behavior_default);
//REFERENCE_DECLARE(0x0166D62C, int16 const, _hs_type_ai_orders_default);
//REFERENCE_DECLARE(0x0166D630, int32 const, _hs_type_ai_line_default);
//REFERENCE_DECLARE(0x0166D634, int16 const, _hs_type_starting_profile_default);
//REFERENCE_DECLARE(0x0166D638, int16 const, _hs_type_conversation_default);
//REFERENCE_DECLARE(0x0166D63C, int16 const, _hs_type_zone_set_default);
//REFERENCE_DECLARE(0x0166D640, int16 const, _hs_type_designer_zone_default);
//REFERENCE_DECLARE(0x0166D644, int32 const, _hs_type_point_ref_default);
//REFERENCE_DECLARE(0x0166D648, int32 const, _hs_type_style_default);
//REFERENCE_DECLARE(0x0166D64C, int32 const, _hs_type_object_list_default);
//REFERENCE_DECLARE(0x0166D650, int32 const, _hs_type_folder_default);
//REFERENCE_DECLARE(0x0166D654, int32 const, _hs_type_sound_default);
//REFERENCE_DECLARE(0x0166D658, int32 const, _hs_type_looping_sound_default);
//REFERENCE_DECLARE(0x0166D65C, int32 const, _hs_type_effect_default);
//REFERENCE_DECLARE(0x0166D660, int32 const, _hs_type_damage_default);
//REFERENCE_DECLARE(0x0166D664, int32 const, _hs_type_animation_graph_default);
//REFERENCE_DECLARE(0x0166D668, int32 const, _hs_type_damage_effect_default);
//REFERENCE_DECLARE(0x0166D66C, int32 const, _hs_type_object_definition_default);
//REFERENCE_DECLARE(0x0166D670, int32 const, _hs_type_bitmap_default);
//REFERENCE_DECLARE(0x0166D674, int32 const, _hs_type_shader_default);
//REFERENCE_DECLARE(0x0166D678, int32 const, _hs_type_render_model_definition_default);
//REFERENCE_DECLARE(0x0166D67C, int32 const, _hs_type_structure_bsp_definition_default);
//REFERENCE_DECLARE(0x0166D680, int32 const, _hs_type_structure_lightmap_definition_default);
//REFERENCE_DECLARE(0x0166D684, int32 const, _hs_type_cinematic_definition_default);
//REFERENCE_DECLARE(0x0166D688, int32 const, _hs_type_cinematic_scene_definition_default);
//REFERENCE_DECLARE(0x0166D68C, int32 const, _hs_type_bink_definition_default);
//REFERENCE_DECLARE(0x0166D690, int32 const, _hs_type_any_tag_default);
//REFERENCE_DECLARE(0x0166D694, int16 const, _hs_type_enum_game_difficulty_default);
//REFERENCE_DECLARE(0x0166D698, int16 const, _hs_type_enum_team_default);
//REFERENCE_DECLARE(0x0166D69C, int16 const, _hs_type_enum_mp_team_default);
//REFERENCE_DECLARE(0x0166D6A0, int16 const, _hs_type_enum_controller_default);
//REFERENCE_DECLARE(0x0166D6A4, int16 const, _hs_type_enum_button_preset_default);
//REFERENCE_DECLARE(0x0166D6A8, int16 const, _hs_type_enum_joystick_preset_default);
//REFERENCE_DECLARE(0x0166D6AC, int16 const, _hs_type_enum_player_character_type_default);
//REFERENCE_DECLARE(0x0166D6B0, int16 const, _hs_type_enum_voice_output_setting_default);
//REFERENCE_DECLARE(0x0166D6B4, int16 const, _hs_type_enum_subtitle_setting_default);
//REFERENCE_DECLARE(0x0166D6B8, int16 const, _hs_type_enum_actor_type_default);
//REFERENCE_DECLARE(0x0166D6BC, int16 const, _hs_type_enum_model_state_default);
//REFERENCE_DECLARE(0x0166D6C0, int16 const, _hs_type_enum_event_default);
//REFERENCE_DECLARE(0x0166D6C8, int16 const, _hs_type_enum_character_physics_override_default);
//REFERENCE_DECLARE(0x0166D6CC, int16 const, _hs_type_enum_primary_skull_default);
//REFERENCE_DECLARE(0x0166D6D0, int16 const, _hs_type_enum_secondary_skull_default);
//REFERENCE_DECLARE(0x0166D6D4, int16 const, _hs_type_object_name_default);
//REFERENCE_DECLARE(0x0166D6D8, int32 const, _hs_type_object_default);
//REFERENCE_DECLARE(0x0166D6DC, int32 const, _hs_type_unit_default);
//REFERENCE_DECLARE(0x0166D6E0, int32 const, _hs_type_vehicle_default);
//REFERENCE_DECLARE(0x0166D6E4, int32 const, _hs_type_weapon_default);
//REFERENCE_DECLARE(0x0166D6E8, int32 const, _hs_type_device_default);
//REFERENCE_DECLARE(0x0166D6EC, int32 const, _hs_type_scenery_default);
//REFERENCE_DECLARE(0x0166D6F0, int32 const, _hs_type_effect_scenery_default);
//REFERENCE_DECLARE(0x0166D6F4, int32 const, _hs_type_cinematic_lightprobe_default);
//REFERENCE_DECLARE(0x0166D6F8, int32 const, _hs_type_budget_reference_animation_graph_default);
//REFERENCE_DECLARE(0x0166D6FC, int32 const, _hs_type_budget_reference_looping_sound_default);
//REFERENCE_DECLARE(0x0166D700, int32 const, _hs_type_budget_reference_sound_default);
//REFERENCE_DECLARE_ARRAY(0x0166D704, tag const, hs_budget_reference_type_group_tags, k_hs_type_budget_reference_count);
//REFERENCE_DECLARE_ARRAY(0x0166D710, int16 const, hs_type_sizes, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x0166D7B8, int16 const, hs_object_type_masks, k_hs_type_object_count);
//REFERENCE_DECLARE_ARRAY(0x0166D7C8, int32 const, hs_tag_reference_type_group_tags, k_hs_tag_reference_type_count);
//REFERENCE_DECLARE_ARRAY(0x0166D808, hs_enum_definition const, hs_enum_table, k_hs_type_enum_count);
//REFERENCE_DECLARE(0x018BEC94, const char* const, _hs_type_string_default);
//REFERENCE_DECLARE_ARRAY(0x018BEC98, const char* const, hs_type_names, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x018BEDE8, const char* const, hs_script_type_names, NUMBER_OF_HS_SCRIPT_TYPES);
REFERENCE_DECLARE(0x024B06D4, s_data_array*, g_hs_syntax_data);

const hs_function_definition* hs_function_get(int16 function_index)
{
	ASSERT(function_index >= 0 && function_index < hs_function_table_count);
	return hs_function_table[function_index];
}

const hs_function_definition_debug* hs_function_get_debug(int16 function_index)
{
	ASSERT(function_index >= 0 && function_index < hs_function_table_debug_count);
	return hs_function_table_debug[function_index];
}

void __cdecl hs_dispose()
{
	INVOKE(0x006791C0, hs_dispose);
}

void __cdecl hs_dispose_from_old_map()
{
	INVOKE(0x006791E0, hs_dispose_from_old_map);
}

int16 __cdecl hs_find_script_by_name(const char* name, int16 num_arguments)
{
	//return INVOKE(0x00679220, hs_find_script_by_name, name, num_arguments);

	if (global_scenario_index_get() != NONE)
	{
		const struct scenario* scenario = global_scenario_get();

		for (int32 script_index = 0; script_index < scenario->scripts.count; script_index++)
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&scenario->scripts, script_index, hs_script);
			if (ascii_stricmp(name, script->name) == 0 && num_arguments == NONE || num_arguments == script->parameters.count)
			{
				return (int16)script_index;
			}
		}
	}

	return NONE;
}

int16 __cdecl hs_global_get_type(int16 global_designator)
{
	//return INVOKE(0x006792E0, hs_global_get_type, global_designator);

	int16 global_index = global_designator & MASK(15);
	if (TEST_BIT(global_designator, 15))
	{
		if (VALID_INDEX(global_index, k_hs_external_global_count))
		{
			return hs_global_external_get(global_index)->type;
		}

		if (VALID_INDEX(global_index, k_hs_external_global_debug_count))
		{
			return hs_global_external_get_debug(global_index)->type;
		}
	}

	hs_global_internal* global = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->globals, global_index, hs_global_internal);
	return global->type;
}

//.text:00679320 ; 

void __cdecl hs_initialize()
{
	INVOKE(0x00679330, hs_initialize);
}

void __cdecl hs_initialize_for_new_map()
{
	INVOKE(0x00679400, hs_initialize_for_new_map);
}

//.text:006794A0 ; 

void __cdecl hs_node_gc()
{
	INVOKE(0x006794D0, hs_node_gc);
}

//.text:006795B0 ; 
//.text:006795C0 ; void __cdecl hs_reset_time(int32)
//.text:006795D0 ; bool __cdecl hs_scenario_postprocess(bool, bool, bool)
//.text:00679670 ; int32 __cdecl hs_seconds_to_ticks(real32)
//.text:006796C0 ; real64 __cdecl hs_ticks_to_seconds(int32)

void __cdecl hs_update()
{
	//INVOKE(0x006796E0, hs_update);

	//hs_looper_game_tick();
	hs_runtime_update();
}

int16 hs_find_function_by_name(const char* name, int16 parameter_count)
{
	for (int16 function_index = 0; function_index < hs_function_table_count; function_index++)
	{
		const hs_function_definition* const function_definition = hs_function_table[function_index];
		if (csstrcmp(hs_function_table_names[function_index], name) == 0
			&& (TEST_BIT(function_definition->flags, _hs_function_flag_internal)
				|| parameter_count == NONE
				|| function_definition->formal_parameter_count == parameter_count))
		{
			return function_index;
		}
	}

	for (int16 function_index = 0; function_index < hs_function_table_debug_count; function_index++)
	{
		const hs_function_definition_debug* const function_definition = hs_function_table_debug[function_index];
		if (csstrcmp(function_definition->name, name) == 0
			&& (TEST_BIT(function_definition->flags, _hs_function_flag_internal)
				|| parameter_count == NONE
				|| function_definition->formal_parameter_count == parameter_count))
		{
			return function_index;
		}
	}

	return NONE;
}

int16 __cdecl hs_script_find_parameter_by_name(int32 script_index, const char* name)
{
	int16 result = NONE;

	hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->scripts, script_index, hs_script);
	for (int16 parameter_index = 0; parameter_index < (int16)script->parameters.count; parameter_index++)
	{
		hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
		if (ascii_stricmp(name, parameter->name) == 0)
		{
			result = parameter_index;
			break;
		}
	}

	return result;
}

hs_global_external* hs_global_external_get(int16 global_index)
{
	ASSERT(global_index >= 0 && global_index < k_hs_external_global_count);

	return hs_external_globals[global_index];
}

hs_global_external_debug* hs_global_external_get_debug(int16 global_index)
{
	ASSERT(global_index >= 0 && global_index < k_hs_external_global_debug_count);

	return hs_external_globals_debug[global_index];
}

int16 hs_find_global_by_name(const char* name)
{
	for (int16 global_index = 0; global_index < k_hs_external_global_count; global_index++)
	{
		if (csstrcmp(name, hs_external_globals_names[global_index]) == 0)
		{
			return global_index & MASK(15) | FLAG(15);
		}
	}

	for (int16 global_index = 0; global_index < k_hs_external_global_debug_count; global_index++)
	{
		hs_global_external_debug* global_external = hs_global_external_get_debug(global_index);
		if (csstrcmp(name, global_external->name) == 0)
		{
			return global_index & MASK(15) | FLAG(15);
		}
	}

	if (global_scenario_index_get() != NONE)
	{
		c_typed_tag_block<hs_global_internal>& globals = global_scenario_get()->globals;
		for (int16 global_index = 0; global_index < (int16)globals.count; global_index++)
		{
			hs_global_internal& global_internal = globals[global_index];
			if (ascii_stricmp(name, global_internal.name) == 0)
			{
				return global_index & MASK(15);
			}
		}
	}

	return NONE;
}

const char* hs_global_get_name(int16 global_designator)
{
	int16 global_index = global_designator & MASK(15);
	if (TEST_BIT(global_designator, 15))
	{
		if (VALID_INDEX(global_index, k_hs_external_global_count))
		{
			return hs_external_globals_names[global_index];
		}

		if (VALID_INDEX(global_index, k_hs_external_global_debug_count))
		{
			return hs_global_external_get_debug(global_index)->name;
		}
	}

	return global_scenario_get()->globals[global_index].name;
}

int16 enumeration_count = 0;
int16 enumeration_results_count = 0;
const char** enumeration_results = NULL;
const char* enumeration_token = NULL;

void hs_tokens_enumerate_add_string(const char* string)
{
	ASSERT(enumeration_results);

	if (enumeration_count < enumeration_results_count
		&& ascii_stristr(string, enumeration_token) != NONE
		&& ascii_strnicmp(string, "!", 1))
		enumeration_results[enumeration_count++] = string;
}

void hs_enumerate_from_string_list(const char* const* string_list, int16 starting_index, int16 count)
{
	while (starting_index < count)
	{
		hs_tokens_enumerate_add_string(string_list[starting_index++]);
	}
}

void hs_enumerate_block_data(const s_tag_block* block, int16 offset, int32 size)
{
	for (int16 index = 0; index < block->count; index++)
	{
		const char* name = static_cast<const char*>(tag_block_get_element_with_size(block, index, size)) + offset;
		hs_tokens_enumerate_add_string(name);
	}
}

void hs_enumerate_scenario_data(int16 scenario_offset, int16 block_offset, int32 block_size)
{
	if (global_scenario_index_get() != NONE)
	{
		const s_tag_block* block = (const s_tag_block*)offset_pointer(global_scenario_get(), scenario_offset);
		hs_enumerate_block_data(block, block_offset, block_size);
	}
}

void hs_enumerate_block_data_string_id(const s_tag_block* block, int16 offset, int32 size)
{
	for (int16 index = 0; index < block->count; index++)
	{
		string_id name_string_id = *reinterpret_cast<const string_id*>(static_cast<const char*>(tag_block_get_element_with_size(block, index, size)) + offset);
		const char* name = string_id_get_string_const(name_string_id);
		hs_tokens_enumerate_add_string(name);
	}
}

void hs_enumerate_scenario_data_string_id(int16 scenario_offset, int16 block_offset, int32 block_size)
{
	if (global_scenario_index_get() != NONE)
	{
		const s_tag_block* block = (const s_tag_block*)offset_pointer(global_scenario_get(), scenario_offset);
		hs_enumerate_block_data_string_id(block, block_offset, block_size);
	}
}

int32 const k_maximum_hs_token_enumerator_count = 32;

using hs_enumerator_t = void __cdecl(void);
hs_enumerator_t* hs_token_enumerators[]
{
	hs_enumerate_special_form_names,
	hs_enumerate_script_type_names,
	hs_enumerate_type_names,
	hs_enumerate_function_names,
	hs_enumerate_script_names,
	hs_enumerate_variable_names,
	hs_enumerate_ai_names,
	hs_enumerate_ai_command_list_names,
	hs_enumerate_ai_command_script_names,
	hs_enumerate_ai_behavior_names,
	hs_enumerate_starting_profile_names,
	hs_enumerate_conversation_names,
	hs_enumerate_object_names,
	hs_enumerate_trigger_volume_names,
	hs_enumerate_cutscene_flag_names,
	hs_enumerate_cutscene_camera_point_names,
	hs_enumerate_cutscene_title_names,
	hs_enumerate_cutscene_recording_names,
	hs_enumerate_enum_skull_names
};
int32 const k_hs_token_enumerator_count = NUMBEROF(hs_token_enumerators);

int16 hs_tokens_enumerate(const char* token, int32 type_mask, const char** matching_items, int16 matching_item_count)
{
	ASSERT(!enumeration_results);

	enumeration_count = 0;
	enumeration_results_count = matching_item_count;
	enumeration_results = matching_items;
	enumeration_token = token ? token : "";

	for (int16 type_index = 0; type_index < k_hs_token_enumerator_count && type_index < k_maximum_hs_token_enumerator_count; type_index++)
	{
		ASSERT(hs_token_enumerators[type_index]);

		if (TEST_BIT(type_mask, type_index))
		{
			hs_token_enumerators[type_index]();
		}
	}

	qsort_4byte(matching_items, enumeration_count, sort_by_found_index, enumeration_token);
	enumeration_results = NULL;

	return enumeration_count;
}

bool __cdecl sort_by_found_index(int32 look_inside1, int32 look_inside2, const void* look_for)
{
	const char* string1 = reinterpret_cast<const char*>(look_inside1);
	const char* string2 = reinterpret_cast<const char*>(look_inside2);
	const char* substring = static_cast<const char*>(look_for);

	int string1_index = ascii_stristr(string1, substring);
	int string2_index = ascii_stristr(string2, substring);

	if (string1_index == string2_index)
	{
		return ascii_stricmp(string1, string2);
	}

	return string1_index > string2_index;
}

void __cdecl hs_enumerate_special_form_names(void)
{
	hs_tokens_enumerate_add_string("script");
	hs_tokens_enumerate_add_string("global");
}

void __cdecl hs_enumerate_script_type_names(void)
{
	hs_enumerate_from_string_list(hs_script_type_names, _hs_script_startup, NUMBER_OF_HS_SCRIPT_TYPES);
}

void __cdecl hs_enumerate_type_names(void)
{
	hs_enumerate_from_string_list(hs_type_names, _hs_type_void, k_hs_type_count);
}

void __cdecl hs_enumerate_function_names(void)
{
	// original names
	hs_enumerate_from_string_list(hs_function_table_names, 0, hs_function_table_count);

	// our names, eventually it'll only be this
	//for (int16 function_index = 0; function_index < int16(hs_function_table_debug_count); function_index++)
	//	hs_tokens_enumerate_add_string(hs_function_get_debug(function_index)->name);
}

void __cdecl hs_enumerate_script_names(void)
{
	hs_enumerate_scenario_data(OFFSETOF(struct scenario, scripts), OFFSETOF(hs_script, name), sizeof(hs_script));
}

void __cdecl hs_enumerate_variable_names(void)
{
	// original names
	hs_enumerate_from_string_list(hs_external_globals_names, 0, k_hs_external_global_count);

	// our names, eventually it'll only be this
	//for (int16 global_index = 0; global_index < int16(k_hs_external_global_debug_count); global_index++)
	//	hs_tokens_enumerate_add_string(hs_global_external_get_debug(global_index)->name);

	hs_enumerate_scenario_data(OFFSETOF(struct scenario, globals), 0, sizeof(hs_global_internal));
}

void __cdecl hs_enumerate_ai_names(void)
{
	if (global_scenario_index_get() != NONE)
	{
		struct scenario* scenario = global_scenario_get();

		for (s_squad_definition& squad : scenario->squads)
		{
			hs_tokens_enumerate_add_string(squad.name.get_string());
		}

		for (squad_group_definition& squad_group : scenario->squad_groups)
		{
			hs_tokens_enumerate_add_string(squad_group.name.get_string());
		}

		for (zone_definition& zone : scenario->zones)
		{
			hs_tokens_enumerate_add_string(zone.name.get_string());
		}

		for (orders_definition& order : scenario->orders)
		{
			hs_tokens_enumerate_add_string(order.name);
		}
	
		if (scenario->scripting_data.count)
		{
			for (int32 cs_index = 0; cs_index < cs_scenario_get_script_data(scenario)->point_sets.count; cs_index++)
			{
				hs_tokens_enumerate_add_string(cs_get_point_set(cs_index)->name);
			}
		}
	}
}

void __cdecl hs_enumerate_ai_command_list_names(void)
{
}

void __cdecl hs_enumerate_ai_command_script_names(void)
{
	if (global_scenario_index_get() != NONE)
	{
		struct scenario* scenario = global_scenario_get();

		for (hs_script& script : scenario->scripts)
		{
			if (script.script_type == _hs_script_command_script)
				hs_tokens_enumerate_add_string(script.name);
		}
	}
}

void __cdecl hs_enumerate_ai_behavior_names(void)
{
	for (int32 i = 0; i < NUMBER_OF_BEHAVIORS; i++)
		hs_tokens_enumerate_add_string(behavior_names[i]);
}

void __cdecl hs_enumerate_starting_profile_names(void)
{
	hs_enumerate_scenario_data(OFFSETOF(struct scenario, player_starting_profile), OFFSETOF(scenario_starting_profile, name), sizeof(scenario_starting_profile));
}

void __cdecl hs_enumerate_conversation_names(void)
{
}

void __cdecl hs_enumerate_object_names(void)
{
	hs_enumerate_scenario_data(OFFSETOF(struct scenario, object_names), OFFSETOF(scenario_object_name, name), sizeof(scenario_object_name));
}

void __cdecl hs_enumerate_trigger_volume_names(void)
{
	hs_enumerate_scenario_data_string_id(OFFSETOF(struct scenario, trigger_volumes), OFFSETOF(scenario_trigger_volume, name), sizeof(scenario_trigger_volume));
}

void __cdecl hs_enumerate_cutscene_flag_names(void)
{
	hs_enumerate_scenario_data_string_id(OFFSETOF(struct scenario, cutscene_flags), OFFSETOF(scenario_cutscene_flag, name), sizeof(scenario_cutscene_flag));
}

void __cdecl hs_enumerate_cutscene_camera_point_names(void)
{
	hs_enumerate_scenario_data(OFFSETOF(struct scenario, cutscene_camera_points), OFFSETOF(scenario_cutscene_camera_point, name), sizeof(scenario_cutscene_camera_point));
}

void __cdecl hs_enumerate_cutscene_title_names(void)
{
	hs_enumerate_scenario_data_string_id(OFFSETOF(struct scenario, cutscene_titles), OFFSETOF(s_scenario_cutscene_title, name), sizeof(s_scenario_cutscene_title));
}

void __cdecl hs_enumerate_cutscene_recording_names(void)
{
	hs_enumerate_scenario_data(OFFSETOF(struct scenario, recorded_animations), OFFSETOF(recorded_animation_definition, name), sizeof(recorded_animation_definition));
}

void __cdecl hs_enumerate_enum_skull_names(void)
{
	hs_enumerate_from_string_list(global_primary_skull_names, _campaign_skull_primary_iron, k_campaign_skull_primary_count);
	hs_enumerate_from_string_list(global_secondary_skull_names, _campaign_skull_secondary_assassin, k_campaign_skull_secondary_count);
}

// 0166D5E8
bool const _hs_type_boolean_default = false;

// 0166D5EC
real32 const _hs_type_real_default = 0.0f;

// 0166D5F0
int16 const _hs_type_short_integer_default = 0;

// 0166D5F4
int32 const _hs_type_long_integer_default = 0;

// 0166D5F8
int16 const _hs_type_script_default = NONE;

// 0166D5FC
int32 const _hs_type_string_id_default = k_string_id_empty_string;

// 0166D600
int32 const _hs_type_unit_seat_mapping_default = NONE;

// 0166D604
int16 const _hs_type_trigger_volume_default = NONE;

// 0166D608
int16 const _hs_type_cutscene_flag_default = NONE;

// 0166D60C
int16 const _hs_type_cutscene_camera_point_default = NONE;

// 0166D610
int16 const _hs_type_cutscene_title_default = NONE;

// 0166D614
int16 const _hs_type_cutscene_recording_default = NONE;

// 0166D618
int32 const _hs_type_device_group_default = NONE;

// 0166D61C
int32 const _hs_type_ai_default = 0;

// 0166D620
int16 const _hs_type_ai_command_list_default = NONE;

// 0166D624
int16 const _hs_type_ai_command_script_default = NONE;

// 0166D628
int16 const _hs_type_ai_behavior_default = NONE;

// 0166D62C
int16 const _hs_type_ai_orders_default = NONE;

// 0166D630
int32 const _hs_type_ai_line_default = NONE;

// 0166D634
int16 const _hs_type_starting_profile_default = NONE;

// 0166D638
int16 const _hs_type_conversation_default = NONE;

// 0166D63C
int16 const _hs_type_zone_set_default = NONE;

// 0166D640
int16 const _hs_type_designer_zone_default = NONE;

// 0166D644
int32 const _hs_type_point_ref_default = NONE;

// 0166D648
int32 const _hs_type_style_default = NONE;

// 0166D64C
int32 const _hs_type_object_list_default = NONE;

// 0166D650
int32 const _hs_type_folder_default = NONE;

// 0166D654
int32 const _hs_type_sound_default = NONE;

// 0166D658
int32 const _hs_type_looping_sound_default = NONE;

// 0166D65C
int32 const _hs_type_effect_default = NONE;

// 0166D660
int32 const _hs_type_damage_default = NONE;

// 0166D664
int32 const _hs_type_animation_graph_default = NONE;

// 0166D668
int32 const _hs_type_damage_effect_default = NONE;

// 0166D66C
int32 const _hs_type_object_definition_default = NONE;

// 0166D670
int32 const _hs_type_bitmap_default = NONE;

// 0166D674
int32 const _hs_type_shader_default = NONE;

// 0166D678
int32 const _hs_type_render_model_definition_default = NONE;

// 0166D67C
int32 const _hs_type_structure_bsp_definition_default = NONE;

// 0166D680
int32 const _hs_type_structure_lightmap_definition_default = NONE;

// 0166D684
int32 const _hs_type_cinematic_definition_default = NONE;

// 0166D688
int32 const _hs_type_cinematic_scene_definition_default = NONE;

// 0166D68C
int32 const _hs_type_bink_definition_default = NONE;

// 0166D690
int32 const _hs_type_any_tag_default = NONE;

// 0166D694
int16 const _hs_type_enum_game_difficulty_default = NONE;

// 0166D698
int16 const _hs_type_enum_team_default = NONE;

// 0166D69C
int16 const _hs_type_enum_mp_team_default = _multiplayer_team_red;

// 0166D6A0
int16 const _hs_type_enum_controller_default = _controller0;

// 0166D6A4
int16 const _hs_type_enum_button_preset_default = _button_preset_standard;

// 0166D6A8
int16 const _hs_type_enum_joystick_preset_default = _joystick_preset_standard;

// 0166D6AC
int16 const _hs_type_enum_player_character_type_default = _player_model_choice_spartan;

// 0166D6B0
int16 const _hs_type_enum_voice_output_setting_default = _voice_output_setting_default;

// 0166D6B4
int16 const _hs_type_enum_subtitle_setting_default = _subtitle_setting_automatic;

// 0166D6B8
int16 const _hs_type_enum_actor_type_default = NONE;

// 0166D6BC
int16 const _hs_type_enum_model_state_default = NONE;

// 0166D6C0
int16 const _hs_type_enum_event_default = NONE;

// 0166D6C8
int16 const _hs_type_enum_character_physics_override_default = _character_physics_default;

// 0166D6CC
int16 const _hs_type_enum_primary_skull_default = NONE;

// 0166D6D0
int16 const _hs_type_enum_secondary_skull_default = NONE;

// 0166D6D4
int16 const _hs_type_object_name_default = NONE;

// 0166D6D8
int32 const _hs_type_object_default = NONE;

// 0166D6DC
int32 const _hs_type_unit_default = NONE;

// 0166D6E0
int32 const _hs_type_vehicle_default = NONE;

// 0166D6E4
int32 const _hs_type_weapon_default = NONE;

// 0166D6E8
int32 const _hs_type_device_default = NONE;

// 0166D6EC
int32 const _hs_type_scenery_default = NONE;

// 0166D6F0
int32 const _hs_type_effect_scenery_default = NONE;

// 0166D6F4
int32 const _hs_type_cinematic_lightprobe_default = NONE;

// 0166D6F8
int32 const _hs_type_budget_reference_animation_graph_default = NONE;

// 0166D6FC
int32 const _hs_type_budget_reference_looping_sound_default = NONE;

// 0166D700
int32 const _hs_type_budget_reference_sound_default = NONE;

// 0166D704
tag const hs_budget_reference_type_group_tags[k_hs_type_budget_reference_count]
{
	MODEL_ANIMATION_GRAPH_TAG,
	SOUND_LOOPING_TAG,
	SOUND_TAG
};

// 0166D710
int16 const hs_type_sizes[k_hs_type_count]
{
	0, // unparsed
	0, // special_form
	0, // function_name
	0, // passthrough

	0, // void
	1, // boolean
	4, // real32
	2, // int16
	4, // int32
	4, // string
	2, // script
	4, // string_id
	4, // unit_seat_mapping
	2, // trigger_volume
	2, // cutscene_flag
	2, // cutscene_camera_point
	2, // cutscene_title
	2, // cutscene_recording
	4, // device_group
	4, // ai
	2, // ai_command_list
	2, // ai_command_script
	2, // ai_behavior
	2, // ai_orders
	4, // ai_line
	2, // starting_profile
	2, // conversation
	2, // zone_set
	2, // designer_zone
	4, // point_reference
	4, // style
	4, // object_list
	4, // folder

	4, // sound
	4, // effect
	4, // damage
	4, // looping_sound
	4, // animation_graph
	4, // damage_effect
	4, // object_definition
	4, // bitmap
	4, // shader
	4, // render_model
	4, // structure_definition
	4, // lightmap_definition
	4, // cinematic_definition
	4, // cinematic_scene_definition
	4, // bink_definition
	4, // any_tag
	4, // any_tag_not_resolving

	2, // game_difficulty
	2, // team
	2, // mp_team
	2, // controller
	2, // button_preset
	2, // joystick_preset
	2, // player_character_type
	2, // voice_output_setting
	2, // voice_mask
	2, // subtitle_setting
	2, // actor_type
	2, // model_state
	2, // event
	2, // character_physics
	2, // primary_skull
	2, // secondary_skull

	4, // object
	4, // unit
	4, // vehicle
	4, // weapon
	4, // device
	4, // scenery
	4, // effect_scenery

	2, // object_name
	2, // unit_name
	2, // vehicle_name
	2, // weapon_name
	2, // device_name
	2, // scenery_name
	2, // effect_scenery_name

	4, // cinematic_lightprobe

	4, // animation_budget_reference
	4, // looping_sound_budget_reference
	4  // sound_budget_reference
};

// 0166D7B8
int16 const hs_object_type_masks[k_hs_type_object_count]
{
	_object_mask_all,           // all
	_object_mask_unit,          // unit
	_object_mask_vehicle,       // vehicle
	_object_mask_weapon,        // weapon
	_object_mask_device,        // device
	_object_mask_scenery,       // scenery
	_object_mask_effect_scenery // effect_scenery
};

// 0166D7C8
int32 const hs_tag_reference_type_group_tags[k_hs_tag_reference_type_count]
{
	SOUND_TAG,                            // sound
	EFFECT_TAG,                           // effect
	DAMAGE_EFFECT_TAG,                    // damage
	SOUND_LOOPING_TAG,                    // looping_sound
	MODEL_ANIMATION_GRAPH_TAG,            // animation_graph
	DAMAGE_EFFECT_TAG,                    // damage_effect
	OBJECT_TAG,                           // object_definition
	BITMAP_TAG,                           // bitmap
	RENDER_METHOD_TAG,                    // shader
	RENDER_MODEL_TAG,                     // render_model
	SCENARIO_STRUCTURE_BSP_TAG,           // structure_definition
	SCENARIO_STRUCTURE_LIGHTING_INFO_TAG, // lightmap_definition
	CINEMATIC_TAG,                        // cinematic_definition
	CINEMATIC_SCENE_TAG,                  // cinematic_scene_definition
	BINK_TAG,                             // bink_definition
	NONE                                  // any_tag
};

// 0166D808
hs_enum_definition const hs_enum_table[k_hs_type_enum_count]
{
#define MAKE_ENUM_TABLE_ENTRY(name){ NUMBEROF((name)), (name) }
	MAKE_ENUM_TABLE_ENTRY(global_campaign_difficulty_level_names),
	MAKE_ENUM_TABLE_ENTRY(global_campaign_team_names),
	MAKE_ENUM_TABLE_ENTRY(global_multiplayer_team_names),
	MAKE_ENUM_TABLE_ENTRY(global_controller_index_names),
	MAKE_ENUM_TABLE_ENTRY(global_button_preset_names),
	MAKE_ENUM_TABLE_ENTRY(global_joystick_preset_names),
	MAKE_ENUM_TABLE_ENTRY(global_player_model_choice_names),
	MAKE_ENUM_TABLE_ENTRY(global_voice_output_setting_names),
	MAKE_ENUM_TABLE_ENTRY(global_voice_mask_names),
	MAKE_ENUM_TABLE_ENTRY(global_subtitle_setting_names),
	MAKE_ENUM_TABLE_ENTRY(global_actor_type_names),
	MAKE_ENUM_TABLE_ENTRY(global_model_state_names),
	MAKE_ENUM_TABLE_ENTRY(k_event_level_names),
	MAKE_ENUM_TABLE_ENTRY(global_character_physics_override_names),
	MAKE_ENUM_TABLE_ENTRY(global_primary_skull_names),
	MAKE_ENUM_TABLE_ENTRY(global_secondary_skull_names)
#undef MAKE_ENUM_TABLE_ENTRY
};

// 018BEC94
const char* const _hs_type_string_default = "";

// 018BEC98
const char* const hs_type_names[k_hs_type_count]
{
	"unparsed",
	"special_form",
	"function_name",
	"passthrough",

	"void",
	"boolean",
	"real32",
	"int16",
	"int32",
	"string",
	"script",
	"string_id",
	"unit_seat_mapping",
	"trigger_volume",
	"cutscene_flag",
	"cutscene_camera_point",
	"cutscene_title",
	"cutscene_recording",
	"device_group",
	"ai",
	"ai_command_list",
	"ai_command_script",
	"ai_behavior",
	"ai_orders",
	"ai_line",
	"starting_profile",
	"conversation",
	"zone_set",
	"designer_zone",
	"point_reference",
	"style",
	"object_list",
	"folder",

	"sound",
	"effect",
	"damage",
	"looping_sound",
	"animation_graph",
	"damage_effect",
	"object_definition",
	"bitmap",
	"shader",
	"render_model",
	"structure_definition",
	"lightmap_definition",
	"cinematic_definition",
	"cinematic_scene_definition",
	"bink_definition",
	"any_tag",
	"any_tag_not_resolving",

	"game_difficulty",
	"team",
	"mp_team",
	"controller",
	"button_preset",
	"joystick_preset",
	"player_character_type",
	"voice_output_setting",
	"voice_mask",
	"subtitle_setting",
	"actor_type",
	"model_state",
	"event",
	"character_physics",
	"primary_skull",
	"secondary_skull",

	"object",
	"unit",
	"vehicle",
	"weapon",
	"device",
	"scenery",
	"effect_scenery",

	"object_name",
	"unit_name",
	"vehicle_name",
	"weapon_name",
	"device_name",
	"scenery_name",
	"effect_scenery_name",

	"cinematic_lightprobe",

	"animation_budget_reference",
	"looping_sound_budget_reference",
	"sound_budget_reference"
};

// 018BEDE8
const char* const hs_script_type_names[NUMBER_OF_HS_SCRIPT_TYPES]
{
	"startup",
	"dormant",
	"continuous",
	"static",
	"command_script",
	"stub"
};

