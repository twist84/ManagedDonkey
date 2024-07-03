#include "hs/hs_compile.hpp"

#include "ai/ai_orders.hpp"
#include "ai/ai_script.hpp"
#include "ai/behavior.hpp"
#include "ai/cl_engine.hpp"
#include "ai/cs_scenario_definitions.hpp"
#include "ai/styles.hpp"
#include "cache/cache_files.hpp"
#include "devices/devices.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/hs_unit_seats.hpp"
#include "scenario/scenario.hpp"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

hs_compile_globals_struct hs_compile_globals = {};

bool hs_parse_object_and_object_name_internal(long expression_index, e_hs_type byteswap_type)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

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
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not an object of type %s.", hs_type_names[expression->type.get()]);

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	if (HS_TYPE_IS_OBJECT(byteswap_type))
	{
		expression->long_value = object_name_index;
		return true;
	}

	if (HS_TYPE_IS_OBJECT_NAME(byteswap_type))
	{
		expression->short_value = object_name_index;
		return true;
	}

	hs_compile_globals.error_message = "invalid byteswap type,";
	hs_compile_globals.error_offset = expression->source_offset;
	return false;
}

bool hs_parse_boolean(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_boolean);
	ASSERT(expression->constant_type == expression->type);

	if (csstrcmp(source_offset, "true") == 0 || csstrcmp(source_offset, "on") == 0 || csstrcmp(source_offset, "1") == 0)
	{
		expression->bool_value = true;
		return true;
	}
	if (csstrcmp(source_offset, "false") == 0 || csstrcmp(source_offset, "off") == 0 || csstrcmp(source_offset, "0") == 0)
	{
		expression->bool_value = false;
		return true;
	}

	hs_compile_globals.error_message = "i expected \"true\" or \"false\".";
	hs_compile_globals.error_offset = expression->source_offset;

	expression->bool_value = false;
	return false;
}

bool hs_parse_real(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_real);
	ASSERT(expression->constant_type == expression->type);

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

	expression->real_value = static_cast<real>(atof(&hs_compile_globals.compiled_source[expression->source_offset]));
	return result;
}

bool hs_parse_integer(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_short_integer || expression->type == _hs_type_long_integer);
	ASSERT(expression->constant_type == expression->type);

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

	expression->short_value = static_cast<short>(source_value);
	if (expression->type == _hs_type_long_integer)
		expression->long_value = static_cast<long>(source_value);

	return result;
}

bool hs_parse_string(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_string);
	ASSERT(expression->constant_type == expression->type);

	expression->string_value = source_offset;
	return true;
}

bool hs_parse_script(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_script);
	ASSERT(expression->constant_type == expression->type);

	short script_index = hs_find_script_by_name(source_offset, NONE);
	if (script_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid script name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = script_index;
	hs_compile_add_reference(script_index, _reference_type_script, expression_index);
	return true;
}

bool hs_parse_string_id(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_string_id);
	ASSERT(expression->constant_type == expression->type);

	string_id retrieved_string_id = string_id_retrieve(source_offset);
	if (retrieved_string_id == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid string parameter.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->string_id_value = retrieved_string_id;
	return true;
}

bool hs_parse_unit_seat_mapping(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_unit_seat_mapping);
	ASSERT(expression->constant_type == expression->type);

	if (*source_offset)
	{
		bool valid = true;
		c_static_stack<s_hs_unit_seat_mapping, 256> seats_stack;
		tag_iterator iterator{};
		tag_iterator_new(&iterator, UNIT_TAG);
		for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
		{
			s_hs_unit_seat_mapping unit_seat_mapping{};
			unit_seat_mapping.unit_definition_tag_index = tag_index;
			if (hs_get_unit_seats_from_substring(tag_index, source_offset, &unit_seat_mapping.unit_seats))
			{
				if (seats_stack.full())
				{
					hs_compile_globals.error_message = "too many units match this seat substring";
					hs_compile_globals.error_offset = expression->source_offset;
					valid = false;
					break;
				}
				seats_stack.push_back(unit_seat_mapping);
			}
		}

		if (!valid || global_scenario_index_get() == NONE)
		{
			hs_compile_globals.error_message = "no scenario loaded";
			hs_compile_globals.error_offset = expression->source_offset;
			return false;
		}

		if (seats_stack.count() <= 0)
		{
			hs_compile_globals.error_message = "no units match this seat substring";
			hs_compile_globals.error_offset = expression->source_offset;
			return false;
		}

		s_scenario* scenario = global_scenario_get();
		long unit_seat_start_index = NONE;
		long unit_seat_mapping_count = seats_stack.count();

		if (scenario->hs_unit_seats.count() > 0)
		{
			s_hs_unit_seat_mapping* seats_blocks_begin = scenario->hs_unit_seats.begin();
			s_hs_unit_seat_mapping* seats_blocks_end = scenario->hs_unit_seats.end();
			s_hs_unit_seat_mapping* seats_stack_begin = seats_stack.get_elements();
			s_hs_unit_seat_mapping* seats_stack_end = seats_stack_begin + seats_stack.count();

			s_hs_unit_seat_mapping* found_seat = std::search(seats_blocks_begin, seats_blocks_end, seats_stack_begin, seats_stack_end, hs_unit_seat_mappings_match);
			if (found_seat != seats_blocks_end)
				unit_seat_start_index = found_seat - seats_blocks_begin;
		}

		if (unit_seat_start_index != NONE)
		{
			expression->long_value = hs_encode_unit_seat_mapping(unit_seat_start_index, unit_seat_mapping_count);
			return true;
		}
	}
	else
	{
		expression->long_value = NONE;
		return true;
	}

	return false;
}

bool hs_parse_trigger_volume(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_trigger_volume);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element_string_id(expression_index, offsetof(scenario_trigger_volume, name), global_scenario_index_get(), &global_scenario_get()->trigger_volumes, sizeof(scenario_trigger_volume)));
}

bool hs_parse_cutscene_flag(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_flag);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element_string_id(expression_index, offsetof(scenario_cutscene_flag, name), global_scenario_index_get(), &global_scenario_get()->cutscene_flags, sizeof(scenario_cutscene_flag)));
}

bool hs_parse_cutscene_camera_point(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element(expression_index, offsetof(scenario_cutscene_camera_point, name), global_scenario_index_get(), &global_scenario_get()->cutscene_camera_points, sizeof(scenario_cutscene_camera_point)));
}

bool hs_parse_cutscene_title(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element_string_id(expression_index, offsetof(s_scenario_cutscene_title, name), global_scenario_index_get(), &global_scenario_get()->cutscene_titles, sizeof(s_scenario_cutscene_title)));
}

bool hs_parse_cutscene_recording(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_recording);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element(expression_index, offsetof(recorded_animation_definition, name), global_scenario_index_get(), &global_scenario_get()->recorded_animations, sizeof(recorded_animation_definition)));
}

bool hs_parse_device_group(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_device_group);

	if (hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element(expression_index, offsetof(scenario_device_group, name), global_scenario_index_get(), &global_scenario_get()->device_groups, sizeof(scenario_device_group))))
	{
		hs_syntax_node* expression = hs_syntax_get(expression_index);
		expression->long_value = device_group_get_from_scenario_index(expression->short_value);

		return true;
	}

	return false;
}

bool hs_parse_ai(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);
	
	bool valid = false;
	if (!HS_TYPE_IS_OBJECT(expression->type))
	{
		ASSERT(hs_syntax_get(expression_index)->type == _hs_type_ai);
		ASSERT(expression->constant_type == expression->type);
	}
	else
	{
		valid = true;
	}
	
	bool ai_index_from_string_result = false;
	if (global_scenario_index_get() != NONE)
	{
		long ai_index_reference = NONE;
		if (ai_index_from_string_result = ai_index_from_string(global_scenario_get(), source_offset, &ai_index_reference))
		{
			expression->long_value = ai_index_from_string_result;
			if (valid)
				expression->constant_type = _hs_type_ai;
		}
	}
	
	if (!ai_index_from_string_result && !valid)
	{
		hs_compile_globals.error_message = "this is not a valid ai squad or squad group";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	
	return ai_index_from_string_result;
}

bool hs_parse_ai_command_list(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_ai_line);

	return false;
}

bool hs_parse_ai_command_script(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	short script_index = hs_find_script_by_name(source_offset, 0);
	if (script_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid command list script";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	if (global_scenario_get()->scripts[script_index].script_type != _hs_script_type_command_script)
	{
		hs_compile_globals.error_message = "script is not a command-script";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = script_index;
	hs_compile_add_reference(script_index, _reference_type_script, expression_index);
	return true;
}

bool hs_parse_ai_behavior(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	short behavior_index = behavior_index_by_name(source_offset);
	if (behavior_index == NONE)
	{
		hs_compile_globals.error_message = "not a valid behavior";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = behavior_index;
	return true;
}

bool hs_parse_ai_orders(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	short orders_index = orders_get_by_name(source_offset);
	if (orders_index == NONE)
	{
		hs_compile_globals.error_message = "not a valid order";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = orders_index;
	return true;
}

bool hs_parse_ai_line(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(expression->type == _hs_type_ai_line);
	ASSERT(expression->constant_type == expression->type);

	expression->string_id_value = string_id_retrieve(source_offset);
	return true;
}

bool hs_parse_starting_profile(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_starting_profile);

	return hs_check_block_index_type_and_return<short>(hs_parse_tag_block_element(expression_index, offsetof(scenario_starting_profile, name), global_scenario_index_get(), &global_scenario_get()->player_starting_profile, sizeof(scenario_starting_profile)));
}

bool hs_parse_conversation(long expression_index)
{
	return false;
}

bool hs_parse_zone_set(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_zone_set);

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	long zone_set_index = NONE;
	if (global_scenario_try_and_get())
		zone_set_index = scenario_get_zone_set_index_by_name(global_scenario_get(), source_offset, tag_name_strip_path(source_offset) == source_offset);

	if (zone_set_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid zone set name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = static_cast<short>(zone_set_index);
	return true;
}

bool hs_parse_designer_zone(long expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_designer_zone);

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	long designer_zone_index = NONE;
	if (global_scenario_try_and_get())
		designer_zone_index = scenario_get_designer_zone_index_by_name(global_scenario_get(), source_offset);

	if (designer_zone_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid zone set name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = static_cast<short>(designer_zone_index);
	return true;
}

bool hs_parse_point_ref(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);
	
	bool valid = false;
	if (global_scenario_index_get() != NONE)
	{
		ASSERT(hs_syntax_get(expression_index)->type == _hs_type_point_ref);
		ASSERT(expression->constant_type == expression->type);
	
		if (char* v7 = strrchr(source_offset, '/'))
		{
			char name[k_tag_string_length]{};
			if (v7 - source_offset < k_tag_string_length)
			{
				dword name_size = (v7 - source_offset + 1) >= k_tag_string_length ? k_tag_string_length : v7 - source_offset + 1;
				csstrnzcpy(name, source_offset, name_size);
				short point_set_index = cs_point_set_index_by_name(name);
				short point_index = NONE;
				if (point_set_index != NONE)
				{
					point_index = cs_point_index_by_name(cs_get_point_set(point_set_index), v7 + 1);
					if (point_index >= 0)
					{
						expression->long_value = (point_set_index << 16) | point_index;
						valid = true;
					}

				}
			}
		}
		else
		{
			short point_set_index = cs_point_set_index_by_name(source_offset);
			if (point_set_index != NONE)
			{
				expression->long_value = (point_set_index << 16) | UNSIGNED_SHORT_MAX;
				valid = true;
			}
		}
	}
	
	if (!valid)
	{
		hs_compile_globals.error_message = "this is not a valid point reference";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	
	return valid;
}

bool hs_parse_style(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_style);
	ASSERT(expression->constant_type == expression->type);

	long style = style_get_by_name(source_offset);
	if (style == NONE)
	{
		hs_compile_globals.error_message = "invalid style";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->long_value = style;
	return true;
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
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "cannot parse editor folder, no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	ASSERT(expression->type == _hs_type_folder);

	s_scenario* scenario = global_scenario_get();
	if (scenario->editor_folders.count() > 0)
	{
		for (long editor_folder_index = 0; editor_folder_index < scenario->editor_folders.count(); editor_folder_index++)
		{
			s_scenario_editor_folder& editor_folder = scenario->editor_folders[editor_folder_index];
			if (editor_folder.name.is_equal(source_offset))
			{
				expression->long_value = editor_folder_index;
				return true;
			}
		}
	}

	hs_compile_globals.error_message = "folder not found";
	hs_compile_globals.error_offset = expression->source_offset;
	return false;
}

bool hs_parse_sound_tag_reference(long expression_index)
{
	if (!hs_parse_tag_reference(expression_index))
	{
		hs_syntax_get(expression_index)->long_value = NONE;
		hs_compile_globals.error_message = 0;
	}

	return true;
}

bool hs_parse_tag_reference(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;
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
				expression->long_value = tag_reference.reference.index;
				break;
			}
		}
	}

	if (expression->long_value == NONE && !hs_compile_globals.__unknown421)
	{
		bool v11 = false;
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			long tag_name_length = extension_offset - source_offset;
			if (hs_compile_get_tag_by_name(extension, &group_tag))
			{
				c_static_string<256> tag_name;
				tag_name.set_bounded(source_offset, tag_name_length);
				expression->long_value = tag_loaded(group_tag, tag_name.get_string());
			}
		}

		if (expression->long_value == NONE)
			expression->long_value = tag_loaded(group_tag, source_offset);
	}

	return true;
}

bool hs_parse_tag_reference_not_resolving(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;

		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	if (expression->long_value == NONE && !hs_compile_globals.__unknown421)
	{
		tag group_tag = NONE;
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			long tag_name_length = extension_offset - source_offset;
			if (hs_compile_get_tag_by_name(extension, &group_tag))
			{
				c_static_string<256> tag_name;
				tag_name.set_bounded(source_offset, tag_name_length);
				expression->long_value = tag_loaded(group_tag, tag_name.get_string());
			}
		}
	}

	if (expression->long_value == NONE)
	{
		hs_compile_globals.error_message = "not a loaded tag";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	return true;
}

bool hs_parse_enum(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (!HS_TYPE_IS_ENUM(expression->type))
	{
		csnzprintf(
			hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"corrupt enum expression (type %d constant-type %d)",
			expression->type.get(),
			expression->constant_type.get());

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	hs_enum_definition const* enum_definition = &hs_enum_table[expression->type.get() - _hs_type_game_difficulty];
	ASSERT(enum_definition->count);

	short i = 0;
	for (; i < enum_definition->count && csstricmp(source_offset, enum_definition->names[i]); i++);

	if (i == enum_definition->count)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"%s must be ", hs_type_names[expression->type.get()]);

		for (i = 0; i < enum_definition->count - 1; i++)
		{
			csstrnzcat(hs_compile_globals.error_buffer, "\"", k_hs_compile_error_buffer_size);
			csstrnzcat(hs_compile_globals.error_buffer, enum_definition->names[i], k_hs_compile_error_buffer_size);
			csstrnzcat(hs_compile_globals.error_buffer, "\", ", k_hs_compile_error_buffer_size);
		}

		if (enum_definition->count > 1)
			csstrnzcat(hs_compile_globals.error_buffer, "or ", k_hs_compile_error_buffer_size);

		csstrnzcat(hs_compile_globals.error_buffer, "\"", k_hs_compile_error_buffer_size);
		csstrnzcat(hs_compile_globals.error_buffer, enum_definition->names[i], k_hs_compile_error_buffer_size);
		csstrnzcat(hs_compile_globals.error_buffer, "\".", k_hs_compile_error_buffer_size);

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = i;
	return true;
}

bool hs_parse_object(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(HS_TYPE_IS_OBJECT(expression->type));

	if (csstrcmp(&hs_compile_globals.compiled_source[expression->source_offset], "none") == 0)
	{
		expression->long_value = NONE;
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
	return hs_parse_object_and_object_name_internal(expression_index, _hs_type_object_name);
}

bool hs_parse_cinematic_lightprobe(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cinematic_lightprobe);
	ASSERT(expression->constant_type == expression->type);

	string_id name = string_id_retrieve(source_offset);

	long cinematic_lighting_palette_entry = scenario_cinematic_lighting_palette_entry_get_by_name(global_scenario_get(), name);
	if (cinematic_lighting_palette_entry != NONE)
	{
		expression->long_value = cinematic_lighting_palette_entry;
		return true;
	}

	hs_compile_globals.error_message = "this is not a lightprobe name.";
	hs_compile_globals.error_offset = expression->source_offset;
	return false;
}

bool hs_parse_budget_reference(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	if (global_scenario_index_get() == NONE)
	{
		ASSERT(HS_TYPE_IS_BUDGET_REFERENCE(expression->type));

		s_scenario* scenario = global_scenario_get();
		if (scenario->budget_references.count() <= 0)
			return true;

		for (long budget_reference_index = 0; budget_reference_index < scenario->budget_references.count(); budget_reference_index++)
		{
			s_scenario_budget_reference& budget_reference = scenario->budget_references[budget_reference_index];
			if (budget_reference.reference.index != NONE && csstrcmp(budget_reference.reference.get_name(), source_offset) == 0)
			{
				expression->long_value = budget_reference.reference.index;
				return true;
			}
		}
	}

	expression->long_value = NONE;
	return true;
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

bool hs_parse_variable(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);
	
	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_unparsed);
	
	bool valid = false;
	short type = NONE;
	bool v9 = false;
	if (hs_compile_globals.current_script_index != NONE && global_scenario_index_get() != NONE)
	{
		expression->short_value = hs_script_find_parameter_by_name(hs_compile_globals.current_script_index, source_offset);
		if (expression->short_value != NONE)
		{
			hs_script& script = global_scenario_get()->scripts[hs_compile_globals.current_script_index];
			type = script.parameters[expression->short_value].return_type.get();
	
			v9 = true;
			valid = true;
		}
	}
	
	if (!valid && (!hs_compile_globals.__unknown424
		|| expression->type.get() == NONE
		|| expression->short_value == NONE
		|| !expression->flags.test(_hs_syntax_node_unknown_bit4)))
	{
		expression->short_value = hs_find_global_by_name(source_offset);
		if (expression->short_value != NONE)
		{
			type = hs_global_get_type(expression->short_value);
			valid = true;
		}
	}
	
	if (!valid)
		return false;
	
	ASSERT(type != NONE);
	if (expression->type.get() && !hs_can_cast(type, expression->type))
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"i expected a value of type %s, but the variable %s has type %s",
			hs_type_names[expression->type.get()],
			hs_global_get_name(expression->short_value),
			hs_type_names[type]);
	
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
	
		return false;
	}
	else
	{
		if (!expression->type)
			expression->type = type;
	
		expression->flags.set(_hs_syntax_node_variable_bit, true);
	
		if (v9)
			expression->flags.set(_hs_syntax_node_unknown_bit4, true);
		else
			hs_compile_add_reference(expression->long_value, _reference_type_global, expression_index);
	
		return true;
	}
	
	if (!hs_compile_globals.__unknown424)
		return false;
	
	if (expression->type.get() == NONE || expression->long_value == NONE || !expression->flags.test(_hs_syntax_node_unknown_bit4))
	{
		hs_compile_globals.error_message = "this is not a valid variable name.";
		hs_compile_globals.error_offset = expression->source_offset;
	
		return false;
	}
	
	return true;
}

bool hs_parse_primitive(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

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

	csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
		"expressions of type %s are currently unsupported.", hs_type_names[expression->type.get()]);

	hs_compile_globals.error_message = hs_compile_globals.error_buffer;
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_parse_nonprimitive(long expression_index)
{
	bool parse_result = false;

	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	hs_compile_globals.some_reference_count++;

	hs_syntax_node* special_form_expression = hs_syntax_get(expression->long_value);
	if (special_form_expression->flags.test(_hs_syntax_node_primitive_bit))
	{
		ASSERT(expression->type != _hs_special_form);

		bool is_function = false;
		bool is_script = false;
		hs_parse_call_predicate(expression_index, &is_function, &is_script);

		if (expression->constant_type == NONE)
		{
			if (is_function)
			{
				hs_compile_globals.error_message = "wrong number of arguments for function";
				hs_compile_globals.error_offset = special_form_expression->source_offset;
			}
			else
			{
				hs_compile_globals.error_message = is_script ? "wrong number of arguments for script" : "this is not a valid function or script name.";
				hs_compile_globals.error_offset = special_form_expression->source_offset;
			}
		}
		else if (expression->flags.test(_hs_syntax_node_script_bit))
		{
			hs_script& script = global_scenario_get()->scripts[expression->script_index];
			if (script.script_type != _hs_script_type_static)
			{
				hs_compile_globals.error_message = "this is not a static script.";
				hs_compile_globals.error_offset = expression->source_offset;
				return false;
			}

			if (expression->type && !hs_can_cast(script.return_type.get(), expression->type.get()))
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"i expected a %s, but this script returns a %s.",
					hs_type_names[expression->type.get()],
					hs_type_names[script.return_type.get()]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
				return false;
			}

			if (!expression->type.get())
				expression->type = script.return_type;

			short script_argument_count = hs_count_children(expression_index) - 1;
			if (script_argument_count != script.parameters.count())
			{
				hs_compile_globals.error_message = "wrong number of script arguments";
				hs_compile_globals.error_offset = expression->source_offset;
				return false;
			}

			short parameter_index = 0;
			long next_node_index = special_form_expression->next_node_index;
			parse_result = true;
			while (next_node_index != NONE && parse_result)
			{
				ASSERT(parameter_index >= 0 && parameter_index < script.parameters.count());

				hs_script_parameter& parameter = script.parameters[parameter_index];
				parse_result = hs_parse(next_node_index, parameter.return_type.get());
				next_node_index = hs_syntax_get(next_node_index)->next_node_index;

				parameter_index++;
			}

			if (parse_result)
				hs_compile_add_reference(expression->constant_type.get(), _reference_type_script, expression_index);
		}
		else
		{
			hs_function_definition_debug const* function = hs_function_get_debug(expression->constant_type.get());
			if (expression->type.get() && !hs_can_cast(function->return_type.get(), expression->type.get()))
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"i expected a %s, but this function returns a %s.",
					hs_type_names[expression->type.get()],
					hs_type_names[function->return_type.get()]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.illegal_block
				&& (expression->constant_type.get() == _hs_type_ai || expression->constant_type.get() == _hs_type_ai_command_script))
			{
				hs_compile_globals.error_message = "it is illegal to block in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.illegal_variable_value_set && expression->constant_type.get() == _hs_type_void)
			{
				hs_compile_globals.error_message = "it is illegal to set the value of variables in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (!TEST_BIT(function->flags, 8)
				|| hs_compile_globals.current_script_index == NONE
				|| global_scenario_get()->scripts[hs_compile_globals.current_script_index].script_type.get() == _hs_script_type_command_script)
			{
				if (!TEST_BIT(function->flags, 10) || hs_compile_globals.current_script_index == NONE)
				{
					if (!expression->type.get() && function->return_type.get() != _hs_passthrough)
						expression->type = function->return_type;

					ASSERT(function->parse);
					parse_result = function->parse(expression->function_index, expression_index);
				}
				else
				{
					hs_compile_globals.error_message = "it is illegal to call this function from a script file";
					hs_compile_globals.error_offset = expression->source_offset;
				}
			}
			else
			{
				hs_compile_globals.error_message = "it is illegal to call a command script atom from outside a command script";
				hs_compile_globals.error_offset = expression->source_offset;
			}
		}
	}
	else
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"i expected %s, but i got an expression.",
			expression->type.get() == _hs_special_form ? "\"script\" or \"global\"" : "a function name");

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
	}

	hs_compile_globals.some_reference_count--;

	return parse_result;
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
	hs_function_definition_debug const* definition = hs_function_get_debug(function_index);
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(hs_type_valid(definition->return_type.get()));

	bool has_remaining_arguments = true;
	short parameter_index;
	for (parameter_index = 0; has_remaining_arguments && parameter_index < definition->formal_parameter_count && next_node_index != NONE; parameter_index++)
	{
		hs_syntax_node* next_expression = hs_syntax_get(next_node_index);
		if (hs_parse(next_node_index, definition->formal_parameters[parameter_index]))
		{
			next_node_index = next_expression->next_node_index;
			continue;
		}

		if (next_expression->type == _hs_type_string_id && hs_syntax_get(next_node_index)->flags.test(_hs_syntax_node_primitive_bit))
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"this is not a valid string for '%s'", definition->name);
			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = next_expression->source_offset;
		}

		has_remaining_arguments = false;
	}

	if (!has_remaining_arguments || (parameter_index == definition->formal_parameter_count && next_node_index == NONE))
		return true;

	csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
		"the \"%s\" call requires exactly %d arguments.", definition->name, definition->formal_parameter_count);
	hs_compile_globals.error_message = hs_compile_globals.error_buffer;
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_compile_get_tag_by_name(char const* group_name, tag* group_tag_out)
{
	tag group_tag = group_name_to_group_tag(group_name);
	if (group_tag == NONE)
		return false;

	*group_tag_out = group_tag;
	return true;
}

short hs_count_children(long expression_index)
{
	short child_count = 0;

	for (long next_node_index = hs_syntax_get(expression_index)->long_value;
		next_node_index != NONE;
		next_node_index = hs_syntax_get(next_node_index)->next_node_index)
	{
		child_count++;
	}

	return child_count;
}

void hs_compile_add_reference(long referred_index, e_reference_type reference_type, long expression_index)
{
	if (reference_type || (referred_index & 0x8000) == 0)
	{
		ASSERT((hs_compile_globals.current_script_index != NONE) ^ (hs_compile_globals.current_global_index != NONE));

		e_reference_type current_reference_type = _reference_type_global;
		long current_index = hs_compile_globals.current_global_index;
		if (hs_compile_globals.current_script_index != NONE)
		{
			current_reference_type = _reference_type_script;
			current_index = hs_compile_globals.current_script_index;
		}

		hs_compile_globals_reference_struct* reference = &hs_compile_globals.references[hs_compile_globals.reference_count++];
		reference->reference_type = current_reference_type;
		reference->index = current_index;
		reference->expression_index = expression_index;
		reference->__unknown10 = NULL;

		switch (reference_type)
		{
		case _reference_type_global:
		{
			ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_globals_per_scenario));

			reference->__unknown10 = (*hs_compile_globals.global_references)[referred_index];
			(*hs_compile_globals.global_references)[referred_index] = reference;
			reference->has_return_type = true;
		}
		break;
		case _reference_type_script:
		{
			ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_scripts_per_scenario));

			reference->__unknown10 = (*hs_compile_globals.script_references)[referred_index];
			(*hs_compile_globals.script_references)[referred_index] = reference;

			reference->has_return_type = global_scenario_get()->scripts[referred_index].return_type != _hs_type_void;
		}
		break;
		default:
			ASSERT2("unreachable");
		}
	}
}

void hs_parse_call_predicate(long expression_index, bool* is_function, bool* is_script)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long predicate_index = expression->long_value;
	hs_syntax_node* predicate = hs_syntax_get(predicate_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + predicate->source_offset, char*, source_offset);

	if (predicate->type.get() == _hs_function_name)
	{
		ASSERT(predicate->function_index != NONE);
		expression->constant_type = predicate->constant_type;
	}
	else
	{
		short parameter_count = hs_count_children(expression_index) - 1;
		expression->constant_type = hs_find_function_by_name(source_offset, parameter_count);
		predicate->type = _hs_function_name;

		if (expression->constant_type.get() == NONE)
		{
			expression->constant_type = hs_find_script_by_name(source_offset, parameter_count);
			if (expression->constant_type == NONE)
			{
				if (is_function)
					*is_function = hs_find_function_by_name(source_offset, parameter_count) != NONE;

				if (is_script)
					*is_script = hs_find_script_by_name(source_offset, parameter_count) != NONE;
			}
			else
			{
				expression->flags.set(_hs_syntax_node_script_bit, true);

				if (is_script)
					*is_script = true;
			}
		}
		else if (is_function)
		{
			*is_function = true;
		}

		predicate->constant_type = expression->constant_type;
	}
}

bool hs_parse_tag_block_element_string_id(long expression_index, long offset, long scenario_index, s_tag_block* block, long element_size)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT((offset + (long)sizeof(string_id)) <= element_size);

	bool valid = false;
	for (long block_index = 0; block_index < block->count; block_index++)
	{
		string_id block_element_string_id = *reinterpret_cast<string_id*>(static_cast<byte*>(tag_block_get_element_with_size(block, block_index, element_size)) + offset);
		if (block_element_string_id == string_id_retrieve(source_offset))
		{
			expression->short_value = static_cast<short>(block_index);
			valid = true;
			break;
		}
	}

	if (!valid)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type.get()]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (hs_compile_globals.__unknown421)
	{
		if (scenario_index == global_scenario_index_get())
		{
			//editor_register_script_referenced_block(block);
		}
		else
		{
			//tag_group_dependencies_register_dependency(global_scenario_index_get(), scenario_index);
		}
	}

	return true;
}

bool hs_parse_tag_block_element(long expression_index, long offset, long scenario_index, s_tag_block* block, long element_size)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	REFERENCE_DECLARE(hs_compile_globals.compiled_source + expression->source_offset, char*, source_offset);

	ASSERT(element_size <= SHRT_MAX);
	ASSERT(offset + (k_tag_string_length - 1) < element_size);

	bool valid = false;
	for (long block_index = 0; block_index < block->count; block_index++)
	{
		char const* block_element = static_cast<char const*>(tag_block_get_element_with_size(block, block_index, element_size));
		if (ascii_stricmp(block_element + offset, source_offset) == 0)
		{
			expression->short_value = static_cast<short>(block_index);
			valid = true;
			break;
		}
	}

	if (!valid)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type.get()]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (hs_compile_globals.__unknown421)
	{
		if (scenario_index == global_scenario_index_get())
		{
			//editor_register_script_referenced_block(block);
		}
		else
		{
			//tag_group_dependencies_register_dependency(global_scenario_index_get(), scenario_index);
		}
	}

	return true;
}

