#include "hs/hs_function.hpp"

#include "ai/ai_script.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_library_external.hpp"
#include "hs/hs_library_internal_compile.hpp"

#include <utility>

template<typename t_parameter_type>
t_parameter_type convert_parameter(int32 value)
{
	if constexpr (std::is_pointer_v<t_parameter_type>)
	{
		return reinterpret_cast<t_parameter_type>(static_cast<uintptr_t>(value));
	}
	else
	{
		return static_cast<t_parameter_type>(value);
	}
}

template<typename t_result_type, typename... t_parameters, size_t... index>
constexpr void call_with_parameters(e_hs_type return_type, t_result_type(*func)(t_parameters...), int32& result, int32* actual_parameters, std::index_sequence<index...>)
{
	if constexpr (std::is_void_v<t_result_type>)
	{
		ASSERT(return_type == _hs_type_void);
		func(convert_parameter<t_parameters>(actual_parameters[index])...);
	}
	else
	{
		ASSERT(return_type != _hs_type_void);
		*reinterpret_cast<t_result_type*>(&result) = func(convert_parameter<t_parameters>(actual_parameters[index])...);
	}
}

template<typename t_result_type>
constexpr void call_without_parameters(e_hs_type return_type, t_result_type(*func)(), int32& result)
{
	if constexpr (std::is_void_v<t_result_type>)
	{
		ASSERT(return_type == _hs_type_void);
		func();
	}
	else
	{
		ASSERT(return_type != _hs_type_void);
		*reinterpret_cast<t_result_type*>(&result) = func();
	}
}

template<typename t_result_type, typename... t_parameters, size_t t_num_parameter = sizeof...(t_parameters)>
constexpr void __cdecl t_macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, e_hs_type return_type, t_result_type(*func)(t_parameters...))
{
	int32 result = 0;
	if constexpr (t_num_parameter > 0)
	{
		int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize);
		if (actual_parameters)
		{
			call_with_parameters(return_type, func, result, actual_parameters, std::make_index_sequence<t_num_parameter>{});
			hs_return(thread_index, result);
		}
	}
	else
	{
		call_without_parameters(return_type, func, result);
		hs_return(thread_index, result);
	}
}

#define MACRO_FUNCTION_EVALUATE_NAME(NAME, FUNCTION, NUM_PARAMS) NAME##_##FUNCTION##_##NUM_PARAMS##_evaluate
#define MACRO_FUNCTION_EVALUATE(NAME, FUNCTION, NUM_PARAMS, RETURN_TYPE) \
void MACRO_FUNCTION_EVALUATE_NAME(NAME, FUNCTION, NUM_PARAMS)(int16 function_index, int32 thread_index, bool initialize) \
{ \
	t_macro_function_evaluate(function_index, thread_index, initialize, RETURN_TYPE, FUNCTION); \
}

void __cdecl MACRO_FUNCTION_EVALUATE_NAME(evaluate, hs_evaluate, 1)(int16 function_index, int32 thread_index, bool initialize)
{
	INVOKE(0x00748490, MACRO_FUNCTION_EVALUATE_NAME(evaluate, hs_evaluate, 1), function_index, thread_index, initialize);
}

DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) begin_definition
{
	.return_type = _hs_passthrough,
	.name = "begin",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_begin,
	.evaluate = hs_evaluate_begin,
	.documentation = "returns the last expression in a sequence after evaluating the sequence in order.\r\nNETWORK SAFE: Yes",
	.parameters = "<expression(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) begin_random_definition
{
	.return_type = _hs_passthrough,
	.name = "begin_random",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_begin,
	.evaluate = hs_evaluate_begin_random,
	.documentation = "evaluates the sequence of expressions in random order and returns the last value evaluated.\r\nNETWORK SAFE: Yes",
	.parameters = "<expression(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) if_definition
{
	.return_type = _hs_passthrough,
	.name = "if",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_if,
	.evaluate = hs_evaluate_if,
	.documentation = "returns one of two values based on the value of a condition.\r\nNETWORK SAFE: Yes",
	.parameters = "<boolean> <then> [<else>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cond_definition
{
	.return_type = _hs_passthrough,
	.name = "cond",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_cond,
	.evaluate = NULL,
	.documentation = "returns the value associated with the first true condition.\r\nNETWORK SAFE: Yes",
	.parameters = "(<boolean1> <result1>) [(<boolean2> <result2>) [...]]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) set_definition
{
	.return_type = _hs_passthrough,
	.name = "set",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_set,
	.evaluate = hs_evaluate_set,
	.documentation = "set the value of a global variable.\r\nNETWORK SAFE: Yes (depending on result)",
	.parameters = "<variable name> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) and_definition
{
	.return_type = _hs_type_boolean,
	.name = "and",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_logical,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns true if all specified expressions are true.\r\nNETWORK SAFE: Yes",
	.parameters = "<boolean(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) or_definition
{
	.return_type = _hs_type_boolean,
	.name = "or",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_logical,
	.evaluate = hs_evaluate_logical,
	.documentation = "returns true if any specified expressions are true.\r\nNETWORK SAFE: Yes",
	.parameters = "<boolean(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) add_definition
{
	.return_type = _hs_type_real,
	.name = "+",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the sum of all specified expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) subtract_definition
{
	.return_type = _hs_type_real,
	.name = "-",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the difference of two expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) multiply_definition
{
	.return_type = _hs_type_real,
	.name = "*",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the product of all specified expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) divide_definition
{
	.return_type = _hs_type_real,
	.name = "/",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the quotient of two expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) modulo_definition
{
	.return_type = _hs_type_real,
	.name = "%",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the remainder of two expressions\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) min_definition
{
	.return_type = _hs_type_real,
	.name = "min",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the minimum of all specified expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number(s)",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) max_definition
{
	.return_type = _hs_type_real,
	.name = "max",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = "returns the maximum of all specified expressions.\r\nNETWORK SAFE: Yes",
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) equal_definition
{
	.return_type = _hs_type_boolean,
	.name = "=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_equality,
	.evaluate = hs_evaluate_equality,
	.documentation = "returns true if two expressions are equal\r\nNETWORK SAFE: Yes",
	.parameters = "<expression> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) not_equal_definition
{
	.return_type = _hs_type_boolean,
	.name = "!=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_equality,
	.evaluate = hs_evaluate_equality,
	.documentation = "returns true if two expressions are not equal\r\nNETWORK SAFE: Yes",
	.parameters = "<expression> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gt_definition
{
	.return_type = _hs_type_boolean,
	.name = ">",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = "returns true if the first number is larger than the second.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) lt_definition
{
	.return_type = _hs_type_boolean,
	.name = "<",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = "returns true if the first number is smaller than the second.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gte_definition
{
	.return_type = _hs_type_boolean,
	.name = ">=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = "returns true if the first number is larger than or equal to the second.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) lte_definition
{
	.return_type = _hs_type_boolean,
	.name = "<=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = "returns true if the first number is smaller than or equal to the second.\r\nNETWORK SAFE: Yes",
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_definition
{
	.return_type = _hs_type_void,
	.name = "sleep",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep,
	.evaluate = hs_evaluate_sleep,
	.documentation = "pauses execution of this script (or, optionally, another script) for the specified number of ticks @ 30Hz.\r\nNETWORK SAFE: Yes",
	.parameters = "<short> [<script>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_for_ticks_definition
{
	.return_type = _hs_type_void,
	.name = "sleep_for_ticks",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = (hs_parse_function_definition)NULL, // $TODO write the function chuckle nuts
	.evaluate = (hs_evaluate_function_definition)NULL, // $TODO write the function chuckle nuts
	.documentation = "pauses execution of this script (or, optionally, another script) for the specified number of ticks.\r\nNETWORK SAFE: Yes",
	.parameters = "<short> [<script>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_forever_definition
{
	.return_type = _hs_type_void,
	.name = "sleep_forever",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep_forever,
	.evaluate = hs_evaluate_sleep_forever,
	.documentation = "pauses execution of this script (or, optionally, another script) forever.\r\nNETWORK SAFE: Yes",
	.parameters = "[<script>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_until_definition
{
	.return_type = _hs_type_boolean,
	.name = "sleep_until",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep_until,
	.evaluate = hs_evaluate_sleep_until,
	.documentation = "pauses execution of this script until the specified condition is true, checking once per second unless a different number of ticks is specified.\r\nNETWORK SAFE: Yes",
	.parameters = "<boolean> [<short>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) wake_definition
{
	.return_type = _hs_type_void,
	.name = "wake",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_wake,
	.evaluate = hs_evaluate_wake,
	.documentation = "wakes a sleeping script in the next update.\r\nNETWORK SAFE: Yes",
	.parameters = "<script name>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) inspect_definition
{
	.return_type = _hs_type_void,
	.name = "inspect",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inspect,
	.evaluate = hs_evaluate_inspect,
	.documentation = "prints the value of an expression to the screen for debugging purposes.\r\nNETWORK SAFE: Yes",
	.parameters = "<expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) object_to_unit_definition
{
	.return_type = _hs_type_unit,
	.name = "unit",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_object_cast_up,
	.evaluate = hs_evaluate_object_cast_up,
	.documentation = "converts an object to a unit.\r\nNETWORK SAFE: No",
	.parameters = "<object>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) evaluate_definition
{
	.return_type = _hs_type_void,
	.name = "evaluate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(evaluate, hs_evaluate, 1), // (hs_evaluate_function_definition)0x00748490,
	.documentation = "Evaluate the given script",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) not_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "not",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748840, // $TODO write the function chuckle nuts
	.documentation = "returns the opposite of the expression.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) pin_3_definition
{
	.return_type = _hs_type_real,
	.name = "pin",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F9B0, // $TODO write the function chuckle nuts
	.documentation = "returns the first value pinned between the second two\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
MACRO_FUNCTION_EVALUATE(print, hs_print, 1, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) print_1_definition
{
	.return_type = _hs_type_void,
	.name = "print",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(print, hs_print, 1), // (hs_evaluate_function_definition)0x0072FE80,
	.documentation = "prints a string to the console.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
MACRO_FUNCTION_EVALUATE(log_print, hs_log_print, 1, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) log_print_1_definition
{
	.return_type = _hs_type_void,
	.name = "log_print",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(log_print, hs_log_print, 1), // (hs_evaluate_function_definition)0x007302D0,
	.documentation = "prints a string to the hs log file.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_scripting_show_thread_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_scripting_show_thread",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007305D0, // $TODO write the function chuckle nuts
	.documentation = "shows or hides the display of any thread containing the given substring.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_script_thread_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_script_thread",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730900, // $TODO write the function chuckle nuts
	.documentation = "Verbose threads spew to log about script and function calls.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_scripting_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_scripting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730B80, // $TODO write the function chuckle nuts
	.documentation = "Turn on/off hs script debugging.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_scripting_globals_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_scripting_globals",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730E80, // $TODO write the function chuckle nuts
	.documentation = "Turn on/off hs global debugging.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_scripting_variable_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_scripting_variable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731220, // $TODO write the function chuckle nuts
	.documentation = "Turn on/off debugging of a specific global variable; enclose the variable name with single quote characters ('my_variable')\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_scripting_variable_all_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_scripting_variable_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007316D0, // $TODO write the function chuckle nuts
	.documentation = "Turn on/off debugging of all global variables\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
MACRO_FUNCTION_EVALUATE(breakpoint, hs_breakpoint, 1, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) breakpoint_1_definition
{
	.return_type = _hs_type_void,
	.name = "breakpoint",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731950, // $TODO write the function chuckle nuts
	.documentation = "If breakpoints are enabled, pause execution when this statement is hit (displaying the given message).\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) kill_active_scripts_0_definition
{
	.return_type = _hs_type_void,
	.name = "kill_active_scripts",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731CD0, // $TODO write the function chuckle nuts
	.documentation = "Terminates all currently running threads.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) get_executing_running_thread_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "get_executing_running_thread",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731F10, // $TODO write the function chuckle nuts
	.documentation = "Retrieves the current executing thread index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) kill_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "kill_thread",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007322A0, // $TODO write the function chuckle nuts
	.documentation = "Kill the specified thread\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) script_started_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "script_started",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732630, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the continuous, dormant or startup script was started.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) script_finished_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "script_finished",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732A20, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the continuous, dormant or startup script was finished.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) players_0_definition
{
	.return_type = _hs_type_object_list,
	.name = "players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732E60, // $TODO write the function chuckle nuts
	.documentation = "returns a list of the players (DO NOT USE TO COUNT PLAYERS IN COOP GAME, USE game_coop_player_count)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_get_1_definition
{
	.return_type = _hs_type_unit,
	.name = "player_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007331C0, // $TODO write the function chuckle nuts
	.documentation = "Get a player by absolute index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) kill_volume_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "kill_volume_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733600, // $TODO write the function chuckle nuts
	.documentation = "enables a kill volume\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_trigger_volume
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) kill_volume_disable_1_definition
{
	.return_type = _hs_type_void,
	.name = "kill_volume_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007339B0, // $TODO write the function chuckle nuts
	.documentation = "disables a kill volume\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_trigger_volume
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) volume_teleport_players_not_inside_2_definition
{
	.return_type = _hs_type_void,
	.name = "volume_teleport_players_not_inside",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733C60, // $TODO write the function chuckle nuts
	.documentation = "moves all players outside a specified trigger volume to a specified flag.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) volume_test_object_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "volume_test_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734130, // $TODO write the function chuckle nuts
	.documentation = "returns true if the specified object is within the specified volume.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) volume_test_objects_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "volume_test_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007344F0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any of the specified objects are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) volume_test_objects_all_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "volume_test_objects_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734A70, // $TODO write the function chuckle nuts
	.documentation = "returns true if any of the specified objects are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) volume_test_players_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "volume_test_players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734F90, // $TODO write the function chuckle nuts
	.documentation = "returns true if any players are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_trigger_volume
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) volume_test_players_all_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "volume_test_players_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735430, // $TODO write the function chuckle nuts
	.documentation = "returns true if all players are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_trigger_volume
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) volume_return_objects_1_definition
{
	.return_type = _hs_type_object_list,
	.name = "volume_return_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007357C0, // $TODO write the function chuckle nuts
	.documentation = "returns list of objects in volume or (max 128)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_trigger_volume
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) volume_return_objects_by_type_2_definition
{
	.return_type = _hs_type_object_list,
	.name = "volume_return_objects_by_type",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735CF0, // $TODO write the function chuckle nuts
	.documentation = "returns list of objects in volume or (max 128).\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) zone_set_trigger_volume_enable_2_definition
{
	.return_type = _hs_type_void,
	.name = "zone_set_trigger_volume_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736100, // $TODO write the function chuckle nuts
	.documentation = "enables/disables the trigger volume(s) with the given name that cause zone set switches\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) list_get_2_definition
{
	.return_type = _hs_type_object,
	.name = "list_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736480, // $TODO write the function chuckle nuts
	.documentation = "returns an item in an object list.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) list_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "list_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736930, // $TODO write the function chuckle nuts
	.documentation = "returns the number of objects in a list\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) list_count_not_dead_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "list_count_not_dead",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736E40, // $TODO write the function chuckle nuts
	.documentation = "returns the number of objects in a list that aren't dead\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) effect_new_2_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007371A0, // $TODO write the function chuckle nuts
	.documentation = "starts the specified effect at the specified flag.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) effect_new_random_2_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new_random",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737580, // $TODO write the function chuckle nuts
	.documentation = "starts the specified effect at one of the points in the given a point set.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) effect_new_at_ai_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new_at_ai_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737A30, // $TODO write the function chuckle nuts
	.documentation = "starts the specified effect at the specified ai point.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) effect_new_on_object_marker_3_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new_on_object_marker",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737D50, // $TODO write the function chuckle nuts
	.documentation = "starts the specified effect on the specified object at the specified marker.""\r\nNETWORK SAFE: ""No (partial support exists, no general solution)",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) effect_new_on_ground_2_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new_on_ground",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007382C0, // $TODO write the function chuckle nuts
	.documentation = "starts the specified effect on the ground underneath the objects root.\r\nNETWORK SAFE: Dunno",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) damage_new_2_definition
{
	.return_type = _hs_type_void,
	.name = "damage_new",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007386E0, // $TODO write the function chuckle nuts
	.documentation = "causes the specified damage at the specified flag.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_damage,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) damage_object_effect_2_definition
{
	.return_type = _hs_type_void,
	.name = "damage_object_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738AC0, // $TODO write the function chuckle nuts
	.documentation = "causes the specified damage at the specified object.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_damage,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) damage_objects_effect_2_definition
{
	.return_type = _hs_type_void,
	.name = "damage_objects_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738E60, // $TODO write the function chuckle nuts
	.documentation = "causes the specified damage at the specified object list.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_damage,
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) damage_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "damage_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739270, // $TODO write the function chuckle nuts
	.documentation = "causes the specified damage at the specified object.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) damage_objects_3_definition
{
	.return_type = _hs_type_void,
	.name = "damage_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007396E0, // $TODO write the function chuckle nuts
	.documentation = "causes the specified damage at the specified object list.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) damage_players_1_definition
{
	.return_type = _hs_type_void,
	.name = "damage_players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739AF0, // $TODO write the function chuckle nuts
	.documentation = "damages all players with the given damage effect\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_damage
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) soft_ceiling_enable_2_definition
{
	.return_type = _hs_type_void,
	.name = "soft_ceiling_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739F20, // $TODO write the function chuckle nuts
	.documentation = "turn on or off a soft ceiling\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A270, // $TODO write the function chuckle nuts
	.documentation = "creates an object from the scenario.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_clone_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_clone",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A700, // $TODO write the function chuckle nuts
	.documentation = "creates an object, potentially resulting in multiple objects if it already exists.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_anew_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_anew",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A9B0, // $TODO write the function chuckle nuts
	.documentation = "creates an object, destroying it first if it already exists.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_if_necessary_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_if_necessary",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ADC0, // $TODO write the function chuckle nuts
	.documentation = "creates an object if it doesn't already exists.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_containing_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_containing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B0E0, // $TODO write the function chuckle nuts
	.documentation = "creates all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_clone_containing_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_clone_containing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B3D0, // $TODO write the function chuckle nuts
	.documentation = "creates clones for all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_anew_containing_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_anew_containing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B7C0, // $TODO write the function chuckle nuts
	.documentation = "creates anew all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_folder_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_folder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BB30, // $TODO write the function chuckle nuts
	.documentation = "creates all the objects in the given folder\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_folder
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_create_folder_anew_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_create_folder_anew",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BF40, // $TODO write the function chuckle nuts
	.documentation = "creates all the objects in the given folder\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_folder
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_destroy_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C2A0, // $TODO write the function chuckle nuts
	.documentation = "destroys an object.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_destroy_containing_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_destroy_containing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C570, // $TODO write the function chuckle nuts
	.documentation = "destroys all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) object_destroy_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "object_destroy_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CA20, // $TODO write the function chuckle nuts
	.documentation = "destroys all non player objects.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_destroy_type_mask_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_destroy_type_mask",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CC10, // $TODO write the function chuckle nuts
	.documentation = "destroys all objects matching the type mask\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objects_delete_by_definition_1_definition
{
	.return_type = _hs_type_void,
	.name = "objects_delete_by_definition",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CF60, // $TODO write the function chuckle nuts
	.documentation = "deletes all objects of type <definition>\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_destroy_folder_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_destroy_folder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D2F0, // $TODO write the function chuckle nuts
	.documentation = "destroys all objects in the given folder.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_folder
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_hide_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_hide",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D620, // $TODO write the function chuckle nuts
	.documentation = "hides or shows the object passed in\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_shadowless_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shadowless",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DA40, // $TODO write the function chuckle nuts
	.documentation = "set/reset shadow castingness of object\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_buckling_magnitude_get_1_definition
{
	.return_type = _hs_type_real,
	.name = "object_buckling_magnitude_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DE90, // $TODO write the function chuckle nuts
	.documentation = "returns the amoount [0-1] that a scarab is buckling\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_function_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_function_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E3C0, // $TODO write the function chuckle nuts
	.documentation = "sets a global object function (0-3) to value\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) object_set_function_variable_4_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_function_variable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E6A0, // $TODO write the function chuckle nuts
	.documentation = "sets funciton variable for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_clear_function_variable_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_clear_function_variable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EA70, // $TODO write the function chuckle nuts
	.documentation = "clears one funciton variables for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_clear_all_function_variables_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_clear_all_function_variables",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EE90, // $TODO write the function chuckle nuts
	.documentation = "clears all funciton variables for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_dynamic_simulation_disable_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_dynamic_simulation_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F240, // $TODO write the function chuckle nuts
	.documentation = "disabled dynamic simulation for this object (makes it fixed)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_phantom_power_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_phantom_power",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F650, // $TODO write the function chuckle nuts
	.documentation = "sets phantom power to be latched at 1.0f or 0.0f\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_wake_physics_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_wake_physics",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FA90, // $TODO write the function chuckle nuts
	.documentation = "wakes physics of an object.  For example it would make an usupported crate fall\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_ranged_attack_inhibited_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_ranged_attack_inhibited",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FD60, // $TODO write the function chuckle nuts
	.documentation = "FALSE prevents object from using ranged attack\r\nNETWORK SAFE: Yes (actors only)",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_melee_attack_inhibited_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_melee_attack_inhibited",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007401C0, // $TODO write the function chuckle nuts
	.documentation = "FALSE prevents object from using melee attack\r\nNETWORK SAFE: Yes (actors only)",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) objects_dump_memory_0_definition
{
	.return_type = _hs_type_void,
	.name = "objects_dump_memory",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007405D0, // $TODO write the function chuckle nuts
	.documentation = "debugs object memory usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_get_health_1_definition
{
	.return_type = _hs_type_real,
	.name = "object_get_health",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740820, // $TODO write the function chuckle nuts
	.documentation = "returns the health [0,1] of the object, returns -1 if the object does not exist\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_get_shield_1_definition
{
	.return_type = _hs_type_real,
	.name = "object_get_shield",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740C00, // $TODO write the function chuckle nuts
	.documentation = "returns the shield [0,1] of the object, returns -1 if the object does not exist\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) object_set_shield_effect_3_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shield_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741110, // $TODO write the function chuckle nuts
	.documentation = "sets the shield response effect (not current shield amount) to a given value over the given number of seconds (cinematic use only, remember to call (object_set_shield_effect 0 0) after use!)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_physics_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_physics",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741460, // $TODO write the function chuckle nuts
	.documentation = "prevents an object from running physics or colliding with any other objects\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_get_parent_1_definition
{
	.return_type = _hs_type_object,
	.name = "object_get_parent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007418B0, // $TODO write the function chuckle nuts
	.documentation = "returns the parent of the given object\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) objects_attach_4_definition
{
	.return_type = _hs_type_void,
	.name = "objects_attach",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741C70, // $TODO write the function chuckle nuts
	.documentation = "attaches the second object to the first both strings can be empty\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_at_marker_2_definition
{
	.return_type = _hs_type_object,
	.name = "object_at_marker",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007421F0, // $TODO write the function chuckle nuts
	.documentation = "returns the object attached to the marker of the given parent object\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) objects_detach_2_definition
{
	.return_type = _hs_type_void,
	.name = "objects_detach",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742670, // $TODO write the function chuckle nuts
	.documentation = "detaches from the given parent object the given child object\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) object_set_scale_3_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_scale",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742A30, // $TODO write the function chuckle nuts
	.documentation = "sets the scale for a given object and interpolates over the given number of frames to achieve that scale\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_velocity_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_velocity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742E50, // $TODO write the function chuckle nuts
	.documentation = "Sets the (object-relative) forward velocity of the given object\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_inertia_tensor_scale_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_inertia_tensor_scale",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743300, // $TODO write the function chuckle nuts
	.documentation = "0==default of 1.0, otherwise it is scale on this objects inertia tensor\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_collision_damage_armor_scale_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_collision_damage_armor_scale",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743680, // $TODO write the function chuckle nuts
	.documentation = "0==default of 1.0, otherwise it is scale on how this objects takes collisoin damage, >1 == more protection, <1 == less\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) object_set_velocity_4_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_velocity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743BA0, // $TODO write the function chuckle nuts
	.documentation = "Sets the (object-relative) velocity of the given object\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_set_deleted_when_deactivated_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_deleted_when_deactivated",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743F70, // $TODO write the function chuckle nuts
	.documentation = "when this object deactivates it will be deleted\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_model_target_destroyed_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "object_model_target_destroyed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744330, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the specified model target is destroyed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_model_targets_destroyed_2_definition
{
	.return_type = _hs_type_short_integer,
	.name = "object_model_targets_destroyed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744890, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the specified model target is destroyed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) object_damage_damage_section_3_definition
{
	.return_type = _hs_type_void,
	.name = "object_damage_damage_section",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744C80, // $TODO write the function chuckle nuts
	.documentation = "applies damage to a damage section, causing all manner of effects/constraint breakage to occur\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_cannot_die_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_cannot_die",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007451F0, // $TODO write the function chuckle nuts
	.documentation = "Set whether the object can die from damage or not (as opposed to by scripting)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_vitality_pinned_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "object_vitality_pinned",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745460, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the object',27h,'s vitality is currently pinned at some minimum value because it cannot die.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) garbage_collect_now_0_definition
{
	.return_type = _hs_type_void,
	.name = "garbage_collect_now",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745AC0, // $TODO write the function chuckle nuts
	.documentation = "causes all garbage objects except those visible to a player to be collected immediately\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) garbage_collect_unsafe_0_definition
{
	.return_type = _hs_type_void,
	.name = "garbage_collect_unsafe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745C20, // $TODO write the function chuckle nuts
	.documentation = "forces all garbage objects to be collected immediately, even those visible to a player (dangerous!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) garbage_collect_multiplayer_0_definition
{
	.return_type = _hs_type_void,
	.name = "garbage_collect_multiplayer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745E40, // $TODO write the function chuckle nuts
	.documentation = "runs multiplayer garbage collection\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_cannot_take_damage_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_cannot_take_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746110, // $TODO write the function chuckle nuts
	.documentation = "prevents an object from taking damage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_can_take_damage_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_can_take_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746410, // $TODO write the function chuckle nuts
	.documentation = "allows an object to take damage again\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_cinematic_lod_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_cinematic_lod",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007467B0, // $TODO write the function chuckle nuts
	.documentation = "makes an object use the highest lod for the remainder of the levels' cutscenes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_cinematic_collision_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_cinematic_collision",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746B70, // $TODO write the function chuckle nuts
	.documentation = "makes an object not collide with other cinematic collision objects.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_cinematic_visibility_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_cinematic_visibility",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746EE0, // $TODO write the function chuckle nuts
	.documentation = "makes an object bypass visibility and always render during cinematics.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objects_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "objects_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747290, // $TODO write the function chuckle nuts
	.documentation = "loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objects_predict_high_1_definition
{
	.return_type = _hs_type_void,
	.name = "objects_predict_high",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747680, // $TODO write the function chuckle nuts
	.documentation = "loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objects_predict_low_1_definition
{
	.return_type = _hs_type_void,
	.name = "objects_predict_low",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747950, // $TODO write the function chuckle nuts
	.documentation = "loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_type_predict_high_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_type_predict_high",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747E10, // $TODO write the function chuckle nuts
	.documentation = "loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_type_predict_low_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_type_predict_low",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748050, // $TODO write the function chuckle nuts
	.documentation = "loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_type_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_type_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748460, // $TODO write the function chuckle nuts
	.documentation = "loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_teleport_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_teleport",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748800, // $TODO write the function chuckle nuts
	.documentation = "moves the specified object to the specified flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_teleport_to_ai_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_teleport_to_ai_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F8C0, // $TODO write the function chuckle nuts
	.documentation = "moves the specified object to the specified ai point.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_facing_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_facing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FCA0, // $TODO write the function chuckle nuts
	.documentation = "turns the specified object in the direction of the specified flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_shield_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shield",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730110, // $TODO write the function chuckle nuts
	.documentation = "sets the shield vitality of the specified object (between 0 and 1).  This does the wrong thing.  Don't use it.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_shield_normalized_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shield_normalized",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730550, // $TODO write the function chuckle nuts
	.documentation = "sets the shield vitality of the specified object (between 0 and 1).\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_shield_stun_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shield_stun",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007309D0, // $TODO write the function chuckle nuts
	.documentation = "set how long the shield will be stunned for, 0 is unstunned\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_set_shield_stun_infinite_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_shield_stun_infinite",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730D70, // $TODO write the function chuckle nuts
	.documentation = "make this objects shield be stunned permanently\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) object_set_permutation_3_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_permutation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731010, // $TODO write the function chuckle nuts
	.documentation = "sets the desired region (use \"\" for all regions) to the permutation with the given name, e.g. (object_set_permutation flood \"right arm\" ~damaged)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_variant_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007315C0, // $TODO write the function chuckle nuts
	.documentation = "sets the specified object to the specified model variant\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) object_set_region_state_3_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_region_state",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731910, // $TODO write the function chuckle nuts
	.documentation = "sets the desired region (use \"\" for all regions) to the model state with the given name, e.g. (object_set_region_state marine head destroyed)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_model_state
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) objects_can_see_object_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "objects_can_see_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731D20, // $TODO write the function chuckle nuts
	.documentation = "returns true if any of the specified units are looking within the specified number of degrees of the object.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) objects_can_see_flag_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "objects_can_see_flag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732250, // $TODO write the function chuckle nuts
	.documentation = "returns true if any of the specified units are looking within the\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_cutscene_flag,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) objects_distance_to_object_2_definition
{
	.return_type = _hs_type_real,
	.name = "objects_distance_to_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732750, // $TODO write the function chuckle nuts
	.documentation = "returns minimum distance from any of the specified objects to the specified destination object. (returns -1 if there are no objects to check)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) objects_distance_to_flag_2_definition
{
	.return_type = _hs_type_real,
	.name = "objects_distance_to_flag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732BF0, // $TODO write the function chuckle nuts
	.documentation = "returns minimum distance from any of the specified objects to the specified flag. (returns -1 if there are no objects, or no flag, to check)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) map_info_0_definition
{
	.return_type = _hs_type_void,
	.name = "map_info",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007330F0, // $TODO write the function chuckle nuts
	.documentation = "prints the map, zone set, active bsps, and current bsp",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) position_predict_3_definition
{
	.return_type = _hs_type_void,
	.name = "position_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733270, // $TODO write the function chuckle nuts
	.documentation = "in: x, y, z position. loads textures/geometry/sounds necessary to present locations that are about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) shader_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "shader_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733760, // $TODO write the function chuckle nuts
	.documentation = "in: shader name. loads textures necessary for a shader.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_shader
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bitmap_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "bitmap_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733AB0, // $TODO write the function chuckle nuts
	.documentation = "in: bitmap name. loads all the bitmaps in that bitmap group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_bitmap
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) script_recompile_0_definition
{
	.return_type = _hs_type_void,
	.name = "script_recompile",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733E70, // $TODO write the function chuckle nuts
	.documentation = "recompiles scripts.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) script_doc_0_definition
{
	.return_type = _hs_type_void,
	.name = "script_doc",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734110, // $TODO write the function chuckle nuts
	.documentation = "saves a file called hs_doc.txt with parameters for all script commands.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) help_1_definition
{
	.return_type = _hs_type_void,
	.name = "help",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007342D0, // $TODO write the function chuckle nuts
	.documentation = "prints a description of the named function.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_engine_objects_0_definition
{
	.return_type = _hs_type_object_list,
	.name = "game_engine_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007344D0, // $TODO write the function chuckle nuts
	.documentation = "returns a list of the special game engine objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) random_range_2_definition
{
	.return_type = _hs_type_short_integer,
	.name = "random_range",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007348C0, // $TODO write the function chuckle nuts
	.documentation = "returns a random value in the range [lower bound, upper bound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) real_random_range_2_definition
{
	.return_type = _hs_type_real,
	.name = "real_random_range",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734DD0, // $TODO write the function chuckle nuts
	.documentation = "returns a random value in the range [lower bound, upper bound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) physics_constants_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "physics_constants_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735410, // $TODO write the function chuckle nuts
	.documentation = "resets all physics constants to earthly values\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) physics_set_gravity_1_definition
{
	.return_type = _hs_type_void,
	.name = "physics_set_gravity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735620, // $TODO write the function chuckle nuts
	.documentation = "set global gravity acceleration relative to halo standard gravity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) physics_set_velocity_frame_3_definition
{
	.return_type = _hs_type_void,
	.name = "physics_set_velocity_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735890, // $TODO write the function chuckle nuts
	.documentation = "sets a local frame of motion for updating physics of things that wish to respect it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) physics_disable_character_ground_adhesion_forces_1_definition
{
	.return_type = _hs_type_void,
	.name = "physics_disable_character_ground_adhesion_forces",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735E70, // $TODO write the function chuckle nuts
	.documentation = "turn off ground adhesion forces so you can play tricks with gravity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) havok_debug_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "havok_debug_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736140, // $TODO write the function chuckle nuts
	.documentation = "start up the havok visual debugger\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) havok_dump_world_2_definition
{
	.return_type = _hs_type_void,
	.name = "havok_dump_world",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736460, // $TODO write the function chuckle nuts
	.documentation = "dump the state of the havok world, with our without a movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) havok_dump_world_close_movie_0_definition
{
	.return_type = _hs_type_void,
	.name = "havok_dump_world_close_movie",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736680, // $TODO write the function chuckle nuts
	.documentation = "end the capture of a havok dump movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) havok_profile_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "havok_profile_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736850, // $TODO write the function chuckle nuts
	.documentation = "start profiling havok\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) havok_profile_end_0_definition
{
	.return_type = _hs_type_void,
	.name = "havok_profile_end",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736A80, // $TODO write the function chuckle nuts
	.documentation = "stop profiling havok, and save the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) havok_profile_range_2_definition
{
	.return_type = _hs_type_void,
	.name = "havok_profile_range",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736D60, // $TODO write the function chuckle nuts
	.documentation = "captures timers and a movie of a specific tick range\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) havok_reset_allocated_state_0_definition
{
	.return_type = _hs_type_void,
	.name = "havok_reset_allocated_state",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736F90, // $TODO write the function chuckle nuts
	.documentation = "strips down and builds back up all allocated havok memory\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) breakable_surfaces_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "breakable_surfaces_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737170, // $TODO write the function chuckle nuts
	.documentation = "enables or disables breakability of all breakable surfaces on level\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) breakable_surfaces_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "breakable_surfaces_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737540, // $TODO write the function chuckle nuts
	.documentation = "restores all breakable surfaces\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) recording_play_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "recording_play",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007377C0, // $TODO write the function chuckle nuts
	.documentation = "make the specified unit run the specified cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_cutscene_recording
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) recording_play_and_delete_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "recording_play_and_delete",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737BE0, // $TODO write the function chuckle nuts
	.documentation = "make the specified unit run the specified cutscene recording, deletes the unit when the animation finishes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_cutscene_recording
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) recording_play_and_hover_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "recording_play_and_hover",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738180, // $TODO write the function chuckle nuts
	.documentation = "make the specified vehicle run the specified cutscene recording, hovers the vehicle when the animation finishes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_cutscene_recording
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) recording_kill_1_definition
{
	.return_type = _hs_type_void,
	.name = "recording_kill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007385F0, // $TODO write the function chuckle nuts
	.documentation = "kill the specified unit's cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) recording_time_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "recording_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738910, // $TODO write the function chuckle nuts
	.documentation = "return the time remaining in the specified unit's cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_lights_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "render_lights",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738DC0, // $TODO write the function chuckle nuts
	.documentation = "enables/disables dynamic lights\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) print_light_state_0_definition
{
	.return_type = _hs_type_void,
	.name = "print_light_state",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739250, // $TODO write the function chuckle nuts
	.documentation = "outputs text light state\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) render_lights_enable_cinematic_shadow_4_definition
{
	.return_type = _hs_type_void,
	.name = "render_lights_enable_cinematic_shadow",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739550, // $TODO write the function chuckle nuts
	.documentation = "enable/disable the specified unit to receive cinematic shadows where the shadow is focused about a radius around a marker name\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) texture_camera_set_object_marker_3_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_object_marker",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739970, // $TODO write the function chuckle nuts
	.documentation = "sets the render texture camera to a given object marker",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) texture_camera_set_position_3_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_position",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739DF0, // $TODO write the function chuckle nuts
	.documentation = "sets the render texture camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) texture_camera_set_target_3_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A220, // $TODO write the function chuckle nuts
	.documentation = "sets the render texture camera target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) texture_camera_attach_to_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_attach_to_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A730, // $TODO write the function chuckle nuts
	.documentation = "attaches the render texture camera to a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) texture_camera_target_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_target_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AA40, // $TODO write the function chuckle nuts
	.documentation = "targets the render texture camera to view a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) texture_camera_position_world_offset_3_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_position_world_offset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AF10, // $TODO write the function chuckle nuts
	.documentation = "adds a worldspace offset to the current texture camera position",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_camera_on_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B260, // $TODO write the function chuckle nuts
	.documentation = "turns on the render texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_camera_bink_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_bink",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)NULL, // $TODO write the function chuckle nuts
	.documentation = "turns on the render texture camera and renders a bink to it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_camera_off_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_off",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B430, // $TODO write the function chuckle nuts
	.documentation = "turns off the render texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_set_aspect_ratio_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_aspect_ratio",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B760, // $TODO write the function chuckle nuts
	.documentation = "sets the texture camera aspet ratio\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) texture_camera_set_resolution_2_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_resolution",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BA40, // $TODO write the function chuckle nuts
	.documentation = "sets the texture camera render resolution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_render_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_render_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BF10, // $TODO write the function chuckle nuts
	.documentation = "switches the texture camera render mode",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_unknownBC_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_unknownBC",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C270, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_set_fov_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_fov",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C530, // $TODO write the function chuckle nuts
	.documentation = "sets the field of view on the texture camera, in degrees",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_set_fov_frame_target_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_set_fov_frame_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C9E0, // $TODO write the function chuckle nuts
	.documentation = "zooms the field of view to frame the target, with target zoom factor to zoom closer",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) texture_camera_enable_dynamic_lights_1_definition
{
	.return_type = _hs_type_void,
	.name = "texture_camera_enable_dynamic_lights",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CCE0, // $TODO write the function chuckle nuts
	.documentation = "toggle rendering of dynamic lights in the texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_camera_on_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D130, // $TODO write the function chuckle nuts
	.documentation = "turns on the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_camera_off_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_off",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D410, // $TODO write the function chuckle nuts
	.documentation = "turns off the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) hud_camera_set_position_4_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_set_position",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D740, // $TODO write the function chuckle nuts
	.documentation = "sets the render texture camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) hud_camera_set_target_4_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_set_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DC10, // $TODO write the function chuckle nuts
	.documentation = "sets the render texture camera target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) hud_camera_attach_to_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_attach_to_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E140, // $TODO write the function chuckle nuts
	.documentation = "attaches the render texture camera to a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) hud_camera_target_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_target_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E560, // $TODO write the function chuckle nuts
	.documentation = "targets the render texture camera to view a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) hud_camera_structure_2_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_structure",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E9D0, // $TODO write the function chuckle nuts
	.documentation = "toggle rendering of structure in the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) hud_camera_highlight_object_6_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_highlight_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EDA0, // $TODO write the function chuckle nuts
	.documentation = "highlights an object in the given color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_object,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_camera_clear_objects_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_clear_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F380, // $TODO write the function chuckle nuts
	.documentation = "clears all objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) hud_camera_spin_around_2_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_spin_around",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F610, // $TODO write the function chuckle nuts
	.documentation = "makes the hud camera spin around the given object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_camera_from_player_view_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_from_player_view",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FA60, // $TODO write the function chuckle nuts
	.documentation = "aligns the hud camera to the player camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) hud_camera_window_5_definition
{
	.return_type = _hs_type_void,
	.name = "hud_camera_window",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FD00, // $TODO write the function chuckle nuts
	.documentation = "sets the render window of the hud camera (0...1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_texture_camera_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_texture_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740300, // $TODO write the function chuckle nuts
	.documentation = "toggles displaying the texture camera in the corner of the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_structure_cluster_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_structure_cluster",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007405B0, // $TODO write the function chuckle nuts
	.documentation = "specify a structure and cluster to debug\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_cluster_fog_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_cluster_fog",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740800, // $TODO write the function chuckle nuts
	.documentation = "enables cluster fog debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_fog_plane_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_fog_plane",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740A30, // $TODO write the function chuckle nuts
	.documentation = "enables fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_fog_plane_infinite_extent_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_fog_plane_infinite_extent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740BE0, // $TODO write the function chuckle nuts
	.documentation = "enables fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_fog_zone_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_fog_zone",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740E00, // $TODO write the function chuckle nuts
	.documentation = "enabled fog zone debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_fog_zone_floodfill_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_fog_zone_floodfill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007411C0, // $TODO write the function chuckle nuts
	.documentation = "enables fog zone debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_all_fog_planes_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_all_fog_planes",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741330, // $TODO write the function chuckle nuts
	.documentation = "enables fog plane debugging (all)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_all_cluster_errors_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_all_cluster_errors",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007415F0, // $TODO write the function chuckle nuts
	.documentation = "enables cluster error debugging (all)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) render_debug_structure_line_opacity_4_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_line_opacity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741810, // $TODO write the function chuckle nuts
	.documentation = "sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) render_debug_structure_text_opacity_4_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_text_opacity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741A90, // $TODO write the function chuckle nuts
	.documentation = "sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) render_debug_structure_opacity_4_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_opacity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741BF0, // $TODO write the function chuckle nuts
	.documentation = "sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_structure_non_occluded_fog_planes_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_non_occluded_fog_planes",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741E30, // $TODO write the function chuckle nuts
	.documentation = "controls non-occluded fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_structure_lightmaps_use_pervertex_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_lightmaps_use_pervertex",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742180, // $TODO write the function chuckle nuts
	.documentation = "sets the lightmap preference to per-vertex, ignores per-pixel if available\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_structure_lightmaps_use_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_lightmaps_use_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742390, // $TODO write the function chuckle nuts
	.documentation = "resets the lightmap preference setting and updates the internal lightmap bsp data if necessary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_structure_lightmaps_sample_enable_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_lightmaps_sample_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007425F0, // $TODO write the function chuckle nuts
	.documentation = "enables visualization of match samples if available\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_structure_lightmaps_sample_disable_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_structure_lightmaps_sample_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742880, // $TODO write the function chuckle nuts
	.documentation = "disables visualization of match samples\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_query_object_bitmaps_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_query_object_bitmaps",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742AF0, // $TODO write the function chuckle nuts
	.documentation = "substring of object definition name to spew out it's debug usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) render_debug_query_bsp_resources_2_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_query_bsp_resources",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742DB0, // $TODO write the function chuckle nuts
	.documentation = "BSP index and whether to spit out all bitmaps used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_query_all_object_resources_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_query_all_object_resources",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742F10, // $TODO write the function chuckle nuts
	.documentation = "Outputs text of texture and geometry memory usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_query_d3d_resources_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_query_d3d_resources",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743170, // $TODO write the function chuckle nuts
	.documentation = "Outputs text of d3d resource usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_debug_text_using_simple_font_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_text_using_simple_font",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743520, // $TODO write the function chuckle nuts
	.documentation = "toggles use of the much faster simple_font system for debug text rendering\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_postprocess_color_tweaking_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_postprocess_color_tweaking_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743660, // $TODO write the function chuckle nuts
	.documentation = "resets hue saturation filters to default\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) scenery_animation_start_3_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743900, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) scenery_animation_start_loop_3_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743E50, // $TODO write the function chuckle nuts
	.documentation = "starts a custom looping animation playing on a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenery_animation_start_relative_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_relative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744240, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a piece of scenery relative to a parent object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenery_animation_start_relative_loop_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_relative_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744740, // $TODO write the function chuckle nuts
	.documentation = "starts a custom looping animation playing on a piece of scenery relative to a parent object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenery_animation_start_at_frame_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_at_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744B10, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a piece of scenery at a specific frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) scenery_animation_start_relative_at_frame_5_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_relative_at_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007450B0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a piece of scenery relative to a specific cutscene flag at a specific frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) scenery_animation_idle_1_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_idle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007454F0, // $TODO write the function chuckle nuts
	.documentation = "starts the idle animation (if any) for a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_scenery
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) scenery_get_animation_time_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "scenery_get_animation_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745A30, // $TODO write the function chuckle nuts
	.documentation = "returns the number of ticks remaining in a custom animation (or zero, if the animation is over).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_scenery
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_can_blink_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_can_blink",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745D40, // $TODO write the function chuckle nuts
	.documentation = "allows a unit to blink or not (units never blink when they are dead)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_set_active_camo_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_active_camo",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007461B0, // $TODO write the function chuckle nuts
	.documentation = "enable or disable active camo for the given unit over the specified number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_open_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_open",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746560, // $TODO write the function chuckle nuts
	.documentation = "opens the hatches on the given unit\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_close_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_close",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746850, // $TODO write the function chuckle nuts
	.documentation = "closes the hatches on a given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_kill_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_kill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746C80, // $TODO write the function chuckle nuts
	.documentation = "kills a given unit, no saving throw\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_kill_silent_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_kill_silent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746F90, // $TODO write the function chuckle nuts
	.documentation = "kills a given unit silently (doesn't make them play their normal death animation or sound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_is_emitting_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_is_emitting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007472F0, // $TODO write the function chuckle nuts
	.documentation = "returns whether or not the given unit is current emitting an ai\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_custom_animation_time_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "unit_get_custom_animation_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007477A0, // $TODO write the function chuckle nuts
	.documentation = "returns the number of ticks remaining in a unit's custom animation (or zero, if the animation is over).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_stop_custom_animation_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_stop_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747BF0, // $TODO write the function chuckle nuts
	.documentation = "stops the custom animation running on the given unit.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) custom_animation_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747F70, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) custom_animation_loop_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "custom_animation_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007484E0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) custom_animation_relative_5_definition
{
	.return_type = _hs_type_boolean,
	.name = "custom_animation_relative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F6B0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation relative to some other object (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) custom_animation_relative_loop_5_definition
{
	.return_type = _hs_type_boolean,
	.name = "custom_animation_relative_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FD60, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation relative to some other object (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) custom_animation_list_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "custom_animation_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730410, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit list (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) unit_custom_animation_at_frame_5_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_custom_animation_at_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730980, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit at a specific frame index(interpolates into animation if next to last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) unit_custom_animation_relative_at_frame_6_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_custom_animation_relative_at_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730F50, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit relative to a specific cutscene flag at a specific frame index(interpolates into animation if next to last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_is_playing_custom_animation_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_is_playing_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731620, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the given unit is still playing a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_set_custom_animations_hold_on_last_frame_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_custom_animations_hold_on_last_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007319A0, // $TODO write the function chuckle nuts
	.documentation = "changes the default behavior for custom animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_set_custom_animations_prevent_lipsync_head_movement_1_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_custom_animations_prevent_lipsync_head_movement",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731CF0, // $TODO write the function chuckle nuts
	.documentation = "when true, prevents lipsync from bobbing the head during custom animations. default is true.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) preferred_animation_list_add_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "preferred_animation_list_add",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007320A0, // $TODO write the function chuckle nuts
	.documentation = "adds an animation into the 'preferred list' of animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) preferred_animation_list_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "preferred_animation_list_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007325C0, // $TODO write the function chuckle nuts
	.documentation = "clears the 'preferred list' of animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_actively_controlled_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_actively_controlled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007327E0, // $TODO write the function chuckle nuts
	.documentation = "sets unit's actively controlled flag\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_team_index_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "unit_get_team_index",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732AD0, // $TODO write the function chuckle nuts
	.documentation = "returns the team index of the unit, returns -1 if the unit does not have a team\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_aim_without_turning_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_aim_without_turning",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732FA0, // $TODO write the function chuckle nuts
	.documentation = "allows a unit to aim in place without turning\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_enterable_by_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_enterable_by_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733330, // $TODO write the function chuckle nuts
	.documentation = "can be used to prevent the player from entering a vehicle\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_set_seat_enterable_by_player_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_seat_enterable_by_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007337B0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_unit_seat_mapping,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_enterable_by_player_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_get_enterable_by_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733AE0, // $TODO write the function chuckle nuts
	.documentation = "returns true if a player may enter the vehicle\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_only_takes_damage_from_players_team_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_only_takes_damage_from_players_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734060, // $TODO write the function chuckle nuts
	.documentation = "used for the tartarus boss fight\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_enter_vehicle_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_enter_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734400, // $TODO write the function chuckle nuts
	.documentation = "puts the specified unit in the specified vehicle (in the named seat)\r\nNETWORK SAFE: Yes (w/o animation on clients)",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_vehicle,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_falling_damage_disable_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_falling_damage_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734880, // $TODO write the function chuckle nuts
	.documentation = "disables falling damage on unit",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_in_vehicle_type_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_in_vehicle_type",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734C90, // $TODO write the function chuckle nuts
	.documentation = "returns true if the unit is in a vehicle that matches the specified vehicle type",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_get_turret_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "object_get_turret_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735140, // $TODO write the function chuckle nuts
	.documentation = "returns the number of turrets attached to the given object\r\nNETWORK SAFE: Unknown, assumed UNSAFE",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_get_turret_2_definition
{
	.return_type = _hs_type_vehicle,
	.name = "object_get_turret",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007355E0, // $TODO write the function chuckle nuts
	.documentation = "returns the nth turret attached to the given object\r\nNETWORK SAFE: Unknown, assumed UNSAFE",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_board_vehicle_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_board_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735AE0, // $TODO write the function chuckle nuts
	.documentation = "Causes the given unit to attempt to board the named seat\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unit_set_emotion_4_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_emotion",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735F10, // $TODO write the function chuckle nuts
	.documentation = "sets a unit's facial expression (-1 is none, other values depend on unit)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unit_set_emotion_by_name_4_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_emotion_by_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736390, // $TODO write the function chuckle nuts
	.documentation = "sets a unit's facial expression by name with weight and transition time\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_enable_eye_tracking_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_enable_eye_tracking",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736810, // $TODO write the function chuckle nuts
	.documentation = "enable/disable eye aiming on a unit\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_integrated_flashlight_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_integrated_flashlight",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736B60, // $TODO write the function chuckle nuts
	.documentation = "sets a unit's flashlight on or off\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_voice_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_voice",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737040, // $TODO write the function chuckle nuts
	.documentation = "set the dialogue for the given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_enable_vision_mode_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_enable_vision_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007373A0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable vision mode for a given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_in_vehicle_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_in_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737780, // $TODO write the function chuckle nuts
	.documentation = "returns true if the given unit is seated on a parent unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vehicle_test_seat_list_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "vehicle_test_seat_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737B90, // $TODO write the function chuckle nuts
	.documentation = "tests whether the named seat has an object in the object list (use \"\" to test all seats for any unit in the list)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping,
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vehicle_test_seat_unit_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "vehicle_test_seat_unit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738130, // $TODO write the function chuckle nuts
	.documentation = "tests whether the named seat has a specified unit in it (use  to test all seats for this unit)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping,
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vehicle_test_seat_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "vehicle_test_seat",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007386A0, // $TODO write the function chuckle nuts
	.documentation = "tests whether the named seat has any unit in it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_prefer_tight_camera_track_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_prefer_tight_camera_track",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738B40, // $TODO write the function chuckle nuts
	.documentation = "sets the unit to prefer a tight camera track\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_exit_vehicle_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_exit_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738F50, // $TODO write the function chuckle nuts
	.documentation = "makes a unit exit its vehicle (0 = normal exit to airborne, 1 = ejection, 2 = ejection + death, 3 = exit to ground)\r\nNETWORK SAFE: Yes (w/o client animation)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_exit_vehicle_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_exit_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007392B0, // $TODO write the function chuckle nuts
	.documentation = "makes a unit exit its vehicle (0 = normal exit to airborne, 1 = ejection, 2 = ejection + death, 3 = exit to ground)\r\nNETWORK SAFE: Yes (w/o client animation)",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_set_maximum_vitality_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_maximum_vitality",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739690, // $TODO write the function chuckle nuts
	.documentation = "sets a unit's maximum body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) units_set_maximum_vitality_3_definition
{
	.return_type = _hs_type_void,
	.name = "units_set_maximum_vitality",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739AA0, // $TODO write the function chuckle nuts
	.documentation = "sets a group of units' maximum body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_set_current_vitality_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_current_vitality",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739F80, // $TODO write the function chuckle nuts
	.documentation = "sets a unit's current body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) units_set_current_vitality_3_definition
{
	.return_type = _hs_type_void,
	.name = "units_set_current_vitality",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A490, // $TODO write the function chuckle nuts
	.documentation = "sets a group of units' current body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vehicle_load_magic_3_definition
{
	.return_type = _hs_type_short_integer,
	.name = "vehicle_load_magic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A7D0, // $TODO write the function chuckle nuts
	.documentation = "makes a list of units (named or by encounter) magically get into a vehicle, in the substring-specified seats (e.g. CD-passenger... empty string matches all seats)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_unit_seat_mapping,
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vehicle_unload_2_definition
{
	.return_type = _hs_type_short_integer,
	.name = "vehicle_unload",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AD80, // $TODO write the function chuckle nuts
	.documentation = "makes units get out of an object from the substring-specified seats (e.g. CD-passenger... empty string matches all seats)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_set_animation_mode_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_animation_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B1D0, // $TODO write the function chuckle nuts
	.documentation = "this unit will assume the named animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) magic_melee_attack_0_definition
{
	.return_type = _hs_type_void,
	.name = "magic_melee_attack",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B470, // $TODO write the function chuckle nuts
	.documentation = "causes player's unit to start a melee attack\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_riders_1_definition
{
	.return_type = _hs_type_object_list,
	.name = "vehicle_riders",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B8A0, // $TODO write the function chuckle nuts
	.documentation = "returns a list of all riders in a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_driver_1_definition
{
	.return_type = _hs_type_unit,
	.name = "vehicle_driver",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BC00, // $TODO write the function chuckle nuts
	.documentation = "returns the driver of a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_gunner_1_definition
{
	.return_type = _hs_type_unit,
	.name = "vehicle_gunner",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C090, // $TODO write the function chuckle nuts
	.documentation = "returns the gunner of a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_health_1_definition
{
	.return_type = _hs_type_real,
	.name = "unit_get_health",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C4B0, // $TODO write the function chuckle nuts
	.documentation = "returns the health [0,1] of the unit, returns -1 if the unit does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_shield_1_definition
{
	.return_type = _hs_type_real,
	.name = "unit_get_shield",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C9A0, // $TODO write the function chuckle nuts
	.documentation = "returns the shield [0,1] of the unit, returns -1 if the unit does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_total_grenade_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "unit_get_total_grenade_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CD30, // $TODO write the function chuckle nuts
	.documentation = "returns the total number of grenades for the given unit, 0 if it does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_has_weapon_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_has_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D1B0, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the <unit> has <object> as a weapon, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_has_weapon_readied_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_has_weapon_readied",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D660, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the <unit> has <object> as the primary weapon, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_has_any_equipment_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_has_any_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DB90, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the <unit> has any equipment, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_has_equipment_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_has_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DF60, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the <unit> has equipment <object> , FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_lower_weapon_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_lower_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E4A0, // $TODO write the function chuckle nuts
	.documentation = "lower the units weapon over x ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_raise_weapon_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_raise_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E8A0, // $TODO write the function chuckle nuts
	.documentation = "raises the units weapon over x ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_drop_support_weapon_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_drop_support_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ED00, // $TODO write the function chuckle nuts
	.documentation = "forces the given unit to drop its support weapon, if it is holding one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_spew_action_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_spew_action",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EFD0, // $TODO write the function chuckle nuts
	.documentation = "runs the spew unit action on the specified unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_force_reload_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_force_reload",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F470, // $TODO write the function chuckle nuts
	.documentation = "Force the give unit to reload his weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) animation_stats_dump_0_definition
{
	.return_type = _hs_type_void,
	.name = "animation_stats_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F700, // $TODO write the function chuckle nuts
	.documentation = "Dump usage information for the animations in the game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_animation_forced_seat_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_animation_forced_seat",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F990, // $TODO write the function chuckle nuts
	.documentation = "all units controlled by the player will assume the given seat name (valid values are 'asleep', 'alert', 'stand', 'crouch' and 'flee')\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_doesnt_drop_items_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_doesnt_drop_items",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FC00, // $TODO write the function chuckle nuts
	.documentation = "prevents any of the given units from dropping weapons or grenades when they die\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_list
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_impervious_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_impervious",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FF60, // $TODO write the function chuckle nuts
	.documentation = "prevents any of the given units from being knocked around or playing ping animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_suspended_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_suspended",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007402C0, // $TODO write the function chuckle nuts
	.documentation = "stops gravity from working on the given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unit_add_equipment_4_definition
{
	.return_type = _hs_type_void,
	.name = "unit_add_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740720, // $TODO write the function chuckle nuts
	.documentation = "adds/resets the unit's health, shield, and inventory (weapons and grenades) to the named profile. resets if third parameter is true, adds if false. weapons will be marked as garbage if fourth parameter is true (for respawning equipment).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_starting_profile,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) weapon_hold_trigger_3_definition
{
	.return_type = _hs_type_void,
	.name = "weapon_hold_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740B50, // $TODO write the function chuckle nuts
	.documentation = "turns the trigger for a weapon  on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_weapon,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) weapon_enable_warthog_chaingun_light_1_definition
{
	.return_type = _hs_type_void,
	.name = "weapon_enable_warthog_chaingun_light",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007410E0, // $TODO write the function chuckle nuts
	.documentation = "turns the trigger for a weapon  on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_set_never_appears_locked_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_set_never_appears_locked",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007412F0, // $TODO write the function chuckle nuts
	.documentation = "changes a machine's never_appears_locked flag, but only if paul is a bastard\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_set_power_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_set_power",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741760, // $TODO write the function chuckle nuts
	.documentation = "immediately sets the power of a named device to the given value\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) device_get_power_1_definition
{
	.return_type = _hs_type_real,
	.name = "device_get_power",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741AF0, // $TODO write the function chuckle nuts
	.documentation = "gets the current power of a named device\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_device
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_set_position_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "device_set_position",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741EB0, // $TODO write the function chuckle nuts
	.documentation = "set the desired position of the given device (used for devices without explicit device groups)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) device_get_position_1_definition
{
	.return_type = _hs_type_real,
	.name = "device_get_position",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742490, // $TODO write the function chuckle nuts
	.documentation = "gets the current position of the given device (used for devices without explicit device groups)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_device
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_set_position_immediate_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_set_position_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742840, // $TODO write the function chuckle nuts
	.documentation = "instantaneously changes the position of the given device (used for devices without explicit device groups\r\nNETWORK SAFE: No (only desired works, need to add immediate support)",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) device_group_get_1_definition
{
	.return_type = _hs_type_real,
	.name = "device_group_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742D20, // $TODO write the function chuckle nuts
	.documentation = "returns the desired value of the specified device group.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_device_group
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) device_group_set_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "device_group_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743000, // $TODO write the function chuckle nuts
	.documentation = "changes the desired value of the specified device group.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_device_group,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_group_set_immediate_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_group_set_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007435D0, // $TODO write the function chuckle nuts
	.documentation = "instantaneously changes the value of the specified device group.\r\nNETWORK SAFE: No (only desired works, need to add immediate support)",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device_group,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_one_sided_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_one_sided_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007439A0, // $TODO write the function chuckle nuts
	.documentation = "TRUE makes the given device one-sided (only able to be opened from one direction), FALSE makes it two-sided\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_ignore_player_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_ignore_player_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743ED0, // $TODO write the function chuckle nuts
	.documentation = "TRUE makes the given device ignore the player (won't open for players), FALSE makes it work for the player\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_operates_automatically_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_operates_automatically_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744200, // $TODO write the function chuckle nuts
	.documentation = "TRUE makes the given device open automatically when any biped is nearby, FALSE makes it not\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_closes_automatically_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_closes_automatically_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744680, // $TODO write the function chuckle nuts
	.documentation = "TRUE makes the given device close automatically after it has opened, FALSE makes it not\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_group_change_only_once_more_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "device_group_change_only_once_more_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744A70, // $TODO write the function chuckle nuts
	.documentation = "TRUE allows a device to change states only once\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device_group,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) device_set_position_track_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "device_set_position_track",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744E40, // $TODO write the function chuckle nuts
	.documentation = "set the desired position track animation to use (optional interpolation time onto track)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) device_set_overlay_track_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "device_set_overlay_track",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745390, // $TODO write the function chuckle nuts
	.documentation = "set the desired overlay animation to use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) device_animate_position_6_definition
{
	.return_type = _hs_type_void,
	.name = "device_animate_position",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745740, // $TODO write the function chuckle nuts
	.documentation = "animate the position over time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) device_animate_overlay_5_definition
{
	.return_type = _hs_type_void,
	.name = "device_animate_overlay",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745CE0, // $TODO write the function chuckle nuts
	.documentation = "animate the overlay over time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cheat_all_powerups_0_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_all_powerups",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746280, // $TODO write the function chuckle nuts
	.documentation = "drops all powerups near player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cheat_all_weapons_0_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_all_weapons",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007464B0, // $TODO write the function chuckle nuts
	.documentation = "drops all weapons near player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cheat_all_vehicles_0_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_all_vehicles",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746720, // $TODO write the function chuckle nuts
	.documentation = "drops all vehicles on player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cheat_teleport_to_camera_0_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_teleport_to_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746960, // $TODO write the function chuckle nuts
	.documentation = "teleports player to camera location\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cheat_active_camouflage_1_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_active_camouflage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746C00, // $TODO write the function chuckle nuts
	.documentation = "gives the player active camouflage\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cheat_active_camouflage_by_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "cheat_active_camouflage_by_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746E00, // $TODO write the function chuckle nuts
	.documentation = "gives a specific player active camouflage\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cheats_load_0_definition
{
	.return_type = _hs_type_void,
	.name = "cheats_load",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007470E0, // $TODO write the function chuckle nuts
	.documentation = "reloads the cheats.txt file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) drop_safe_1_definition
{
	.return_type = _hs_type_void,
	.name = "drop_safe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747230, // $TODO write the function chuckle nuts
	.documentation = "drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle\r\nNETWORK SAFE: Yes, for objects",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) drop_1_definition
{
	.return_type = _hs_type_void,
	.name = "drop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007474E0, // $TODO write the function chuckle nuts
	.documentation = "drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle\r\nNETWORK SAFE: Yes, for objects",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) drop_variant_2_definition
{
	.return_type = _hs_type_void,
	.name = "drop_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747700, // $TODO write the function chuckle nuts
	.documentation = "drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle using the specified variant name\r\nNETWORK SAFE: Yes, for objects",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747900, // $TODO write the function chuckle nuts
	.documentation = "turns all AI on or off.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_enabled_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747DB0, // $TODO write the function chuckle nuts
	.documentation = "returns whether AI is turned on or off.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_grenades_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747FC0, // $TODO write the function chuckle nuts
	.documentation = "turns grenade inventory on or off.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_dialogue_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_dialogue_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748350, // $TODO write the function chuckle nuts
	.documentation = "turn combat dialogue on/off\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_player_dialogue_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_player_dialogue_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748750, // $TODO write the function chuckle nuts
	.documentation = "turn player combat dialogue on/off\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_infection_suppress_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_infection_suppress",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F790, // $TODO write the function chuckle nuts
	.documentation = "disable infection for this number of ticks\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_fast_and_dumb_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_fast_and_dumb",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FBC0, // $TODO write the function chuckle nuts
	.documentation = "turn on/off perception performance hacks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_dialogue_log_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_dialogue_log_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FEB0, // $TODO write the function chuckle nuts
	.documentation = "reset the dialogue log\r\nNETWORK SAFE: Yes (local)",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_dialogue_log_dump_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_dialogue_log_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007301E0, // $TODO write the function chuckle nuts
	.documentation = "dump a file of the given name with csv statistics on vocalizations\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_get_object_1_definition
{
	.return_type = _hs_type_object,
	.name = "ai_get_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730490, // $TODO write the function chuckle nuts
	.documentation = "returns the unit/object corresponding to the given actor\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_get_unit_1_definition
{
	.return_type = _hs_type_unit,
	.name = "ai_get_unit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007308D0, // $TODO write the function chuckle nuts
	.documentation = "returns the unit/object corresponding to the given actor\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_get_squad_1_definition
{
	.return_type = _hs_type_ai,
	.name = "ai_get_squad",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730D00, // $TODO write the function chuckle nuts
	.documentation = "returns the squad of the given actor. Invalid if the given ai is NOT an actor-type (i.e. is itself a squad or squad group or task\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_get_turret_ai_2_definition
{
	.return_type = _hs_type_ai,
	.name = "ai_get_turret_ai",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731080, // $TODO write the function chuckle nuts
	.documentation = "returns an the ai controlling the Nth turret attached to the given ai (or its vehicle)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) ai_random_smart_point_4_definition
{
	.return_type = _hs_type_point_ref,
	.name = "ai_random_smart_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731680, // $TODO write the function chuckle nuts
	.documentation = "returns a point in the given point set that a player is probably going to be able to see. Args are [point set], [min distance], [max distance], [angle-from-forward], all relative to the player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_nearest_point_2_definition
{
	.return_type = _hs_type_point_ref,
	.name = "ai_nearest_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731B80, // $TODO write the function chuckle nuts
	.documentation = "returns the nearest point in the given point set to the given object\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_get_point_count_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "ai_get_point_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732010, // $TODO write the function chuckle nuts
	.documentation = "returns the number of points in the given point set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_point_set_get_point_2_definition
{
	.return_type = _hs_type_point_ref,
	.name = "ai_point_set_get_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732330, // $TODO write the function chuckle nuts
	.documentation = "returns Nth point in the point set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_place_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_place",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007328A0, // $TODO write the function chuckle nuts
	.documentation = "places the given number of members of the specified squad.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_place_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_place",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732C50, // $TODO write the function chuckle nuts
	.documentation = "places the given number of members of the specified squad.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_place_in_vehicle_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_place_in_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733020, // $TODO write the function chuckle nuts
	.documentation = "places the specified squad (1st arg) on the map in the vehicles belonging to the specified vehicle squad (2nd arg).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_cannot_die_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_cannot_die",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007333A0, // $TODO write the function chuckle nuts
	.documentation = "AI cannot die from damage (as opposed to by scripting)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vitality_pinned_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_vitality_pinned",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007337F0, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai's units are ALL vitality pinned (see object_vitality_pinned)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_index_from_spawn_formation_2_definition
{
	.return_type = _hs_type_ai,
	.name = "ai_index_from_spawn_formation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733B60, // $TODO write the function chuckle nuts
	.documentation = "Create an AI index from the given squad and spawn formation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_resurrect_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_resurrect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007341A0, // $TODO write the function chuckle nuts
	.documentation = "Resurrect the specified mission critical AI character.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_kill_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_kill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734440, // $TODO write the function chuckle nuts
	.documentation = "instantly kills the specified encounter and/or squad.\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_kill_silent_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_kill_silent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734850, // $TODO write the function chuckle nuts
	.documentation = "instantly and silently (no animation or sound played) kills the specified encounter and/or squad.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_erase_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_erase",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734C20, // $TODO write the function chuckle nuts
	.documentation = "erases the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_erase_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_erase_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734FD0, // $TODO write the function chuckle nuts
	.documentation = "erases all AI.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_disposable_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_disposable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007351C0, // $TODO write the function chuckle nuts
	.documentation = "enables or disables automatic garbage collection for actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_select_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_select",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007355B0, // $TODO write the function chuckle nuts
	.documentation = "selects the specified squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_deselect_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_deselect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735870, // $TODO write the function chuckle nuts
	.documentation = "clears the selected encounter.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_deaf_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_deaf",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735C70, // $TODO write the function chuckle nuts
	.documentation = "enables or disables hearing for actors in the specified encounter.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_blind_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_blind",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735FD0, // $TODO write the function chuckle nuts
	.documentation = "enables or disables sight for actors in the specified encounter.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_weapon_up_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_weapon_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736420, // $TODO write the function chuckle nuts
	.documentation = "force actor to hold their weapon up, instead of lowering it when idle. this does NOT cause them to draw their weapon if stowed\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_flood_disperse_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_flood_disperse",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736750, // $TODO write the function chuckle nuts
	.documentation = "Disperese a flood encounter, stimulating the various forms to break down into infections and transition to the new objective\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_magically_see_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_magically_see",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736B20, // $TODO write the function chuckle nuts
	.documentation = "Make one squad magically aware of another.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_magically_see_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_magically_see_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736F50, // $TODO write the function chuckle nuts
	.documentation = "Make a squad magically aware of a particular object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_active_camo_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_active_camo",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737340, // $TODO write the function chuckle nuts
	.documentation = "Turn on active camoflage on actor/squad/squad-group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_suppress_combat_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_suppress_combat",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007376A0, // $TODO write the function chuckle nuts
	.documentation = "Turn on/off combat suppression on actor/squad/squad-group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_engineer_explode_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_engineer_explode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737A90, // $TODO write the function chuckle nuts
	.documentation = "Register an explosion stimulus with all specified actors, causing engineers to explode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_grunt_kamikaze_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_grunt_kamikaze",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737FC0, // $TODO write the function chuckle nuts
	.documentation = "Register a kamikaze stimulus with all specified actors, causing grunts to GO WILD\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_migrate_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_migrate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738280, // $TODO write the function chuckle nuts
	.documentation = "makes all or part of an encounter move to another encounter.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allegiance_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_allegiance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x6D3A2E1F, // $TODO write the function chuckle nuts
	.documentation = "creates an allegiance between two teams.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allegiance_remove_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_allegiance_remove",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738A80, // $TODO write the function chuckle nuts
	.documentation = "destroys an allegiance between two teams.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allegiance_break_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_allegiance_break",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738E20, // $TODO write the function chuckle nuts
	.documentation = "fake a betrayal incident between two teams\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_braindead_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_braindead",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739210, // $TODO write the function chuckle nuts
	.documentation = "makes a group of actors braindead, or restores them to life (in their initial state)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_braindead_by_unit_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_braindead_by_unit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739650, // $TODO write the function chuckle nuts
	.documentation = "makes a list of objects braindead, or restores them to life. if you pass in a vehicle index, it makes all actors in that vehicle braindead (including any built-in guns)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_disregard_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_disregard",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739A00, // $TODO write the function chuckle nuts
	.documentation = "if TRUE, forces all actors to completely disregard the specified units, otherwise lets them acknowledge the units again\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_prefer_target_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_prefer_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739E70, // $TODO write the function chuckle nuts
	.documentation = "if TRUE, *ALL* enemies will prefer to attack the specified units. if FALSE, removes the preference.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object_list,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_prefer_target_team_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_prefer_target_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A1E0, // $TODO write the function chuckle nuts
	.documentation = "the given ai will prefer the indicated team.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_prefer_target_ai_3_definition
{
	.return_type = _hs_type_void,
	.name = "ai_prefer_target_ai",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A6C0, // $TODO write the function chuckle nuts
	.documentation = "the given ai (actor, squad, task, etc.) will prefer the given other ai (actor, squad, task, etc.)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_targeting_group_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_targeting_group",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AA00, // $TODO write the function chuckle nuts
	.documentation = "Set the AI to only target other guys of targetin group X (true/false whether the player should also be targeted)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_set_targeting_group_3_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_targeting_group",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AED0, // $TODO write the function chuckle nuts
	.documentation = "Set the AI to only target other guys of targetin group X (true/false whether the player should also be targeted)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_teleport_to_starting_location_if_outside_bsp_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_teleport_to_starting_location_if_outside_bsp",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B230, // $TODO write the function chuckle nuts
	.documentation = "PLEASE USE ai_teleport_to_spawn_point_if_outside_bsp INSTEAD\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_teleport_to_spawn_point_if_outside_bsp_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_teleport_to_spawn_point_if_outside_bsp",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B670, // $TODO write the function chuckle nuts
	.documentation = "teleports a group of actors to the spawn points of their current squad(s) if they are currently outside the world.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_teleport_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_teleport",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B970, // $TODO write the function chuckle nuts
	.documentation = "teleports the given squad to the given point.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_bring_forward_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_bring_forward",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BDA0, // $TODO write the function chuckle nuts
	.documentation = "teleports the given mission critical AI character to the current encounter, if beyond the given range from a player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_migrate_form_3_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_migrate_form",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C140, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_morph_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_morph",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C5C0, // $TODO write the function chuckle nuts
	.documentation = "Cause an ai (or a group of ai) to morph to the given form\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) biped_morph_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "biped_morph",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CB70, // $TODO write the function chuckle nuts
	.documentation = "Cause a biped to morph to its floodified form\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_renew_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_renew",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CFD0, // $TODO write the function chuckle nuts
	.documentation = "refreshes the health and grenade count of a group of actors, so they are as good as new\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_force_active_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_force_active",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D320, // $TODO write the function chuckle nuts
	.documentation = "forces an encounter to remain active (i.e. not freeze in place) even if there are no players nearby\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_force_active_by_unit_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_force_active_by_unit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D700, // $TODO write the function chuckle nuts
	.documentation = "forces a named actor that is NOT in an encounter to remain active (i.e. not freeze in place) even if there are no players nearby\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_playfight_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_playfight",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DB50, // $TODO write the function chuckle nuts
	.documentation = "sets an encounter to be playfighting or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_reconnect_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_reconnect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DED0, // $TODO write the function chuckle nuts
	.documentation = "reconnects all AI information to the current structure bsp (use this after you create encounters or command lists in sapien, or place new firing points or command list points)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_berserk_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_berserk",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E2F0, // $TODO write the function chuckle nuts
	.documentation = "forces a group of actors to start or stop berserking\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_team_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_set_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E520, // $TODO write the function chuckle nuts
	.documentation = "makes an encounter change to a new team\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allow_dormant_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_allow_dormant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E990, // $TODO write the function chuckle nuts
	.documentation = "either enables or disables automatic dormancy for a group of actors\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_is_attacking_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_is_attacking",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ED60, // $TODO write the function chuckle nuts
	.documentation = "returns whether a platoon is in the attacking mode (or if an encounter is specified, returns whether any platoon in that encounter is attacking)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_fighting_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_fighting_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F180, // $TODO write the function chuckle nuts
	.documentation = "return the number of actors that are fighting in a squad or squad_group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_living_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_living_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F5D0, // $TODO write the function chuckle nuts
	.documentation = "return the number of living actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_living_fraction_1_definition
{
	.return_type = _hs_type_real,
	.name = "ai_living_fraction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FA00, // $TODO write the function chuckle nuts
	.documentation = "return the fraction [0-1] of living actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_in_vehicle_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_in_vehicle_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FE50, // $TODO write the function chuckle nuts
	.documentation = "return the number of living actors in the specified encounter and/or squad who are currently in a vehicle.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_body_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_body_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740240, // $TODO write the function chuckle nuts
	.documentation = "return the number of actors who have died in the completiong of the given objective/task.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_strength_1_definition
{
	.return_type = _hs_type_real,
	.name = "ai_strength",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007406E0, // $TODO write the function chuckle nuts
	.documentation = "return the current strength (average body vitality from 0-1) of the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_swarm_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_swarm_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740AE0, // $TODO write the function chuckle nuts
	.documentation = "return the number of swarm actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_nonswarm_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_nonswarm_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740F40, // $TODO write the function chuckle nuts
	.documentation = "return the number of non-swarm actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_actors_1_definition
{
	.return_type = _hs_type_object_list,
	.name = "ai_actors",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741370, // $TODO write the function chuckle nuts
	.documentation = "converts an ai reference to an object list.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allegiance_broken_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_allegiance_broken",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007417C0, // $TODO write the function chuckle nuts
	.documentation = "returns whether two teams have an allegiance that is currently broken by traitorous behavior\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_spawn_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_spawn_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741BB0, // $TODO write the function chuckle nuts
	.documentation = "returns the number of actors spawned in the given squad or squad group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) object_get_ai_1_definition
{
	.return_type = _hs_type_ai,
	.name = "object_get_ai",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742090, // $TODO write the function chuckle nuts
	.documentation = "returns the ai attached to this object, if any\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) ai_rotate_scenario_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_rotate_scenario",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742560, // $TODO write the function chuckle nuts
	.documentation = "rotates ai points and normals (e.g. firing points, spawn points) around the given axis (x,y,z) by the given rotation angle (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_translate_scenario_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_translate_scenario",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742A90, // $TODO write the function chuckle nuts
	.documentation = "translates ai points and objects by the given x y z.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) scenario_duplicate_bsp_objects_2_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_duplicate_bsp_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742F30, // $TODO write the function chuckle nuts
	.documentation = "<input bsp> <output bsp> <filter>: duplicate filtered objects in the first BSP into the second BSP; available filters: all, crates, scenery, bipeds, machines, terminals, controls, arg_devices\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) scenario_duplicate_bsp_objects_3_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_duplicate_bsp_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743490, // $TODO write the function chuckle nuts
	.documentation = "<input bsp> <output bsp> <filter>: duplicate filtered objects in the first BSP into the second BSP; available filters: all, crates, scenery, bipeds, machines, terminals, controls, arg_devices\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenario_rotate_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_rotate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743810, // $TODO write the function chuckle nuts
	.documentation = "rotates scenario objects (specified by the filter) in the specified BSP around the given axis (x,y,z) by the given amount (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenario_reflect_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_reflect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743DB0, // $TODO write the function chuckle nuts
	.documentation = "reflects scenario objects (specified by the filter) in the specified BSP across the given plane (specified by normal x,y,z and offset k)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) scenario_translate_3_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_translate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007441B0, // $TODO write the function chuckle nuts
	.documentation = "translates scenario objects (specified by the filter) in the specified BSP the given amount (x,y,z)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) scenario_rotate_6_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_rotate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007446E0, // $TODO write the function chuckle nuts
	.documentation = "rotates scenario objects (specified by the filter) in the specified BSP around the given axis (x,y,z) by the given amount (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) scenario_reflect_6_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_reflect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744B70, // $TODO write the function chuckle nuts
	.documentation = "reflects scenario objects (specified by the filter) in the specified BSP across the given plane (specified by normal x,y,z and offset k)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) scenario_translate_5_definition
{
	.return_type = _hs_type_void,
	.name = "scenario_translate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745270, // $TODO write the function chuckle nuts
	.documentation = "translates scenario objects (specified by the filter) in the specified BSP the given amount (x,y,z)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_set_task_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_set_task",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745680, // $TODO write the function chuckle nuts
	.documentation = "Sets the current task of the current task\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_objective_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_set_objective",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745BE0, // $TODO write the function chuckle nuts
	.documentation = "Sets the squad's current objective\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_task_status_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_task_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007460D0, // $TODO write the function chuckle nuts
	.documentation = "Returns the status of the given objective/task. See ai_task_status_<tab> for return codes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_set_task_condition_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_set_task_condition",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746460, // $TODO write the function chuckle nuts
	.documentation = "Latches the task condition true or false\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_leadership_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_leadership",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746920, // $TODO write the function chuckle nuts
	.documentation = "Returns whether a squad or task has a leader\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_leadership_all_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_leadership_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746DC0, // $TODO write the function chuckle nuts
	.documentation = "Returns whether a task or any of its children has a leader\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_task_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_task_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007471B0, // $TODO write the function chuckle nuts
	.documentation = "Returns the living count of the AI in the given task OR any of its children\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_reset_objective_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_reset_objective",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747620, // $TODO write the function chuckle nuts
	.documentation = "Completely reset the given objective, task or task hierarchy (if passed a gate task).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_squad_patrol_objective_disallow_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_squad_patrol_objective_disallow",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007478C0, // $TODO write the function chuckle nuts
	.documentation = "Disallow the selection of an objective by any squad patrol.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) generate_pathfinding_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "generate_pathfinding",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747DE0, // $TODO write the function chuckle nuts
	.documentation = "Generate pathfinding info for all structure bsps in the current scenario\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_render_paths_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_render_paths_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747FF0, // $TODO write the function chuckle nuts
	.documentation = "Turns on raw, smoothed, avoided paths and avoidance obstacles\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_activity_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_activity_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748280, // $TODO write the function chuckle nuts
	.documentation = "Set the given activity running on the given AI(s)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_activity_abort_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_activity_abort",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748680, // $TODO write the function chuckle nuts
	.documentation = "Aborts the activities running on the given AI(s)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_get_1_definition
{
	.return_type = _hs_type_vehicle,
	.name = "ai_vehicle_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F680, // $TODO write the function chuckle nuts
	.documentation = "Returns the vehicle that the given actor is in.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_get_from_starting_location_1_definition
{
	.return_type = _hs_type_vehicle,
	.name = "ai_vehicle_get_from_starting_location",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FBF0, // $TODO write the function chuckle nuts
	.documentation = "PLEASE START USING ai_vehicle_get_from_spawn_point INSTEAD\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_get_from_spawn_point_1_definition
{
	.return_type = _hs_type_vehicle,
	.name = "ai_vehicle_get_from_spawn_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FFB0, // $TODO write the function chuckle nuts
	.documentation = "Returns the vehicle that was spawned at the given spawn point\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_get_squad_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_vehicle_get_squad_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007304E0, // $TODO write the function chuckle nuts
	.documentation = "Returns the number of vehicles in the given squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_vehicle_get_from_squad_2_definition
{
	.return_type = _hs_type_vehicle,
	.name = "ai_vehicle_get_from_squad",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730940, // $TODO write the function chuckle nuts
	.documentation = "Returns the nth vehicle from the given squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_vehicle_reserve_seat_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_vehicle_reserve_seat",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730E30, // $TODO write the function chuckle nuts
	.documentation = "Reserves the given seat on the given vehicle (so that AI may not enter it\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_vehicle_reserve_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_vehicle_reserve",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731410, // $TODO write the function chuckle nuts
	.documentation = "Reserves the given vehicle (so that AI may not enter it\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_player_get_vehicle_squad_1_definition
{
	.return_type = _hs_type_ai,
	.name = "ai_player_get_vehicle_squad",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731790, // $TODO write the function chuckle nuts
	.documentation = "Returns the squad (if any) of the vehicle the player is riding in (note, the argument is a UNIT)\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_count_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_vehicle_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731C70, // $TODO write the function chuckle nuts
	.documentation = "Returns the number of vehicles being driven / ridden by the actors in the given squad/squad-group/task/etc.\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_carrying_player_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_carrying_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732040, // $TODO write the function chuckle nuts
	.documentation = "Returns whether any of the given AI are in a vehicle with the player\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_in_vehicle_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_in_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732480, // $TODO write the function chuckle nuts
	.documentation = "Returns whether any players are in the given vehicle\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_player_needs_vehicle_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_player_needs_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732860, // $TODO write the function chuckle nuts
	.documentation = "Returns if the given player needs a vehicle at the moment\r\nNETWORK SAFE: UNKNOWN, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_player_any_needs_vehicle_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_player_any_needs_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732CD0, // $TODO write the function chuckle nuts
	.documentation = "Returns if any player needs a vehicle at the moment\r\nNETWORK SAFE: UNKNOWN, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_vehicle_enter_3_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_enter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732FE0, // $TODO write the function chuckle nuts
	.documentation = "tells a group of actors to get into a vehicle... does not interrupt any actors who are already going to vehicles\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_unit,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_vehicle_enter_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_enter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733400, // $TODO write the function chuckle nuts
	.documentation = "tells a group of actors to get into a vehicle... does not interrupt any actors who are already going to vehicles\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_vehicle_enter_immediate_3_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_enter_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733970, // $TODO write the function chuckle nuts
	.documentation = "the given group of actors is snapped into a vehicle\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_unit,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_vehicle_enter_immediate_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_enter_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733C20, // $TODO write the function chuckle nuts
	.documentation = "the given group of actors is snapped into a vehicle\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_enter_squad_vehicles_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_enter_squad_vehicles",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007340D0, // $TODO write the function chuckle nuts
	.documentation = "Instructs the ai in the given squad to get in all their vehicles\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_vehicle_exit_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734470, // $TODO write the function chuckle nuts
	.documentation = "tells a group of actors to get out of any vehicles that they are in\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_vehicle_exit_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734970, // $TODO write the function chuckle nuts
	.documentation = "tells a group of actors to get out of any vehicles that they are in\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_overturned_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "vehicle_overturned",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734C50, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the vehicle is overturned\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_flip_1_definition
{
	.return_type = _hs_type_void,
	.name = "vehicle_flip",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735110, // $TODO write the function chuckle nuts
	.documentation = "Flips an overturned vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_combat_status_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007354B0, // $TODO write the function chuckle nuts
	.documentation = "Returns the highest integer combat status in the given squad-group/squad/actor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) flock_start_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "flock_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735830, // $TODO write the function chuckle nuts
	.documentation = "The flock starts producing boids\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) flock_stop_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "flock_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735D70, // $TODO write the function chuckle nuts
	.documentation = "The flock stops producing boids\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) flock_create_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "flock_create",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736160, // $TODO write the function chuckle nuts
	.documentation = "Create the given flock\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) flock_delete_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "flock_delete",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736640, // $TODO write the function chuckle nuts
	.documentation = "Delete the given flock\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) flock_definition_set_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "flock_definition_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736A00, // $TODO write the function chuckle nuts
	.documentation = "Set the definition of the given flock to the given tag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) flock_unperch_1_definition
{
	.return_type = _hs_type_void,
	.name = "flock_unperch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736F20, // $TODO write the function chuckle nuts
	.documentation = "Cause the given flock to unperch (if it is perched)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) drop_ai_2_definition
{
	.return_type = _hs_type_void,
	.name = "drop_ai",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007371E0, // $TODO write the function chuckle nuts
	.documentation = "Drop an ai with the given weapon at the current camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_verify_tags_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_verify_tags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737520, // $TODO write the function chuckle nuts
	.documentation = "Verifies state of ai-related tags (e.g. orders, squads, zones, etc.)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_wall_lean_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "ai_wall_lean",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737660, // $TODO write the function chuckle nuts
	.documentation = "Makes the actor lean against a wall RIGHT NOW\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_play_line_2_definition
{
	.return_type = _hs_type_real,
	.name = "ai_play_line",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737AE0, // $TODO write the function chuckle nuts
	.documentation = "Play the given mission dialogue line on the given ai\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_line
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_play_line_at_player_2_definition
{
	.return_type = _hs_type_real,
	.name = "ai_play_line_at_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007380A0, // $TODO write the function chuckle nuts
	.documentation = "Play the given mission dialogue line on the given ai, directing the ai's gaze at the nearest visible player\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_line
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_play_line_on_object_2_definition
{
	.return_type = _hs_type_real,
	.name = "ai_play_line_on_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738400, // $TODO write the function chuckle nuts
	.documentation = "Play the given mission dialogue line on the given object (uses first available variant)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_ai_line
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_play_line_on_object_for_team_3_definition
{
	.return_type = _hs_type_real,
	.name = "ai_play_line_on_object_for_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007389E0, // $TODO write the function chuckle nuts
	.documentation = "Play the given mission dialogue line on the given object (uses first available variant) if anyone on the local box is on the specified team\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_ai_line,
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_play_line_on_point_set_3_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_play_line_on_point_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738F00, // $TODO write the function chuckle nuts
	.documentation = "Play the given line (with the given 3-letter variant) on the N closest point to players in the given point set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) ai_play_line_on_point_set_4_definition
{
	.return_type = _hs_type_short_integer,
	.name = "ai_play_line_on_point_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739490, // $TODO write the function chuckle nuts
	.documentation = "Play the given line (with the given 3-letter variant) on the N closest point to players in the given point set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_point_ref,
		_hs_type_short_integer,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) campaign_metagame_time_pause_1_definition
{
	.return_type = _hs_type_void,
	.name = "campaign_metagame_time_pause",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007399B0, // $TODO write the function chuckle nuts
	.documentation = "pause meta game time, while paused your completion multiplier stops going down\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) campaign_metagame_award_points_2_definition
{
	.return_type = _hs_type_void,
	.name = "campaign_metagame_award_points",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739D70, // $TODO write the function chuckle nuts
	.documentation = "award bonus metagame points to a player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) campaign_metagame_award_primary_skull_2_definition
{
	.return_type = _hs_type_void,
	.name = "campaign_metagame_award_primary_skull",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A0A0, // $TODO write the function chuckle nuts
	.documentation = "award a primary skull to a player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_primary_skull
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) campaign_metagame_award_secondary_skull_2_definition
{
	.return_type = _hs_type_void,
	.name = "campaign_metagame_award_secondary_skull",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A590, // $TODO write the function chuckle nuts
	.documentation = "award a secondary skull to a player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_secondary_skull
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) campaign_metagame_award_event_2_definition
{
	.return_type = _hs_type_void,
	.name = "campaign_metagame_award_event",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A8F0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_metagame_enabled_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_metagame_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AD50, // $TODO write the function chuckle nuts
	.documentation = "TRUE if the metagame is enabled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_survival_enabled_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_survival_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B090, // $TODO write the function chuckle nuts
	.documentation = "TRUE if survival mode is enabled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_is_finished_easy_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_is_finished_easy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B2D0, // $TODO write the function chuckle nuts
	.documentation = "TRUE if any of the players have finished the campaign on easy\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_is_finished_normal_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_is_finished_normal",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B700, // $TODO write the function chuckle nuts
	.documentation = "TRUE if any of the players have finished the campaign on normal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_is_finished_heroic_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_is_finished_heroic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B940, // $TODO write the function chuckle nuts
	.documentation = "TRUE if any of the players have finished the campaign on heroic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) campaign_is_finished_legendary_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "campaign_is_finished_legendary",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BC50, // $TODO write the function chuckle nuts
	.documentation = "TRUE if any of the players have finished the campaign on legendary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_run_command_script_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_run_command_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BFD0, // $TODO write the function chuckle nuts
	.documentation = "Causes the specified actor(s) to start executing a command script immediately (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_command_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_queue_command_script_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_queue_command_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C380, // $TODO write the function chuckle nuts
	.documentation = "Add a command script onto the end of an actor's command script queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_command_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_stack_command_script_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_stack_command_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C650, // $TODO write the function chuckle nuts
	.documentation = "Push a command script to the top of the actor's command script queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_command_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_reserve_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_reserve",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CBD0, // $TODO write the function chuckle nuts
	.documentation = "Reserves the specified actor(s) for the use of the current script (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_reserve_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_reserve",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CF90, // $TODO write the function chuckle nuts
	.documentation = "Reserves the specified actor(s) for the use of the current script (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_cast_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D3C0, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_cast_5_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D8E0, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) vs_cast_6_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DF10, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 14) vs_cast_7_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E610, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 7,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 16) vs_cast_8_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ECA0, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 8,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 18) vs_cast_9_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F410, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 9,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 20) vs_cast_10_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_cast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FAC0, // $TODO write the function chuckle nuts
	.documentation = "Cast 7 actors for a vignette (at the given priority, higher = more important)",
	.parameters = NULL,
	.formal_parameter_count = 10,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_role_1_definition
{
	.return_type = _hs_type_ai,
	.name = "vs_role",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740200, // $TODO write the function chuckle nuts
	.documentation = "Returns the given member of the cast (as per more recent vs_cast call)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_abort_on_alert_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_alert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007406A0, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_abort_on_damage_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740A50, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_abort_on_combat_status_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740CD0, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_abort_on_vehicle_exit_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741200, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_alert_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_alert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741420, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_damage_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741850, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_combat_status_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741B50, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_vehicle_exit_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741E70, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_alert_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_alert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007422A0, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_abort_on_alert_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_alert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742910, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_damage_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007430C0, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_abort_on_damage_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_damage",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743780, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_combat_status_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744070, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_abort_on_combat_status_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007447C0, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_abort_on_vehicle_exit_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_abort_on_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744F10, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_abort_on_vehicle_exit_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_abort_on_vehicle_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745540, // $TODO write the function chuckle nuts
	.documentation = "Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_set_cleanup_script_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_set_cleanup_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745DC0, // $TODO write the function chuckle nuts
	.documentation = "Sets the script that is automatically called when the current thread ends\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_release_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_release",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746180, // $TODO write the function chuckle nuts
	.documentation = "Lets the given ai out of the current command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) vs_release_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "vs_release_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746440, // $TODO write the function chuckle nuts
	.documentation = "Releases all the ai attached to the current thread \r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_command_script_running_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "cs_command_script_running",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007466D0, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai is running the command script in question\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_command_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_command_script_queued_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "cs_command_script_queued",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746BB0, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the command script is in the ai's cs queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai_command_script
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_number_queued_1_definition
{
	.return_type = _hs_type_short_integer,
	.name = "cs_number_queued",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007470A0, // $TODO write the function chuckle nuts
	.documentation = "Returns the number of queued command scripts\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_moving_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "cs_moving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747390, // $TODO write the function chuckle nuts
	.documentation = "Returns TRUE if the actor is currently following a path\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_moving_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "cs_moving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007479D0, // $TODO write the function chuckle nuts
	.documentation = "Returns TRUE if the actor is currently following a path\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_running_atom_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_running_atom",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748240, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai is running any atom\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_running_atom_movement_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_running_atom_movement",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748710, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai is running an atom on the movement channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_running_atom_action_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_running_atom_action",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F800, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai is running an atom on the action channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_running_atom_dialogue_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "vs_running_atom_dialogue",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FC60, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the ai is running an atom on the dialogue channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_fly_to_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_to",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730150, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_fly_to_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007307F0, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_fly_to_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_to",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731370, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_fly_to_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007319D0, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_fly_to_and_face_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_to_and_face",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007324C0, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_fly_to_and_face_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_to_and_face",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732B10, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_fly_to_and_face_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_to_and_face",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733690, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_fly_to_and_face_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_to_and_face",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733D80, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_fly_by_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_by",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007349A0, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_fly_by_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_by",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735030, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_fly_by_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_fly_by",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735BD0, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_fly_by_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_fly_by",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736210, // $TODO write the function chuckle nuts
	.documentation = "Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_go_to_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736D80, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_go_to_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007373E0, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_go_to_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737F20, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_go_to_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738500, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_go_by_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_by",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739150, // $TODO write the function chuckle nuts
	.documentation = "Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_go_by_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_by",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739850, // $TODO write the function chuckle nuts
	.documentation = "Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_go_by_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_by",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A3D0, // $TODO write the function chuckle nuts
	.documentation = "Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_go_by_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_by",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AAC0, // $TODO write the function chuckle nuts
	.documentation = "Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_go_to_and_face_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to_and_face",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B5D0, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point and has him face the second point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_go_to_and_face_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to_and_face",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BCA0, // $TODO write the function chuckle nuts
	.documentation = "Moves the actor to a specified point and has him face the second point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_go_to_and_posture_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to_and_posture",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C710, // $TODO write the function chuckle nuts
	.documentation = "Actor moves to the point and performs a posture once there\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_go_to_and_posture_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to_and_posture",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CE80, // $TODO write the function chuckle nuts
	.documentation = "Actor moves to the point and performs a posture once there\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_go_to_nearest_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to_nearest",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D9B0, // $TODO write the function chuckle nuts
	.documentation = "Given a point set, AI goes toward the nearest point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_go_to_nearest_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to_nearest",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E060, // $TODO write the function chuckle nuts
	.documentation = "Given a point set, AI goes toward the nearest point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_move_in_direction_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_move_in_direction",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EAD0, // $TODO write the function chuckle nuts
	.documentation = "Actor moves at given angle, for the given distance, optionally with the given facing (angle, distance, facing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_move_in_direction_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_move_in_direction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F280, // $TODO write the function chuckle nuts
	.documentation = "Actor moves at given angle, for the given distance, optionally with the given facing (angle, distance, facing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_move_towards_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_move_towards",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FDC0, // $TODO write the function chuckle nuts
	.documentation = "Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_move_towards_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_move_towards",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740490, // $TODO write the function chuckle nuts
	.documentation = "Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_move_towards_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_move_towards",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740EA0, // $TODO write the function chuckle nuts
	.documentation = "Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_move_towards_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_move_towards",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741660, // $TODO write the function chuckle nuts
	.documentation = "Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_move_towards_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_move_towards_point",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007420E0, // $TODO write the function chuckle nuts
	.documentation = "Move to within the given radius of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_move_towards_point_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_move_towards_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742750, // $TODO write the function chuckle nuts
	.documentation = "Move to within the given radius of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_swarm_to_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_swarm_to",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007433D0, // $TODO write the function chuckle nuts
	.documentation = "A swarm swarms towards the point (or point set) in question, the individual forms stopping when they get within the given radius\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_swarm_to_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_swarm_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743AB0, // $TODO write the function chuckle nuts
	.documentation = "A swarm swarms towards the point (or point set) in question, the individual forms stopping when they get within the given radius\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_swarm_from_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_swarm_from",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007445E0, // $TODO write the function chuckle nuts
	.documentation = "A swarm swarms away from the point in question, going at least the given distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_swarm_from_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_swarm_from",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744CF0, // $TODO write the function chuckle nuts
	.documentation = "A swarm swarms away from the point in question, going at least the given distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_pause_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_pause",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745860, // $TODO write the function chuckle nuts
	.documentation = "The actor does nothing for the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_pause_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_pause",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745F30, // $TODO write the function chuckle nuts
	.documentation = "The actor does nothing for the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_grenade_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_grenade",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746880, // $TODO write the function chuckle nuts
	.documentation = "Actor throws a grenade, either by tossing (arg2=0), lobbing (1) or bouncing (2)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_grenade_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_grenade",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746FC0, // $TODO write the function chuckle nuts
	.documentation = "Actor throws a grenade, either by tossing (arg2=0), lobbing (1) or bouncing (2)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_equipment_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_equipment",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747A60, // $TODO write the function chuckle nuts
	.documentation = "Actor throws their equipment in the direction of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_equipment_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748110, // $TODO write the function chuckle nuts
	.documentation = "Actor throws their equipment in the direction of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_jump_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_jump",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F900, // $TODO write the function chuckle nuts
	.documentation = "Actor jumps in direction of angle at the given velocity (angle, velocity)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_jump_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_jump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730000, // $TODO write the function chuckle nuts
	.documentation = "Actor jumps in direction of angle at the given velocity (angle, velocity)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_jump_to_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_jump_to_point",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730C00, // $TODO write the function chuckle nuts
	.documentation = "Actor jumps with given horizontal and vertical velocity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_jump_to_point_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_jump_to_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731280, // $TODO write the function chuckle nuts
	.documentation = "Actor jumps with given horizontal and vertical velocity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_vocalize_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_vocalize",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731DB0, // $TODO write the function chuckle nuts
	.documentation = "Actor emits vocalization of given type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_vocalize_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_vocalize",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007323A0, // $TODO write the function chuckle nuts
	.documentation = "Actor emits vocalization of given type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_play_sound_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_play_sound",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732EF0, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_play_sound_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_play_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733520, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_play_sound_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_play_sound",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733FC0, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_play_sound_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_play_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734670, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_sound,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_play_sound_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_play_sound",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735330, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_play_sound_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_play_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007359E0, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_sound,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_action_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_action",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736530, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the given point. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_action_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_action",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736C80, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the given point. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_action_at_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_action_at_object",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007376E0, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the given object. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_action_at_object_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_action_at_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737E40, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the given object. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_action_at_player_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_action_at_player",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738950, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the nearest_player. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_action_at_player_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_action_at_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739040, // $TODO write the function chuckle nuts
	.documentation = "Actor plays an AI action towards the nearest_player. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_custom_animation_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739B40, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_custom_animation_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A2E0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cs_custom_animation_4_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AE30, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) vs_custom_animation_6_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B4E0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_custom_animation_death_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation_death",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C1B0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_custom_animation_death_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation_death",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C8B0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cs_custom_animation_death_4_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation_death",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D4B0, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) vs_custom_animation_death_6_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation_death",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DC70, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_custom_animation_loop_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E810, // $TODO write the function chuckle nuts
	.documentation = "starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_custom_animation_loop_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EF40, // $TODO write the function chuckle nuts
	.documentation = "starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cs_custom_animation_loop_4_definition
{
	.return_type = _hs_type_void,
	.name = "cs_custom_animation_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F8E0, // $TODO write the function chuckle nuts
	.documentation = "starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_custom_animation_loop_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_custom_animation_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740070, // $TODO write the function chuckle nuts
	.documentation = "starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_play_line_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_play_line",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740910, // $TODO write the function chuckle nuts
	.documentation = "Play the named line in the current scene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai_line
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_play_line_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_play_line",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741000, // $TODO write the function chuckle nuts
	.documentation = "Play the named line in the current scene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_ai_line
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_die_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_die",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741A00, // $TODO write the function chuckle nuts
	.documentation = "Actor dies in specified manner\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_die_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_die",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741FB0, // $TODO write the function chuckle nuts
	.documentation = "Actor dies in specified manner\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_deploy_turret_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_deploy_turret",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742B60, // $TODO write the function chuckle nuts
	.documentation = "Deploy a turret at the given script point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_deploy_turret_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_deploy_turret",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743190, // $TODO write the function chuckle nuts
	.documentation = "Deploy a turret at the given script point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cs_approach_4_definition
{
	.return_type = _hs_type_void,
	.name = "cs_approach",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743CF0, // $TODO write the function chuckle nuts
	.documentation = "(approach <object> <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) vs_approach_6_definition
{
	.return_type = _hs_type_void,
	.name = "vs_approach",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007444B0, // $TODO write the function chuckle nuts
	.documentation = "(approach <object> <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cs_approach_player_3_definition
{
	.return_type = _hs_type_void,
	.name = "cs_approach_player",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745130, // $TODO write the function chuckle nuts
	.documentation = "(approach player <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vs_approach_player_5_definition
{
	.return_type = _hs_type_void,
	.name = "vs_approach_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745900, // $TODO write the function chuckle nuts
	.documentation = "(approach player <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_go_to_vehicle_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to_vehicle",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746380, // $TODO write the function chuckle nuts
	.documentation = "Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_go_to_vehicle_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746A40, // $TODO write the function chuckle nuts
	.documentation = "Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_go_to_vehicle_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_go_to_vehicle",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747440, // $TODO write the function chuckle nuts
	.documentation = "Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) vs_go_to_vehicle_4_definition
{
	.return_type = _hs_type_void,
	.name = "vs_go_to_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747B10, // $TODO write the function chuckle nuts
	.documentation = "Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_vehicle,
		_hs_type_unit_seat_mapping
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_set_style_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_set_style",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748600, // $TODO write the function chuckle nuts
	.documentation = "Override the actor's style\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_style
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_set_style_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_set_style",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FA90, // $TODO write the function chuckle nuts
	.documentation = "Override the actor's style\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_style
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_force_combat_status_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_force_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730340, // $TODO write the function chuckle nuts
	.documentation = "Force the actor's combat status (0= no override, 1=idle, 2= alert, 3= active)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_force_combat_status_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_force_combat_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730A10, // $TODO write the function chuckle nuts
	.documentation = "Force the actor's combat status (0= no override, 1=idle, 2= alert, 3= active)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_enable_targeting_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_enable_targeting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731100, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous target selection enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_enable_targeting_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_enable_targeting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007317F0, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous target selection enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_enable_looking_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_enable_looking",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731F80, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous looking enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_enable_looking_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_enable_looking",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007326C0, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous looking enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_enable_moving_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_enable_moving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732DD0, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous moving enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_enable_moving_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_enable_moving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733470, // $TODO write the function chuckle nuts
	.documentation = "Actor autonomous moving enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_enable_dialogue_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_enable_dialogue",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733B90, // $TODO write the function chuckle nuts
	.documentation = "Actor combat dialogue enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_enable_dialogue_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_enable_dialogue",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734320, // $TODO write the function chuckle nuts
	.documentation = "Actor combat dialogue enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_suppress_activity_termination_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_suppress_activity_termination",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734B90, // $TODO write the function chuckle nuts
	.documentation = "Autonomous termination of activities is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_suppress_activity_termination_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_suppress_activity_termination",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735240, // $TODO write the function chuckle nuts
	.documentation = "Autonomous termination of activities is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_suppress_dialogue_global_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_suppress_dialogue_global",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735950, // $TODO write the function chuckle nuts
	.documentation = "Combat dialogue is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_suppress_dialogue_global_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_suppress_dialogue_global",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736070, // $TODO write the function chuckle nuts
	.documentation = "Combat dialogue is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_look_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_look",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736870, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at the point for the remainder of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_look_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_look",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736FB0, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at the point for the remainder of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_look_player_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_look_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737810, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at nearest player for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_look_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_look_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737DB0, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at nearest player for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_look_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_look_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738720, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at the object for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_look_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_look_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738D30, // $TODO write the function chuckle nuts
	.documentation = "Actor looks at the object for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_aim_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_aim",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007395C0, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at the point for the remainder of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_aim_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_aim",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739CE0, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at the point for the remainder of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_aim_player_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_aim_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A5F0, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at nearest player for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_aim_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_aim_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AC10, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at nearest player for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_aim_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_aim_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B340, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at the object for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_aim_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_aim_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BAA0, // $TODO write the function chuckle nuts
	.documentation = "Actor aims at the object for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_face_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_face",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C2F0, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the point for the remainder of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_face_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_face",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CAC0, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the point for the remainder of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_face_player_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_face_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D260, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the nearest player for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_face_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_face_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D850, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the nearest player for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_face_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_face_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E260, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the given object for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_face_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_face_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E780, // $TODO write the function chuckle nuts
	.documentation = "Actor faces exactly the given object for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_shoot_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_shoot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F0D0, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_shoot_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_shoot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F760, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_shoot_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_shoot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FED0, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_shoot_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_shoot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740610, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_shoot_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_shoot_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740D10, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_shoot_point_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_shoot_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007414D0, // $TODO write the function chuckle nuts
	.documentation = "Actor shoots at given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_shoot_secondary_trigger_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_shoot_secondary_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741D60, // $TODO write the function chuckle nuts
	.documentation = "Actor uses secondary trigger whenever he shoots\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_shoot_secondary_trigger_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_shoot_secondary_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742400, // $TODO write the function chuckle nuts
	.documentation = "Actor uses secondary trigger whenever he shoots\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_lower_weapon_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_lower_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742BF0, // $TODO write the function chuckle nuts
	.documentation = "Actor keeps weapon lowered\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_lower_weapon_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_lower_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743270, // $TODO write the function chuckle nuts
	.documentation = "Actor keeps weapon lowered\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_vehicle_speed_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_vehicle_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743A20, // $TODO write the function chuckle nuts
	.documentation = "Set the speed at which the actor will drive a vehicle, expressed as a multiplier 0-1\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_vehicle_speed_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_vehicle_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744120, // $TODO write the function chuckle nuts
	.documentation = "Set the speed at which the actor will drive a vehicle, expressed as a multiplier 0-1\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_vehicle_speed_instantaneous_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_vehicle_speed_instantaneous",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744990, // $TODO write the function chuckle nuts
	.documentation = "Set the instantaneous speed of the vehicle we're driving\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_vehicle_speed_instantaneous_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_vehicle_speed_instantaneous",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745020, // $TODO write the function chuckle nuts
	.documentation = "Set the instantaneous speed of the vehicle we're driving\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_vehicle_boost_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_vehicle_boost",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007457D0, // $TODO write the function chuckle nuts
	.documentation = "Enables or disables boost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_vehicle_boost_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_vehicle_boost",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745EA0, // $TODO write the function chuckle nuts
	.documentation = "Enables or disables boost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_turn_sharpness_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_turn_sharpness",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746610, // $TODO write the function chuckle nuts
	.documentation = "Set the sharpness of a vehicle turn (values 0 -> 1). Only applicable to nondirectional flying vehicles (e.g. dropships)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_turn_sharpness_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_turn_sharpness",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746D30, // $TODO write the function chuckle nuts
	.documentation = "Set the sharpness of a vehicle turn (values 0 -> 1). Only applicable to nondirectional flying vehicles (e.g. dropships)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_enable_pathfinding_failsafe_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_enable_pathfinding_failsafe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747570, // $TODO write the function chuckle nuts
	.documentation = "Actor blocks until pathfinding calls succeed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_enable_pathfinding_failsafe_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_enable_pathfinding_failsafe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747CC0, // $TODO write the function chuckle nuts
	.documentation = "Actor blocks until pathfinding calls succeed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_set_pathfinding_radius_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_set_pathfinding_radius",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007483A0, // $TODO write the function chuckle nuts
	.documentation = "Sets the actor's pathfinding radius (this distance at which a destination is considered to have been reached) for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_set_pathfinding_radius_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_set_pathfinding_radius",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F700, // $TODO write the function chuckle nuts
	.documentation = "Sets the actor's pathfinding radius (this distance at which a destination is considered to have been reached) for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_ignore_obstacles_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_ignore_obstacles",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FEF0, // $TODO write the function chuckle nuts
	.documentation = "Actor does not avoid obstacles when true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_ignore_obstacles_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_ignore_obstacles",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730680, // $TODO write the function chuckle nuts
	.documentation = "Actor does not avoid obstacles when true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_approach_stop_0_definition
{
	.return_type = _hs_type_void,
	.name = "cs_approach_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730EE0, // $TODO write the function chuckle nuts
	.documentation = "Actor stops approaching\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_approach_stop_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_approach_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007314A0, // $TODO write the function chuckle nuts
	.documentation = "Actor stops approaching\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_movement_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_movement_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731BB0, // $TODO write the function chuckle nuts
	.documentation = "Actor switches to given animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_movement_mode_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_movement_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007321C0, // $TODO write the function chuckle nuts
	.documentation = "Actor switches to given animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_crouch_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_crouch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732990, // $TODO write the function chuckle nuts
	.documentation = "Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_crouch_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_crouch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733060, // $TODO write the function chuckle nuts
	.documentation = "Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_crouch_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_crouch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007338E0, // $TODO write the function chuckle nuts
	.documentation = "Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_crouch_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_crouch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733F30, // $TODO write the function chuckle nuts
	.documentation = "Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_walk_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_walk",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007347C0, // $TODO write the function chuckle nuts
	.documentation = "Actor walks for the remainder of the command script, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_walk_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_walk",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734E90, // $TODO write the function chuckle nuts
	.documentation = "Actor walks for the remainder of the command script, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_posture_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_posture_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007356A0, // $TODO write the function chuckle nuts
	.documentation = "Actor enters the given posture (enters immediately rather than plays the entry animation if argument is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_posture_set_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_posture_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735DE0, // $TODO write the function chuckle nuts
	.documentation = "Actor enters the given posture (enters immediately rather than plays the entry animation if argument is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_posture_exit_0_definition
{
	.return_type = _hs_type_void,
	.name = "cs_posture_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007365D0, // $TODO write the function chuckle nuts
	.documentation = "Actor exits the posture it's in\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_posture_exit_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_posture_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736AA0, // $TODO write the function chuckle nuts
	.documentation = "Actor exits the posture it's in\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_stow_0_definition
{
	.return_type = _hs_type_void,
	.name = "cs_stow",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737280, // $TODO write the function chuckle nuts
	.documentation = "Actor stows its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_stow_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_stow",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737910, // $TODO write the function chuckle nuts
	.documentation = "Actor stows its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_draw_0_definition
{
	.return_type = _hs_type_void,
	.name = "cs_draw",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738010, // $TODO write the function chuckle nuts
	.documentation = "Actor draws its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_draw_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_draw",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738480, // $TODO write the function chuckle nuts
	.documentation = "Actor draws its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cs_teleport_2_definition
{
	.return_type = _hs_type_void,
	.name = "cs_teleport",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738C70, // $TODO write the function chuckle nuts
	.documentation = "Actor teleports to point1 facing point2\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_teleport_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_teleport",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739400, // $TODO write the function chuckle nuts
	.documentation = "Actor teleports to point1 facing point2\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_point_ref,
		_hs_type_point_ref
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cs_stop_custom_animation_0_definition
{
	.return_type = _hs_type_void,
	.name = "cs_stop_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739C40, // $TODO write the function chuckle nuts
	.documentation = "Stop running a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vs_stop_custom_animation_1_definition
{
	.return_type = _hs_type_void,
	.name = "vs_stop_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A160, // $TODO write the function chuckle nuts
	.documentation = "Stop running a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_stop_sound_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_stop_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A930, // $TODO write the function chuckle nuts
	.documentation = "Stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vs_stop_sound_2_definition
{
	.return_type = _hs_type_void,
	.name = "vs_stop_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B000, // $TODO write the function chuckle nuts
	.documentation = "Stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_player_melee_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_player_melee",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B810, // $TODO write the function chuckle nuts
	.documentation = "Actor performs player-specific melee\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_player_melee_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_player_melee",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BE30, // $TODO write the function chuckle nuts
	.documentation = "Actor performs player-specific melee\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_melee_direction_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_melee_direction",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C820, // $TODO write the function chuckle nuts
	.documentation = "Actor performs melee in provided direction (0= front, 1= back, 2= offhand, 3= tackle)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_melee_direction_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_melee_direction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D050, // $TODO write the function chuckle nuts
	.documentation = "Actor performs melee in provided direction (0= front, 1= back, 2= offhand, 3= tackle)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cs_smash_direction_1_definition
{
	.return_type = _hs_type_void,
	.name = "cs_smash_direction",
	.flags = FLAG(_hs_function_flag_command_script_atom),
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DAC0, // $TODO write the function chuckle nuts
	.documentation = "Actor performs smash in provided direction\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) vs_smash_direction_3_definition
{
	.return_type = _hs_type_void,
	.name = "vs_smash_direction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E180, // $TODO write the function chuckle nuts
	.documentation = "Actor performs smash in provided direction\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) camera_control_1_definition
{
	.return_type = _hs_type_void,
	.name = "camera_control",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EC20, // $TODO write the function chuckle nuts
	.documentation = "toggles script control of the camera.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EEC0, // $TODO write the function chuckle nuts
	.documentation = "moves the camera to the specified camera point over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) camera_set_relative_3_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_relative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F3D0, // $TODO write the function chuckle nuts
	.documentation = "moves the camera to the specified camera point over the specified number of ticks (position is relative to the specified object).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point,
		_hs_type_short_integer,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_animation_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F7F0, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) camera_set_animation_relative_4_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation_relative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FBC0, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_unit,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) camera_set_animation_with_speed_3_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation_with_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740030, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) camera_set_animation_relative_with_speed_5_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation_relative_with_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007403F0, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_unit,
		_hs_type_cutscene_flag,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) camera_set_animation_relative_with_speed_loop_6_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation_relative_with_speed_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007409A0, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_unit,
		_hs_type_cutscene_flag,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 14) camera_set_animation_relative_with_speed_loop_offset_7_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_animation_relative_with_speed_loop_offset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740E20, // $TODO write the function chuckle nuts
	.documentation = "begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 7,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_unit,
		_hs_type_cutscene_flag,
		_hs_type_real,
		_hs_type_boolean,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) camera_predict_resources_at_frame_5_definition
{
	.return_type = _hs_type_void,
	.name = "camera_predict_resources_at_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007413E0, // $TODO write the function chuckle nuts
	.documentation = "predict resources at a frame in camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_unit,
		_hs_type_cutscene_flag,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) camera_predict_resources_at_point_1_definition
{
	.return_type = _hs_type_void,
	.name = "camera_predict_resources_at_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007419B0, // $TODO write the function chuckle nuts
	.documentation = "predict resources given a camera point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) camera_set_first_person_1_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_first_person",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741D30, // $TODO write the function chuckle nuts
	.documentation = "makes the scripted camera follow a unit.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) camera_set_cinematic_0_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_cinematic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741F90, // $TODO write the function chuckle nuts
	.documentation = "makes the scripted camera follow the path of a cinematic scene.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) camera_set_cinematic_scene_3_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_cinematic_scene",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742330, // $TODO write the function chuckle nuts
	.documentation = "sets the scene, shot, and cutscene flag for the cinematic camera (NOTE: camera must be in cinematic mode by calling camera_set_cinematic).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_cinematic_scene_definition,
		_hs_type_long_integer,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) camera_place_relative_1_definition
{
	.return_type = _hs_type_void,
	.name = "camera_place_relative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742710, // $TODO write the function chuckle nuts
	.documentation = "all subsequent camera placement in sapien be marked as relative to this object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) camera_place_worldspace_0_definition
{
	.return_type = _hs_type_void,
	.name = "camera_place_worldspace",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007429F0, // $TODO write the function chuckle nuts
	.documentation = "all subsequent camera placement in sapien will be marked as worldspace\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) camera_time_0_definition
{
	.return_type = _hs_type_short_integer,
	.name = "camera_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742CF0, // $TODO write the function chuckle nuts
	.documentation = "returns the number of ticks remaining in the current camera interpolation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_field_of_view_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_field_of_view",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742ED0, // $TODO write the function chuckle nuts
	.documentation = "sets the field of view\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) cinematic_camera_set_easing_in_6_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_camera_set_easing_in",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743360, // $TODO write the function chuckle nuts
	.documentation = "controls the camera easing from player position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_camera_set_easing_out_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_camera_set_easing_out",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007438C0, // $TODO write the function chuckle nuts
	.documentation = "controls the camera easing back to the player position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_print_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_print",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743CC0, // $TODO write the function chuckle nuts
	.documentation = "Print status lines for cinematic debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_pan_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_pan",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744030, // $TODO write the function chuckle nuts
	.documentation = "moves the camera to the specified camera point over the specified number of ticks with a constant speed.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 14) camera_pan_7_definition
{
	.return_type = _hs_type_void,
	.name = "camera_pan",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007443C0, // $TODO write the function chuckle nuts
	.documentation = "camera_pan <start point> <end point> <ticks> <ease-in ticks> <start velocity scale> <ease-out ticks> <end velocity scale>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 7,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point,
		_hs_type_cutscene_camera_point,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_real,
		_hs_type_short_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_camera_save_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_camera_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744AD0, // $TODO write the function chuckle nuts
	.documentation = "saves the camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_camera_load_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_camera_load",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744C60, // $TODO write the function chuckle nuts
	.documentation = "loads the saved camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_camera_save_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_camera_save_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744EF0, // $TODO write the function chuckle nuts
	.documentation = "saves the camera position and facing to filename\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_camera_load_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_camera_load_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745250, // $TODO write the function chuckle nuts
	.documentation = "loads the camera position and facing from filename\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) director_debug_camera_1_definition
{
	.return_type = _hs_type_void,
	.name = "director_debug_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745420, // $TODO write the function chuckle nuts
	.documentation = "enable/disable camera debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_difficulty_get_0_definition
{
	.return_type = _hs_type_game_difficulty,
	.name = "game_difficulty_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745610, // $TODO write the function chuckle nuts
	.documentation = "returns the current difficulty setting, but lies to you and will never return easy, instead returning normal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_difficulty_get_real_0_definition
{
	.return_type = _hs_type_game_difficulty,
	.name = "game_difficulty_get_real",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745A90, // $TODO write the function chuckle nuts
	.documentation = "returns the actual current difficulty setting without lying\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_insertion_point_get_0_definition
{
	.return_type = _hs_type_short_integer,
	.name = "game_insertion_point_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745C90, // $TODO write the function chuckle nuts
	.documentation = "returns the number of the insertion point the map was started at\r\nNETWORK SAFE: Yes.",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_insertion_point_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_insertion_point_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746040, // $TODO write the function chuckle nuts
	.documentation = "retroactively sets the insertion point that we started at.\r\nNETWORK SAFE: Unknown, assumed unsafe.",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) pvs_set_object_1_definition
{
	.return_type = _hs_type_void,
	.name = "pvs_set_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746250, // $TODO write the function chuckle nuts
	.documentation = "sets the specified object as the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) pvs_set_camera_1_definition
{
	.return_type = _hs_type_void,
	.name = "pvs_set_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746530, // $TODO write the function chuckle nuts
	.documentation = "sets the specified cutscene camera point as the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_cutscene_camera_point
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) pvs_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "pvs_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746830, // $TODO write the function chuckle nuts
	.documentation = "removes the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) pvs_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "pvs_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746B50, // $TODO write the function chuckle nuts
	.documentation = "forces pvs to empty then back to full.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) players_unzoom_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "players_unzoom_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746D10, // $TODO write the function chuckle nuts
	.documentation = "resets zoom levels on all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_enable_input_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_enable_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746F60, // $TODO write the function chuckle nuts
	.documentation = "toggle player input. the player can still free-look, but nothing else.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_disable_movement_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_disable_movement",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007472C0, // $TODO write the function chuckle nuts
	.documentation = "toggle player input. the look stick works, but nothing else.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_disable_weapon_pickup_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_disable_weapon_pickup",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007476D0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable all players' ability to pick up weapons\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_disable_sprint_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_disable_sprint",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747980, // $TODO write the function chuckle nuts
	.documentation = "enable/disable all players' ability to sprint\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_disable_equipment_use_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_disable_equipment_use",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747E90, // $TODO write the function chuckle nuts
	.documentation = "enable/disable all players' ability to equipment\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_flashlight_on_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_flashlight_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007480C0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has a flashlight on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_active_camouflage_on_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_active_camouflage_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748430, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player is active camouflaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_camera_control_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_camera_control",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748780, // $TODO write the function chuckle nuts
	.documentation = "enables/disables camera control globally\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_action_test_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F8A0, // $TODO write the function chuckle nuts
	.documentation = "resets the player action test state so that all tests will return false.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_jump_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_jump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FB90, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has jumped since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_primary_trigger_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_primary_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FE50, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has used primary trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_grenade_trigger_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_grenade_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730220, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has used grenade trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_vision_trigger_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_vision_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730520, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has used vision trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_rotate_weapons_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_rotate_weapons",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007307C0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the rotate-weapon button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_rotate_grenades_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_rotate_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730BD0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the rotate-grenades button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_melee_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_melee",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730EB0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the melee button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_action_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_action",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007311F0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the action key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_accept_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_accept",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731590, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit accept since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_cancel_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_cancel",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007317C0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit cancel key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_look_relative_up_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_look_relative_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731B50, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked up since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_look_relative_down_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_look_relative_down",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731EB0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked down since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_look_relative_left_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_look_relative_left",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007320E0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked left since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_look_relative_right_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_look_relative_right",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732370, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_look_relative_all_directions_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_look_relative_all_directions",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007327B0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked up, down, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_move_relative_all_directions_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_move_relative_all_directions",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732A80, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has moved forward, backward, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_back_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_back",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732DA0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the back button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_dpad_left_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_dpad_left",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733130, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad left button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_dpad_right_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_dpad_right",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733370, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad right button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_dpad_up_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_dpad_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733730, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad up button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_dpad_down_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_dpad_down",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733A20, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad down button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_x_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_x",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733D50, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the x button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_y_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_y",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007340A0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the y button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_left_shoulder_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_left_shoulder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007343B0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the left shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_right_shoulder_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_right_shoulder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734600, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the right shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_action_test_waypoint_activate_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_waypoint_activate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734A30, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the button action to activate the waypoint since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_reset_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_action_test_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734E60, // $TODO write the function chuckle nuts
	.documentation = "resets the player action test state so that all tests will return false.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_primary_trigger_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_primary_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735200, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has used primary trigger since the last call to (player_action_test_reset).",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_vision_trigger_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_vision_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735660, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has used vision trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_rotate_weapons_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_rotate_weapons",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735B90, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the rotate-weapon button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_rotate_grenades_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_rotate_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735F90, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the rotate-grenades button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_melee_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_melee",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007363E0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the melee button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_action_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_action",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007367D0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit the action key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_accept_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_accept",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736BE0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit accept since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_cancel_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_cancel",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007370A0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has hit cancel key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_look_relative_up_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_look_relative_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007374E0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked up since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_look_relative_down_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_look_relative_down",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007379B0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked down since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_look_relative_left_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_look_relative_left",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737CF0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked left since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_look_relative_right_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_look_relative_right",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738220, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_look_relative_all_directions_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_look_relative_all_directions",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738620, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has looked up, down, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_move_relative_all_directions_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_move_relative_all_directions",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738B00, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has moved forward, backward, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_back_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_back",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738EC0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the back button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_dpad_left_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_dpad_left",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739350, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad left button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_dpad_right_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_dpad_right",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007397D0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad right button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_dpad_up_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_dpad_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739BE0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad up button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_dpad_down_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_dpad_down",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A040, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the dpad down button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_x_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_x",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A4E0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the x button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_y_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_y",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A8B0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the y button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_left_shoulder_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_left_shoulder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ADF0, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the left shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_action_test_right_shoulder_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_action_test_right_shoulder",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B190, // $TODO write the function chuckle nuts
	.documentation = "returns true if any player has pressed the right shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player0_looking_up_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player0_looking_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B4B0, // $TODO write the function chuckle nuts
	.documentation = "true if the first player is looking up\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player0_looking_down_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player0_looking_down",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B910, // $TODO write the function chuckle nuts
	.documentation = "true if the first player is looking down\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player0_set_pitch_2_definition
{
	.return_type = _hs_type_void,
	.name = "player0_set_pitch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BBC0, // $TODO write the function chuckle nuts
	.documentation = "sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player1_set_pitch_2_definition
{
	.return_type = _hs_type_void,
	.name = "player1_set_pitch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C050, // $TODO write the function chuckle nuts
	.documentation = "sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player2_set_pitch_2_definition
{
	.return_type = _hs_type_void,
	.name = "player2_set_pitch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C3E0, // $TODO write the function chuckle nuts
	.documentation = "sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player3_set_pitch_2_definition
{
	.return_type = _hs_type_void,
	.name = "player3_set_pitch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C6D0, // $TODO write the function chuckle nuts
	.documentation = "sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_lookstick_forward_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_lookstick_forward",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CC60, // $TODO write the function chuckle nuts
	.documentation = "true if the first player pushed forward on lookstick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_action_test_lookstick_backward_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_action_test_lookstick_backward",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CE50, // $TODO write the function chuckle nuts
	.documentation = "true if the first player pushed backward on lookstick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_teleport_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_teleport_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D240, // $TODO write the function chuckle nuts
	.documentation = "for testing: teleports one player to another's location\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) map_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "map_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D490, // $TODO write the function chuckle nuts
	.documentation = "starts the map from the beginning.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) map_reset_random_0_definition
{
	.return_type = _hs_type_void,
	.name = "map_reset_random",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D6E0, // $TODO write the function chuckle nuts
	.documentation = "starts the map from the beginning with a new random seed.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) switch_bsp_1_definition
{
	.return_type = _hs_type_void,
	.name = "switch_bsp",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D820, // $TODO write the function chuckle nuts
	.documentation = "switches to a different zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) switch_zone_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "switch_zone_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DDB0, // $TODO write the function chuckle nuts
	.documentation = "switches to a different zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_zone_set
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) current_zone_set_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "current_zone_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E040, // $TODO write the function chuckle nuts
	.documentation = "returns the current zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) current_zone_set_fully_active_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "current_zone_set_fully_active",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E440, // $TODO write the function chuckle nuts
	.documentation = "returns the current zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) switch_map_and_zone_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "switch_map_and_zone_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E6F0, // $TODO write the function chuckle nuts
	.documentation = "switches to a different scenario and zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) crash_1_definition
{
	.return_type = _hs_type_void,
	.name = "crash",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E970, // $TODO write the function chuckle nuts
	.documentation = "crashes (for debugging).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) version_0_definition
{
	.return_type = _hs_type_void,
	.name = "version",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EAB0, // $TODO write the function chuckle nuts
	.documentation = "prints the build version.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) status_0_definition
{
	.return_type = _hs_type_void,
	.name = "status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EE50, // $TODO write the function chuckle nuts
	.documentation = "prints the value of all global status variables.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) record_movie_4_definition
{
	.return_type = _hs_type_void,
	.name = "record_movie",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F0B0, // $TODO write the function chuckle nuts
	.documentation = "<frame rate> <seconds> <width> <height>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_real,
		_hs_type_short_integer,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) record_movie_distributed_5_definition
{
	.return_type = _hs_type_void,
	.name = "record_movie_distributed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F220, // $TODO write the function chuckle nuts
	.documentation = "<frame rate> <seconds> <screen size> <mod count> <mod index>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_long_integer,
		_hs_type_short_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) screenshot_1_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F590, // $TODO write the function chuckle nuts
	.documentation = "takes a screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) screenshot_debug_0_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_debug",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F740, // $TODO write the function chuckle nuts
	.documentation = "takes a screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) screenshot_big_2_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_big",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F9E0, // $TODO write the function chuckle nuts
	.documentation = "takes an NxN multiple-page screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) screenshot_big_raw_2_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_big_raw",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FC30, // $TODO write the function chuckle nuts
	.documentation = "takes an NxN multiple-page screenshot and saves as an HDR <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) screenshot_size_3_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_size",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FDA0, // $TODO write the function chuckle nuts
	.documentation = "takes a <width> x <height> screenshot and saves as an HDR <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) screenshot_simple_1_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_simple",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740160, // $TODO write the function chuckle nuts
	.documentation = "takes a simple screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) screenshot_cubemap_1_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_cubemap",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007402A0, // $TODO write the function chuckle nuts
	.documentation = "takes a cubemap screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) screenshot_webmap_1_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_webmap",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740590, // $TODO write the function chuckle nuts
	.documentation = "takes two special screenshots and saves them, along with the camera information, as <name>.tif, <name>_secondary.tif and <name>_camera.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) screenshot_unknown344_0_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_unknown344",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007407E0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) screenshot_capture_0_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_capture",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740A10, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) screenshot_set_path_1_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_set_path",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740BB0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) screenshot_unknown347_5_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_unknown347",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740FA0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) main_menu_0_definition
{
	.return_type = _hs_type_void,
	.name = "main_menu",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007413C0, // $TODO write the function chuckle nuts
	.documentation = "goes back to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) main_halt_0_definition
{
	.return_type = _hs_type_void,
	.name = "main_halt",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741740, // $TODO write the function chuckle nuts
	.documentation = "goes to a halted pregame state\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) map_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "map_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741960, // $TODO write the function chuckle nuts
	.documentation = "the same as game_start: launches a game for debugging purposes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_multiplayer_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_multiplayer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741B30, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the multiplayer engine for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_set_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_set_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741DF0, // $TODO write the function chuckle nuts
	.documentation = "Launches a game variant a on a map",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_splitscreen_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_splitscreen",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741F40, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the number of multiplayer splitscreen players for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_difficulty_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_difficulty",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742280, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the difficulty of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_game_difficulty
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_active_primary_skulls_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_active_primary_skulls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742540, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the active primary skulls of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_active_secondary_skulls_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_active_secondary_skulls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007426F0, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the active primary skulls of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_coop_players_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_coop_players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007429D0, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the number of coop players for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_initial_zone_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_initial_zone_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742CD0, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the initial bsp for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_tick_rate_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_tick_rate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742E30, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: sets the tick rate for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_start_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743050, // $TODO write the function chuckle nuts
	.documentation = "debug map launching: starts a game on the specified map.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_start_when_ready_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_start_when_ready",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743340, // $TODO write the function chuckle nuts
	.documentation = "nicely starts the current game once the pregame lobby is stable\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_start_when_joined_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_start_when_joined",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007435B0, // $TODO write the function chuckle nuts
	.documentation = "nicely starts a game when the specified number of users are in the lobby\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) game_rate_3_definition
{
	.return_type = _hs_type_void,
	.name = "game_rate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743730, // $TODO write the function chuckle nuts
	.documentation = "changes game update rate (DANGER: only use if you know what you're doing!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_cache_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_cache_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743CA0, // $TODO write the function chuckle nuts
	.documentation = "don't make me kick your ass\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) geometry_cache_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "geometry_cache_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743F50, // $TODO write the function chuckle nuts
	.documentation = "we fear change\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sound_cache_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "sound_cache_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744100, // $TODO write the function chuckle nuts
	.documentation = "i'm a rebel!\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) animation_cache_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "animation_cache_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007443A0, // $TODO write the function chuckle nuts
	.documentation = "it's just sooooo wrong\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) font_cache_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "font_cache_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007446C0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) language_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "language_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744910, // $TODO write the function chuckle nuts
	.documentation = "change the language for localization\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_cache_test_malloc_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_cache_test_malloc",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744AF0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_memory_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_memory",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744E20, // $TODO write the function chuckle nuts
	.documentation = "dumps memory leaks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_memory_by_file_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_memory_by_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745000, // $TODO write the function chuckle nuts
	.documentation = "dumps memory leaks by source file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_memory_for_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_memory_for_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745300, // $TODO write the function chuckle nuts
	.documentation = "dumps memory leaks from the specified source file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_tags_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_tags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745440, // $TODO write the function chuckle nuts
	.documentation = "writes all memory being used by tag files into tag_dump.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) tags_verify_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "tags_verify_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745700, // $TODO write the function chuckle nuts
	.documentation = "verifies usage of infidel fields is correct\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745A70, // $TODO write the function chuckle nuts
	.documentation = "toggles the profile display on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_set_thread_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_set_thread",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745BC0, // $TODO write the function chuckle nuts
	.documentation = "sets the thread index being profiled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_set_sort_method_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_set_sort_method",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745E20, // $TODO write the function chuckle nuts
	.documentation = "sets the sorting method used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_set_range_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_set_range",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007460B0, // $TODO write the function chuckle nuts
	.documentation = "sets the collection range used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_set_attribute_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_set_attribute",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007462C0, // $TODO write the function chuckle nuts
	.documentation = "sets an attribute to profile by name\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) trace_next_frame_1_definition
{
	.return_type = _hs_type_void,
	.name = "trace_next_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007464F0, // $TODO write the function chuckle nuts
	.documentation = "creates a tracedump of the next frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) trace_next_frame_to_file_2_definition
{
	.return_type = _hs_type_void,
	.name = "trace_next_frame_to_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746790, // $TODO write the function chuckle nuts
	.documentation = "creates a tracedump of the next frame in a specific file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) trace_tick_1_definition
{
	.return_type = _hs_type_void,
	.name = "trace_tick",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007469A0, // $TODO write the function chuckle nuts
	.documentation = "creates a tracedump of a specific game tick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) collision_log_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "collision_log_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746C60, // $TODO write the function chuckle nuts
	.documentation = "enables or disables collision logging.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) damage_control_get_1_definition
{
	.return_type = _hs_type_void,
	.name = "damage_control_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746EA0, // $TODO write the function chuckle nuts
	.documentation = "gets a damage control setting by string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) damage_control_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "damage_control_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747120, // $TODO write the function chuckle nuts
	.documentation = "sets a damage control setting by string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ai_lines_0_definition
{
	.return_type = _hs_type_void,
	.name = "ai_lines",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747270, // $TODO write the function chuckle nuts
	.documentation = "cycles through AI line-spray modes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ai_dialogue_break_on_vocalization_1_definition
{
	.return_type = _hs_type_void,
	.name = "ai_dialogue_break_on_vocalization",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747550, // $TODO write the function chuckle nuts
	.documentation = "break when the following vocalization is chosen for utterance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) fade_in_4_definition
{
	.return_type = _hs_type_void,
	.name = "fade_in",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747740, // $TODO write the function chuckle nuts
	.documentation = "does a screen fade in from a particular color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) fade_out_4_definition
{
	.return_type = _hs_type_void,
	.name = "fade_out",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747C60, // $TODO write the function chuckle nuts
	.documentation = "does a screen fade out to a particular color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748030, // $TODO write the function chuckle nuts
	.documentation = "initializes game to start a cinematic (interruptive) cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_stop_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007482E0, // $TODO write the function chuckle nuts
	.documentation = "initializes the game to end a cinematic (interruptive) cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_skip_start_internal_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_skip_start_internal",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748580, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_skip_stop_internal_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_skip_stop_internal",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007487E0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_show_letterbox_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_show_letterbox",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F650, // $TODO write the function chuckle nuts
	.documentation = "sets or removes the letterbox bars\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_show_letterbox_immediate_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_show_letterbox_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FB40, // $TODO write the function chuckle nuts
	.documentation = "sets or removes the letterbox bars\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_title_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_title",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FE20, // $TODO write the function chuckle nuts
	.documentation = "activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_cutscene_title
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_set_title_delayed_2_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_title_delayed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730290, // $TODO write the function chuckle nuts
	.documentation = "activates the chapter title, delayed by <real> seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_cutscene_title,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_suppress_bsp_object_creation_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_suppress_bsp_object_creation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730630, // $TODO write the function chuckle nuts
	.documentation = "suppresses or enables the automatic creation of objects during cutscenes due to a bsp switch\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_subtitle_2_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_subtitle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730AE0, // $TODO write the function chuckle nuts
	.documentation = "displays the named subtitle for <real> seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730E00, // $TODO write the function chuckle nuts
	.documentation = "sets the cinematic name for the current cinematic.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_cinematic_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_set_shot_2_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_shot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007310C0, // $TODO write the function chuckle nuts
	.documentation = "sets the scene and shot for the current cinematic.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_cinematic_scene_definition,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_get_shot_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_get_shot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731600, // $TODO write the function chuckle nuts
	.documentation = "Print the stored scene and shot information to the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_early_exit_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_early_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731760, // $TODO write the function chuckle nuts
	.documentation = "Used to store the state of skippable cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_get_early_exit_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "cinematic_get_early_exit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731B30, // $TODO write the function chuckle nuts
	.documentation = "Used to retrieve the state of skippable cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_active_camera_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_active_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731E80, // $TODO write the function chuckle nuts
	.documentation = "Used to change the rasterizer to display one of the texture cameras\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_object_create_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_object_create",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732110, // $TODO write the function chuckle nuts
	.documentation = "Creates an object for use by cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_object_create_cinematic_anchor_2_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_object_create_cinematic_anchor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732580, // $TODO write the function chuckle nuts
	.documentation = "Creates a cinematic anchor object for use by cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_object_destroy_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_object_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007328D0, // $TODO write the function chuckle nuts
	.documentation = "Destroys an object created by cinematic_object_create\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_destroy_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732CB0, // $TODO write the function chuckle nuts
	.documentation = "Releases everything created by the cinematics system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_clips_initialize_for_shot_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_clips_initialize_for_shot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732EC0, // $TODO write the function chuckle nuts
	.documentation = "intializes the cinematic clip planes for the given shot of the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_clips_destroy_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_clips_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733250, // $TODO write the function chuckle nuts
	.documentation = "destroys any existing cinematic clip planes for the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_lights_initialize_for_shot_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lights_initialize_for_shot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733440, // $TODO write the function chuckle nuts
	.documentation = "intializes the cinematic lighting for the given shot of the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_lights_destroy_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lights_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007338C0, // $TODO write the function chuckle nuts
	.documentation = "destroys any existing cinematic lights for the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_lights_destroy_shot_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lights_destroy_shot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733A90, // $TODO write the function chuckle nuts
	.documentation = "destroys any non-persistent existing cinematic lights for the current shot\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_light_object_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_light_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733CE0, // $TODO write the function chuckle nuts
	.documentation = "attaches cinematic lights to <object> at <marker> using scenario cinematic lighting palette entry <scenario-cinematic-lighting-name> from point of view of camera reference <camera>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_cinematic_lightprobe,
		_hs_type_cutscene_camera_point
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_light_object_off_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_light_object_off",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734260, // $TODO write the function chuckle nuts
	.documentation = "Turn off cinematic lighting for the object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_lighting_rebuild_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lighting_rebuild_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007344B0, // $TODO write the function chuckle nuts
	.documentation = "Rebuild all cinematic lighting (SH only)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) cinematic_lighting_update_dynamic_light_direction_5_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lighting_update_dynamic_light_direction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734760, // $TODO write the function chuckle nuts
	.documentation = "Updates the direction of a loaded cinematic dynamic light",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 16) cinematic_lighting_update_sh_light_8_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lighting_update_sh_light",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734CD0, // $TODO write the function chuckle nuts
	.documentation = "Updates the direction, intensity, and color of a loaded cinematic sh light",
	.parameters = NULL,
	.formal_parameter_count = 8,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_object_get_1_definition
{
	.return_type = _hs_type_object,
	.name = "cinematic_object_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007353E0, // $TODO write the function chuckle nuts
	.documentation = "Returns an object created by cinematic_object_create\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007357A0, // $TODO write the function chuckle nuts
	.documentation = "Resets the cinematics internal state\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) camera_set_briefing_1_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_briefing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735920, // $TODO write the function chuckle nuts
	.documentation = "Used for briefing cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_tag_reference_get_cinematic_1_definition
{
	.return_type = _hs_type_cinematic_definition,
	.name = "cinematic_tag_reference_get_cinematic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735DB0, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_tag_reference_get_scene_2_definition
{
	.return_type = _hs_type_cinematic_scene_definition,
	.name = "cinematic_tag_reference_get_scene",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007361E0, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_tag_reference_get_effect_4_definition
{
	.return_type = _hs_type_effect,
	.name = "cinematic_tag_reference_get_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736710, // $TODO write the function chuckle nuts
	.documentation = "Retrieve an effect tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_tag_reference_get_dialogue_4_definition
{
	.return_type = _hs_type_sound,
	.name = "cinematic_tag_reference_get_dialogue",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736BA0, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_tag_reference_get_music_4_definition
{
	.return_type = _hs_type_sound,
	.name = "cinematic_tag_reference_get_music",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737130, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_tag_reference_get_music_looping_4_definition
{
	.return_type = _hs_type_looping_sound,
	.name = "cinematic_tag_reference_get_music_looping",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737620, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a looping sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_tag_reference_get_animation_3_definition
{
	.return_type = _hs_type_animation_graph,
	.name = "cinematic_tag_reference_get_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737C30, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a looping sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_scripting_object_coop_flags_valid_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "cinematic_scripting_object_coop_flags_valid",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007381D0, // $TODO write the function chuckle nuts
	.documentation = "Evaluate if a cinematic object should be displayed for the current coop flags\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_fade_out_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_fade_out",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738800, // $TODO write the function chuckle nuts
	.documentation = "Fade out at the end of the cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_scripting_create_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738BE0, // $TODO write the function chuckle nuts
	.documentation = "Create a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_scripting_create_cinematic_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_cinematic_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738FE0, // $TODO write the function chuckle nuts
	.documentation = "Create a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_start_animation_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_start_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739510, // $TODO write the function chuckle nuts
	.documentation = "Animate a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_scripting_destroy_object_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_destroy_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739930, // $TODO write the function chuckle nuts
	.documentation = "Destroy a cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) cinematic_scripting_start_effect_5_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_start_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739DB0, // $TODO write the function chuckle nuts
	.documentation = "Play a cinematic effect\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_start_music_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_start_music",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A2A0, // $TODO write the function chuckle nuts
	.documentation = "Play a cinematic music\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) cinematic_scripting_start_dialogue_5_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_start_dialogue",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A770, // $TODO write the function chuckle nuts
	.documentation = "Play a cinematic dialogue\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_stop_music_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_stop_music",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ACC0, // $TODO write the function chuckle nuts
	.documentation = "Stops a cinematic music\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) cinematic_scripting_create_and_animate_object_5_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_and_animate_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B130, // $TODO write the function chuckle nuts
	.documentation = "Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) cinematic_scripting_create_and_animate_cinematic_object_5_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_and_animate_cinematic_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B6C0, // $TODO write the function chuckle nuts
	.documentation = "Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_create_and_animate_object_no_animation_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_and_animate_object_no_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BB60, // $TODO write the function chuckle nuts
	.documentation = "Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_create_and_animate_cinematic_object_no_animation_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_create_and_animate_cinematic_object_no_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C010, // $TODO write the function chuckle nuts
	.documentation = "Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_play_cortana_effect_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_play_cortana_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C470, // $TODO write the function chuckle nuts
	.documentation = "Play the associated cortana effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) attract_mode_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "attract_mode_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CA40, // $TODO write the function chuckle nuts
	.documentation = "starts an attract mode movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) attract_mode_set_seconds_1_definition
{
	.return_type = _hs_type_void,
	.name = "attract_mode_set_seconds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CC40, // $TODO write the function chuckle nuts
	.documentation = "sets number of seconds of the attract mode countdown timer.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_level_advance_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_level_advance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CE00, // $TODO write the function chuckle nuts
	.documentation = "transition game to another level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_won_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_won",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D190, // $TODO write the function chuckle nuts
	.documentation = "causes the player to successfully finish the current level and move to the next\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_lost_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_lost",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D440, // $TODO write the function chuckle nuts
	.documentation = "marks the game as lost or not lost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_revert_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_revert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D7A0, // $TODO write the function chuckle nuts
	.documentation = "causes the player to revert to his previous saved game (for testing and cinematic skipping only please!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_is_cooperative_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_is_cooperative",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D980, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the game is cooperative\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_is_playtest_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_is_playtest",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DDE0, // $TODO write the function chuckle nuts
	.documentation = "returns the hs global boolean 'global_playtest_mode' which can be set in your init.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_can_use_flashlights_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_can_use_flashlights",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E010, // $TODO write the function chuckle nuts
	.documentation = "allows or disallows the user of player flashlights\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_and_quit_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_and_quit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E480, // $TODO write the function chuckle nuts
	.documentation = "save & quit to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_unsafe_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_unsafe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E680, // $TODO write the function chuckle nuts
	.documentation = "saves right now, even if the game is in an immediate-loss state (NEVER USE THIS! EVER!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_insertion_point_unlock_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_insertion_point_unlock",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E900, // $TODO write the function chuckle nuts
	.documentation = "unlocks the given insertion point for the current map for all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_insertion_point_lock_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_insertion_point_lock",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EC70, // $TODO write the function chuckle nuts
	.documentation = "unlocks the given insertion point for the current map for all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_games_enumerate_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_games_enumerate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EF20, // $TODO write the function chuckle nuts
	.documentation = "test enumeration of saved game files on a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_games_delete_campaign_save_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_games_delete_campaign_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F1E0, // $TODO write the function chuckle nuts
	.documentation = "delete the campaign saved game file for a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_games_save_last_film_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_games_save_last_film",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F510, // $TODO write the function chuckle nuts
	.documentation = "test saving of last recorded film for a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saved_games_autosave_free_up_space_0_definition
{
	.return_type = _hs_type_void,
	.name = "saved_games_autosave_free_up_space",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F6E0, // $TODO write the function chuckle nuts
	.documentation = "tell the autosave queue to run its cleanup routine for each content type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) content_catalogue_display_status_3_definition
{
	.return_type = _hs_type_void,
	.name = "content_catalogue_display_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F970, // $TODO write the function chuckle nuts
	.documentation = "display the status of content catalogue for a controller, optionally showing only open content items\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) achievement_grant_to_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "achievement_grant_to_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FB80, // $TODO write the function chuckle nuts
	.documentation = "award named achievement to player\r\nNETWORK SAFE: Safe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) achievement_grant_to_all_players_1_definition
{
	.return_type = _hs_type_void,
	.name = "achievement_grant_to_all_players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FFA0, // $TODO write the function chuckle nuts
	.documentation = "award named achievement to all players\r\nNETWORK SAFE: Safe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) achievements_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "achievements_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740280, // $TODO write the function chuckle nuts
	.documentation = "enable or disable the awarding of achievements\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) achievements_skip_validation_checks_1_definition
{
	.return_type = _hs_type_void,
	.name = "achievements_skip_validation_checks",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740570, // $TODO write the function chuckle nuts
	.documentation = "toggle the disabling of validation checks on achievement awarding\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_influencers_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_influencers",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007407C0, // $TODO write the function chuckle nuts
	.documentation = "toggle debugging of any & all dynamic respawn influencers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_respawn_zones_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_respawn_zones",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007409F0, // $TODO write the function chuckle nuts
	.documentation = "debugs respawn zone influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_proximity_forbid_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_proximity_forbid",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740B90, // $TODO write the function chuckle nuts
	.documentation = "debugs enemy proximity forbid influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_moving_vehicle_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_moving_vehicle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740DE0, // $TODO write the function chuckle nuts
	.documentation = "debugs moving vehicle danger influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_weapon_influences_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_weapon_influences",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007411A0, // $TODO write the function chuckle nuts
	.documentation = "debugs enemy weapon danger influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_dangerous_projectiles_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_dangerous_projectiles",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007412D0, // $TODO write the function chuckle nuts
	.documentation = "debugs dangerous projectile influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_deployed_equipment_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_deployed_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007415D0, // $TODO write the function chuckle nuts
	.documentation = "debugs deployed equipment influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_proximity_enemy_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_proximity_enemy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007417A0, // $TODO write the function chuckle nuts
	.documentation = "debugs enemy proximity bias influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_teammates_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_teammates",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007419E0, // $TODO write the function chuckle nuts
	.documentation = "debugs teammate proximity bias influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_random_influence_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_random_influence",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741B90, // $TODO write the function chuckle nuts
	.documentation = "toggles display of random influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_nominal_weight_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_nominal_weight",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741E10, // $TODO write the function chuckle nuts
	.documentation = "toggles display of the nominal weight influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_natural_weight_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_natural_weight",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007420C0, // $TODO write the function chuckle nuts
	.documentation = "toggles display of the natural weight influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_target_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742370, // $TODO write the function chuckle nuts
	.documentation = "sets target player for respawn influencer debugging display\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_spawning_use_history_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_spawning_use_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007425D0, // $TODO write the function chuckle nuts
	.documentation = "sets the saved respawn state for respawn influencer debugging display\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_initial_spawn_point_objects_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_initial_spawn_point_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742730, // $TODO write the function chuckle nuts
	.documentation = "toggles debug cycling through multiplayer initial spawn points objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_respawn_point_objects_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_respawn_point_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742A70, // $TODO write the function chuckle nuts
	.documentation = "toggles debug cycling through multiplayer respawn points objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_export_variant_settings_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_export_variant_settings",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742D90, // $TODO write the function chuckle nuts
	.documentation = "export the current game engine variant settings to the specified text file\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_general_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_general",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742EB0, // $TODO write the function chuckle nuts
	.documentation = "test game engine general event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_flavor_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_flavor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743150, // $TODO write the function chuckle nuts
	.documentation = "test game engine flavor event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_slayer_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_slayer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743500, // $TODO write the function chuckle nuts
	.documentation = "test game engine slayer event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_ctf_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_ctf",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743640, // $TODO write the function chuckle nuts
	.documentation = "test game engine ctf event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_oddball_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_oddball",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007438A0, // $TODO write the function chuckle nuts
	.documentation = "test game engine oddball event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_king_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_king",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743C20, // $TODO write the function chuckle nuts
	.documentation = "test game engine king event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_vip_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_vip",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743EB0, // $TODO write the function chuckle nuts
	.documentation = "test game engine vip event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_juggernaut_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_juggernaut",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744010, // $TODO write the function chuckle nuts
	.documentation = "test game engine juggernaut event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_territories_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_territories",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744310, // $TODO write the function chuckle nuts
	.documentation = "test game engine territories event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_assault_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_assault",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744490, // $TODO write the function chuckle nuts
	.documentation = "test game engine assault event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_engine_event_test_infection_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_engine_event_test_infection",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744870, // $TODO write the function chuckle nuts
	.documentation = "test game engine infection event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) core_load_0_definition
{
	.return_type = _hs_type_void,
	.name = "core_load",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744A50, // $TODO write the function chuckle nuts
	.documentation = "loads debug game state from core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) core_load_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "core_load_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744C10, // $TODO write the function chuckle nuts
	.documentation = "loads debug game state from core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) core_save_0_definition
{
	.return_type = _hs_type_void,
	.name = "core_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744FE0, // $TODO write the function chuckle nuts
	.documentation = "saves debug game state to core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) core_save_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "core_save_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007452D0, // $TODO write the function chuckle nuts
	.documentation = "saves debug game state to core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) core_load_game_0_definition
{
	.return_type = _hs_type_void,
	.name = "core_load_game",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745520, // $TODO write the function chuckle nuts
	.documentation = "loads level and game state from core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) core_load_game_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "core_load_game_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007457B0, // $TODO write the function chuckle nuts
	.documentation = "loads level and game state from core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) core_regular_upload_to_debug_server_1_definition
{
	.return_type = _hs_type_void,
	.name = "core_regular_upload_to_debug_server",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745BA0, // $TODO write the function chuckle nuts
	.documentation = "toggle periodic core uploading to debug server\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) core_set_upload_option_1_definition
{
	.return_type = _hs_type_void,
	.name = "core_set_upload_option",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745CC0, // $TODO write the function chuckle nuts
	.documentation = "sets options for game state uploading (current options are 'default', 'repro', and 'stress'\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) force_debugger_not_present_1_definition
{
	.return_type = _hs_type_void,
	.name = "force_debugger_not_present",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746020, // $TODO write the function chuckle nuts
	.documentation = "forces is_debugger_present() to return false\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) force_debugger_always_present_1_definition
{
	.return_type = _hs_type_void,
	.name = "force_debugger_always_present",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007461F0, // $TODO write the function chuckle nuts
	.documentation = "forces is_debugger_present() to return true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_safe_to_save_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_safe_to_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746350, // $TODO write the function chuckle nuts
	.documentation = "returns FALSE if it would be a bad idea to save the player's game right now\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_safe_to_speak_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_safe_to_speak",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007466A0, // $TODO write the function chuckle nuts
	.documentation = "returns FALSE if it would be a bad idea to play mission dialog right now\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_all_quiet_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_all_quiet",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007469F0, // $TODO write the function chuckle nuts
	.documentation = "returns FALSE if there are bad guys around, projectiles in the air, etc.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746CD0, // $TODO write the function chuckle nuts
	.documentation = "checks to see if it is safe to save game, then saves (gives up after 8 seconds)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_cancel_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_cancel",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746F20, // $TODO write the function chuckle nuts
	.documentation = "cancels any pending game_save, timeout or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_no_timeout_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_no_timeout",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747190, // $TODO write the function chuckle nuts
	.documentation = "checks to see if it is safe to save game, then saves (this version never gives up)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_immediate_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_immediate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747350, // $TODO write the function chuckle nuts
	.documentation = "disregards player's current situation and saves (BE VERY CAREFUL!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_saving_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_saving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747650, // $TODO write the function chuckle nuts
	.documentation = "checks to see if the game is trying to save the map.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_reverted_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "game_reverted",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747890, // $TODO write the function chuckle nuts
	.documentation = "don't use this for anything, you black-hearted bastards.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sound_ui_screen_init_0_definition
{
	.return_type = _hs_type_void,
	.name = "sound_ui_screen_init",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747D70, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sound_ui_screen_term_0_definition
{
	.return_type = _hs_type_void,
	.name = "sound_ui_screen_term",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747F30, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 16) sound_set_tag_parameter_unsafe_8_definition
{
	.return_type = _hs_type_void,
	.name = "sound_set_tag_parameter_unsafe",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007480A0, // $TODO write the function chuckle nuts
	.documentation = "this is the second sign of the coming of the antichrist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 8,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_impulse_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748320, // $TODO write the function chuckle nuts
	.documentation = "your mom part 2.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) sound_impulse_trigger_4_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_trigger",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007486D0, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_impulse_start_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F860, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) sound_impulse_start_cinematic_5_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_cinematic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FD00, // $TODO write the function chuckle nuts
	.documentation = "<sound> <object> <scale> <3d gain> <first person gain> plays an impulse sound from the specified source object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) sound_impulse_start_effect_4_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730300, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale and effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) sound_impulse_start_with_subtitle_5_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_with_subtitle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730730, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale and displays a subtitle.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_impulse_language_time_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "sound_impulse_language_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730CD0, // $TODO write the function chuckle nuts
	.documentation = "returns the time remaining for the specified impulse sound. DO NOT CALL IN CUTSCENES.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_impulse_stop_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731050, // $TODO write the function chuckle nuts
	.documentation = "stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_impulse_start_3d_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_3d",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731520, // $TODO write the function chuckle nuts
	.documentation = "<sound> <azimuth> <scale> at the sound's minimum distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_impulse_mark_as_outro_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_mark_as_outro",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007318E0, // $TODO write the function chuckle nuts
	.documentation = "marks a sound as outro (meaning it plays past an unskipped cinematic).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_impulse_start_naked_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_naked",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731CB0, // $TODO write the function chuckle nuts
	.documentation = "<sound> <pitch range name> <permutation index name> through the speakers it was encoded for\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_looping_predict_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_predict",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731EE0, // $TODO write the function chuckle nuts
	.documentation = "your mom.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_looping_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_looping_start_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732180, // $TODO write the function chuckle nuts
	.documentation = "plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_looping_stop_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732690, // $TODO write the function chuckle nuts
	.documentation = "stops the specified looping sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_looping_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) sound_looping_stop_immediately_1_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_stop_immediately",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732960, // $TODO write the function chuckle nuts
	.documentation = "stops the specified looping sound immediately.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_looping_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_looping_set_scale_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_set_scale",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732D20, // $TODO write the function chuckle nuts
	.documentation = "changes the scale of the sound (which should affect the volume) within the range 0 to 1.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_looping_set_alternate_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_set_alternate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733180, // $TODO write the function chuckle nuts
	.documentation = "enables or disables the alternate loop/alternate end for a looping sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sound_loop_spam_0_definition
{
	.return_type = _hs_type_void,
	.name = "sound_loop_spam",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733500, // $TODO write the function chuckle nuts
	.documentation = "start all loaded looping sounds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_class_show_channel_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_class_show_channel",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733790, // $TODO write the function chuckle nuts
	.documentation = "shows/hides sound classes w/ substring in debug_sound_channels view\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_class_debug_sound_start_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_class_debug_sound_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733A00, // $TODO write the function chuckle nuts
	.documentation = "shows/hides when sounds of sound classes w/ substring start\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_sounds_enable_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sounds_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733B40, // $TODO write the function chuckle nuts
	.documentation = "enables or disables all sound classes matching the substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_class_set_gain_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_class_set_gain",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733EB0, // $TODO write the function chuckle nuts
	.documentation = "changes the gain on the specified sound class(es) to the specified gain over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_class_set_gain_db_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_class_set_gain_db",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734290, // $TODO write the function chuckle nuts
	.documentation = "changes the gain on the specified sound class(es) to the specified gain(dB) over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_real,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_class_enable_ducker_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_class_enable_ducker",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734630, // $TODO write the function chuckle nuts
	.documentation = "enables or disables the ducker on all sound classes matching the substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_sound_environment_parameter_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_environment_parameter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734B70, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_set_global_effect_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_set_global_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734D90, // $TODO write the function chuckle nuts
	.documentation = "bleh\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) sound_set_global_effect_scale_2_definition
{
	.return_type = _hs_type_void,
	.name = "sound_set_global_effect_scale",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735180, // $TODO write the function chuckle nuts
	.documentation = "more bleh\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) vehicle_auto_turret_5_definition
{
	.return_type = _hs_type_void,
	.name = "vehicle_auto_turret",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735550, // $TODO write the function chuckle nuts
	.documentation = "Sets the specified trigger volume and parameters for an auto turret\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_trigger_volume,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) vehicle_hover_2_definition
{
	.return_type = _hs_type_void,
	.name = "vehicle_hover",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735B50, // $TODO write the function chuckle nuts
	.documentation = "stops the vehicle from running real physics and runs fake hovering physics instead.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_vehicle,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_count_bipeds_killed_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "vehicle_count_bipeds_killed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735F60, // $TODO write the function chuckle nuts
	.documentation = "returns how many people this vehicle has killed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_vehicle
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) biped_ragdoll_1_definition
{
	.return_type = _hs_type_void,
	.name = "biped_ragdoll",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736360, // $TODO write the function chuckle nuts
	.documentation = "given a dead biped, turns on ragdoll\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) water_float_reset_1_definition
{
	.return_type = _hs_type_void,
	.name = "water_float_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007366E0, // $TODO write the function chuckle nuts
	.documentation = "allows an object to continue floating",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_show_training_text_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_show_training_text",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007369D0, // $TODO write the function chuckle nuts
	.documentation = "true turns on scripted training text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_set_training_text_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_set_training_text",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736E10, // $TODO write the function chuckle nuts
	.documentation = "sets the string id fo the scripted training text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) hud_enable_training_1_definition
{
	.return_type = _hs_type_void,
	.name = "hud_enable_training",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737100, // $TODO write the function chuckle nuts
	.documentation = "true turns training on, false turns it off.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_training_activate_flashlight_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_activate_flashlight",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007374C0, // $TODO write the function chuckle nuts
	.documentation = "guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_training_activate_crouch_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_activate_crouch",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737600, // $TODO write the function chuckle nuts
	.documentation = "guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_training_activate_stealth_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_activate_stealth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737A10, // $TODO write the function chuckle nuts
	.documentation = "guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_training_activate_equipment_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_activate_equipment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737B70, // $TODO write the function chuckle nuts
	.documentation = "guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_training_activate_jump_0_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_activate_jump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737D90, // $TODO write the function chuckle nuts
	.documentation = "guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_training_reset_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_training_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738110, // $TODO write the function chuckle nuts
	.documentation = "Resets all player training data\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) hud_activate_team_nav_point_flag_3_definition
{
	.return_type = _hs_type_void,
	.name = "hud_activate_team_nav_point_flag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738370, // $TODO write the function chuckle nuts
	.documentation = "activates a nav point type <string> attached to a team anchored to a flag with a vertical offset <real>. If the player is not local to the machine, this will fail\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_cutscene_flag,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) hud_deactivate_team_nav_point_flag_2_definition
{
	.return_type = _hs_type_void,
	.name = "hud_deactivate_team_nav_point_flag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738860, // $TODO write the function chuckle nuts
	.documentation = "deactivates a nav point type attached to a team anchored to a flag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_cutscene_flag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) chud_cortana_suck_3_definition
{
	.return_type = _hs_type_void,
	.name = "chud_cortana_suck",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738B80, // $TODO write the function chuckle nuts
	.documentation = "turns on the cortana suck, attached to the object at the given marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_texture_cam_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_texture_cam",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738F80, // $TODO write the function chuckle nuts
	.documentation = "turns the chud texturecam on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_cortana_set_range_multiplier_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_cortana_set_range_multiplier",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739310, // $TODO write the function chuckle nuts
	.documentation = "scales the cortana effect distances\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) play_cortana_effect_1_definition
{
	.return_type = _hs_type_void,
	.name = "play_cortana_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739720, // $TODO write the function chuckle nuts
	.documentation = "scales the cortana effect distances\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739A40, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the entire chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_weapon_stats_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_weapon_stats",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739E40, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the weapon huds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_crosshair_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_crosshair",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A0E0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the crosshair\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_shield_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_shield",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A560, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the shield\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_grenades_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A880, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the grenade hud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_messages_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_messages",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ABE0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the chud messages\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_motion_sensor_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_motion_sensor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AFD0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the motion sensor chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_spike_grenades_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_spike_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B2A0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the spike grenade chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_fire_grenades_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_fire_grenades",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B730, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the firebomb chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_compass_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_compass",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B9F0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the compass chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_stamina_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_stamina",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BDE0, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the stamina chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_energy_meters_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_energy_meters",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C110, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the energy meters chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_show_consumables_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_consumables",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C440, // $TODO write the function chuckle nuts
	.documentation = "hides/shows the consumables chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) chud_cinematic_fade_2_definition
{
	.return_type = _hs_type_void,
	.name = "chud_cinematic_fade",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C7B0, // $TODO write the function chuckle nuts
	.documentation = "parameter 1 is how, parameter 2 is when\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_bonus_round_show_timer_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_bonus_round_show_timer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CC90, // $TODO write the function chuckle nuts
	.documentation = "show the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_bonus_round_start_timer_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_bonus_round_start_timer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D020, // $TODO write the function chuckle nuts
	.documentation = "tick the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_bonus_round_set_timer_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_bonus_round_set_timer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D380, // $TODO write the function chuckle nuts
	.documentation = "set the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cls_0_definition
{
	.return_type = _hs_type_void,
	.name = "cls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D6C0, // $TODO write the function chuckle nuts
	.documentation = "clears console text from the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) events_spam_suppression_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "events_spam_suppression_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D800, // $TODO write the function chuckle nuts
	.documentation = "enables or disables the suppression of event spamming\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) error_geometry_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "error_geometry_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DBF0, // $TODO write the function chuckle nuts
	.documentation = "highlights all error geometry with a name that includes the given substring\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) error_geometry_hide_1_definition
{
	.return_type = _hs_type_void,
	.name = "error_geometry_hide",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DE70, // $TODO write the function chuckle nuts
	.documentation = "hides all error geometry with a name that includes the given substring\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) error_geometry_show_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "error_geometry_show_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DFF0, // $TODO write the function chuckle nuts
	.documentation = "highlights all error geometry\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) error_geometry_hide_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "error_geometry_hide_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E420, // $TODO write the function chuckle nuts
	.documentation = "hides all error geometry\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) error_geometry_list_0_definition
{
	.return_type = _hs_type_void,
	.name = "error_geometry_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E500, // $TODO write the function chuckle nuts
	.documentation = "prints out a list of all error geometry types and counts\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_effect_set_max_translation_3_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_translation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E730, // $TODO write the function chuckle nuts
	.documentation = "<x> <y> <z>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_effect_set_max_rotation_3_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EBD0, // $TODO write the function chuckle nuts
	.documentation = "<yaw> <pitch> <roll>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_effect_set_max_rumble_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_rumble",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F060, // $TODO write the function chuckle nuts
	.documentation = "<left> <right>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_effect_start_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F4A0, // $TODO write the function chuckle nuts
	.documentation = "<max_intensity> <attack time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_effect_stop_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F850, // $TODO write the function chuckle nuts
	.documentation = "<decay>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) player_effect_set_max_translation_for_player_4_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_translation_for_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FB20, // $TODO write the function chuckle nuts
	.documentation = "<x> <y> <z>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) player_effect_set_max_rotation_for_player_4_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_rotation_for_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FFD0, // $TODO write the function chuckle nuts
	.documentation = "<yaw> <pitch> <roll>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_effect_set_max_rumble_for_player_3_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_set_max_rumble_for_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740440, // $TODO write the function chuckle nuts
	.documentation = "<left> <right>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_effect_start_for_player_3_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_start_for_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007408C0, // $TODO write the function chuckle nuts
	.documentation = "<max_intensity> <attack time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_effect_stop_for_player_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_effect_stop_for_player",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740C90, // $TODO write the function chuckle nuts
	.documentation = "<decay>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) time_code_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "time_code_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007411E0, // $TODO write the function chuckle nuts
	.documentation = "shows the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) time_code_start_1_definition
{
	.return_type = _hs_type_void,
	.name = "time_code_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741350, // $TODO write the function chuckle nuts
	.documentation = "starts/stops the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) time_code_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "time_code_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741610, // $TODO write the function chuckle nuts
	.documentation = "resets the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) script_screen_effect_set_value_2_definition
{
	.return_type = _hs_type_void,
	.name = "script_screen_effect_set_value",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741830, // $TODO write the function chuckle nuts
	.documentation = "sets a screen effect script value\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_screen_effect_start_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_screen_effect_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741AB0, // $TODO write the function chuckle nuts
	.documentation = "starts screen effect pass TRUE to clear\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_screen_effect_set_crossfade_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_screen_effect_set_crossfade",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741C50, // $TODO write the function chuckle nuts
	.documentation = "transition-time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) cinematic_screen_effect_set_crossfade_2_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_screen_effect_set_crossfade",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741E50, // $TODO write the function chuckle nuts
	.documentation = "transition-time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_screen_effect_stop_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_screen_effect_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007421D0, // $TODO write the function chuckle nuts
	.documentation = "returns control of the screen effects to the rest of the game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_near_clip_distance_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_near_clip_distance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007423E0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_far_clip_distance_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_far_clip_distance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742650, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_atmosphere_fog_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_atmosphere_fog",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007428A0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable atmosphere fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) atmosphere_fog_override_fade_3_definition
{
	.return_type = _hs_type_void,
	.name = "atmosphere_fog_override_fade",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742C80, // $TODO write the function chuckle nuts
	.documentation = "override the atmosphere fog setting to <index> by <percentage> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) motion_blur_1_definition
{
	.return_type = _hs_type_void,
	.name = "motion_blur",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742FD0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable motion blur\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_weather_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_weather",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007434D0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable atmosphere fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_patchy_fog_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_patchy_fog",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743700, // $TODO write the function chuckle nuts
	.documentation = "enable/disable patchy fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_set_environment_map_attenuation_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_environment_map_attenuation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743C00, // $TODO write the function chuckle nuts
	.documentation = "interpolates environment-map attenuation (on flagged shaders) from <low> to <high> over <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_environment_map_bitmap_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_environment_map_bitmap",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743E90, // $TODO write the function chuckle nuts
	.documentation = "sets environment-map bitmap (on flagged shaders) instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_bitmap
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_reset_environment_map_bitmap_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_reset_environment_map_bitmap",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743FF0, // $TODO write the function chuckle nuts
	.documentation = "resets environment-map bitmap (on flagged shaders) to default instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 16) cinematic_set_environment_map_tint_8_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_environment_map_tint",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007442F0, // $TODO write the function chuckle nuts
	.documentation = "perpendicular color: (red green blue brightness), parallel color: (red green blue brightness)... sets environment-map tint (on flagged shaders) instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 8,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_reset_environment_map_tint_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_reset_environment_map_tint",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744470, // $TODO write the function chuckle nuts
	.documentation = "resets environment-map tint (on flagged shaders) to default instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_layer_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_layer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744850, // $TODO write the function chuckle nuts
	.documentation = "interpolates the value of <cinematic layer x> from current position to <value> over <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) player_has_skills_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "player_has_skills",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744A20, // $TODO write the function chuckle nuts
	.documentation = "Are you the best?\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_has_mad_secret_skills_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_has_mad_secret_skills",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744CC0, // $TODO write the function chuckle nuts
	.documentation = "You are the best!\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) controller_invert_look_0_definition
{
	.return_type = _hs_type_void,
	.name = "controller_invert_look",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745110, // $TODO write the function chuckle nuts
	.documentation = "invert look on all attached controllers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_look_speed_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_look_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745370, // $TODO write the function chuckle nuts
	.documentation = "set look speed for all attached controllers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_set_look_invert_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_look_invert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007454C0, // $TODO write the function chuckle nuts
	.documentation = "invert player0's look\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) controller_get_look_invert_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "controller_get_look_invert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745A00, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if player0's look pitch is inverted\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_unlock_solo_levels_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "controller_unlock_solo_levels",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745C40, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_look_inverted_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_look_inverted",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746090, // $TODO write the function chuckle nuts
	.documentation = "set look inversion for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_vibration_enabled_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_vibration_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007462A0, // $TODO write the function chuckle nuts
	.documentation = "set vibration for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_flight_stick_aircraft_controls_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_flight_stick_aircraft_controls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007464D0, // $TODO write the function chuckle nuts
	.documentation = "set airrcraft flight stick controls for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_auto_center_look_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_auto_center_look",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746770, // $TODO write the function chuckle nuts
	.documentation = "set auto center look for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_crouch_lock_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_crouch_lock",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746980, // $TODO write the function chuckle nuts
	.documentation = "set crouch lock for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_button_preset_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_button_preset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746C40, // $TODO write the function chuckle nuts
	.documentation = "set button preset for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_button_preset
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_joystick_preset_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_joystick_preset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746E80, // $TODO write the function chuckle nuts
	.documentation = "set joystick preset for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_joystick_preset
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_look_sensitivity_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_look_sensitivity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747100, // $TODO write the function chuckle nuts
	.documentation = "set look sensitivity for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_unlock_single_player_levels_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_unlock_single_player_levels",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747250, // $TODO write the function chuckle nuts
	.documentation = "<controller> unlock all single player levels for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_lock_single_player_levels_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_lock_single_player_levels",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747530, // $TODO write the function chuckle nuts
	.documentation = "<controller> lock all single player levels for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_unlock_skulls_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_unlock_skulls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747720, // $TODO write the function chuckle nuts
	.documentation = "<controller> unlock all campaign skulls for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_lock_skulls_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_lock_skulls",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747930, // $TODO write the function chuckle nuts
	.documentation = "<controller> lock all campaign skulls for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_unlock_models_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_unlock_models",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747C40, // $TODO write the function chuckle nuts
	.documentation = "<controller> unlock all model permutations for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_lock_models_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_lock_models",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747EF0, // $TODO write the function chuckle nuts
	.documentation = "<controller> lock all model permutations for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) controller_set_single_player_level_completed_5_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_single_player_level_completed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748010, // $TODO write the function chuckle nuts
	.documentation = "<controller> <level index> <co-op> <difficulty> <completed> set the single player levels completed state for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer,
		_hs_type_boolean,
		_hs_type_game_difficulty,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_player_character_type_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_player_character_type",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007482C0, // $TODO write the function chuckle nuts
	.documentation = "set player character type for specified controller",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_player_character_type
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) controller_set_emblem_info_5_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_emblem_info",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748560, // $TODO write the function chuckle nuts
	.documentation = "set emblem for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_voice_output_setting_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_voice_output_setting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007487C0, // $TODO write the function chuckle nuts
	.documentation = "set voice output setting for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_voice_output_setting
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_voice_mask_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_voice_mask",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F630, // $TODO write the function chuckle nuts
	.documentation = "set voice mask for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_voice_mask
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_subtitle_setting_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_subtitle_setting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FA10, // $TODO write the function chuckle nuts
	.documentation = "set subtitle setting for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_subtitle_setting
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) controller_set_campaign_solo_high_score_3_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_campaign_solo_high_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FC20, // $TODO write the function chuckle nuts
	.documentation = "<controller> <map_id> <score> set high score for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) controller_set_campaign_coop_high_score_4_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_campaign_coop_high_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FE00, // $TODO write the function chuckle nuts
	.documentation = "<controller> <map_id> <score> <player_count> set high score for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) controller_set_survival_solo_high_score_4_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_survival_solo_high_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007300F0, // $TODO write the function chuckle nuts
	.documentation = "<controller> <map_id> <insertion point> <score> set high score for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer,
		_hs_type_short_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) controller_set_survival_coop_high_score_5_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_survival_coop_high_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007303F0, // $TODO write the function chuckle nuts
	.documentation = "<controller> <map_id> <insertion point> <score> <player_count> set high score for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer,
		_hs_type_short_integer,
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_clear_high_scores_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_clear_high_scores",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007305B0, // $TODO write the function chuckle nuts
	.documentation = "clear high scores for specified controller\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_unsignedin_user_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_unsignedin_user",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007307A0, // $TODO write the function chuckle nuts
	.documentation = "Toggle controller as unsigned-in user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) controller_display_storage_device_selection_1_definition
{
	.return_type = _hs_type_void,
	.name = "controller_display_storage_device_selection",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730B60, // $TODO write the function chuckle nuts
	.documentation = "display storage device selection ui for the specified controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) font_cache_bitmap_save_1_definition
{
	.return_type = _hs_type_void,
	.name = "font_cache_bitmap_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730D50, // $TODO write the function chuckle nuts
	.documentation = "save font cache bitmap to targa file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) ui_debug_load_main_menu_0_definition
{
	.return_type = _hs_type_void,
	.name = "ui_debug_load_main_menu",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730FD0, // $TODO write the function chuckle nuts
	.documentation = "loads the main menu screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_debug_text_bounds_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_debug_text_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007311D0, // $TODO write the function chuckle nuts
	.documentation = "toggle rendering of ui text boundaries\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_debug_text_font_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_debug_text_font",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731480, // $TODO write the function chuckle nuts
	.documentation = "toggle display of ui text font\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_debug_show_title_safe_bounds_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_debug_show_title_safe_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731720, // $TODO write the function chuckle nuts
	.documentation = "toggle display of title safe boundary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_debug_element_bounds_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_debug_element_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007318C0, // $TODO write the function chuckle nuts
	.documentation = "toggle rendering of widget tag block bounds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_memory_dump_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_memory_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731B10, // $TODO write the function chuckle nuts
	.documentation = "dump the UI memory tracked allocations to a specified file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ui_time_scale_step_1_definition
{
	.return_type = _hs_type_void,
	.name = "ui_time_scale_step",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731D90, // $TODO write the function chuckle nuts
	.documentation = "move the ui time forward by a specified amount on next tick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) xoverlapped_debug_render_1_definition
{
	.return_type = _hs_type_void,
	.name = "xoverlapped_debug_render",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731F50, // $TODO write the function chuckle nuts
	.documentation = "toggle display a list of active xoverlapped tasks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_load_screen_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_load_screen",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732310, // $TODO write the function chuckle nuts
	.documentation = "load the named screen\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gui_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "gui_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732610, // $TODO write the function chuckle nuts
	.documentation = "cleans out the halox ui screens and errors\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gui_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "gui_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732820, // $TODO write the function chuckle nuts
	.documentation = "tells the window location manager to resume\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gui_stop_0_definition
{
	.return_type = _hs_type_void,
	.name = "gui_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732A60, // $TODO write the function chuckle nuts
	.documentation = "tells the window location manager to stop bringing up screens on its own\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_error_post_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_error_post",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732C90, // $TODO write the function chuckle nuts
	.documentation = "<error_code> <controller_index> <must_be_cleared> sets an error condition in the ui\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_error_post_toast_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_error_post_toast",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732EA0, // $TODO write the function chuckle nuts
	.documentation = "<error_code> <controller_index> <must_be_cleared> sets an error condition in the ui\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_error_resolve_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_error_resolve",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733160, // $TODO write the function chuckle nuts
	.documentation = "<error_name> <controller_index> resolves an error condition in the ui\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_error_clear_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_error_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733310, // $TODO write the function chuckle nuts
	.documentation = "<error_name> <controller_index> clears an error condition in the ui\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_dialog_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_dialog_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733650, // $TODO write the function chuckle nuts
	.documentation = "<dialog name>\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_spartan_milestone_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_spartan_milestone_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007338A0, // $TODO write the function chuckle nuts
	.documentation = "show dialog for milestone upgrade (1..3)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_spartan_rank_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_spartan_rank_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733A70, // $TODO write the function chuckle nuts
	.documentation = "show toast for rank upgrade (1..36)\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gui_print_active_screens_0_definition
{
	.return_type = _hs_type_void,
	.name = "gui_print_active_screens",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733CC0, // $TODO write the function chuckle nuts
	.documentation = "display list of active halox ui screens\r\nNETWORK SAFEL No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gui_print_active_screen_strings_0_definition
{
	.return_type = _hs_type_void,
	.name = "gui_print_active_screen_strings",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733F10, // $TODO write the function chuckle nuts
	.documentation = "display strings tag file name for current topmost screen\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_screen_name_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_screen_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734240, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given screen's name, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_screen_animation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_screen_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007343E0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given screen animations, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_screen_bounds_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_screen_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007345E0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given screen's bounds, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_screen_rotation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_screen_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734950, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given screen's rotation, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_group_name_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_group_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734B50, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given group's name, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_group_animation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_group_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734D70, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given group's animations, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_group_bounds_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_group_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735010, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given group's bounds, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_group_rotation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_group_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735310, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given group's rotation, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_name_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735530, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list's name, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_animation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735780, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list's animations, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_bounds_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735900, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list's bounds, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_rotation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735CD0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list's rotation, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_item_name_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_item_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735EF0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list item's name, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_item_animation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_item_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736050, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list item's animation, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_item_bounds_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_item_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736340, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list item's bounds, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) gui_debug_list_item_rotation_3_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_list_item_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736510, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given list item's rotation, optionally recursive\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_text_name_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_text_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007367B0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given text widget's name\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_text_animation_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_text_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007369B0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given text widget's animation state\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_text_bounds_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_text_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736C60, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given text widget's bounds\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_text_rotation_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_text_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736F00, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given text widget's rotation\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_bitmap_name_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_bitmap_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007370E0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given bitmap widget's name\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_bitmap_animation_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_bitmap_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737380, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given bitmap widget's animation state\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_bitmap_bounds_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_bitmap_bounds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737560, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given bitmap widget's bounds\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gui_debug_bitmap_rotation_2_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_bitmap_rotation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007378F0, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of given bitmap widget's rotation\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_debug_music_state_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_debug_music_state",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737A70, // $TODO write the function chuckle nuts
	.documentation = "Toggle display of UI music state\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cc_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "cc_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737C70, // $TODO write the function chuckle nuts
	.documentation = "toggle closed captions\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cc_test_1_definition
{
	.return_type = _hs_type_void,
	.name = "cc_test",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007380F0, // $TODO write the function chuckle nuts
	.documentation = "test a line of cc text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) objectives_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738350, // $TODO write the function chuckle nuts
	.documentation = "clears the mission objectives.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_show_up_to_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_show_up_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738450, // $TODO write the function chuckle nuts
	.documentation = "show primary campaign objectives 0..n\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_finish_up_to_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_finish_up_to",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007388C0, // $TODO write the function chuckle nuts
	.documentation = "mark primary campaign objectives 0..n as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738C40, // $TODO write the function chuckle nuts
	.documentation = "show secondary campaign objective N\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_finish_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_finish",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738FB0, // $TODO write the function chuckle nuts
	.documentation = "mark secondary campaign objective N as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_unavailable_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_unavailable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007393D0, // $TODO write the function chuckle nuts
	.documentation = "mark secondary campaign objective N as unavailable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_secondary_show_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_secondary_show",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739750, // $TODO write the function chuckle nuts
	.documentation = "show secondary campaign objective N\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_secondary_finish_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_secondary_finish",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739A70, // $TODO write the function chuckle nuts
	.documentation = "mark secondary campaign objective N as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) objectives_secondary_unavailable_1_definition
{
	.return_type = _hs_type_void,
	.name = "objectives_secondary_unavailable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739EF0, // $TODO write the function chuckle nuts
	.documentation = "mark secondary campaign objective N as unavailable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) input_suppress_rumble_1_definition
{
	.return_type = _hs_type_void,
	.name = "input_suppress_rumble",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A130, // $TODO write the function chuckle nuts
	.documentation = "disable the friggin' rumble\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) input_disable_claw_button_combos_1_definition
{
	.return_type = _hs_type_void,
	.name = "input_disable_claw_button_combos",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A5D0, // $TODO write the function chuckle nuts
	.documentation = "disabe the Claw button combos (for monkey testing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) update_remote_camera_0_definition
{
	.return_type = _hs_type_void,
	.name = "update_remote_camera",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A7B0, // $TODO write the function chuckle nuts
	.documentation = "force synchronization of remote machine camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_build_network_config_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_build_network_config",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A9E0, // $TODO write the function chuckle nuts
	.documentation = "writes a new network configuration file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_build_game_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_build_game_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ACA0, // $TODO write the function chuckle nuts
	.documentation = "writes the current game variant to a file\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_verify_game_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_verify_game_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AF90, // $TODO write the function chuckle nuts
	.documentation = "verifies the contents of a packed game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_load_and_use_game_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_load_and_use_game_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B110, // $TODO write the function chuckle nuts
	.documentation = "loads the contents of a packed game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_use_hopper_directory_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_use_hopper_directory",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B300, // $TODO write the function chuckle nuts
	.documentation = "set which hopper directory to use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_quality_dump_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B490, // $TODO write the function chuckle nuts
	.documentation = "dump all network quality statistics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_quality_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B8D0, // $TODO write the function chuckle nuts
	.documentation = "clear all network quality statistics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_connection_badness_history_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_connection_badness_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BA20, // $TODO write the function chuckle nuts
	.documentation = "set connection history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_squad_host_badness_history_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_squad_host_badness_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BC80, // $TODO write the function chuckle nuts
	.documentation = "set host badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_squad_client_badness_history_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_squad_client_badness_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BF90, // $TODO write the function chuckle nuts
	.documentation = "set client badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_group_host_badness_history_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_group_host_badness_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C190, // $TODO write the function chuckle nuts
	.documentation = "set host badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_group_client_badness_history_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_group_client_badness_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C3C0, // $TODO write the function chuckle nuts
	.documentation = "set client badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_quality_set_estimated_bandwidth_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_quality_set_estimated_bandwidth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C5A0, // $TODO write the function chuckle nuts
	.documentation = "set upstream bandwidth history, <is reliable> <bandwidth in kbps>\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_join_friend_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_join_friend",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C800, // $TODO write the function chuckle nuts
	.documentation = "join a friend's squad session, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_join_squad_to_friend_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_join_squad_to_friend",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CB50, // $TODO write the function chuckle nuts
	.documentation = "join our squad to a friend's squad session, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_join_sessionid_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_join_sessionid",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CD10, // $TODO write the function chuckle nuts
	.documentation = "join a squad session by sessionid, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_join_squad_to_sessionid_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_join_squad_to_sessionid",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D000, // $TODO write the function chuckle nuts
	.documentation = "join a squad session by sessionid, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_enable_join_friend_loop_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_enable_join_friend_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D220, // $TODO write the function chuckle nuts
	.documentation = "randomly join a friend every <param> seconds, 0 to reset\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_set_maximum_player_count_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_set_maximum_player_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D470, // $TODO write the function chuckle nuts
	.documentation = "sets the maximum player count for this squad\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_set_campaign_insertion_point_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_set_campaign_insertion_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D6A0, // $TODO write the function chuckle nuts
	.documentation = "sets the campaign insertion point for this squad\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_status_filter_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_status_filter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D7E0, // $TODO write the function chuckle nuts
	.documentation = "filters the set of network status to display\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_sim_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_sim_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DBD0, // $TODO write the function chuckle nuts
	.documentation = "network simulation: resets the simulation state\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_sim_spike_now_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_sim_spike_now",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DE50, // $TODO write the function chuckle nuts
	.documentation = "network simulation: starts a latency spike immediately\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_sim_dropspike_now_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_sim_dropspike_now",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DFD0, // $TODO write the function chuckle nuts
	.documentation = "network simulation: starts a packet loss spike immediately\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_ping_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_ping",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E400, // $TODO write the function chuckle nuts
	.documentation = "network test: sends a ping\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_channel_delete_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_channel_delete",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E4E0, // $TODO write the function chuckle nuts
	.documentation = "network test: deletes all channels\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_delegate_host_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_delegate_host",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E710, // $TODO write the function chuckle nuts
	.documentation = "network test: delegate host to the specified player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_delegate_leader_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_delegate_leader",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EA50, // $TODO write the function chuckle nuts
	.documentation = "network test: delegate leadership to the specified player\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_map_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_map_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EC50, // $TODO write the function chuckle nuts
	.documentation = "network test: sets the name of the scenario to play\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_campaign_difficulty_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_campaign_difficulty",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EE70, // $TODO write the function chuckle nuts
	.documentation = "network test: sets the difficulty of the campaign game to play\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_reset_objects_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_reset_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F160, // $TODO write the function chuckle nuts
	.documentation = "network test: resets all objects on the map\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_fatal_error_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_fatal_error",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F3B0, // $TODO write the function chuckle nuts
	.documentation = "creates a fatal simulation error\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_set_machine_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_set_machine_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F5B0, // $TODO write the function chuckle nuts
	.documentation = "sets the nickname of your xbox\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) events_enabled_1_definition
{
	.return_type = _hs_type_void,
	.name = "events_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F830, // $TODO write the function chuckle nuts
	.documentation = "enable/disable all events\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) events_disable_suppression_1_definition
{
	.return_type = _hs_type_void,
	.name = "events_disable_suppression",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FA40, // $TODO write the function chuckle nuts
	.documentation = "enable/disable event suppression\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_global_display_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_global_display_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x6D3A9CBA, // $TODO write the function chuckle nuts
	.documentation = "sets the global event display level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_global_log_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_global_log_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x6D3A6DDF, // $TODO write the function chuckle nuts
	.documentation = "sets the global evetn log level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_global_remote_log_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_global_remote_log_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007401A0, // $TODO write the function chuckle nuts
	.documentation = "sets the global remote evetn log level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_display_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_display_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740360, // $TODO write the function chuckle nuts
	.documentation = "sets the display level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_force_display_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_force_display_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007405F0, // $TODO write the function chuckle nuts
	.documentation = "sets the forced display level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_log_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_log_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740860, // $TODO write the function chuckle nuts
	.documentation = "sets the log level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_remote_log_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_remote_log_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740A90, // $TODO write the function chuckle nuts
	.documentation = "sets the remote log level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_debugger_break_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_debugger_break_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740C40, // $TODO write the function chuckle nuts
	.documentation = "sets the debugger break level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) event_halt_category_2_definition
{
	.return_type = _hs_type_void,
	.name = "event_halt_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740E80, // $TODO write the function chuckle nuts
	.documentation = "sets the halt (assert/crash) level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_event
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_list_categories_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_list_categories",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741240, // $TODO write the function chuckle nuts
	.documentation = "lists all categories that exist under a particular category string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) events_suppress_console_display_1_definition
{
	.return_type = _hs_type_void,
	.name = "events_suppress_console_display",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007413A0, // $TODO write the function chuckle nuts
	.documentation = "disables console display for events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) play_bink_movie_1_definition
{
	.return_type = _hs_type_void,
	.name = "play_bink_movie",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741630, // $TODO write the function chuckle nuts
	.documentation = "um...\r\nNETWORK SAFE: safe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) play_bink_movie_from_tag_1_definition
{
	.return_type = _hs_type_void,
	.name = "play_bink_movie_from_tag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741980, // $TODO write the function chuckle nuts
	.documentation = "play a bink movie from a tag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_bink_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) play_credits_skip_to_menu_0_definition
{
	.return_type = _hs_type_void,
	.name = "play_credits_skip_to_menu",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741D10, // $TODO write the function chuckle nuts
	.documentation = "plays credits movie, pressing any button will stop credits and launch main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) bink_time_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "bink_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741F20, // $TODO write the function chuckle nuts
	.documentation = "uh...\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) set_global_doppler_factor_1_definition
{
	.return_type = _hs_type_void,
	.name = "set_global_doppler_factor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742240, // $TODO write the function chuckle nuts
	.documentation = "new doppler factor: <real>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) set_global_mixbin_headroom_2_definition
{
	.return_type = _hs_type_void,
	.name = "set_global_mixbin_headroom",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742610, // $TODO write the function chuckle nuts
	.documentation = "blah\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) debug_sound_environment_source_parameter_2_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_environment_source_parameter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742A10, // $TODO write the function chuckle nuts
	.documentation = "blah\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) data_mine_set_mission_segment_1_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_set_mission_segment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742D60, // $TODO write the function chuckle nuts
	.documentation = "sets the mission segment for single player data mine events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) data_mine_insert_1_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_insert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742FA0, // $TODO write the function chuckle nuts
	.documentation = "insert text and camera position in the data mine\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) data_mine_upload_0_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_upload",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743470, // $TODO write the function chuckle nuts
	.documentation = "upload all data mining data files to debug server\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) data_mine_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743610, // $TODO write the function chuckle nuts
	.documentation = "enable/disable data mining\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) data_mine_flush_0_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_flush",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743980, // $TODO write the function chuckle nuts
	.documentation = "flush any pending data mine data to disk\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) data_mine_debug_menu_setting_1_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_debug_menu_setting",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743C70, // $TODO write the function chuckle nuts
	.documentation = "close debug menu and set user experience setting to string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) data_mine_open_debug_menu_0_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_open_debug_menu",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743FD0, // $TODO write the function chuckle nuts
	.documentation = "open debug menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) data_mine_set_display_mission_segment_1_definition
{
	.return_type = _hs_type_void,
	.name = "data_mine_set_display_mission_segment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007442B0, // $TODO write the function chuckle nuts
	.documentation = "toggle display of data mine mission segment\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown4F4_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown4F4",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744420, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) test_memory_allocators_5_definition
{
	.return_type = _hs_type_void,
	.name = "test_memory_allocators",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007447A0, // $TODO write the function chuckle nuts
	.documentation = "performs tests on different memory allocators and saves the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) test_memory_allocators_6_definition
{
	.return_type = _hs_type_void,
	.name = "test_memory_allocators",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744970, // $TODO write the function chuckle nuts
	.documentation = "performs tests on different memory allocators and saves the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) display_video_standard_0_definition
{
	.return_type = _hs_type_void,
	.name = "display_video_standard",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744B50, // $TODO write the function chuckle nuts
	.documentation = "displays the video standard the game is in\r\nNETWORK SAFE: safe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_xcr_monkey_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_xcr_monkey_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744E90, // $TODO write the function chuckle nuts
	.documentation = "enable/disable controller monkeys for all in game players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_show_guide_status_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_show_guide_status",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007450F0, // $TODO write the function chuckle nuts
	.documentation = "prints the guide status as automation events.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_show_users_xuids_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_show_users_xuids",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745350, // $TODO write the function chuckle nuts
	.documentation = "prints the signed in users' xuids as automation events.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) test_show_are_users_friends_2_definition
{
	.return_type = _hs_type_void,
	.name = "test_show_are_users_friends",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007454A0, // $TODO write the function chuckle nuts
	.documentation = "prints whether the current controller and specified xuid are friends as automation events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) test_invite_friend_2_definition
{
	.return_type = _hs_type_void,
	.name = "test_invite_friend",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745720, // $TODO write the function chuckle nuts
	.documentation = "shows the guide send friend invite ui from the current controller to the specified xuid\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_get_squad_session_id_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_get_squad_session_id",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745B20, // $TODO write the function chuckle nuts
	.documentation = "get the current squad session id\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_auto_get_screens_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_get_screens",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745C70, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> list the current gui screens\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) test_auto_get_screen_widgets_2_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_get_screen_widgets",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745E80, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> lists widgets in a screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) test_auto_screen_get_datasources_2_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_get_datasources",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746160, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> lists screen datasources\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) test_auto_screen_get_data_columns_3_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_get_data_columns",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746300, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <datasource string id> lists columns that make up this datasource\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) test_auto_screen_get_data_4_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_get_data",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746590, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <datasource string id> <datasource column string id> lists data from a datasource\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) test_auto_screen_invoke_list_item_by_name_5_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_invoke_list_item_by_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746810, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <list string id> <datasource column string id> <column value string id> triggers the list item who's column's value matches the specified one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) test_auto_screen_invoke_list_item_by_text_5_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_invoke_list_item_by_text",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746A20, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <list string id> <datasource column string id> <column value string id> triggers the list item who's column's value matches the specified one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) test_auto_screen_invoke_list_item_by_handle_4_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_invoke_list_item_by_handle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746CB0, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <list string id> <element handle> triggers the list item specified by the element handle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string_id,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) test_auto_screen_send_button_press_3_definition
{
	.return_type = _hs_type_void,
	.name = "test_auto_screen_send_button_press",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746EC0, // $TODO write the function chuckle nuts
	.documentation = "<arbitrary int> <screen index> <button name> sends the button press to the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) test_download_storage_file_2_definition
{
	.return_type = _hs_type_void,
	.name = "test_download_storage_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747140, // $TODO write the function chuckle nuts
	.documentation = "<url> <filename> downloads a file from LSP to the client\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_game_results_save_to_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_game_results_save_to_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747330, // $TODO write the function chuckle nuts
	.documentation = "<filename> save game results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_game_results_load_from_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_game_results_load_from_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747600, // $TODO write the function chuckle nuts
	.documentation = "<filename> load game results into memory for use in the carnage report.  the format may change build to build causing this to fail or weird results to come up.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_roster_save_to_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_roster_save_to_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007477E0, // $TODO write the function chuckle nuts
	.documentation = "<filename> save roster to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_roster_load_from_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_roster_load_from_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007479B0, // $TODO write the function chuckle nuts
	.documentation = "<filename> load roster into memory.  the format may change build to build causing this to fail or weird results to come up.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) test_fragment_utility_drive_3_definition
{
	.return_type = _hs_type_void,
	.name = "test_fragment_utility_drive",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747D90, // $TODO write the function chuckle nuts
	.documentation = "<file_count> <growth_repetitions> <growth_size> writes files of varying sizes to the utiltiy drive in an effort to fragment it really badly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_metagame_results_save_to_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_metagame_results_save_to_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747F50, // $TODO write the function chuckle nuts
	.documentation = "<filename> save metagame results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) test_metagame_results_load_from_file_1_definition
{
	.return_type = _hs_type_void,
	.name = "test_metagame_results_load_from_file",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007480F0, // $TODO write the function chuckle nuts
	.documentation = "<filename> save metagame results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) clear_webcache_0_definition
{
	.return_type = _hs_type_void,
	.name = "clear_webcache",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748380, // $TODO write the function chuckle nuts
	.documentation = "clears the webcache\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) force_manifest_redownload_0_definition
{
	.return_type = _hs_type_void,
	.name = "force_manifest_redownload",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007485E0, // $TODO write the function chuckle nuts
	.documentation = "redownload the online storage manifest\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) online_files_retry_0_definition
{
	.return_type = _hs_type_void,
	.name = "online_files_retry",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F5A0, // $TODO write the function chuckle nuts
	.documentation = "tell the upload queue to start up again after a failure\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) online_files_upload_2_definition
{
	.return_type = _hs_type_void,
	.name = "online_files_upload",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F7C0, // $TODO write the function chuckle nuts
	.documentation = "<filename> <slot_index> enqueues the specified filename for upload\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) online_files_throttle_bandwidth_1_definition
{
	.return_type = _hs_type_void,
	.name = "online_files_throttle_bandwidth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FA70, // $TODO write the function chuckle nuts
	.documentation = "<bytes per second> sets the maximum bandwidth that online_files can use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) online_marketplace_refresh_0_definition
{
	.return_type = _hs_type_void,
	.name = "online_marketplace_refresh",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FC40, // $TODO write the function chuckle nuts
	.documentation = "tells the marketplace manager to take another look at signed in users\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) webstats_disable_1_definition
{
	.return_type = _hs_type_void,
	.name = "webstats_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FED0, // $TODO write the function chuckle nuts
	.documentation = "disables webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) webstats_test_submit_1_definition
{
	.return_type = _hs_type_void,
	.name = "webstats_test_submit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730200, // $TODO write the function chuckle nuts
	.documentation = "submits a string to webstats upload\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) webstats_test_submit_multiplayer_0_definition
{
	.return_type = _hs_type_void,
	.name = "webstats_test_submit_multiplayer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007304C0, // $TODO write the function chuckle nuts
	.documentation = "submits a test multiplayer blob to webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) webstats_test_submit_campaign_0_definition
{
	.return_type = _hs_type_void,
	.name = "webstats_test_submit_campaign",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730660, // $TODO write the function chuckle nuts
	.documentation = "submits a test campaign blob to webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) webstats_throttle_bandwidth_1_definition
{
	.return_type = _hs_type_void,
	.name = "webstats_throttle_bandwidth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730920, // $TODO write the function chuckle nuts
	.documentation = "<bytes per second> sets the maximum bandwidth that webstats can use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) netdebug_prefer_internet_1_definition
{
	.return_type = _hs_type_void,
	.name = "netdebug_prefer_internet",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730BB0, // $TODO write the function chuckle nuts
	.documentation = "hints to the netdebug code that that we're on the internet\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) flag_new_2_definition
{
	.return_type = _hs_type_void,
	.name = "flag_new",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730DE0, // $TODO write the function chuckle nuts
	.documentation = "<name> <description>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) flag_new_at_look_2_definition
{
	.return_type = _hs_type_void,
	.name = "flag_new_at_look",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730FF0, // $TODO write the function chuckle nuts
	.documentation = "<name> <description>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) flags_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "flags_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731260, // $TODO write the function chuckle nuts
	.documentation = "erases all comment flags when not in editor (sapien)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bug_now_1_definition
{
	.return_type = _hs_type_void,
	.name = "bug_now",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731570, // $TODO write the function chuckle nuts
	.documentation = "uploads files for bug <name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bug_now_lite_1_definition
{
	.return_type = _hs_type_void,
	.name = "bug_now_lite",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731740, // $TODO write the function chuckle nuts
	.documentation = "uploads files (w/ no minidump) for bug <name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bug_now_auto_1_definition
{
	.return_type = _hs_type_void,
	.name = "bug_now_auto",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731980, // $TODO write the function chuckle nuts
	.documentation = "uploads bug files, auto named <include_minidump>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_list_children_2_definition
{
	.return_type = _hs_type_object_list,
	.name = "object_list_children",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731C40, // $TODO write the function chuckle nuts
	.documentation = "returns list of child objects by definition.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_object_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) voice_set_outgoing_channel_count_2_definition
{
	.return_type = _hs_type_void,
	.name = "voice_set_outgoing_channel_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732080, // $TODO write the function chuckle nuts
	.documentation = "sets the outgoing channel count <in-game> <out-of-game>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) voice_set_voice_repeater_peer_index_1_definition
{
	.return_type = _hs_type_void,
	.name = "voice_set_voice_repeater_peer_index",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007322D0, // $TODO write the function chuckle nuts
	.documentation = "sets the repeater peer index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) voice_set_mute_3_definition
{
	.return_type = _hs_type_void,
	.name = "voice_set_mute",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732560, // $TODO write the function chuckle nuts
	.documentation = "mute a player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_hopper_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_hopper",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732790, // $TODO write the function chuckle nuts
	.documentation = "<user index> clears the hopper stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_global_arbitrated_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_global_arbitrated",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732940, // $TODO write the function chuckle nuts
	.documentation = "<user index> clears the global arbitrated stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_global_unarbitrated_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_global_unarbitrated",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732C30, // $TODO write the function chuckle nuts
	.documentation = "<user index> clears the global unarbitrated stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_leaderboard_refresh_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_refresh",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732E80, // $TODO write the function chuckle nuts
	.documentation = "<user index> refreshes stats for everybody in the session\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dump_loaded_tags_0_definition
{
	.return_type = _hs_type_void,
	.name = "dump_loaded_tags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733110, // $TODO write the function chuckle nuts
	.documentation = "dump a list of open tags to <map_name>_tags.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_start_3_definition
{
	.return_type = _hs_type_long_integer,
	.name = "interpolator_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007332C0, // $TODO write the function chuckle nuts
	.documentation = "<name> <final value> <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_start_smooth_3_definition
{
	.return_type = _hs_type_long_integer,
	.name = "interpolator_start_smooth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733850, // $TODO write the function chuckle nuts
	.documentation = "<name> <final value> <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_stop_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "interpolator_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733D20, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_restart_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "interpolator_restart",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734210, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_is_active_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "interpolator_is_active",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734540, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_is_finished_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "interpolator_is_finished",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734AC0, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) interpolator_set_current_value_2_definition
{
	.return_type = _hs_type_long_integer,
	.name = "interpolator_set_current_value",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734F50, // $TODO write the function chuckle nuts
	.documentation = "<name> <current value>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) interpolator_get_current_value_2_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_current_value",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735470, // $TODO write the function chuckle nuts
	.documentation = "<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) interpolator_get_start_value_2_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_start_value",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007357F0, // $TODO write the function chuckle nuts
	.documentation = "<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) interpolator_get_final_value_2_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_final_value",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735D30, // $TODO write the function chuckle nuts
	.documentation = "<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_get_current_phase_1_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_current_phase",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007361A0, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_get_current_time_fraction_1_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_current_time_fraction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007366A0, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_get_start_time_1_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_start_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736A40, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) interpolator_get_final_time_1_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_get_final_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736EC0, // $TODO write the function chuckle nuts
	.documentation = "<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_evaluate_at_3_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_evaluate_at",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007372F0, // $TODO write the function chuckle nuts
	.documentation = "<name> <value in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_evaluate_at_time_fraction_3_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_evaluate_at_time_fraction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007378A0, // $TODO write the function chuckle nuts
	.documentation = "<name> <time fraction in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_evaluate_at_time_3_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_evaluate_at_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737CA0, // $TODO write the function chuckle nuts
	.documentation = "<name> <time in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) interpolator_evaluate_at_time_delta_3_definition
{
	.return_type = _hs_type_real,
	.name = "interpolator_evaluate_at_time_delta",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738300, // $TODO write the function chuckle nuts
	.documentation = "<name> <time delta> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) interpolator_stop_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "interpolator_stop_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007388A0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) interpolator_restart_all_0_definition
{
	.return_type = _hs_type_void,
	.name = "interpolator_restart_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738A60, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) interpolator_flip_0_definition
{
	.return_type = _hs_type_void,
	.name = "interpolator_flip",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738C20, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) set_pc_runtime_language_1_definition
{
	.return_type = _hs_type_void,
	.name = "set_pc_runtime_language",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738EA0, // $TODO write the function chuckle nuts
	.documentation = "sets the pc language from a string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) animation_cache_stats_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "animation_cache_stats_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007391F0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_clone_players_weapon_3_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_clone_players_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739390, // $TODO write the function chuckle nuts
	.documentation = "clone the first player's most reasonable weapon and attach it to the specified object's marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_move_attached_objects_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_move_attached_objects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739810, // $TODO write the function chuckle nuts
	.documentation = "move the object's children attached to the first marker to the second marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) vehicle_enable_ghost_effects_1_definition
{
	.return_type = _hs_type_void,
	.name = "vehicle_enable_ghost_effects",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739CB0, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) set_global_sound_environment_6_definition
{
	.return_type = _hs_type_void,
	.name = "set_global_sound_environment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739FD0, // $TODO write the function chuckle nuts
	.documentation = "this is your brain on drugs\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) reset_dsp_image_0_definition
{
	.return_type = _hs_type_void,
	.name = "reset_dsp_image",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A6A0, // $TODO write the function chuckle nuts
	.documentation = "if we ever need to use this someone is getting fired\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_save_cinematic_skip_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_save_cinematic_skip",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A840, // $TODO write the function chuckle nuts
	.documentation = "don't use this, except in one place.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_outro_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_outro_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AAA0, // $TODO write the function chuckle nuts
	.documentation = "flag this cutscene as an outro cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_enable_ambience_details_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_enable_ambience_details",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AD20, // $TODO write the function chuckle nuts
	.documentation = "enable/disable ambience details in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B0C0, // $TODO write the function chuckle nuts
	.documentation = "enable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_reset_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B280, // $TODO write the function chuckle nuts
	.documentation = "reset\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_blur_amount_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_blur_amount",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B450, // $TODO write the function chuckle nuts
	.documentation = "blur mount\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_threshold_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_threshold",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B7A0, // $TODO write the function chuckle nuts
	.documentation = "threshold\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_brightness_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_brightness",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B9B0, // $TODO write the function chuckle nuts
	.documentation = "brightness\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_box_factor_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_box_factor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BBA0, // $TODO write the function chuckle nuts
	.documentation = "box factor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_max_factor_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_max_factor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BE10, // $TODO write the function chuckle nuts
	.documentation = "max factor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_silver_bullet_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_silver_bullet",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C0C0, // $TODO write the function chuckle nuts
	.documentation = "silver bullet\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_only_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_only",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C2D0, // $TODO write the function chuckle nuts
	.documentation = "only\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_high_res_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_high_res",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C510, // $TODO write the function chuckle nuts
	.documentation = "high res\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_brightness_alpha_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_brightness_alpha",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C6B0, // $TODO write the function chuckle nuts
	.documentation = "brightness alpha\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) rasterizer_bloom_override_max_factor_alpha_1_definition
{
	.return_type = _hs_type_void,
	.name = "rasterizer_bloom_override_max_factor_alpha",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CAA0, // $TODO write the function chuckle nuts
	.documentation = "max factor alpha\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cache_block_for_one_frame_0_definition
{
	.return_type = _hs_type_void,
	.name = "cache_block_for_one_frame",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CCC0, // $TODO write the function chuckle nuts
	.documentation = "call this to force texture and geometry cache to block until satiated\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sound_suppress_ambience_update_on_revert_0_definition
{
	.return_type = _hs_type_void,
	.name = "sound_suppress_ambience_update_on_revert",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CE30, // $TODO write the function chuckle nuts
	.documentation = "call this when transitioning between two cinematics so ambience won't fade in between the skips\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_autoexposure_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_autoexposure_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D160, // $TODO write the function chuckle nuts
	.documentation = "toggle autoexposure\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) render_exposure_full_5_definition
{
	.return_type = _hs_type_void,
	.name = "render_exposure_full",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D550, // $TODO write the function chuckle nuts
	.documentation = "animate exposure from <initial> to <final> stops using <initial> to <final> opacity in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) render_exposure_fade_in_2_definition
{
	.return_type = _hs_type_void,
	.name = "render_exposure_fade_in",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D930, // $TODO write the function chuckle nuts
	.documentation = "fade to animated exposure <stops> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_exposure_fade_out_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_exposure_fade_out",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DE10, // $TODO write the function chuckle nuts
	.documentation = "fade to default exposure settings in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) render_exposure_2_definition
{
	.return_type = _hs_type_void,
	.name = "render_exposure",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E370, // $TODO write the function chuckle nuts
	.documentation = "fade to animated exposure <stops> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_autoexposure_instant_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_autoexposure_instant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E5E0, // $TODO write the function chuckle nuts
	.documentation = "instantly adopt auto exposure values in the next N frames\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_exposure_set_environment_darken_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_exposure_set_environment_darken",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E930, // $TODO write the function chuckle nuts
	.documentation = "set environmental darkening\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_depth_of_field_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_depth_of_field_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ED30, // $TODO write the function chuckle nuts
	.documentation = "turns depth of field on and off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) render_depth_of_field_4_definition
{
	.return_type = _hs_type_void,
	.name = "render_depth_of_field",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F000, // $TODO write the function chuckle nuts
	.documentation = "animate depth of field, in focus from <near> to <far>, with depth of field <depth>, in <ticks>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_dof_focus_depth_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_dof_focus_depth",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F550, // $TODO write the function chuckle nuts
	.documentation = "interpolate depth of field to <depth>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) render_dof_blur_animate_2_definition
{
	.return_type = _hs_type_void,
	.name = "render_dof_blur_animate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F890, // $TODO write the function chuckle nuts
	.documentation = "interpolate depth of field maximum blur to <blur> in <ticks>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) render_debug_video_mode_0_definition
{
	.return_type = _hs_type_void,
	.name = "render_debug_video_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FC90, // $TODO write the function chuckle nuts
	.documentation = "displays the current video mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_lightmap_shadow_disable_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lightmap_shadow_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FE90, // $TODO write the function chuckle nuts
	.documentation = "turn off lightmap shadow in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_lightmap_shadow_enable_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_lightmap_shadow_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740180, // $TODO write the function chuckle nuts
	.documentation = "turn on lightmap shadow in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) predict_animation_2_definition
{
	.return_type = _hs_type_void,
	.name = "predict_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740320, // $TODO write the function chuckle nuts
	.documentation = "begin loading an animation for the graph provided\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_animation_graph,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_players_by_team_1_definition
{
	.return_type = _hs_type_object_list,
	.name = "mp_players_by_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740760, // $TODO write the function chuckle nuts
	.documentation = "given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_active_player_count_by_team_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "mp_active_player_count_by_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740B20, // $TODO write the function chuckle nuts
	.documentation = "given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) deterministic_end_game_0_definition
{
	.return_type = _hs_type_void,
	.name = "deterministic_end_game",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740F80, // $TODO write the function chuckle nuts
	.documentation = "end game deterministically, by inserting a simulation queue event\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_game_won_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_game_won",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007412A0, // $TODO write the function chuckle nuts
	.documentation = "given a team index, declares the game a victory for that team and a loss for all others\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_respawn_override_timers_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_respawn_override_timers",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007415A0, // $TODO write the function chuckle nuts
	.documentation = "causes all players on the specified team waiting to respawn (due to timer) to respawn immediately\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mp_ai_allegiance_2_definition
{
	.return_type = _hs_type_void,
	.name = "mp_ai_allegiance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007418E0, // $TODO write the function chuckle nuts
	.documentation = "causes an allegiance to be formed between an AI squad team and a multiplayer team\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_team,
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mp_allegiance_2_definition
{
	.return_type = _hs_type_void,
	.name = "mp_allegiance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741C10, // $TODO write the function chuckle nuts
	.documentation = "create an allegiance between two multiplayer teams\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_mp_team,
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) mp_round_started_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "mp_round_started",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741F60, // $TODO write the function chuckle nuts
	.documentation = "used to determine if the mp game is in the active-round phase or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) give_medal_1_definition
{
	.return_type = _hs_type_void,
	.name = "give_medal",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007423B0, // $TODO write the function chuckle nuts
	.documentation = "awards a medal (0..58) to pad 1 due to pad 2 (if they're there)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) mp_scripts_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "mp_scripts_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007426D0, // $TODO write the function chuckle nuts
	.documentation = "on the host in a distributed game, resets the scripts (but not internal hs globals)\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_ai_place_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_ai_place",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007429A0, // $TODO write the function chuckle nuts
	.documentation = "places the given number of members of the specified squad as non-authoritative objects\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mp_ai_place_2_definition
{
	.return_type = _hs_type_void,
	.name = "mp_ai_place",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742DD0, // $TODO write the function chuckle nuts
	.documentation = "places the given number of members of the specified squad as non-authoritative objects\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_ai_kill_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_ai_kill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743090, // $TODO write the function chuckle nuts
	.documentation = "instantly kills the specified non-authoritative encounter and/or squad.\r\nNETWORK SAFE: YEs",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_ai_kill_silent_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_ai_kill_silent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743580, // $TODO write the function chuckle nuts
	.documentation = "instantly and silently (no animation or sound played) kills the specified non-authoritative encounter and/or squad.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_object_create_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_object_create",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743870, // $TODO write the function chuckle nuts
	.documentation = "creates a non-authoratative object from the scenario.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_object_create_clone_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_object_create_clone",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743C40, // $TODO write the function chuckle nuts
	.documentation = "creates a non-authoratative object, potentially resulting in multiple objects if it already exists.\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_object_create_anew_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_object_create_anew",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743FA0, // $TODO write the function chuckle nuts
	.documentation = "creates a non-authoratative object, destroying it first if it already exists.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object_name
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_object_destroy_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_object_destroy",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744370, // $TODO write the function chuckle nuts
	.documentation = "destroys a non-authoratative object.\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_file_set_backend_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_file_set_backend",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744780, // $TODO write the function chuckle nuts
	.documentation = "override the tag file system backend\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mp_object_belongs_to_team_2_definition
{
	.return_type = _hs_type_void,
	.name = "mp_object_belongs_to_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744930, // $TODO write the function chuckle nuts
	.documentation = "causes specified object to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mp_weapon_belongs_to_team_2_definition
{
	.return_type = _hs_type_void,
	.name = "mp_weapon_belongs_to_team",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744DE0, // $TODO write the function chuckle nuts
	.documentation = "causes specified weapon to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_mp_team
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) mp_debug_goal_object_boundary_geometry_1_definition
{
	.return_type = _hs_type_void,
	.name = "mp_debug_goal_object_boundary_geometry",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745230, // $TODO write the function chuckle nuts
	.documentation = "toggle debug geometry for multiplayer goal objects\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) mp_dump_candy_monitor_state_0_definition
{
	.return_type = _hs_type_void,
	.name = "mp_dump_candy_monitor_state",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745400, // $TODO write the function chuckle nuts
	.documentation = "dumps the current candy monitor state to a file on disk\r\nNETWORK SFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) set_camera_third_person_2_definition
{
	.return_type = _hs_type_void,
	.name = "set_camera_third_person",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007455D0, // $TODO write the function chuckle nuts
	.documentation = "<user_index> <third_person (yes/no)> sets user's camera perspective\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) get_camera_third_person_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "get_camera_third_person",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745AE0, // $TODO write the function chuckle nuts
	.documentation = "<user_index> gets user's camera perspective\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) determinism_debug_manager_enable_logging_1_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_enable_logging",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745E60, // $TODO write the function chuckle nuts
	.documentation = "enable/disable determinism logging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) determinism_debug_manager_set_trace_flags_1_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_set_trace_flags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746140, // $TODO write the function chuckle nuts
	.documentation = "set which determinism debugging traces should be turned on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) determinism_debug_manager_enable_game_state_checksum_1_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_enable_game_state_checksum",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007462E0, // $TODO write the function chuckle nuts
	.documentation = "enable/disable determinism checkpoints\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) determinism_debug_manager_enable_trace_1_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_enable_trace",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746510, // $TODO write the function chuckle nuts
	.documentation = "enable determinism tracing\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) determinism_debug_manager_set_consumer_sample_level_2_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_set_consumer_sample_level",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007467F0, // $TODO write the function chuckle nuts
	.documentation = "sets sample level for a consumer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown584_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown584",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007469C0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) saved_film_play_2_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_play",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746CF0, // $TODO write the function chuckle nuts
	.documentation = "plays a saved film for given controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saved_film_play_last_0_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_play_last",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746F40, // $TODO write the function chuckle nuts
	.documentation = "play back last recorded saved film for given controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_disable_version_checking_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_disable_version_checking",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007471F0, // $TODO write the function chuckle nuts
	.documentation = "disable saved film version checking\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_toggle_debug_saving_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_toggle_debug_saving",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747370, // $TODO write the function chuckle nuts
	.documentation = "toggle saving of last film to the debug output directory automatically after each game level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_films_delete_on_level_load_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_films_delete_on_level_load",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007476B0, // $TODO write the function chuckle nuts
	.documentation = "toggle deletion last saved film after loading a new game level\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_films_show_timestamp_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_films_show_timestamp",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747870, // $TODO write the function chuckle nuts
	.documentation = "toggle showing timestamp for saved film playback\r\nNETWORK SAFE: Unkown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_manager_should_record_film_default_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_manager_should_record_film_default",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747AF0, // $TODO write the function chuckle nuts
	.documentation = "Toggle recording of replay in single player game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) mover_set_program_2_definition
{
	.return_type = _hs_type_boolean,
	.name = "mover_set_program",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747E40, // $TODO write the function chuckle nuts
	.documentation = "Set the motor program of the given mover object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) determinism_log_compare_log_files_2_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_log_compare_log_files",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748220, // $TODO write the function chuckle nuts
	.documentation = "comares determinism log files\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) floating_point_exceptions_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "floating_point_exceptions_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007484C0, // $TODO write the function chuckle nuts
	.documentation = "enables/disables floating point exceptions\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) determinism_debug_manager_enable_log_file_comparision_on_oos_1_definition
{
	.return_type = _hs_type_void,
	.name = "determinism_debug_manager_enable_log_file_comparision_on_oos",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007486B0, // $TODO write the function chuckle nuts
	.documentation = "enables/disables determinism log file comparision on oos\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) event_logs_snapshot_0_definition
{
	.return_type = _hs_type_void,
	.name = "event_logs_snapshot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F5E0, // $TODO write the function chuckle nuts
	.documentation = "copy error files to the error snapshot folder\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_reload_force_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_reload_force",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F840, // $TODO write the function chuckle nuts
	.documentation = "rather than force a painful xsync to test xsync, allow me to fake a tag_reload\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_unload_force_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_unload_force",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FB70, // $TODO write the function chuckle nuts
	.documentation = "call tag_unload\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_load_force_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_load_force",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FDB0, // $TODO write the function chuckle nuts
	.documentation = "call tag_load\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) predict_bink_movie_1_definition
{
	.return_type = _hs_type_void,
	.name = "predict_bink_movie",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FF80, // $TODO write the function chuckle nuts
	.documentation = "um2...\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) predict_bink_movie_from_tag_1_definition
{
	.return_type = _hs_type_void,
	.name = "predict_bink_movie_from_tag",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730460, // $TODO write the function chuckle nuts
	.documentation = "um2...\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_bink_definition
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) profiler_dump_history_0_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_dump_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730710, // $TODO write the function chuckle nuts
	.documentation = "dump profile history.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_mode_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730AA0, // $TODO write the function chuckle nuts
	.documentation = "<user_index> <mode_index> sets user's camera perspective\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_flying_cam_at_point_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_flying_cam_at_point",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730DA0, // $TODO write the function chuckle nuts
	.documentation = "<user_index> <camera_point> sets a flying camera perspective\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_cutscene_camera_point
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) camera_set_target_2_definition
{
	.return_type = _hs_type_void,
	.name = "camera_set_target",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731190, // $TODO write the function chuckle nuts
	.documentation = "<user_index> <object_index> sets the current camera's target\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_coop_player_count_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "game_coop_player_count",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731660, // $TODO write the function chuckle nuts
	.documentation = "returns the number of coop players active in game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_force_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_force_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007318A0, // $TODO write the function chuckle nuts
	.documentation = "force your will upon the player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) profiler_output_pulse_0_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_output_pulse",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731AF0, // $TODO write the function chuckle nuts
	.documentation = "dump profile stats to telnet console.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) string_id_name_1_definition
{
	.return_type = _hs_type_void,
	.name = "string_id_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731D70, // $TODO write the function chuckle nuts
	.documentation = "prints the name of the string id to the console\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) find_1_definition
{
	.return_type = _hs_type_void,
	.name = "find",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731F30, // $TODO write the function chuckle nuts
	.documentation = "prints any hs command matching the string passed in\r\nNETWORK SAFE: of course you idiot",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) add_recycling_volume_3_definition
{
	.return_type = _hs_type_void,
	.name = "add_recycling_volume",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732140, // $TODO write the function chuckle nuts
	.documentation = "removes garbage in specified trigger volume to a max count over n seconds\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_trigger_volume,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_resources_set_per_frame_publish_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_resources_set_per_frame_publish",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732670, // $TODO write the function chuckle nuts
	.documentation = "turns on/off per frame resource publish (stress testing)\r\nNETWORK SAFE: If not, Ooops",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) object_recycling_clear_history_0_definition
{
	.return_type = _hs_type_void,
	.name = "object_recycling_clear_history",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732840, // $TODO write the function chuckle nuts
	.documentation = "clear object recycling history\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dump_cinematics_script_0_definition
{
	.return_type = _hs_type_void,
	.name = "dump_cinematics_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732AB0, // $TODO write the function chuckle nuts
	.documentation = "dump the cinematic script to cinematic_dump.txt",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) global_preferences_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "global_preferences_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732D00, // $TODO write the function chuckle nuts
	.documentation = "clears all game preference information\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) network_storage_set_storage_subdirectory_1_definition
{
	.return_type = _hs_type_void,
	.name = "network_storage_set_storage_subdirectory",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732F80, // $TODO write the function chuckle nuts
	.documentation = "<subdirectory> The default is the build number or 'untracked'\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) network_storage_set_storage_user_1_definition
{
	.return_type = _hs_type_void,
	.name = "network_storage_set_storage_user",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733230, // $TODO write the function chuckle nuts
	.documentation = "<user> Use the network file stored on untracked\\<user>.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) status_line_dump_0_definition
{
	.return_type = _hs_type_void,
	.name = "status_line_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007333E0, // $TODO write the function chuckle nuts
	.documentation = "dumps active status_lines to system_log.txt\r\nNETWORK SAFE: I hope so",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_tick_get_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "game_tick_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733670, // $TODO write the function chuckle nuts
	.documentation = "returns current game tick\r\nNETWORK SAFE: only if game_time_get is",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) loop_it_1_definition
{
	.return_type = _hs_type_void,
	.name = "loop_it",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007339E0, // $TODO write the function chuckle nuts
	.documentation = "runs the expression at most once per tick\r\nNETWORK SAFE: just assume it isn't",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) loop_clear_0_definition
{
	.return_type = _hs_type_void,
	.name = "loop_clear",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733B20, // $TODO write the function chuckle nuts
	.documentation = "runs the expression at most once per tick\r\nNETWORK SAFE: just assume it isn't",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) status_lines_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "status_lines_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733E90, // $TODO write the function chuckle nuts
	.documentation = "enables status_lines that were initialized with identifiers that match the input substring\r\nNETWORK SAFE: I hope so",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) status_lines_disable_1_definition
{
	.return_type = _hs_type_void,
	.name = "status_lines_disable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734180, // $TODO write the function chuckle nuts
	.documentation = "disables status_lines that were initialized with identifiers that match the input substring\r\nNETWORK SAFE: I hope so",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) on_target_platform_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "on_target_platform",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007342F0, // $TODO write the function chuckle nuts
	.documentation = "returns true if the game thinks it's running on the target platform\r\nNETWORK SAFE: no",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profile_activate_1_definition
{
	.return_type = _hs_type_void,
	.name = "profile_activate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007345C0, // $TODO write the function chuckle nuts
	.documentation = "activates profile sections based on a substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profile_deactivate_1_definition
{
	.return_type = _hs_type_void,
	.name = "profile_deactivate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734930, // $TODO write the function chuckle nuts
	.documentation = "deactivates profile sections based on a substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_game_set_player_standing_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_game_set_player_standing",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734B30, // $TODO write the function chuckle nuts
	.documentation = "set's a player's standing by gamertag\r\nNETWORK SAFE: host only",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_get_game_id_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_get_game_id",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734D50, // $TODO write the function chuckle nuts
	.documentation = "prints the id of the game to the console\r\nNETWORK SAFE: if it's not, it wouldn't be very useful",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_matchmaking_hopper_list_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_matchmaking_hopper_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734FF0, // $TODO write the function chuckle nuts
	.documentation = "prints the hopper list\r\nNETWORK SAFE: just assume it isn't",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_matchmaking_hopper_game_list_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_matchmaking_hopper_game_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007352F0, // $TODO write the function chuckle nuts
	.documentation = "prints games for the current hopper\r\nNETWORK SAFE: just assume it isn't",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_matchmaking_hopper_set_game_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_matchmaking_hopper_set_game",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735510, // $TODO write the function chuckle nuts
	.documentation = "sets the game vairant for the current hopper\r\nNETWORK SAFE: just assume it isn't",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_set_playback_game_speed_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_set_playback_game_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735750, // $TODO write the function chuckle nuts
	.documentation = "set the saved film playback speed",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_set_pending_playback_game_speed_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_set_pending_playback_game_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735B20, // $TODO write the function chuckle nuts
	.documentation = "set the pending saved film playback speed",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) noguchis_mystery_tour_1_definition
{
	.return_type = _hs_type_void,
	.name = "noguchis_mystery_tour",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735ED0, // $TODO write the function chuckle nuts
	.documentation = "set the additional music directory to dump into the cache file",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) designer_zone_sync_0_definition
{
	.return_type = _hs_type_void,
	.name = "designer_zone_sync",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736030, // $TODO write the function chuckle nuts
	.documentation = "synchronizes global objects with loaded designer zones\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) dump_designer_zone_1_definition
{
	.return_type = _hs_type_void,
	.name = "dump_designer_zone",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736320, // $TODO write the function chuckle nuts
	.documentation = "dump interesting info about a designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_designer_zone
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) designer_zone_activate_1_definition
{
	.return_type = _hs_type_void,
	.name = "designer_zone_activate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007364E0, // $TODO write the function chuckle nuts
	.documentation = "activate designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_designer_zone
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) designer_zone_deactivate_1_definition
{
	.return_type = _hs_type_void,
	.name = "designer_zone_deactivate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736900, // $TODO write the function chuckle nuts
	.documentation = "deactivates designer zone, deletes objects that were part of that designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_designer_zone
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_always_active_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_always_active",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736C20, // $TODO write the function chuckle nuts
	.documentation = "sets/unsets the object always active flag to ignore pvs changes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_seek_to_film_tick_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_seek_to_film_tick",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737080, // $TODO write the function chuckle nuts
	.documentation = "seeks to a film tick",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_is_active_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "tag_is_active",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737240, // $TODO write the function chuckle nuts
	.documentation = "returns true/false if you can create this tag with the active zones\r\nNETWORK SAFE: only if the global zone state is network safe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_any_tag_not_resolving
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_resources_enable_optional_caching_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_resources_enable_optional_caching",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007375E0, // $TODO write the function chuckle nuts
	.documentation = "turns on/off optional resource prediction\r\nNETWORK SAFE: If not, Oooops",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dump_active_resources_0_definition
{
	.return_type = _hs_type_void,
	.name = "dump_active_resources",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007379F0, // $TODO write the function chuckle nuts
	.documentation = "blah",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_persistent_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_persistent",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737B30, // $TODO write the function chuckle nuts
	.documentation = "<object_index> <true/false> prevent this object from being garbage collected\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) display_zone_size_estimates_1_definition
{
	.return_type = _hs_type_void,
	.name = "display_zone_size_estimates",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737FF0, // $TODO write the function chuckle nuts
	.documentation = "display zone size estimates HUD\r\nNETWORK SAFE: YES",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) report_zone_size_estimates_0_definition
{
	.return_type = _hs_type_void,
	.name = "report_zone_size_estimates",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738260, // $TODO write the function chuckle nuts
	.documentation = "dump zone size estimates to design report log\r\nNETWORK SAFE: YES",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_disconnect_squad_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_disconnect_squad",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007383E0, // $TODO write the function chuckle nuts
	.documentation = "disconnects the squad session\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_disconnect_group_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_disconnect_group",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007387E0, // $TODO write the function chuckle nuts
	.documentation = "disconnects the group session\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_clear_squad_session_parameter_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_clear_squad_session_parameter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007388F0, // $TODO write the function chuckle nuts
	.documentation = "clears a squad session parameter\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_test_clear_group_session_parameter_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_clear_group_session_parameter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738BC0, // $TODO write the function chuckle nuts
	.documentation = "clears a group session parameter\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_test_life_cycle_pause_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_life_cycle_pause",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738E00, // $TODO write the function chuckle nuts
	.documentation = "pauses the life cycle in the specified state\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_test_life_cycle_display_states_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_test_life_cycle_display_states",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739020, // $TODO write the function chuckle nuts
	.documentation = "display the life cycle states\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) overlapped_display_task_descriptions_0_definition
{
	.return_type = _hs_type_void,
	.name = "overlapped_display_task_descriptions",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007392F0, // $TODO write the function chuckle nuts
	.documentation = "displays all tasks\r\nNETWORK SAFE: lol",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) overlapped_task_inject_error_2_definition
{
	.return_type = _hs_type_void,
	.name = "overlapped_task_inject_error",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007395A0, // $TODO write the function chuckle nuts
	.documentation = "inject error for tasks\r\nNETWORK SAFE: lol",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_hopper_all_users_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_hopper_all_users",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007397B0, // $TODO write the function chuckle nuts
	.documentation = "clears the hopper stats for the all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_global_arbitrated_all_users_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_global_arbitrated_all_users",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007399E0, // $TODO write the function chuckle nuts
	.documentation = "clears the global arbitrated stats for all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_leaderboard_clear_global_unarbitrated_all_users_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_clear_global_unarbitrated_all_users",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739C20, // $TODO write the function chuckle nuts
	.documentation = "clears the global unarbitrated stats for all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_sapien_crash_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_sapien_crash",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739F60, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) output_window_add_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "output_window_add_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A080, // $TODO write the function chuckle nuts
	.documentation = "Add an event category to be watched in the output window.",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) output_window_remove_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "output_window_remove_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A470, // $TODO write the function chuckle nuts
	.documentation = "Remove an event category from being watched in the output window.",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) output_window_list_categories_0_definition
{
	.return_type = _hs_type_void,
	.name = "output_window_list_categories",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A680, // $TODO write the function chuckle nuts
	.documentation = "List all the event categories being watched in the output window.",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) decorators_split_2_definition
{
	.return_type = _hs_type_void,
	.name = "decorators_split",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A820, // $TODO write the function chuckle nuts
	.documentation = "duplicates decorator set <index> and moves <percentage> of the decorators into it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bandwidth_profiler_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "bandwidth_profiler_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AA80, // $TODO write the function chuckle nuts
	.documentation = "enables bandwidth profiler\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) bandwidth_profiler_set_context_1_definition
{
	.return_type = _hs_type_void,
	.name = "bandwidth_profiler_set_context",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AD00, // $TODO write the function chuckle nuts
	.documentation = "sets bandwidth profiler context\r\nNETWORK SAFE: probably not",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) overlapped_task_pause_2_definition
{
	.return_type = _hs_type_void,
	.name = "overlapped_task_pause",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AFB0, // $TODO write the function chuckle nuts
	.documentation = "pause tasks\r\nNETWORK SAFE: lol",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) net_banhammer_set_controller_cheat_flags_3_definition
{
	.return_type = _hs_type_void,
	.name = "net_banhammer_set_controller_cheat_flags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B170, // $TODO write the function chuckle nuts
	.documentation = "sets cheat flags\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_string,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) net_banhammer_set_controller_ban_flags_4_definition
{
	.return_type = _hs_type_void,
	.name = "net_banhammer_set_controller_ban_flags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B320, // $TODO write the function chuckle nuts
	.documentation = "sets ban flags\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_banhammer_dump_strings_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_banhammer_dump_strings",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B6A0, // $TODO write the function chuckle nuts
	.documentation = "display cheat/ban strings\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) net_banhammer_dump_repeated_play_list_0_definition
{
	.return_type = _hs_type_void,
	.name = "net_banhammer_dump_repeated_play_list",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B8F0, // $TODO write the function chuckle nuts
	.documentation = "display repeated play lists\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12) net_leaderboard_set_user_stats_6_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_set_user_stats",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BA80, // $TODO write the function chuckle nuts
	.documentation = "slams user leaderboard values locally\r\nNETWORK SAFE: no",
	.parameters = NULL,
	.formal_parameter_count = 6,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10) net_leaderboard_set_user_game_stats_5_definition
{
	.return_type = _hs_type_void,
	.name = "net_leaderboard_set_user_game_stats",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BD80, // $TODO write the function chuckle nuts
	.documentation = "slams more user stats\r\nNETWORK SAFE: no",
	.parameters = NULL,
	.formal_parameter_count = 5,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_build_map_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_build_map_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BFB0, // $TODO write the function chuckle nuts
	.documentation = "writes the current map variant to a file\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) net_verify_map_variant_1_definition
{
	.return_type = _hs_type_void,
	.name = "net_verify_map_variant",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C250, // $TODO write the function chuckle nuts
	.documentation = "verifies the contents of a packed map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) async_set_work_delay_milliseconds_1_definition
{
	.return_type = _hs_type_void,
	.name = "async_set_work_delay_milliseconds",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C420, // $TODO write the function chuckle nuts
	.documentation = "set an artificial delay time for the performance of any asynchronous task work\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) sound_looping_start_with_effect_4_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_start_with_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C610, // $TODO write the function chuckle nuts
	.documentation = "plays a looping sound from the specified source object (or \"none\"), with the specified scale and playback effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_resources_set_demand_throttle_to_io_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_resources_set_demand_throttle_to_io",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CBB0, // $TODO write the function chuckle nuts
	.documentation = "turns on/off demand throttling based on io\r\nNETWORK SAFE: If not, Ooops",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) set_performance_throttle_3_definition
{
	.return_type = _hs_type_void,
	.name = "set_performance_throttle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CD70, // $TODO write the function chuckle nuts
	.documentation = "sets a default value for a performance throttle for a given number of players",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) global_preferences_quality_set_all_1_definition
{
	.return_type = _hs_type_void,
	.name = "global_preferences_quality_set_all",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D1F0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) get_performance_throttle_2_definition
{
	.return_type = _hs_type_real,
	.name = "get_performance_throttle",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D5E0, // $TODO write the function chuckle nuts
	.documentation = "gets the value of a performance throttle for a given number of players",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown5E4_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown5E4",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DAA0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_zone_activate_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_zone_activate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DD80, // $TODO write the function chuckle nuts
	.documentation = "activates scenario cinematic zone (automatically generated script function!)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_zone_deactivate_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_zone_deactivate",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DFA0, // $TODO write the function chuckle nuts
	.documentation = "deactivates scenario cinematic zone (automatically generated script function!)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_zone_activate_from_editor_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_zone_activate_from_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E460, // $TODO write the function chuckle nuts
	.documentation = "activates cinematic zone from tag (automatically generated script function!)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_zone_deactivate_from_editor_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_zone_deactivate_from_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E660, // $TODO write the function chuckle nuts
	.documentation = "deactivates cinematic zone from tag (automatically generated script function!)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) tiling_current_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "tiling_current",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E8E0, // $TODO write the function chuckle nuts
	.documentation = "returns the actual tiling configuration (could have been overridden from the requested configuration)",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_limit_lipsync_to_mouth_only_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_limit_lipsync_to_mouth_only",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EB90, // $TODO write the function chuckle nuts
	.documentation = "cinematic lipsync means you can't use all the facial bones, just the mouth\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dump_active_zone_tags_0_definition
{
	.return_type = _hs_type_void,
	.name = "dump_active_zone_tags",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EF00, // $TODO write the function chuckle nuts
	.documentation = "blah",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) calculate_tag_prediction_1_definition
{
	.return_type = _hs_type_void,
	.name = "calculate_tag_prediction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F1C0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_any_tag
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_resources_enable_fast_prediction_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_resources_enable_fast_prediction",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F4F0, // $TODO write the function chuckle nuts
	.documentation = "turns on/off faster prediction, in case I fucked up something\r\nNETWORK SAFE: If not, Ooops",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unit_start_first_person_custom_animation_4_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_start_first_person_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F690, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a unit (puts away weapon if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_is_playing_custom_first_person_animation_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_is_playing_custom_first_person_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FC50, // $TODO write the function chuckle nuts
	.documentation = "returns TRUE if the given unit is still playing a custom animation in first person\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_stop_first_person_custom_animation_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_stop_first_person_custom_animation",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740130, // $TODO write the function chuckle nuts
	.documentation = "ends any custom first person animation running on the unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) prepare_to_switch_to_zone_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "prepare_to_switch_to_zone_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007403C0, // $TODO write the function chuckle nuts
	.documentation = "prepare for switch to zone set\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_zone_set
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_zone_activate_for_debugging_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_zone_activate_for_debugging",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740790, // $TODO write the function chuckle nuts
	.documentation = "activates scenario cinematic zone for debugging (automatically generated script function!)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_play_random_ping_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_play_random_ping",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740AB0, // $TODO write the function chuckle nuts
	.documentation = "plays a random ping on the unit",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_control_fade_out_all_input_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_control_fade_out_all_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740DA0, // $TODO write the function chuckle nuts
	.documentation = "fade out all play control over x seconds",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_control_fade_in_all_input_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_control_fade_in_all_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741260, // $TODO write the function chuckle nuts
	.documentation = "fade in all play control over x seconds",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_control_fade_out_all_input_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_control_fade_out_all_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741560, // $TODO write the function chuckle nuts
	.documentation = "fade out all play control for player over x seconds",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unit_control_fade_in_all_input_2_definition
{
	.return_type = _hs_type_void,
	.name = "unit_control_fade_in_all_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741920, // $TODO write the function chuckle nuts
	.documentation = "fade in all play control for player over x seconds",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_control_lock_gaze_3_definition
{
	.return_type = _hs_type_void,
	.name = "player_control_lock_gaze",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741CD0, // $TODO write the function chuckle nuts
	.documentation = "player turn to face point with max velocity degrees/second",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_point_ref,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) player_control_unlock_gaze_1_definition
{
	.return_type = _hs_type_void,
	.name = "player_control_unlock_gaze",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007421A0, // $TODO write the function chuckle nuts
	.documentation = "unlock player from any gaze point",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_control_scale_all_input_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_control_scale_all_input",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007424F0, // $TODO write the function chuckle nuts
	.documentation = "scale input to x strength over y seconds",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) run_like_dvd_0_definition
{
	.return_type = _hs_type_void,
	.name = "run_like_dvd",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007428F0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) profiler_auto_core_save_1_definition
{
	.return_type = _hs_type_void,
	.name = "profiler_auto_core_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742B40, // $TODO write the function chuckle nuts
	.documentation = "profiler dumps a core for each encounter\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) run_no_hdd_0_definition
{
	.return_type = _hs_type_void,
	.name = "run_no_hdd",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742E10, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_tag_reference_get_bink_1_definition
{
	.return_type = _hs_type_bink_definition,
	.name = "cinematic_tag_reference_get_bink",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742F70, // $TODO write the function chuckle nuts
	.documentation = "Retrieve the bink tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) voice_set_force_match_configurations_4_definition
{
	.return_type = _hs_type_void,
	.name = "voice_set_force_match_configurations",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743560, // $TODO write the function chuckle nuts
	.documentation = "sets matchmaking voice options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) voice_set_force_hud_2_definition
{
	.return_type = _hs_type_void,
	.name = "voice_set_force_hud",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007436C0, // $TODO write the function chuckle nuts
	.documentation = "sets matchmaking voice hud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_custom_animation_speed_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_custom_animation_speed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743940, // $TODO write the function chuckle nuts
	.documentation = "if the object is playing a custom animation, scale it to n playback speed",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) scenery_animation_start_at_frame_loop_4_definition
{
	.return_type = _hs_type_void,
	.name = "scenery_animation_start_at_frame_loop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743E10, // $TODO write the function chuckle nuts
	.documentation = "starts a custom animation playing on a piece of scenery at a specific frame and then loops\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_scenery,
		_hs_type_animation_graph,
		_hs_type_string_id,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) saved_film_set_repro_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "saved_film_set_repro_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744280, // $TODO write the function chuckle nuts
	.documentation = "set the saved film into repro mode (splitscreen users are created based on the number of splitscreen players that recorded the film)",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cortana_tag_reference_get_scene_1_definition
{
	.return_type = _hs_type_cinematic_scene_definition,
	.name = "cortana_tag_reference_get_scene",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007445B0, // $TODO write the function chuckle nuts
	.documentation = "Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) net_banhammer_force_download_2_definition
{
	.return_type = _hs_type_void,
	.name = "net_banhammer_force_download",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744AB0, // $TODO write the function chuckle nuts
	.documentation = "force banhammer file download\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) font_set_emergency_0_definition
{
	.return_type = _hs_type_void,
	.name = "font_set_emergency",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744C40, // $TODO write the function chuckle nuts
	.documentation = "sets the font system into emergency mode\r\nNETWORK SAFE: yes",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) biped_force_ground_fitting_on_2_definition
{
	.return_type = _hs_type_void,
	.name = "biped_force_ground_fitting_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744EB0, // $TODO write the function chuckle nuts
	.documentation = "force ground fitting on, even during custom animations or movement",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_chud_objective_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_chud_objective",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745320, // $TODO write the function chuckle nuts
	.documentation = "activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_cutscene_title
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) chud_show_cinematic_title_2_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_cinematic_title",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745640, // $TODO write the function chuckle nuts
	.documentation = "activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_cutscene_title
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) terminal_is_being_read_0_definition
{
	.return_type = _hs_type_boolean,
	.name = "terminal_is_being_read",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745B70, // $TODO write the function chuckle nuts
	.documentation = "returns whether or not a terminal is currently being read",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) terminal_was_accessed_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "terminal_was_accessed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745D80, // $TODO write the function chuckle nuts
	.documentation = "returns whether or not the given terminal was accessed",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) terminal_was_completed_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "terminal_was_completed",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746210, // $TODO write the function chuckle nuts
	.documentation = "returns whether or not the given terminal was read to completion",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_object
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_get_primary_weapon_1_definition
{
	.return_type = _hs_type_weapon,
	.name = "unit_get_primary_weapon",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007465B0, // $TODO write the function chuckle nuts
	.documentation = "return the primary weapon of the specified unit",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dump_cortana_script_0_definition
{
	.return_type = _hs_type_void,
	.name = "dump_cortana_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746C20, // $TODO write the function chuckle nuts
	.documentation = "dump the cinematic script to cinematic_dump_cortana.txt",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) budget_resource_get_animation_graph_1_definition
{
	.return_type = _hs_type_animation_graph,
	.name = "budget_resource_get_animation_graph",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746E50, // $TODO write the function chuckle nuts
	.documentation = "Get a non resolving reference to a model animation graph budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_budget_reference_animation_graph
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_award_level_complete_achievements_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_award_level_complete_achievements",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747210, // $TODO write the function chuckle nuts
	.documentation = "causes the player to successfully finish the current level and move to the next\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) budget_resource_get_looping_sound_1_definition
{
	.return_type = _hs_type_looping_sound,
	.name = "budget_resource_get_looping_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747410, // $TODO write the function chuckle nuts
	.documentation = "Get a non resolving reference to a looping sound budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_budget_reference_looping_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_safe_to_respawn_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_safe_to_respawn",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747840, // $TODO write the function chuckle nuts
	.documentation = "\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cortana_effect_kill_0_definition
{
	.return_type = _hs_type_void,
	.name = "cortana_effect_kill",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747D50, // $TODO write the function chuckle nuts
	.documentation = "kills the cortana effect\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_create_content_item_slayer_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_create_content_item_slayer",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747F10, // $TODO write the function chuckle nuts
	.documentation = "creates a slayer variant\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) test_create_content_item_screenshot_0_definition
{
	.return_type = _hs_type_void,
	.name = "test_create_content_item_screenshot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748080, // $TODO write the function chuckle nuts
	.documentation = "creates a screenshot (which isn't actually viewable)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cinematic_scripting_destroy_cortana_effect_cinematic_0_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_destroy_cortana_effect_cinematic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748300, // $TODO write the function chuckle nuts
	.documentation = "Destroy cortana effect cinematic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_migrate_infanty_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_migrate_infanty",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007485A0, // $TODO write the function chuckle nuts
	.documentation = "migrates the infantry in an encounter into a different squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_cinematic_motion_blur_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_cinematic_motion_blur",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F600, // $TODO write the function chuckle nuts
	.documentation = "enable/disable cinematic motion blur\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_dont_do_avoidance_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_dont_do_avoidance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FA30, // $TODO write the function chuckle nuts
	.documentation = "set the AI to not do any vector avoidance when flying\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_scripting_clean_up_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_clean_up",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FDD0, // $TODO write the function chuckle nuts
	.documentation = "Unload the scenario cinematic zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_erase_inactive_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_erase_inactive",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730250, // $TODO write the function chuckle nuts
	.documentation = "Erase inactive actors in the given ai index, and any of those actors who become inactive within the next designated ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) ai_survival_cleanup_3_definition
{
	.return_type = _hs_type_void,
	.name = "ai_survival_cleanup",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007305F0, // $TODO write the function chuckle nuts
	.documentation = "Activate cure isolation death for the given AI, and optionally delete inactive actors in that group too (ai, on/off, kill inactive)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_boolean,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) stop_bink_movie_0_definition
{
	.return_type = _hs_type_void,
	.name = "stop_bink_movie",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730B40, // $TODO write the function chuckle nuts
	.documentation = "Stops all bink movie playback\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) play_credits_unskippable_0_definition
{
	.return_type = _hs_type_void,
	.name = "play_credits_unskippable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730D30, // $TODO write the function chuckle nuts
	.documentation = "plays credits movie, no button press detection (you can still use cinematic skipping around this)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) budget_resource_get_sound_1_definition
{
	.return_type = _hs_type_sound,
	.name = "budget_resource_get_sound",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730FA0, // $TODO write the function chuckle nuts
	.documentation = "Get a non resolving reference to a looping sound budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_budget_reference_sound
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) controller_set_single_player_level_unlocked_3_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_single_player_level_unlocked",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731460, // $TODO write the function chuckle nuts
	.documentation = "<controller> <level index> <bool locked> set single player level locked state for specified controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_short_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) physical_memory_dump_0_definition
{
	.return_type = _hs_type_void,
	.name = "physical_memory_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731700, // $TODO write the function chuckle nuts
	.documentation = "dumps a record of all physical memory allocations",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) tag_resources_validate_all_pages_1_definition
{
	.return_type = _hs_type_void,
	.name = "tag_resources_validate_all_pages",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731880, // $TODO write the function chuckle nuts
	.documentation = "resource validation every main_loop_body call",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) cinematic_set_debug_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_set_debug_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731AC0, // $TODO write the function chuckle nuts
	.documentation = "Puts all cinematic playback into debug mode (automatically generated script function!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) cinematic_scripting_get_object_3_definition
{
	.return_type = _hs_type_object,
	.name = "cinematic_scripting_get_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731E40, // $TODO write the function chuckle nuts
	.documentation = "Get an object used in a cinematic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) dummy_function_0_definition
{
	.return_type = _hs_type_void,
	.name = "dummy_function",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007322F0, // $TODO write the function chuckle nuts
	.documentation = "Function that does nothing.",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gp_integer_get_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "gp_integer_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007325E0, // $TODO write the function chuckle nuts
	.documentation = "get a game progression integer variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gp_integer_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "gp_integer_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732900, // $TODO write the function chuckle nuts
	.documentation = "set a game progression integer variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gp_boolean_get_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "gp_boolean_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00732D60, // $TODO write the function chuckle nuts
	.documentation = "get a game progression boolean variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) gp_boolean_set_2_definition
{
	.return_type = _hs_type_void,
	.name = "gp_boolean_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007331F0, // $TODO write the function chuckle nuts
	.documentation = "set a game progression boolean variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gp_dump_1_definition
{
	.return_type = _hs_type_void,
	.name = "gp_dump",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733630, // $TODO write the function chuckle nuts
	.documentation = "dumps all game progression variables to a file with specified file name.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gp_dump_debug_1_definition
{
	.return_type = _hs_type_void,
	.name = "gp_dump_debug",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733830, // $TODO write the function chuckle nuts
	.documentation = "dumps all game progression variables to a file with specified file name - from both game options and network sessions parameters.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gp_startup_1_definition
{
	.return_type = _hs_type_void,
	.name = "gp_startup",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733A50, // $TODO write the function chuckle nuts
	.documentation = "sets the file from which to load game progression data on startup.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gp_reset_0_definition
{
	.return_type = _hs_type_void,
	.name = "gp_reset",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733CA0, // $TODO write the function chuckle nuts
	.documentation = "resets all the state of the game progression system\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gp_commit_options_0_definition
{
	.return_type = _hs_type_void,
	.name = "gp_commit_options",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00733EF0, // $TODO write the function chuckle nuts
	.documentation = "debugging command that commits game options to game options so they will persist after a map reset\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_start_screen_effect_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_start_screen_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007341D0, // $TODO write the function chuckle nuts
	.documentation = "Play the associated area screen effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_stop_screen_effect_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_stop_screen_effect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734580, // $TODO write the function chuckle nuts
	.documentation = "Stop the associated area screen effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_level_prepare_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_level_prepare",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734B00, // $TODO write the function chuckle nuts
	.documentation = "prepare to transition game to another level\r\nNETWORK SAFE: safe-ish in deterministic networking as long as it is not invoked from the console",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) prepare_game_level_1_definition
{
	.return_type = _hs_type_void,
	.name = "prepare_game_level",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00734F20, // $TODO write the function chuckle nuts
	.documentation = "prepare to transition game to another level\r\nNETWORK SAFE: safe-ish in deterministic networking as long as it is not invoked from the console",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) game_start_with_network_session_3_definition
{
	.return_type = _hs_type_void,
	.name = "game_start_with_network_session",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007352D0, // $TODO write the function chuckle nuts
	.documentation = "<campaign id> <map id> <scenario> debug map launching: starts a game on the specified map with campaign_id and map_id.\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) levels_add_campaign_map_with_id_2_definition
{
	.return_type = _hs_type_void,
	.name = "levels_add_campaign_map_with_id",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007354F0, // $TODO write the function chuckle nuts
	.documentation = "<map id> <scenario> add scenario to levels array\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) levels_add_campaign_map_1_definition
{
	.return_type = _hs_type_void,
	.name = "levels_add_campaign_map",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735730, // $TODO write the function chuckle nuts
	.documentation = "<scenario> add a single scenario to levels array\r\nNETWORK SAFE: No, for init.txt only",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_impulse_start_editor_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007358E0, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) sound_impulse_start_effect_editor_4_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_effect_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735CB0, // $TODO write the function chuckle nuts
	.documentation = "plays an impulse sound from the specified source object (or \"none\"), with the specified scale and effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_object,
		_hs_type_real,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_impulse_start_3d_editor_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_impulse_start_3d_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00735EB0, // $TODO write the function chuckle nuts
	.documentation = "<sound> <azimuth> <scale> at the sound's minimum distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_sound,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_looping_start_editor_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_start_editor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736010, // $TODO write the function chuckle nuts
	.documentation = "plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_sound_channels_log_start_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_channels_log_start",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736300, // $TODO write the function chuckle nuts
	.documentation = "starts the sound channel debugger log\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_sound_channels_log_start_named_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_channels_log_start_named",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007364C0, // $TODO write the function chuckle nuts
	.documentation = "starts the sound channel debugger log with the specified file name\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_sound_channels_log_stop_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_channels_log_stop",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736790, // $TODO write the function chuckle nuts
	.documentation = "stops the sound channel debugger log\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) cinematic_scripting_set_user_input_constraints_4_definition
{
	.return_type = _hs_type_void,
	.name = "cinematic_scripting_set_user_input_constraints",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736970, // $TODO write the function chuckle nuts
	.documentation = "Set the user input constraints for a cinematic.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) is_skull_primary_enable_2_definition
{
	.return_type = _hs_type_void,
	.name = "is_skull_primary_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00736E80, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_primary_skull,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) is_skull_secondary_enable_2_definition
{
	.return_type = _hs_type_void,
	.name = "is_skull_secondary_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737200, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_secondary_skull,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_popup_message_index_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_popup_message_index",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007375C0, // $TODO write the function chuckle nuts
	.documentation = "<controller> <message index> set the highest popup message that the player has accepted\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) controller_set_vidmaster_seen_2_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_vidmaster_seen",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737990, // $TODO write the function chuckle nuts
	.documentation = "<controller> <vidmaster_seen> set whether the player has accepted the vidmaster popup\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_enter_lobby_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_enter_lobby",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737AC0, // $TODO write the function chuckle nuts
	.documentation = "<lobby index> index 0-4\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_respawn_dead_players_0_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_respawn_dead_players",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00737D30, // $TODO write the function chuckle nuts
	.documentation = "respawns all dead players even if no lives are left\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_lives_get_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "survival_mode_lives_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738080, // $TODO write the function chuckle nuts
	.documentation = "get the number of lives left in survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_lives_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_lives_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007383B0, // $TODO write the function chuckle nuts
	.documentation = "set the number of lives left in survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_set_get_0_definition
{
	.return_type = _hs_type_short_integer,
	.name = "survival_mode_set_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007387B0, // $TODO write the function chuckle nuts
	.documentation = "get the current set index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_set_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_set_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738A30, // $TODO write the function chuckle nuts
	.documentation = "set the current set index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_round_get_0_definition
{
	.return_type = _hs_type_short_integer,
	.name = "survival_mode_round_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00738D00, // $TODO write the function chuckle nuts
	.documentation = "get the current round index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_round_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_round_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739120, // $TODO write the function chuckle nuts
	.documentation = "set the current round index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_wave_get_0_definition
{
	.return_type = _hs_type_short_integer,
	.name = "survival_mode_wave_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007394E0, // $TODO write the function chuckle nuts
	.documentation = "get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_wave_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_wave_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739780, // $TODO write the function chuckle nuts
	.documentation = "set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_set_multiplier_get_0_definition
{
	.return_type = _hs_type_real,
	.name = "survival_mode_set_multiplier_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739B20, // $TODO write the function chuckle nuts
	.documentation = "get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_set_multiplier_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_set_multiplier_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00739EB0, // $TODO write the function chuckle nuts
	.documentation = "set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_bonus_multiplier_get_0_definition
{
	.return_type = _hs_type_real,
	.name = "survival_mode_bonus_multiplier_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A110, // $TODO write the function chuckle nuts
	.documentation = "get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_bonus_multiplier_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_bonus_multiplier_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A520, // $TODO write the function chuckle nuts
	.documentation = "set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_sound_channel_fadeout_time_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_sound_channel_fadeout_time",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073A860, // $TODO write the function chuckle nuts
	.documentation = "set fadeout time for sound channel debugger",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_set_rounds_per_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_set_rounds_per_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073ABB0, // $TODO write the function chuckle nuts
	.documentation = "set the number of rounds in each set for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_set_waves_per_round_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_set_waves_per_round",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073AF60, // $TODO write the function chuckle nuts
	.documentation = "set the number of waves in each round for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) render_model_marker_name_filter_1_definition
{
	.return_type = _hs_type_void,
	.name = "render_model_marker_name_filter",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B210, // $TODO write the function chuckle nuts
	.documentation = "sets a substring to search for when rendering model markers",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) survival_mode_event_new_1_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_event_new",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B400, // $TODO write the function chuckle nuts
	.documentation = "fires a start new set game event\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_begin_new_set_0_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_begin_new_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B7F0, // $TODO write the function chuckle nuts
	.documentation = "Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_begin_new_round_0_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_begin_new_round",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073B9D0, // $TODO write the function chuckle nuts
	.documentation = "Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_begin_new_wave_0_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_begin_new_wave",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BC30, // $TODO write the function chuckle nuts
	.documentation = "Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) survival_mode_award_hero_medal_0_definition
{
	.return_type = _hs_type_void,
	.name = "survival_mode_award_hero_medal",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073BF70, // $TODO write the function chuckle nuts
	.documentation = "lets the engine award the hero medal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) campaign_metagame_get_player_score_1_definition
{
	.return_type = _hs_type_long_integer,
	.name = "campaign_metagame_get_player_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C0E0, // $TODO write the function chuckle nuts
	.documentation = "Calculates the score for an individual player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) collision_debug_lightmaps_print_0_definition
{
	.return_type = _hs_type_void,
	.name = "collision_debug_lightmaps_print",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C4F0, // $TODO write the function chuckle nuts
	.documentation = "Prints the collision_debug_lightmaps text to the console\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) debug_coop_campaign_save_1_definition
{
	.return_type = _hs_type_void,
	.name = "debug_coop_campaign_save",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073C690, // $TODO write the function chuckle nuts
	.documentation = "Enables/disables debugging of co-op campaign save\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_set_look_training_hack_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_set_look_training_hack",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CA60, // $TODO write the function chuckle nuts
	.documentation = "enables or disables the look training hack.  This allows players to touch devices when in a vehicle, but not exit the vehicle or do anything else.",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) device_arg_has_been_touched_by_unit_3_definition
{
	.return_type = _hs_type_boolean,
	.name = "device_arg_has_been_touched_by_unit",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073CDB0, // $TODO write the function chuckle nuts
	.documentation = "Returns true if the unit has touched the device in the past X ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_device,
		_hs_type_unit,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) gui_hide_all_screens_1_definition
{
	.return_type = _hs_type_void,
	.name = "gui_hide_all_screens",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D360, // $TODO write the function chuckle nuts
	.documentation = "Hide all UI screens\r\nNETWORK SAFE: No",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) debug_lightmap_inspect_0_definition
{
	.return_type = _hs_type_void,
	.name = "debug_lightmap_inspect",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D5C0, // $TODO write the function chuckle nuts
	.documentation = "show the lightmap status",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) achievements_display_debug_names_0_definition
{
	.return_type = _hs_type_void,
	.name = "achievements_display_debug_names",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D7C0, // $TODO write the function chuckle nuts
	.documentation = "display debug achievement names",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) achievement_grant_to_controller_by_debug_name_2_definition
{
	.return_type = _hs_type_void,
	.name = "achievement_grant_to_controller_by_debug_name",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DA80, // $TODO write the function chuckle nuts
	.documentation = "unlocks an achievement by debug name for the indicated controller\r\nNETWORK SAFE: NO",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_controller,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) achievements_grant_all_to_controller_1_definition
{
	.return_type = _hs_type_void,
	.name = "achievements_grant_all_to_controller",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DD60, // $TODO write the function chuckle nuts
	.documentation = "grants all achievements to the indicated controller\r\nNETWORK SAFE: NO",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) achievements_award_all_vidmaster_achievements_1_definition
{
	.return_type = _hs_type_void,
	.name = "achievements_award_all_vidmaster_achievements",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073DEF0, // $TODO write the function chuckle nuts
	.documentation = "grants all vidmaster achievements to the indicated controller\r\nNETWORK SAFE: NO",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_controller
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) chud_fade_for_first_person_cinematic_2_definition
{
	.return_type = _hs_type_void,
	.name = "chud_fade_for_first_person_cinematic",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E330, // $TODO write the function chuckle nuts
	.documentation = "Fades all chud elements except for those marked as 'motion sensor'\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) achievement_post_chud_progression_3_definition
{
	.return_type = _hs_type_void,
	.name = "achievement_post_chud_progression",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073E5A0, // $TODO write the function chuckle nuts
	.documentation = "posts an achievement progression toast from script\r\nNETWORK SAFE: Yes",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_string_id,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) object_set_vision_mode_render_default_2_definition
{
	.return_type = _hs_type_void,
	.name = "object_set_vision_mode_render_default",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EA10, // $TODO write the function chuckle nuts
	.documentation = "Flag an object so that it renders in the everything else color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) chud_show_navpoint_4_definition
{
	.return_type = _hs_type_void,
	.name = "chud_show_navpoint",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073EE00, // $TODO write the function chuckle nuts
	.documentation = "Activate a scripted navpoint for a specific player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_cutscene_flag,
		_hs_type_real,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) run_like_package_0_definition
{
	.return_type = _hs_type_void,
	.name = "run_like_package",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F200, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) clear_map_slot_1_definition
{
	.return_type = _hs_type_void,
	.name = "clear_map_slot",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F530, // $TODO write the function chuckle nuts
	.documentation = "nuke cache<number>.map",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) clear_map_type_1_definition
{
	.return_type = _hs_type_void,
	.name = "clear_map_type",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F720, // $TODO write the function chuckle nuts
	.documentation = "nuke all maps corresponding to specified type. 0 = mainmenu, 1 = shared, 2 = campaign, 3 = hub, 4 = linear, 5 = scene, 6 = cinematic",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_confirm_message_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_confirm_message",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073F9B0, // $TODO write the function chuckle nuts
	.documentation = "Locks the accept button until the user presses accept\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_confirm_cancel_message_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_confirm_cancel_message",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FCD0, // $TODO write the function chuckle nuts
	.documentation = "Locks the cancel button until the user presses cancel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_confirm_y_button_1_definition
{
	.return_type = _hs_type_void,
	.name = "unit_confirm_y_button",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740100, // $TODO write the function chuckle nuts
	.documentation = "Locks the accept button until the user presses accept\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) player_get_kills_by_type_3_definition
{
	.return_type = _hs_type_long_integer,
	.name = "player_get_kills_by_type",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740380, // $TODO write the function chuckle nuts
	.documentation = "Retrieves the number of kills a specific player has gotten by metagame type and class, ask an engineer for the mapping\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_unit,
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unit_flashlight_on_1_definition
{
	.return_type = _hs_type_boolean,
	.name = "unit_flashlight_on",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740880, // $TODO write the function chuckle nuts
	.documentation = "returns true if the unit has a flashlight on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_unit
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) clear_command_buffer_cache_from_script_1_definition
{
	.return_type = _hs_type_void,
	.name = "clear_command_buffer_cache_from_script",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00740C60, // $TODO write the function chuckle nuts
	.documentation = "clear the command buffer cache\r\nYes",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) sound_looping_resume_3_definition
{
	.return_type = _hs_type_void,
	.name = "sound_looping_resume",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741160, // $TODO write the function chuckle nuts
	.documentation = "plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_looping_sound,
		_hs_type_object,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) chud_bonus_round_set_target_score_1_definition
{
	.return_type = _hs_type_void,
	.name = "chud_bonus_round_set_target_score",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007414A0, // $TODO write the function chuckle nuts
	.documentation = "set the bonus round score target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown673_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown673",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741890, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unknown674_3_definition
{
	.return_type = _hs_type_void,
	.name = "unknown674",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741AD0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unknown675_4_definition
{
	.return_type = _hs_type_void,
	.name = "unknown675",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741CB0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_short_integer,
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown676_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown676",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00741F00, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown677_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown677",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742220, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown678_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown678",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007424D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown679_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown679",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007426B0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown67A_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown67A",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007428D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown67B_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown67B",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742B10, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown67C_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown67C",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00742E90, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saber_ui_get_player_model_id_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "saber_ui_get_player_model_id",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743070, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saber_ui_get_music_id_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "saber_ui_get_music_id",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743540, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unknown67F_2_definition
{
	.return_type = _hs_type_void,
	.name = "unknown67F",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007436E0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unit_set_armor_3_definition
{
	.return_type = _hs_type_void,
	.name = "unit_set_armor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007439E0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_object,
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) player_set_armor_2_definition
{
	.return_type = _hs_type_void,
	.name = "player_set_armor",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00743F10, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_string_id,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) change_network_privacy_to_system_link_0_definition
{
	.return_type = _hs_type_void,
	.name = "change_network_privacy_to_system_link",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007442D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ssao_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "ssao_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744440, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ssao_radius_1_definition
{
	.return_type = _hs_type_void,
	.name = "ssao_radius",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007448D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ssao_intensity_1_definition
{
	.return_type = _hs_type_void,
	.name = "ssao_intensity",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744BD0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) ssao_sampleZThreshold_1_definition
{
	.return_type = _hs_type_void,
	.name = "ssao_sampleZThreshold",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744FA0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) fxaa_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "fxaa_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007453D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) color_grading_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "color_grading_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007456D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) lightshafts_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "lightshafts_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745B40, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unknown68A_2_definition
{
	.return_type = _hs_type_void,
	.name = "unknown68A",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00745E00, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflections_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflections_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746060, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_blur_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_blur_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746320, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_multisampling_enabled_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_multisampling_enabled",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746740, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_multisample_passes_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_multisample_passes",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746B20, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_fxaa_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_fxaa_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00746E20, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_debug_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_debug_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747160, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) reflection_color_bilinear_filter_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "reflection_color_bilinear_filter_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747500, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) end_match_wait_time_increment_1_definition
{
	.return_type = _hs_type_void,
	.name = "end_match_wait_time_increment",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747800, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown693_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown693",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747C20, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) background_bitmap_set_1_definition
{
	.return_type = _hs_type_void,
	.name = "background_bitmap_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747EC0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_short_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) local_player_earn_wp_event_1_definition
{
	.return_type = _hs_type_void,
	.name = "local_player_earn_wp_event",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007481F0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) global_screen_effect_enable_1_definition
{
	.return_type = _hs_type_void,
	.name = "global_screen_effect_enable",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00748530, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) unknown697_3_definition
{
	.return_type = _hs_type_void,
	.name = "unknown697",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F5C0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_string,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown698_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown698",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072F7E0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saber_mouse_inversion_get_0_definition
{
	.return_type = _hs_type_void,
	.name = "saber_mouse_inversion_get",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FB20, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) saber_mouse_inversion_set_0_definition
{
	.return_type = _hs_type_long_integer,
	.name = "saber_mouse_inversion_set",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FCE0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 14) unknown69B_7_definition
{
	.return_type = _hs_type_void,
	.name = "unknown69B",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0072FFE0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 7,
	.formal_parameters =
	{
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) unknown69C_2_definition
{
	.return_type = _hs_type_void,
	.name = "unknown69C",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007303D0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_long_integer
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown69D_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown69D",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730590, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8) unknown69E_4_definition
{
	.return_type = _hs_type_void,
	.name = "unknown69E",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730780, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 4,
	.formal_parameters =
	{
		_hs_type_long_integer,
		_hs_type_real,
		_hs_type_real,
		_hs_type_real
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) unknown69F_1_definition
{
	.return_type = _hs_type_void,
	.name = "unknown69F",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730B20, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) unknown6A0_0_definition
{
	.return_type = _hs_type_void,
	.name = "unknown6A0",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00730CB0, // $TODO write the function chuckle nuts
	.documentation = "",
	.parameters = NULL,
	.formal_parameter_count = 0,
};

