#include "hs/hs_compile.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "scenario/scenario.hpp"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

short scenario_object_name_index_from_string(s_scenario* scenario, char const* name)
{
	for (short object_name_index = 0; object_name_index < static_cast<short>(global_scenario_get()->object_names.count()); object_name_index++)
	{
		scenario_object_name& object_name = global_scenario_get()->object_names[object_name_index];
		if (object_name.name.equals(name))
			return object_name_index;
	}

	return NONE;
}

bool hs_parse_object_and_object_name_internal(long expression_index, short byteswap_type)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	decltype(hs_compile_globals.error_message_buffer)& error_message_buffer = hs_compile_globals.error_message_buffer;

	long& long_value = *reinterpret_cast<long*>(expression->data);
	short& short_value = *reinterpret_cast<short*>(expression->data);

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	ASSERT(HS_TYPE_IS_OBJECT(byteswap_type) || HS_TYPE_IS_OBJECT_NAME(byteswap_type));
	ASSERT(HS_TYPE_IS_OBJECT_NAME(expression->type));

	short object_name_index = scenario_object_name_index_from_string(global_scenario_get(), source_offset);
	if (object_name_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid object name.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	scenario_object_name& object_name = global_scenario_get()->object_names[object_name_index];

	if (object_name.object_type == NONE)
	{
		hs_compile_globals.error_message = "no object exists with this name.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}
	
	if (object_name.scenario_datum_index == NONE)
	{
		hs_compile_globals.error_message = "this object no longer exists in the scenario.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (!TEST_BIT(hs_object_type_masks[expression->type.get() - _hs_type_object_name], object_name.object_type))
	{
		csnzprintf(error_message_buffer, sizeof(error_message_buffer), "this is not an object of type %s.", hs_type_names[expression->type.get()]);
		hs_compile_globals.error_message = hs_compile_globals.error_message_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (HS_TYPE_IS_OBJECT(byteswap_type))
	{
		long_value = object_name_index;
		return true;
	}

	if (HS_TYPE_IS_OBJECT_NAME(byteswap_type))
	{
		short_value = object_name_index;
		return true;
	}

	hs_compile_globals.error_message = "invalid byteswap type,";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_boolean(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_boolean);
	ASSERT(expression->constant_type == expression->type);

	bool& value = *reinterpret_cast<bool*>(expression->data);

	if (csstrcmp(source_offset, "true") == 0 || csstrcmp(source_offset, "on") == 0 || csstrcmp(source_offset, "1") == 0)
	{
		value = true;
		return true;
	}
	if (csstrcmp(source_offset, "false") == 0 || csstrcmp(source_offset, "off") == 0 || csstrcmp(source_offset, "0") == 0)
	{
		value = false;
		return true;
	}

	hs_compile_globals.error_message = "i expected \"true\" or \"false\".";
	hs_compile_globals.error_offset = expression->source_offset;

	value = false;
	return false;
}

bool hs_parse_real(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_real);
	ASSERT(expression->constant_type == expression->type);

	real& value = *reinterpret_cast<real*>(expression->data);

	if (*source_offset == '-')
		++source_offset;

	bool result = true;
	bool error_occurred = false;
	while (*source_offset)
	{
		if (!isdigit(*source_offset))
		{
			if (error_occurred || *source_offset != '.')
			{
				hs_compile_globals.error_message = "this is not a valid real number.";
				hs_compile_globals.error_offset = expression->source_offset;

				result = false;
				break;
			}
			error_occurred = true;
		}
		source_offset++;
	}

	value = static_cast<real>(atof(&hs_compile_globals.compiled_source[expression->source_offset]));
	return result;
}

bool hs_parse_integer(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_short_integer || expression->type == _hs_type_long_integer);
	ASSERT(expression->constant_type == expression->type);

	short& short_value = *reinterpret_cast<short*>(expression->data);
	long& long_value = *reinterpret_cast<long*>(expression->data);

	if (*source_offset == '-')
		++source_offset;

	bool result = true;
	bool error_occurred = false;
	while (*source_offset)
	{
		if (!isdigit(*source_offset))
		{
			if (error_occurred || *source_offset != '.')
			{
				hs_compile_globals.error_message = "this is not a valid integer.";
				hs_compile_globals.error_offset = expression->source_offset;

				result = false;
				break;
			}
			error_occurred = true;
		}
		source_offset++;
	}

	long source_value = atoi(&hs_compile_globals.compiled_source[expression->source_offset]);
	if (result && !(expression->type == _hs_type_short_integer || IN_RANGE_INCLUSIVE(source_value, -32767, 32768)))
	{
		hs_compile_globals.error_message = "shorts must be in the range [-32767, 32768].";
		hs_compile_globals.error_offset = expression->source_offset;

		result = false;
	}

	short_value = static_cast<short>(source_value);
	if (expression->type == _hs_type_long_integer)
		long_value = static_cast<long>(source_value);

	return result;
}

bool hs_parse_string(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string);
	ASSERT(expression->constant_type == expression->type);

	char const* value = reinterpret_cast<char const*>(expression->data);

	value = source_offset;
	return true;
}

bool hs_parse_script(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_script);
	ASSERT(expression->constant_type == expression->type);

	short& value = *reinterpret_cast<short*>(expression->data);

	short script_index = hs_find_script_by_name(source_offset, NONE);
	if (script_index != NONE)
	{
		value = script_index;
		//hs_compile_add_reference(script_index, 1, expression_index);
		return true;
	}

	hs_compile_globals.error_message = "this is not a valid script name.";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_string_id(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string_id);
	ASSERT(expression->constant_type == expression->type);

	string_id& value = *reinterpret_cast<string_id*>(expression->data);

	string_id id = string_id_retrieve(source_offset);
	if (id != NONE)
	{
		value = id;
		return true;
	}

	hs_compile_globals.error_message = "this is not a valid string parameter.";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_unit_seat_mapping(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_trigger_volume(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_cutscene_flag(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_cutscene_camera_point(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_cutscene_title(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_cutscene_recording(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_device_group(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_ai(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_ai_command_list(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_ai_line);

	return false;
}

bool hs_parse_ai_command_script(long expression_index)
{
	// #TODO
	return false;
}

short __cdecl behavior_index_by_name(char* name)
{
	return INVOKE(0x014745C0, behavior_index_by_name, name);
}

bool hs_parse_ai_behavior(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	short& value = *reinterpret_cast<short*>(expression->data);

	short behavior_index = behavior_index_by_name(source_offset);
	if (behavior_index != NONE)
	{
		value = behavior_index;
		return true;
	}

	hs_compile_globals.error_message = "not a valid behavior";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_ai_orders(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_ai_line(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_ai_line);
	ASSERT(expression->constant_type == expression->type);

	string_id& value = *reinterpret_cast<string_id*>(expression->data);

	value = string_id_retrieve(source_offset);
	return true;
}

bool hs_parse_starting_profile(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_conversation(long expression_index)
{
	return false;
}

bool hs_parse_zone_set(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_designer_zone(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_point_ref(long expression_index)
{
	// #TODO
	return false;
}

long __cdecl style_get_by_name(char const* name)
{
	return INVOKE(0x014B32D0, style_get_by_name, name);
}

bool hs_parse_style(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	long& value = *reinterpret_cast<long*>(expression->data);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_style);
	ASSERT(expression->constant_type == expression->type);

	long style = style_get_by_name(source_offset);
	if (style != NONE)
	{
		value = style;
		return true;
	}

	hs_compile_globals.error_message = "invalid style";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_object_list(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_object_list);

	expression->constant_type = _hs_type_object_name;
	expression->type = _hs_type_object_name;
	bool result = hs_parse_object_and_object_name_internal(expression_index, expression->constant_type);
	expression->type = _hs_type_object_list;

	return result;
}

bool hs_parse_folder(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse_sound_tag_reference(long expression_index)
{
	if (!hs_parse_tag_reference(expression_index))
	{
		*reinterpret_cast<long*>(hs_syntax_get(expression_index)->data) = NONE;
		hs_compile_globals.error_message = 0;
	}

	return true;
}

// #TODO: find the actual name for this function
bool group_tag_from_group_name(char const* group_name, tag* group_tag_out)
{
	tag group_tag = group_name_to_group_tag(group_name);
	if (group_tag == NONE)
		return false;

	*group_tag_out = group_tag;
	return true;
}

bool hs_parse_tag_reference(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	long& value = *reinterpret_cast<long*>(expression->data);

	if (global_scenario_index_get() == NONE)
	{
		value = NONE;
		return true;
	}

	ASSERT(HS_TYPE_IS_TAG_REFERENCE(expression->type));

	tag group_tag = hs_tag_reference_type_group_tags[expression->type.get() - _hs_type_sound];

	for (hs_tag_reference& tag_reference : global_scenario_get()->references)
	{
		if (tag_reference.reference.index != NONE)
		{
			char const* tag_name = tag_get_name(tag_reference.reference.index);
			if (csstrcmp(tag_name, source_offset) == 0 && tag_reference.reference.group_tag == group_tag)
			{
				value = tag_reference.reference.index;
				break;
			}
		}
	}

	if (value == NONE && !hs_compile_globals.__unknown421)
	{
		bool v11 = false;
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			long tag_name_length = extension_offset - source_offset;
			if (group_tag_from_group_name(extension, &group_tag))
			{
				c_static_string<256> tag_name;
				tag_name.set_bounded(source_offset, tag_name_length);
				value = tag_loaded(group_tag, tag_name.get_string());

				v11 = value != NONE;
			}
		}

		if (!v11)
			value = tag_loaded(group_tag, source_offset);
	}

	return true;
}

bool hs_parse_tag_reference_not_resolving(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	long& value = *reinterpret_cast<long*>(expression->data);

	if (global_scenario_index_get() == NONE)
	{
		value = NONE;
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (value == NONE && !hs_compile_globals.__unknown421)
	{
		tag group_tag = NONE;
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			long tag_name_length = extension_offset - source_offset;
			if (group_tag_from_group_name(extension, &group_tag))
			{
				c_static_string<256> tag_name;
				tag_name.set_bounded(source_offset, tag_name_length);
				value = tag_loaded(group_tag, tag_name.get_string());
			}
		}
	}

	if (value != NONE)
		return true;

	hs_compile_globals.error_message = "not a loaded tag";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_enum(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	decltype(hs_compile_globals.error_message_buffer)& error_message_buffer = hs_compile_globals.error_message_buffer;

	short& value = *reinterpret_cast<short*>(expression->data);

	if (!HS_TYPE_IS_ENUM(expression->type))
	{
		csnzprintf(
			error_message_buffer,
			sizeof(error_message_buffer),
			"corrupt enum expression (type %d constant-type %d)",
			expression->type.get(),
			expression->constant_type.get());

		hs_compile_globals.error_message = error_message_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	hs_enum_definition const* enum_definition = &hs_enum_table[expression->type.get() - _hs_type_game_difficulty];
	ASSERT(enum_definition->count);

	short i = 0;
	for (; i < enum_definition->count && csstricmp(source_offset, enum_definition->names[i]); i++);

	bool result = true;
	if (i == enum_definition->count)
	{
		csnzprintf(error_message_buffer, sizeof(error_message_buffer), "%s must be ", hs_type_names[expression->type.get()]);

		for (i = 0; i < enum_definition->count - 1; i++)
		{
			csstrnzcat(error_message_buffer, "\"", sizeof(error_message_buffer));
			csstrnzcat(error_message_buffer, enum_definition->names[i], sizeof(error_message_buffer));
			csstrnzcat(error_message_buffer, "\", ", sizeof(error_message_buffer));
		}

		if (enum_definition->count > 1)
			csstrnzcat(error_message_buffer, "or ", sizeof(error_message_buffer));

		csstrnzcat(error_message_buffer, "\"", sizeof(error_message_buffer));
		csstrnzcat(error_message_buffer, enum_definition->names[i], sizeof(error_message_buffer));
		csstrnzcat(error_message_buffer, "\".", sizeof(error_message_buffer));

		hs_compile_globals.error_message = error_message_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		result = false;
	}

	value = i;
	return result;
}

bool hs_parse_object(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	long& value = *reinterpret_cast<long*>(expression->data);

	ASSERT(HS_TYPE_IS_OBJECT(expression->type));

	if (csstrcmp(&hs_compile_globals.compiled_source[expression->source_offset], "none") == 0)
	{
		value = NONE;
		return true;
	}

	expression->type += k_hs_script_type_count;
	expression->constant_type = expression->type;
	bool result = hs_parse_object_and_object_name_internal(expression_index, expression->constant_type);
	expression->type -= k_hs_script_type_count;

	if (!result && hs_parse_ai(expression_index))
	{
		hs_compile_globals.error_message = NULL;
		hs_compile_globals.error_offset = NONE;
		result = true;
	}

	return result;
}

bool hs_parse_object_name(long expression_index)
{
	return hs_parse_object_and_object_name_internal(expression_index, _hs_type_object_name);;
}

long __cdecl scenario_cinematic_lighting_palette_entry_get_by_name(s_scenario const* scenario, string_id name)
{
	for (long cinematic_lighting_palette_entry = 0; cinematic_lighting_palette_entry < scenario->cinematic_lighting_palette.count(); cinematic_lighting_palette_entry++)
	{
		if (scenario->cinematic_lighting_palette[cinematic_lighting_palette_entry].name.get_value() == name)
			return cinematic_lighting_palette_entry;
	}

	return NONE;
}

bool hs_parse_cinematic_lightprobe(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	long& value = *reinterpret_cast<long*>(expression->data);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cinematic_lightprobe);
	ASSERT(expression->constant_type == expression->type);

	string_id name = string_id_retrieve(source_offset);

	long cinematic_lighting_palette_entry = scenario_cinematic_lighting_palette_entry_get_by_name(global_scenario_get(), name);
	if (cinematic_lighting_palette_entry != NONE)
	{
		value = cinematic_lighting_palette_entry;
		return true;
	}

	hs_compile_globals.error_message = "this is not a lightprobe name.";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_budget_reference(long expression_index)
{
	// #TODO
	return false;
}

hs_type_primitive_parser_t* hs_type_primitive_parsers[k_hs_type_count]
{
	nullptr,                              // unparsed
	nullptr,                              // special_form
	nullptr,                              // function_name
	nullptr,                              // passthrough

	nullptr,                              // void
	hs_parse_boolean,                     // boolean
	hs_parse_real,                        // real
	hs_parse_integer,                     // short_integer
	hs_parse_integer,                     // long_integer
	hs_parse_string,                      // string
	hs_parse_script,                      // script
	hs_parse_string_id,                   // string_id
	hs_parse_unit_seat_mapping,           // unit_seat_mapping,
	hs_parse_trigger_volume,              // trigger_volume,
	hs_parse_cutscene_flag,               // cutscene_flag,
	hs_parse_cutscene_camera_point,       // cutscene_camera_point,
	hs_parse_cutscene_title,              // cutscene_title,
	hs_parse_cutscene_recording,          // cutscene_recording,
	hs_parse_device_group,                // device_group,
	hs_parse_ai,                          // ai,
	hs_parse_ai_command_list,             // ai_command_list,
	hs_parse_ai_command_script,           // ai_command_script,
	hs_parse_ai_behavior,                 // ai_behavior,
	hs_parse_ai_orders,                   // ai_orders,
	hs_parse_ai_line,                     // ai_line,
	hs_parse_starting_profile,            // starting_profile,
	hs_parse_conversation,                // conversation,
	hs_parse_zone_set,                    // zone_set,
	hs_parse_designer_zone,               // designer_zone,
	hs_parse_point_ref,                   // point_reference,
	hs_parse_style,                       // style,
	hs_parse_object_list,                 // object_list,
	hs_parse_folder,                      // folder,

	hs_parse_sound_tag_reference,         // sound,
	hs_parse_tag_reference,               // effect,
	hs_parse_tag_reference,               // damage,
	hs_parse_tag_reference,               // looping_sound,
	hs_parse_tag_reference,               // animation_graph,
	hs_parse_tag_reference,               // damage_effect,
	hs_parse_tag_reference,               // object_definition,
	hs_parse_tag_reference,               // bitmap,
	hs_parse_tag_reference,               // shader,
	hs_parse_tag_reference,               // render_model,
	hs_parse_tag_reference,               // structure_definition,
	hs_parse_tag_reference,               // lightmap_definition,
	hs_parse_tag_reference,               // cinematic_definition,
	hs_parse_tag_reference,               // cinematic_scene_definition,
	hs_parse_tag_reference,               // bink_definition,
	hs_parse_tag_reference,               // any_tag,
	hs_parse_tag_reference_not_resolving, // any_tag_not_resolving,

	hs_parse_enum,                        // game_difficulty,
	hs_parse_enum,                        // team,
	hs_parse_enum,                        // mp_team,
	hs_parse_enum,                        // controller,
	hs_parse_enum,                        // button_preset,
	hs_parse_enum,                        // joystick_preset,
	hs_parse_enum,                        // player_character_type,
	hs_parse_enum,                        // voice_output_setting,
	hs_parse_enum,                        // voice_mask,
	hs_parse_enum,                        // subtitle_setting,
	hs_parse_enum,                        // actor_type,
	hs_parse_enum,                        // model_state,
	hs_parse_enum,                        // event,
	hs_parse_enum,                        // character_physics,
	hs_parse_enum,                        // primary_skull,
	hs_parse_enum,                        // secondary_skull,

	hs_parse_object,                      // object,
	hs_parse_object,                      // unit,
	hs_parse_object,                      // vehicle,
	hs_parse_object,                      // weapon,
	hs_parse_object,                      // device,
	hs_parse_object,                      // scenery,
	hs_parse_object,                      // effect_scenery,

	hs_parse_object_name,                 // object_name,
	hs_parse_object_name,                 // unit_name,
	hs_parse_object_name,                 // vehicle_name,
	hs_parse_object_name,                 // weapon_name,
	hs_parse_object_name,                 // device_name,
	hs_parse_object_name,                 // scenery_name,
	hs_parse_object_name,                 // effect_scenery_name,

	hs_parse_cinematic_lightprobe,        // cinematic_lightprobe,

	hs_parse_budget_reference,            // animation_budget_reference,
	hs_parse_budget_reference,            // looping_sound_budget_reference,
	hs_parse_budget_reference,            // sound_budget_reference,
};

short __cdecl hs_script_find_parameter_by_name(long script_index, char const* name)
{
	hs_script& script = global_scenario_get()->scripts[script_index];
	for (short parameter_index = 0; parameter_index < static_cast<short>(script.parameters.count()); parameter_index++)
	{
		hs_script_parameter& parameter = script.parameters[parameter_index];
		if (parameter.name.equals(name))
			return parameter_index;
	}

	return NONE;
}

bool hs_parse_variable(long expression_index)
{
	// #TODO
	return false;

	//hs_syntax_node* expression = hs_syntax_get(expression_index);
	//char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];
	//
	//decltype(hs_compile_globals.error_message_buffer)& error_message_buffer = hs_compile_globals.error_message_buffer;
	//
	//long& value = *reinterpret_cast<long*>(expression->data);
	//
	//ASSERT(hs_type_valid(expression->type) || expression->type == _hs_unparsed);
	//
	//bool valid = false;
	//short type = NONE;
	//bool v9 = false;
	//if (hs_compile_globals.current_script_index != NONE && global_scenario_index_get() != NONE)
	//{
	//	value = hs_script_find_parameter_by_name(hs_compile_globals.current_script_index, source_offset);
	//	if (value != NONE)
	//	{
	//		hs_script& script = global_scenario_get()->scripts[hs_compile_globals.current_script_index];
	//		type = script.parameters[value].return_type.get();
	//
	//		v9 = true;
	//		valid = true;
	//	}
	//}
	//
	//if (!valid && (!hs_compile_globals.__unknown424
	//	|| expression->type.get() == NONE
	//	|| value == NONE
	//	|| !expression->flags.test(_hs_syntax_node_unknown_bit4)))
	//{
	//	value = hs_find_global_by_name(source_offset);
	//	if (value != NONE)
	//	{
	//		type = hs_global_get_type(value);
	//		valid = true;
	//	}
	//}
	//
	//if (!valid)
	//	return false;
	//
	//ASSERT(type != NONE);
	//if (expression->type.get() && !hs_can_cast(type, expression->type))
	//{
	//	csnzprintf(error_message_buffer,
	//		sizeof(error_message_buffer),
	//		"i expected a value of type %s, but the variable %s has type %s",
	//		hs_type_names[expression->type.get()],
	//		hs_global_get_name(value),
	//		hs_type_names[type]);
	//
	//	hs_compile_globals.error_message = hs_compile_globals.error_message_buffer;
	//	hs_compile_globals.error_offset = expression->source_offset;
	//
	//	return false;
	//}
	//else
	//{
	//	if (!expression->type)
	//		expression->type = type;
	//
	//	expression->flags.set(_hs_syntax_node_variable_bit, true);
	//
	//	if (v9)
	//		expression->flags.set(_hs_syntax_node_unknown_bit4, true);
	//	else
	//		hs_compile_add_reference(value, 0, expression_index);
	//
	//	return true;
	//}
	//
	//if (!hs_compile_globals.__unknown424)
	//	return false;
	//
	//if (expression->type.get() == NONE || value == NONE || !expression->flags.test(_hs_syntax_node_unknown_bit4))
	//{
	//	hs_compile_globals.error_message = "this is not a valid variable name.";
	//	hs_compile_globals.error_offset = expression->source_offset;
	//
	//	return false;
	//}
	//
	//return true;
}

bool hs_parse_primitive(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	decltype(hs_compile_globals.error_message_buffer)& error_message_buffer = hs_compile_globals.error_message_buffer;

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	if (expression->type == _hs_special_form)
	{
		hs_compile_globals.error_message = "i expected a script or variable definition.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (expression->type == _hs_type_void)
	{
		hs_compile_globals.error_message = "the value of this expression (in a <void> slot) can never be used.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	bool error_occurred = false;
	if (!hs_compile_globals.__unknown424 || expression->flags.test(_hs_syntax_node_variable_bit))
		error_occurred = hs_parse_variable(expression_index);

	if (error_occurred || !expression->type.get() || hs_compile_globals.error_message || (!hs_compile_globals.__unknown424 || !expression->flags.test(_hs_syntax_node_variable_bit)))
		return false;

	if (hs_type_primitive_parser_t* primitive_parser = hs_type_primitive_parsers[expression->type.get()])
		return primitive_parser(expression_index);

	csnzprintf(error_message_buffer, sizeof(error_message_buffer), "expressions of type %s are currently unsupported.", hs_type_names[expression->type.get()]);
	hs_compile_globals.error_message = hs_compile_globals.error_message_buffer;
	hs_compile_globals.error_offset = expression->source_offset;
	return false;
}

bool hs_parse_nonprimitive(long expression_index)
{
	// #TODO
	return false;
}

bool hs_parse(long expression_index, short expected_type)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(!hs_compile_globals.error_message);
	ASSERT(hs_type_valid(expected_type) || expected_type == _hs_special_form || expected_type == _hs_unparsed);

	if (expression->type)
		return true;

	expression->type = expected_type;
	if (hs_syntax_get(expression_index)->flags.test(_hs_syntax_node_primitive_bit))
	{
		expression->constant_type = expected_type;
		return hs_parse_primitive(expression_index);
	}

	return hs_parse_nonprimitive(expression_index);
}

bool hs_macro_function_parse(short function_index, long expression_index)
{
	bool valid = true;

	decltype(hs_compile_globals.error_message_buffer)& error_message_buffer = hs_compile_globals.error_message_buffer;

	hs_function_definition_debug const* definition = hs_function_get_debug(function_index);
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long next_node_index = hs_syntax_get(reinterpret_cast<long>(expression->data))->next_node_index;

	ASSERT(hs_type_valid(definition->return_type.get()));

	short parameter_index;
	for (parameter_index = 0;
		valid && parameter_index < definition->parameter_count && next_node_index != NONE;
		parameter_index++)
	{
		hs_syntax_node* next_expression = hs_syntax_get(next_node_index);
		if (hs_parse(next_node_index, definition->parameters[parameter_index]))
		{
			next_node_index = next_expression->next_node_index;
		}
		else
		{
			if (next_expression->type == _hs_type_string_id && hs_syntax_get(next_node_index)->flags.test(_hs_syntax_node_primitive_bit))
			{
				csnzprintf(error_message_buffer, sizeof(error_message_buffer), "this is not a valid string for '%s'", definition->name);
				hs_compile_globals.error_message = hs_compile_globals.error_message_buffer;
				hs_compile_globals.error_offset = next_expression->source_offset;
			}
			valid = false;
		}
	}

	if (valid && (parameter_index != definition->parameter_count || next_node_index != NONE))
	{
		csnzprintf(error_message_buffer, sizeof(error_message_buffer), "the \"%s\" call requires exactly %d arguments.", definition->name, definition->parameter_count);
		hs_compile_globals.error_message = hs_compile_globals.error_message_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		valid = false;
	}

	return valid;
}

