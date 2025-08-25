#include "hs/hs_library_internal_compile.hpp"

bool hs_get_parameter_indices(const char* function_name, int16 count, int32* result_indices, int32 expression_index)
{
	hs_syntax_node* expr_node = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expr_node->long_value)->next_node_index;

	bool match_count = false;

	if (next_node_index == NONE)
	{
		match_count = (count == 0);
	}
	else
	{
		for (int16 i = 0; i < count; i++)
		{
			result_indices[i] = next_node_index;

			if (hs_syntax_get(next_node_index))
			{
				next_node_index = hs_syntax_get(next_node_index)->next_node_index;
			}
			else
			{
				break;
			}

			if (next_node_index == NONE)
			{
				match_count = (i + 1 == count);
				break;
			}
		}
	}

	if (match_count && next_node_index == NONE)
		return true;

	csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
		"the %s call requires %d arguments.",
		function_name,
		count);

	hs_compile_globals.error_message = hs_compile_globals.error_buffer;
	hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;

	return false;
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
			int16 type = next_next_node_index == NONE ? expression->type.get() : _hs_type_void;
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
				hs_function_get_debug(function_index)->name);
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
	bool parse_success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	if (next_node_index == NONE)
	{
		hs_compile_globals.error_message = "i expected a variable to set and a value.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		return false;
	}

	int32 next_next_node_index = hs_syntax_get(next_node_index)->next_node_index;
	if (next_next_node_index == NONE)
	{
		hs_compile_globals.error_message = "i expected a variable to set and a value.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		return false;
	}

	if (hs_syntax_get(next_next_node_index)->next_node_index != NONE)
	{
		hs_compile_globals.error_message = "i didn't expect this argument.";
		hs_compile_globals.error_offset = hs_syntax_get(hs_syntax_get(next_next_node_index)->next_node_index)->source_offset;
		return false;
	}

	hs_syntax_node* next_expression = hs_syntax_get(next_node_index);
	int16 global_index = hs_find_global_by_name(&hs_compile_globals.compiled_source[next_expression->source_offset]);
	if (global_index == NONE)
	{
		hs_compile_globals.error_message = "this is not a valid global variable.";
		hs_compile_globals.error_offset = next_expression->source_offset;
		return false;
	}

	next_expression->type = hs_global_get_type(global_index);
	if (expression->type && !hs_can_cast(next_expression->type, expression->type))
	{
		hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"you cannot pass the result of this set (type %s) to a function that expects type %s.",
			hs_type_names[next_expression->type.get()],
			hs_type_names[expression->type.get()]);
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		return false;
	}

	ASSERT(hs_parse_variable(next_node_index));
	if (!expression->type)
		expression->type = next_expression->type;

	if (!hs_parse(next_next_node_index, next_expression->type))
		return false;

	return true;
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
	// $IMPLEMENT

	return false;
};

bool hs_parse_sleep(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

bool hs_parse_sleep_for_ticks(int16 function_index, int32 expression_index)
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

	int32 argument_index;
	if (!hs_get_parameter_indices(hs_function_table_debug[function_index]->name, 1, &argument_index, expression_index))
	{
		return false;
	}

	if (!hs_parse(argument_index, _hs_unparsed))
	{
		if (!hs_compile_globals.error_message)
		{
			hs_compile_globals.error_message = "this is not a global variable reference, function call, or script call.";
			hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		}
		return false;
	}

	return true;
};

bool hs_parse_object_cast_up(int16 function_index, int32 expression_index)
{
	// $IMPLEMENT

	return false;
};

