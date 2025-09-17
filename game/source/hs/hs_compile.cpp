#include "hs/hs_compile.hpp"

#include "ai/ai_orders.hpp"
#include "ai/ai_script.hpp"
#include "ai/behavior.hpp"
#include "ai/cl_engine.hpp"
#include "ai/cs_scenario_definitions.hpp"
#include "ai/styles.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/progress.hpp"
#include "devices/devices.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_library_internal_compile.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/hs_unit_seats.hpp"
#include "main/console.hpp"
#include "scenario/scenario.hpp"

#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// $TODO completely reorganize this file

s_hs_compile_globals hs_compile_globals{};

struct c_hs_compile_error_listener
{
public:
	virtual ~c_hs_compile_error_listener() = 0;
	virtual void handle_error(char const*, int32, const char*, const char*, c_wrapped_array<const char>*, const char*) = 0;
};

static struct
{
	static const int32 k_maximum_hs_compile_error_listeners = 1;
	c_hs_compile_error_listener* compile_error_listeners[k_maximum_hs_compile_error_listeners];
} hs_static_globals{};

bool hs_compile_register_error_listener(c_hs_compile_error_listener* listener)
{
	bool successfully_added = 0;
	for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
	{
		if (hs_static_globals.compile_error_listeners[compile_error_listener_index] == NULL)
		{
			hs_static_globals.compile_error_listeners[compile_error_listener_index] = listener;

			successfully_added = true;
			break;
		}
	}
	return successfully_added;
}

void hs_compile_unregister_error_listener(c_hs_compile_error_listener* listener)
{
	for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
	{
		if (hs_static_globals.compile_error_listeners[compile_error_listener_index] == listener)
		{
			hs_static_globals.compile_error_listeners[compile_error_listener_index] = NULL;
		}
	}
}

enum
{
	_skip_whitespace_state_no_comment = 0,
	_skip_whitespace_state_comment,
	_skip_whitespace_state_block_comment,
	_skip_whitespace_state_done,

	NUMBER_OF_SKIP_WHITESPACE_STATES
};

const char whitespace_characters[] = { ' ', '\t' };
const char eol_characters[] = { '\n', '\r' };

bool character_in_list(char c, int16 list_count, const char* list)
{
	for (int16 list_index = 0; list_index < list_count; list_index++)
	{
		if (c == list[list_index])
		{
			return true;
		}
	}
	return false;
}

void skip_whitespace(char** c)
{
	int32 state = _skip_whitespace_state_no_comment;
	while (state != _skip_whitespace_state_done)
	{
		if (state)
		{
			if (state == _skip_whitespace_state_comment)
			{
				if (**c)
				{
					if (character_in_list(**c, NUMBEROF(eol_characters), eol_characters))
					{
						state = _skip_whitespace_state_no_comment;
						++*c;
					}
					else
					{
						state = _skip_whitespace_state_done;
					}
				}
			}
			else if (state < _skip_whitespace_state_done)
			{
				if (**c)
				{
					if (**c == '*' && (*c)[1] == ';')
					{
						state = _skip_whitespace_state_no_comment;
						++*c;
					}
					++ * c;
				}
				else
				{
					state = _skip_whitespace_state_done;
				}
			}
			else UNREACHABLE();
		}
		else if (**c == ';')
		{
			state = _skip_whitespace_state_comment;
			if (*++ * c == '*')
			{
				state = _skip_whitespace_state_done;
				++*c;
			}
		}
		else if (character_in_list(**c, NUMBEROF(whitespace_characters), whitespace_characters) || character_in_list(**c, NUMBEROF(eol_characters), eol_characters))
		{
			++*c;
		}
		else
		{
			state = _skip_whitespace_state_done;
		}
	}
}

bool hs_parse_object_and_object_name_internal(int32 expression_index, e_hs_type byteswap_type)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	ASSERT(HS_TYPE_IS_OBJECT(byteswap_type) || HS_TYPE_IS_OBJECT_NAME(byteswap_type));
	ASSERT(HS_TYPE_IS_OBJECT_NAME(expression->type));

	int16 object_name_index = scenario_object_name_index_from_string(global_scenario_get(), source_offset);
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

	if (!TEST_BIT(hs_object_type_masks[expression->type - _hs_type_object_name], object_name.object_type))
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not an object of type %s.", hs_type_names[expression->type]);

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

bool hs_parse_boolean(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_parse_real(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_real);
	ASSERT(expression->constant_type == expression->type);

	if (*source_offset == '-')
	{
		source_offset++;
	}

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

	expression->real_value = (real32)atof(&hs_compile_globals.compiled_source[expression->source_offset]);
	return result;
}

bool hs_parse_integer(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_short_integer || expression->type == _hs_type_long_integer);
	ASSERT(expression->constant_type == expression->type);

	if (*source_offset == '-')
	{
		source_offset++;
	}

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

	int32 source_value = atoi(&hs_compile_globals.compiled_source[expression->source_offset]);
	if (result && !(expression->type == _hs_type_short_integer || IN_RANGE_INCLUSIVE(source_value, -32767, 32768)))
	{
		hs_compile_globals.error_message = "shorts must be in the range [-32767, 32768].";
		hs_compile_globals.error_offset = expression->source_offset;
		result = false;
	}

	expression->short_value = (int16)source_value;
	if (expression->type == _hs_type_long_integer)
	{
		expression->long_value = (int32)source_value;
	}

	return result;
}

bool hs_parse_string(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string);
	ASSERT(expression->constant_type == expression->type);

	expression->string_value = source_offset;
	return true;
}

bool hs_parse_script(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_script);
	ASSERT(expression->constant_type == expression->type);

	int16 script_index = hs_find_script_by_name(source_offset, NONE);
	if (script_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid script name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = script_index;
	hs_compile_add_reference(script_index, _hs_reference_type_script, expression_index);
	return true;
}

bool hs_parse_string_id(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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

bool hs_parse_unit_seat_mapping(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_unit_seat_mapping);
	ASSERT(expression->constant_type == expression->type);

	if (*source_offset)
	{
		bool valid = true;
		c_static_stack<s_hs_unit_seat_mapping, 256> seats_stack;
		tag_iterator iterator{};
		tag_iterator_new(&iterator, UNIT_TAG);
		for (int32 tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
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

		struct scenario* scenario = global_scenario_get();
		int32 unit_seat_start_index = NONE;
		int32 unit_seat_mapping_count = seats_stack.count();

		if (scenario->hs_unit_seats.count > 0)
		{
			s_hs_unit_seat_mapping* seats_blocks_begin = scenario->hs_unit_seats.begin();
			s_hs_unit_seat_mapping* seats_blocks_end = scenario->hs_unit_seats.end();
			s_hs_unit_seat_mapping* seats_stack_begin = seats_stack.get_elements();
			s_hs_unit_seat_mapping* seats_stack_end = seats_stack_begin + seats_stack.count();

			s_hs_unit_seat_mapping* found_seat = std::search(seats_blocks_begin, seats_blocks_end, seats_stack_begin, seats_stack_end, hs_unit_seat_mappings_match);
			if (found_seat != seats_blocks_end)
			{
				unit_seat_start_index = found_seat - seats_blocks_begin;
			}
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

bool hs_parse_trigger_volume(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_trigger_volume);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, OFFSETOF(scenario_trigger_volume, name), global_scenario_index_get(), &global_scenario_get()->trigger_volumes, sizeof(scenario_trigger_volume)));
}

bool hs_parse_cutscene_flag(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_flag);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, OFFSETOF(scenario_cutscene_flag, name), global_scenario_index_get(), &global_scenario_get()->cutscene_flags, sizeof(scenario_cutscene_flag)));
}

bool hs_parse_cutscene_camera_point(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, OFFSETOF(scenario_cutscene_camera_point, name), global_scenario_index_get(), &global_scenario_get()->cutscene_camera_points, sizeof(scenario_cutscene_camera_point)));
}

bool hs_parse_cutscene_title(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, OFFSETOF(s_scenario_cutscene_title, name), global_scenario_index_get(), &global_scenario_get()->cutscene_titles, sizeof(s_scenario_cutscene_title)));
}

bool hs_parse_cutscene_recording(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_recording);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, OFFSETOF(recorded_animation_definition, name), global_scenario_index_get(), &global_scenario_get()->recorded_animations, sizeof(recorded_animation_definition)));
}

bool hs_parse_device_group(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_device_group);

	if (hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, OFFSETOF(scenario_device_group, name), global_scenario_index_get(), &global_scenario_get()->device_groups, sizeof(scenario_device_group))))
	{
		hs_syntax_node* expression = hs_syntax_get(expression_index);
		expression->long_value = device_group_get_from_scenario_index(expression->short_value);

		return true;
	}

	return false;
}

bool hs_parse_ai(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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
		int32 ai_index_reference = NONE;
		if (ai_index_from_string_result = ai_index_from_string(global_scenario_get(), source_offset, &ai_index_reference))
		{
			expression->long_value = ai_index_from_string_result;
			if (valid)
			{
				expression->constant_type = _hs_type_ai;
			}
		}
	}

	if (!ai_index_from_string_result && !valid)
	{
		hs_compile_globals.error_message = "this is not a valid ai squad or squad group";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return ai_index_from_string_result;
}

bool hs_parse_ai_command_list(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_ai_line);

	return false;
}

bool hs_parse_ai_command_script(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 script_index = hs_find_script_by_name(source_offset, 0);
	if (script_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid command list script";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	if (global_scenario_get()->scripts[script_index].script_type != _hs_script_command_script)
	{
		hs_compile_globals.error_message = "script is not a command-script";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = script_index;
	hs_compile_add_reference(script_index, _hs_reference_type_script, expression_index);
	return true;
}

bool hs_parse_ai_behavior(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 behavior_index = behavior_index_by_name(source_offset);
	if (behavior_index == NONE)
	{
		hs_compile_globals.error_message = "not a valid behavior";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = behavior_index;
	return true;
}

bool hs_parse_ai_orders(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 orders_index = orders_get_by_name(source_offset);
	if (orders_index == NONE)
	{
		hs_compile_globals.error_message = "not a valid order";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = orders_index;
	return true;
}

bool hs_parse_ai_line(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_ai_line);
	ASSERT(expression->constant_type == expression->type);

	expression->string_id_value = string_id_retrieve(source_offset);
	return true;
}

bool hs_parse_starting_profile(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_starting_profile);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, OFFSETOF(scenario_starting_profile, name), global_scenario_index_get(), &global_scenario_get()->player_starting_profile, sizeof(scenario_starting_profile)));
}

bool hs_parse_conversation(int32 expression_index)
{
	return false;
}

bool hs_parse_zone_set(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_zone_set);

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	int32 zone_set_index = NONE;
	if (global_scenario_try_and_get())
	{
		zone_set_index = scenario_get_zone_set_index_by_name(global_scenario_get(), source_offset, tag_name_strip_path(source_offset) == source_offset);
	}

	if (zone_set_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid zone set name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = (int16)zone_set_index;
	return true;
}

bool hs_parse_designer_zone(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_designer_zone);

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	int32 designer_zone_index = NONE;
	if (global_scenario_try_and_get())
	{
		designer_zone_index = scenario_get_designer_zone_index_by_name(global_scenario_get(), source_offset);
	}

	if (designer_zone_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid zone set name.";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->short_value = (int16)designer_zone_index;
	return true;
}

bool hs_parse_point_ref(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

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
				uns32 name_size = (v7 - source_offset + 1) >= k_tag_string_length ? k_tag_string_length : v7 - source_offset + 1;
				csstrnzcpy(name, source_offset, name_size);
				int16 point_set_index = cs_point_set_index_by_name(name);
				int16 point_index = NONE;
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
			int16 point_set_index = cs_point_set_index_by_name(source_offset);
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

bool hs_parse_style(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_style);
	ASSERT(expression->constant_type == expression->type);

	int32 style = style_get_by_name(source_offset);
	if (style == NONE)
	{
		hs_compile_globals.error_message = "invalid style";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	expression->long_value = style;
	return true;
}

bool hs_parse_object_list(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_object_list);

	expression->constant_type = _hs_type_object_name;
	expression->type = _hs_type_object_name;
	bool result = hs_parse_object_and_object_name_internal(expression_index, (e_hs_type)expression->constant_type);
	expression->type = _hs_type_object_list;

	return result;
}

bool hs_parse_folder(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "cannot parse editor folder, no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	ASSERT(expression->type == _hs_type_folder);

	struct scenario* scenario = global_scenario_get();
	if (scenario->editor_folders.count > 0)
	{
		for (int32 editor_folder_index = 0; editor_folder_index < scenario->editor_folders.count; editor_folder_index++)
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

bool hs_parse_sound_tag_reference(int32 expression_index)
{
	if (!hs_parse_tag_reference(expression_index))
	{
		hs_syntax_get(expression_index)->long_value = NONE;
		hs_compile_globals.error_message = 0;
	}

	return true;
}

bool hs_parse_tag_reference(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;
		return true;
	}

	ASSERT(HS_TYPE_IS_TAG_REFERENCE(expression->type));

	tag group_tag = hs_tag_reference_type_group_tags[expression->type - _hs_type_sound];

	for (hs_tag_reference& tag_reference : global_scenario_get()->references)
	{
		if (tag_reference.reference.index != NONE)
		{
			const char* tag_name = tag_get_name(tag_reference.reference.index);
			if (csstrcmp(tag_name, source_offset) == 0 && tag_reference.reference.group_tag == group_tag)
			{
				expression->long_value = tag_reference.reference.index;
				break;
			}
		}
	}

	if (expression->long_value == NONE && !hs_compile_globals.permanent)
	{
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			int32 tag_name_length = extension_offset - source_offset;
			if (hs_compile_get_tag_by_name(extension, &group_tag))
			{
				c_static_string<256> tag_name;
				tag_name.set_bounded(source_offset, tag_name_length);
				expression->long_value = tag_loaded(group_tag, tag_name.get_string());
			}
		}

		if (expression->long_value == NONE)
		{
			expression->long_value = tag_loaded(group_tag, source_offset);
		}
	}

	return true;
}

bool hs_parse_tag_reference_not_resolving(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;

		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	if (expression->long_value == NONE && !hs_compile_globals.permanent)
	{
		tag group_tag = NONE;
		if (char* extension_offset = strrchr(source_offset, '.'))
		{
			char* extension = extension_offset + 1;
			int32 tag_name_length = extension_offset - source_offset;
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

bool hs_parse_enum(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (!HS_TYPE_IS_ENUM(expression->type))
	{
		csnzprintf(
			hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"corrupt enum expression (type %d constant-type %d)",
			expression->type,
			expression->constant_type);

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		return false;
	}

	const hs_enum_definition* enum_definition = &hs_enum_table[expression->type - _hs_type_game_difficulty];
	ASSERT(enum_definition->count);

	int16 i = 0;
	for (; i < enum_definition->count && csstricmp(source_offset, enum_definition->names[i]); i++);

	if (i == enum_definition->count)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"%s must be ", hs_type_names[expression->type]);

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

bool hs_parse_object(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(HS_TYPE_IS_OBJECT(expression->type));

	if (csstrcmp(&hs_compile_globals.compiled_source[expression->source_offset], "none") == 0)
	{
		expression->long_value = NONE;
		return true;
	}

	expression->type += NUMBER_OF_HS_SCRIPT_TYPES;
	expression->constant_type = expression->type;
	bool result = hs_parse_object_and_object_name_internal(expression_index, (e_hs_type)expression->constant_type);
	expression->type -= NUMBER_OF_HS_SCRIPT_TYPES;

	if (!result && hs_parse_ai(expression_index))
	{
		hs_compile_globals.error_message = NULL;
		hs_compile_globals.error_offset = NONE;
		result = true;
	}

	return result;
}

bool hs_parse_object_name(int32 expression_index)
{
	return hs_parse_object_and_object_name_internal(expression_index, _hs_type_object_name);
}

bool hs_parse_cinematic_lightprobe(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cinematic_lightprobe);
	ASSERT(expression->constant_type == expression->type);

	string_id name = string_id_retrieve(source_offset);

	int32 cinematic_lighting_palette_entry = scenario_cinematic_lighting_palette_entry_get_by_name(global_scenario_get(), name);
	if (cinematic_lighting_palette_entry != NONE)
	{
		expression->long_value = cinematic_lighting_palette_entry;
		return true;
	}

	hs_compile_globals.error_message = "this is not a lightprobe name.";
	hs_compile_globals.error_offset = expression->source_offset;
	return false;
}

bool hs_parse_budget_reference(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		ASSERT(HS_TYPE_IS_BUDGET_REFERENCE(expression->type));

		struct scenario* scenario = global_scenario_get();
		if (scenario->budget_references.count <= 0)
			return true;

		for (int32 budget_reference_index = 0; budget_reference_index < scenario->budget_references.count; budget_reference_index++)
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
	NULL,                                 // unparsed
	NULL,                                 // special_form
	NULL,                                 // function_name
	NULL,                                 // passthrough

	NULL,                                 // void
	hs_parse_boolean,                     // boolean
	hs_parse_real,                        // real32
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

bool hs_parse_variable(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = hs_compile_globals.compiled_source + expression->source_offset;

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_unparsed);

	bool valid = false;
	int16 type = NONE;
	bool is_parameter = false;
	if (hs_compile_globals.current_script_index != NONE && global_scenario_index_get() != NONE)
	{
		expression->short_value = hs_script_find_parameter_by_name(hs_compile_globals.current_script_index, source_offset);
		if (expression->short_value != NONE)
		{
			hs_script& script = global_scenario_get()->scripts[hs_compile_globals.current_script_index];
			type = script.parameters[expression->short_value].type;

			is_parameter = true;
			valid = true;
		}
	}

	if (!valid && (!hs_compile_globals.variables_predetermined
		|| expression->type == NONE
		|| expression->short_value == NONE
		|| !TEST_BIT(expression->flags, _hs_syntax_node_parameter_bit)))
	{
		expression->short_value = hs_find_global_by_name(source_offset);
		if (expression->short_value != NONE)
		{
			type = hs_global_get_type(expression->short_value);
			valid = true;
		}
	}

	if (!valid)
	{
		return false;
	}

	ASSERT(type != NONE);
	if (expression->type && !hs_can_cast(type, expression->type))
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"i expected a value of type %s, but the variable %s has type %s",
			hs_type_names[expression->type],
			hs_global_get_name(expression->short_value),
			hs_type_names[type]);

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}
	else
	{
		if (expression->type == _hs_unparsed)
		{
			expression->type = type;
		}

		SET_BIT(expression->flags, _hs_syntax_node_variable_bit, true);

		if (is_parameter)
		{
			SET_BIT(expression->flags, _hs_syntax_node_parameter_bit, true);
		}
		else
		{
			hs_compile_add_reference(expression->long_value, _hs_reference_type_global, expression_index);
		}

		return true;
	}

	if (!hs_compile_globals.variables_predetermined)
	{
		return false;
	}

	if (expression->type == NONE || expression->long_value == NONE || !TEST_BIT(expression->flags, _hs_syntax_node_parameter_bit))
	{
		hs_compile_globals.error_message = "this is not a valid variable name.";
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	return true;
}

bool hs_parse_primitive(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	bool success = false;
	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	if (expression->type == _hs_special_form)
	{
		hs_compile_globals.error_message = "i expected a script or variable definition.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else if (expression->type == _hs_type_void)
	{
		hs_compile_globals.error_message = "the value of this expression (in a <void> slot) can never be used.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		if (!hs_compile_globals.variables_predetermined || TEST_BIT(expression->flags, _hs_syntax_node_variable_bit))
		{
			success = hs_parse_variable(expression_index);
		}

		if (!success
			&& expression->type
			&& !hs_compile_globals.error_message
			&& (!hs_compile_globals.variables_predetermined || !TEST_BIT(expression->flags, _hs_syntax_node_variable_bit)))
		{
			if (hs_type_primitive_parsers[expression->type])
			{
				success = hs_type_primitive_parsers[expression->type](expression_index);
			}
			else
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"expressions of type %s are currently unsupported.",
					hs_type_names[expression->type]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
				success = false;
			}
		}
	}

	return success;
}

bool hs_parse_nonprimitive(int32 expression_index)
{
	bool success = false;

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 predicate_index = hs_syntax_get(expression_index)->long_value;
	hs_syntax_node* predicate = hs_syntax_get(predicate_index);

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	hs_compile_globals.indent++;

	if (TEST_BIT(hs_syntax_get(predicate_index)->flags, _hs_syntax_node_primitive_bit))
	{
		bool function_exists = false;
		bool script_exists = false;
		ASSERT(expression->type != _hs_special_form);

		hs_parse_call_predicate(expression_index, &function_exists, &script_exists);
		if (expression->constant_type == NONE)
		{
			if (function_exists)
			{
				hs_compile_globals.error_message = "wrong number of arguments for function";
			}
			else if (script_exists)
			{
				hs_compile_globals.error_message = "wrong number of arguments for script";
			}
			else
			{
				hs_compile_globals.error_message = "this is not a valid function or script name.";
			}

			hs_compile_globals.error_offset = predicate->source_offset;
		}
		else if (TEST_BIT(expression->flags, _hs_syntax_node_script_bit))
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->scripts, expression->script_index, hs_script);
			if (script->script_type == _hs_script_static)
			{
				if (expression->type && !hs_can_cast(script->return_type, expression->type))
				{
					csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
						"i expected a %s, but this script returns a %s.",
						hs_type_names[expression->type],
						hs_type_names[script->return_type]);
					hs_compile_globals.error_message = hs_compile_globals.error_buffer;
					hs_compile_globals.error_offset = expression->source_offset;
				}
				else
				{
					if (!expression->type)
					{
						expression->type = script->return_type;
					}

					int16 num_arguments = hs_count_children(expression_index) - 1;
					if (num_arguments == script->parameters.count)
					{
						int16 parameter_index = 0;
						int32 parameter_node_index = hs_syntax_get(predicate_index)->next_node_index;
						success = true;
						while (parameter_node_index != NONE && success)
						{
							ASSERT(parameter_index >= 0 && parameter_index < script->parameters.count);
							hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
							success = hs_parse(parameter_node_index, parameter->type);
							parameter_node_index = hs_syntax_get(parameter_node_index)->next_node_index;
						}

						if (success)
						{
							hs_compile_add_reference(expression->constant_type, _hs_reference_type_script, expression_index);
						}
					}
					else
					{
						hs_compile_globals.error_message = "wrong number of script arguments";
						hs_compile_globals.error_offset = expression->source_offset;
						success = false;
					}
				}
			}
			else
			{
				hs_compile_globals.error_message = "this is not a static script.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
		}
		else
		{
			const hs_function_definition_debug* function = hs_function_get_debug(expression->function_index);
			if (expression->type && !hs_can_cast(function->return_type, expression->type))
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"i expected a %s, but this function returns a %s.",
					hs_type_names[expression->type],
					hs_type_names[function->return_type]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.disallow_blocks
				&& (expression->constant_type == _hs_type_ai || expression->constant_type == _hs_type_ai_command_script))
			{
				hs_compile_globals.error_message = "it is illegal to block in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.disallow_sets
				&& expression->constant_type == _hs_type_void)
			{
				hs_compile_globals.error_message = "it is illegal to set the value of variables in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (!TEST_BIT(function->flags, _hs_function_flag_command_script_atom)
				|| hs_compile_globals.current_script_index == NONE
				|| TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->scripts, hs_compile_globals.current_script_index, hs_script)->script_type == _hs_script_command_script)
			{
				if (!TEST_BIT(function->flags, _hs_function_flag_debug) || hs_compile_globals.current_script_index == NONE)
				{
					if (expression->type == _hs_unparsed && function->return_type != _hs_passthrough)
					{
						expression->type = function->return_type;
					}

					ASSERT(function->parse);
					success = function->parse(expression->function_index, expression_index);
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
		if (expression->type == _hs_special_form)
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"i expected %s, but i got an expression.",
				expression->type == _hs_special_form ? "\"script\" or \"global\"" : "a function name");

			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}

	hs_compile_globals.indent--;

	return success;
}

bool hs_parse(int32 expression_index, int16 expected_type)
{
	ASSERT(!hs_compile_globals.error_message);
	ASSERT(hs_type_valid(expected_type) || expected_type == _hs_special_form || expected_type == _hs_unparsed);

	hs_syntax_node* expression = hs_syntax_get(expression_index);

	bool success = true;
	if (expression->type == _hs_unparsed)
	{
		expression->type = expected_type;
		if (TEST_BIT(hs_syntax_get(expression_index)->flags, _hs_syntax_node_primitive_bit))
		{
			expression->constant_type = expected_type;
			success = hs_parse_primitive(expression_index);
		}
		else
		{
			success = hs_parse_nonprimitive(expression_index);
		}
	}
	return success;
}

bool hs_macro_function_parse(int16 function_index, int32 expression_index)
{
	const hs_function_definition_debug* definition = hs_function_get_debug(function_index);
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(hs_type_valid(definition->return_type));

	bool has_remaining_arguments = true;
	int16 parameter_index;
	for (parameter_index = 0; has_remaining_arguments && parameter_index < definition->formal_parameter_count && next_node_index != NONE; parameter_index++)
	{
		hs_syntax_node* next_expression = hs_syntax_get(next_node_index);
		if (hs_parse(next_node_index, definition->formal_parameters[parameter_index]))
		{
			next_node_index = next_expression->next_node_index;
			continue;
		}

		if (next_expression->type == _hs_type_string_id && TEST_BIT(hs_syntax_get(next_node_index)->flags, _hs_syntax_node_primitive_bit))
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"this is not a valid string for '%s'", definition->name);
			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = next_expression->source_offset;
		}

		has_remaining_arguments = false;
	}

	if (!has_remaining_arguments || (parameter_index == definition->formal_parameter_count && next_node_index == NONE))
	{
		return true;
	}

	csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
		"the \"%s\" call requires exactly %d arguments.", definition->name, definition->formal_parameter_count);
	hs_compile_globals.error_message = hs_compile_globals.error_buffer;
	hs_compile_globals.error_offset = expression->source_offset;

	return false;
}

bool hs_compile_get_tag_by_name(const char* group_name, tag* group_tag_out)
{
	tag group_tag = group_name_to_group_tag(group_name);
	if (group_tag == NONE)
	{
		return false;
	}

	*group_tag_out = group_tag;
	return true;
}

int16 hs_count_children(int32 expression_index)
{
	int16 child_count = 0;

	for (int32 next_node_index = hs_syntax_get(expression_index)->long_value;
		next_node_index != NONE;
		next_node_index = hs_syntax_get(next_node_index)->next_node_index)
	{
		child_count++;
	}

	return child_count;
}

void hs_compile_add_reference(int32 referred_index, e_reference_type reference_type, int32 expression_index)
{
	if (reference_type || (referred_index & 0x8000) == 0)
	{
		ASSERT((hs_compile_globals.current_script_index != NONE) ^ (hs_compile_globals.current_global_index != NONE));

		e_reference_type current_reference_type = _hs_reference_type_global;
		int32 current_index = hs_compile_globals.current_global_index;
		if (hs_compile_globals.current_script_index != NONE)
		{
			current_reference_type = _hs_reference_type_script;
			current_index = hs_compile_globals.current_script_index;
		}

		s_hs_reference* reference = &hs_compile_globals.references[hs_compile_globals.allocated_references++];
		reference->type = current_reference_type;
		reference->index = current_index;
		reference->node_index = expression_index;
		reference->next = NULL;

		switch (reference_type)
		{
		case _hs_reference_type_global:
		{
			ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_globals_per_scenario));

			reference->next = hs_compile_globals.global_references[referred_index];
			hs_compile_globals.global_references[referred_index] = reference;
			reference->strong = true;
		}
		break;
		case _hs_reference_type_script:
		{
			ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_scripts_per_scenario));

			reference->next = hs_compile_globals.script_references[referred_index];
			hs_compile_globals.script_references[referred_index] = reference;

			reference->strong = global_scenario_get()->scripts[referred_index].return_type != _hs_type_void;
		}
		break;
		default:
		{
			UNREACHABLE();
		}
		break;
		}
	}
}

void hs_parse_call_predicate(int32 expression_index, bool* is_function, bool* is_script)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 predicate_index = expression->long_value;
	hs_syntax_node* predicate = hs_syntax_get(predicate_index);
	char* source_offset = &hs_compile_globals.compiled_source[predicate->source_offset];

	if (predicate->type == _hs_function_name)
	{
		ASSERT(predicate->function_index != NONE);
		expression->function_index = predicate->function_index;
	}
	else
	{
		int16 num_arguments = hs_count_children(expression_index) - 1;
		expression->function_index = hs_find_function_by_name(source_offset, num_arguments);
		predicate->type = _hs_function_name;

		if (expression->function_index == NONE)
		{
			expression->script_index = hs_find_script_by_name(source_offset, num_arguments);
			if (expression->script_index == NONE)
			{
				if (is_function)
				{
					*is_function = hs_find_function_by_name(source_offset, num_arguments) != NONE;
				}

				if (is_script)
				{
					*is_script = hs_find_script_by_name(source_offset, num_arguments) != NONE;
				}
			}
			else
			{
				expression->flags |= FLAG(_hs_syntax_node_script_bit);

				if (is_script)
				{
					*is_script = true;
				}
			}
		}
		else if (is_function)
		{
			*is_function = true;
		}

		predicate->constant_type = expression->constant_type;
	}
}

bool hs_parse_tag_block_element_string_id(int32 expression_index, int32 offset, int32 scenario_index, s_tag_block* block, int32 element_size)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT((offset + (int32)sizeof(string_id)) <= element_size);

	bool valid = false;
	for (int32 block_index = 0; block_index < block->count; block_index++)
	{
		byte* block_data = (byte*)tag_block_get_element_with_size(block, block_index, element_size);

		string_id block_element_string_id = *(string_id*)(block_data + offset);
		if (block_element_string_id == string_id_retrieve(source_offset))
		{
			expression->short_value = (int16)block_index;
			valid = true;
			break;
		}
	}

	if (!valid)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (hs_compile_globals.permanent)
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

bool hs_parse_tag_block_element(int32 expression_index, int32 offset, int32 scenario_index, s_tag_block* block, int32 element_size)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(element_size <= SHRT_MAX);
	ASSERT(offset + (k_tag_string_length - 1) < element_size);

	bool valid = false;
	for (int32 block_index = 0; block_index < block->count; block_index++)
	{
		const char* block_element = (const char*)tag_block_get_element_with_size(block, block_index, element_size);
		if (ascii_stricmp(block_element + offset, source_offset) == 0)
		{
			expression->short_value = (int16)block_index;
			valid = true;
			break;
		}
	}

	if (!valid)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;

		return false;
	}

	if (hs_compile_globals.permanent)
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

void hs_compile_initialize(bool permanent)
{
	ASSERT(!hs_compile_globals.initialized);

	hs_compile_globals.initialized = true;
	hs_compile_globals.compiled_source = NULL;
	hs_compile_globals.compiled_source_size = 0;
	hs_compile_globals.permanent = permanent;
	hs_compile_globals.error_message = NULL;
	hs_compile_globals.error_offset = NONE;
	hs_compile_globals.indent = 0;
	hs_compile_globals.current_script_index = NONE;
	hs_compile_globals.current_global_index = NONE;
	hs_compile_globals.references = NULL;
	hs_compile_globals.script_references = NULL;
	hs_compile_globals.global_references = NULL;
	hs_compile_globals.allocated_references = 0;

	if (permanent)
	{
		// not using constant number variables or enum values is bad mkay

		//editor_reset_script_referenced_blocks();
		//resize_scenario_syntax_data(61440);

		hs_compile_globals.references = 
			(s_hs_reference*)system_malloc(sizeof(s_hs_reference*)  * k_maximum_number_of_references);
		hs_compile_globals.script_references =
			(s_hs_reference**)system_malloc(sizeof(s_hs_reference*) * k_maximum_number_of_script_references);
		hs_compile_globals.global_references =
			(s_hs_reference**)system_malloc(sizeof(s_hs_reference*) * k_maximum_number_of_global_references);
	
		for (int32 i = 0; i < k_maximum_number_of_script_references; i++)
		{
			hs_compile_globals.script_references = NULL;
		}

		for (int32 i = 0; i < k_maximum_number_of_global_references; i++)
		{
			hs_compile_globals.global_references = NULL;
		}
	}
}

struct s_hs_compile_state
{
	int32 failed_scripts[32];
	int32 failed_globals[9];
};

void hs_compile_state_initialize(struct scenario* scenario, s_hs_compile_state* state)
{
	csmemset(state->failed_globals, 0, sizeof(state->failed_globals));
	csmemset(state->failed_scripts, 0, sizeof(state->failed_scripts));
}

char* hs_compile_add_source(int32 source_size, const char* source_data)
{
	// $IMPLEMENT

	return NULL;

	//int32 initial_size = source_size;
	//char* result = (char*)system_realloc(hs_compile_globals.compiled_source, source_size + hs_compile_globals.compiled_source_size + 1);
	//if (result)
	//{
	//	char* new_source = &result[hs_compile_globals.compiled_source_size];
	//	hs_compile_globals.compiled_source = result;
	//	csmemcpy(new_source, source_data, initial_size);
	//	hs_compile_globals.compiled_source_size += initial_size;
	//	hs_compile_globals.compiled_source[hs_compile_globals.compiled_source_size] = 0;
	//	return new_source;
	//}
	//return result;
}

int32 hs_source_pointer_get_line_number(const char* source_pointer, const char* source)
{
	int32 line_number = 1;

	ASSERT(source_pointer);

	for (; source < source_pointer; source++)
	{
		if (*source == '\n')
		{
			line_number++;
		}
	}

	return line_number;
}

struct hs_tokenizer
{
	char* cursor;
	const char* source_file_data;
	int32 source_file_size;
};

int32 hs_tokenize(hs_tokenizer* state)
{
	ASSERT(!hs_compile_globals.error_message);
	ASSERT(g_hs_syntax_data);
	
	int32 syntax_index = datum_new(g_hs_syntax_data);
	if (syntax_index != NONE)
	{
		hs_syntax_node* node = hs_syntax_get(syntax_index);
		node->type = 0;
		node->flags = 0;
		node->script_index = NONE;
		node->source_offset = NONE;
		node->line_number = NONE;
		node->next_node_index = NONE;

		SET_BIT(node->flags, _hs_syntax_node_primitive_bit, *state->cursor != '(');

		if (TEST_BIT(node->flags, _hs_syntax_node_primitive_bit))
		{
			hs_tokenize_primitive(state, syntax_index);
		}
		else
		{
			hs_tokenize_nonprimitive(state, syntax_index);
		}

		if (node->source_offset != NONE && state->source_file_data)
		{
			int32 offset = state->source_file_size + node->source_offset - hs_compile_globals.compiled_source_size;
			ASSERT(offset >= 0 && offset < state->source_file_size);

			char const* source_pointer = &state->source_file_data[offset];
			node->line_number = (int16)hs_source_pointer_get_line_number(source_pointer, state->source_file_data);
		}
	}
	else
	{
		hs_compile_globals.error_message = "i couldn't allocate a syntax node.";
		hs_compile_globals.error_offset = state->cursor - hs_compile_globals.compiled_source;
		return NONE;
	}
	
	return syntax_index;
}

void hs_tokenize_nonprimitive(hs_tokenizer* state, int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32* tail = &expression->long_value;

	expression->source_offset = state->cursor - hs_compile_globals.compiled_source;
	state->cursor++;

	while (!hs_compile_globals.error_message)
	{
		char* last_expression_end = state->cursor;

		skip_whitespace(&state->cursor);
		if (state->cursor != last_expression_end)
		{
			*last_expression_end = 0;
		}

		if (*state->cursor)
		{
			if (*state->cursor == ')')
			{
				*state->cursor++ = 0;
				break;
			}

			*tail = hs_tokenize(state);
			if (*tail != NONE)
			{
				tail = &hs_syntax_get(*tail)->next_node_index;
			}
		}
		else
		{
			hs_compile_globals.error_message = "this left parenthesis is unmatched.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}

	if (tail == (int32*)&expression->long_value && !hs_compile_globals.error_message)
	{
		hs_compile_globals.error_message = "this expression is empty.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
}

void hs_tokenize_primitive(hs_tokenizer* state, int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (*state->cursor == '"')
	{
		hs_token_primitive_until_delimiter(state, expression_index, '"',
			"this quoted constant is unterminated.");
	}
	else if (*state->cursor == '{')
	{
		hs_token_primitive_until_delimiter(state, expression_index, '}',
			"this superstring constant is unterminated. (did you forget a '}' ?)");
	}
	else
	{
		expression->source_offset = state->cursor - hs_compile_globals.compiled_source;
		while (*state->cursor
			&& *state->cursor != ')'
			&& *state->cursor != ';'
			&& !character_in_list(*state->cursor, NUMBEROF(whitespace_characters), whitespace_characters)
			&& !character_in_list(*state->cursor, NUMBEROF(eol_characters), eol_characters))
		{
			state->cursor++;
		}
	}
}

void hs_token_primitive_until_delimiter(hs_tokenizer* state, int32 expression_index, int end_delimiter, const char* error_message)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	expression->source_offset = ++state->cursor - hs_compile_globals.compiled_source;
	while (*state->cursor && *state->cursor != end_delimiter)
	{
		state->cursor++;
	}

	if (*state->cursor)
	{
		*state->cursor++ = 0;
	}
	else
	{
		hs_compile_globals.error_message = error_message;
		hs_compile_globals.error_offset = expression->source_offset - 1;
	}
}

void hs_compile_first_pass(s_hs_compile_state* compile_state, int32 source_file_size, const char* source_file_data, const char** error_message_pointer, int32* error_offset)
{
	// $IMPLEMENT

	//hs_tokenizer tokenizer{};
	//tokenizer.source_file_data = source_file_data;
	//tokenizer.source_file_size = source_file_size;
	//if (tokenizer.cursor = hs_compile_add_source(source_file_size, source_file_data))
	//{
	//	hs_compile_globals.error_message = NULL;
	//	*error_message_pointer = NULL;
	//	hs_compile_globals.error_offset = NONE;
	//
	//	skip_whitespace(&tokenizer.cursor);
	//	while (*tokenizer.cursor)
	//	{
	//		int32 expression_index = hs_tokenize(&tokenizer);
	//		skip_whitespace(&tokenizer.cursor);
	//		if (hs_compile_globals.error_message || !hs_parse_special_form(expression_index))
	//		{
	//			VASSERT(!hs_compile_globals.error_message, "tell DAMIAN (or whomever owns HS) that somebody failed to correctly report a parsing error.");
	//			*error_message_pointer = hs_compile_globals.error_message;
	//			*error_offset = hs_compile_globals.error_offset;
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	*error_message_pointer = "couldn't allocate memory for compiled source.";
	//}
}

bool hs_compile_second_pass(s_hs_compile_state* compile_state, bool verbose)
{
	// $IMPLEMENT

	return false;
}

char* g_error_output_buffer = NULL;
int32 g_error_buffer_length = 0;

bool hs_compile_source(bool fail_on_error, bool verbose)
{
	return false;

	//int32 total_source_size = 0;
	//bool success = true;
	//s_hs_compile_state state;
	//
	//progress_new("compiling scripts");
	//hs_compile_initialize(true);
	//hs_compile_state_initialize(global_scenario_get(), &state);
	//
	//if (g_error_output_buffer && verbose)
	//{
	//	csstrnzcpy(g_error_output_buffer, "", g_error_buffer_length);
	//}
	//
	//for (hs_source_file& source_file : global_scenario_get()->source_files)
	//{
	//
	//}
	//
	//if (success || !fail_on_error)
	//{
	//	success = hs_compile_second_pass(&state, verbose);
	//	if (!success)
	//	{
	//		hs_compile_strip_failed_special_forms(&state);
	//	}
	//}
	//
	//hs_runtime_require_gc();
	//
	//if (verbose)
	//{
	//	if (success)
	//	{
	//		console_printf("scripts successfully compiled.");
	//	}
	//	else
	//	{
	//		console_printf("script compile errors");
	//	}
	//}
	//
	//if (!success)
	//{
	//	success = !fail_on_error;
	//}
	//
	//hs_compile_dispose();
	//progress_done();
	//
	//return success;
}

void hs_compile_dispose()
{
	ASSERT(hs_compile_globals.initialized);

	char* compiled_source = hs_compile_globals.compiled_source;
	if (hs_compile_globals.permanent && hs_compile_globals.compiled_source)
	{
		system_free(hs_compile_globals.compiled_source);
		compiled_source = hs_compile_globals.compiled_source;
	}

	if (hs_compile_globals.malloced)
	{
		system_free(compiled_source);
		hs_compile_globals.compiled_source = NULL;
		hs_compile_globals.malloced = false;
	}

	if (hs_compile_globals.references)
	{
		system_free(hs_compile_globals.references);
		hs_compile_globals.references = NULL;
	}

	if (hs_compile_globals.script_references)
	{
		system_free(hs_compile_globals.script_references);
		hs_compile_globals.script_references = NULL;
	}

	if (hs_compile_globals.global_references)
	{
		system_free(hs_compile_globals.global_references);
		hs_compile_globals.global_references = NULL;
	}

	hs_compile_globals.initialized = false;

	//int32 count = 61440;
	//if (g_hs_syntax_data->maximum_count + 512 < 61440)
	//{
	//	count = g_hs_syntax_data->maximum_count + 512;
	//}
	//resize_scenario_syntax_data(count);
}

int32 hs_compile_expression(int32 source_size, const char* source_data, const char** error_message_pointer, const char** error_source_pointer)
{
	int32 result_expression_index = NONE;
	hs_tokenizer tokenizer{};

	if (source_size < 4096)
	{
		int32 old_size = 0;

		bool valid_buffer = true;
		if (global_scenario_index_get() == NONE)
		{
			old_size = 0;
			hs_compile_globals.compiled_source = (char*)system_malloc(source_size + 1);
			hs_compile_globals.malloced = true;
			ASSERT(hs_compile_globals.compiled_source);
		}
		else if (global_scenario_get()->script_string_data.size < 4096)
		{
			valid_buffer = false;
		}
		else
		{
			old_size = global_scenario_get()->script_string_data.size - 4096;
			hs_compile_globals.compiled_source = (char*)global_scenario_get()->script_string_data.address;
		}

		if (valid_buffer)
		{
			ASSERT(g_hs_syntax_data);
			csmemcpy(&hs_compile_globals.compiled_source[old_size], source_data, source_size);
			hs_compile_globals.compiled_source_size = old_size + source_size;
			hs_compile_globals.compiled_source[old_size + source_size] = 0;

			*error_message_pointer = NULL;
			*error_source_pointer = NULL;

			hs_compile_globals.error_message = 0;
			hs_compile_globals.error_offset = -1;

			tokenizer.cursor = &hs_compile_globals.compiled_source[old_size];
			tokenizer.source_file_data = 0;
			tokenizer.source_file_size = 0;

			skip_whitespace(&tokenizer.cursor);
			if (*tokenizer.cursor)
			{
				int32 root_expression_index = hs_tokenize(&tokenizer);
				if (!hs_compile_globals.error_message)
				{
					int32 implicit_inspect_index = datum_new(g_hs_syntax_data);
					int32 implicit_inspect_name_index = datum_new(g_hs_syntax_data);
					if (implicit_inspect_index != NONE && implicit_inspect_name_index != NONE)
					{
						hs_syntax_node* implicit_inspect = hs_syntax_get(implicit_inspect_index);
						hs_syntax_node* implicit_inspect_name = hs_syntax_get(implicit_inspect_name_index);
						implicit_inspect->long_value = implicit_inspect_name_index;
						implicit_inspect->next_node_index = NONE;
						implicit_inspect->source_offset = hs_syntax_get(root_expression_index)->source_offset;
						implicit_inspect->flags = 0;
						implicit_inspect_name->next_node_index = root_expression_index;
						implicit_inspect_name->source_offset = -1;
						implicit_inspect_name->function_index = _hs_function_inspect;
						implicit_inspect_name->type = _hs_function_name;
						implicit_inspect_name->flags = FLAG(_hs_syntax_node_primitive_bit);
						if (hs_parse(implicit_inspect_index, _hs_type_void))
						{
							result_expression_index = implicit_inspect_index;
						}
					}
				}

				if (result_expression_index == NONE)
				{
					*error_message_pointer = hs_compile_globals.error_message;
					if (hs_compile_globals.error_offset != -1)
					{
						hs_compile_globals.error_offset -= old_size;
						ASSERT(hs_compile_globals.error_offset >= 0 && hs_compile_globals.error_offset < source_size);
						*error_source_pointer = &source_data[hs_compile_globals.error_offset];
					}
				}
			}
			else
			{
				//hs_runtime_require_gc();
			}
		}
		else
		{
			event(_event_error, "hs: not enough space to allocate a temporary hs compiled-source buffer! You got yourself into a REALLY weird state!(show Damian)");
		}
	}

	return result_expression_index;
}

void string_copy_bounded(c_wrapped_array<char> out_dest_string, c_wrapped_array<char const> const in_source_string)
{
	// $IMPLEMENT `hs_validify_expression` properly

	ASSERT(out_dest_string.count() > 0);
	
	int32 copy_length = out_dest_string.m_count - 1;
	if (copy_length > in_source_string.m_count)
	{
		copy_length = in_source_string.m_count;
	}
	
	_memccpy(out_dest_string.m_elements, in_source_string.m_elements, 0, copy_length);
	
	out_dest_string.m_elements[copy_length] = 0;
}

enum e_hs_fakery
{
	_hs_fakery_none = 0,
	_hs_fakery_parenthesize,
	_hs_fakery_set,
	_hs_fakery_begin,

	NUMBER_OF_HS_FAKERY_TYPES,
};

bool char_is_delimiter(char character, const char* delimiter_string)
{
	bool found;
	for (found = false; !found && *delimiter_string; found = character == *delimiter_string++);
	return found;
}

void hs_validify_expression(const char* expression, char* out_valid_expression_buffer, int32 out_expression_length)
{
	c_wrapped_array<char> out_valid_expression(out_valid_expression_buffer, out_expression_length);

	const char* expression_end = find_string_end(expression, ";");
	const char* expression_begin = find_string_end_not_in_delimiter(expression, " \t\r\n");
	if (expression_begin > expression_end)
	{
		expression_begin = expression;
	}

	while (char_is_delimiter(*(expression_end - 1), " \t\r\n"))
	{
		expression_end--;
	}

	if (expression_begin >= expression_end)
	{
		csstrnzcpy(out_valid_expression.begin(), "", out_valid_expression.count());
	}
	else
	{
		e_hs_fakery fakery = _hs_fakery_none;
		char fakery_buffer[4096]{};
		string_copy_bounded(fakery_buffer, make_wrapped_array(expression_begin, expression_end));

		if (fakery_buffer[0] != '(')
		{
			char* space = strchr(fakery_buffer, ' ');
			if (space)
			{
				*space = 0;
			}

			if (hs_find_global_by_name(fakery_buffer) == NONE)
			{
				fakery = _hs_fakery_parenthesize;
			}
			else if (space)
			{
				fakery = _hs_fakery_set;
			}

			if (space)
			{
				*space = ' ';
			}
		}
		else
		{
			int32 expression_depth = 0;
			int32 expression_count = 0;
			for (char const* expression_character = expression_begin; expression_character < expression_end; ++expression_character)
			{
				if (*expression_character == '(')
				{
					++expression_count;
				}
				else if (*expression_character == ')')
				{
					if (!expression_count)
					{
						break;
					}

					if (!--expression_count)
					{
						++expression_depth;
					}
				}
			}

			if (expression_depth > 1)
			{
				fakery = _hs_fakery_begin;
			}
		}

		switch (fakery)
		{
		case _hs_fakery_none:
		{
			csstrnzcpy(out_valid_expression.begin(), fakery_buffer, out_valid_expression.count());
		}
		break;
		case _hs_fakery_parenthesize:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(%s)", fakery_buffer);
		}
		break;
		case _hs_fakery_set:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(set %s)", fakery_buffer);
		}
		break;
		case _hs_fakery_begin:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(begin %s)", fakery_buffer);
		}
		break;
		default:
		{
			HALT();
		}
		break;
		}

		ascii_strnlwr(out_valid_expression.begin(), out_valid_expression.count());
	}
}

bool hs_runtime_safe_to_gc()
{
	// $IMPLEMENT

	return false;
}

bool hs_compile_and_evaluate(e_event_level event_level, const char* source, const char* expression, bool interactive)
{
	bool result = false;

	event(event_level, "hs:evaluate: %s: %s", source, expression);
	
	//random_seed_allow_use();
	
	char expression_buffer[4096]{};
	hs_validify_expression(expression, expression_buffer, sizeof(expression_buffer));
	if (string_is_not_empty(expression_buffer))
	{
		const char* error_message = NULL;
		const char* error_source = NULL;
	
		if (g_hs_syntax_data && g_hs_syntax_data->valid && hs_runtime_safe_to_gc())
		{
			hs_node_gc();
		}
	
		hs_compile_initialize(false);
	
		hs_syntax_node temporary_syntax_data[128]{};
		if (TEST_BIT(g_hs_syntax_data->flags, _hs_syntax_node_script_bit))
		{
			csmemset(temporary_syntax_data, 0, sizeof(temporary_syntax_data));
			data_connect(g_hs_syntax_data, NUMBEROF(temporary_syntax_data), temporary_syntax_data);
		}
	
		int32 source_size = csstrnlen(expression_buffer, sizeof(expression_buffer));
		int32 expression_index = hs_compile_expression(source_size, expression_buffer, &error_message, &error_source);
		if (expression_index == NONE)
		{
			if (error_message)
			{
				hs_compile_source_error(NULL, error_message, error_source, expression_buffer);
			}
		}
		else
		{
			result = true;
			hs_runtime_evaluate(expression_index, interactive, false);
		}
	
		if (g_hs_syntax_data->data == temporary_syntax_data)
		{
			data_disconnect(g_hs_syntax_data);
		}
	
		hs_compile_dispose();
	}
	
	//if (g_recompile_scripts)
	//{
	//	hs_rebuild_and_compile(NULL, false, true);
	//	g_recompile_scripts = false;
	//}
	
	//random_seed_disallow_use();

	return result;
}

void hs_compile_source_error(const char* file_name, const char* error_message, const char* error_source, const char* source)
{
	char const* newline = NULL;
	if (error_source)
	{
		newline = strchr(error_source, '\n');
		if (!newline)
		{
			newline = &error_source[strlen_debug(error_source)];
		}
	}

	c_wrapped_array<char const> bounded_expression(error_source, newline);
	if (file_name && newline)
	{
		int16 line_number = 1;
		while (newline > source)
		{
			if (*newline == '\n')
			{
				line_number++;
			}
			newline--;
		}

		event(_event_critical, "design: [%s line %d] %s: %.*s",
			file_name, line_number,
			error_message,
			bounded_expression.count(), bounded_expression.begin());

		if (g_error_output_buffer)
		{
			csnzappendf(g_error_output_buffer, g_error_buffer_length, "design: [%s line %d] %s: %.*s",
				file_name, line_number,
				error_message,
				bounded_expression.count(), bounded_expression.begin());
		}

		for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
		{
			if (hs_static_globals.compile_error_listeners[compile_error_listener_index])
			{
				hs_static_globals.compile_error_listeners[compile_error_listener_index]->handle_error(
					file_name, line_number, error_message, error_source, &bounded_expression, source);
			}
		}
	}
	else
	{
		event(_event_critical, "design: %s: %.*s",
			error_message,
			bounded_expression.count(), bounded_expression.begin());

		if (g_error_output_buffer)
		{
			csnzappendf(g_error_output_buffer, g_error_buffer_length, "%s: %.*s\n",
				error_message,
				bounded_expression.count(), bounded_expression.begin());
		}
	}
}

