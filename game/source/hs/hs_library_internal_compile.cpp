#include "hs/hs_library_internal_compile.hpp"

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
	bool parse_success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(function_index == _hs_function_begin || function_index == _hs_function_begin_random);

	int16 argument_count = 0;
	while (parse_success && next_node_index != NONE)
	{
		int32 next_next_node_index = hs_syntax_get(next_node_index)->next_node_index;
		if (function_index)
		{
			parse_success = hs_parse(next_node_index, expression->type);
			if (!expression->type && parse_success)
				expression->type = hs_syntax_get(next_node_index)->type;
		}
		else
		{
			int16 type = next_next_node_index == NONE ? expression->type : _hs_type_void;
			parse_success = hs_parse(next_node_index, type);
			if (next_next_node_index == NONE && !expression->type && parse_success)
				expression->type = hs_syntax_get(next_node_index)->type;
		}
		next_node_index = next_next_node_index;

		argument_count++;
	}

	if (parse_success)
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

	return parse_success;
}

bool hs_parse_if(int16 function_index, int32 expression_index)
{
	bool parse_success = false;
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

			parse_success = next_next_next_node_index == -1 || hs_parse(next_next_next_node_index, expression->type);
		}
		else if (!hs_compile_globals.error_message
			&& !expression->type
			&& next_next_next_node_index != NONE
			&& hs_parse(next_next_next_node_index, expression->type))
		{
			expression->type = hs_syntax_get(next_next_next_node_index)->type;
			parse_success = hs_parse(next_next_expression, expression->type);
		}
	}

	return parse_success;
}

bool hs_parse_cond(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
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
	// $IMPLEMENT

	return false;
};

bool hs_parse_arithmetic(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

bool hs_parse_equality(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

bool hs_parse_inequality(int16 function_index, int32 expression_index)
{
	ASSERT(function_index >= _hs_function_gt && function_index <= _hs_function_lte);
	bool success = false;

	long parameter_indices[2]{};
	if (hs_get_parameter_indices(hs_function_get(function_index)->name, NUMBEROF(parameter_indices), parameter_indices, expression_index))
	{
		if (hs_parse(parameter_indices[0], _hs_unparsed)
			&& (hs_syntax_get(parameter_indices[0])->type >= _hs_type_game_difficulty
				&& hs_syntax_get(parameter_indices[0])->type <= _hs_type_character_physics
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
			|| (hs_syntax_get(parameter_indices[1])->type < _hs_type_game_difficulty
				|| hs_syntax_get(parameter_indices[1])->type > _hs_type_character_physics)
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
	// $IMPLEMENT

	return false;
};

bool hs_parse_sleep_forever(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

bool hs_parse_sleep_until(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

bool hs_parse_wake(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
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
	// $IMPLEMENT

	return false;
};

