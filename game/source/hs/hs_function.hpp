#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_scenario_definitions.hpp"

using hs_function_parser_t = bool __cdecl(int16 function_index, int32 expression_index);
using hs_function_evaluate_t = void __cdecl(int16 function_index, int32 thread_index, bool a3);

struct hs_function_definition
{
	c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> return_type;
	uint16 flags;
	hs_function_evaluate_t* evaluate;
	char const* parameters;

	int16 formal_parameter_count;
	union
	{
		__pragma(warning(disable : 4200)) c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> formal_parameters_[];
		__pragma(warning(disable : 4200)) int16 formal_parameters[];
	};
};
static_assert(sizeof(hs_function_definition) == 0x10);

struct hs_function_definition_debug
{
	c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> return_type;
	char const* name;
	uint16 flags;

	hs_function_parser_t* parse;
	hs_function_evaluate_t* evaluate;

	char const* documentation;
	char const* parameters;

	int16 formal_parameter_count;
	union
	{
		//__pragma(warning(disable : 4200)) c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> formal_parameters_[];
		__pragma(warning(disable : 4200)) int16 formal_parameters[];
	};
};
static_assert(sizeof(hs_function_definition_debug) >= 0x20);

int32 const hs_function_table_count = 1697;
extern hs_function_definition const* (&hs_function_table)[hs_function_table_count];
extern char const* const hs_function_table_names[hs_function_table_count];

extern hs_function_definition_debug* hs_function_table_debug[hs_function_table_count];
extern int32 const hs_function_table_debug_count;

