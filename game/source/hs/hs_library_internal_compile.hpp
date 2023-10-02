#pragma once

#include "hs/hs.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_runtime.hpp"

enum e_hs_function
{
	_hs_function_begin = 0,
	_hs_function_begin_random,
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
		if (argument_count >= 1)
		{
			if (argument_count > 32 && function_index == _hs_function_begin_random)
			{
				hs_compile_globals.error_message = "begin_random can take a maximum of 32 arguments (matt can increase this.)";
				hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
				parse_success = false;
			}
		}
		else
		{
			hs_compile_globals.error_message = csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"a statement block must contain at least one argument.",
				hs_function_get_debug(function_index)->name);
			hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
			parse_success = false;
		}
	}

	return parse_success;
}

