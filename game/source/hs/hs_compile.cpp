#include "hs/hs_compile.hpp"

#include "hs/hs.hpp"

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
};

