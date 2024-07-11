#pragma once

#include "hs/hs.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_runtime.hpp"

enum e_hs_function
{
	_hs_function_begin = 0,
	_hs_function_begin_random,
	_hs_function_if,
	_hs_function_cond,
	_hs_function_set,

	// ...
};

bool hs_parse_begin(short function_index, long expression_index)
{
	bool parse_success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(function_index == _hs_function_begin || function_index == _hs_function_begin_random);

	short argument_count = 0;
	while (parse_success && next_node_index != NONE)
	{
		long next_next_node_index = hs_syntax_get(next_node_index)->next_node_index;
		if (function_index)
		{
			parse_success = hs_parse(next_node_index, expression->type);
			if (!expression->type && parse_success)
				expression->type = hs_syntax_get(next_node_index)->type;
		}
		else
		{
			short type = next_next_node_index == NONE ? expression->type.get() : _hs_type_void;
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

bool hs_parse_if(short function_index, long expression_index)
{
	bool parse_success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	ASSERT(function_index == _hs_function_if);

	long next_next_expression = NONE;
	long next_next_next_node_index = NONE;
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

bool hs_parse_cond(short function_index, long expression_index)
{
	// #TODO: implement

	return false;
}

bool hs_parse_set(short function_index, long expression_index)
{
	bool parse_success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	long next_node_index = hs_syntax_get(expression->long_value)->next_node_index;

	if (next_node_index == NONE)
	{
		hs_compile_globals.error_message = "i expected a variable to set and a value.";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
		return false;
	}

	long next_next_node_index = hs_syntax_get(next_node_index)->next_node_index;
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
	short global_index = hs_find_global_by_name(&hs_compile_globals.compiled_source[next_expression->source_offset]);
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

