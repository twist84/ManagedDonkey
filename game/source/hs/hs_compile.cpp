#include "hs/hs_compile.hpp"

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"

#include <ctype.h>
#include <stdlib.h>

bool hs_type_primitive_parser_bool(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_type_primitive_parser_real(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_type_primitive_parser_integer(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_type_primitive_parser_string(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string);
	ASSERT(expression->constant_type == expression->type);

	char const* value = reinterpret_cast<char const*>(expression->data);

	value = source_offset;
	return true;
}

bool hs_type_primitive_parser_script(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_script);
	ASSERT(expression->constant_type == expression->type);

	short& value = *reinterpret_cast<short*>(expression->data);

	short script_index = hs_find_script_by_name(source_offset, NONE);
	if (script_index != NONE)
	{
		value = script_index;
		//sub_82D12368(script_index, 1, expression_index);
		return true;
	}

	hs_compile_globals.error_message = "this is not a valid script name.";
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_type_primitive_parser_string_id(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_type_primitive_parser_ai_command_list(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_ai_line);

	return false;
}

bool hs_type_primitive_parser_ai_line(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_ai_line);
	ASSERT(expression->constant_type == expression->type);

	string_id& value = *reinterpret_cast<string_id*>(expression->data);

	value = string_id_retrieve(source_offset);
	return true;
}

bool hs_type_primitive_parser_enum(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool __cdecl hs_macro_function_parse(short function_index, long expression_index)
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
		//if (hs_parse(next_node_index, definition->parameters[parameter_index])) // #TODO
		//{
		//	next_node_index = next_expression->next_node_index;
		//}
		//else
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

hs_type_primitive_parser_t hs_type_primitive_parsers[k_hs_type_count]
{
	nullptr,                                  // unparsed
	nullptr,                                  // special_form
	nullptr,                                  // function_name
	nullptr,                                  // passthrough
	nullptr,                                  // void
	hs_type_primitive_parser_bool,            // boolean
	hs_type_primitive_parser_real,            // real
	hs_type_primitive_parser_integer,         // short_integer
	hs_type_primitive_parser_integer,         // long_integer
	hs_type_primitive_parser_string,          // string
	hs_type_primitive_parser_script,          // script
	hs_type_primitive_parser_string_id,       // string_id
	nullptr,                                  // unit_seat_mapping,
	nullptr,                                  // trigger_volume,
	nullptr,                                  // cutscene_flag,
	nullptr,                                  // cutscene_camera_point,
	nullptr,                                  // cutscene_title,
	nullptr,                                  // cutscene_recording,
	nullptr,                                  // device_group,
	nullptr,                                  // ai,
	hs_type_primitive_parser_ai_command_list, // ai_command_list,
	nullptr,                                  // ai_command_script,
	nullptr,                                  // ai_behavior,
	nullptr,                                  // ai_orders,
	hs_type_primitive_parser_ai_line,         // ai_line,
	nullptr,                                  // starting_profile,
	nullptr,                                  // conversation,
	nullptr,                                  // zone_set,
	nullptr,                                  // designer_zone,
	nullptr,                                  // point_reference,
	nullptr,                                  // style,
	nullptr,                                  // object_list,
	nullptr,                                  // folder,
	nullptr,                                  // sound,
	nullptr,                                  // effect,
	nullptr,                                  // damage,
	nullptr,                                  // looping_sound,
	nullptr,                                  // animation_graph,
	nullptr,                                  // damage_effect,
	nullptr,                                  // object_definition,
	nullptr,                                  // bitmap,
	nullptr,                                  // shader,
	nullptr,                                  // render_model,
	nullptr,                                  // structure_definition,
	nullptr,                                  // lightmap_definition,
	nullptr,                                  // cinematic_definition,
	nullptr,                                  // cinematic_scene_definition,
	nullptr,                                  // bink_definition,

	nullptr,                                  // any_tag,
	nullptr,                                  // any_tag_not_resolving,

	hs_type_primitive_parser_enum,            // game_difficulty,
	hs_type_primitive_parser_enum,            // team,
	hs_type_primitive_parser_enum,            // mp_team,
	hs_type_primitive_parser_enum,            // controller,
	hs_type_primitive_parser_enum,            // button_preset,
	hs_type_primitive_parser_enum,            // joystick_preset,
	hs_type_primitive_parser_enum,            // player_character_type,
	hs_type_primitive_parser_enum,            // voice_output_setting,
	hs_type_primitive_parser_enum,            // voice_mask,
	hs_type_primitive_parser_enum,            // subtitle_setting,
	hs_type_primitive_parser_enum,            // actor_type,
	hs_type_primitive_parser_enum,            // model_state,
	hs_type_primitive_parser_enum,            // event,
	hs_type_primitive_parser_enum,            // character_physics,
	hs_type_primitive_parser_enum,            // primary_skull,
	hs_type_primitive_parser_enum,            // secondary_skull,

	nullptr,                                  // object,
	nullptr,                                  // unit,
	nullptr,                                  // vehicle,
	nullptr,                                  // weapon,
	nullptr,                                  // device,
	nullptr,                                  // scenery,
	nullptr,                                  // effect_scenery,

	nullptr,                                  // object_name,
	nullptr,                                  // unit_name,
	nullptr,                                  // vehicle_name,
	nullptr,                                  // weapon_name,
	nullptr,                                  // device_name,
	nullptr,                                  // scenery_name,
	nullptr,                                  // effect_scenery_name,

	nullptr,                                  // cinematic_lightprobe,
	nullptr,                                  // animation_budget_reference,
	nullptr,                                  // looping_sound_budget_reference,
	nullptr,                                  // sound_budget_reference,
};

