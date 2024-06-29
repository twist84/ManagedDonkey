#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_scenario_definitions.hpp"

using hs_function_parser_t = bool __cdecl(short function_index, long expression_index);
using hs_function_evaluate_t = void __cdecl(short function_index, long thread_index, bool a3);

struct hs_function_definition
{
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;
	word_flags flags;
	hs_function_evaluate_t* evaluate;
	char const* usage;

	short parameter_count;
#pragma warning(push)
#pragma warning(disable : 4200)
	union
	{
		c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> parameters[];
		short formal_parameters[];
	};
#pragma warning(pop)
};
static_assert(sizeof(hs_function_definition) == 0x10);

struct hs_function_definition_debug
{
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;
	char const* name;
	word_flags flags;

	hs_function_parser_t* parse;
	hs_function_evaluate_t* evaluate;

	char const* description;
	char const* usage;

	short parameter_count;
#pragma warning(push)
#pragma warning(disable : 4200)
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> parameters[];
#pragma warning(pop)
};
static_assert(sizeof(hs_function_definition_debug) == 0x20);

long const hs_function_table_count = 1697;
extern hs_function_definition const* (&hs_function_table)[hs_function_table_count];
extern char const* const hs_function_table_names[hs_function_table_count];

extern hs_function_definition_debug const* const hs_function_table_debug[];
extern long const hs_function_table_debug_count;

