#include "hs/hs_library_internal_compile.hpp"

#include "cache/cache_files.hpp"
#include "hs/hs_scenario_definitions.hpp"

bool hs_get_parameter_indices(const char* function_name, int16 count, int32* result_indices, int32 expression_index)
{
	bool success = true;

	int32 parameters_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	int16 parameter_index;
	for (parameter_index = 0; parameters_index != NONE && parameter_index < count; parameter_index++)
	{
		result_indices[parameter_index] = parameters_index;
		parameters_index = hs_syntax_get(parameters_index)->next_node_index;
	}

	if (parameter_index != count || parameters_index != NONE)
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"the %s call requires %d arguments.",
			function_name,
			count);
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		success = false;
	}

	return success;
}

bool hs_parse_begin(int16 function_index, int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(function_index == _hs_function_begin || function_index == _hs_function_begin_random);

	int16 argument_count = 0;
	while (success && next_node_index != NONE)
	{
		int32 next_next_node_index = hs_syntax_get(next_node_index)->next_node_index;
		if (function_index)
		{
			success = hs_parse(next_node_index, expression->type);
			if (!expression->type && success)
				expression->type = hs_syntax_get(next_node_index)->type;
		}
		else
		{
			int16 type = next_next_node_index == NONE ? expression->type : _hs_type_void;
			success = hs_parse(next_node_index, type);
			if (next_next_node_index == NONE && !expression->type && success)
				expression->type = hs_syntax_get(next_node_index)->type;
		}
		next_node_index = next_next_node_index;

		argument_count++;
	}

	if (success)
	{
		if (argument_count < 1)
		{
			hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"a statement block must contain at least one argument.",
				hs_function_get(function_index)->name);
			hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
			return false;
		}

		if (argument_count > 32 && function_index == _hs_function_begin_random)
		{
			hs_compile_globals.error_message = "begin_random can take a maximum of 32 arguments (matt can increase this.)";
			hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
			return false;
		}
	}

	return success;
}

bool hs_parse_debug_string(int16 function_index, int32 expression_index)
{
	bool success = true;

	int32 parameters_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	ASSERT(IN_RANGE_INCLUSIVE(function_index, _hs_function_debug_string__first, _hs_function_debug_string__last));

	for (int16 actual_parameter_count = 0; success; actual_parameter_count++)
	{
		if (parameters_index != NONE)
		{
			success = hs_parse(parameters_index, _hs_type_string);
			parameters_index = hs_syntax_get(parameters_index)->next_node_index;
		}
	}

	return success;
}

bool hs_parse_if(int16 function_index, int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(function_index == _hs_function_if);

	int32 next_next_expression = NONE;
	int32 next_next_next_node_index = NONE;
	if (next_node_index == NONE
		|| (next_next_expression = hs_syntax_get(next_node_index)->next_node_index, next_next_expression == NONE)
		|| (next_next_next_node_index = hs_syntax_get(next_next_expression)->next_node_index, next_next_next_node_index != NONE)
		&& hs_syntax_get(next_next_next_node_index)->next_node_index != NONE)
	{
		hs_compile_globals.error_message = "i expected (if <condition> <then> [<else>]).";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}
	else if (hs_parse(next_node_index, _hs_type_boolean))
	{
		if (hs_parse(next_next_expression, expression->type))
		{
			if (!expression->type)
				expression->type = hs_syntax_get(next_next_expression)->type;

			success = next_next_next_node_index == -1 || hs_parse(next_next_next_node_index, expression->type);
		}
		else if (!hs_compile_globals.error_message
			&& !expression->type
			&& next_next_next_node_index != NONE
			&& hs_parse(next_next_next_node_index, expression->type))
		{
			expression->type = hs_syntax_get(next_next_next_node_index)->type;
			success = hs_parse(next_next_expression, expression->type);
		}
	}

	return success;
}

int32 hs_parse_cond_recursive(int32 root_expression_index, int32 expression_index)
{
	int32 result_index = datum_new(g_hs_syntax_data);
	if (result_index == NONE)
	{
		hs_compile_globals.error_message = "i couldn't allocate a syntax node.";
		hs_compile_globals.error_offset = hs_syntax_get(root_expression_index)->source_offset;
	}
	else
	{
		hs_syntax_node* result_node = hs_syntax_get(result_index);
		result_node->flags = 0;
		result_node->next_node_index = NONE;
		result_node->source_offset = hs_syntax_get(root_expression_index)->source_offset;

		if (expression_index == NONE)
		{
			result_node->constant_type = hs_syntax_get(root_expression_index)->type;
			result_node->type = result_node->constant_type;
			result_node->flags |= FLAG(_hs_syntax_node_primitive_bit);
			result_node->long_value = 0;
		}
		else
		{
			int32 condition_result_pair_index = expression_index;

			if (TEST_BIT(hs_syntax_get(condition_result_pair_index)->flags, _hs_syntax_node_primitive_bit))
			{
				hs_compile_globals.error_message = "this argument to cond should be a condition/result pair";
				hs_compile_globals.error_offset = hs_syntax_get(condition_result_pair_index)->source_offset;
				result_index = NONE;
			}
			else
			{
				int32 link_index = hs_syntax_get(condition_result_pair_index)->long_value;
				hs_syntax_node* link_node = hs_syntax_get(link_index);

				if (link_node->next_node_index == NONE)
				{
					hs_compile_globals.error_message = "this argument to cond needs a result.";
					hs_compile_globals.error_offset = hs_syntax_get(link_index)->source_offset;
					result_index = NONE;
				}
				else
				{
					int32 implicit_begin_index = datum_new(g_hs_syntax_data);
					int32 implicit_begin_name_index = datum_new(g_hs_syntax_data);

					if (implicit_begin_index == NONE || implicit_begin_name_index == NONE)
					{
						hs_compile_globals.error_message = "i couldn't allocate a syntax node.";
						hs_compile_globals.error_offset = hs_syntax_get(root_expression_index)->source_offset;
						result_index = NONE;
					}
					else
					{
						hs_syntax_node* implicit_begin_node = hs_syntax_get(implicit_begin_index);
						hs_syntax_node* implicit_begin_name_node = hs_syntax_get(implicit_begin_name_index);
						hs_syntax_node* condition_result_pair_node = hs_syntax_get(condition_result_pair_index);

						implicit_begin_node->next_node_index = hs_parse_cond_recursive(root_expression_index, hs_syntax_get(condition_result_pair_index)->next_node_index);

						if (implicit_begin_node->next_node_index == NONE)
						{
							result_index = NONE;
						}
						else
						{
							result_node->constant_type = _hs_function_name;
							result_node->long_value = condition_result_pair_index;
							
							condition_result_pair_node->constant_type = _hs_function_name;
							condition_result_pair_node->type = _hs_function_name;
							condition_result_pair_node->flags = FLAG(_hs_syntax_node_primitive_bit);
							condition_result_pair_node->next_node_index = link_index;
							condition_result_pair_node->source_offset = NONE;
							condition_result_pair_node->long_value = 0;
							
							implicit_begin_node->flags = 0;
							implicit_begin_node->source_offset = result_node->source_offset;
							implicit_begin_node->long_value = implicit_begin_name_index;
							
							implicit_begin_name_node->constant_type = _hs_unparsed;
							implicit_begin_name_node->type = _hs_function_name;
							implicit_begin_name_node->flags = FLAG(_hs_syntax_node_primitive_bit);
							implicit_begin_name_node->next_node_index = hs_syntax_get(link_index)->next_node_index;
							implicit_begin_name_node->source_offset = NONE;
							implicit_begin_name_node->long_value = 0;
							
							link_node->next_node_index = implicit_begin_index;
						}
					}
				}
			}
		}
	}
	return result_index;
}

bool hs_parse_cond(int16 function_index, int32 expression_index)
{
	bool success = false;
	int32 reformatted_expression_index = hs_parse_cond_recursive(expression_index, hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index);
	if (reformatted_expression_index != NONE)
	{
		hs_syntax_node* cond_node = hs_syntax_get(expression_index);
		hs_syntax_node* reformatted_node = hs_syntax_get(reformatted_expression_index);
		int16 identifier = cond_node->identifier;
		int16 type = cond_node->type;
		reformatted_node->next_node_index = cond_node->next_node_index;
		*cond_node = *reformatted_node;
		cond_node->identifier = identifier;
		success = hs_parse(expression_index, type);
	}
	return success;
}

bool hs_parse_set(int16 function_index, int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 variable_reference_index = hs_syntax_get(expression->long_value)->next_node_index;

	if (variable_reference_index == NONE)
	{
		hs_compile_globals.error_message = "i expected a variable to set and a value.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		hs_syntax_node* variable_reference = hs_syntax_get(variable_reference_index);

		if (variable_reference->next_node_index == NONE)
		{
			hs_compile_globals.error_message = "i expected an assignment value.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else if (hs_syntax_get(variable_reference->next_node_index)->next_node_index != NONE)
		{
			hs_compile_globals.error_message = "i didn't expect this argument.";
			hs_compile_globals.error_offset = hs_syntax_get(hs_syntax_get(variable_reference->next_node_index)->next_node_index)->source_offset;
		}
		else
		{
			char* source_offset = &hs_compile_globals.compiled_source[variable_reference->source_offset];
			int16 global_index = hs_find_global_by_name(source_offset);
			if (global_index == NONE)
			{
				hs_compile_globals.error_message = "this is not a valid global variable.";
				hs_compile_globals.error_offset = variable_reference->source_offset;
			}
			else
			{
				variable_reference->type = hs_global_get_type(global_index);
				if (expression->type && !hs_can_cast(variable_reference->type, expression->type))
				{
					hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
						"you cannot pass the result of this set (type %s) to a function that expects type %s.",
						hs_type_names[variable_reference->type],
						hs_type_names[expression->type]);
					hs_compile_globals.error_offset = expression->source_offset;
				}
				else
				{
					bool asserted = hs_parse_variable(variable_reference_index);
					ASSERT(asserted);

					if (expression->type == _hs_unparsed)
					{
						expression->type = variable_reference->type;
					}

					if (hs_parse(variable_reference->next_node_index, variable_reference->type))
					{
						success = true;
					}
				}
			}
		}
	}

	return success;
}

bool hs_parse_logical(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_and || function_index == _hs_function_or);

	bool success = true;

	int32 parameters_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	int32 actual_parameter_count = 0;
	for (; success && parameters_index != NONE; actual_parameter_count++)
	{
		success = hs_parse(parameters_index, _hs_type_boolean);
		parameters_index = hs_syntax_get(parameters_index)->next_node_index;
	}

	if (success && actual_parameter_count < 2)
	{
		hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, NUMBEROF(hs_compile_globals.error_buffer),
			"the %s call requires at least 2 arguments.",
			hs_function_get(function_index)->name);

		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		success = false;
	}

	return success;
};

bool hs_parse_arithmetic(int16 function_index, int32 expression_index)
{
	ASSERT(function_index >= _hs_function_plus && function_index <= _hs_function_max);

	bool success = true;

	int32 parameters_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	int32 actual_parameter_count = 0;
	for (; success && parameters_index != NONE; actual_parameter_count++)
	{
		success = hs_parse(parameters_index, _hs_type_real);
		parameters_index = hs_syntax_get(parameters_index)->next_node_index;
	}

	if (success && actual_parameter_count < 2 || function_index == _hs_function_divide && actual_parameter_count > 2)
	{
		hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, NUMBEROF(hs_compile_globals.error_buffer),
			"the %s call requires %s2 arguments.",
			hs_function_get(function_index)->name,
			(function_index == _hs_function_divide) ? "" : "at least ");

		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		success = false;
	}

	return success;
};

bool hs_parse_equality(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_equal || function_index == _hs_function_not_equal);

	bool success = false;
	int32 parameter_indices[2];
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, NUMBEROF(parameter_indices), parameter_indices, expression_index))
	{
		if (hs_parse(parameter_indices[0], _hs_unparsed))
		{
			if (hs_parse(parameter_indices[1], hs_syntax_get(parameter_indices[0])->type))
			{
				success = true;
			}
		}
		else if (!hs_compile_globals.error_message && hs_parse(parameter_indices[1], _hs_unparsed))
		{
			if (hs_parse(parameter_indices[0], hs_syntax_get(parameter_indices[1])->type))
			{
				success = true;
			}
		}
		else if (!hs_compile_globals.error_message && hs_parse(parameter_indices[0], _hs_type_real) && hs_parse(parameter_indices[1], _hs_type_real))
		{
			success = true;
		} 
	}
	return success;
};

bool hs_parse_inequality(int16 function_index, int32 expression_index)
{
	ASSERT(function_index >= _hs_function_gt && function_index <= _hs_function_lte);

	bool success = false;
	long parameter_indices[2];
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, NUMBEROF(parameter_indices), parameter_indices, expression_index))
	{
		if (hs_parse(parameter_indices[0], _hs_unparsed)
			&& (hs_syntax_get(parameter_indices[0])->type >= _hs_type_enum_game_difficulty
				&& hs_syntax_get(parameter_indices[0])->type <= _hs_type_enum_character_physics_override
				|| hs_syntax_get(parameter_indices[0])->type >= _hs_type_real
				&& hs_syntax_get(parameter_indices[0])->type <= _hs_type_long_integer))
		{
			if (hs_parse(parameter_indices[1], hs_syntax_get(parameter_indices[0])->type))
			{
				success = true;
			}
		}
		else if (hs_compile_globals.error_message
			|| !hs_parse(parameter_indices[1], 0)
			|| (hs_syntax_get(parameter_indices[1])->type < _hs_type_enum_game_difficulty
				|| hs_syntax_get(parameter_indices[1])->type > _hs_type_enum_character_physics_override)
			&& (hs_syntax_get(parameter_indices[1])->type < _hs_type_real
				|| hs_syntax_get(parameter_indices[1])->type > _hs_type_long_integer))
		{
			int16 type0 = hs_syntax_get(parameter_indices[0])->type;
			int16 type1 = hs_syntax_get(parameter_indices[1])->type;

			if (!hs_compile_globals.error_message
				&& (!type0 || hs_can_cast(type0, _hs_type_real))
				&& (!type1 || hs_can_cast(type1, _hs_type_real))
				&& hs_parse(parameter_indices[0], _hs_type_real)
				&& hs_parse(parameter_indices[1], _hs_type_real))
			{
				success = true;
			}
		}
		else
		{
			if (hs_parse(parameter_indices[0], hs_syntax_get(parameter_indices[1])->type))
			{
				success = true;
			}
		}
	}

	if (!success)
	{
		hs_compile_globals.error_message = "invalid arguments to inequality operator.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}

	return success;
};

bool hs_parse_sleep(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_sleep);

	bool success = false;
	int32 time_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	if (time_index == NONE)
	{
		hs_compile_globals.error_message = "the sleep call requires a time and, optionally, a script name.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}
	else if (hs_parse(time_index, _hs_type_short_integer))
	{
		int32 script_name_index = hs_syntax_get(time_index)->next_node_index;
		if (script_name_index == NONE || hs_parse(script_name_index, _hs_type_script))
		{
			success = true;
		}
	}
	return success;
};

bool hs_parse_sleep_forever(int16 function_index, int32 expression_index)
{
	bool success = false;
	int32 script_name_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	if (script_name_index == NONE || hs_parse(script_name_index, _hs_type_script))
	{
		success = true;
	}
	return success;
};

bool hs_parse_sleep_until(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_sleep_until);

	bool success = false;
	int32 condition_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	if (condition_index == NONE)
	{
		hs_compile_globals.error_message = "the sleep_until call requires a condition and, optionally, a period.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}
	else
	{
		int32 period_index = hs_syntax_get(condition_index)->next_node_index;
		success = hs_parse(condition_index, _hs_type_boolean);
		if (success && period_index != NONE)
		{
			int32 expiration_index = hs_syntax_get(period_index)->next_node_index;
			success = hs_parse(period_index, _hs_type_short_integer);
			if (success && expiration_index != NONE)
			{
				success = hs_parse(expiration_index, _hs_type_long_integer);
			}
		}
	}
	return success;
};

bool hs_parse_wake(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_wake);

	bool success = false;
	int32 script_name_index;
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, 1, &script_name_index, expression_index))
	{
		hs_syntax_node* script_name_node = hs_syntax_get(script_name_index);
		if (hs_parse(script_name_index, _hs_type_script))
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_name_node->short_value, hs_script);
			if (script->script_type == _hs_script_static || script->script_type == _hs_script_command_script)
			{
				hs_compile_globals.error_message = "static or command-script scripts cannot be awakened.";
				hs_compile_globals.error_offset = script_name_node->source_offset;
			}
			else
			{
				success = true;
			}
		}
	}
	return success;
};

bool hs_parse_inspect(int16 function_index, int32 expression_index)
{
	ASSERT(function_index == _hs_function_inspect);

	bool success = false;
	int32 argument_index;
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, 1, &argument_index, expression_index))
	{
		if (hs_parse(argument_index, _hs_unparsed))
		{
			success = true;
		}
		else if (!hs_compile_globals.error_message)
		{
			hs_compile_globals.error_message = "this is not a global variable reference, function call, or script call.";
			hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		}
	}
	return success;
};

bool hs_parse_object_cast_up(int16 function_index, int32 expression_index)
{
	bool success = false;
	int32 object_reference_index;
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, 1, &object_reference_index, expression_index))
	{
		success = hs_parse(object_reference_index, _hs_type_object);
	}

	return success;
};

