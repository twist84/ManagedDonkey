#pragma once

enum
{
	_hs_function_flag_command_script_atom = 0,
	_hs_function_flag_internal,
	_hs_function_flag_debug,

	NUMBER_OF_HS_FUNCTION_FLAGS,
};

typedef bool(__cdecl* hs_parse_function_definition)(int16 function_index, int32 expression_index);
typedef void(__cdecl* hs_evaluate_function_definition)(int16 function_index, int32 thread_index, bool initialize);

struct hs_function_definition
{
	int16 return_type; // e_hs_type
	const char* name;
	uns16 flags;

	bool(__cdecl* parse)(int16 function_index, int32 expression_index);
	void(__cdecl* evaluate)(int16 function_index, int32 thread_index, bool initialize);

	const char* documentation;
	const char* parameters;

	int16 formal_parameter_count;
	__pragma(warning(disable : 4200)) int16 formal_parameters[];
};
static_assert(sizeof(hs_function_definition) >= 0x20);

extern const hs_function_definition* const hs_function_table[];
extern const int32 hs_function_table_count;

