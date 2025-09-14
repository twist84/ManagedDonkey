#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs.hpp"
#include "hs/hs_scenario_definitions.hpp"

enum
{
	_hs_function_flag_command_script_atom = 0,
	_hs_function_flag_internal,
	_hs_function_flag_debug,

	NUMBER_OF_HS_FUNCTION_FLAGS,
};

using hs_function_parser_t = bool __cdecl(int16 function_index, int32 expression_index);
using hs_function_evaluate_t = void __cdecl(int16 function_index, int32 thread_index, bool a3);

struct hs_function_definition
{
	c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> return_type;
	uns16 flags;
	hs_function_evaluate_t* evaluate;
	const char* parameters;

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
	const char* name;
	uns16 flags;

	hs_function_parser_t* parse;
	hs_function_evaluate_t* evaluate;

	const char* documentation;
	const char* parameters;

	int16 formal_parameter_count;
	union
	{
		//__pragma(warning(disable : 4200)) c_enum<e_hs_type, int16, _hs_unparsed, k_hs_type_count> formal_parameters_[];
		__pragma(warning(disable : 4200)) int16 formal_parameters[];
	};
};
static_assert(sizeof(hs_function_definition_debug) >= 0x20);

int32 const hs_function_table_count = 1697;
extern const hs_function_definition* (&hs_function_table)[hs_function_table_count];
extern const char* const hs_function_table_names[hs_function_table_count];

extern hs_function_definition_debug* hs_function_table_debug[hs_function_table_count];
extern int32 const hs_function_table_debug_count;

