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

#define MAKE_HS_FUNCTION_DOCUMENTATION(BODY, NETWORK_SAFETY) STRCONCAT(BODY, STRCONCAT("\r\nNETWORK SAFE: ", NETWORK_SAFETY))

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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the last expression in a sequence after evaluating the sequence in order.", "Yes"),
	.parameters = "<expression(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) begin_random_definition
{
	.return_type = _hs_passthrough,
	.name = "begin_random",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_begin,
	.evaluate = hs_evaluate_begin_random,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("evaluates the sequence of expressions in random order and returns the last value evaluated.", "Yes"),
	.parameters = "<expression(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) if_definition
{
	.return_type = _hs_passthrough,
	.name = "if",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_if,
	.evaluate = hs_evaluate_if,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns one of two values based on the value of a condition.", "Yes"),
	.parameters = "<boolean> <then> [<else>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) cond_definition
{
	.return_type = _hs_passthrough,
	.name = "cond",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_cond,
	.evaluate = NULL,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the value associated with the first true condition.", "Yes"),
	.parameters = "(<boolean1> <result1>) [(<boolean2> <result2>) [...]]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) set_definition
{
	.return_type = _hs_passthrough,
	.name = "set",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_set,
	.evaluate = hs_evaluate_set,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("set the value of a global variable.", "Yes (depending on result)"),
	.parameters = "<variable name> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) and_definition
{
	.return_type = _hs_type_boolean,
	.name = "and",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_logical,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if all specified expressions are true.", "Yes"),
	.parameters = "<boolean(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) or_definition
{
	.return_type = _hs_type_boolean,
	.name = "or",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_logical,
	.evaluate = hs_evaluate_logical,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if any specified expressions are true.", "Yes"),
	.parameters = "<boolean(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) add_definition
{
	.return_type = _hs_type_real,
	.name = "+",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the sum of all specified expressions.", "Yes"),
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) subtract_definition
{
	.return_type = _hs_type_real,
	.name = "-",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the difference of two expressions.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) multiply_definition
{
	.return_type = _hs_type_real,
	.name = "*",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the product of all specified expressions.", "Yes"),
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) divide_definition
{
	.return_type = _hs_type_real,
	.name = "/",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the quotient of two expressions.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) modulo_definition
{
	.return_type = _hs_type_real,
	.name = "%",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION(/* maybe? */ "returns the remainder of two expressions", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) min_definition
{
	.return_type = _hs_type_real,
	.name = "min",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the minimum of all specified expressions.", "Yes"),
	.parameters = "<number(s)",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) max_definition
{
	.return_type = _hs_type_real,
	.name = "max",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_arithmetic,
	.evaluate = hs_evaluate_arithmetic,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the maximum of all specified expressions.", "Yes"),
	.parameters = "<number(s)>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) equal_definition
{
	.return_type = _hs_type_boolean,
	.name = "=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_equality,
	.evaluate = hs_evaluate_equality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if two expressions are equal", "Yes"),
	.parameters = "<expression> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) not_equal_definition
{
	.return_type = _hs_type_boolean,
	.name = "!=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_equality,
	.evaluate = hs_evaluate_equality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if two expressions are not equal", "Yes"),
	.parameters = "<expression> <expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gt_definition
{
	.return_type = _hs_type_boolean,
	.name = ">",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if the first number is larger than the second.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) lt_definition
{
	.return_type = _hs_type_boolean,
	.name = "<",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if the first number is smaller than the second.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) gte_definition
{
	.return_type = _hs_type_boolean,
	.name = ">=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if the first number is larger than or equal to the second.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) lte_definition
{
	.return_type = _hs_type_boolean,
	.name = "<=",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inequality,
	.evaluate = hs_evaluate_inequality,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns true if the first number is smaller than or equal to the second.", "Yes"),
	.parameters = "<number> <number>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_definition
{
	.return_type = _hs_type_void,
	.name = "sleep",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep,
	.evaluate = hs_evaluate_sleep,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("pauses execution of this script (or, optionally, another script) for the specified number of ticks @ 30Hz.", "Yes"),
	.parameters = "<short> [<script>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_forever_definition
{
	.return_type = _hs_type_void,
	.name = "sleep_forever",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep_forever,
	.evaluate = hs_evaluate_sleep_forever,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("pauses execution of this script (or, optionally, another script) forever.", "Yes"),
	.parameters = "[<script>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_until_definition
{
	.return_type = _hs_type_boolean,
	.name = "sleep_until",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_sleep_until,
	.evaluate = hs_evaluate_sleep_until,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("pauses execution of this script until the specified condition is true, checking once per second unless a different number of ticks is specified.", "Yes"),
	.parameters = "<boolean> [<short>]",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) wake_definition
{
	.return_type = _hs_type_void,
	.name = "wake",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_wake,
	.evaluate = hs_evaluate_wake,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("wakes a sleeping script in the next update.", "Yes"),
	.parameters = "<script name>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) inspect_definition
{
	.return_type = _hs_type_void,
	.name = "inspect",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_inspect,
	.evaluate = hs_evaluate_inspect,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("prints the value of an expression to the screen for debugging purposes.", "Yes"),
	.parameters = "<expression>",
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) object_to_unit_definition
{
	.return_type = _hs_type_unit,
	.name = "unit",
	.flags = FLAG(_hs_function_flag_internal),
	.parse = hs_parse_object_cast_up,
	.evaluate = hs_evaluate_object_cast_up,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("converts an object to a unit.", "No"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the opposite of the expression.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("returns the first value pinned between the second two", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("prints a string to the console.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("prints a string to the hs log file.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("shows or hides the display of any thread containing the given substring.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("Verbose threads spew to log about script and function calls.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("Turn on/off hs script debugging.", "Yes"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("Turn on/off hs global debugging.", "Yes"),
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("If breakpoints are enabled, pause execution when this statement is hit (displaying the given message).", "Yes"),
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 3,
	.formal_parameters =
	{
		_hs_type_effect,
		_hs_type_object,
		_hs_type_string_id
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) effect_new_on_object_2_definition
{
	.return_type = _hs_type_void,
	.name = "effect_new_on_object",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x007382C0, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_ai,
		_hs_type_ai
	},
};
MACRO_FUNCTION_EVALUATE(ai_allegiance, ai_scripting_allegiance, 2, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) ai_allegiance_2_definition
{
	.return_type = _hs_type_void,
	.name = "ai_allegiance",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(ai_allegiance, ai_scripting_allegiance, 2), // (hs_evaluate_function_definition)0x00738660,
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 2,
	.formal_parameters =
	{
		_hs_type_short_integer,
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4) screenshot_big_jittered_2_definition
{
	.return_type = _hs_type_void,
	.name = "screenshot_big_jittered",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073FC30, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_string
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) texture_cache_0_definition
{
	.return_type = _hs_type_void,
	.name = "texture_cache",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744AF0, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) error_overflow_suppression_1_definition
{
	.return_type = _hs_type_void,
	.name = "error_overflow_suppression",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x0073D800, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_boolean
	},
};
void event_set_display_level_global(int32 display_level)
{
	printf("");
}
MACRO_FUNCTION_EVALUATE(event_global_display_category, event_set_display_level_global, 1, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_global_display_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_global_display_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(event_global_display_category, event_set_display_level_global, 1), // (hs_evaluate_function_definition)0x0073FCB0,
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_event
	},
};
void event_set_log_level_global(int32 log_level)
{
	printf("");
}
MACRO_FUNCTION_EVALUATE(event_global_log_category, event_set_log_level_global, 1, _hs_type_void);
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) event_global_log_category_1_definition
{
	.return_type = _hs_type_void,
	.name = "event_global_log_category",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = MACRO_FUNCTION_EVALUATE_NAME(event_global_log_category, event_set_log_level_global, 1), // (hs_evaluate_function_definition)0x0073FEB0,
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("sets the global event log level", "Unknown, assumed unsafe"),
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
	.documentation = MAKE_HS_FUNCTION_DOCUMENTATION("sets the global event display level", "Unknown, assumed unsafe"),
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) film_manager_set_reproduction_mode_1_definition
{
	.return_type = _hs_type_void,
	.name = "film_manager_set_reproduction_mode",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00744280, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_animation_budget_reference
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) game_award_level_complete_achievements_0_definition
{
	.return_type = _hs_type_void,
	.name = "game_award_level_complete_achievements",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747210, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_looping_sound_budget_reference
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2) game_safe_to_respawn_1_definition
{
	.return_type = _hs_type_void,
	.name = "game_safe_to_respawn",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00747840, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 1,
	.formal_parameters =
	{
		_hs_type_sound_budget_reference
	},
};
DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6) controller_set_single_player_level_unlocked_3_definition
{
	.return_type = _hs_type_void,
	.name = "controller_set_single_player_level_unlocked",
	.flags = 0,
	.parse = hs_macro_function_parse,
	.evaluate = (hs_evaluate_function_definition)0x00731460, // $TODO write the function chuckle nuts
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
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
	.documentation = "$TODO write this",
	.parameters = NULL,
	.formal_parameter_count = 0,
};
#undef MAKE_HS_FUNCTION_DOCUMENTATION

