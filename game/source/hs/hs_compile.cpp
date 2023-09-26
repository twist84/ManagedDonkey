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

hs_type_primitive_parser_t hs_type_primitive_parsers[k_hs_type_count]
{
	// non-types
	nullptr,
	nullptr,
	nullptr,

	// void
	nullptr,

	hs_type_primitive_parser_bool,
};

