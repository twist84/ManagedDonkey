#include "hs/hs_function.hpp"

#include "ai/ai.hpp"
#include "ai/ai_script.hpp"
#include "camera/director.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "game/game_engine_scripting.hpp"
#include "game/game_save.hpp"
#include "game/multiplayer_game_hopper.hpp"
#include "hf2p/hf2p.hpp"
#include "hs/hs.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_library_external.hpp"
#include "hs/hs_library_internal_compile.hpp"
#include "hs/hs_looper.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "interface/c_controller.hpp"
#include "interface/gui_location_manager.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "interface/user_interface_window_manager_debug.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_game_launch.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_globals.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "sound/game_sound.hpp"
#include "test/test_functions.hpp"
#include "text/font_loading.hpp"

enum
{
	k_maximum_number_of_ms23_hs_functions = 1697,
	MAX_HS_FUNCTION_PARAMETERS = 12,
};

#pragma region HS_FUNCTION_DEFINITIONS

#define DEFINE_HS_FUNCTION_DEFINITION_STRUCT(STRUCT_NAME, EXTRA_BYTES_SIZE) static $##STRUCT_NAME##$_extra_bytes_##EXTRA_BYTES_SIZE
#define DECLARE_HS_FUNCTION_DEFINITION_STRUCT(STRUCT_NAME, EXTRA_BYTES_SIZE) \
struct $##STRUCT_NAME##$_extra_bytes_##EXTRA_BYTES_SIZE \
{ \
	int16 return_type; \
	const char* name; \
	uns16 flags; \
	bool(__cdecl* parse)(int16 function_index, int32 expression_index); \
	void(__cdecl* evaluate)(int16 function_index, int32 thread_index, bool initialize); \
	const char* documentation; \
	const char* parameters; \
	int16 formal_parameter_count; \
	int16 formal_parameters[EXTRA_BYTES_SIZE / sizeof(int16)]; \
}
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0);  // 0
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 2);  // 1
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 4);  // 2
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 6);  // 3
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 8);  // 4
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 10); // 5
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 12); // 6
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 14); // 7
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 16); // 8
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 18); // 9
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 20); // 10
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 22); // 11
DECLARE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 24); // 12
#undef DECLARE_HS_FUNCTION_DEFINITION_STRUCT

template<typename t_return_type>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)());
template<typename t_return_type, typename t_arg0>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0));
template<typename t_return_type, typename t_arg0, typename t_arg1>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10, typename t_arg11>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10, t_arg11));
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10, typename t_arg11, typename t_arg12>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10, t_arg11, t_arg12));

template<typename... t_formal_parameters>
constexpr size_t count_formal_parameters(t_formal_parameters...)
{
	return sizeof...(t_formal_parameters);
}

#define MACRO_FUNCTION_EVALUATE(STRUCT_NAME, EXTRA_BYTES_SIZE, RETURN_TYPE, NAME, FLAGS, FUNCTION, DOCUMENTATION, PARAMETERS, FORMAL_PARAMETER_COUNT, ...) \
static_assert(EXTRA_BYTES_SIZE == sizeof(int16) * FORMAL_PARAMETER_COUNT); \
static_assert(FORMAL_PARAMETER_COUNT <= MAX_HS_FUNCTION_PARAMETERS); \
static_assert(FORMAL_PARAMETER_COUNT == count_formal_parameters(__VA_ARGS__)); \
void NAME##_##FUNCTION##_##FORMAL_PARAMETER_COUNT##_evaluate(int16 function_index, int32 thread_index, bool initialize) \
{ \
	macro_function_evaluate(function_index, thread_index, initialize, FUNCTION); \
} \
static $##STRUCT_NAME##$_extra_bytes_##EXTRA_BYTES_SIZE NAME##_##FORMAL_PARAMETER_COUNT##_definition = \
{ \
    .return_type = (RETURN_TYPE), \
    .name = #NAME, \
    .flags = (FLAGS), \
    .parse = hs_macro_function_parse, \
    .evaluate = NAME##_##FUNCTION##_##FORMAL_PARAMETER_COUNT##_evaluate, \
    .documentation = (DOCUMENTATION), \
    .parameters = (PARAMETERS), \
    .formal_parameter_count = (FORMAL_PARAMETER_COUNT), \
    .formal_parameters = { __VA_ARGS__ }, \
}

#define MACRO_FUNCTION_EVALUATE2(STRUCT_NAME, EXTRA_BYTES_SIZE, RETURN_TYPE, NAME, FLAGS, FUNCTION_ADDRESS, DOCUMENTATION, PARAMETERS, FORMAL_PARAMETER_COUNT, ...) \
static_assert(EXTRA_BYTES_SIZE == sizeof(int16) * FORMAL_PARAMETER_COUNT); \
static_assert(FORMAL_PARAMETER_COUNT <= MAX_HS_FUNCTION_PARAMETERS); \
static_assert(FORMAL_PARAMETER_COUNT == count_formal_parameters(__VA_ARGS__)); \
static $##STRUCT_NAME##$_extra_bytes_##EXTRA_BYTES_SIZE NAME##_##FORMAL_PARAMETER_COUNT##_definition = \
{ \
    .return_type = (RETURN_TYPE), \
    .name = #NAME, \
    .flags = (FLAGS), \
    .parse = hs_macro_function_parse, \
    .evaluate = (hs_evaluate_function_definition)(FUNCTION_ADDRESS), \
    .documentation = (DOCUMENTATION), \
    .parameters = (PARAMETERS), \
    .formal_parameter_count = (FORMAL_PARAMETER_COUNT), \
    .formal_parameters = { __VA_ARGS__ }, \
}

#define MACRO_FUNCTION_EVALUATE3(STRUCT_NAME, EXTRA_BYTES_SIZE, RETURN_TYPE, NAME, FLAGS, FUNCTION, FUNCTION_ADDRESS, DOCUMENTATION, PARAMETERS, FORMAL_PARAMETER_COUNT, ...) \
static_assert(EXTRA_BYTES_SIZE == sizeof(int16) * FORMAL_PARAMETER_COUNT); \
static_assert(FORMAL_PARAMETER_COUNT <= MAX_HS_FUNCTION_PARAMETERS); \
static_assert(FORMAL_PARAMETER_COUNT == count_formal_parameters(__VA_ARGS__)); \
static $##STRUCT_NAME##$_extra_bytes_##EXTRA_BYTES_SIZE FUNCTION##_##FORMAL_PARAMETER_COUNT##_definition = \
{ \
    .return_type = (RETURN_TYPE), \
    .name = #NAME, \
    .flags = (FLAGS), \
    .parse = hs_macro_function_parse, \
    .evaluate = (hs_evaluate_function_definition)(FUNCTION_ADDRESS), \
    .documentation = (DOCUMENTATION), \
    .parameters = (PARAMETERS), \
    .formal_parameter_count = (FORMAL_PARAMETER_COUNT), \
    .formal_parameters = { __VA_ARGS__ }, \
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
	.evaluate = hs_evaluate_logical,
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
	.documentation = "returns the remainder of the division of two numbers\r\nNETWORK SAFE: Yes",
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
//DEFINE_HS_FUNCTION_DEFINITION_STRUCT(hs_function_definition, 0) sleep_for_ticks_definition
//{
//	.return_type = _hs_type_void,
//	.name = "sleep_for_ticks",
//	.flags = FLAG(_hs_function_flag_internal),
//	.parse = (hs_parse_function_definition)NULL, // $TODO write the function chuckle nuts
//	.evaluate = (hs_evaluate_function_definition)NULL, // $TODO write the function chuckle nuts
//	.documentation = "pauses execution of this script (or, optionally, another script) for the specified number of ticks.\r\nNETWORK SAFE: Yes",
//	.parameters = "<short> [<script>]",
//};
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
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	evaluate,
	0,
	0x00748490, // $TODO write the function chuckle nuts
	"Evaluate the given script",
	NULL,
	1, _hs_type_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	not,
	0,
	0x00748840, // $TODO write the function chuckle nuts
	"returns the opposite of the expression.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	pin,
	0,
	0x0072F9B0, // $TODO write the function chuckle nuts
	"returns the first value pinned between the second two\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	print,
	0,
	hs_print, // 0x0072FE80
	"prints a string to the console.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	log_print,
	0,
	hs_log_print, // 0x007302D0
	"prints a string to the hs log file.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_scripting_show_thread,
	0,
	0x007305D0, // $TODO write the function chuckle nuts
	"shows or hides the display of any thread containing the given substring.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_script_thread,
	0,
	0x00730900, // $TODO write the function chuckle nuts
	"Verbose threads spew to log about script and function calls.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_scripting,
	0,
	0x00730B80, // $TODO write the function chuckle nuts
	"Turn on/off hs script debugging.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_scripting_globals,
	0,
	0x00730E80, // $TODO write the function chuckle nuts
	"Turn on/off hs global debugging.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_scripting_variable,
	0,
	0x00731220, // $TODO write the function chuckle nuts
	"Turn on/off debugging of a specific global variable; enclose the variable name with single quote characters ('my_variable')\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_scripting_variable_all,
	0,
	0x007316D0, // $TODO write the function chuckle nuts
	"Turn on/off debugging of all global variables\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	breakpoint,
	0,
	hs_breakpoint, // 0x00731950
	"If breakpoints are enabled, pause execution when this statement is hit (displaying the given message).\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	kill_active_scripts,
	0,
	0x00731CD0, // $TODO write the function chuckle nuts
	"Terminates all currently running threads.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	get_executing_running_thread,
	0,
	0x00731F10, // $TODO write the function chuckle nuts
	"Retrieves the current executing thread index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	kill_thread,
	0,
	0x007322A0, // $TODO write the function chuckle nuts
	"Kill the specified thread\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	script_started,
	0,
	0x00732630, // $TODO write the function chuckle nuts
	"Returns true if the continuous, dormant or startup script was started.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	script_finished,
	0,
	0x00732A20, // $TODO write the function chuckle nuts
	"Returns true if the continuous, dormant or startup script was finished.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);

MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_object_list,
	players,
	0,
	0x00732E60, // $TODO write the function chuckle nuts
	"returns a list of the players (DO NOT USE TO COUNT PLAYERS IN COOP GAME, USE game_coop_player_count)\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_unit,
	player_get,
	0,
	0x007331C0, // $TODO write the function chuckle nuts
	"Get a player by absolute index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	kill_volume_enable,
	0,
	0x00733600, // $TODO write the function chuckle nuts
	"enables a kill volume\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_trigger_volume
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	kill_volume_disable,
	0,
	0x007339B0, // $TODO write the function chuckle nuts
	"disables a kill volume\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_trigger_volume
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	volume_teleport_players_not_inside,
	0,
	0x00733C60, // $TODO write the function chuckle nuts
	"moves all players outside a specified trigger volume to a specified flag.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_trigger_volume, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	volume_test_object,
	0,
	0x00734130, // $TODO write the function chuckle nuts
	"returns true if the specified object is within the specified volume.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_trigger_volume, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	volume_test_objects,
	0,
	0x007344F0, // $TODO write the function chuckle nuts
	"returns true if any of the specified objects are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_trigger_volume, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	volume_test_objects_all,
	0,
	0x00734A70, // $TODO write the function chuckle nuts
	"returns true if any of the specified objects are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_trigger_volume, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	volume_test_players,
	0,
	0x00734F90, // $TODO write the function chuckle nuts
	"returns true if any players are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_trigger_volume
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	volume_test_players_all,
	0,
	0x00735430, // $TODO write the function chuckle nuts
	"returns true if all players are within the specified volume. trigger volume must have been postprocessed\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_trigger_volume
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object_list,
	volume_return_objects,
	0,
	0x007357C0, // $TODO write the function chuckle nuts
	"returns list of objects in volume or (max 128)\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_trigger_volume
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_object_list,
	volume_return_objects_by_type,
	0,
	0x00735CF0, // $TODO write the function chuckle nuts
	"returns list of objects in volume or (max 128).\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_trigger_volume, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	zone_set_trigger_volume_enable,
	0,
	0x00736100, // $TODO write the function chuckle nuts
	"enables/disables the trigger volume(s) with the given name that cause zone set switches\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_object,
	list_get,
	0,
	0x00736480, // $TODO write the function chuckle nuts
	"returns an item in an object list.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object_list, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	list_count,
	0,
	0x00736930, // $TODO write the function chuckle nuts
	"returns the number of objects in a list\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	list_count_not_dead,
	0,
	0x00736E40, // $TODO write the function chuckle nuts
	"returns the number of objects in a list that aren't dead\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	effect_new,
	0,
	0x007371A0, // $TODO write the function chuckle nuts
	"starts the specified effect at the specified flag.\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_effect, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	effect_new_random,
	0,
	0x00737580, // $TODO write the function chuckle nuts
	"starts the specified effect at one of the points in the given a point set.\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_effect, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	effect_new_at_ai_point,
	0,
	0x00737A30, // $TODO write the function chuckle nuts
	"starts the specified effect at the specified ai point.\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_effect, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	effect_new_on_object_marker,
	0,
	0x00737D50, // $TODO write the function chuckle nuts
	"starts the specified effect on the specified object at the specified marker.""\r\nNETWORK SAFE: ""No (partial support exists, no general solution)",
	NULL,
	3, _hs_type_effect, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	effect_new_on_ground,
	0,
	0x007382C0, // $TODO write the function chuckle nuts
	"starts the specified effect on the ground underneath the objects root.\r\nNETWORK SAFE: Dunno",
	NULL,
	2, _hs_type_effect, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	damage_new,
	0,
	0x007386E0, // $TODO write the function chuckle nuts
	"causes the specified damage at the specified flag.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_damage, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	damage_object_effect,
	0,
	0x00738AC0, // $TODO write the function chuckle nuts
	"causes the specified damage at the specified object.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_damage, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	damage_objects_effect,
	0,
	0x00738E60, // $TODO write the function chuckle nuts
	"causes the specified damage at the specified object list.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_damage, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	damage_object,
	0,
	0x00739270, // $TODO write the function chuckle nuts
	"causes the specified damage at the specified object.\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	damage_objects,
	0,
	0x007396E0, // $TODO write the function chuckle nuts
	"causes the specified damage at the specified object list.\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object_list, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	damage_players,
	0,
	0x00739AF0, // $TODO write the function chuckle nuts
	"damages all players with the given damage effect\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_damage
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	soft_ceiling_enable,
	0,
	0x00739F20, // $TODO write the function chuckle nuts
	"turn on or off a soft ceiling\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create,
	0,
	0x0073A270, // $TODO write the function chuckle nuts
	"creates an object from the scenario.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_clone,
	0,
	0x0073A700, // $TODO write the function chuckle nuts
	"creates an object, potentially resulting in multiple objects if it already exists.\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_anew,
	0,
	0x0073A9B0, // $TODO write the function chuckle nuts
	"creates an object, destroying it first if it already exists.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_if_necessary,
	0,
	0x0073ADC0, // $TODO write the function chuckle nuts
	"creates an object if it doesn't already exists.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_containing,
	0,
	0x0073B0E0, // $TODO write the function chuckle nuts
	"creates all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_clone_containing,
	0,
	0x0073B3D0, // $TODO write the function chuckle nuts
	"creates clones for all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_anew_containing,
	0,
	0x0073B7C0, // $TODO write the function chuckle nuts
	"creates anew all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_folder,
	0,
	0x0073BB30, // $TODO write the function chuckle nuts
	"creates all the objects in the given folder\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_folder
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_create_folder_anew,
	0,
	0x0073BF40, // $TODO write the function chuckle nuts
	"creates all the objects in the given folder\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_folder
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_destroy,
	0,
	0x0073C2A0, // $TODO write the function chuckle nuts
	"destroys an object.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_destroy_containing,
	0,
	0x0073C570, // $TODO write the function chuckle nuts
	"destroys all objects from the scenario whose names contain the given substring.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	object_destroy_all,
	0,
	0x0073CA20, // $TODO write the function chuckle nuts
	"destroys all non player objects.\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_destroy_type_mask,
	0,
	0x0073CC10, // $TODO write the function chuckle nuts
	"destroys all objects matching the type mask\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objects_delete_by_definition,
	0,
	0x0073CF60, // $TODO write the function chuckle nuts
	"deletes all objects of type <definition>\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_destroy_folder,
	0,
	0x0073D2F0, // $TODO write the function chuckle nuts
	"destroys all objects in the given folder.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_folder
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_hide,
	0,
	0x0073D620, // $TODO write the function chuckle nuts
	"hides or shows the object passed in\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_shadowless,
	0,
	0x0073DA40, // $TODO write the function chuckle nuts
	"set/reset shadow castingness of object\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	object_buckling_magnitude_get,
	0,
	0x0073DE90, // $TODO write the function chuckle nuts
	"returns the amoount [0-1] that a scarab is buckling\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_function_set,
	0,
	0x0073E3C0, // $TODO write the function chuckle nuts
	"sets a global object function (0-3) to value\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	object_set_function_variable,
	0,
	0x0073E6A0, // $TODO write the function chuckle nuts
	"sets funciton variable for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_object, _hs_type_string_id, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_clear_function_variable,
	0,
	0x0073EA70, // $TODO write the function chuckle nuts
	"clears one funciton variables for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_clear_all_function_variables,
	0,
	0x0073EE90, // $TODO write the function chuckle nuts
	"clears all funciton variables for sin-o-matic use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_dynamic_simulation_disable,
	0,
	0x0073F240, // $TODO write the function chuckle nuts
	"disabled dynamic simulation for this object (makes it fixed)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_phantom_power,
	0,
	0x0073F650, // $TODO write the function chuckle nuts
	"sets phantom power to be latched at 1.0f or 0.0f\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_wake_physics,
	0,
	0x0073FA90, // $TODO write the function chuckle nuts
	"wakes physics of an object.  For example it would make an usupported crate fall\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_ranged_attack_inhibited,
	0,
	0x0073FD60, // $TODO write the function chuckle nuts
	"FALSE prevents object from using ranged attack\r\nNETWORK SAFE: Yes (actors only)",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_melee_attack_inhibited,
	0,
	0x007401C0, // $TODO write the function chuckle nuts
	"FALSE prevents object from using melee attack\r\nNETWORK SAFE: Yes (actors only)",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	objects_dump_memory,
	0,
	0x007405D0, // $TODO write the function chuckle nuts
	"debugs object memory usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	object_get_health,
	0,
	0x00740820, // $TODO write the function chuckle nuts
	"returns the health [0,1] of the object, returns -1 if the object does not exist\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	object_get_shield,
	0,
	0x00740C00, // $TODO write the function chuckle nuts
	"returns the shield [0,1] of the object, returns -1 if the object does not exist\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	object_set_shield_effect,
	0,
	0x00741110, // $TODO write the function chuckle nuts
	"sets the shield response effect (not current shield amount) to a given value over the given number of seconds (cinematic use only, remember to call (object_set_shield_effect 0 0) after use!)\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_object, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_physics,
	0,
	0x00741460, // $TODO write the function chuckle nuts
	"prevents an object from running physics or colliding with any other objects\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object,
	object_get_parent,
	0,
	0x007418B0, // $TODO write the function chuckle nuts
	"returns the parent of the given object\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	objects_attach,
	0,
	0x00741C70, // $TODO write the function chuckle nuts
	"attaches the second object to the first both strings can be empty\r\nNETWORK SAFE: No",
	NULL,
	4, _hs_type_object, _hs_type_string_id, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_object,
	object_at_marker,
	0,
	0x007421F0, // $TODO write the function chuckle nuts
	"returns the object attached to the marker of the given parent object\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	objects_detach,
	0,
	0x00742670, // $TODO write the function chuckle nuts
	"detaches from the given parent object the given child object\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	object_set_scale,
	0,
	0x00742A30, // $TODO write the function chuckle nuts
	"sets the scale for a given object and interpolates over the given number of frames to achieve that scale\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_object, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_velocity,
	0,
	0x00742E50, // $TODO write the function chuckle nuts
	"Sets the (object-relative) forward velocity of the given object\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_inertia_tensor_scale,
	0,
	0x00743300, // $TODO write the function chuckle nuts
	"0==default of 1.0, otherwise it is scale on this objects inertia tensor\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_collision_damage_armor_scale,
	0,
	0x00743680, // $TODO write the function chuckle nuts
	"0==default of 1.0, otherwise it is scale on how this objects takes collisoin damage, >1 == more protection, <1 == less\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	object_set_velocity,
	0,
	0x00743BA0, // $TODO write the function chuckle nuts
	"Sets the (object-relative) velocity of the given object\r\nNETWORK SAFE: Yes",
	NULL,
	4, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_set_deleted_when_deactivated,
	0,
	0x00743F70, // $TODO write the function chuckle nuts
	"when this object deactivates it will be deleted\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	object_model_target_destroyed,
	0,
	0x00744330, // $TODO write the function chuckle nuts
	"returns TRUE if the specified model target is destroyed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_short_integer,
	object_model_targets_destroyed,
	0,
	0x00744890, // $TODO write the function chuckle nuts
	"returns TRUE if the specified model target is destroyed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	object_damage_damage_section,
	0,
	0x00744C80, // $TODO write the function chuckle nuts
	"applies damage to a damage section, causing all manner of effects/constraint breakage to occur\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_cannot_die,
	0,
	0x007451F0, // $TODO write the function chuckle nuts
	"Set whether the object can die from damage or not (as opposed to by scripting)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	object_vitality_pinned,
	0,
	0x00745460, // $TODO write the function chuckle nuts
	"returns TRUE if the object's vitality is currently pinned at some minimum value because it cannot die.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	garbage_collect_now,
	0,
	0x00745AC0, // $TODO write the function chuckle nuts
	"causes all garbage objects except those visible to a player to be collected immediately\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	garbage_collect_unsafe,
	0,
	0x00745C20, // $TODO write the function chuckle nuts
	"forces all garbage objects to be collected immediately, even those visible to a player (dangerous!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	garbage_collect_multiplayer,
	0,
	0x00745E40, // $TODO write the function chuckle nuts
	"runs multiplayer garbage collection\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_cannot_take_damage,
	0,
	0x00746110, // $TODO write the function chuckle nuts
	"prevents an object from taking damage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_can_take_damage,
	0,
	0x00746410, // $TODO write the function chuckle nuts
	"allows an object to take damage again\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_cinematic_lod,
	0,
	0x007467B0, // $TODO write the function chuckle nuts
	"makes an object use the highest lod for the remainder of the levels' cutscenes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_cinematic_collision,
	0,
	0x00746B70, // $TODO write the function chuckle nuts
	"makes an object not collide with other cinematic collision objects.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_cinematic_visibility,
	0,
	0x00746EE0, // $TODO write the function chuckle nuts
	"makes an object bypass visibility and always render during cinematics.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objects_predict,
	0,
	0x00747290, // $TODO write the function chuckle nuts
	"loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objects_predict_high,
	0,
	0x00747680, // $TODO write the function chuckle nuts
	"loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objects_predict_low,
	0,
	0x00747950, // $TODO write the function chuckle nuts
	"loads textures/geometry/sounds necessary to present objects that are about to come on-screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_type_predict_high,
	0,
	0x00747E10, // $TODO write the function chuckle nuts
	"loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_type_predict_low,
	0,
	0x00748050, // $TODO write the function chuckle nuts
	"loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_type_predict,
	0,
	0x00748460, // $TODO write the function chuckle nuts
	"loads textures necessary to draw an object that's about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_teleport,
	0,
	0x00748800, // $TODO write the function chuckle nuts
	"moves the specified object to the specified flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_teleport_to_ai_point,
	0,
	0x0072F8C0, // $TODO write the function chuckle nuts
	"moves the specified object to the specified ai point.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_facing,
	0,
	0x0072FCA0, // $TODO write the function chuckle nuts
	"turns the specified object in the direction of the specified flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_shield,
	0,
	0x00730110, // $TODO write the function chuckle nuts
	"sets the shield vitality of the specified object (between 0 and 1).  This does the wrong thing.  Don't use it.\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_shield_normalized,
	0,
	0x00730550, // $TODO write the function chuckle nuts
	"sets the shield vitality of the specified object (between 0 and 1).\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_shield_stun,
	0,
	0x007309D0, // $TODO write the function chuckle nuts
	"set how long the shield will be stunned for, 0 is unstunned\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_set_shield_stun_infinite,
	0,
	0x00730D70, // $TODO write the function chuckle nuts
	"make this objects shield be stunned permanently\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	object_set_permutation,
	0,
	0x00731010, // $TODO write the function chuckle nuts
	"sets the desired region (use \"\" for all regions) to the permutation with the given name, e.g. (object_set_permutation flood \"right arm\" ~damaged)\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_variant,
	0,
	0x007315C0, // $TODO write the function chuckle nuts
	"sets the specified object to the specified model variant\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	object_set_region_state,
	0,
	0x00731910, // $TODO write the function chuckle nuts
	"sets the desired region (use \"\" for all regions) to the model state with the given name, e.g. (object_set_region_state marine head destroyed)\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_enum_model_state
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	objects_can_see_object,
	0,
	0x00731D20, // $TODO write the function chuckle nuts
	"returns true if any of the specified units are looking within the specified number of degrees of the object.\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object_list, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	objects_can_see_flag,
	0,
	0x00732250, // $TODO write the function chuckle nuts
	"returns true if any of the specified units are looking within the\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object_list, _hs_type_cutscene_flag, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	objects_distance_to_object,
	0,
	0x00732750, // $TODO write the function chuckle nuts
	"returns minimum distance from any of the specified objects to the specified destination object. (returns -1 if there are no objects to check)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object_list, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	objects_distance_to_flag,
	0,
	0x00732BF0, // $TODO write the function chuckle nuts
	"returns minimum distance from any of the specified objects to the specified flag. (returns -1 if there are no objects, or no flag, to check)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object_list, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	map_info,
	0,
	0x007330F0, // $TODO write the function chuckle nuts
	"prints the map, zone set, active bsps, and current bsp",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	position_predict,
	0,
	0x00733270, // $TODO write the function chuckle nuts
	"in: x, y, z position. loads textures/geometry/sounds necessary to present locations that are about to come on-screen.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	shader_predict,
	0,
	0x00733760, // $TODO write the function chuckle nuts
	"in: shader name. loads textures necessary for a shader.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_shader
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bitmap_predict,
	0,
	0x00733AB0, // $TODO write the function chuckle nuts
	"in: bitmap name. loads all the bitmaps in that bitmap group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_bitmap
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	script_recompile,
	0,
	0x00733E70, // $TODO write the function chuckle nuts
	"recompiles scripts.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	script_doc,
	0,
	hs_doc, // 0x00734110
	"saves a file called hs_doc.txt with parameters for all script commands.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	help,
	0,
	hs_help, // 0x007342D0
	"prints a description of the named function.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_object_list,
	game_engine_objects,
	0,
	0x007344D0, // $TODO write the function chuckle nuts
	"returns a list of the special game engine objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_short_integer,
	random_range,
	0,
	0x007348C0, // $TODO write the function chuckle nuts
	"returns a random value in the range [lower bound, upper bound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_short_integer, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	real_random_range,
	0,
	0x00734DD0, // $TODO write the function chuckle nuts
	"returns a random value in the range [lower bound, upper bound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	physics_constants_reset,
	0,
	0x00735410, // $TODO write the function chuckle nuts
	"resets all physics constants to earthly values\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	physics_set_gravity,
	0,
	0x00735620, // $TODO write the function chuckle nuts
	"set global gravity acceleration relative to halo standard gravity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	physics_set_velocity_frame,
	0,
	0x00735890, // $TODO write the function chuckle nuts
	"sets a local frame of motion for updating physics of things that wish to respect it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	physics_disable_character_ground_adhesion_forces,
	0,
	0x00735E70, // $TODO write the function chuckle nuts
	"turn off ground adhesion forces so you can play tricks with gravity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	havok_debug_start,
	0,
	0x00736140, // $TODO write the function chuckle nuts
	"start up the havok visual debugger\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	havok_dump_world,
	0,
	0x00736460, // $TODO write the function chuckle nuts
	"dump the state of the havok world, with our without a movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	havok_dump_world_close_movie,
	0,
	0x00736680, // $TODO write the function chuckle nuts
	"end the capture of a havok dump movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	havok_profile_start,
	0,
	0x00736850, // $TODO write the function chuckle nuts
	"start profiling havok\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	havok_profile_end,
	0,
	0x00736A80, // $TODO write the function chuckle nuts
	"stop profiling havok, and save the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	havok_profile_range,
	0,
	0x00736D60, // $TODO write the function chuckle nuts
	"captures timers and a movie of a specific tick range\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	havok_reset_allocated_state,
	0,
	0x00736F90, // $TODO write the function chuckle nuts
	"strips down and builds back up all allocated havok memory\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	breakable_surfaces_enable,
	0,
	0x00737170, // $TODO write the function chuckle nuts
	"enables or disables breakability of all breakable surfaces on level\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	breakable_surfaces_reset,
	0,
	0x00737540, // $TODO write the function chuckle nuts
	"restores all breakable surfaces\r\nNETWORK SAFE: No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	recording_play,
	0,
	0x007377C0, // $TODO write the function chuckle nuts
	"make the specified unit run the specified cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_cutscene_recording
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	recording_play_and_delete,
	0,
	0x00737BE0, // $TODO write the function chuckle nuts
	"make the specified unit run the specified cutscene recording, deletes the unit when the animation finishes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_cutscene_recording
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	recording_play_and_hover,
	0,
	0x00738180, // $TODO write the function chuckle nuts
	"make the specified vehicle run the specified cutscene recording, hovers the vehicle when the animation finishes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_vehicle, _hs_type_cutscene_recording
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	recording_kill,
	0,
	0x007385F0, // $TODO write the function chuckle nuts
	"kill the specified unit's cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	recording_time,
	0,
	0x00738910, // $TODO write the function chuckle nuts
	"return the time remaining in the specified unit's cutscene recording.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	render_lights,
	0,
	0x00738DC0, // $TODO write the function chuckle nuts
	"enables/disables dynamic lights\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	print_light_state,
	0,
	0x00739250, // $TODO write the function chuckle nuts
	"outputs text light state\r\nNETWORK SAFE: yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	render_lights_enable_cinematic_shadow,
	0,
	0x00739550, // $TODO write the function chuckle nuts
	"enable/disable the specified unit to receive cinematic shadows where the shadow is focused about a radius around a marker name\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_boolean, _hs_type_object, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	texture_camera_set_object_marker,
	0,
	0x00739970, // $TODO write the function chuckle nuts
	"sets the render texture camera to a given object marker",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	texture_camera_set_position,
	0,
	0x00739DF0, // $TODO write the function chuckle nuts
	"sets the render texture camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	texture_camera_set_target,
	0,
	0x0073A220, // $TODO write the function chuckle nuts
	"sets the render texture camera target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	texture_camera_attach_to_object,
	0,
	0x0073A730, // $TODO write the function chuckle nuts
	"attaches the render texture camera to a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	texture_camera_target_object,
	0,
	0x0073AA40, // $TODO write the function chuckle nuts
	"targets the render texture camera to view a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	texture_camera_position_world_offset,
	0,
	0x0073AF10, // $TODO write the function chuckle nuts
	"adds a worldspace offset to the current texture camera position",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	texture_camera_on,
	0,
	0x0073B260, // $TODO write the function chuckle nuts
	"turns on the render texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
//MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
//	_hs_type_void,
//	texture_camera_bink,
//	0
//	NULL, // $TODO write the function chuckle nuts
//	"turns on the render texture camera and renders a bink to it\r\nNETWORK SAFE: Unknown, assumed unsafe",
//	NULL,
//	0,
//);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	texture_camera_off,
	0,
	0x0073B430, // $TODO write the function chuckle nuts
	"turns off the render texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_set_aspect_ratio,
	0,
	0x0073B760, // $TODO write the function chuckle nuts
	"sets the texture camera aspet ratio\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	texture_camera_set_resolution,
	0,
	0x0073BA40, // $TODO write the function chuckle nuts
	"sets the texture camera render resolution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_render_mode,
	0,
	0x0073BF10, // $TODO write the function chuckle nuts
	"switches the texture camera render mode",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_unknownBC,
	0,
	0x0073C270, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_set_fov,
	0,
	0x0073C530, // $TODO write the function chuckle nuts
	"sets the field of view on the texture camera, in degrees",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_set_fov_frame_target,
	0,
	0x0073C9E0, // $TODO write the function chuckle nuts
	"zooms the field of view to frame the target, with target zoom factor to zoom closer",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	texture_camera_enable_dynamic_lights,
	0,
	0x0073CCE0, // $TODO write the function chuckle nuts
	"toggle rendering of dynamic lights in the texture camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_camera_on,
	0,
	0x0073D130, // $TODO write the function chuckle nuts
	"turns on the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_camera_off,
	0,
	0x0073D410, // $TODO write the function chuckle nuts
	"turns off the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	hud_camera_set_position,
	0,
	0x0073D740, // $TODO write the function chuckle nuts
	"sets the render texture camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	hud_camera_set_target,
	0,
	0x0073DC10, // $TODO write the function chuckle nuts
	"sets the render texture camera target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	hud_camera_attach_to_object,
	0,
	0x0073E140, // $TODO write the function chuckle nuts
	"attaches the render texture camera to a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	hud_camera_target_object,
	0,
	0x0073E560, // $TODO write the function chuckle nuts
	"targets the render texture camera to view a given object marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	hud_camera_structure,
	0,
	0x0073E9D0, // $TODO write the function chuckle nuts
	"toggle rendering of structure in the hud camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	hud_camera_highlight_object,
	0,
	0x0073EDA0, // $TODO write the function chuckle nuts
	"highlights an object in the given color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_long_integer, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_camera_clear_objects,
	0,
	0x0073F380, // $TODO write the function chuckle nuts
	"clears all objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	hud_camera_spin_around,
	0,
	0x0073F610, // $TODO write the function chuckle nuts
	"makes the hud camera spin around the given object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_camera_from_player_view,
	0,
	0x0073FA60, // $TODO write the function chuckle nuts
	"aligns the hud camera to the player camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	hud_camera_window,
	0,
	0x0073FD00, // $TODO write the function chuckle nuts
	"sets the render window of the hud camera (0...1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_texture_camera,
	0,
	0x00740300, // $TODO write the function chuckle nuts
	"toggles displaying the texture camera in the corner of the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_structure_cluster,
	0,
	0x007405B0, // $TODO write the function chuckle nuts
	"specify a structure and cluster to debug\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_cluster_fog,
	0,
	0x00740800, // $TODO write the function chuckle nuts
	"enables cluster fog debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_fog_plane,
	0,
	0x00740A30, // $TODO write the function chuckle nuts
	"enables fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_fog_plane_infinite_extent,
	0,
	0x00740BE0, // $TODO write the function chuckle nuts
	"enables fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_fog_zone,
	0,
	0x00740E00, // $TODO write the function chuckle nuts
	"enabled fog zone debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_fog_zone_floodfill,
	0,
	0x007411C0, // $TODO write the function chuckle nuts
	"enables fog zone debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_all_fog_planes,
	0,
	0x00741330, // $TODO write the function chuckle nuts
	"enables fog plane debugging (all)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_all_cluster_errors,
	0,
	0x007415F0, // $TODO write the function chuckle nuts
	"enables cluster error debugging (all)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	render_debug_structure_line_opacity,
	0,
	0x00741810, // $TODO write the function chuckle nuts
	"sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	render_debug_structure_text_opacity,
	0,
	0x00741A90, // $TODO write the function chuckle nuts
	"sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	render_debug_structure_opacity,
	0,
	0x00741BF0, // $TODO write the function chuckle nuts
	"sets the opacity (0 is default)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_structure_non_occluded_fog_planes,
	0,
	0x00741E30, // $TODO write the function chuckle nuts
	"controls non-occluded fog plane debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_structure_lightmaps_use_pervertex,
	0,
	0x00742180, // $TODO write the function chuckle nuts
	"sets the lightmap preference to per-vertex, ignores per-pixel if available\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_structure_lightmaps_use_reset,
	0,
	0x00742390, // $TODO write the function chuckle nuts
	"resets the lightmap preference setting and updates the internal lightmap bsp data if necessary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_structure_lightmaps_sample_enable,
	0,
	0x007425F0, // $TODO write the function chuckle nuts
	"enables visualization of match samples if available\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_structure_lightmaps_sample_disable,
	0,
	0x00742880, // $TODO write the function chuckle nuts
	"disables visualization of match samples\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_query_object_bitmaps,
	0,
	0x00742AF0, // $TODO write the function chuckle nuts
	"substring of object definition name to spew out it's debug usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	render_debug_query_bsp_resources,
	0,
	0x00742DB0, // $TODO write the function chuckle nuts
	"BSP index and whether to spit out all bitmaps used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_query_all_object_resources,
	0,
	0x00742F10, // $TODO write the function chuckle nuts
	"Outputs text of texture and geometry memory usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_query_d3d_resources,
	0,
	0x00743170, // $TODO write the function chuckle nuts
	"Outputs text of d3d resource usage\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_debug_text_using_simple_font,
	0,
	0x00743520, // $TODO write the function chuckle nuts
	"toggles use of the much faster simple_font system for debug text rendering\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_postprocess_color_tweaking_reset,
	0,
	0x00743660, // $TODO write the function chuckle nuts
	"resets hue saturation filters to default\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	scenery_animation_start,
	0,
	0x00743900, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	scenery_animation_start_loop,
	0,
	0x00743E50, // $TODO write the function chuckle nuts
	"starts a custom looping animation playing on a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenery_animation_start_relative,
	0,
	0x00744240, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a piece of scenery relative to a parent object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenery_animation_start_relative_loop,
	0,
	0x00744740, // $TODO write the function chuckle nuts
	"starts a custom looping animation playing on a piece of scenery relative to a parent object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenery_animation_start_at_frame,
	0,
	0x00744B10, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a piece of scenery at a specific frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	scenery_animation_start_relative_at_frame,
	0,
	0x007450B0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a piece of scenery relative to a specific cutscene flag at a specific frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	scenery_animation_idle,
	0,
	0x007454F0, // $TODO write the function chuckle nuts
	"starts the idle animation (if any) for a piece of scenery\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_scenery
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	scenery_get_animation_time,
	0,
	0x00745A30, // $TODO write the function chuckle nuts
	"returns the number of ticks remaining in a custom animation (or zero, if the animation is over).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_scenery
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_can_blink,
	0,
	0x00745D40, // $TODO write the function chuckle nuts
	"allows a unit to blink or not (units never blink when they are dead)\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_set_active_camo,
	0,
	0x007461B0, // $TODO write the function chuckle nuts
	"enable or disable active camo for the given unit over the specified number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_open,
	0,
	0x00746560, // $TODO write the function chuckle nuts
	"opens the hatches on the given unit\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_close,
	0,
	0x00746850, // $TODO write the function chuckle nuts
	"closes the hatches on a given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_kill,
	0,
	0x00746C80, // $TODO write the function chuckle nuts
	"kills a given unit, no saving throw\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_kill_silent,
	0,
	0x00746F90, // $TODO write the function chuckle nuts
	"kills a given unit silently (doesn't make them play their normal death animation or sound)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_is_emitting,
	0,
	0x007472F0, // $TODO write the function chuckle nuts
	"returns whether or not the given unit is current emitting an ai\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	unit_get_custom_animation_time,
	0,
	0x007477A0, // $TODO write the function chuckle nuts
	"returns the number of ticks remaining in a unit's custom animation (or zero, if the animation is over).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_stop_custom_animation,
	0,
	0x00747BF0, // $TODO write the function chuckle nuts
	"stops the custom animation running on the given unit.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	custom_animation,
	0,
	0x00747F70, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	custom_animation_loop,
	0,
	0x007484E0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_boolean,
	custom_animation_relative,
	0,
	0x0072F6B0, // $TODO write the function chuckle nuts
	"starts a custom animation relative to some other object (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_boolean,
	custom_animation_relative_loop,
	0,
	0x0072FD60, // $TODO write the function chuckle nuts
	"starts a custom animation relative to some other object (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	custom_animation_list,
	0,
	0x00730410, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit list (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_object_list, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_boolean,
	unit_custom_animation_at_frame,
	0,
	0x00730980, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit at a specific frame index(interpolates into animation if next to last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_boolean,
	unit_custom_animation_relative_at_frame,
	0,
	0x00730F50, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit relative to a specific cutscene flag at a specific frame index(interpolates into animation if next to last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_string_id, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_is_playing_custom_animation,
	0,
	0x00731620, // $TODO write the function chuckle nuts
	"returns TRUE if the given unit is still playing a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_set_custom_animations_hold_on_last_frame,
	0,
	0x007319A0, // $TODO write the function chuckle nuts
	"changes the default behavior for custom animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	object_set_custom_animations_prevent_lipsync_head_movement,
	0,
	0x00731CF0, // $TODO write the function chuckle nuts
	"when true, prevents lipsync from bobbing the head during custom animations. default is true.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	preferred_animation_list_add,
	0,
	0x007320A0, // $TODO write the function chuckle nuts
	"adds an animation into the 'preferred list' of animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	preferred_animation_list_clear,
	0,
	0x007325C0, // $TODO write the function chuckle nuts
	"clears the 'preferred list' of animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_actively_controlled,
	0,
	0x007327E0, // $TODO write the function chuckle nuts
	"sets unit's actively controlled flag\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	unit_get_team_index,
	0,
	0x00732AD0, // $TODO write the function chuckle nuts
	"returns the team index of the unit, returns -1 if the unit does not have a team\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_aim_without_turning,
	0,
	0x00732FA0, // $TODO write the function chuckle nuts
	"allows a unit to aim in place without turning\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_enterable_by_player,
	0,
	0x00733330, // $TODO write the function chuckle nuts
	"can be used to prevent the player from entering a vehicle\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_set_seat_enterable_by_player,
	0,
	0x007337B0, // $TODO write the function chuckle nuts
	"",
	NULL,
	3, _hs_type_unit, _hs_type_unit_seat_mapping, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_get_enterable_by_player,
	0,
	0x00733AE0, // $TODO write the function chuckle nuts
	"returns true if a player may enter the vehicle\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_only_takes_damage_from_players_team,
	0,
	0x00734060, // $TODO write the function chuckle nuts
	"used for the tartarus boss fight\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_enter_vehicle,
	0,
	0x00734400, // $TODO write the function chuckle nuts
	"puts the specified unit in the specified vehicle (in the named seat)\r\nNETWORK SAFE: Yes (w/o animation on clients)",
	NULL,
	3, _hs_type_unit, _hs_type_vehicle, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_falling_damage_disable,
	0,
	0x00734880, // $TODO write the function chuckle nuts
	"disables falling damage on unit",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	unit_in_vehicle_type,
	0,
	0x00734C90, // $TODO write the function chuckle nuts
	"returns true if the unit is in a vehicle that matches the specified vehicle type",
	NULL,
	2, _hs_type_unit, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	object_get_turret_count,
	0,
	0x00735140, // $TODO write the function chuckle nuts
	"returns the number of turrets attached to the given object\r\nNETWORK SAFE: Unknown, assumed UNSAFE",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_vehicle,
	object_get_turret,
	0,
	0x007355E0, // $TODO write the function chuckle nuts
	"returns the nth turret attached to the given object\r\nNETWORK SAFE: Unknown, assumed UNSAFE",
	NULL,
	2, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_board_vehicle,
	0,
	0x00735AE0, // $TODO write the function chuckle nuts
	"Causes the given unit to attempt to board the named seat\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_unit, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	unit_set_emotion,
	0,
	0x00735F10, // $TODO write the function chuckle nuts
	"sets a unit's facial expression (-1 is none, other values depend on unit)\r\nNETWORK SAFE: No",
	NULL,
	4, _hs_type_unit, _hs_type_long_integer, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	unit_set_emotion_by_name,
	0,
	0x00736390, // $TODO write the function chuckle nuts
	"sets a unit's facial expression by name with weight and transition time\r\nNETWORK SAFE: No",
	NULL,
	4, _hs_type_unit, _hs_type_string_id, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_enable_eye_tracking,
	0,
	0x00736810, // $TODO write the function chuckle nuts
	"enable/disable eye aiming on a unit\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_integrated_flashlight,
	0,
	0x00736B60, // $TODO write the function chuckle nuts
	"sets a unit's flashlight on or off\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_voice,
	0,
	0x00737040, // $TODO write the function chuckle nuts
	"set the dialogue for the given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_enable_vision_mode,
	0,
	0x007373A0, // $TODO write the function chuckle nuts
	"enable/disable vision mode for a given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_in_vehicle,
	0,
	0x00737780, // $TODO write the function chuckle nuts
	"returns true if the given unit is seated on a parent unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	vehicle_test_seat_list,
	0,
	0x00737B90, // $TODO write the function chuckle nuts
	"tests whether the named seat has an object in the object list (use \"\" to test all seats for any unit in the list)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_vehicle, _hs_type_unit_seat_mapping, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	vehicle_test_seat_unit,
	0,
	0x00738130, // $TODO write the function chuckle nuts
	"tests whether the named seat has a specified unit in it (use  to test all seats for this unit)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_vehicle, _hs_type_unit_seat_mapping, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	vehicle_test_seat,
	0,
	0x007386A0, // $TODO write the function chuckle nuts
	"tests whether the named seat has any unit in it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_vehicle, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_prefer_tight_camera_track,
	0,
	0x00738B40, // $TODO write the function chuckle nuts
	"sets the unit to prefer a tight camera track\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_exit_vehicle,
	0,
	0x00738F50, // $TODO write the function chuckle nuts
	"makes a unit exit its vehicle (0 = normal exit to airborne, 1 = ejection, 2 = ejection + death, 3 = exit to ground)\r\nNETWORK SAFE: Yes (w/o client animation)",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_exit_vehicle,
	0,
	0x007392B0, // $TODO write the function chuckle nuts
	"makes a unit exit its vehicle (0 = normal exit to airborne, 1 = ejection, 2 = ejection + death, 3 = exit to ground)\r\nNETWORK SAFE: Yes (w/o client animation)",
	NULL,
	2, _hs_type_unit, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_set_maximum_vitality,
	0,
	0x00739690, // $TODO write the function chuckle nuts
	"sets a unit's maximum body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	units_set_maximum_vitality,
	0,
	0x00739AA0, // $TODO write the function chuckle nuts
	"sets a group of units' maximum body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_object_list, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_set_current_vitality,
	0,
	0x00739F80, // $TODO write the function chuckle nuts
	"sets a unit's current body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	units_set_current_vitality,
	0,
	0x0073A490, // $TODO write the function chuckle nuts
	"sets a group of units' current body and shield vitality\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_object_list, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_short_integer,
	vehicle_load_magic,
	0,
	0x0073A7D0, // $TODO write the function chuckle nuts
	"makes a list of units (named or by encounter) magically get into a vehicle, in the substring-specified seats (e.g. CD-passenger... empty string matches all seats)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_object, _hs_type_unit_seat_mapping, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_short_integer,
	vehicle_unload,
	0,
	0x0073AD80, // $TODO write the function chuckle nuts
	"makes units get out of an object from the substring-specified seats (e.g. CD-passenger... empty string matches all seats)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_set_animation_mode,
	0,
	0x0073B1D0, // $TODO write the function chuckle nuts
	"this unit will assume the named animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	magic_melee_attack,
	0,
	0x0073B470, // $TODO write the function chuckle nuts
	"causes player's unit to start a melee attack\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object_list,
	vehicle_riders,
	0,
	0x0073B8A0, // $TODO write the function chuckle nuts
	"returns a list of all riders in a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_unit,
	vehicle_driver,
	0,
	0x0073BC00, // $TODO write the function chuckle nuts
	"returns the driver of a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_unit,
	vehicle_gunner,
	0,
	0x0073C090, // $TODO write the function chuckle nuts
	"returns the gunner of a vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	unit_get_health,
	0,
	0x0073C4B0, // $TODO write the function chuckle nuts
	"returns the health [0,1] of the unit, returns -1 if the unit does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	unit_get_shield,
	0,
	0x0073C9A0, // $TODO write the function chuckle nuts
	"returns the shield [0,1] of the unit, returns -1 if the unit does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	unit_get_total_grenade_count,
	0,
	0x0073CD30, // $TODO write the function chuckle nuts
	"returns the total number of grenades for the given unit, 0 if it does not exist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	unit_has_weapon,
	0,
	0x0073D1B0, // $TODO write the function chuckle nuts
	"returns TRUE if the <unit> has <object> as a weapon, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	unit_has_weapon_readied,
	0,
	0x0073D660, // $TODO write the function chuckle nuts
	"returns TRUE if the <unit> has <object> as the primary weapon, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_has_any_equipment,
	0,
	0x0073DB90, // $TODO write the function chuckle nuts
	"returns TRUE if the <unit> has any equipment, FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	unit_has_equipment,
	0,
	0x0073DF60, // $TODO write the function chuckle nuts
	"returns TRUE if the <unit> has equipment <object> , FALSE otherwise\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_lower_weapon,
	0,
	0x0073E4A0, // $TODO write the function chuckle nuts
	"lower the units weapon over x ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_raise_weapon,
	0,
	0x0073E8A0, // $TODO write the function chuckle nuts
	"raises the units weapon over x ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_drop_support_weapon,
	0,
	0x0073ED00, // $TODO write the function chuckle nuts
	"forces the given unit to drop its support weapon, if it is holding one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_spew_action,
	0,
	0x0073EFD0, // $TODO write the function chuckle nuts
	"runs the spew unit action on the specified unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_force_reload,
	0,
	0x0073F470, // $TODO write the function chuckle nuts
	"Force the give unit to reload his weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	animation_stats_dump,
	0,
	0x0073F700, // $TODO write the function chuckle nuts
	"Dump usage information for the animations in the game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_animation_forced_seat,
	0,
	0x0073F990, // $TODO write the function chuckle nuts
	"all units controlled by the player will assume the given seat name (valid values are 'asleep', 'alert', 'stand', 'crouch' and 'flee')\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_doesnt_drop_items,
	0,
	0x0073FC00, // $TODO write the function chuckle nuts
	"prevents any of the given units from dropping weapons or grenades when they die\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object_list
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_impervious,
	0,
	0x0073FF60, // $TODO write the function chuckle nuts
	"prevents any of the given units from being knocked around or playing ping animations\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object_list, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_suspended,
	0,
	0x007402C0, // $TODO write the function chuckle nuts
	"stops gravity from working on the given unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	unit_add_equipment,
	0,
	0x00740720, // $TODO write the function chuckle nuts
	"adds/resets the unit's health, shield, and inventory (weapons and grenades) to the named profile. resets if third parameter is true, adds if false. weapons will be marked as garbage if fourth parameter is true (for respawning equipment).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_starting_profile, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	weapon_hold_trigger,
	0,
	0x00740B50, // $TODO write the function chuckle nuts
	"turns the trigger for a weapon  on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_weapon, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	weapon_enable_warthog_chaingun_light,
	0,
	0x007410E0, // $TODO write the function chuckle nuts
	"turns the trigger for a weapon  on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_set_never_appears_locked,
	0,
	0x007412F0, // $TODO write the function chuckle nuts
	"changes a machine's never_appears_locked flag, but only if paul is a bastard\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_set_power,
	0,
	0x00741760, // $TODO write the function chuckle nuts
	"immediately sets the power of a named device to the given value\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_device, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	device_get_power,
	0,
	0x00741AF0, // $TODO write the function chuckle nuts
	"gets the current power of a named device\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_device
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	device_set_position,
	0,
	0x00741EB0, // $TODO write the function chuckle nuts
	"set the desired position of the given device (used for devices without explicit device groups)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_device, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	device_get_position,
	0,
	0x00742490, // $TODO write the function chuckle nuts
	"gets the current position of the given device (used for devices without explicit device groups)\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_device
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_set_position_immediate,
	0,
	0x00742840, // $TODO write the function chuckle nuts
	"instantaneously changes the position of the given device (used for devices without explicit device groups\r\nNETWORK SAFE: No (only desired works, need to add immediate support)",
	NULL,
	2, _hs_type_device, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	device_group_get,
	0,
	0x00742D20, // $TODO write the function chuckle nuts
	"returns the desired value of the specified device group.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_device_group
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	device_group_set,
	0,
	0x00743000, // $TODO write the function chuckle nuts
	"changes the desired value of the specified device group.\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_device, _hs_type_device_group, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_group_set_immediate,
	0,
	0x007435D0, // $TODO write the function chuckle nuts
	"instantaneously changes the value of the specified device group.\r\nNETWORK SAFE: No (only desired works, need to add immediate support)",
	NULL,
	2, _hs_type_device_group, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_one_sided_set,
	0,
	0x007439A0, // $TODO write the function chuckle nuts
	"TRUE makes the given device one-sided (only able to be opened from one direction), FALSE makes it two-sided\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_ignore_player_set,
	0,
	0x00743ED0, // $TODO write the function chuckle nuts
	"TRUE makes the given device ignore the player (won't open for players), FALSE makes it work for the player\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_operates_automatically_set,
	0,
	0x00744200, // $TODO write the function chuckle nuts
	"TRUE makes the given device open automatically when any biped is nearby, FALSE makes it not\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_closes_automatically_set,
	0,
	0x00744680, // $TODO write the function chuckle nuts
	"TRUE makes the given device close automatically after it has opened, FALSE makes it not\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	device_group_change_only_once_more_set,
	0,
	0x00744A70, // $TODO write the function chuckle nuts
	"TRUE allows a device to change states only once\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_device_group, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	device_set_position_track,
	0,
	0x00744E40, // $TODO write the function chuckle nuts
	"set the desired position track animation to use (optional interpolation time onto track)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_device, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	device_set_overlay_track,
	0,
	0x00745390, // $TODO write the function chuckle nuts
	"set the desired overlay animation to use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_device, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	device_animate_position,
	0,
	0x00745740, // $TODO write the function chuckle nuts
	"animate the position over time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_device, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	device_animate_overlay,
	0,
	0x00745CE0, // $TODO write the function chuckle nuts
	"animate the overlay over time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_device, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	cheat_all_powerups,
	0,
	cheat_all_powerups, // 0x00746280
	"drops all powerups near player\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	cheat_all_weapons,
	0,
	cheat_all_weapons, // 0x007464B0
	"drops all weapons near player\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	cheat_all_vehicles,
	0,
	cheat_all_vehicles, // 0x00746720
	"drops all vehicles on player\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	cheat_teleport_to_camera,
	0,
	cheat_teleport_to_camera, // 0x00746960
	"teleports player to camera location\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	cheat_active_camouflage,
	0,
	cheat_active_camouflage, // 0x00746C00
	"gives the player active camouflage\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	cheat_active_camouflage_by_player,
	0,
	cheat_active_camouflage_by_user, // 0x00746E00
	"gives a specific player active camouflage\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_short_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	cheats_load,
	0,
	cheats_load, // 0x007470E0
	"reloads the cheats.txt file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	drop_safe,
	0,
	cheat_drop_tag_safe, // 0x00747230
	"drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle\r\nNETWORK SAFE: Yes, for objects",
	NULL,
	1, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	drop,
	0,
	cheat_drop_tag_name, // 0x007474E0
	"drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle\r\nNETWORK SAFE: Yes, for objects",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	drop_variant,
	0,
	cheat_drop_tag_name_with_variant_hs, // 0x00747700
	"drops the named tag e.g. objects\\vehicles\\banshee\\banshee.vehicle using the specified variant name\r\nNETWORK SAFE: Yes, for objects",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	ai_enable,
	0,
	ai_globals_set_ai_active, // 0x00747900
	"turns all AI on or off.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	ai_enabled,
	0,
	0x00747DB0, // $TODO write the function chuckle nuts
	"returns whether AI is turned on or off.\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_grenades,
	0,
	0x00747FC0, // $TODO write the function chuckle nuts
	"turns grenade inventory on or off.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_dialogue_enable,
	0,
	0x00748350, // $TODO write the function chuckle nuts
	"turn combat dialogue on/off\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_player_dialogue_enable,
	0,
	0x00748750, // $TODO write the function chuckle nuts
	"turn player combat dialogue on/off\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_infection_suppress,
	0,
	0x0072F790, // $TODO write the function chuckle nuts
	"disable infection for this number of ticks\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_fast_and_dumb,
	0,
	0x0072FBC0, // $TODO write the function chuckle nuts
	"turn on/off perception performance hacks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_dialogue_log_reset,
	0,
	0x0072FEB0, // $TODO write the function chuckle nuts
	"reset the dialogue log\r\nNETWORK SAFE: Yes (local)",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_dialogue_log_dump,
	0,
	0x007301E0, // $TODO write the function chuckle nuts
	"dump a file of the given name with csv statistics on vocalizations\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object,
	ai_get_object,
	0,
	0x00730490, // $TODO write the function chuckle nuts
	"returns the unit/object corresponding to the given actor\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_unit,
	ai_get_unit,
	0,
	0x007308D0, // $TODO write the function chuckle nuts
	"returns the unit/object corresponding to the given actor\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_ai,
	ai_get_squad,
	0,
	0x00730D00, // $TODO write the function chuckle nuts
	"returns the squad of the given actor. Invalid if the given ai is NOT an actor-type (i.e. is itself a squad or squad group or task\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_ai,
	ai_get_turret_ai,
	0,
	0x00731080, // $TODO write the function chuckle nuts
	"returns an the ai controlling the Nth turret attached to the given ai (or its vehicle)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_point_ref,
	ai_random_smart_point,
	0,
	0x00731680, // $TODO write the function chuckle nuts
	"returns a point in the given point set that a player is probably going to be able to see. Args are [point set], [min distance], [max distance], [angle-from-forward], all relative to the player\r\nNETWORK SAFE: Yes",
	NULL,
	4, _hs_type_point_ref, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_point_ref,
	ai_nearest_point,
	0,
	0x00731B80, // $TODO write the function chuckle nuts
	"returns the nearest point in the given point set to the given object\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	ai_get_point_count,
	0,
	0x00732010, // $TODO write the function chuckle nuts
	"returns the number of points in the given point set\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_point_ref,
	ai_point_set_get_point,
	0,
	0x00732330, // $TODO write the function chuckle nuts
	"returns Nth point in the point set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_place,
	0,
	0x007328A0, // $TODO write the function chuckle nuts
	"places the given number of members of the specified squad.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_place,
	0,
	0x00732C50, // $TODO write the function chuckle nuts
	"places the given number of members of the specified squad.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_place_in_vehicle,
	0,
	0x00733020, // $TODO write the function chuckle nuts
	"places the specified squad (1st arg) on the map in the vehicles belonging to the specified vehicle squad (2nd arg).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_cannot_die,
	0,
	0x007333A0, // $TODO write the function chuckle nuts
	"AI cannot die from damage (as opposed to by scripting)\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_vitality_pinned,
	0,
	0x007337F0, // $TODO write the function chuckle nuts
	"Returns true if the ai's units are ALL vitality pinned (see object_vitality_pinned)\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_ai,
	ai_index_from_spawn_formation,
	0,
	0x00733B60, // $TODO write the function chuckle nuts
	"Create an AI index from the given squad and spawn formation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_resurrect,
	0,
	0x007341A0, // $TODO write the function chuckle nuts
	"Resurrect the specified mission critical AI character.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_kill,
	0,
	0x00734440, // $TODO write the function chuckle nuts
	"instantly kills the specified encounter and/or squad.\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_kill_silent,
	0,
	0x00734850, // $TODO write the function chuckle nuts
	"instantly and silently (no animation or sound played) kills the specified encounter and/or squad.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_erase,
	0,
	0x00734C20, // $TODO write the function chuckle nuts
	"erases the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_erase_all,
	0,
	0x00734FD0, // $TODO write the function chuckle nuts
	"erases all AI.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_disposable,
	0,
	0x007351C0, // $TODO write the function chuckle nuts
	"enables or disables automatic garbage collection for actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_select,
	0,
	0x007355B0, // $TODO write the function chuckle nuts
	"selects the specified squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_deselect,
	0,
	0x00735870, // $TODO write the function chuckle nuts
	"clears the selected encounter.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_deaf,
	0,
	0x00735C70, // $TODO write the function chuckle nuts
	"enables or disables hearing for actors in the specified encounter.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_blind,
	0,
	0x00735FD0, // $TODO write the function chuckle nuts
	"enables or disables sight for actors in the specified encounter.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_weapon_up,
	0,
	0x00736420, // $TODO write the function chuckle nuts
	"force actor to hold their weapon up, instead of lowering it when idle. this does NOT cause them to draw their weapon if stowed\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_flood_disperse,
	0,
	0x00736750, // $TODO write the function chuckle nuts
	"Disperese a flood encounter, stimulating the various forms to break down into infections and transition to the new objective\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_magically_see,
	0,
	0x00736B20, // $TODO write the function chuckle nuts
	"Make one squad magically aware of another.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_magically_see_object,
	0,
	0x00736F50, // $TODO write the function chuckle nuts
	"Make a squad magically aware of a particular object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_active_camo,
	0,
	0x00737340, // $TODO write the function chuckle nuts
	"Turn on active camoflage on actor/squad/squad-group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_suppress_combat,
	0,
	0x007376A0, // $TODO write the function chuckle nuts
	"Turn on/off combat suppression on actor/squad/squad-group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_engineer_explode,
	0,
	0x00737A90, // $TODO write the function chuckle nuts
	"Register an explosion stimulus with all specified actors, causing engineers to explode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_grunt_kamikaze,
	0,
	0x00737FC0, // $TODO write the function chuckle nuts
	"Register a kamikaze stimulus with all specified actors, causing grunts to GO WILD\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_migrate,
	0,
	0x00738280, // $TODO write the function chuckle nuts
	"makes all or part of an encounter move to another encounter.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	ai_allegiance,
	0,
	ai_scripting_allegiance, // 0x00738660
	"creates an allegiance between two teams.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_enum_team, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_allegiance_remove,
	0,
	0x00738A80, // $TODO write the function chuckle nuts
	"destroys an allegiance between two teams.\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_enum_team, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_allegiance_break,
	0,
	0x00738E20, // $TODO write the function chuckle nuts
	"fake a betrayal incident between two teams\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_team, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_braindead,
	0,
	0x00739210, // $TODO write the function chuckle nuts
	"makes a group of actors braindead, or restores them to life (in their initial state)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_braindead_by_unit,
	0,
	0x00739650, // $TODO write the function chuckle nuts
	"makes a list of objects braindead, or restores them to life. if you pass in a vehicle index, it makes all actors in that vehicle braindead (including any built-in guns)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object_list, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_disregard,
	0,
	0x00739A00, // $TODO write the function chuckle nuts
	"if TRUE, forces all actors to completely disregard the specified units, otherwise lets them acknowledge the units again\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object_list, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_prefer_target,
	0,
	0x00739E70, // $TODO write the function chuckle nuts
	"if TRUE, *ALL* enemies will prefer to attack the specified units. if FALSE, removes the preference.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object_list, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_prefer_target_team,
	0,
	0x0073A1E0, // $TODO write the function chuckle nuts
	"the given ai will prefer the indicated team.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	ai_prefer_target_ai,
	0,
	0x0073A6C0, // $TODO write the function chuckle nuts
	"the given ai (actor, squad, task, etc.) will prefer the given other ai (actor, squad, task, etc.)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_targeting_group,
	0,
	0x0073AA00, // $TODO write the function chuckle nuts
	"Set the AI to only target other guys of targetin group X (true/false whether the player should also be targeted)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	ai_set_targeting_group,
	0,
	0x0073AED0, // $TODO write the function chuckle nuts
	"Set the AI to only target other guys of targetin group X (true/false whether the player should also be targeted)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_short_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_teleport_to_starting_location_if_outside_bsp,
	0,
	0x0073B230, // $TODO write the function chuckle nuts
	"PLEASE USE ai_teleport_to_spawn_point_if_outside_bsp INSTEAD\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_teleport_to_spawn_point_if_outside_bsp,
	0,
	0x0073B670, // $TODO write the function chuckle nuts
	"teleports a group of actors to the spawn points of their current squad(s) if they are currently outside the world.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_teleport,
	0,
	0x0073B970, // $TODO write the function chuckle nuts
	"teleports the given squad to the given point.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_bring_forward,
	0,
	0x0073BDA0, // $TODO write the function chuckle nuts
	"teleports the given mission critical AI character to the current encounter, if beyond the given range from a player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_short_integer,
	ai_migrate_form,
	0,
	0x0073C140, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	ai_morph,
	0,
	0x0073C5C0, // $TODO write the function chuckle nuts
	"Cause an ai (or a group of ai) to morph to the given form\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	biped_morph,
	0,
	0x0073CB70, // $TODO write the function chuckle nuts
	"Cause a biped to morph to its floodified form\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_renew,
	0,
	0x0073CFD0, // $TODO write the function chuckle nuts
	"refreshes the health and grenade count of a group of actors, so they are as good as new\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_force_active,
	0,
	0x0073D320, // $TODO write the function chuckle nuts
	"forces an encounter to remain active (i.e. not freeze in place) even if there are no players nearby\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_force_active_by_unit,
	0,
	0x0073D700, // $TODO write the function chuckle nuts
	"forces a named actor that is NOT in an encounter to remain active (i.e. not freeze in place) even if there are no players nearby\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_playfight,
	0,
	0x0073DB50, // $TODO write the function chuckle nuts
	"sets an encounter to be playfighting or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_reconnect,
	0,
	0x0073DED0, // $TODO write the function chuckle nuts
	"reconnects all AI information to the current structure bsp (use this after you create encounters or command lists in sapien, or place new firing points or command list points)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_berserk,
	0,
	0x0073E2F0, // $TODO write the function chuckle nuts
	"forces a group of actors to start or stop berserking\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_set_team,
	0,
	0x0073E520, // $TODO write the function chuckle nuts
	"makes an encounter change to a new team\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_allow_dormant,
	0,
	0x0073E990, // $TODO write the function chuckle nuts
	"either enables or disables automatic dormancy for a group of actors\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_is_attacking,
	0,
	0x0073ED60, // $TODO write the function chuckle nuts
	"returns whether a platoon is in the attacking mode (or if an encounter is specified, returns whether any platoon in that encounter is attacking)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_fighting_count,
	0,
	0x0073F180, // $TODO write the function chuckle nuts
	"return the number of actors that are fighting in a squad or squad_group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_living_count,
	0,
	0x0073F5D0, // $TODO write the function chuckle nuts
	"return the number of living actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	ai_living_fraction,
	0,
	0x0073FA00, // $TODO write the function chuckle nuts
	"return the fraction [0-1] of living actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_in_vehicle_count,
	0,
	0x0073FE50, // $TODO write the function chuckle nuts
	"return the number of living actors in the specified encounter and/or squad who are currently in a vehicle.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_body_count,
	0,
	0x00740240, // $TODO write the function chuckle nuts
	"return the number of actors who have died in the completiong of the given objective/task.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	ai_strength,
	0,
	0x007406E0, // $TODO write the function chuckle nuts
	"return the current strength (average body vitality from 0-1) of the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_swarm_count,
	0,
	0x00740AE0, // $TODO write the function chuckle nuts
	"return the number of swarm actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_nonswarm_count,
	0,
	0x00740F40, // $TODO write the function chuckle nuts
	"return the number of non-swarm actors in the specified encounter and/or squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object_list,
	ai_actors,
	0,
	0x00741370, // $TODO write the function chuckle nuts
	"converts an ai reference to an object list.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	ai_allegiance_broken,
	0,
	0x007417C0, // $TODO write the function chuckle nuts
	"returns whether two teams have an allegiance that is currently broken by traitorous behavior\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_team, _hs_type_enum_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_spawn_count,
	0,
	0x00741BB0, // $TODO write the function chuckle nuts
	"returns the number of actors spawned in the given squad or squad group\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_ai,
	object_get_ai,
	0,
	0x00742090, // $TODO write the function chuckle nuts
	"returns the ai attached to this object, if any\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	ai_rotate_scenario,
	0,
	0x00742560, // $TODO write the function chuckle nuts
	"rotates ai points and normals (e.g. firing points, spawn points) around the given axis (x,y,z) by the given rotation angle (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	ai_translate_scenario,
	0,
	0x00742A90, // $TODO write the function chuckle nuts
	"translates ai points and objects by the given x y z.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	scenario_duplicate_bsp_objects,
	0,
	0x00742F30, // $TODO write the function chuckle nuts
	"<input bsp> <output bsp> <filter>: duplicate filtered objects in the first BSP into the second BSP; available filters: all, crates, scenery, bipeds, machines, terminals, controls, arg_devices\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	scenario_duplicate_bsp_objects,
	0,
	0x00743490, // $TODO write the function chuckle nuts
	"<input bsp> <output bsp> <filter>: duplicate filtered objects in the first BSP into the second BSP; available filters: all, crates, scenery, bipeds, machines, terminals, controls, arg_devices\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenario_rotate,
	0,
	0x00743810, // $TODO write the function chuckle nuts
	"rotates scenario objects (specified by the filter) in the specified BSP around the given axis (x,y,z) by the given amount (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenario_reflect,
	0,
	0x00743DB0, // $TODO write the function chuckle nuts
	"reflects scenario objects (specified by the filter) in the specified BSP across the given plane (specified by normal x,y,z and offset k)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	scenario_translate,
	0,
	0x007441B0, // $TODO write the function chuckle nuts
	"translates scenario objects (specified by the filter) in the specified BSP the given amount (x,y,z)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	scenario_rotate,
	0,
	0x007446E0, // $TODO write the function chuckle nuts
	"rotates scenario objects (specified by the filter) in the specified BSP around the given axis (x,y,z) by the given amount (degrees)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	scenario_reflect,
	0,
	0x00744B70, // $TODO write the function chuckle nuts
	"reflects scenario objects (specified by the filter) in the specified BSP across the given plane (specified by normal x,y,z and offset k)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	scenario_translate,
	0,
	0x00745270, // $TODO write the function chuckle nuts
	"translates scenario objects (specified by the filter) in the specified BSP the given amount (x,y,z)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	ai_set_task,
	0,
	0x00745680, // $TODO write the function chuckle nuts
	"Sets the current task of the current task\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	ai_set_objective,
	0,
	0x00745BE0, // $TODO write the function chuckle nuts
	"Sets the squad's current objective\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_task_status,
	0,
	0x007460D0, // $TODO write the function chuckle nuts
	"Returns the status of the given objective/task. See ai_task_status_<tab> for return codes.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	ai_set_task_condition,
	0,
	0x00746460, // $TODO write the function chuckle nuts
	"Latches the task condition true or false\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_leadership,
	0,
	0x00746920, // $TODO write the function chuckle nuts
	"Returns whether a squad or task has a leader\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_leadership_all,
	0,
	0x00746DC0, // $TODO write the function chuckle nuts
	"Returns whether a task or any of its children has a leader\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_task_count,
	0,
	0x007471B0, // $TODO write the function chuckle nuts
	"Returns the living count of the AI in the given task OR any of its children\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_reset_objective,
	0,
	0x00747620, // $TODO write the function chuckle nuts
	"Completely reset the given objective, task or task hierarchy (if passed a gate task).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_squad_patrol_objective_disallow,
	0,
	0x007478C0, // $TODO write the function chuckle nuts
	"Disallow the selection of an objective by any squad patrol.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	generate_pathfinding,
	0,
	0x00747DE0, // $TODO write the function chuckle nuts
	"Generate pathfinding info for all structure bsps in the current scenario\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_render_paths_all,
	0,
	0x00747FF0, // $TODO write the function chuckle nuts
	"Turns on raw, smoothed, avoided paths and avoidance obstacles\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_activity_set,
	0,
	0x00748280, // $TODO write the function chuckle nuts
	"Set the given activity running on the given AI(s)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_activity_abort,
	0,
	0x00748680, // $TODO write the function chuckle nuts
	"Aborts the activities running on the given AI(s)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_vehicle,
	ai_vehicle_get,
	0,
	0x0072F680, // $TODO write the function chuckle nuts
	"Returns the vehicle that the given actor is in.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_vehicle,
	ai_vehicle_get_from_starting_location,
	0,
	0x0072FBF0, // $TODO write the function chuckle nuts
	"PLEASE START USING ai_vehicle_get_from_spawn_point INSTEAD\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_vehicle,
	ai_vehicle_get_from_spawn_point,
	0,
	0x0072FFB0, // $TODO write the function chuckle nuts
	"Returns the vehicle that was spawned at the given spawn point\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_vehicle_get_squad_count,
	0,
	0x007304E0, // $TODO write the function chuckle nuts
	"Returns the number of vehicles in the given squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_vehicle,
	ai_vehicle_get_from_squad,
	0,
	0x00730940, // $TODO write the function chuckle nuts
	"Returns the nth vehicle from the given squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	ai_vehicle_reserve_seat,
	0,
	0x00730E30, // $TODO write the function chuckle nuts
	"Reserves the given seat on the given vehicle (so that AI may not enter it\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_vehicle, _hs_type_unit_seat_mapping, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	ai_vehicle_reserve,
	0,
	0x00731410, // $TODO write the function chuckle nuts
	"Reserves the given vehicle (so that AI may not enter it\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_vehicle, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_ai,
	ai_player_get_vehicle_squad,
	0,
	0x00731790, // $TODO write the function chuckle nuts
	"Returns the squad (if any) of the vehicle the player is riding in (note, the argument is a UNIT)\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_vehicle_count,
	0,
	0x00731C70, // $TODO write the function chuckle nuts
	"Returns the number of vehicles being driven / ridden by the actors in the given squad/squad-group/task/etc.\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_carrying_player,
	0,
	0x00732040, // $TODO write the function chuckle nuts
	"Returns whether any of the given AI are in a vehicle with the player\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	player_in_vehicle,
	0,
	0x00732480, // $TODO write the function chuckle nuts
	"Returns whether any players are in the given vehicle\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_player_needs_vehicle,
	0,
	0x00732860, // $TODO write the function chuckle nuts
	"Returns if the given player needs a vehicle at the moment\r\nNETWORK SAFE: UNKNOWN, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	ai_player_any_needs_vehicle,
	0,
	0x00732CD0, // $TODO write the function chuckle nuts
	"Returns if any player needs a vehicle at the moment\r\nNETWORK SAFE: UNKNOWN, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	ai_vehicle_enter,
	0,
	0x00732FE0, // $TODO write the function chuckle nuts
	"tells a group of actors to get into a vehicle... does not interrupt any actors who are already going to vehicles\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_ai, _hs_type_unit, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_vehicle_enter,
	0,
	0x00733400, // $TODO write the function chuckle nuts
	"tells a group of actors to get into a vehicle... does not interrupt any actors who are already going to vehicles\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	ai_vehicle_enter_immediate,
	0,
	0x00733970, // $TODO write the function chuckle nuts
	"the given group of actors is snapped into a vehicle\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_ai, _hs_type_unit, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_vehicle_enter_immediate,
	0,
	0x00733C20, // $TODO write the function chuckle nuts
	"the given group of actors is snapped into a vehicle\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_enter_squad_vehicles,
	0,
	0x007340D0, // $TODO write the function chuckle nuts
	"Instructs the ai in the given squad to get in all their vehicles\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_vehicle_exit,
	0,
	0x00734470, // $TODO write the function chuckle nuts
	"tells a group of actors to get out of any vehicles that they are in\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_vehicle_exit,
	0,
	0x00734970, // $TODO write the function chuckle nuts
	"tells a group of actors to get out of any vehicles that they are in\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	vehicle_overturned,
	0,
	0x00734C50, // $TODO write the function chuckle nuts
	"Returns true if the vehicle is overturned\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vehicle_flip,
	0,
	0x00735110, // $TODO write the function chuckle nuts
	"Flips an overturned vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	ai_combat_status,
	0,
	0x007354B0, // $TODO write the function chuckle nuts
	"Returns the highest integer combat status in the given squad-group/squad/actor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	flock_start,
	0,
	0x00735830, // $TODO write the function chuckle nuts
	"The flock starts producing boids\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	flock_stop,
	0,
	0x00735D70, // $TODO write the function chuckle nuts
	"The flock stops producing boids\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	flock_create,
	0,
	0x00736160, // $TODO write the function chuckle nuts
	"Create the given flock\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	flock_delete,
	0,
	0x00736640, // $TODO write the function chuckle nuts
	"Delete the given flock\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	flock_definition_set,
	0,
	0x00736A00, // $TODO write the function chuckle nuts
	"Set the definition of the given flock to the given tag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	flock_unperch,
	0,
	0x00736F20, // $TODO write the function chuckle nuts
	"Cause the given flock to unperch (if it is perched)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	drop_ai,
	0,
	0x007371E0, // $TODO write the function chuckle nuts
	"Drop an ai with the given weapon at the current camera position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_verify_tags,
	0,
	0x00737520, // $TODO write the function chuckle nuts
	"Verifies state of ai-related tags (e.g. orders, squads, zones, etc.)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	ai_wall_lean,
	0,
	0x00737660, // $TODO write the function chuckle nuts
	"Makes the actor lean against a wall RIGHT NOW\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	ai_play_line,
	0,
	0x00737AE0, // $TODO write the function chuckle nuts
	"Play the given mission dialogue line on the given ai\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_ai, _hs_type_ai_line
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	ai_play_line_at_player,
	0,
	0x007380A0, // $TODO write the function chuckle nuts
	"Play the given mission dialogue line on the given ai, directing the ai's gaze at the nearest visible player\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_ai, _hs_type_ai_line
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	ai_play_line_on_object,
	0,
	0x00738400, // $TODO write the function chuckle nuts
	"Play the given mission dialogue line on the given object (uses first available variant)\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_ai_line
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	ai_play_line_on_object_for_team,
	0,
	0x007389E0, // $TODO write the function chuckle nuts
	"Play the given mission dialogue line on the given object (uses first available variant) if anyone on the local box is on the specified team\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_object, _hs_type_ai_line, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_short_integer,
	ai_play_line_on_point_set,
	0,
	0x00738F00, // $TODO write the function chuckle nuts
	"Play the given line (with the given 3-letter variant) on the N closest point to players in the given point set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_short_integer,
	ai_play_line_on_point_set,
	0,
	0x00739490, // $TODO write the function chuckle nuts
	"Play the given line (with the given 3-letter variant) on the N closest point to players in the given point set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_string_id, _hs_type_point_ref, _hs_type_short_integer, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	campaign_metagame_time_pause,
	0,
	0x007399B0, // $TODO write the function chuckle nuts
	"pause meta game time, while paused your completion multiplier stops going down\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	campaign_metagame_award_points,
	0,
	0x00739D70, // $TODO write the function chuckle nuts
	"award bonus metagame points to a player\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	campaign_metagame_award_primary_skull,
	0,
	0x0073A0A0, // $TODO write the function chuckle nuts
	"award a primary skull to a player\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_enum_primary_skull
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	campaign_metagame_award_secondary_skull,
	0,
	0x0073A590, // $TODO write the function chuckle nuts
	"award a secondary skull to a player\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_enum_secondary_skull
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	campaign_metagame_award_event,
	0,
	0x0073A8F0, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_metagame_enabled,
	0,
	0x0073AD50, // $TODO write the function chuckle nuts
	"TRUE if the metagame is enabled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_survival_enabled,
	0,
	0x0073B090, // $TODO write the function chuckle nuts
	"TRUE if survival mode is enabled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_is_finished_easy,
	0,
	0x0073B2D0, // $TODO write the function chuckle nuts
	"TRUE if any of the players have finished the campaign on easy\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_is_finished_normal,
	0,
	0x0073B700, // $TODO write the function chuckle nuts
	"TRUE if any of the players have finished the campaign on normal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_is_finished_heroic,
	0,
	0x0073B940, // $TODO write the function chuckle nuts
	"TRUE if any of the players have finished the campaign on heroic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	campaign_is_finished_legendary,
	0,
	0x0073BC50, // $TODO write the function chuckle nuts
	"TRUE if any of the players have finished the campaign on legendary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_run_command_script,
	0,
	0x0073BFD0, // $TODO write the function chuckle nuts
	"Causes the specified actor(s) to start executing a command script immediately (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai_command_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_queue_command_script,
	0,
	0x0073C380, // $TODO write the function chuckle nuts
	"Add a command script onto the end of an actor's command script queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai_command_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_stack_command_script,
	0,
	0x0073C650, // $TODO write the function chuckle nuts
	"Push a command script to the top of the actor's command script queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai_command_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_reserve,
	0,
	0x0073CBD0, // $TODO write the function chuckle nuts
	"Reserves the specified actor(s) for the use of the current script (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_reserve,
	0,
	0x0073CF90, // $TODO write the function chuckle nuts
	"Reserves the specified actor(s) for the use of the current script (discarding any other command scripts in the queue)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073D3C0, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073D8E0, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073DF10, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	6, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 14,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073E610, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	7, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 16,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073ECA0, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	8, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 18,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073F410, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	9, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 20,
	_hs_type_boolean,
	vs_cast,
	0,
	0x0073FAC0, // $TODO write the function chuckle nuts
	"Cast 7 actors for a vignette (at the given priority, higher = more important)",
	NULL,
	10, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_ai,
	vs_role,
	0,
	0x00740200, // $TODO write the function chuckle nuts
	"Returns the given member of the cast (as per more recent vs_cast call)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_abort_on_alert,
	0,
	0x007406A0, // $TODO write the function chuckle nuts
	"Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_abort_on_damage,
	0,
	0x00740A50, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_abort_on_combat_status,
	0,
	0x00740CD0, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_abort_on_vehicle_exit,
	0,
	0x00741200, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_alert,
	0,
	0x00741420, // $TODO write the function chuckle nuts
	"Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE3(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_damage,
	0,
	cs_abort_on_damage_thread, 0x00741850, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE3(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_combat_status,
	0,
	cs_abort_on_combat_status_thread, 0x00741B50, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE3(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_vehicle_exit,
	0,
	cs_abort_on_vehicle_exit_thread, 0x00741E70, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE3(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_alert,
	0,
	cs_abort_on_alert_thread, 0x007422A0, // $TODO write the function chuckle nuts
	"Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_abort_on_alert,
	0,
	0x00742910, // $TODO write the function chuckle nuts
	"Command script ends prematurely when given actor's combat status raises to 'alert' or higher\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_damage,
	0,
	0x007430C0, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_abort_on_damage,
	0,
	0x00743780, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor is damaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_combat_status,
	0,
	0x00744070, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_abort_on_combat_status,
	0,
	0x007447C0, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor's combat status rises to given level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_abort_on_vehicle_exit,
	0,
	0x00744F10, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_abort_on_vehicle_exit,
	0,
	0x00745540, // $TODO write the function chuckle nuts
	"Command script ends prematurely when actor gets out of its vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_set_cleanup_script,
	0,
	0x00745DC0, // $TODO write the function chuckle nuts
	"Sets the script that is automatically called when the current thread ends\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_release,
	0,
	0x00746180, // $TODO write the function chuckle nuts
	"Lets the given ai out of the current command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	vs_release_all,
	0,
	0x00746440, // $TODO write the function chuckle nuts
	"Releases all the ai attached to the current thread \r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	cs_command_script_running,
	0,
	0x007466D0, // $TODO write the function chuckle nuts
	"Returns true if the ai is running the command script in question\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai_command_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	cs_command_script_queued,
	0,
	0x00746BB0, // $TODO write the function chuckle nuts
	"Returns true if the command script is in the ai's cs queue\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai_command_script
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_short_integer,
	cs_number_queued,
	0,
	0x007470A0, // $TODO write the function chuckle nuts
	"Returns the number of queued command scripts\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	cs_moving,
	0,
	0x00747390, // $TODO write the function chuckle nuts
	"Returns TRUE if the actor is currently following a path\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	cs_moving,
	0,
	0x007479D0, // $TODO write the function chuckle nuts
	"Returns TRUE if the actor is currently following a path\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	vs_running_atom,
	0,
	0x00748240, // $TODO write the function chuckle nuts
	"Returns true if the ai is running any atom\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	vs_running_atom_movement,
	0,
	0x00748710, // $TODO write the function chuckle nuts
	"Returns true if the ai is running an atom on the movement channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	vs_running_atom_action,
	0,
	0x0072F800, // $TODO write the function chuckle nuts
	"Returns true if the ai is running an atom on the action channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	vs_running_atom_dialogue,
	0,
	0x0072FC60, // $TODO write the function chuckle nuts
	"Returns true if the ai is running an atom on the dialogue channel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_fly_to,
	FLAG(_hs_function_flag_command_script_atom),
	0x00730150, // $TODO write the function chuckle nuts
	"Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_fly_to,
	0,
	0x007307F0, // $TODO write the function chuckle nuts
	"Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_fly_to,
	FLAG(_hs_function_flag_command_script_atom),
	0x00731370, // $TODO write the function chuckle nuts
	"Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_fly_to,
	0,
	0x007319D0, // $TODO write the function chuckle nuts
	"Flies the actor to the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_fly_to_and_face,
	FLAG(_hs_function_flag_command_script_atom),
	0x007324C0, // $TODO write the function chuckle nuts
	"Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_fly_to_and_face,
	0,
	0x00732B10, // $TODO write the function chuckle nuts
	"Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_fly_to_and_face,
	FLAG(_hs_function_flag_command_script_atom),
	0x00733690, // $TODO write the function chuckle nuts
	"Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_point_ref, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_fly_to_and_face,
	0,
	0x00733D80, // $TODO write the function chuckle nuts
	"Flies the actor to the given point and orients him in the appropriate direction (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_fly_by,
	FLAG(_hs_function_flag_command_script_atom),
	0x007349A0, // $TODO write the function chuckle nuts
	"Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_fly_by,
	0,
	0x00735030, // $TODO write the function chuckle nuts
	"Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_fly_by,
	FLAG(_hs_function_flag_command_script_atom),
	0x00735BD0, // $TODO write the function chuckle nuts
	"Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_fly_by,
	0,
	0x00736210, // $TODO write the function chuckle nuts
	"Flies the actor through the given point (within the given tolerance)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_go_to,
	FLAG(_hs_function_flag_command_script_atom),
	0x00736D80, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_go_to,
	0,
	0x007373E0, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_go_to,
	FLAG(_hs_function_flag_command_script_atom),
	0x00737F20, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_go_to,
	0,
	0x00738500, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_go_by,
	FLAG(_hs_function_flag_command_script_atom),
	0x00739150, // $TODO write the function chuckle nuts
	"Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_go_by,
	0,
	0x00739850, // $TODO write the function chuckle nuts
	"Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_go_by,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073A3D0, // $TODO write the function chuckle nuts
	"Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_point_ref, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_go_by,
	0,
	0x0073AAC0, // $TODO write the function chuckle nuts
	"Actor moves toward the point, and considers it hit when it breaks the indicated plane, attenuating throttle by the given amount when near the goal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_go_to_and_face,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073B5D0, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point and has him face the second point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_go_to_and_face,
	0,
	0x0073BCA0, // $TODO write the function chuckle nuts
	"Moves the actor to a specified point and has him face the second point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_go_to_and_posture,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073C710, // $TODO write the function chuckle nuts
	"Actor moves to the point and performs a posture once there\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_go_to_and_posture,
	0,
	0x0073CE80, // $TODO write the function chuckle nuts
	"Actor moves to the point and performs a posture once there\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_go_to_nearest,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073D9B0, // $TODO write the function chuckle nuts
	"Given a point set, AI goes toward the nearest point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_go_to_nearest,
	0,
	0x0073E060, // $TODO write the function chuckle nuts
	"Given a point set, AI goes toward the nearest point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_move_in_direction,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073EAD0, // $TODO write the function chuckle nuts
	"Actor moves at given angle, for the given distance, optionally with the given facing (angle, distance, facing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_move_in_direction,
	0,
	0x0073F280, // $TODO write the function chuckle nuts
	"Actor moves at given angle, for the given distance, optionally with the given facing (angle, distance, facing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_move_towards,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073FDC0, // $TODO write the function chuckle nuts
	"Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_move_towards,
	0,
	0x00740490, // $TODO write the function chuckle nuts
	"Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_move_towards,
	FLAG(_hs_function_flag_command_script_atom),
	0x00740EA0, // $TODO write the function chuckle nuts
	"Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_move_towards,
	0,
	0x00741660, // $TODO write the function chuckle nuts
	"Move in the direction of the given object (and melee the target or not)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_move_towards_point,
	FLAG(_hs_function_flag_command_script_atom),
	0x007420E0, // $TODO write the function chuckle nuts
	"Move to within the given radius of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_move_towards_point,
	0,
	0x00742750, // $TODO write the function chuckle nuts
	"Move to within the given radius of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_swarm_to,
	FLAG(_hs_function_flag_command_script_atom),
	0x007433D0, // $TODO write the function chuckle nuts
	"A swarm swarms towards the point (or point set) in question, the individual forms stopping when they get within the given radius\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_swarm_to,
	0,
	0x00743AB0, // $TODO write the function chuckle nuts
	"A swarm swarms towards the point (or point set) in question, the individual forms stopping when they get within the given radius\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_swarm_from,
	FLAG(_hs_function_flag_command_script_atom),
	0x007445E0, // $TODO write the function chuckle nuts
	"A swarm swarms away from the point in question, going at least the given distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_swarm_from,
	0,
	0x00744CF0, // $TODO write the function chuckle nuts
	"A swarm swarms away from the point in question, going at least the given distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_pause,
	FLAG(_hs_function_flag_command_script_atom),
	0x00745860, // $TODO write the function chuckle nuts
	"The actor does nothing for the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_pause,
	0,
	0x00745F30, // $TODO write the function chuckle nuts
	"The actor does nothing for the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_grenade,
	FLAG(_hs_function_flag_command_script_atom),
	0x00746880, // $TODO write the function chuckle nuts
	"Actor throws a grenade, either by tossing (arg2=0), lobbing (1) or bouncing (2)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_grenade,
	0,
	0x00746FC0, // $TODO write the function chuckle nuts
	"Actor throws a grenade, either by tossing (arg2=0), lobbing (1) or bouncing (2)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_equipment,
	FLAG(_hs_function_flag_command_script_atom),
	0x00747A60, // $TODO write the function chuckle nuts
	"Actor throws their equipment in the direction of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_equipment,
	0,
	0x00748110, // $TODO write the function chuckle nuts
	"Actor throws their equipment in the direction of the given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_jump,
	FLAG(_hs_function_flag_command_script_atom),
	0x0072F900, // $TODO write the function chuckle nuts
	"Actor jumps in direction of angle at the given velocity (angle, velocity)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_jump,
	0,
	0x00730000, // $TODO write the function chuckle nuts
	"Actor jumps in direction of angle at the given velocity (angle, velocity)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_jump_to_point,
	FLAG(_hs_function_flag_command_script_atom),
	0x00730C00, // $TODO write the function chuckle nuts
	"Actor jumps with given horizontal and vertical velocity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_jump_to_point,
	0,
	0x00731280, // $TODO write the function chuckle nuts
	"Actor jumps with given horizontal and vertical velocity\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_vocalize,
	FLAG(_hs_function_flag_command_script_atom),
	0x00731DB0, // $TODO write the function chuckle nuts
	"Actor emits vocalization of given type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_vocalize,
	0,
	0x007323A0, // $TODO write the function chuckle nuts
	"Actor emits vocalization of given type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_play_sound,
	FLAG(_hs_function_flag_command_script_atom),
	0x00732EF0, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_play_sound,
	0,
	0x00733520, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_play_sound,
	FLAG(_hs_function_flag_command_script_atom),
	0x00733FC0, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_sound, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_play_sound,
	0,
	0x00734670, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_sound, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_play_sound,
	FLAG(_hs_function_flag_command_script_atom),
	0x00735330, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_play_sound,
	0,
	0x007359E0, // $TODO write the function chuckle nuts
	"Actor plays an impulse sound and the atom blocks for the given percentage of the sound's total length, at the given volume (0..1)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_sound, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_action,
	FLAG(_hs_function_flag_command_script_atom),
	0x00736530, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the given point. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_action,
	0,
	0x00736C80, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the given point. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_action_at_object,
	FLAG(_hs_function_flag_command_script_atom),
	0x007376E0, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the given object. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_action_at_object,
	0,
	0x00737E40, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the given object. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_action_at_player,
	FLAG(_hs_function_flag_command_script_atom),
	0x00738950, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the nearest_player. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_action_at_player,
	0,
	0x00739040, // $TODO write the function chuckle nuts
	"Actor plays an AI action towards the nearest_player. ai_action_<tab> to see action options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_custom_animation,
	FLAG(_hs_function_flag_command_script_atom),
	0x00739B40, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_custom_animation,
	0,
	0x0073A2E0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cs_custom_animation,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073AE30, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	vs_custom_animation,
	0,
	0x0073B4E0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_ai, _hs_type_boolean, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_custom_animation_death,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073C1B0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_custom_animation_death,
	0,
	0x0073C8B0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cs_custom_animation_death,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073D4B0, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	vs_custom_animation_death,
	0,
	0x0073DC70, // $TODO write the function chuckle nuts
	"starts a custom animation playing on the actor (interpolates into animation if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_ai, _hs_type_boolean, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_custom_animation_loop,
	0,
	0x0073E810, // $TODO write the function chuckle nuts
	"starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_custom_animation_loop,
	0,
	0x0073EF40, // $TODO write the function chuckle nuts
	"starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cs_custom_animation_loop,
	0,
	0x0073F8E0, // $TODO write the function chuckle nuts
	"starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_custom_animation_loop,
	0,
	0x00740070, // $TODO write the function chuckle nuts
	"starts a looping custom animation playing on the actor at the given point (interpolates into animation if last parameter is TRUE) NON-BLOCKING\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_play_line,
	FLAG(_hs_function_flag_command_script_atom),
	0x00740910, // $TODO write the function chuckle nuts
	"Play the named line in the current scene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai_line
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_play_line,
	0,
	0x00741000, // $TODO write the function chuckle nuts
	"Play the named line in the current scene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_ai_line
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_die,
	FLAG(_hs_function_flag_command_script_atom),
	0x00741A00, // $TODO write the function chuckle nuts
	"Actor dies in specified manner\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_die,
	0,
	0x00741FB0, // $TODO write the function chuckle nuts
	"Actor dies in specified manner\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_deploy_turret,
	FLAG(_hs_function_flag_command_script_atom),
	0x00742B60, // $TODO write the function chuckle nuts
	"Deploy a turret at the given script point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_deploy_turret,
	0,
	0x00743190, // $TODO write the function chuckle nuts
	"Deploy a turret at the given script point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cs_approach,
	FLAG(_hs_function_flag_command_script_atom),
	0x00743CF0, // $TODO write the function chuckle nuts
	"(approach <object> <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	vs_approach,
	0,
	0x007444B0, // $TODO write the function chuckle nuts
	"(approach <object> <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_ai, _hs_type_boolean, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cs_approach_player,
	FLAG(_hs_function_flag_command_script_atom),
	0x00745130, // $TODO write the function chuckle nuts
	"(approach player <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vs_approach_player,
	0,
	0x00745900, // $TODO write the function chuckle nuts
	"(approach player <distance - how close I want to get> <max-distance  - start approaching when target is within this range> <follow-distance - give up when target is outside this range>)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_ai, _hs_type_boolean, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_go_to_vehicle,
	FLAG(_hs_function_flag_command_script_atom),
	0x00746380, // $TODO write the function chuckle nuts
	"Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_go_to_vehicle,
	0,
	0x00746A40, // $TODO write the function chuckle nuts
	"Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_go_to_vehicle,
	FLAG(_hs_function_flag_command_script_atom),
	0x00747440, // $TODO write the function chuckle nuts
	"Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_vehicle, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	vs_go_to_vehicle,
	0,
	0x00747B10, // $TODO write the function chuckle nuts
	"Actor gets in the named seat of the appropriate vehicle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_ai, _hs_type_boolean, _hs_type_vehicle, _hs_type_unit_seat_mapping
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_set_style,
	0,
	0x00748600, // $TODO write the function chuckle nuts
	"Override the actor's style\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_style
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_set_style,
	0,
	0x0072FA90, // $TODO write the function chuckle nuts
	"Override the actor's style\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_style
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_force_combat_status,
	0,
	0x00730340, // $TODO write the function chuckle nuts
	"Force the actor's combat status (0= no override, 1=idle, 2= alert, 3= active)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_force_combat_status,
	0,
	0x00730A10, // $TODO write the function chuckle nuts
	"Force the actor's combat status (0= no override, 1=idle, 2= alert, 3= active)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_enable_targeting,
	0,
	0x00731100, // $TODO write the function chuckle nuts
	"Actor autonomous target selection enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_enable_targeting,
	0,
	0x007317F0, // $TODO write the function chuckle nuts
	"Actor autonomous target selection enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_enable_looking,
	0,
	0x00731F80, // $TODO write the function chuckle nuts
	"Actor autonomous looking enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_enable_looking,
	0,
	0x007326C0, // $TODO write the function chuckle nuts
	"Actor autonomous looking enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_enable_moving,
	0,
	0x00732DD0, // $TODO write the function chuckle nuts
	"Actor autonomous moving enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_enable_moving,
	0,
	0x00733470, // $TODO write the function chuckle nuts
	"Actor autonomous moving enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_enable_dialogue,
	0,
	0x00733B90, // $TODO write the function chuckle nuts
	"Actor combat dialogue enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_enable_dialogue,
	0,
	0x00734320, // $TODO write the function chuckle nuts
	"Actor combat dialogue enabled/disabled.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_suppress_activity_termination,
	0,
	0x00734B90, // $TODO write the function chuckle nuts
	"Autonomous termination of activities is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_suppress_activity_termination,
	0,
	0x00735240, // $TODO write the function chuckle nuts
	"Autonomous termination of activities is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_suppress_dialogue_global,
	0,
	0x00735950, // $TODO write the function chuckle nuts
	"Combat dialogue is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_suppress_dialogue_global,
	0,
	0x00736070, // $TODO write the function chuckle nuts
	"Combat dialogue is suppressed for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_look,
	0,
	0x00736870, // $TODO write the function chuckle nuts
	"Actor looks at the point for the remainder of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_look,
	0,
	0x00736FB0, // $TODO write the function chuckle nuts
	"Actor looks at the point for the remainder of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_look_player,
	0,
	0x00737810, // $TODO write the function chuckle nuts
	"Actor looks at nearest player for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_look_player,
	0,
	0x00737DB0, // $TODO write the function chuckle nuts
	"Actor looks at nearest player for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_look_object,
	0,
	0x00738720, // $TODO write the function chuckle nuts
	"Actor looks at the object for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_look_object,
	0,
	0x00738D30, // $TODO write the function chuckle nuts
	"Actor looks at the object for the duration of the cs, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_aim,
	0,
	0x007395C0, // $TODO write the function chuckle nuts
	"Actor aims at the point for the remainder of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_aim,
	0,
	0x00739CE0, // $TODO write the function chuckle nuts
	"Actor aims at the point for the remainder of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_aim_player,
	0,
	0x0073A5F0, // $TODO write the function chuckle nuts
	"Actor aims at nearest player for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_aim_player,
	0,
	0x0073AC10, // $TODO write the function chuckle nuts
	"Actor aims at nearest player for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_aim_object,
	0,
	0x0073B340, // $TODO write the function chuckle nuts
	"Actor aims at the object for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_aim_object,
	0,
	0x0073BAA0, // $TODO write the function chuckle nuts
	"Actor aims at the object for the duration of the cs, or until overridden (overrides look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_face,
	0,
	0x0073C2F0, // $TODO write the function chuckle nuts
	"Actor faces exactly the point for the remainder of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_face,
	0,
	0x0073CAC0, // $TODO write the function chuckle nuts
	"Actor faces exactly the point for the remainder of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_face_player,
	0,
	0x0073D260, // $TODO write the function chuckle nuts
	"Actor faces exactly the nearest player for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_face_player,
	0,
	0x0073D850, // $TODO write the function chuckle nuts
	"Actor faces exactly the nearest player for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_face_object,
	0,
	0x0073E260, // $TODO write the function chuckle nuts
	"Actor faces exactly the given object for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_face_object,
	0,
	0x0073E780, // $TODO write the function chuckle nuts
	"Actor faces exactly the given object for the duration of the cs, or until overridden (overrides aim, look)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_shoot,
	0,
	0x0073F0D0, // $TODO write the function chuckle nuts
	"Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_shoot,
	0,
	0x0073F760, // $TODO write the function chuckle nuts
	"Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_shoot,
	0,
	0x0073FED0, // $TODO write the function chuckle nuts
	"Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_shoot,
	0,
	0x00740610, // $TODO write the function chuckle nuts
	"Actor shoots at given target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_shoot_point,
	0,
	0x00740D10, // $TODO write the function chuckle nuts
	"Actor shoots at given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_shoot_point,
	0,
	0x007414D0, // $TODO write the function chuckle nuts
	"Actor shoots at given point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_shoot_secondary_trigger,
	0,
	0x00741D60, // $TODO write the function chuckle nuts
	"Actor uses secondary trigger whenever he shoots\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_shoot_secondary_trigger,
	0,
	0x00742400, // $TODO write the function chuckle nuts
	"Actor uses secondary trigger whenever he shoots\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_lower_weapon,
	0,
	0x00742BF0, // $TODO write the function chuckle nuts
	"Actor keeps weapon lowered\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_lower_weapon,
	0,
	0x00743270, // $TODO write the function chuckle nuts
	"Actor keeps weapon lowered\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_vehicle_speed,
	0,
	0x00743A20, // $TODO write the function chuckle nuts
	"Set the speed at which the actor will drive a vehicle, expressed as a multiplier 0-1\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_vehicle_speed,
	0,
	0x00744120, // $TODO write the function chuckle nuts
	"Set the speed at which the actor will drive a vehicle, expressed as a multiplier 0-1\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_vehicle_speed_instantaneous,
	0,
	0x00744990, // $TODO write the function chuckle nuts
	"Set the instantaneous speed of the vehicle we're driving\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_vehicle_speed_instantaneous,
	0,
	0x00745020, // $TODO write the function chuckle nuts
	"Set the instantaneous speed of the vehicle we're driving\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_vehicle_boost,
	0,
	0x007457D0, // $TODO write the function chuckle nuts
	"Enables or disables boost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_vehicle_boost,
	0,
	0x00745EA0, // $TODO write the function chuckle nuts
	"Enables or disables boost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_turn_sharpness,
	0,
	0x00746610, // $TODO write the function chuckle nuts
	"Set the sharpness of a vehicle turn (values 0 -> 1). Only applicable to nondirectional flying vehicles (e.g. dropships)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_turn_sharpness,
	0,
	0x00746D30, // $TODO write the function chuckle nuts
	"Set the sharpness of a vehicle turn (values 0 -> 1). Only applicable to nondirectional flying vehicles (e.g. dropships)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_enable_pathfinding_failsafe,
	0,
	0x00747570, // $TODO write the function chuckle nuts
	"Actor blocks until pathfinding calls succeed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_enable_pathfinding_failsafe,
	0,
	0x00747CC0, // $TODO write the function chuckle nuts
	"Actor blocks until pathfinding calls succeed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_set_pathfinding_radius,
	0,
	0x007483A0, // $TODO write the function chuckle nuts
	"Sets the actor's pathfinding radius (this distance at which a destination is considered to have been reached) for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_set_pathfinding_radius,
	0,
	0x0072F700, // $TODO write the function chuckle nuts
	"Sets the actor's pathfinding radius (this distance at which a destination is considered to have been reached) for the remainder of the command script\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_ignore_obstacles,
	0,
	0x0072FEF0, // $TODO write the function chuckle nuts
	"Actor does not avoid obstacles when true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_ignore_obstacles,
	0,
	0x00730680, // $TODO write the function chuckle nuts
	"Actor does not avoid obstacles when true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cs_approach_stop,
	0,
	0x00730EE0, // $TODO write the function chuckle nuts
	"Actor stops approaching\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_approach_stop,
	0,
	0x007314A0, // $TODO write the function chuckle nuts
	"Actor stops approaching\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_movement_mode,
	0,
	0x00731BB0, // $TODO write the function chuckle nuts
	"Actor switches to given animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_movement_mode,
	0,
	0x007321C0, // $TODO write the function chuckle nuts
	"Actor switches to given animation mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_crouch,
	0,
	0x00732990, // $TODO write the function chuckle nuts
	"Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_crouch,
	0,
	0x00733060, // $TODO write the function chuckle nuts
	"Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_crouch,
	0,
	0x007338E0, // $TODO write the function chuckle nuts
	"Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_crouch,
	0,
	0x00733F30, // $TODO write the function chuckle nuts
	"Actor crouches / uncrouches, transitioning over the given number of seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_walk,
	0,
	0x007347C0, // $TODO write the function chuckle nuts
	"Actor walks for the remainder of the command script, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_walk,
	0,
	0x00734E90, // $TODO write the function chuckle nuts
	"Actor walks for the remainder of the command script, or until overridden\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_posture_set,
	0,
	0x007356A0, // $TODO write the function chuckle nuts
	"Actor enters the given posture (enters immediately rather than plays the entry animation if argument is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_posture_set,
	0,
	0x00735DE0, // $TODO write the function chuckle nuts
	"Actor enters the given posture (enters immediately rather than plays the entry animation if argument is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cs_posture_exit,
	0,
	0x007365D0, // $TODO write the function chuckle nuts
	"Actor exits the posture it's in\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_posture_exit,
	0,
	0x00736AA0, // $TODO write the function chuckle nuts
	"Actor exits the posture it's in\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cs_stow,
	0,
	0x00737280, // $TODO write the function chuckle nuts
	"Actor stows its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_stow,
	0,
	0x00737910, // $TODO write the function chuckle nuts
	"Actor stows its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cs_draw,
	0,
	0x00738010, // $TODO write the function chuckle nuts
	"Actor draws its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_draw,
	0,
	0x00738480, // $TODO write the function chuckle nuts
	"Actor draws its current weapon\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cs_teleport,
	0,
	0x00738C70, // $TODO write the function chuckle nuts
	"Actor teleports to point1 facing point2\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_teleport,
	0,
	0x00739400, // $TODO write the function chuckle nuts
	"Actor teleports to point1 facing point2\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_point_ref, _hs_type_point_ref
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cs_stop_custom_animation,
	0,
	0x00739C40, // $TODO write the function chuckle nuts
	"Stop running a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vs_stop_custom_animation,
	0,
	0x0073A160, // $TODO write the function chuckle nuts
	"Stop running a custom animation\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_stop_sound,
	0,
	0x0073A930, // $TODO write the function chuckle nuts
	"Stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vs_stop_sound,
	0,
	0x0073B000, // $TODO write the function chuckle nuts
	"Stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_player_melee,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073B810, // $TODO write the function chuckle nuts
	"Actor performs player-specific melee\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_player_melee,
	0,
	0x0073BE30, // $TODO write the function chuckle nuts
	"Actor performs player-specific melee\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_melee_direction,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073C820, // $TODO write the function chuckle nuts
	"Actor performs melee in provided direction (0= front, 1= back, 2= offhand, 3= tackle)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_melee_direction,
	0,
	0x0073D050, // $TODO write the function chuckle nuts
	"Actor performs melee in provided direction (0= front, 1= back, 2= offhand, 3= tackle)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cs_smash_direction,
	FLAG(_hs_function_flag_command_script_atom),
	0x0073DAC0, // $TODO write the function chuckle nuts
	"Actor performs smash in provided direction\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	vs_smash_direction,
	0,
	0x0073E180, // $TODO write the function chuckle nuts
	"Actor performs smash in provided direction\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	camera_control,
	0,
	director_script_camera, // 0x0073EC20,
	"toggles script control of the camera.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set,
	0,
	0x0073EEC0, // $TODO write the function chuckle nuts
	"moves the camera to the specified camera point over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_cutscene_camera_point, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	camera_set_relative,
	0,
	0x0073F3D0, // $TODO write the function chuckle nuts
	"moves the camera to the specified camera point over the specified number of ticks (position is relative to the specified object).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_cutscene_camera_point, _hs_type_short_integer, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set_animation,
	0,
	0x0073F7F0, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_animation_graph, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	camera_set_animation_relative,
	0,
	0x0073FBC0, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_animation_graph, _hs_type_string_id, _hs_type_unit, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	camera_set_animation_with_speed,
	0,
	0x00740030, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_animation_graph, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	camera_set_animation_relative_with_speed,
	0,
	0x007403F0, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_animation_graph, _hs_type_string_id, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	camera_set_animation_relative_with_speed_loop,
	0,
	0x007409A0, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_animation_graph, _hs_type_string_id, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 14,
	_hs_type_void,
	camera_set_animation_relative_with_speed_loop_offset,
	0,
	0x00740E20, // $TODO write the function chuckle nuts
	"begins a prerecorded camera animation synchronized to unit relative to cutscene flag.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	7, _hs_type_animation_graph, _hs_type_string_id, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_real, _hs_type_boolean, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	camera_predict_resources_at_frame,
	0,
	0x007413E0, // $TODO write the function chuckle nuts
	"predict resources at a frame in camera animation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_animation_graph, _hs_type_string_id, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	camera_predict_resources_at_point,
	0,
	0x007419B0, // $TODO write the function chuckle nuts
	"predict resources given a camera point\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_cutscene_camera_point
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	camera_set_first_person,
	0,
	0x00741D30, // $TODO write the function chuckle nuts
	"makes the scripted camera follow a unit.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	camera_set_cinematic,
	0,
	0x00741F90, // $TODO write the function chuckle nuts
	"makes the scripted camera follow the path of a cinematic scene.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	camera_set_cinematic_scene,
	0,
	0x00742330, // $TODO write the function chuckle nuts
	"sets the scene, shot, and cutscene flag for the cinematic camera (NOTE: camera must be in cinematic mode by calling camera_set_cinematic).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_cinematic_scene_definition, _hs_type_long_integer, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	camera_place_relative,
	0,
	0x00742710, // $TODO write the function chuckle nuts
	"all subsequent camera placement in sapien be marked as relative to this object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	camera_place_worldspace,
	0,
	0x007429F0, // $TODO write the function chuckle nuts
	"all subsequent camera placement in sapien will be marked as worldspace\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_short_integer,
	camera_time,
	0,
	0x00742CF0, // $TODO write the function chuckle nuts
	"returns the number of ticks remaining in the current camera interpolation.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set_field_of_view,
	0,
	0x00742ED0, // $TODO write the function chuckle nuts
	"sets the field of view\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	cinematic_camera_set_easing_in,
	0,
	0x00743360, // $TODO write the function chuckle nuts
	"controls the camera easing from player position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_camera_set_easing_out,
	0,
	0x007438C0, // $TODO write the function chuckle nuts
	"controls the camera easing back to the player position\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_print,
	0,
	0x00743CC0, // $TODO write the function chuckle nuts
	"Print status lines for cinematic debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set_pan,
	0,
	0x00744030, // $TODO write the function chuckle nuts
	"moves the camera to the specified camera point over the specified number of ticks with a constant speed.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_cutscene_camera_point, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 14,
	_hs_type_void,
	camera_pan,
	0,
	0x007443C0, // $TODO write the function chuckle nuts
	"camera_pan <start point> <end point> <ticks> <ease-in ticks> <start velocity scale> <ease-out ticks> <end velocity scale>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	7, _hs_type_cutscene_camera_point, _hs_type_cutscene_camera_point, _hs_type_short_integer, _hs_type_short_integer, _hs_type_real, _hs_type_short_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	debug_camera_save,
	0,
	director_save_camera, // 0x00744AD0,
	"saves the camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	debug_camera_load,
	0,
	director_load_camera, // 0x00744C60,
	"loads the saved camera position and facing.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_camera_save_name,
	0,
	0x00744EF0, // $TODO write the function chuckle nuts
	"saves the camera position and facing to filename\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_camera_load_name,
	0,
	0x00745250, // $TODO write the function chuckle nuts
	"loads the camera position and facing from filename\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	director_debug_camera,
	0,
	director_debug_camera, // 0x00745420,
	"enable/disable camera debugging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_enum_game_difficulty,
	game_difficulty_get,
	0,
	0x00745610, // $TODO write the function chuckle nuts
	"returns the current difficulty setting, but lies to you and will never return easy, instead returning normal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_enum_game_difficulty,
	game_difficulty_get_real,
	0,
	0x00745A90, // $TODO write the function chuckle nuts
	"returns the actual current difficulty setting without lying\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_short_integer,
	game_insertion_point_get,
	0,
	0x00745C90, // $TODO write the function chuckle nuts
	"returns the number of the insertion point the map was started at\r\nNETWORK SAFE: Yes.",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_insertion_point_set,
	0,
	0x00746040, // $TODO write the function chuckle nuts
	"retroactively sets the insertion point that we started at.\r\nNETWORK SAFE: Unknown, assumed unsafe.",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	pvs_set_object,
	0,
	0x00746250, // $TODO write the function chuckle nuts
	"sets the specified object as the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	pvs_set_camera,
	0,
	0x00746530, // $TODO write the function chuckle nuts
	"sets the specified cutscene camera point as the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_cutscene_camera_point
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	pvs_clear,
	0,
	0x00746830, // $TODO write the function chuckle nuts
	"removes the special place that activates everything it sees.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	pvs_reset,
	0,
	0x00746B50, // $TODO write the function chuckle nuts
	"forces pvs to empty then back to full.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	players_unzoom_all,
	0,
	0x00746D10, // $TODO write the function chuckle nuts
	"resets zoom levels on all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_enable_input,
	0,
	0x00746F60, // $TODO write the function chuckle nuts
	"toggle player input. the player can still free-look, but nothing else.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_disable_movement,
	0,
	0x007472C0, // $TODO write the function chuckle nuts
	"toggle player input. the look stick works, but nothing else.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_disable_weapon_pickup,
	0,
	0x007476D0, // $TODO write the function chuckle nuts
	"enable/disable all players' ability to pick up weapons\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_disable_sprint,
	0,
	0x00747980, // $TODO write the function chuckle nuts
	"enable/disable all players' ability to sprint\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_disable_equipment_use,
	0,
	0x00747E90, // $TODO write the function chuckle nuts
	"enable/disable all players' ability to equipment\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_flashlight_on,
	0,
	0x007480C0, // $TODO write the function chuckle nuts
	"returns true if any player has a flashlight on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_active_camouflage_on,
	0,
	0x00748430, // $TODO write the function chuckle nuts
	"returns true if any player is active camouflaged\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	player_camera_control,
	0,
	0x00748780, // $TODO write the function chuckle nuts
	"enables/disables camera control globally\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_action_test_reset,
	0,
	0x0072F8A0, // $TODO write the function chuckle nuts
	"resets the player action test state so that all tests will return false.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_jump,
	0,
	0x0072FB90, // $TODO write the function chuckle nuts
	"returns true if any player has jumped since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_primary_trigger,
	0,
	0x0072FE50, // $TODO write the function chuckle nuts
	"returns true if any player has used primary trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_grenade_trigger,
	0,
	0x00730220, // $TODO write the function chuckle nuts
	"returns true if any player has used grenade trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_vision_trigger,
	0,
	0x00730520, // $TODO write the function chuckle nuts
	"returns true if any player has used vision trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_rotate_weapons,
	0,
	0x007307C0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the rotate-weapon button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_rotate_grenades,
	0,
	0x00730BD0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the rotate-grenades button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_melee,
	0,
	0x00730EB0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the melee button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_action,
	0,
	0x007311F0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the action key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_accept,
	0,
	0x00731590, // $TODO write the function chuckle nuts
	"returns true if any player has hit accept since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_cancel,
	0,
	0x007317C0, // $TODO write the function chuckle nuts
	"returns true if any player has hit cancel key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_look_relative_up,
	0,
	0x00731B50, // $TODO write the function chuckle nuts
	"returns true if any player has looked up since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_look_relative_down,
	0,
	0x00731EB0, // $TODO write the function chuckle nuts
	"returns true if any player has looked down since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_look_relative_left,
	0,
	0x007320E0, // $TODO write the function chuckle nuts
	"returns true if any player has looked left since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_look_relative_right,
	0,
	0x00732370, // $TODO write the function chuckle nuts
	"returns true if any player has looked right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_look_relative_all_directions,
	0,
	0x007327B0, // $TODO write the function chuckle nuts
	"returns true if any player has looked up, down, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_move_relative_all_directions,
	0,
	0x00732A80, // $TODO write the function chuckle nuts
	"returns true if any player has moved forward, backward, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_back,
	0,
	0x00732DA0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the back button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_dpad_left,
	0,
	0x00733130, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad left button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_dpad_right,
	0,
	0x00733370, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad right button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_dpad_up,
	0,
	0x00733730, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad up button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_dpad_down,
	0,
	0x00733A20, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad down button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_x,
	0,
	0x00733D50, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the x button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_y,
	0,
	0x007340A0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the y button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_left_shoulder,
	0,
	0x007343B0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the left shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_right_shoulder,
	0,
	0x00734600, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the right shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	player_action_test_waypoint_activate,
	0,
	0x00734A30, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the button action to activate the waypoint since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_action_test_reset,
	0,
	0x00734E60, // $TODO write the function chuckle nuts
	"resets the player action test state so that all tests will return false.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_primary_trigger,
	0,
	0x00735200, // $TODO write the function chuckle nuts
	"returns true if any player has used primary trigger since the last call to (player_action_test_reset).",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_vision_trigger,
	0,
	0x00735660, // $TODO write the function chuckle nuts
	"returns true if any player has used vision trigger since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_rotate_weapons,
	0,
	0x00735B90, // $TODO write the function chuckle nuts
	"returns true if any player has hit the rotate-weapon button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_rotate_grenades,
	0,
	0x00735F90, // $TODO write the function chuckle nuts
	"returns true if any player has hit the rotate-grenades button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_melee,
	0,
	0x007363E0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the melee button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_action,
	0,
	0x007367D0, // $TODO write the function chuckle nuts
	"returns true if any player has hit the action key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_accept,
	0,
	0x00736BE0, // $TODO write the function chuckle nuts
	"returns true if any player has hit accept since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_cancel,
	0,
	0x007370A0, // $TODO write the function chuckle nuts
	"returns true if any player has hit cancel key since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_look_relative_up,
	0,
	0x007374E0, // $TODO write the function chuckle nuts
	"returns true if any player has looked up since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_look_relative_down,
	0,
	0x007379B0, // $TODO write the function chuckle nuts
	"returns true if any player has looked down since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_look_relative_left,
	0,
	0x00737CF0, // $TODO write the function chuckle nuts
	"returns true if any player has looked left since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_look_relative_right,
	0,
	0x00738220, // $TODO write the function chuckle nuts
	"returns true if any player has looked right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_look_relative_all_directions,
	0,
	0x00738620, // $TODO write the function chuckle nuts
	"returns true if any player has looked up, down, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_move_relative_all_directions,
	0,
	0x00738B00, // $TODO write the function chuckle nuts
	"returns true if any player has moved forward, backward, left, and right since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_back,
	0,
	0x00738EC0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the back button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_dpad_left,
	0,
	0x00739350, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad left button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_dpad_right,
	0,
	0x007397D0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad right button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_dpad_up,
	0,
	0x00739BE0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad up button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_dpad_down,
	0,
	0x0073A040, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the dpad down button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_x,
	0,
	0x0073A4E0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the x button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_y,
	0,
	0x0073A8B0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the y button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_left_shoulder,
	0,
	0x0073ADF0, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the left shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_action_test_right_shoulder,
	0,
	0x0073B190, // $TODO write the function chuckle nuts
	"returns true if any player has pressed the right shoulder button since the last call to (player_action_test_reset).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player0_looking_up,
	0,
	0x0073B4B0, // $TODO write the function chuckle nuts
	"true if the first player is looking up\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player0_looking_down,
	0,
	0x0073B910, // $TODO write the function chuckle nuts
	"true if the first player is looking down\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player0_set_pitch,
	0,
	0x0073BBC0, // $TODO write the function chuckle nuts
	"sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player1_set_pitch,
	0,
	0x0073C050, // $TODO write the function chuckle nuts
	"sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player2_set_pitch,
	0,
	0x0073C3E0, // $TODO write the function chuckle nuts
	"sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player3_set_pitch,
	0,
	0x0073C6D0, // $TODO write the function chuckle nuts
	"sets up player look up test\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_lookstick_forward,
	0,
	0x0073CC60, // $TODO write the function chuckle nuts
	"true if the first player pushed forward on lookstick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_action_test_lookstick_backward,
	0,
	0x0073CE50, // $TODO write the function chuckle nuts
	"true if the first player pushed backward on lookstick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_teleport_player,
	0,
	0x0073D240, // $TODO write the function chuckle nuts
	"for testing: teleports one player to another's location\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	map_reset,
	0,
	main_reset_map, // 0x0073D490
	"starts the map from the beginning.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	map_reset_random,
	0,
	main_reset_map_random, // 0x0073D6E0
	"starts the map from the beginning with a new random seed.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	switch_bsp,
	0,
	0x0073D820, // $TODO write the function chuckle nuts
	"switches to a different zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	switch_zone_set,
	0,
	0x0073DDB0, // $TODO write the function chuckle nuts
	"switches to a different zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_zone_set
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	current_zone_set,
	0,
	0x0073E040, // $TODO write the function chuckle nuts
	"returns the current zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	current_zone_set_fully_active,
	0,
	0x0073E440, // $TODO write the function chuckle nuts
	"returns the current zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	switch_map_and_zone_set,
	0,
	0x0073E6F0, // $TODO write the function chuckle nuts
	"switches to a different scenario and zone set\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	crash,
	0,
	main_crash, // 0x0073E970,
	"crashes (for debugging).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	version,
	0,
	0x0073EAB0, // $TODO write the function chuckle nuts
	"prints the build version.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	status,
	0,
	main_status_print, // 0x0073EE50,
	"prints the value of all global status variables.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	record_movie,
	0,
	0x0073F0B0, // $TODO write the function chuckle nuts
	"<frame rate> <seconds> <width> <height>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_short_integer, _hs_type_real, _hs_type_short_integer, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	record_movie_distributed,
	0,
	0x0073F220, // $TODO write the function chuckle nuts
	"<frame rate> <seconds> <screen size> <mod count> <mod index>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_short_integer, _hs_type_long_integer, _hs_type_short_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	screenshot,
	0,
	0x0073F590, // $TODO write the function chuckle nuts
	"takes a screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	screenshot_debug,
	0,
	0x0073F740, // $TODO write the function chuckle nuts
	"takes a screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	screenshot_big,
	0,
	0x0073F9E0, // $TODO write the function chuckle nuts
	"takes an NxN multiple-page screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_short_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	screenshot_big_raw,
	0,
	0x0073FC30, // $TODO write the function chuckle nuts
	"takes an NxN multiple-page screenshot and saves as an HDR <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_short_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	screenshot_size,
	0,
	0x0073FDA0, // $TODO write the function chuckle nuts
	"takes a <width> x <height> screenshot and saves as an HDR <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_short_integer, _hs_type_short_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	screenshot_simple,
	0,
	0x00740160, // $TODO write the function chuckle nuts
	"takes a simple screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	screenshot_cubemap,
	0,
	0x007402A0, // $TODO write the function chuckle nuts
	"takes a cubemap screenshot and saves as <name>.tif\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	screenshot_webmap,
	0,
	0x00740590, // $TODO write the function chuckle nuts
	"takes two special screenshots and saves them, along with the camera information, as <name>.tif, <name>_secondary.tif and <name>_camera.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	screenshot_unknown344,
	0,
	0x007407E0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	screenshot_capture,
	0,
	0x00740A10, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	screenshot_set_path,
	0,
	0x00740BB0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	screenshot_unknown347,
	0,
	0x00740FA0, // $TODO write the function chuckle nuts
	"",
	NULL,
	5, _hs_type_string, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	main_menu,
	0,
	main_menu_launch_force, // 0x007413C0
	"goes back to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	main_halt,
	0,
	main_halt_and_display_errors, // 0x00741740
	"goes to a halted pregame state\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	map_name,
	0,
	main_game_launch_legacy, // 0x00741960
	"the same as game_start: launches a game for debugging purposes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_multiplayer,
	0,
	main_game_launch_set_multiplayer_engine, // 0x00741B30
	"debug map launching: sets the multiplayer engine for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_set_variant,
	0,
	0x00741DF0, // $TODO write the function chuckle nuts
	"Launches a game variant a on a map",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_splitscreen,
	0,
	main_game_launch_set_multiplayer_splitscreen_count, // 0x00741F40
	"debug map launching: sets the number of multiplayer splitscreen players for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_difficulty,
	0,
	main_game_launch_set_difficulty, // 0x00742280,
	"debug map launching: sets the difficulty of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_enum_game_difficulty
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_active_primary_skulls,
	0,
	main_game_launch_set_active_primary_skulls, // 0x00742540,
	"debug map launching: sets the active primary skulls of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_active_secondary_skulls,
	0,
	main_game_launch_set_active_secondary_skulls, // 0x007426F0,
	"debug map launching: sets the active primary skulls of the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_coop_players,
	0,
	main_game_launch_set_coop_player_count, // 0x007429D0,
	"debug map launching: sets the number of coop players for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_initial_zone_set,
	0,
	main_game_launch_set_initial_zone_set_index, // 0x00742CD0,
	"debug map launching: sets the initial bsp for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_tick_rate,
	0,
	main_game_launch_set_tick_rate, // 0x00742E30,
	"debug map launching: sets the tick rate for the next map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_start,
	0,
	main_game_launch, // 0x00743050
	"debug map launching: starts a game on the specified map.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	game_start_when_ready,
	0,
	0x00743340, // $TODO write the function chuckle nuts
	"nicely starts the current game once the pregame lobby is stable\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_start_when_joined,
	0,
	0x007435B0, // $TODO write the function chuckle nuts
	"nicely starts a game when the specified number of users are in the lobby\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	game_rate,
	0,
	0x00743730, // $TODO write the function chuckle nuts
	"changes game update rate (DANGER: only use if you know what you're doing!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	texture_cache_flush,
	0,
	0x00743CA0, // $TODO write the function chuckle nuts
	"don't make me kick your ass\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	geometry_cache_flush,
	0,
	0x00743F50, // $TODO write the function chuckle nuts
	"we fear change\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	sound_cache_flush,
	0,
	0x00744100, // $TODO write the function chuckle nuts
	"i'm a rebel!\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	animation_cache_flush,
	0,
	0x007443A0, // $TODO write the function chuckle nuts
	"it's just sooooo wrong\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	font_cache_flush,
	0,
	0x007446C0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	language_set,
	0,
	set_current_language_from_display_name_slow, // 0x00744910
	"change the language for localization\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	texture_cache_test_malloc,
	0,
	0x00744AF0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_memory,
	0,
	0x00744E20, // $TODO write the function chuckle nuts
	"dumps memory leaks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_memory_by_file,
	0,
	0x00745000, // $TODO write the function chuckle nuts
	"dumps memory leaks by source file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_memory_for_file,
	0,
	0x00745300, // $TODO write the function chuckle nuts
	"dumps memory leaks from the specified source file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_tags,
	0,
	0x00745440, // $TODO write the function chuckle nuts
	"writes all memory being used by tag files into tag_dump.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	tags_verify_all,
	0,
	0x00745700, // $TODO write the function chuckle nuts
	"verifies usage of infidel fields is correct\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_enable,
	0,
	0x00745A70, // $TODO write the function chuckle nuts
	"toggles the profile display on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_set_thread,
	0,
	0x00745BC0, // $TODO write the function chuckle nuts
	"sets the thread index being profiled\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_set_sort_method,
	0,
	0x00745E20, // $TODO write the function chuckle nuts
	"sets the sorting method used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_set_range,
	0,
	0x007460B0, // $TODO write the function chuckle nuts
	"sets the collection range used\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_set_attribute,
	0,
	0x007462C0, // $TODO write the function chuckle nuts
	"sets an attribute to profile by name\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	trace_next_frame,
	0,
	0x007464F0, // $TODO write the function chuckle nuts
	"creates a tracedump of the next frame\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	trace_next_frame_to_file,
	0,
	0x00746790, // $TODO write the function chuckle nuts
	"creates a tracedump of the next frame in a specific file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	trace_tick,
	0,
	0x007469A0, // $TODO write the function chuckle nuts
	"creates a tracedump of a specific game tick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	collision_log_enable,
	0,
	0x00746C60, // $TODO write the function chuckle nuts
	"enables or disables collision logging.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	damage_control_get,
	0,
	0x00746EA0, // $TODO write the function chuckle nuts
	"gets a damage control setting by string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	damage_control_set,
	0,
	0x00747120, // $TODO write the function chuckle nuts
	"sets a damage control setting by string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ai_lines,
	0,
	0x00747270, // $TODO write the function chuckle nuts
	"cycles through AI line-spray modes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ai_dialogue_break_on_vocalization,
	0,
	0x00747550, // $TODO write the function chuckle nuts
	"break when the following vocalization is chosen for utterance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	fade_in,
	0,
	0x00747740, // $TODO write the function chuckle nuts
	"does a screen fade in from a particular color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	fade_out,
	0,
	0x00747C60, // $TODO write the function chuckle nuts
	"does a screen fade out to a particular color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_start,
	0,
	0x00748030, // $TODO write the function chuckle nuts
	"initializes game to start a cinematic (interruptive) cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_stop,
	0,
	0x007482E0, // $TODO write the function chuckle nuts
	"initializes the game to end a cinematic (interruptive) cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_skip_start_internal,
	0,
	0x00748580, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_skip_stop_internal,
	0,
	0x007487E0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_show_letterbox,
	0,
	0x0072F650, // $TODO write the function chuckle nuts
	"sets or removes the letterbox bars\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_show_letterbox_immediate,
	0,
	0x0072FB40, // $TODO write the function chuckle nuts
	"sets or removes the letterbox bars\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_title,
	0,
	0x0072FE20, // $TODO write the function chuckle nuts
	"activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_cutscene_title
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cinematic_set_title_delayed,
	0,
	0x00730290, // $TODO write the function chuckle nuts
	"activates the chapter title, delayed by <real> seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_cutscene_title, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_suppress_bsp_object_creation,
	0,
	0x00730630, // $TODO write the function chuckle nuts
	"suppresses or enables the automatic creation of objects during cutscenes due to a bsp switch\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cinematic_subtitle,
	0,
	0x00730AE0, // $TODO write the function chuckle nuts
	"displays the named subtitle for <real> seconds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set,
	0,
	0x00730E00, // $TODO write the function chuckle nuts
	"sets the cinematic name for the current cinematic.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_cinematic_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cinematic_set_shot,
	0,
	0x007310C0, // $TODO write the function chuckle nuts
	"sets the scene and shot for the current cinematic.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_cinematic_scene_definition, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_get_shot,
	0,
	0x00731600, // $TODO write the function chuckle nuts
	"Print the stored scene and shot information to the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_early_exit,
	0,
	0x00731760, // $TODO write the function chuckle nuts
	"Used to store the state of skippable cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	cinematic_get_early_exit,
	0,
	0x00731B30, // $TODO write the function chuckle nuts
	"Used to retrieve the state of skippable cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_active_camera,
	0,
	0x00731E80, // $TODO write the function chuckle nuts
	"Used to change the rasterizer to display one of the texture cameras\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_object_create,
	0,
	0x00732110, // $TODO write the function chuckle nuts
	"Creates an object for use by cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cinematic_object_create_cinematic_anchor,
	0,
	0x00732580, // $TODO write the function chuckle nuts
	"Creates a cinematic anchor object for use by cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_object_destroy,
	0,
	0x007328D0, // $TODO write the function chuckle nuts
	"Destroys an object created by cinematic_object_create\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_destroy,
	0,
	0x00732CB0, // $TODO write the function chuckle nuts
	"Releases everything created by the cinematics system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_clips_initialize_for_shot,
	0,
	0x00732EC0, // $TODO write the function chuckle nuts
	"intializes the cinematic clip planes for the given shot of the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_clips_destroy,
	0,
	0x00733250, // $TODO write the function chuckle nuts
	"destroys any existing cinematic clip planes for the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_lights_initialize_for_shot,
	0,
	0x00733440, // $TODO write the function chuckle nuts
	"intializes the cinematic lighting for the given shot of the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_lights_destroy,
	0,
	0x007338C0, // $TODO write the function chuckle nuts
	"destroys any existing cinematic lights for the current cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_lights_destroy_shot,
	0,
	0x00733A90, // $TODO write the function chuckle nuts
	"destroys any non-persistent existing cinematic lights for the current shot\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_light_object,
	0,
	0x00733CE0, // $TODO write the function chuckle nuts
	"attaches cinematic lights to <object> at <marker> using scenario cinematic lighting palette entry <scenario-cinematic-lighting-name> from point of view of camera reference <camera>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_object, _hs_type_string_id, _hs_type_cinematic_lightprobe, _hs_type_cutscene_camera_point
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_light_object_off,
	0,
	0x00734260, // $TODO write the function chuckle nuts
	"Turn off cinematic lighting for the object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_lighting_rebuild_all,
	0,
	0x007344B0, // $TODO write the function chuckle nuts
	"Rebuild all cinematic lighting (SH only)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	cinematic_lighting_update_dynamic_light_direction,
	0,
	0x00734760, // $TODO write the function chuckle nuts
	"Updates the direction of a loaded cinematic dynamic light",
	NULL,
	5, _hs_type_string, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 16,
	_hs_type_void,
	cinematic_lighting_update_sh_light,
	0,
	0x00734CD0, // $TODO write the function chuckle nuts
	"Updates the direction, intensity, and color of a loaded cinematic sh light",
	NULL,
	8, _hs_type_string, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_object,
	cinematic_object_get,
	0,
	0x007353E0, // $TODO write the function chuckle nuts
	"Returns an object created by cinematic_object_create\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_reset,
	0,
	0x007357A0, // $TODO write the function chuckle nuts
	"Resets the cinematics internal state\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	camera_set_briefing,
	0,
	0x00735920, // $TODO write the function chuckle nuts
	"Used for briefing cinematics\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_cinematic_definition,
	cinematic_tag_reference_get_cinematic,
	0,
	0x00735DB0, // $TODO write the function chuckle nuts
	"Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_cinematic_scene_definition,
	cinematic_tag_reference_get_scene,
	0,
	0x007361E0, // $TODO write the function chuckle nuts
	"Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_effect,
	cinematic_tag_reference_get_effect,
	0,
	0x00736710, // $TODO write the function chuckle nuts
	"Retrieve an effect tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_sound,
	cinematic_tag_reference_get_dialogue,
	0,
	0x00736BA0, // $TODO write the function chuckle nuts
	"Retrieve a sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_sound,
	cinematic_tag_reference_get_music,
	0,
	0x00737130, // $TODO write the function chuckle nuts
	"Retrieve a sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_looping_sound,
	cinematic_tag_reference_get_music_looping,
	0,
	0x00737620, // $TODO write the function chuckle nuts
	"Retrieve a looping sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_animation_graph,
	cinematic_tag_reference_get_animation,
	0,
	0x00737C30, // $TODO write the function chuckle nuts
	"Retrieve a looping sound tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	cinematic_scripting_object_coop_flags_valid,
	0,
	0x007381D0, // $TODO write the function chuckle nuts
	"Evaluate if a cinematic object should be displayed for the current coop flags\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_fade_out,
	0,
	0x00738800, // $TODO write the function chuckle nuts
	"Fade out at the end of the cinematic\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_scripting_create_object,
	0,
	0x00738BE0, // $TODO write the function chuckle nuts
	"Create a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_scripting_create_cinematic_object,
	0,
	0x00738FE0, // $TODO write the function chuckle nuts
	"Create a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_start_animation,
	0,
	0x00739510, // $TODO write the function chuckle nuts
	"Animate a manually placed cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_scripting_destroy_object,
	0,
	0x00739930, // $TODO write the function chuckle nuts
	"Destroy a cinematic object\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	cinematic_scripting_start_effect,
	0,
	0x00739DB0, // $TODO write the function chuckle nuts
	"Play a cinematic effect\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_start_music,
	0,
	0x0073A2A0, // $TODO write the function chuckle nuts
	"Play a cinematic music\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	cinematic_scripting_start_dialogue,
	0,
	0x0073A770, // $TODO write the function chuckle nuts
	"Play a cinematic dialogue\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_stop_music,
	0,
	0x0073ACC0, // $TODO write the function chuckle nuts
	"Stops a cinematic music\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	cinematic_scripting_create_and_animate_object,
	0,
	0x0073B130, // $TODO write the function chuckle nuts
	"Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	cinematic_scripting_create_and_animate_cinematic_object,
	0,
	0x0073B6C0, // $TODO write the function chuckle nuts
	"Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_create_and_animate_object_no_animation,
	0,
	0x0073BB60, // $TODO write the function chuckle nuts
	"Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_create_and_animate_cinematic_object_no_animation,
	0,
	0x0073C010, // $TODO write the function chuckle nuts
	"Create and Animate a cinematic object.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_play_cortana_effect,
	0,
	0x0073C470, // $TODO write the function chuckle nuts
	"Play the associated cortana effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	attract_mode_start,
	0,
	0x0073CA40, // $TODO write the function chuckle nuts
	"starts an attract mode movie\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	attract_mode_set_seconds,
	0,
	0x0073CC40, // $TODO write the function chuckle nuts
	"sets number of seconds of the attract mode countdown timer.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_level_advance,
	0,
	0x0073CE00, // $TODO write the function chuckle nuts
	"transition game to another level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_won,
	0,
	game_won, // 0x0073D190
	"causes the player to successfully finish the current level and move to the next\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_lost,
	0,
	0x0073D440, // $TODO write the function chuckle nuts
	"marks the game as lost or not lost\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_revert,
	0,
	main_revert_map_scripting, // 0x0073D7A0
	"causes the player to revert to his previous saved game (for testing and cinematic skipping only please!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_is_cooperative,
	0,
	0x0073D980, // $TODO write the function chuckle nuts
	"returns TRUE if the game is cooperative\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_is_playtest,
	0,
	0x0073DDE0, // $TODO write the function chuckle nuts
	"returns the hs global boolean 'global_playtest_mode' which can be set in your init.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_can_use_flashlights,
	0,
	0x0073E010, // $TODO write the function chuckle nuts
	"allows or disallows the user of player flashlights\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_and_quit,
	0,
	main_save_and_exit_campaign, // 0x0073E480,
	"save & quit to the main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_unsafe,
	0,
	main_save_map, // 0x0073E680,
	"saves right now, even if the game is in an immediate-loss state (NEVER USE THIS! EVER!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_insertion_point_unlock,
	0,
	0x0073E900, // $TODO write the function chuckle nuts
	"unlocks the given insertion point for the current map for all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_insertion_point_lock,
	0,
	0x0073EC70, // $TODO write the function chuckle nuts
	"unlocks the given insertion point for the current map for all players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	saved_games_enumerate,
	0,
	0x0073EF20, // $TODO write the function chuckle nuts
	"test enumeration of saved game files on a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	saved_games_delete_campaign_save,
	0,
	0x0073F1E0, // $TODO write the function chuckle nuts
	"delete the campaign saved game file for a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	saved_games_save_last_film,
	0,
	0x0073F510, // $TODO write the function chuckle nuts
	"test saving of last recorded film for a controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	saved_games_autosave_free_up_space,
	0,
	0x0073F6E0, // $TODO write the function chuckle nuts
	"tell the autosave queue to run its cleanup routine for each content type\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	content_catalogue_display_status,
	0,
	0x0073F970, // $TODO write the function chuckle nuts
	"display the status of content catalogue for a controller, optionally showing only open content items\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_enum_controller, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	achievement_grant_to_player,
	0,
	0x0073FB80, // $TODO write the function chuckle nuts
	"award named achievement to player\r\nNETWORK SAFE: Safe",
	NULL,
	2, _hs_type_unit, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	achievement_grant_to_all_players,
	0,
	0x0073FFA0, // $TODO write the function chuckle nuts
	"award named achievement to all players\r\nNETWORK SAFE: Safe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	achievements_enable,
	0,
	0x00740280, // $TODO write the function chuckle nuts
	"enable or disable the awarding of achievements\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	achievements_skip_validation_checks,
	0,
	0x00740570, // $TODO write the function chuckle nuts
	"toggle the disabling of validation checks on achievement awarding\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_influencers,
	0,
	0x007407C0, // $TODO write the function chuckle nuts
	"toggle debugging of any & all dynamic respawn influencers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_respawn_zones,
	0,
	0x007409F0, // $TODO write the function chuckle nuts
	"debugs respawn zone influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_proximity_forbid,
	0,
	0x00740B90, // $TODO write the function chuckle nuts
	"debugs enemy proximity forbid influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_moving_vehicle,
	0,
	0x00740DE0, // $TODO write the function chuckle nuts
	"debugs moving vehicle danger influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_weapon_influences,
	0,
	0x007411A0, // $TODO write the function chuckle nuts
	"debugs enemy weapon danger influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_dangerous_projectiles,
	0,
	0x007412D0, // $TODO write the function chuckle nuts
	"debugs dangerous projectile influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_deployed_equipment,
	0,
	0x007415D0, // $TODO write the function chuckle nuts
	"debugs deployed equipment influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_proximity_enemy,
	0,
	0x007417A0, // $TODO write the function chuckle nuts
	"debugs enemy proximity bias influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_teammates,
	0,
	0x007419E0, // $TODO write the function chuckle nuts
	"debugs teammate proximity bias influences\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_random_influence,
	0,
	0x00741B90, // $TODO write the function chuckle nuts
	"toggles display of random influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_nominal_weight,
	0,
	0x00741E10, // $TODO write the function chuckle nuts
	"toggles display of the nominal weight influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_natural_weight,
	0,
	0x007420C0, // $TODO write the function chuckle nuts
	"toggles display of the natural weight influence contribution\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_target,
	0,
	0x00742370, // $TODO write the function chuckle nuts
	"sets target player for respawn influencer debugging display\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_spawning_use_history,
	0,
	0x007425D0, // $TODO write the function chuckle nuts
	"sets the saved respawn state for respawn influencer debugging display\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_initial_spawn_point_objects,
	0,
	0x00742730, // $TODO write the function chuckle nuts
	"toggles debug cycling through multiplayer initial spawn points objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_respawn_point_objects,
	0,
	0x00742A70, // $TODO write the function chuckle nuts
	"toggles debug cycling through multiplayer respawn points objects\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	game_export_variant_settings,
	0,
	game_engine_dump_variant_settings, // 0x00742D90,
	"export the current game engine variant settings to the specified text file\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_general,
	0,
	0x00742EB0, // $TODO write the function chuckle nuts
	"test game engine general event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_flavor,
	0,
	0x00743150, // $TODO write the function chuckle nuts
	"test game engine flavor event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_slayer,
	0,
	0x00743500, // $TODO write the function chuckle nuts
	"test game engine slayer event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_ctf,
	0,
	0x00743640, // $TODO write the function chuckle nuts
	"test game engine ctf event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_oddball,
	0,
	0x007438A0, // $TODO write the function chuckle nuts
	"test game engine oddball event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_king,
	0,
	0x00743C20, // $TODO write the function chuckle nuts
	"test game engine king event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_vip,
	0,
	0x00743EB0, // $TODO write the function chuckle nuts
	"test game engine vip event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_juggernaut,
	0,
	0x00744010, // $TODO write the function chuckle nuts
	"test game engine juggernaut event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_territories,
	0,
	0x00744310, // $TODO write the function chuckle nuts
	"test game engine territories event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_assault,
	0,
	0x00744490, // $TODO write the function chuckle nuts
	"test game engine assault event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_engine_event_test_infection,
	0,
	0x00744870, // $TODO write the function chuckle nuts
	"test game engine infection event\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	core_load,
	0,
	main_load_core, // 0x00744A50
	"loads debug game state from core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	core_load_name,
	0,
	main_load_core_name, // 0x00744C10
	"loads debug game state from core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	core_save,
	0,
	main_save_core, // 0x00744FE0
	"saves debug game state to core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	core_save_name,
	0,
	main_save_core_name, // 0x007452D0
	"saves debug game state to core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	core_load_game,
	0,
	main_game_load_from_core, // 0x00745520
	"loads level and game state from core\\core.bin\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	core_load_game_name,
	0,
	main_game_load_from_core_name, // 0x007457B0
	"loads level and game state from core\\<path>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	core_regular_upload_to_debug_server,
	0,
	0x00745BA0, // $TODO write the function chuckle nuts
	"toggle periodic core uploading to debug server\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	core_set_upload_option,
	0,
	0x00745CC0, // $TODO write the function chuckle nuts
	"sets options for game state uploading (current options are 'default', 'repro', and 'stress'\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	force_debugger_not_present,
	0,
	0x00746020, // $TODO write the function chuckle nuts
	"forces is_debugger_present() to return false\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	force_debugger_always_present,
	0,
	0x007461F0, // $TODO write the function chuckle nuts
	"forces is_debugger_present() to return true\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_safe_to_save,
	0,
	0x00746350, // $TODO write the function chuckle nuts
	"returns FALSE if it would be a bad idea to save the player's game right now\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_safe_to_speak,
	0,
	0x007466A0, // $TODO write the function chuckle nuts
	"returns FALSE if it would be a bad idea to play mission dialog right now\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_all_quiet,
	0,
	0x007469F0, // $TODO write the function chuckle nuts
	"returns FALSE if there are bad guys around, projectiles in the air, etc.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save,
	0,
	game_save_safe, // 0x00746CD0,
	"checks to see if it is safe to save game, then saves (gives up after 8 seconds)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_cancel,
	0,
	game_save_cancel, // 0x00746F20,
	"cancels any pending game_save, timeout or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_no_timeout,
	0,
	game_save_no_timeout, // 0x00747190,
	"checks to see if it is safe to save game, then saves (this version never gives up)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_immediate,
	0,
	game_save_immediate, // 0x00747350,
	"disregards player's current situation and saves (BE VERY CAREFUL!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_saving,
	0,
	0x00747650, // $TODO write the function chuckle nuts
	"checks to see if the game is trying to save the map.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	game_reverted,
	0,
	0x00747890, // $TODO write the function chuckle nuts
	"don't use this for anything, you black-hearted bastards.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	sound_ui_screen_init,
	0,
	0x00747D70, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	sound_ui_screen_term,
	0,
	0x00747F30, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 16,
	_hs_type_void,
	sound_set_tag_parameter_unsafe,
	0,
	0x007480A0, // $TODO write the function chuckle nuts
	"this is the second sign of the coming of the antichrist\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	8, _hs_type_string, _hs_type_string, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_impulse_predict,
	0,
	0x00748320, // $TODO write the function chuckle nuts
	"your mom part 2.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	sound_impulse_trigger,
	0,
	0x007486D0, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_impulse_start,
	0,
	0x0072F860, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	sound_impulse_start_cinematic,
	0,
	0x0072FD00, // $TODO write the function chuckle nuts
	"<sound> <object> <scale> <3d gain> <first person gain> plays an impulse sound from the specified source object.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	sound_impulse_start_effect,
	0,
	0x00730300, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale and effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	sound_impulse_start_with_subtitle,
	0,
	0x00730730, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale and displays a subtitle.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	sound_impulse_language_time,
	0,
	0x00730CD0, // $TODO write the function chuckle nuts
	"returns the time remaining for the specified impulse sound. DO NOT CALL IN CUTSCENES.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_impulse_stop,
	0,
	0x00731050, // $TODO write the function chuckle nuts
	"stops the specified impulse sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_impulse_start_3d,
	0,
	0x00731520, // $TODO write the function chuckle nuts
	"<sound> <azimuth> <scale> at the sound's minimum distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_impulse_mark_as_outro,
	0,
	0x007318E0, // $TODO write the function chuckle nuts
	"marks a sound as outro (meaning it plays past an unskipped cinematic).\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_impulse_start_naked,
	0,
	0x00731CB0, // $TODO write the function chuckle nuts
	"<sound> <pitch range name> <permutation index name> through the speakers it was encoded for\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_looping_predict,
	0,
	0x00731EE0, // $TODO write the function chuckle nuts
	"your mom.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_looping_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_looping_start,
	0,
	0x00732180, // $TODO write the function chuckle nuts
	"plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_looping_sound, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_looping_stop,
	0,
	0x00732690, // $TODO write the function chuckle nuts
	"stops the specified looping sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_looping_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	sound_looping_stop_immediately,
	0,
	0x00732960, // $TODO write the function chuckle nuts
	"stops the specified looping sound immediately.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_looping_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_looping_set_scale,
	0,
	0x00732D20, // $TODO write the function chuckle nuts
	"changes the scale of the sound (which should affect the volume) within the range 0 to 1.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_looping_sound, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_looping_set_alternate,
	0,
	0x00733180, // $TODO write the function chuckle nuts
	"enables or disables the alternate loop/alternate end for a looping sound.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_looping_sound, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	sound_loop_spam,
	0,
	scripting_looping_sound_spam, // 0x00733500,
	"start all loaded looping sounds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_class_show_channel,
	0,
	0x00733790, // $TODO write the function chuckle nuts
	"shows/hides sound classes w/ substring in debug_sound_channels view\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_class_debug_sound_start,
	0,
	0x00733A00, // $TODO write the function chuckle nuts
	"shows/hides when sounds of sound classes w/ substring start\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_sounds_enable,
	0,
	0x00733B40, // $TODO write the function chuckle nuts
	"enables or disables all sound classes matching the substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_class_set_gain,
	0,
	0x00733EB0, // $TODO write the function chuckle nuts
	"changes the gain on the specified sound class(es) to the specified gain over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_class_set_gain_db,
	0,
	0x00734290, // $TODO write the function chuckle nuts
	"changes the gain on the specified sound class(es) to the specified gain(dB) over the specified number of ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string, _hs_type_real, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_class_enable_ducker,
	0,
	0x00734630, // $TODO write the function chuckle nuts
	"enables or disables the ducker on all sound classes matching the substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_sound_environment_parameter,
	0,
	0x00734B70, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_set_global_effect,
	0,
	0x00734D90, // $TODO write the function chuckle nuts
	"bleh\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	sound_set_global_effect_scale,
	0,
	0x00735180, // $TODO write the function chuckle nuts
	"more bleh\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	vehicle_auto_turret,
	0,
	0x00735550, // $TODO write the function chuckle nuts
	"Sets the specified trigger volume and parameters for an auto turret\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_vehicle, _hs_type_trigger_volume, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	vehicle_hover,
	0,
	0x00735B50, // $TODO write the function chuckle nuts
	"stops the vehicle from running real physics and runs fake hovering physics instead.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_vehicle, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	vehicle_count_bipeds_killed,
	0,
	0x00735F60, // $TODO write the function chuckle nuts
	"returns how many people this vehicle has killed\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_vehicle
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	biped_ragdoll,
	0,
	0x00736360, // $TODO write the function chuckle nuts
	"given a dead biped, turns on ragdoll\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	water_float_reset,
	0,
	0x007366E0, // $TODO write the function chuckle nuts
	"allows an object to continue floating",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_show_training_text,
	0,
	0x007369D0, // $TODO write the function chuckle nuts
	"true turns on scripted training text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_set_training_text,
	0,
	0x00736E10, // $TODO write the function chuckle nuts
	"sets the string id fo the scripted training text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	hud_enable_training,
	0,
	0x00737100, // $TODO write the function chuckle nuts
	"true turns training on, false turns it off.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_training_activate_flashlight,
	0,
	0x007374C0, // $TODO write the function chuckle nuts
	"guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_training_activate_crouch,
	0,
	0x00737600, // $TODO write the function chuckle nuts
	"guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_training_activate_stealth,
	0,
	0x00737A10, // $TODO write the function chuckle nuts
	"guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_training_activate_equipment,
	0,
	0x00737B70, // $TODO write the function chuckle nuts
	"guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	player_training_activate_jump,
	0,
	0x00737D90, // $TODO write the function chuckle nuts
	"guess\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_training_reset,
	0,
	0x00738110, // $TODO write the function chuckle nuts
	"Resets all player training data\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	hud_activate_team_nav_point_flag,
	0,
	0x00738370, // $TODO write the function chuckle nuts
	"activates a nav point type <string> attached to a team anchored to a flag with a vertical offset <real>. If the player is not local to the machine, this will fail\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_enum_team, _hs_type_cutscene_flag, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	hud_deactivate_team_nav_point_flag,
	0,
	0x00738860, // $TODO write the function chuckle nuts
	"deactivates a nav point type attached to a team anchored to a flag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_team, _hs_type_cutscene_flag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	chud_cortana_suck,
	0,
	0x00738B80, // $TODO write the function chuckle nuts
	"turns on the cortana suck, attached to the object at the given marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_texture_cam,
	0,
	0x00738F80, // $TODO write the function chuckle nuts
	"turns the chud texturecam on/off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_cortana_set_range_multiplier,
	0,
	0x00739310, // $TODO write the function chuckle nuts
	"scales the cortana effect distances\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	play_cortana_effect,
	0,
	0x00739720, // $TODO write the function chuckle nuts
	"scales the cortana effect distances\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show,
	0,
	0x00739A40, // $TODO write the function chuckle nuts
	"hides/shows the entire chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_weapon_stats,
	0,
	0x00739E40, // $TODO write the function chuckle nuts
	"hides/shows the weapon huds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_crosshair,
	0,
	0x0073A0E0, // $TODO write the function chuckle nuts
	"hides/shows the crosshair\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_shield,
	0,
	0x0073A560, // $TODO write the function chuckle nuts
	"hides/shows the shield\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_grenades,
	0,
	0x0073A880, // $TODO write the function chuckle nuts
	"hides/shows the grenade hud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_messages,
	0,
	0x0073ABE0, // $TODO write the function chuckle nuts
	"hides/shows the chud messages\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_motion_sensor,
	0,
	0x0073AFD0, // $TODO write the function chuckle nuts
	"hides/shows the motion sensor chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_spike_grenades,
	0,
	0x0073B2A0, // $TODO write the function chuckle nuts
	"hides/shows the spike grenade chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_fire_grenades,
	0,
	0x0073B730, // $TODO write the function chuckle nuts
	"hides/shows the firebomb chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_compass,
	0,
	0x0073B9F0, // $TODO write the function chuckle nuts
	"hides/shows the compass chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_stamina,
	0,
	0x0073BDE0, // $TODO write the function chuckle nuts
	"hides/shows the stamina chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_energy_meters,
	0,
	0x0073C110, // $TODO write the function chuckle nuts
	"hides/shows the energy meters chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_show_consumables,
	0,
	0x0073C440, // $TODO write the function chuckle nuts
	"hides/shows the consumables chud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	chud_cinematic_fade,
	0,
	0x0073C7B0, // $TODO write the function chuckle nuts
	"parameter 1 is how, parameter 2 is when\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_bonus_round_show_timer,
	0,
	0x0073CC90, // $TODO write the function chuckle nuts
	"show the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_bonus_round_start_timer,
	0,
	0x0073D020, // $TODO write the function chuckle nuts
	"tick the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_bonus_round_set_timer,
	0,
	0x0073D380, // $TODO write the function chuckle nuts
	"set the bonus timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cls,
	0,
	0x0073D6C0, // $TODO write the function chuckle nuts
	"clears console text from the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	events_spam_suppression_enable,
	0,
	0x0073D800, // $TODO write the function chuckle nuts
	"enables or disables the suppression of event spamming\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	error_geometry_show,
	0,
	0x0073DBF0, // $TODO write the function chuckle nuts
	"highlights all error geometry with a name that includes the given substring\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	error_geometry_hide,
	0,
	0x0073DE70, // $TODO write the function chuckle nuts
	"hides all error geometry with a name that includes the given substring\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	error_geometry_show_all,
	0,
	0x0073DFF0, // $TODO write the function chuckle nuts
	"highlights all error geometry\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	error_geometry_hide_all,
	0,
	0x0073E420, // $TODO write the function chuckle nuts
	"hides all error geometry\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	error_geometry_list,
	0,
	0x0073E500, // $TODO write the function chuckle nuts
	"prints out a list of all error geometry types and counts\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	player_effect_set_max_translation,
	0,
	0x0073E730, // $TODO write the function chuckle nuts
	"<x> <y> <z>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	player_effect_set_max_rotation,
	0,
	0x0073EBD0, // $TODO write the function chuckle nuts
	"<yaw> <pitch> <roll>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_effect_set_max_rumble,
	0,
	0x0073F060, // $TODO write the function chuckle nuts
	"<left> <right>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_effect_start,
	0,
	0x0073F4A0, // $TODO write the function chuckle nuts
	"<max_intensity> <attack time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_effect_stop,
	0,
	0x0073F850, // $TODO write the function chuckle nuts
	"<decay>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	player_effect_set_max_translation_for_player,
	0,
	0x0073FB20, // $TODO write the function chuckle nuts
	"<x> <y> <z>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	player_effect_set_max_rotation_for_player,
	0,
	0x0073FFD0, // $TODO write the function chuckle nuts
	"<yaw> <pitch> <roll>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	player_effect_set_max_rumble_for_player,
	0,
	0x00740440, // $TODO write the function chuckle nuts
	"<left> <right>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	player_effect_start_for_player,
	0,
	0x007408C0, // $TODO write the function chuckle nuts
	"<max_intensity> <attack time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_effect_stop_for_player,
	0,
	0x00740C90, // $TODO write the function chuckle nuts
	"<decay>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	time_code_show,
	0,
	0x007411E0, // $TODO write the function chuckle nuts
	"shows the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	time_code_start,
	0,
	0x00741350, // $TODO write the function chuckle nuts
	"starts/stops the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	time_code_reset,
	0,
	0x00741610, // $TODO write the function chuckle nuts
	"resets the time code timer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	script_screen_effect_set_value,
	0,
	0x00741830, // $TODO write the function chuckle nuts
	"sets a screen effect script value\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_short_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_screen_effect_start,
	0,
	0x00741AB0, // $TODO write the function chuckle nuts
	"starts screen effect pass TRUE to clear\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_screen_effect_set_crossfade,
	0,
	0x00741C50, // $TODO write the function chuckle nuts
	"transition-time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	cinematic_screen_effect_set_crossfade,
	0,
	0x00741E50, // $TODO write the function chuckle nuts
	"transition-time\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_screen_effect_stop,
	0,
	0x007421D0, // $TODO write the function chuckle nuts
	"returns control of the screen effects to the rest of the game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_near_clip_distance,
	0,
	0x007423E0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_far_clip_distance,
	0,
	0x00742650, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_atmosphere_fog,
	0,
	0x007428A0, // $TODO write the function chuckle nuts
	"enable/disable atmosphere fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	atmosphere_fog_override_fade,
	0,
	0x00742C80, // $TODO write the function chuckle nuts
	"override the atmosphere fog setting to <index> by <percentage> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	motion_blur,
	0,
	0x00742FD0, // $TODO write the function chuckle nuts
	"enable/disable motion blur\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_weather,
	0,
	0x007434D0, // $TODO write the function chuckle nuts
	"enable/disable atmosphere fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_patchy_fog,
	0,
	0x00743700, // $TODO write the function chuckle nuts
	"enable/disable patchy fog\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_set_environment_map_attenuation,
	0,
	0x00743C00, // $TODO write the function chuckle nuts
	"interpolates environment-map attenuation (on flagged shaders) from <low> to <high> over <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_environment_map_bitmap,
	0,
	0x00743E90, // $TODO write the function chuckle nuts
	"sets environment-map bitmap (on flagged shaders) instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_bitmap
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_reset_environment_map_bitmap,
	0,
	0x00743FF0, // $TODO write the function chuckle nuts
	"resets environment-map bitmap (on flagged shaders) to default instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 16,
	_hs_type_void,
	cinematic_set_environment_map_tint,
	0,
	0x007442F0, // $TODO write the function chuckle nuts
	"perpendicular color: (red green blue brightness), parallel color: (red green blue brightness)... sets environment-map tint (on flagged shaders) instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	8, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_reset_environment_map_tint,
	0,
	0x00744470, // $TODO write the function chuckle nuts
	"resets environment-map tint (on flagged shaders) to default instantly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_layer,
	0,
	0x00744850, // $TODO write the function chuckle nuts
	"interpolates the value of <cinematic layer x> from current position to <value> over <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	player_has_skills,
	0,
	0x00744A20, // $TODO write the function chuckle nuts
	"Are you the best?\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_has_mad_secret_skills,
	0,
	0x00744CC0, // $TODO write the function chuckle nuts
	"You are the best!\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	controller_invert_look,
	0,
	0x00745110, // $TODO write the function chuckle nuts
	"invert look on all attached controllers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_look_speed,
	0,
	0x00745370, // $TODO write the function chuckle nuts
	"set look speed for all attached controllers\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_set_look_invert,
	0,
	0x007454C0, // $TODO write the function chuckle nuts
	"invert player0's look\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	controller_get_look_invert,
	0,
	0x00745A00, // $TODO write the function chuckle nuts
	"returns TRUE if player0's look pitch is inverted\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	controller_unlock_solo_levels,
	0,
	0x00745C40, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_look_inverted,
	0,
	debug_set_controller_look_inverted, // 0x00746090,
	"set look inversion for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_vibration_enabled,
	0,
	debug_set_controller_vibration_enabled, // 0x007462A0,
	"set vibration for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_flight_stick_aircraft_controls,
	0,
	debug_set_controller_flight_stick_aircraft_controls, // 0x007464D0,
	"set airrcraft flight stick controls for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_auto_center_look,
	0,
	debug_set_controller_auto_center_look, // 0x00746770,
	"set auto center look for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_crouch_lock,
	0,
	debug_set_controller_crouch_lock, // 0x00746980,
	"set crouch lock for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_button_preset,
	0,
	debug_set_button_preset, // 0x00746C40,
	"set button preset for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_button_preset
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_joystick_preset,
	0,
	debug_set_joystick_preset, // 0x00746E80,
	"set joystick preset for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_joystick_preset
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_look_sensitivity,
	0,
	debug_set_look_sensitivity, // 0x00747100,
	"set look sensitivity for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_unlock_single_player_levels,
	0,
	0x00747250, // $TODO write the function chuckle nuts
	"<controller> unlock all single player levels for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_lock_single_player_levels,
	0,
	0x00747530, // $TODO write the function chuckle nuts
	"<controller> lock all single player levels for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_unlock_skulls,
	0,
	0x00747720, // $TODO write the function chuckle nuts
	"<controller> unlock all campaign skulls for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_lock_skulls,
	0,
	0x00747930, // $TODO write the function chuckle nuts
	"<controller> lock all campaign skulls for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_unlock_models,
	0,
	0x00747C40, // $TODO write the function chuckle nuts
	"<controller> unlock all model permutations for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_lock_models,
	0,
	0x00747EF0, // $TODO write the function chuckle nuts
	"<controller> lock all model permutations for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 10,
	_hs_type_void,
	controller_set_single_player_level_completed,
	0,
	debug_set_single_player_level_completed, // 0x00748010,
	"<controller> <level index> <co-op> <difficulty> <completed> set the single player levels completed state for specified controller\r\nNETWORK SAFE: No",
	NULL,
	5, _hs_type_enum_controller, _hs_type_long_integer, _hs_type_boolean, _hs_type_enum_game_difficulty, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_player_character_type,
	0,
	debug_set_player_character_type, // 0x007482C0,
	"set player character type for specified controller",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_player_character_type
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	controller_set_emblem_info,
	0,
	0x00748560, // $TODO write the function chuckle nuts
	"set emblem for specified controller\r\nNETWORK SAFE: No",
	NULL,
	5, _hs_type_enum_controller, _hs_type_short_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_voice_output_setting,
	0,
	debug_set_voice_output_setting, // 0x007487C0,
	"set voice output setting for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_voice_output_setting
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_voice_mask,
	0,
	debug_set_voice_mask, // 0x0072F630,
	"set voice mask for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_voice_mask
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_subtitle_setting,
	0,
	debug_set_subtitle_setting, // 0x0072FA10,
	"set subtitle setting for specified controller\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_enum_controller, _hs_type_enum_subtitle_setting
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	controller_set_campaign_solo_high_score,
	0,
	0x0072FC20, // $TODO write the function chuckle nuts
	"<controller> <map_id> <score> set high score for specified controller\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_enum_controller, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	controller_set_campaign_coop_high_score,
	0,
	0x0072FE00, // $TODO write the function chuckle nuts
	"<controller> <map_id> <score> <player_count> set high score for specified controller\r\nNETWORK SAFE: No",
	NULL,
	4, _hs_type_enum_controller, _hs_type_long_integer, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	controller_set_survival_solo_high_score,
	0,
	0x007300F0, // $TODO write the function chuckle nuts
	"<controller> <map_id> <insertion point> <score> set high score for specified controller\r\nNETWORK SAFE: No",
	NULL,
	4, _hs_type_enum_controller, _hs_type_long_integer, _hs_type_short_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	controller_set_survival_coop_high_score,
	0,
	0x007303F0, // $TODO write the function chuckle nuts
	"<controller> <map_id> <insertion point> <score> <player_count> set high score for specified controller\r\nNETWORK SAFE: No",
	NULL,
	5, _hs_type_enum_controller, _hs_type_long_integer, _hs_type_short_integer, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_clear_high_scores,
	0,
	0x007305B0, // $TODO write the function chuckle nuts
	"clear high scores for specified controller\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	controller_set_unsignedin_user,
	0,
	0x007307A0, // $TODO write the function chuckle nuts
	"Toggle controller as unsigned-in user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	controller_display_storage_device_selection,
	0,
	0x00730B60, // $TODO write the function chuckle nuts
	"display storage device selection ui for the specified controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	font_cache_bitmap_save,
	0,
	0x00730D50, // $TODO write the function chuckle nuts
	"save font cache bitmap to targa file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	ui_debug_load_main_menu,
	0,
	0x00730FD0, // $TODO write the function chuckle nuts
	"loads the main menu screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ui_debug_text_bounds,
	0,
	0x007311D0, // $TODO write the function chuckle nuts
	"toggle rendering of ui text boundaries\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	ui_debug_text_font,
	0,
	user_interface_text_debug_display_font_index, // 0x00731480,
	"toggle display of ui text font\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ui_debug_show_title_safe_bounds,
	0,
	0x00731720, // $TODO write the function chuckle nuts
	"toggle display of title safe boundary\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ui_debug_element_bounds,
	0,
	0x007318C0, // $TODO write the function chuckle nuts
	"toggle rendering of widget tag block bounds\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ui_memory_dump,
	0,
	0x00731B10, // $TODO write the function chuckle nuts
	"dump the UI memory tracked allocations to a specified file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ui_time_scale_step,
	0,
	0x00731D90, // $TODO write the function chuckle nuts
	"move the ui time forward by a specified amount on next tick\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	xoverlapped_debug_render,
	0,
	overlapped_task_toggle_debug_rendering, // 0x00731F50,
	"toggle display a list of active xoverlapped tasks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_load_screen,
	0,
	0x00732310, // $TODO write the function chuckle nuts
	"load the named screen\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	gui_reset,
	0,
	window_manager_reset_screens, // 0x00732610
	"cleans out the halox ui screens and errors\r\nNETWORK SAFEL No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	gui_start,
	0,
	location_manager_start, // 0x00732820
	"tells the window location manager to resume\r\nNETWORK SAFEL No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	gui_stop,
	0,
	location_manager_stop, // 0x00732A60
	"tells the window location manager to stop bringing up screens on its own\r\nNETWORK SAFEL No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	gui_error_post,
	0,
	0x00732C90, // $TODO write the function chuckle nuts
	"<error_code> <controller_index> <must_be_cleared> sets an error condition in the ui\r\nNETWORK SAFEL No",
	NULL,
	3, _hs_type_string_id, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	gui_error_post_toast,
	0,
	0x00732EA0, // $TODO write the function chuckle nuts
	"<error_code> <controller_index> <must_be_cleared> sets an error condition in the ui\r\nNETWORK SAFEL No",
	NULL,
	3, _hs_type_string_id, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	gui_error_resolve,
	0,
	0x00733160, // $TODO write the function chuckle nuts
	"<error_name> <controller_index> resolves an error condition in the ui\r\nNETWORK SAFEL No",
	NULL,
	2, _hs_type_string_id, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	gui_error_clear,
	0,
	0x00733310, // $TODO write the function chuckle nuts
	"<error_name> <controller_index> clears an error condition in the ui\r\nNETWORK SAFEL No",
	NULL,
	2, _hs_type_string_id, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_dialog_show,
	0,
	0x00733650, // $TODO write the function chuckle nuts
	"<dialog name>\r\nNETWORK SAFEL No",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_spartan_milestone_show,
	0,
	0x007338A0, // $TODO write the function chuckle nuts
	"show dialog for milestone upgrade (1..3)\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_spartan_rank_show,
	0,
	0x00733A70, // $TODO write the function chuckle nuts
	"show toast for rank upgrade (1..36)\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	gui_print_active_screens,
	0,
	gui_print_active_screens, // 0x00733CC0,
	"display list of active halox ui screens\r\nNETWORK SAFEL No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	gui_print_active_screen_strings,
	0,
	gui_print_active_screen_strings_tag_name, // 0x00733F10,
	"display strings tag file name for current topmost screen\r\nNETWORK SAFE: No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_screen_name,
	0,
	gui_debug_screen_display_name, // 0x00734240,
	"Toggle display of given screen's name, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_screen_animation,
	0,
	gui_debug_screen_animation_state, // 0x007343E0,
	"Toggle display of given screen animations, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_screen_bounds,
	0,
	gui_debug_screen_bounds, // 0x007345E0,
	"Toggle display of given screen's bounds, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_screen_rotation,
	0,
	gui_debug_screen_rotation, // 0x00734950,
	"Toggle display of given screen's rotation, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_group_name,
	0,
	gui_debug_group_display_name, // 0x00734B50,
	"Toggle display of given group's name, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_group_animation,
	0,
	gui_debug_group_animation_state, // 0x00734D70,
	"Toggle display of given group's animations, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_group_bounds,
	0,
	gui_debug_group_bounds, // 0x00735010,
	"Toggle display of given group's bounds, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_group_rotation,
	0,
	gui_debug_group_rotation, // 0x00735310,
	"Toggle display of given group's rotation, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_name,
	0,
	gui_debug_list_display_name, // 0x00735530,
	"Toggle display of given list's name, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_animation,
	0,
	gui_debug_list_animation_state, // 0x00735780,
	"Toggle display of given list's animations, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_bounds,
	0,
	gui_debug_list_bounds, // 0x00735900,
	"Toggle display of given list's bounds, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_rotation,
	0,
	gui_debug_list_rotation, // 0x00735CD0,
	"Toggle display of given list's rotation, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_item_name,
	0,
	gui_debug_list_item_display_name, // 0x00735EF0,
	"Toggle display of given list item's name, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_item_animation,
	0,
	gui_debug_list_item_animation_state, // 0x00736050,
	"Toggle display of given list item's animation, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_item_bounds,
	0,
	gui_debug_list_item_bounds, // 0x00736340,
	"Toggle display of given list item's bounds, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	gui_debug_list_item_rotation,
	0,
	gui_debug_list_item_rotation, // 0x00736510,
	"Toggle display of given list item's rotation, optionally recursive\r\nNETWORK SAFE: No",
	NULL,
	3, _hs_type_string_id, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_text_name,
	0,
	gui_debug_text_display_name, // 0x007367B0,
	"Toggle display of given text widget's name\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_text_animation,
	0,
	gui_debug_text_animation_state, // 0x007369B0,
	"Toggle display of given text widget's animation state\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_text_bounds,
	0,
	gui_debug_text_bounds, // 0x00736C60,
	"Toggle display of given text widget's bounds\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_text_rotation,
	0,
	gui_debug_text_rotation, // 0x00736F00,
	"Toggle display of given text widget's rotation\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_bitmap_name,
	0,
	gui_debug_bitmap_display_name, // 0x007370E0,
	"Toggle display of given bitmap widget's name\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_bitmap_animation,
	0,
	gui_debug_bitmap_animation_state, // 0x00737380,
	"Toggle display of given bitmap widget's animation state\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_bitmap_bounds,
	0,
	gui_debug_bitmap_bounds, // 0x00737560,
	"Toggle display of given bitmap widget's bounds\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	gui_debug_bitmap_rotation,
	0,
	gui_debug_bitmap_rotation, // 0x007378F0,
	"Toggle display of given bitmap widget's rotation\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_debug_music_state,
	0,
	0x00737A70, // $TODO write the function chuckle nuts
	"Toggle display of UI music state\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cc_enable,
	0,
	0x00737C70, // $TODO write the function chuckle nuts
	"toggle closed captions\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cc_test,
	0,
	0x007380F0, // $TODO write the function chuckle nuts
	"test a line of cc text\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	objectives_clear,
	0,
	0x00738350, // $TODO write the function chuckle nuts
	"clears the mission objectives.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_show_up_to,
	0,
	0x00738450, // $TODO write the function chuckle nuts
	"show primary campaign objectives 0..n\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_finish_up_to,
	0,
	0x007388C0, // $TODO write the function chuckle nuts
	"mark primary campaign objectives 0..n as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_show,
	0,
	0x00738C40, // $TODO write the function chuckle nuts
	"show secondary campaign objective N\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_finish,
	0,
	0x00738FB0, // $TODO write the function chuckle nuts
	"mark secondary campaign objective N as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_unavailable,
	0,
	0x007393D0, // $TODO write the function chuckle nuts
	"mark secondary campaign objective N as unavailable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_secondary_show,
	0,
	0x00739750, // $TODO write the function chuckle nuts
	"show secondary campaign objective N\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_secondary_finish,
	0,
	0x00739A70, // $TODO write the function chuckle nuts
	"mark secondary campaign objective N as complete\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	objectives_secondary_unavailable,
	0,
	0x00739EF0, // $TODO write the function chuckle nuts
	"mark secondary campaign objective N as unavailable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	input_suppress_rumble,
	0,
	0x0073A130, // $TODO write the function chuckle nuts
	"disable the friggin' rumble\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	input_disable_claw_button_combos,
	0,
	0x0073A5D0, // $TODO write the function chuckle nuts
	"disabe the Claw button combos (for monkey testing)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	update_remote_camera,
	0,
	0x0073A7B0, // $TODO write the function chuckle nuts
	"force synchronization of remote machine camera\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	net_build_network_config,
	0,
	network_build_network_configuration, // 0x0073A9E0,
	"writes a new network configuration file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_build_game_variant,
	0,
	network_build_game_variant, // 0x0073ACA0,
	"writes the current game variant to a file\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_verify_game_variant,
	0,
	network_verify_packed_game_variant_file, // 0x0073AF90,
	"verifies the contents of a packed game variant file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_load_and_use_game_variant,
	0,
	network_load_and_use_packed_game_variant_file, // 0x0073B110,
	"loads the contents of a packed game variant file and submits to networking for use in the current game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_use_hopper_directory,
	0,
	0x0073B300, // $TODO write the function chuckle nuts
	"set which hopper directory to use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_quality_dump,
	0,
	0x0073B490, // $TODO write the function chuckle nuts
	"dump all network quality statistics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_quality_clear,
	0,
	0x0073B8D0, // $TODO write the function chuckle nuts
	"clear all network quality statistics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_connection_badness_history,
	0,
	0x0073BA20, // $TODO write the function chuckle nuts
	"set connection history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_squad_host_badness_history,
	0,
	0x0073BC80, // $TODO write the function chuckle nuts
	"set host badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_squad_client_badness_history,
	0,
	0x0073BF90, // $TODO write the function chuckle nuts
	"set client badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_group_host_badness_history,
	0,
	0x0073C190, // $TODO write the function chuckle nuts
	"set host badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_group_client_badness_history,
	0,
	0x0073C3C0, // $TODO write the function chuckle nuts
	"set client badness history, true/false, # of bits to set\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_quality_set_estimated_bandwidth,
	0,
	0x0073C5A0, // $TODO write the function chuckle nuts
	"set upstream bandwidth history, <is reliable> <bandwidth in kbps>\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_boolean, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_join_friend,
	0,
	0x0073C800, // $TODO write the function chuckle nuts
	"join a friend's squad session, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_join_squad_to_friend,
	0,
	0x0073CB50, // $TODO write the function chuckle nuts
	"join our squad to a friend's squad session, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_join_sessionid,
	0,
	0x0073CD10, // $TODO write the function chuckle nuts
	"join a squad session by sessionid, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_join_squad_to_sessionid,
	0,
	0x0073D000, // $TODO write the function chuckle nuts
	"join a squad session by sessionid, 'none' to cancel a previous join\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_enable_join_friend_loop,
	0,
	0x0073D220, // $TODO write the function chuckle nuts
	"randomly join a friend every <param> seconds, 0 to reset\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_set_maximum_player_count,
	0,
	0x0073D470, // $TODO write the function chuckle nuts
	"sets the maximum player count for this squad\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_set_campaign_insertion_point,
	0,
	0x0073D6A0, // $TODO write the function chuckle nuts
	"sets the campaign insertion point for this squad\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_status_filter,
	0,
	0x0073D7E0, // $TODO write the function chuckle nuts
	"filters the set of network status to display\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_sim_reset,
	0,
	0x0073DBD0, // $TODO write the function chuckle nuts
	"network simulation: resets the simulation state\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_sim_spike_now,
	0,
	0x0073DE50, // $TODO write the function chuckle nuts
	"network simulation: starts a latency spike immediately\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_sim_dropspike_now,
	0,
	0x0073DFD0, // $TODO write the function chuckle nuts
	"network simulation: starts a packet loss spike immediately\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	net_test_ping,
	0,
	network_test_ping, // 0x0073E400,
	"network test: sends a ping\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_channel_delete,
	0,
	0x0073E4E0, // $TODO write the function chuckle nuts
	"network test: deletes all channels\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_delegate_host,
	0,
	0x0073E710, // $TODO write the function chuckle nuts
	"network test: delegate host to the specified player\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_delegate_leader,
	0,
	0x0073EA50, // $TODO write the function chuckle nuts
	"network test: delegate leadership to the specified player\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_test_map_name,
	0,
	network_test_set_map_name, // 0x0073EC50,
	"network test: sets the name of the scenario to play\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_campaign_difficulty,
	0,
	0x0073EE70, // $TODO write the function chuckle nuts
	"network test: sets the difficulty of the campaign game to play\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	net_test_reset_objects,
	0,
	network_test_reset_objects, // 0x0073F160,
	"network test: resets all objects on the map\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_fatal_error,
	0,
	0x0073F3B0, // $TODO write the function chuckle nuts
	"creates a fatal simulation error\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_set_machine_name,
	0,
	0x0073F5B0, // $TODO write the function chuckle nuts
	"sets the nickname of your xbox\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	events_enabled,
	0,
	0x0073F830, // $TODO write the function chuckle nuts
	"enable/disable all events\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	events_disable_suppression,
	0,
	0x0073FA40, // $TODO write the function chuckle nuts
	"enable/disable event suppression\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
void event_set_display_level_global(int32 display_level)
{
	printf("");
}
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	event_global_display_category,
	0,
	event_set_display_level_global, // 0x0073FCB0
	"sets the global event display level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_event
);
void event_set_log_level_global(int32 log_level)
{
	printf("");
}
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	event_global_log_category,
	0,
	event_set_log_level_global, // 0x0073FEB0
	"sets the global evetn log level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	event_global_remote_log_category,
	0,
	0x007401A0, // $TODO write the function chuckle nuts
	"sets the global remote evetn log level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_display_category,
	0,
	0x00740360, // $TODO write the function chuckle nuts
	"sets the display level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_force_display_category,
	0,
	0x007405F0, // $TODO write the function chuckle nuts
	"sets the forced display level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_log_category,
	0,
	0x00740860, // $TODO write the function chuckle nuts
	"sets the log level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_remote_log_category,
	0,
	0x00740A90, // $TODO write the function chuckle nuts
	"sets the remote log level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_debugger_break_category,
	0,
	0x00740C40, // $TODO write the function chuckle nuts
	"sets the debugger break level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	event_halt_category,
	0,
	0x00740E80, // $TODO write the function chuckle nuts
	"sets the halt (assert/crash) level for a named category of network events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_enum_event
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	event_list_categories,
	0,
	0x00741240, // $TODO write the function chuckle nuts
	"lists all categories that exist under a particular category string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	events_suppress_console_display,
	0,
	0x007413A0, // $TODO write the function chuckle nuts
	"disables console display for events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	play_bink_movie,
	0,
	0x00741630, // $TODO write the function chuckle nuts
	"um...\r\nNETWORK SAFE: safe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	play_bink_movie_from_tag,
	0,
	0x00741980, // $TODO write the function chuckle nuts
	"play a bink movie from a tag\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_bink_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	play_credits_skip_to_menu,
	0,
	0x00741D10, // $TODO write the function chuckle nuts
	"plays credits movie, pressing any button will stop credits and launch main menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	bink_time,
	0,
	0x00741F20, // $TODO write the function chuckle nuts
	"uh...\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	set_global_doppler_factor,
	0,
	0x00742240, // $TODO write the function chuckle nuts
	"new doppler factor: <real>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	set_global_mixbin_headroom,
	0,
	0x00742610, // $TODO write the function chuckle nuts
	"blah\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	debug_sound_environment_source_parameter,
	0,
	0x00742A10, // $TODO write the function chuckle nuts
	"blah\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	data_mine_set_mission_segment,
	0,
	0x00742D60, // $TODO write the function chuckle nuts
	"sets the mission segment for single player data mine events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	data_mine_insert,
	0,
	0x00742FA0, // $TODO write the function chuckle nuts
	"insert text and camera position in the data mine\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	data_mine_upload,
	0,
	0x00743470, // $TODO write the function chuckle nuts
	"upload all data mining data files to debug server\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	data_mine_enable,
	0,
	0x00743610, // $TODO write the function chuckle nuts
	"enable/disable data mining\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	data_mine_flush,
	0,
	0x00743980, // $TODO write the function chuckle nuts
	"flush any pending data mine data to disk\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	data_mine_debug_menu_setting,
	0,
	0x00743C70, // $TODO write the function chuckle nuts
	"close debug menu and set user experience setting to string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	data_mine_open_debug_menu,
	0,
	0x00743FD0, // $TODO write the function chuckle nuts
	"open debug menu\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	data_mine_set_display_mission_segment,
	0,
	0x007442B0, // $TODO write the function chuckle nuts
	"toggle display of data mine mission segment\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown4F4,
	0,
	0x00744420, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	test_memory_allocators,
	0,
	0x007447A0, // $TODO write the function chuckle nuts
	"performs tests on different memory allocators and saves the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	test_memory_allocators,
	0,
	0x00744970, // $TODO write the function chuckle nuts
	"performs tests on different memory allocators and saves the results\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_long_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	display_video_standard,
	0,
	0x00744B50, // $TODO write the function chuckle nuts
	"displays the video standard the game is in\r\nNETWORK SAFE: safe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_xcr_monkey_enable,
	0,
	0x00744E90, // $TODO write the function chuckle nuts
	"enable/disable controller monkeys for all in game players\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_show_guide_status,
	0,
	0x007450F0, // $TODO write the function chuckle nuts
	"prints the guide status as automation events.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_show_users_xuids,
	0,
	0x00745350, // $TODO write the function chuckle nuts
	"prints the signed in users' xuids as automation events.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	test_show_are_users_friends,
	0,
	0x007454A0, // $TODO write the function chuckle nuts
	"prints whether the current controller and specified xuid are friends as automation events\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	test_invite_friend,
	0,
	0x00745720, // $TODO write the function chuckle nuts
	"shows the guide send friend invite ui from the current controller to the specified xuid\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_get_squad_session_id,
	0,
	0x00745B20, // $TODO write the function chuckle nuts
	"get the current squad session id\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_auto_get_screens,
	0,
	0x00745C70, // $TODO write the function chuckle nuts
	"<arbitrary int> list the current gui screens\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	test_auto_get_screen_widgets,
	0,
	0x00745E80, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> lists widgets in a screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	test_auto_screen_get_datasources,
	0,
	0x00746160, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> lists screen datasources\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	test_auto_screen_get_data_columns,
	0,
	0x00746300, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <datasource string id> lists columns that make up this datasource\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	test_auto_screen_get_data,
	0,
	0x00746590, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <datasource string id> <datasource column string id> lists data from a datasource\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	test_auto_screen_invoke_list_item_by_name,
	0,
	0x00746810, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <list string id> <datasource column string id> <column value string id> triggers the list item who's column's value matches the specified one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	test_auto_screen_invoke_list_item_by_text,
	0,
	0x00746A20, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <list string id> <datasource column string id> <column value string id> triggers the list item who's column's value matches the specified one\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_string_id, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	test_auto_screen_invoke_list_item_by_handle,
	0,
	0x00746CB0, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <list string id> <element handle> triggers the list item specified by the element handle\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string_id, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	test_auto_screen_send_button_press,
	0,
	0x00746EC0, // $TODO write the function chuckle nuts
	"<arbitrary int> <screen index> <button name> sends the button press to the screen\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	test_download_storage_file,
	0,
	test_download_storage_file, // 0x00747140,
	"<url> <filename> downloads a file from LSP to the client\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_game_results_save_to_file,
	0,
	0x00747330, // $TODO write the function chuckle nuts
	"<filename> save game results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_game_results_load_from_file,
	0,
	0x00747600, // $TODO write the function chuckle nuts
	"<filename> load game results into memory for use in the carnage report.  the format may change build to build causing this to fail or weird results to come up.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_roster_save_to_file,
	0,
	0x007477E0, // $TODO write the function chuckle nuts
	"<filename> save roster to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_roster_load_from_file,
	0,
	0x007479B0, // $TODO write the function chuckle nuts
	"<filename> load roster into memory.  the format may change build to build causing this to fail or weird results to come up.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	test_fragment_utility_drive,
	0,
	0x00747D90, // $TODO write the function chuckle nuts
	"<file_count> <growth_repetitions> <growth_size> writes files of varying sizes to the utiltiy drive in an effort to fragment it really badly\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_metagame_results_save_to_file,
	0,
	0x00747F50, // $TODO write the function chuckle nuts
	"<filename> save metagame results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	test_metagame_results_load_from_file,
	0,
	0x007480F0, // $TODO write the function chuckle nuts
	"<filename> save metagame results to a file.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	clear_webcache,
	0,
	0x00748380, // $TODO write the function chuckle nuts
	"clears the webcache\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	force_manifest_redownload,
	0,
	0x007485E0, // $TODO write the function chuckle nuts
	"redownload the online storage manifest\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	online_files_retry,
	0,
	0x0072F5A0, // $TODO write the function chuckle nuts
	"tell the upload queue to start up again after a failure\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	online_files_upload,
	0,
	0x0072F7C0, // $TODO write the function chuckle nuts
	"<filename> <slot_index> enqueues the specified filename for upload\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	online_files_throttle_bandwidth,
	0,
	0x0072FA70, // $TODO write the function chuckle nuts
	"<bytes per second> sets the maximum bandwidth that online_files can use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	online_marketplace_refresh,
	0,
	0x0072FC40, // $TODO write the function chuckle nuts
	"tells the marketplace manager to take another look at signed in users\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	webstats_disable,
	0,
	0x0072FED0, // $TODO write the function chuckle nuts
	"disables webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	webstats_test_submit,
	0,
	0x00730200, // $TODO write the function chuckle nuts
	"submits a string to webstats upload\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	webstats_test_submit_multiplayer,
	0,
	0x007304C0, // $TODO write the function chuckle nuts
	"submits a test multiplayer blob to webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	webstats_test_submit_campaign,
	0,
	0x00730660, // $TODO write the function chuckle nuts
	"submits a test campaign blob to webstats\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	webstats_throttle_bandwidth,
	0,
	0x00730920, // $TODO write the function chuckle nuts
	"<bytes per second> sets the maximum bandwidth that webstats can use\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	netdebug_prefer_internet,
	0,
	0x00730BB0, // $TODO write the function chuckle nuts
	"hints to the netdebug code that that we're on the internet\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	flag_new,
	0,
	0x00730DE0, // $TODO write the function chuckle nuts
	"<name> <description>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	flag_new_at_look,
	0,
	0x00730FF0, // $TODO write the function chuckle nuts
	"<name> <description>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	flags_clear,
	0,
	0x00731260, // $TODO write the function chuckle nuts
	"erases all comment flags when not in editor (sapien)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bug_now,
	0,
	0x00731570, // $TODO write the function chuckle nuts
	"uploads files for bug <name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bug_now_lite,
	0,
	0x00731740, // $TODO write the function chuckle nuts
	"uploads files (w/ no minidump) for bug <name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bug_now_auto,
	0,
	0x00731980, // $TODO write the function chuckle nuts
	"uploads bug files, auto named <include_minidump>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_object_list,
	object_list_children,
	0,
	0x00731C40, // $TODO write the function chuckle nuts
	"returns list of child objects by definition.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_object_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	voice_set_outgoing_channel_count,
	0,
	0x00732080, // $TODO write the function chuckle nuts
	"sets the outgoing channel count <in-game> <out-of-game>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	voice_set_voice_repeater_peer_index,
	0,
	0x007322D0, // $TODO write the function chuckle nuts
	"sets the repeater peer index\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	voice_set_mute,
	0,
	0x00732560, // $TODO write the function chuckle nuts
	"mute a player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_hopper,
	0,
	0x00732790, // $TODO write the function chuckle nuts
	"<user index> clears the hopper stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_global_arbitrated,
	0,
	0x00732940, // $TODO write the function chuckle nuts
	"<user index> clears the global arbitrated stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_global_unarbitrated,
	0,
	0x00732C30, // $TODO write the function chuckle nuts
	"<user index> clears the global unarbitrated stats for the user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_leaderboard_refresh,
	0,
	0x00732E80, // $TODO write the function chuckle nuts
	"<user index> refreshes stats for everybody in the session\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dump_loaded_tags,
	0,
	0x00733110, // $TODO write the function chuckle nuts
	"dump a list of open tags to <map_name>_tags.txt\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_long_integer,
	interpolator_start,
	0,
	0x007332C0, // $TODO write the function chuckle nuts
	"<name> <final value> <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_long_integer,
	interpolator_start_smooth,
	0,
	0x00733850, // $TODO write the function chuckle nuts
	"<name> <final value> <time>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	interpolator_stop,
	0,
	0x00733D20, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	interpolator_restart,
	0,
	0x00734210, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	interpolator_is_active,
	0,
	0x00734540, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	interpolator_is_finished,
	0,
	0x00734AC0, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_long_integer,
	interpolator_set_current_value,
	0,
	0x00734F50, // $TODO write the function chuckle nuts
	"<name> <current value>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	interpolator_get_current_value,
	0,
	0x00735470, // $TODO write the function chuckle nuts
	"<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	interpolator_get_start_value,
	0,
	0x007357F0, // $TODO write the function chuckle nuts
	"<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	interpolator_get_final_value,
	0,
	0x00735D30, // $TODO write the function chuckle nuts
	"<name> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	interpolator_get_current_phase,
	0,
	0x007361A0, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	interpolator_get_current_time_fraction,
	0,
	0x007366A0, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	interpolator_get_start_time,
	0,
	0x00736A40, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_real,
	interpolator_get_final_time,
	0,
	0x00736EC0, // $TODO write the function chuckle nuts
	"<name>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	interpolator_evaluate_at,
	0,
	0x007372F0, // $TODO write the function chuckle nuts
	"<name> <value in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	interpolator_evaluate_at_time_fraction,
	0,
	0x007378A0, // $TODO write the function chuckle nuts
	"<name> <time fraction in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	interpolator_evaluate_at_time,
	0,
	0x00737CA0, // $TODO write the function chuckle nuts
	"<name> <time in> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_real,
	interpolator_evaluate_at_time_delta,
	0,
	0x00738300, // $TODO write the function chuckle nuts
	"<name> <time delta> <use function ON/OFF>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_string_id, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	interpolator_stop_all,
	0,
	0x007388A0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	interpolator_restart_all,
	0,
	0x00738A60, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	interpolator_flip,
	0,
	0x00738C20, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	set_pc_runtime_language,
	0,
	0x00738EA0, // $TODO write the function chuckle nuts
	"sets the pc language from a string\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	animation_cache_stats_reset,
	0,
	0x007391F0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	cinematic_clone_players_weapon,
	0,
	0x00739390, // $TODO write the function chuckle nuts
	"clone the first player's most reasonable weapon and attach it to the specified object's marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_move_attached_objects,
	0,
	0x00739810, // $TODO write the function chuckle nuts
	"move the object's children attached to the first marker to the second marker\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_object, _hs_type_string_id, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	vehicle_enable_ghost_effects,
	0,
	0x00739CB0, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	set_global_sound_environment,
	0,
	0x00739FD0, // $TODO write the function chuckle nuts
	"this is your brain on drugs\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	6, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	reset_dsp_image,
	0,
	0x0073A6A0, // $TODO write the function chuckle nuts
	"if we ever need to use this someone is getting fired\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	game_save_cinematic_skip,
	0,
	game_save_cinematic_skip, // 0x0073A840,
	"don't use this, except in one place.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_outro_start,
	0,
	0x0073AAA0, // $TODO write the function chuckle nuts
	"flag this cutscene as an outro cutscene\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_enable_ambience_details,
	0,
	0x0073AD20, // $TODO write the function chuckle nuts
	"enable/disable ambience details in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override,
	0,
	0x0073B0C0, // $TODO write the function chuckle nuts
	"enable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_reset,
	0,
	0x0073B280, // $TODO write the function chuckle nuts
	"reset\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_blur_amount,
	0,
	0x0073B450, // $TODO write the function chuckle nuts
	"blur mount\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_threshold,
	0,
	0x0073B7A0, // $TODO write the function chuckle nuts
	"threshold\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_brightness,
	0,
	0x0073B9B0, // $TODO write the function chuckle nuts
	"brightness\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_box_factor,
	0,
	0x0073BBA0, // $TODO write the function chuckle nuts
	"box factor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_max_factor,
	0,
	0x0073BE10, // $TODO write the function chuckle nuts
	"max factor\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_silver_bullet,
	0,
	0x0073C0C0, // $TODO write the function chuckle nuts
	"silver bullet\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_only,
	0,
	0x0073C2D0, // $TODO write the function chuckle nuts
	"only\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_high_res,
	0,
	0x0073C510, // $TODO write the function chuckle nuts
	"high res\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_brightness_alpha,
	0,
	0x0073C6B0, // $TODO write the function chuckle nuts
	"brightness alpha\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	rasterizer_bloom_override_max_factor_alpha,
	0,
	0x0073CAA0, // $TODO write the function chuckle nuts
	"max factor alpha\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cache_block_for_one_frame,
	0,
	0x0073CCC0, // $TODO write the function chuckle nuts
	"call this to force texture and geometry cache to block until satiated\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	sound_suppress_ambience_update_on_revert,
	0,
	0x0073CE30, // $TODO write the function chuckle nuts
	"call this when transitioning between two cinematics so ambience won't fade in between the skips\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_autoexposure_enable,
	0,
	0x0073D160, // $TODO write the function chuckle nuts
	"toggle autoexposure\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	render_exposure_full,
	0,
	0x0073D550, // $TODO write the function chuckle nuts
	"animate exposure from <initial> to <final> stops using <initial> to <final> opacity in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	5, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	render_exposure_fade_in,
	0,
	0x0073D930, // $TODO write the function chuckle nuts
	"fade to animated exposure <stops> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_exposure_fade_out,
	0,
	0x0073DE10, // $TODO write the function chuckle nuts
	"fade to default exposure settings in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	render_exposure,
	0,
	0x0073E370, // $TODO write the function chuckle nuts
	"fade to animated exposure <stops> in <seconds>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_autoexposure_instant,
	0,
	0x0073E5E0, // $TODO write the function chuckle nuts
	"instantly adopt auto exposure values in the next N frames\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_exposure_set_environment_darken,
	0,
	0x0073E930, // $TODO write the function chuckle nuts
	"set environmental darkening\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_depth_of_field_enable,
	0,
	0x0073ED30, // $TODO write the function chuckle nuts
	"turns depth of field on and off\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	render_depth_of_field,
	0,
	0x0073F000, // $TODO write the function chuckle nuts
	"animate depth of field, in focus from <near> to <far>, with depth of field <depth>, in <ticks>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_dof_focus_depth,
	0,
	0x0073F550, // $TODO write the function chuckle nuts
	"interpolate depth of field to <depth>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	render_dof_blur_animate,
	0,
	0x0073F890, // $TODO write the function chuckle nuts
	"interpolate depth of field maximum blur to <blur> in <ticks>\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	render_debug_video_mode,
	0,
	0x0073FC90, // $TODO write the function chuckle nuts
	"displays the current video mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_lightmap_shadow_disable,
	0,
	0x0073FE90, // $TODO write the function chuckle nuts
	"turn off lightmap shadow in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_lightmap_shadow_enable,
	0,
	0x00740180, // $TODO write the function chuckle nuts
	"turn on lightmap shadow in cinematics\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	predict_animation,
	0,
	0x00740320, // $TODO write the function chuckle nuts
	"begin loading an animation for the graph provided\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_animation_graph, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_object_list,
	mp_players_by_team,
	0,
	game_engine_players_by_team, // 0x00740760,
	"given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_long_integer,
	mp_active_player_count_by_team,
	0,
	game_engine_active_player_count_by_team, // 0x00740B20,
	"given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	deterministic_end_game,
	0,
	game_engine_finish_game, // 0x00740F80,
	"end game deterministically, by inserting a simulation queue event\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	mp_game_won,
	0,
	game_engine_game_won, // 0x007412A0,
	"given a team index, declares the game a victory for that team and a loss for all others\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	mp_respawn_override_timers,
	0,
	game_engine_respawn_override_timers, // 0x007415A0,
	"causes all players on the specified team waiting to respawn (due to timer) to respawn immediately\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	mp_ai_allegiance,
	0,
	game_engine_ai_scripting_allegiance, // 0x007418E0,
	"causes an allegiance to be formed between an AI squad team and a multiplayer team\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_enum_team, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	mp_allegiance,
	0,
	game_engine_mp_team_allegiance, // 0x00741C10,
	"create an allegiance between two multiplayer teams\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_enum_mp_team, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	mp_round_started,
	0,
	0x00741F60, // $TODO write the function chuckle nuts
	"used to determine if the mp game is in the active-round phase or not\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	give_medal,
	0,
	0x007423B0, // $TODO write the function chuckle nuts
	"awards a medal (0..58) to pad 1 due to pad 2 (if they're there)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	mp_scripts_reset,
	0,
	0x007426D0, // $TODO write the function chuckle nuts
	"on the host in a distributed game, resets the scripts (but not internal hs globals)\r\nNETWORK SAFE: Yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_ai_place,
	0,
	0x007429A0, // $TODO write the function chuckle nuts
	"places the given number of members of the specified squad as non-authoritative objects\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	mp_ai_place,
	0,
	0x00742DD0, // $TODO write the function chuckle nuts
	"places the given number of members of the specified squad as non-authoritative objects\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_ai_kill,
	0,
	0x00743090, // $TODO write the function chuckle nuts
	"instantly kills the specified non-authoritative encounter and/or squad.\r\nNETWORK SAFE: YEs",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_ai_kill_silent,
	0,
	0x00743580, // $TODO write the function chuckle nuts
	"instantly and silently (no animation or sound played) kills the specified non-authoritative encounter and/or squad.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_object_create,
	0,
	0x00743870, // $TODO write the function chuckle nuts
	"creates a non-authoratative object from the scenario.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_object_create_clone,
	0,
	0x00743C40, // $TODO write the function chuckle nuts
	"creates a non-authoratative object, potentially resulting in multiple objects if it already exists.\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_object_create_anew,
	0,
	0x00743FA0, // $TODO write the function chuckle nuts
	"creates a non-authoratative object, destroying it first if it already exists.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object_name
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	mp_object_destroy,
	0,
	0x00744370, // $TODO write the function chuckle nuts
	"destroys a non-authoratative object.\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_file_set_backend,
	0,
	0x00744780, // $TODO write the function chuckle nuts
	"override the tag file system backend\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	mp_object_belongs_to_team,
	0,
	game_engine_give_object_ownership_to_team, // 0x00744930,
	"causes specified object to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	mp_weapon_belongs_to_team,
	0,
	game_engine_give_weapon_ownership_to_team, // 0x00744DE0,
	"causes specified weapon to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_object, _hs_type_enum_mp_team
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	mp_debug_goal_object_boundary_geometry,
	0,
	debug_multiplayer_object_boundary_geometry, // 0x00745230,
	"toggle debug geometry for multiplayer goal objects\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	mp_dump_candy_monitor_state,
	0,
	0x00745400, // $TODO write the function chuckle nuts
	"dumps the current candy monitor state to a file on disk\r\nNETWORK SFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	set_camera_third_person,
	0,
	0x007455D0, // $TODO write the function chuckle nuts
	"<user_index> <third_person (yes/no)> sets user's camera perspective\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	get_camera_third_person,
	0,
	0x00745AE0, // $TODO write the function chuckle nuts
	"<user_index> gets user's camera perspective\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	determinism_debug_manager_enable_logging,
	0,
	0x00745E60, // $TODO write the function chuckle nuts
	"enable/disable determinism logging\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	determinism_debug_manager_set_trace_flags,
	0,
	0x00746140, // $TODO write the function chuckle nuts
	"set which determinism debugging traces should be turned on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	determinism_debug_manager_enable_game_state_checksum,
	0,
	0x007462E0, // $TODO write the function chuckle nuts
	"enable/disable determinism checkpoints\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	determinism_debug_manager_enable_trace,
	0,
	0x00746510, // $TODO write the function chuckle nuts
	"enable determinism tracing\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	determinism_debug_manager_set_consumer_sample_level,
	0,
	0x007467F0, // $TODO write the function chuckle nuts
	"sets sample level for a consumer\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown584,
	0,
	0x007469C0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	saved_film_play,
	0,
	saved_film_manager_play_hs, // 0x00746CF0,
	"plays a saved film for given controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_controller, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	saved_film_play_last,
	0,
	saved_film_manager_play_last_hs, // 0x00746F40,
	"play back last recorded saved film for given controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_disable_version_checking,
	0,
	saved_film_manager_disable_version_checking, // 0x007471F0,
	"disable saved film version checking\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_toggle_debug_saving,
	0,
	saved_film_manager_toggle_automatic_debug_saving, // 0x00747370,
	"toggle saving of last film to the debug output directory automatically after each game level\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_films_delete_on_level_load,
	0,
	saved_film_manager_delete_on_level_load, // 0x007476B0,
	"toggle deletion last saved film after loading a new game level\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_films_show_timestamp,
	0,
	saved_film_manager_show_timestamp, // 0x00747870,
	"toggle showing timestamp for saved film playback\r\nNETWORK SAFE: Unkown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_manager_should_record_film_default,
	0,
	saved_film_manager_should_record_film_default_set, // 0x00747AF0
	"Toggle recording of replay in single player game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_boolean,
	mover_set_program,
	0,
	0x00747E40, // $TODO write the function chuckle nuts
	"Set the motor program of the given mover object\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	determinism_log_compare_log_files,
	0,
	0x00748220, // $TODO write the function chuckle nuts
	"comares determinism log files\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	floating_point_exceptions_enable,
	0,
	0x007484C0, // $TODO write the function chuckle nuts
	"enables/disables floating point exceptions\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	determinism_debug_manager_enable_log_file_comparision_on_oos,
	0,
	0x007486B0, // $TODO write the function chuckle nuts
	"enables/disables determinism log file comparision on oos\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	event_logs_snapshot,
	0,
	0x0072F5E0, // $TODO write the function chuckle nuts
	"copy error files to the error snapshot folder\r\nNETWORK SAFE: No",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_reload_force,
	0,
	0x0072F840, // $TODO write the function chuckle nuts
	"rather than force a painful xsync to test xsync, allow me to fake a tag_reload\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_unload_force,
	0,
	0x0072FB70, // $TODO write the function chuckle nuts
	"call tag_unload\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_load_force,
	0,
	0x0072FDB0, // $TODO write the function chuckle nuts
	"call tag_load\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	predict_bink_movie,
	0,
	0x0072FF80, // $TODO write the function chuckle nuts
	"um2...\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	predict_bink_movie_from_tag,
	0,
	0x00730460, // $TODO write the function chuckle nuts
	"um2...\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_bink_definition
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	profiler_dump_history,
	0,
	0x00730710, // $TODO write the function chuckle nuts
	"dump profile history.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	camera_set_mode,
	0,
	director_set_camera_mode, // 0x00730AA0,
	"<user_index> <mode_index> sets user's camera perspective\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set_flying_cam_at_point,
	0,
	0x00730DA0, // $TODO write the function chuckle nuts
	"<user_index> <camera_point> sets a flying camera perspective\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_long_integer, _hs_type_cutscene_camera_point
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	camera_set_target,
	0,
	0x00731190, // $TODO write the function chuckle nuts
	"<user_index> <object_index> sets the current camera's target\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_long_integer, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	game_coop_player_count,
	0,
	0x00731660, // $TODO write the function chuckle nuts
	"returns the number of coop players active in game\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	player_force_mode,
	0,
	player_override_desired_mode, // 0x007318A0,
	"force your will upon the player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	profiler_output_pulse,
	0,
	0x00731AF0, // $TODO write the function chuckle nuts
	"dump profile stats to telnet console.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	string_id_name,
	0,
	0x00731D70, // $TODO write the function chuckle nuts
	"prints the name of the string id to the console\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	find,
	0,
	0x00731F30, // $TODO write the function chuckle nuts
	"prints any hs command matching the string passed in\r\nNETWORK SAFE: of course you idiot",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	add_recycling_volume,
	0,
	0x00732140, // $TODO write the function chuckle nuts
	"removes garbage in specified trigger volume to a max count over n seconds\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_trigger_volume, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_resources_set_per_frame_publish,
	0,
	0x00732670, // $TODO write the function chuckle nuts
	"turns on/off per frame resource publish (stress testing)\r\nNETWORK SAFE: If not, Ooops",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	object_recycling_clear_history,
	0,
	0x00732840, // $TODO write the function chuckle nuts
	"clear object recycling history\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dump_cinematics_script,
	0,
	0x00732AB0, // $TODO write the function chuckle nuts
	"dump the cinematic script to cinematic_dump.txt",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	global_preferences_clear,
	0,
	0x00732D00, // $TODO write the function chuckle nuts
	"clears all game preference information\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	network_storage_set_storage_subdirectory,
	0,
	0x00732F80, // $TODO write the function chuckle nuts
	"<subdirectory> The default is the build number or 'untracked'\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	network_storage_set_storage_user,
	0,
	0x00733230, // $TODO write the function chuckle nuts
	"<user> Use the network file stored on untracked\\<user>.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	status_line_dump,
	0,
	0x007333E0, // $TODO write the function chuckle nuts
	"dumps active status_lines to system_log.txt\r\nNETWORK SAFE: I hope so",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	game_tick_get,
	0,
	0x00733670, // $TODO write the function chuckle nuts
	"returns current game tick\r\nNETWORK SAFE: only if game_time_get is",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	loop_it,
	0,
	hs_looper_setup, // 0x007339E0,
	"runs the expression at most once per tick\r\nNETWORK SAFE: just assume it isn't",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	loop_clear,
	0,
	hs_looper_clear, // 0x00733B20,
	"runs the expression at most once per tick\r\nNETWORK SAFE: just assume it isn't",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	status_lines_enable,
	0,
	0x00733E90, // $TODO write the function chuckle nuts
	"enables status_lines that were initialized with identifiers that match the input substring\r\nNETWORK SAFE: I hope so",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	status_lines_disable,
	0,
	0x00734180, // $TODO write the function chuckle nuts
	"disables status_lines that were initialized with identifiers that match the input substring\r\nNETWORK SAFE: I hope so",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	on_target_platform,
	0,
	0x007342F0, // $TODO write the function chuckle nuts
	"returns true if the game thinks it's running on the target platform\r\nNETWORK SAFE: no",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profile_activate,
	0,
	0x007345C0, // $TODO write the function chuckle nuts
	"activates profile sections based on a substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profile_deactivate,
	0,
	0x00734930, // $TODO write the function chuckle nuts
	"deactivates profile sections based on a substring.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_game_set_player_standing,
	0,
	0x00734B30, // $TODO write the function chuckle nuts
	"set's a player's standing by gamertag\r\nNETWORK SAFE: host only",
	NULL,
	2, _hs_type_string, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_get_game_id,
	0,
	0x00734D50, // $TODO write the function chuckle nuts
	"prints the id of the game to the console\r\nNETWORK SAFE: if it's not, it wouldn't be very useful",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_matchmaking_hopper_list,
	0,
	0x00734FF0, // $TODO write the function chuckle nuts
	"prints the hopper list\r\nNETWORK SAFE: just assume it isn't",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_matchmaking_hopper_game_list,
	0,
	0x007352F0, // $TODO write the function chuckle nuts
	"prints games for the current hopper\r\nNETWORK SAFE: just assume it isn't",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_matchmaking_hopper_set_game,
	0,
	0x00735510, // $TODO write the function chuckle nuts
	"sets the game vairant for the current hopper\r\nNETWORK SAFE: just assume it isn't",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_set_playback_game_speed,
	0,
	saved_film_manager_set_playback_game_speed, // 0x00735750,
	"set the saved film playback speed",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_set_pending_playback_game_speed,
	0,
	saved_film_manager_set_pending_playback_game_speed, // 0x00735B20,
	"set the pending saved film playback speed",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	noguchis_mystery_tour,
	0,
	0x00735ED0, // $TODO write the function chuckle nuts
	"set the additional music directory to dump into the cache file",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	designer_zone_sync,
	0,
	0x00736030, // $TODO write the function chuckle nuts
	"synchronizes global objects with loaded designer zones\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	dump_designer_zone,
	0,
	0x00736320, // $TODO write the function chuckle nuts
	"dump interesting info about a designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_designer_zone
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	designer_zone_activate,
	0,
	0x007364E0, // $TODO write the function chuckle nuts
	"activate designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_designer_zone
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	designer_zone_deactivate,
	0,
	0x00736900, // $TODO write the function chuckle nuts
	"deactivates designer zone, deletes objects that were part of that designer zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_designer_zone
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_always_active,
	0,
	0x00736C20, // $TODO write the function chuckle nuts
	"sets/unsets the object always active flag to ignore pvs changes\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	saved_film_seek_to_film_tick,
	0,
	saved_film_manager_seek_to_film_tick_hs, // 0x00737080,
	"seeks to a film tick",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	tag_is_active,
	0,
	0x00737240, // $TODO write the function chuckle nuts
	"returns true/false if you can create this tag with the active zones\r\nNETWORK SAFE: only if the global zone state is network safe",
	NULL,
	1, _hs_type_any_tag_not_resolving
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_resources_enable_optional_caching,
	0,
	0x007375E0, // $TODO write the function chuckle nuts
	"turns on/off optional resource prediction\r\nNETWORK SAFE: If not, Oooops",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dump_active_resources,
	0,
	0x007379F0, // $TODO write the function chuckle nuts
	"blah",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_persistent,
	0,
	0x00737B30, // $TODO write the function chuckle nuts
	"<object_index> <true/false> prevent this object from being garbage collected\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	display_zone_size_estimates,
	0,
	0x00737FF0, // $TODO write the function chuckle nuts
	"display zone size estimates HUD\r\nNETWORK SAFE: YES",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	report_zone_size_estimates,
	0,
	0x00738260, // $TODO write the function chuckle nuts
	"dump zone size estimates to design report log\r\nNETWORK SAFE: YES",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_disconnect_squad,
	0,
	0x007383E0, // $TODO write the function chuckle nuts
	"disconnects the squad session\r\nNETWORK SAFE: probably not",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_disconnect_group,
	0,
	0x007387E0, // $TODO write the function chuckle nuts
	"disconnects the group session\r\nNETWORK SAFE: probably not",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_clear_squad_session_parameter,
	0,
	0x007388F0, // $TODO write the function chuckle nuts
	"clears a squad session parameter\r\nNETWORK SAFE: probably not",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_test_clear_group_session_parameter,
	0,
	0x00738BC0, // $TODO write the function chuckle nuts
	"clears a group session parameter\r\nNETWORK SAFE: probably not",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_test_life_cycle_pause,
	0,
	0x00738E00, // $TODO write the function chuckle nuts
	"pauses the life cycle in the specified state\r\nNETWORK SAFE: probably not",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_test_life_cycle_display_states,
	0,
	0x00739020, // $TODO write the function chuckle nuts
	"display the life cycle states\r\nNETWORK SAFE: probably not",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	overlapped_display_task_descriptions,
	0,
	overlapped_task_display_task_descriptions, // 0x007392F0,
	"displays all tasks\r\nNETWORK SAFE: lol",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	overlapped_task_inject_error,
	0,
	overlapped_task_inject_error, // 0x007395A0,
	"inject error for tasks\r\nNETWORK SAFE: lol",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_hopper_all_users,
	0,
	0x007397B0, // $TODO write the function chuckle nuts
	"clears the hopper stats for the all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_global_arbitrated_all_users,
	0,
	0x007399E0, // $TODO write the function chuckle nuts
	"clears the global arbitrated stats for all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	net_leaderboard_clear_global_unarbitrated_all_users,
	0,
	0x00739C20, // $TODO write the function chuckle nuts
	"clears the global unarbitrated stats for all user\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_sapien_crash,
	0,
	0x00739F60, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	output_window_add_category,
	0,
	0x0073A080, // $TODO write the function chuckle nuts
	"Add an event category to be watched in the output window.",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	output_window_remove_category,
	0,
	0x0073A470, // $TODO write the function chuckle nuts
	"Remove an event category from being watched in the output window.",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	output_window_list_categories,
	0,
	0x0073A680, // $TODO write the function chuckle nuts
	"List all the event categories being watched in the output window.",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	decorators_split,
	0,
	0x0073A820, // $TODO write the function chuckle nuts
	"duplicates decorator set <index> and moves <percentage> of the decorators into it\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bandwidth_profiler_enable,
	0,
	0x0073AA80, // $TODO write the function chuckle nuts
	"enables bandwidth profiler\r\nNETWORK SAFE: probably not",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	bandwidth_profiler_set_context,
	0,
	0x0073AD00, // $TODO write the function chuckle nuts
	"sets bandwidth profiler context\r\nNETWORK SAFE: probably not",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	overlapped_task_pause,
	0,
	overlapped_task_pause, // 0x0073AFB0,
	"pause tasks\r\nNETWORK SAFE: lol",
	NULL,
	2, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	net_banhammer_set_controller_cheat_flags,
	0,
	0x0073B170, // $TODO write the function chuckle nuts
	"sets cheat flags\r\nNETWORK SAFE: yes",
	NULL,
	3, _hs_type_enum_controller, _hs_type_string, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	net_banhammer_set_controller_ban_flags,
	0,
	0x0073B320, // $TODO write the function chuckle nuts
	"sets ban flags\r\nNETWORK SAFE: yes",
	NULL,
	4, _hs_type_enum_controller, _hs_type_string, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_banhammer_dump_strings,
	0,
	0x0073B6A0, // $TODO write the function chuckle nuts
	"display cheat/ban strings\r\nNETWORK SAFE: yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	net_banhammer_dump_repeated_play_list,
	0,
	0x0073B8F0, // $TODO write the function chuckle nuts
	"display repeated play lists\r\nNETWORK SAFE: yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 12,
	_hs_type_void,
	net_leaderboard_set_user_stats,
	0,
	0x0073BA80, // $TODO write the function chuckle nuts
	"slams user leaderboard values locally\r\nNETWORK SAFE: no",
	NULL,
	6, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 10,
	_hs_type_void,
	net_leaderboard_set_user_game_stats,
	0,
	0x0073BD80, // $TODO write the function chuckle nuts
	"slams more user stats\r\nNETWORK SAFE: no",
	NULL,
	5, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_build_map_variant,
	0,
	network_build_map_variant, // 0x0073BFB0,
	"writes the current map variant to a file\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	net_verify_map_variant,
	0,
	network_verify_packed_map_variant_file, // 0x0073C250,
	"verifies the contents of a packed map variant file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	async_set_work_delay_milliseconds,
	0,
	0x0073C420, // $TODO write the function chuckle nuts
	"set an artificial delay time for the performance of any asynchronous task work\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	sound_looping_start_with_effect,
	0,
	0x0073C610, // $TODO write the function chuckle nuts
	"plays a looping sound from the specified source object (or \"none\"), with the specified scale and playback effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_looping_sound, _hs_type_object, _hs_type_real, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_resources_set_demand_throttle_to_io,
	0,
	0x0073CBB0, // $TODO write the function chuckle nuts
	"turns on/off demand throttling based on io\r\nNETWORK SAFE: If not, Ooops",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	set_performance_throttle,
	0,
	0x0073CD70, // $TODO write the function chuckle nuts
	"sets a default value for a performance throttle for a given number of players",
	NULL,
	3, _hs_type_string, _hs_type_long_integer, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	global_preferences_quality_set_all,
	0,
	0x0073D1F0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_real,
	get_performance_throttle,
	0,
	0x0073D5E0, // $TODO write the function chuckle nuts
	"gets the value of a performance throttle for a given number of players",
	NULL,
	2, _hs_type_string, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown5E4,
	0,
	0x0073DAA0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_zone_activate,
	0,
	0x0073DD80, // $TODO write the function chuckle nuts
	"activates scenario cinematic zone (automatically generated script function!)",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_zone_deactivate,
	0,
	0x0073DFA0, // $TODO write the function chuckle nuts
	"deactivates scenario cinematic zone (automatically generated script function!)",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_zone_activate_from_editor,
	0,
	0x0073E460, // $TODO write the function chuckle nuts
	"activates cinematic zone from tag (automatically generated script function!)",
	NULL,
	1, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_zone_deactivate_from_editor,
	0,
	0x0073E660, // $TODO write the function chuckle nuts
	"deactivates cinematic zone from tag (automatically generated script function!)",
	NULL,
	1, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	tiling_current,
	0,
	0x0073E8E0, // $TODO write the function chuckle nuts
	"returns the actual tiling configuration (could have been overridden from the requested configuration)",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_limit_lipsync_to_mouth_only,
	0,
	0x0073EB90, // $TODO write the function chuckle nuts
	"cinematic lipsync means you can't use all the facial bones, just the mouth\r\nNETWORK SAFE: No",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dump_active_zone_tags,
	0,
	0x0073EF00, // $TODO write the function chuckle nuts
	"blah",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	calculate_tag_prediction,
	0,
	0x0073F1C0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_any_tag
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_resources_enable_fast_prediction,
	0,
	0x0073F4F0, // $TODO write the function chuckle nuts
	"turns on/off faster prediction, in case I fucked up something\r\nNETWORK SAFE: If not, Ooops",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_boolean,
	unit_start_first_person_custom_animation,
	0,
	0x0073F690, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a unit (puts away weapon if last parameter is TRUE)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_animation_graph, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_is_playing_custom_first_person_animation,
	0,
	0x0073FC50, // $TODO write the function chuckle nuts
	"returns TRUE if the given unit is still playing a custom animation in first person\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_stop_first_person_custom_animation,
	0,
	0x00740130, // $TODO write the function chuckle nuts
	"ends any custom first person animation running on the unit\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	prepare_to_switch_to_zone_set,
	0,
	0x007403C0, // $TODO write the function chuckle nuts
	"prepare for switch to zone set\r\nNETWORK SAFE: Yes",
	NULL,
	1, _hs_type_zone_set
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_zone_activate_for_debugging,
	0,
	0x00740790, // $TODO write the function chuckle nuts
	"activates scenario cinematic zone for debugging (automatically generated script function!)",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_play_random_ping,
	0,
	0x00740AB0, // $TODO write the function chuckle nuts
	"plays a random ping on the unit",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_control_fade_out_all_input,
	0,
	0x00740DA0, // $TODO write the function chuckle nuts
	"fade out all play control over x seconds",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_control_fade_in_all_input,
	0,
	0x00741260, // $TODO write the function chuckle nuts
	"fade in all play control over x seconds",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_control_fade_out_all_input,
	0,
	0x00741560, // $TODO write the function chuckle nuts
	"fade out all play control for player over x seconds",
	NULL,
	2, _hs_type_unit, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unit_control_fade_in_all_input,
	0,
	0x00741920, // $TODO write the function chuckle nuts
	"fade in all play control for player over x seconds",
	NULL,
	2, _hs_type_unit, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	player_control_lock_gaze,
	0,
	0x00741CD0, // $TODO write the function chuckle nuts
	"player turn to face point with max velocity degrees/second",
	NULL,
	3, _hs_type_unit, _hs_type_point_ref, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	player_control_unlock_gaze,
	0,
	0x007421A0, // $TODO write the function chuckle nuts
	"unlock player from any gaze point",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_control_scale_all_input,
	0,
	0x007424F0, // $TODO write the function chuckle nuts
	"scale input to x strength over y seconds",
	NULL,
	2, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	run_like_dvd,
	0,
	0x007428F0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	profiler_auto_core_save,
	0,
	0x00742B40, // $TODO write the function chuckle nuts
	"profiler dumps a core for each encounter\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	run_no_hdd,
	0,
	0x00742E10, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_bink_definition,
	cinematic_tag_reference_get_bink,
	0,
	0x00742F70, // $TODO write the function chuckle nuts
	"Retrieve the bink tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	voice_set_force_match_configurations,
	0,
	0x00743560, // $TODO write the function chuckle nuts
	"sets matchmaking voice options\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	voice_set_force_hud,
	0,
	0x007436C0, // $TODO write the function chuckle nuts
	"sets matchmaking voice hud\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_custom_animation_speed,
	0,
	0x00743940, // $TODO write the function chuckle nuts
	"if the object is playing a custom animation, scale it to n playback speed",
	NULL,
	2, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	scenery_animation_start_at_frame_loop,
	0,
	0x00743E10, // $TODO write the function chuckle nuts
	"starts a custom animation playing on a piece of scenery at a specific frame and then loops\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_scenery, _hs_type_animation_graph, _hs_type_string_id, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	saved_film_set_repro_mode,
	0,
	0x00744280, // $TODO write the function chuckle nuts
	"set the saved film into repro mode (splitscreen users are created based on the number of splitscreen players that recorded the film)",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_cinematic_scene_definition,
	cortana_tag_reference_get_scene,
	0,
	0x007445B0, // $TODO write the function chuckle nuts
	"Retrieve a cinematic tag from the cinematic system\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	net_banhammer_force_download,
	0,
	0x00744AB0, // $TODO write the function chuckle nuts
	"force banhammer file download\r\nNETWORK SAFE: yes",
	NULL,
	2, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	font_set_emergency,
	0,
	font_initialize_emergency, // 0x00744C40
	"sets the font system into emergency mode\r\nNETWORK SAFE: yes",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	biped_force_ground_fitting_on,
	0,
	0x00744EB0, // $TODO write the function chuckle nuts
	"force ground fitting on, even during custom animations or movement",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_chud_objective,
	0,
	0x00745320, // $TODO write the function chuckle nuts
	"activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_cutscene_title
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	chud_show_cinematic_title,
	0,
	0x00745640, // $TODO write the function chuckle nuts
	"activates the chapter title\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_unit, _hs_type_cutscene_title
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_boolean,
	terminal_is_being_read,
	0,
	0x00745B70, // $TODO write the function chuckle nuts
	"returns whether or not a terminal is currently being read",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	terminal_was_accessed,
	0,
	0x00745D80, // $TODO write the function chuckle nuts
	"returns whether or not the given terminal was accessed",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	terminal_was_completed,
	0,
	0x00746210, // $TODO write the function chuckle nuts
	"returns whether or not the given terminal was read to completion",
	NULL,
	1, _hs_type_object
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_weapon,
	unit_get_primary_weapon,
	0,
	0x007465B0, // $TODO write the function chuckle nuts
	"return the primary weapon of the specified unit",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dump_cortana_script,
	0,
	0x00746C20, // $TODO write the function chuckle nuts
	"dump the cinematic script to cinematic_dump_cortana.txt",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_animation_graph,
	budget_resource_get_animation_graph,
	0,
	0x00746E50, // $TODO write the function chuckle nuts
	"Get a non resolving reference to a model animation graph budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_budget_reference_animation_graph
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	game_award_level_complete_achievements,
	0,
	0x00747210, // $TODO write the function chuckle nuts
	"causes the player to successfully finish the current level and move to the next\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_looping_sound,
	budget_resource_get_looping_sound,
	0,
	0x00747410, // $TODO write the function chuckle nuts
	"Get a non resolving reference to a looping sound budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_budget_reference_looping_sound
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_safe_to_respawn,
	0,
	0x00747840, // $TODO write the function chuckle nuts
	"\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cortana_effect_kill,
	0,
	0x00747D50, // $TODO write the function chuckle nuts
	"kills the cortana effect\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_create_content_item_slayer,
	0,
	0x00747F10, // $TODO write the function chuckle nuts
	"creates a slayer variant\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	test_create_content_item_screenshot,
	0,
	0x00748080, // $TODO write the function chuckle nuts
	"creates a screenshot (which isn't actually viewable)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	cinematic_scripting_destroy_cortana_effect_cinematic,
	0,
	0x00748300, // $TODO write the function chuckle nuts
	"Destroy cortana effect cinematic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_migrate_infanty,
	0,
	0x007485A0, // $TODO write the function chuckle nuts
	"migrates the infantry in an encounter into a different squad.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_ai
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_cinematic_motion_blur,
	0,
	0x0072F600, // $TODO write the function chuckle nuts
	"enable/disable cinematic motion blur\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_dont_do_avoidance,
	0,
	0x0072FA30, // $TODO write the function chuckle nuts
	"set the AI to not do any vector avoidance when flying\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_scripting_clean_up,
	0,
	0x0072FDD0, // $TODO write the function chuckle nuts
	"Unload the scenario cinematic zone\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	ai_erase_inactive,
	0,
	0x00730250, // $TODO write the function chuckle nuts
	"Erase inactive actors in the given ai index, and any of those actors who become inactive within the next designated ticks\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_ai, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	ai_survival_cleanup,
	0,
	0x007305F0, // $TODO write the function chuckle nuts
	"Activate cure isolation death for the given AI, and optionally delete inactive actors in that group too (ai, on/off, kill inactive)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_ai, _hs_type_boolean, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	stop_bink_movie,
	0,
	0x00730B40, // $TODO write the function chuckle nuts
	"Stops all bink movie playback\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	play_credits_unskippable,
	0,
	0x00730D30, // $TODO write the function chuckle nuts
	"plays credits movie, no button press detection (you can still use cinematic skipping around this)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_sound,
	budget_resource_get_sound,
	0,
	0x00730FA0, // $TODO write the function chuckle nuts
	"Get a non resolving reference to a looping sound budget reference\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_budget_reference_sound
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 6,
	_hs_type_void,
	controller_set_single_player_level_unlocked,
	0,
	debug_set_single_player_level_unlocked, // 0x00731460,
	"<controller> <level index> <bool locked> set single player level locked state for specified controller\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_enum_controller, _hs_type_short_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	physical_memory_dump,
	0,
	0x00731700, // $TODO write the function chuckle nuts
	"dumps a record of all physical memory allocations",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	tag_resources_validate_all_pages,
	0,
	0x00731880, // $TODO write the function chuckle nuts
	"resource validation every main_loop_body call",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	cinematic_set_debug_mode,
	0,
	0x00731AC0, // $TODO write the function chuckle nuts
	"Puts all cinematic playback into debug mode (automatically generated script function!)\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_object,
	cinematic_scripting_get_object,
	0,
	0x00731E40, // $TODO write the function chuckle nuts
	"Get an object used in a cinematic\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	dummy_function,
	0,
	0x007322F0, // $TODO write the function chuckle nuts
	"Function that does nothing.",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	gp_integer_get,
	0,
	0x007325E0, // $TODO write the function chuckle nuts
	"get a game progression integer variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	gp_integer_set,
	0,
	0x00732900, // $TODO write the function chuckle nuts
	"set a game progression integer variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	gp_boolean_get,
	0,
	0x00732D60, // $TODO write the function chuckle nuts
	"get a game progression boolean variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	gp_boolean_set,
	0,
	0x007331F0, // $TODO write the function chuckle nuts
	"set a game progression boolean variable\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_string_id, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gp_dump,
	0,
	0x00733630, // $TODO write the function chuckle nuts
	"dumps all game progression variables to a file with specified file name.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gp_dump_debug,
	0,
	0x00733830, // $TODO write the function chuckle nuts
	"dumps all game progression variables to a file with specified file name - from both game options and network sessions parameters.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gp_startup,
	0,
	0x00733A50, // $TODO write the function chuckle nuts
	"sets the file from which to load game progression data on startup.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	gp_reset,
	0,
	0x00733CA0, // $TODO write the function chuckle nuts
	"resets all the state of the game progression system\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	gp_commit_options,
	0,
	0x00733EF0, // $TODO write the function chuckle nuts
	"debugging command that commits game options to game options so they will persist after a map reset\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_start_screen_effect,
	0,
	0x007341D0, // $TODO write the function chuckle nuts
	"Play the associated area screen effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_stop_screen_effect,
	0,
	0x00734580, // $TODO write the function chuckle nuts
	"Stop the associated area screen effect.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	game_level_prepare,
	0,
	0x00734B00, // $TODO write the function chuckle nuts
	"prepare to transition game to another level\r\nNETWORK SAFE: safe-ish in deterministic networking as long as it is not invoked from the console",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	prepare_game_level,
	0,
	0x00734F20, // $TODO write the function chuckle nuts
	"prepare to transition game to another level\r\nNETWORK SAFE: safe-ish in deterministic networking as long as it is not invoked from the console",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	game_start_with_network_session,
	0,
	0x007352D0, // $TODO write the function chuckle nuts
	"<campaign id> <map id> <scenario> debug map launching: starts a game on the specified map with campaign_id and map_id.\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	3, _hs_type_long_integer, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	levels_add_campaign_map_with_id,
	0,
	0x007354F0, // $TODO write the function chuckle nuts
	"<map id> <scenario> add scenario to levels array\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	2, _hs_type_long_integer, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	levels_add_campaign_map,
	0,
	0x00735730, // $TODO write the function chuckle nuts
	"<scenario> add a single scenario to levels array\r\nNETWORK SAFE: No, for init.txt only",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_impulse_start_editor,
	0,
	0x007358E0, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	sound_impulse_start_effect_editor,
	0,
	0x00735CB0, // $TODO write the function chuckle nuts
	"plays an impulse sound from the specified source object (or \"none\"), with the specified scale and effect.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_impulse_start_3d_editor,
	0,
	0x00735EB0, // $TODO write the function chuckle nuts
	"<sound> <azimuth> <scale> at the sound's minimum distance\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_sound, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_looping_start_editor,
	0,
	0x00736010, // $TODO write the function chuckle nuts
	"plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_looping_sound, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_sound_channels_log_start,
	0,
	0x00736300, // $TODO write the function chuckle nuts
	"starts the sound channel debugger log\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_sound_channels_log_start_named,
	0,
	0x007364C0, // $TODO write the function chuckle nuts
	"starts the sound channel debugger log with the specified file name\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_sound_channels_log_stop,
	0,
	0x00736790, // $TODO write the function chuckle nuts
	"stops the sound channel debugger log\r\nNETWORK SAFE: doesn't affect global state, but does kick-off I/O",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	cinematic_scripting_set_user_input_constraints,
	0,
	0x00736970, // $TODO write the function chuckle nuts
	"Set the user input constraints for a cinematic.\r\nNETWORKING SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	is_skull_primary_enable,
	0,
	0x00736E80, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_enum_primary_skull, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	is_skull_secondary_enable,
	0,
	0x00737200, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_enum_secondary_skull, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	controller_set_popup_message_index,
	0,
	debug_set_popup_message_index, // 0x007375C0,
	"<controller> <message index> set the highest popup message that the player has accepted\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_controller, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	controller_set_vidmaster_seen,
	0,
	0x00737990, // $TODO write the function chuckle nuts
	"<controller> <vidmaster_seen> set whether the player has accepted the vidmaster popup\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_enum_controller, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_enter_lobby,
	0,
	0x00737AC0, // $TODO write the function chuckle nuts
	"<lobby index> index 0-5\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	survival_mode_respawn_dead_players,
	0,
	0x00737D30, // $TODO write the function chuckle nuts
	"respawns all dead players even if no lives are left\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	survival_mode_lives_get,
	0,
	0x00738080, // $TODO write the function chuckle nuts
	"get the number of lives left in survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_lives_set,
	0,
	0x007383B0, // $TODO write the function chuckle nuts
	"set the number of lives left in survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_short_integer,
	survival_mode_set_get,
	0,
	0x007387B0, // $TODO write the function chuckle nuts
	"get the current set index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_set_set,
	0,
	0x00738A30, // $TODO write the function chuckle nuts
	"set the current set index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_short_integer,
	survival_mode_round_get,
	0,
	0x00738D00, // $TODO write the function chuckle nuts
	"get the current round index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_round_set,
	0,
	0x00739120, // $TODO write the function chuckle nuts
	"set the current round index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_short_integer,
	survival_mode_wave_get,
	0,
	0x007394E0, // $TODO write the function chuckle nuts
	"get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_wave_set,
	0,
	0x00739780, // $TODO write the function chuckle nuts
	"set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_real,
	survival_mode_set_multiplier_get,
	0,
	0x00739B20, // $TODO write the function chuckle nuts
	"get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_set_multiplier_set,
	0,
	0x00739EB0, // $TODO write the function chuckle nuts
	"set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_real,
	survival_mode_bonus_multiplier_get,
	0,
	0x0073A110, // $TODO write the function chuckle nuts
	"get the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_bonus_multiplier_set,
	0,
	0x0073A520, // $TODO write the function chuckle nuts
	"set the current wave index for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_sound_channel_fadeout_time,
	0,
	0x0073A860, // $TODO write the function chuckle nuts
	"set fadeout time for sound channel debugger",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_set_rounds_per_set,
	0,
	0x0073ABB0, // $TODO write the function chuckle nuts
	"set the number of rounds in each set for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_set_waves_per_round,
	0,
	0x0073AF60, // $TODO write the function chuckle nuts
	"set the number of waves in each round for survival mode\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	render_model_marker_name_filter,
	0,
	0x0073B210, // $TODO write the function chuckle nuts
	"sets a substring to search for when rendering model markers",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	survival_mode_event_new,
	0,
	0x0073B400, // $TODO write the function chuckle nuts
	"fires a start new set game event\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	survival_mode_begin_new_set,
	0,
	0x0073B7F0, // $TODO write the function chuckle nuts
	"Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	survival_mode_begin_new_round,
	0,
	0x0073B9D0, // $TODO write the function chuckle nuts
	"Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	survival_mode_begin_new_wave,
	0,
	0x0073BC30, // $TODO write the function chuckle nuts
	"Starts a new round\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	survival_mode_award_hero_medal,
	0,
	0x0073BF70, // $TODO write the function chuckle nuts
	"lets the engine award the hero medal\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_long_integer,
	campaign_metagame_get_player_score,
	0,
	0x0073C0E0, // $TODO write the function chuckle nuts
	"Calculates the score for an individual player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	collision_debug_lightmaps_print,
	0,
	0x0073C4F0, // $TODO write the function chuckle nuts
	"Prints the collision_debug_lightmaps text to the console\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	debug_coop_campaign_save,
	0,
	0x0073C690, // $TODO write the function chuckle nuts
	"Enables/disables debugging of co-op campaign save\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_set_look_training_hack,
	0,
	0x0073CA60, // $TODO write the function chuckle nuts
	"enables or disables the look training hack.  This allows players to touch devices when in a vehicle, but not exit the vehicle or do anything else.",
	NULL,
	2, _hs_type_unit, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_boolean,
	device_arg_has_been_touched_by_unit,
	0,
	0x0073CDB0, // $TODO write the function chuckle nuts
	"Returns true if the unit has touched the device in the past X ticks.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_device, _hs_type_unit, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	gui_hide_all_screens,
	0,
	0x0073D360, // $TODO write the function chuckle nuts
	"Hide all UI screens\r\nNETWORK SAFE: No",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	debug_lightmap_inspect,
	0,
	0x0073D5C0, // $TODO write the function chuckle nuts
	"show the lightmap status",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	achievements_display_debug_names,
	0,
	0x0073D7C0, // $TODO write the function chuckle nuts
	"display debug achievement names",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	achievement_grant_to_controller_by_debug_name,
	0,
	0x0073DA80, // $TODO write the function chuckle nuts
	"unlocks an achievement by debug name for the indicated controller\r\nNETWORK SAFE: NO",
	NULL,
	2, _hs_type_enum_controller, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	achievements_grant_all_to_controller,
	0,
	0x0073DD60, // $TODO write the function chuckle nuts
	"grants all achievements to the indicated controller\r\nNETWORK SAFE: NO",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	achievements_award_all_vidmaster_achievements,
	0,
	0x0073DEF0, // $TODO write the function chuckle nuts
	"grants all vidmaster achievements to the indicated controller\r\nNETWORK SAFE: NO",
	NULL,
	1, _hs_type_enum_controller
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	chud_fade_for_first_person_cinematic,
	0,
	0x0073E330, // $TODO write the function chuckle nuts
	"Fades all chud elements except for those marked as 'motion sensor'\r\nNETWORK SAFE: Yes",
	NULL,
	2, _hs_type_real, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	achievement_post_chud_progression,
	0,
	0x0073E5A0, // $TODO write the function chuckle nuts
	"posts an achievement progression toast from script\r\nNETWORK SAFE: Yes",
	NULL,
	3, _hs_type_unit, _hs_type_string_id, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	object_set_vision_mode_render_default,
	0,
	0x0073EA10, // $TODO write the function chuckle nuts
	"Flag an object so that it renders in the everything else color\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	2, _hs_type_object, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	chud_show_navpoint,
	0,
	0x0073EE00, // $TODO write the function chuckle nuts
	"Activate a scripted navpoint for a specific player\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	4, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_real, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	run_like_package,
	0,
	0x0073F200, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	clear_map_slot,
	0,
	0x0073F530, // $TODO write the function chuckle nuts
	"nuke cache<number>.map",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	clear_map_type,
	0,
	0x0073F720, // $TODO write the function chuckle nuts
	"nuke all maps corresponding to specified type. 0 = mainmenu, 1 = shared, 2 = campaign, 3 = hub, 4 = linear, 5 = scene, 6 = cinematic",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_confirm_message,
	0,
	0x0073F9B0, // $TODO write the function chuckle nuts
	"Locks the accept button until the user presses accept\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_confirm_cancel_message,
	0,
	0x0073FCD0, // $TODO write the function chuckle nuts
	"Locks the cancel button until the user presses cancel\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unit_confirm_y_button,
	0,
	0x00740100, // $TODO write the function chuckle nuts
	"Locks the accept button until the user presses accept\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_long_integer,
	player_get_kills_by_type,
	0,
	0x00740380, // $TODO write the function chuckle nuts
	"Retrieves the number of kills a specific player has gotten by metagame type and class, ask an engineer for the mapping\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_unit, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_boolean,
	unit_flashlight_on,
	0,
	0x00740880, // $TODO write the function chuckle nuts
	"returns true if the unit has a flashlight on\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_unit
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	clear_command_buffer_cache_from_script,
	0,
	0x00740C60, // $TODO write the function chuckle nuts
	"clear the command buffer cache\r\nYes",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	sound_looping_resume,
	0,
	0x00741160, // $TODO write the function chuckle nuts
	"plays a looping sound from the specified source object (or \"none\"), with the specified scale.\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	3, _hs_type_looping_sound, _hs_type_object, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	chud_bonus_round_set_target_score,
	0,
	0x007414A0, // $TODO write the function chuckle nuts
	"set the bonus round score target\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown673,
	0,
	0x00741890, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unknown674,
	0,
	0x00741AD0, // $TODO write the function chuckle nuts
	"",
	NULL,
	3, _hs_type_short_integer, _hs_type_short_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	unknown675,
	0,
	0x00741CB0, // $TODO write the function chuckle nuts
	"",
	NULL,
	4, _hs_type_short_integer, _hs_type_short_integer, _hs_type_short_integer, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown676,
	0,
	0x00741F00, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown677,
	0,
	0x00742220, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown678,
	0,
	0x007424D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown679,
	0,
	0x007426B0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown67A,
	0,
	0x007428D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown67B,
	0,
	0x00742B10, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown67C,
	0,
	0x00742E90, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	saber_ui_get_player_model_id,
	0,
	0x00743070, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	saber_ui_get_music_id,
	0,
	0x00743540, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unknown67F,
	0,
	0x007436E0, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_long_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unit_set_armor,
	0,
	0x007439E0, // $TODO write the function chuckle nuts
	"",
	NULL,
	3, _hs_type_object, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	player_set_armor,
	0,
	0x00743F10, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_string_id, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	change_network_privacy_to_system_link,
	0,
	0x007442D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ssao_enable,
	0,
	0x00744440, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ssao_radius,
	0,
	0x007448D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ssao_intensity,
	0,
	0x00744BD0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	ssao_sampleZThreshold,
	0,
	0x00744FA0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	fxaa_enable,
	0,
	0x007453D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	color_grading_enable,
	0,
	0x007456D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	lightshafts_enable,
	0,
	0x00745B40, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unknown68A,
	0,
	0x00745E00, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_short_integer, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflections_enable,
	0,
	0x00746060, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_blur_enable,
	0,
	0x00746320, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_multisampling_enabled,
	0,
	0x00746740, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_multisample_passes,
	0,
	0x00746B20, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_fxaa_enable,
	0,
	0x00746E20, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_debug_enable,
	0,
	0x00747160, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	reflection_color_bilinear_filter_enable,
	0,
	0x00747500, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	end_match_wait_time_increment,
	0,
	0x00747800, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown693,
	0,
	0x00747C20, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	background_bitmap_set,
	0,
	0x00747EC0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_short_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	local_player_earn_wp_event,
	0,
	0x007481F0, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_string_id
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	global_screen_effect_enable,
	0,
	0x00748530, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 6,
	_hs_type_void,
	unknown697,
	0,
	0x0072F5C0, // $TODO write the function chuckle nuts
	"",
	NULL,
	3, _hs_type_long_integer, _hs_type_string, _hs_type_string
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown698,
	0,
	0x0072F7E0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	saber_mouse_inversion_get,
	0,
	0x0072FB20, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_long_integer,
	saber_mouse_inversion_set,
	0,
	0x0072FCE0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 14,
	_hs_type_void,
	unknown69B,
	0,
	0x0072FFE0, // $TODO write the function chuckle nuts
	"",
	NULL,
	7, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 4,
	_hs_type_void,
	unknown69C,
	0,
	0x007303D0, // $TODO write the function chuckle nuts
	"",
	NULL,
	2, _hs_type_short_integer, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown69D,
	0,
	0x00730590, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 8,
	_hs_type_void,
	unknown69E,
	0,
	0x00730780, // $TODO write the function chuckle nuts
	"",
	NULL,
	4, _hs_type_long_integer, _hs_type_real, _hs_type_real, _hs_type_real
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 2,
	_hs_type_void,
	unknown69F,
	0,
	0x00730B20, // $TODO write the function chuckle nuts
	"",
	NULL,
	1, _hs_type_boolean
);
MACRO_FUNCTION_EVALUATE2(hs_function_definition, 0,
	_hs_type_void,
	unknown6A0,
	0,
	0x00730CB0, // $TODO write the function chuckle nuts
	"",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 4,
	_hs_type_void,
	object_copy_player_appearance,
	0,
	object_scripting_copy_player_appearance,
	"copy appearance into object from player n (starts counting from 0)",
	NULL,
	2, _hs_type_object, _hs_type_long_integer
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 0,
	_hs_type_void,
	exit_game,
	0,
	main_exit_game,
	"exits the game.",
	NULL,
	0,
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	load_preferences_from_file,
	0,
	load_preferences_from_file_hs,
	"Loads preferences from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);
MACRO_FUNCTION_EVALUATE(hs_function_definition, 2,
	_hs_type_void,
	load_customization_from_file,
	0,
	load_customization_from_file_hs,
	"Loads customization from the specified file\r\nNETWORK SAFE: Unknown, assumed unsafe",
	NULL,
	1, _hs_type_string
);

#pragma endregion // HS_FUNCTION_DEFINITIONS

static const hs_function_definition* const hs_function_table[]
{
	(hs_function_definition*)&begin_definition,
	(hs_function_definition*)&begin_random_definition,
	(hs_function_definition*)&if_definition,
	(hs_function_definition*)&cond_definition,
	(hs_function_definition*)&set_definition,
	(hs_function_definition*)&and_definition,
	(hs_function_definition*)&or_definition,
	(hs_function_definition*)&add_definition,
	(hs_function_definition*)&subtract_definition,
	(hs_function_definition*)&multiply_definition,
	(hs_function_definition*)&divide_definition,
	(hs_function_definition*)&modulo_definition,
	(hs_function_definition*)&min_definition,
	(hs_function_definition*)&max_definition,
	(hs_function_definition*)&equal_definition,
	(hs_function_definition*)&not_equal_definition,
	(hs_function_definition*)&gt_definition,
	(hs_function_definition*)&lt_definition,
	(hs_function_definition*)&gte_definition,
	(hs_function_definition*)&lte_definition,
	(hs_function_definition*)&sleep_definition,
	(hs_function_definition*)&sleep_forever_definition,
	//(hs_function_definition*)&sleep_for_ticks_definition,
	(hs_function_definition*)&sleep_until_definition,
	(hs_function_definition*)&wake_definition,
	(hs_function_definition*)&inspect_definition,
	(hs_function_definition*)&object_to_unit_definition,
	(hs_function_definition*)&evaluate_1_definition,
	(hs_function_definition*)&not_1_definition,
	(hs_function_definition*)&pin_3_definition,
	(hs_function_definition*)&print_1_definition,
	(hs_function_definition*)&log_print_1_definition,
	(hs_function_definition*)&debug_scripting_show_thread_2_definition,
	(hs_function_definition*)&debug_script_thread_2_definition,
	(hs_function_definition*)&debug_scripting_1_definition,
	(hs_function_definition*)&debug_scripting_globals_1_definition,
	(hs_function_definition*)&debug_scripting_variable_2_definition,
	(hs_function_definition*)&debug_scripting_variable_all_1_definition,
	(hs_function_definition*)&breakpoint_1_definition,
	(hs_function_definition*)&kill_active_scripts_0_definition,
	(hs_function_definition*)&get_executing_running_thread_0_definition,
	(hs_function_definition*)&kill_thread_1_definition,
	(hs_function_definition*)&script_started_1_definition,
	(hs_function_definition*)&script_finished_1_definition,
	(hs_function_definition*)&players_0_definition,
	(hs_function_definition*)&player_get_1_definition,
	(hs_function_definition*)&kill_volume_enable_1_definition,
	(hs_function_definition*)&kill_volume_disable_1_definition,
	(hs_function_definition*)&volume_teleport_players_not_inside_2_definition,
	(hs_function_definition*)&volume_test_object_2_definition,
	(hs_function_definition*)&volume_test_objects_2_definition,
	(hs_function_definition*)&volume_test_objects_all_2_definition,
	(hs_function_definition*)&volume_test_players_1_definition,
	(hs_function_definition*)&volume_test_players_all_1_definition,
	(hs_function_definition*)&volume_return_objects_1_definition,
	(hs_function_definition*)&volume_return_objects_by_type_2_definition,
	(hs_function_definition*)&zone_set_trigger_volume_enable_2_definition,
	(hs_function_definition*)&list_get_2_definition,
	(hs_function_definition*)&list_count_1_definition,
	(hs_function_definition*)&list_count_not_dead_1_definition,
	(hs_function_definition*)&effect_new_2_definition,
	(hs_function_definition*)&effect_new_random_2_definition,
	(hs_function_definition*)&effect_new_at_ai_point_2_definition,
	(hs_function_definition*)&effect_new_on_object_marker_3_definition,
	(hs_function_definition*)&effect_new_on_ground_2_definition,
	(hs_function_definition*)&damage_new_2_definition,
	(hs_function_definition*)&damage_object_effect_2_definition,
	(hs_function_definition*)&damage_objects_effect_2_definition,
	(hs_function_definition*)&damage_object_3_definition,
	(hs_function_definition*)&damage_objects_3_definition,
	(hs_function_definition*)&damage_players_1_definition,
	(hs_function_definition*)&soft_ceiling_enable_2_definition,
	(hs_function_definition*)&object_create_1_definition,
	(hs_function_definition*)&object_create_clone_1_definition,
	(hs_function_definition*)&object_create_anew_1_definition,
	(hs_function_definition*)&object_create_if_necessary_1_definition,
	(hs_function_definition*)&object_create_containing_1_definition,
	(hs_function_definition*)&object_create_clone_containing_1_definition,
	(hs_function_definition*)&object_create_anew_containing_1_definition,
	(hs_function_definition*)&object_create_folder_1_definition,
	(hs_function_definition*)&object_create_folder_anew_1_definition,
	(hs_function_definition*)&object_destroy_1_definition,
	(hs_function_definition*)&object_destroy_containing_1_definition,
	(hs_function_definition*)&object_destroy_all_0_definition,
	(hs_function_definition*)&object_destroy_type_mask_1_definition,
	(hs_function_definition*)&objects_delete_by_definition_1_definition,
	(hs_function_definition*)&object_destroy_folder_1_definition,
	(hs_function_definition*)&object_hide_2_definition,
	(hs_function_definition*)&object_set_shadowless_2_definition,
	(hs_function_definition*)&object_buckling_magnitude_get_1_definition,
	(hs_function_definition*)&object_function_set_2_definition,
	(hs_function_definition*)&object_set_function_variable_4_definition,
	(hs_function_definition*)&object_clear_function_variable_2_definition,
	(hs_function_definition*)&object_clear_all_function_variables_1_definition,
	(hs_function_definition*)&object_dynamic_simulation_disable_2_definition,
	(hs_function_definition*)&object_set_phantom_power_2_definition,
	(hs_function_definition*)&object_wake_physics_1_definition,
	(hs_function_definition*)&object_set_ranged_attack_inhibited_2_definition,
	(hs_function_definition*)&object_set_melee_attack_inhibited_2_definition,
	(hs_function_definition*)&objects_dump_memory_0_definition,
	(hs_function_definition*)&object_get_health_1_definition,
	(hs_function_definition*)&object_get_shield_1_definition,
	(hs_function_definition*)&object_set_shield_effect_3_definition,
	(hs_function_definition*)&object_set_physics_2_definition,
	(hs_function_definition*)&object_get_parent_1_definition,
	(hs_function_definition*)&objects_attach_4_definition,
	(hs_function_definition*)&object_at_marker_2_definition,
	(hs_function_definition*)&objects_detach_2_definition,
	(hs_function_definition*)&object_set_scale_3_definition,
	(hs_function_definition*)&object_set_velocity_2_definition,
	(hs_function_definition*)&object_set_inertia_tensor_scale_2_definition,
	(hs_function_definition*)&object_set_collision_damage_armor_scale_2_definition,
	(hs_function_definition*)&object_set_velocity_4_definition,
	(hs_function_definition*)&object_set_deleted_when_deactivated_1_definition,
	(hs_function_definition*)&object_model_target_destroyed_2_definition,
	(hs_function_definition*)&object_model_targets_destroyed_2_definition,
	(hs_function_definition*)&object_damage_damage_section_3_definition,
	(hs_function_definition*)&object_cannot_die_2_definition,
	(hs_function_definition*)&object_vitality_pinned_1_definition,
	(hs_function_definition*)&garbage_collect_now_0_definition,
	(hs_function_definition*)&garbage_collect_unsafe_0_definition,
	(hs_function_definition*)&garbage_collect_multiplayer_0_definition,
	(hs_function_definition*)&object_cannot_take_damage_1_definition,
	(hs_function_definition*)&object_can_take_damage_1_definition,
	(hs_function_definition*)&object_cinematic_lod_2_definition,
	(hs_function_definition*)&object_cinematic_collision_2_definition,
	(hs_function_definition*)&object_cinematic_visibility_2_definition,
	(hs_function_definition*)&objects_predict_1_definition,
	(hs_function_definition*)&objects_predict_high_1_definition,
	(hs_function_definition*)&objects_predict_low_1_definition,
	(hs_function_definition*)&object_type_predict_high_1_definition,
	(hs_function_definition*)&object_type_predict_low_1_definition,
	(hs_function_definition*)&object_type_predict_1_definition,
	(hs_function_definition*)&object_teleport_2_definition,
	(hs_function_definition*)&object_teleport_to_ai_point_2_definition,
	(hs_function_definition*)&object_set_facing_2_definition,
	(hs_function_definition*)&object_set_shield_2_definition,
	(hs_function_definition*)&object_set_shield_normalized_2_definition,
	(hs_function_definition*)&object_set_shield_stun_2_definition,
	(hs_function_definition*)&object_set_shield_stun_infinite_1_definition,
	(hs_function_definition*)&object_set_permutation_3_definition,
	(hs_function_definition*)&object_set_variant_2_definition,
	(hs_function_definition*)&object_set_region_state_3_definition,
	(hs_function_definition*)&objects_can_see_object_3_definition,
	(hs_function_definition*)&objects_can_see_flag_3_definition,
	(hs_function_definition*)&objects_distance_to_object_2_definition,
	(hs_function_definition*)&objects_distance_to_flag_2_definition,
	(hs_function_definition*)&map_info_0_definition,
	(hs_function_definition*)&position_predict_3_definition,
	(hs_function_definition*)&shader_predict_1_definition,
	(hs_function_definition*)&bitmap_predict_1_definition,
	(hs_function_definition*)&script_recompile_0_definition,
	(hs_function_definition*)&script_doc_0_definition,
	(hs_function_definition*)&help_1_definition,
	(hs_function_definition*)&game_engine_objects_0_definition,
	(hs_function_definition*)&random_range_2_definition,
	(hs_function_definition*)&real_random_range_2_definition,
	(hs_function_definition*)&physics_constants_reset_0_definition,
	(hs_function_definition*)&physics_set_gravity_1_definition,
	(hs_function_definition*)&physics_set_velocity_frame_3_definition,
	(hs_function_definition*)&physics_disable_character_ground_adhesion_forces_1_definition,
	(hs_function_definition*)&havok_debug_start_0_definition,
	(hs_function_definition*)&havok_dump_world_2_definition,
	(hs_function_definition*)&havok_dump_world_close_movie_0_definition,
	(hs_function_definition*)&havok_profile_start_0_definition,
	(hs_function_definition*)&havok_profile_end_0_definition,
	(hs_function_definition*)&havok_profile_range_2_definition,
	(hs_function_definition*)&havok_reset_allocated_state_0_definition,
	(hs_function_definition*)&breakable_surfaces_enable_1_definition,
	(hs_function_definition*)&breakable_surfaces_reset_0_definition,
	(hs_function_definition*)&recording_play_2_definition,
	(hs_function_definition*)&recording_play_and_delete_2_definition,
	(hs_function_definition*)&recording_play_and_hover_2_definition,
	(hs_function_definition*)&recording_kill_1_definition,
	(hs_function_definition*)&recording_time_1_definition,
	(hs_function_definition*)&render_lights_1_definition,
	(hs_function_definition*)&print_light_state_0_definition,
	(hs_function_definition*)&render_lights_enable_cinematic_shadow_4_definition,
	(hs_function_definition*)&texture_camera_set_object_marker_3_definition,
	(hs_function_definition*)&texture_camera_set_position_3_definition,
	(hs_function_definition*)&texture_camera_set_target_3_definition,
	(hs_function_definition*)&texture_camera_attach_to_object_2_definition,
	(hs_function_definition*)&texture_camera_target_object_2_definition,
	(hs_function_definition*)&texture_camera_position_world_offset_3_definition,
	(hs_function_definition*)&texture_camera_on_0_definition,
	//(hs_function_definition*)&texture_camera_bink_0_definition,
	(hs_function_definition*)&texture_camera_off_0_definition,
	(hs_function_definition*)&texture_camera_set_aspect_ratio_1_definition,
	(hs_function_definition*)&texture_camera_set_resolution_2_definition,
	(hs_function_definition*)&texture_camera_render_mode_1_definition,
	(hs_function_definition*)&texture_camera_unknownBC_1_definition,
	(hs_function_definition*)&texture_camera_set_fov_1_definition,
	(hs_function_definition*)&texture_camera_set_fov_frame_target_1_definition,
	(hs_function_definition*)&texture_camera_enable_dynamic_lights_1_definition,
	(hs_function_definition*)&hud_camera_on_1_definition,
	(hs_function_definition*)&hud_camera_off_1_definition,
	(hs_function_definition*)&hud_camera_set_position_4_definition,
	(hs_function_definition*)&hud_camera_set_target_4_definition,
	(hs_function_definition*)&hud_camera_attach_to_object_3_definition,
	(hs_function_definition*)&hud_camera_target_object_3_definition,
	(hs_function_definition*)&hud_camera_structure_2_definition,
	(hs_function_definition*)&hud_camera_highlight_object_6_definition,
	(hs_function_definition*)&hud_camera_clear_objects_1_definition,
	(hs_function_definition*)&hud_camera_spin_around_2_definition,
	(hs_function_definition*)&hud_camera_from_player_view_1_definition,
	(hs_function_definition*)&hud_camera_window_5_definition,
	(hs_function_definition*)&render_debug_texture_camera_1_definition,
	(hs_function_definition*)&debug_structure_cluster_2_definition,
	(hs_function_definition*)&render_debug_structure_cluster_fog_1_definition,
	(hs_function_definition*)&render_debug_structure_fog_plane_1_definition,
	(hs_function_definition*)&render_debug_structure_fog_plane_infinite_extent_1_definition,
	(hs_function_definition*)&render_debug_structure_fog_zone_1_definition,
	(hs_function_definition*)&render_debug_structure_fog_zone_floodfill_1_definition,
	(hs_function_definition*)&render_debug_structure_all_fog_planes_1_definition,
	(hs_function_definition*)&render_debug_structure_all_cluster_errors_1_definition,
	(hs_function_definition*)&render_debug_structure_line_opacity_4_definition,
	(hs_function_definition*)&render_debug_structure_text_opacity_4_definition,
	(hs_function_definition*)&render_debug_structure_opacity_4_definition,
	(hs_function_definition*)&render_debug_structure_non_occluded_fog_planes_1_definition,
	(hs_function_definition*)&render_debug_structure_lightmaps_use_pervertex_0_definition,
	(hs_function_definition*)&render_debug_structure_lightmaps_use_reset_0_definition,
	(hs_function_definition*)&render_debug_structure_lightmaps_sample_enable_0_definition,
	(hs_function_definition*)&render_debug_structure_lightmaps_sample_disable_0_definition,
	(hs_function_definition*)&render_debug_query_object_bitmaps_1_definition,
	(hs_function_definition*)&render_debug_query_bsp_resources_2_definition,
	(hs_function_definition*)&render_debug_query_all_object_resources_0_definition,
	(hs_function_definition*)&render_debug_query_d3d_resources_0_definition,
	(hs_function_definition*)&render_debug_text_using_simple_font_1_definition,
	(hs_function_definition*)&render_postprocess_color_tweaking_reset_0_definition,
	(hs_function_definition*)&scenery_animation_start_3_definition,
	(hs_function_definition*)&scenery_animation_start_loop_3_definition,
	(hs_function_definition*)&scenery_animation_start_relative_4_definition,
	(hs_function_definition*)&scenery_animation_start_relative_loop_4_definition,
	(hs_function_definition*)&scenery_animation_start_at_frame_4_definition,
	(hs_function_definition*)&scenery_animation_start_relative_at_frame_5_definition,
	(hs_function_definition*)&scenery_animation_idle_1_definition,
	(hs_function_definition*)&scenery_get_animation_time_1_definition,
	(hs_function_definition*)&unit_can_blink_2_definition,
	(hs_function_definition*)&unit_set_active_camo_3_definition,
	(hs_function_definition*)&unit_open_1_definition,
	(hs_function_definition*)&unit_close_1_definition,
	(hs_function_definition*)&unit_kill_1_definition,
	(hs_function_definition*)&unit_kill_silent_1_definition,
	(hs_function_definition*)&unit_is_emitting_1_definition,
	(hs_function_definition*)&unit_get_custom_animation_time_1_definition,
	(hs_function_definition*)&unit_stop_custom_animation_1_definition,
	(hs_function_definition*)&custom_animation_4_definition,
	(hs_function_definition*)&custom_animation_loop_4_definition,
	(hs_function_definition*)&custom_animation_relative_5_definition,
	(hs_function_definition*)&custom_animation_relative_loop_5_definition,
	(hs_function_definition*)&custom_animation_list_4_definition,
	(hs_function_definition*)&unit_custom_animation_at_frame_5_definition,
	(hs_function_definition*)&unit_custom_animation_relative_at_frame_6_definition,
	(hs_function_definition*)&unit_is_playing_custom_animation_1_definition,
	(hs_function_definition*)&object_set_custom_animations_hold_on_last_frame_1_definition,
	(hs_function_definition*)&object_set_custom_animations_prevent_lipsync_head_movement_1_definition,
	(hs_function_definition*)&preferred_animation_list_add_2_definition,
	(hs_function_definition*)&preferred_animation_list_clear_0_definition,
	(hs_function_definition*)&unit_set_actively_controlled_2_definition,
	(hs_function_definition*)&unit_get_team_index_1_definition,
	(hs_function_definition*)&unit_aim_without_turning_2_definition,
	(hs_function_definition*)&unit_set_enterable_by_player_2_definition,
	(hs_function_definition*)&unit_set_seat_enterable_by_player_3_definition,
	(hs_function_definition*)&unit_get_enterable_by_player_1_definition,
	(hs_function_definition*)&unit_only_takes_damage_from_players_team_2_definition,
	(hs_function_definition*)&unit_enter_vehicle_3_definition,
	(hs_function_definition*)&unit_falling_damage_disable_2_definition,
	(hs_function_definition*)&unit_in_vehicle_type_2_definition,
	(hs_function_definition*)&object_get_turret_count_1_definition,
	(hs_function_definition*)&object_get_turret_2_definition,
	(hs_function_definition*)&unit_board_vehicle_2_definition,
	(hs_function_definition*)&unit_set_emotion_4_definition,
	(hs_function_definition*)&unit_set_emotion_by_name_4_definition,
	(hs_function_definition*)&unit_enable_eye_tracking_2_definition,
	(hs_function_definition*)&unit_set_integrated_flashlight_2_definition,
	(hs_function_definition*)&unit_set_voice_2_definition,
	(hs_function_definition*)&unit_enable_vision_mode_2_definition,
	(hs_function_definition*)&unit_in_vehicle_1_definition,
	(hs_function_definition*)&vehicle_test_seat_list_3_definition,
	(hs_function_definition*)&vehicle_test_seat_unit_3_definition,
	(hs_function_definition*)&vehicle_test_seat_2_definition,
	(hs_function_definition*)&unit_set_prefer_tight_camera_track_2_definition,
	(hs_function_definition*)&unit_exit_vehicle_1_definition,
	(hs_function_definition*)&unit_exit_vehicle_2_definition,
	(hs_function_definition*)&unit_set_maximum_vitality_3_definition,
	(hs_function_definition*)&units_set_maximum_vitality_3_definition,
	(hs_function_definition*)&unit_set_current_vitality_3_definition,
	(hs_function_definition*)&units_set_current_vitality_3_definition,
	(hs_function_definition*)&vehicle_load_magic_3_definition,
	(hs_function_definition*)&vehicle_unload_2_definition,
	(hs_function_definition*)&unit_set_animation_mode_2_definition,
	(hs_function_definition*)&magic_melee_attack_0_definition,
	(hs_function_definition*)&vehicle_riders_1_definition,
	(hs_function_definition*)&vehicle_driver_1_definition,
	(hs_function_definition*)&vehicle_gunner_1_definition,
	(hs_function_definition*)&unit_get_health_1_definition,
	(hs_function_definition*)&unit_get_shield_1_definition,
	(hs_function_definition*)&unit_get_total_grenade_count_1_definition,
	(hs_function_definition*)&unit_has_weapon_2_definition,
	(hs_function_definition*)&unit_has_weapon_readied_2_definition,
	(hs_function_definition*)&unit_has_any_equipment_1_definition,
	(hs_function_definition*)&unit_has_equipment_2_definition,
	(hs_function_definition*)&unit_lower_weapon_2_definition,
	(hs_function_definition*)&unit_raise_weapon_2_definition,
	(hs_function_definition*)&unit_drop_support_weapon_1_definition,
	(hs_function_definition*)&unit_spew_action_1_definition,
	(hs_function_definition*)&unit_force_reload_1_definition,
	(hs_function_definition*)&animation_stats_dump_0_definition,
	(hs_function_definition*)&unit_animation_forced_seat_1_definition,
	(hs_function_definition*)&unit_doesnt_drop_items_1_definition,
	(hs_function_definition*)&unit_impervious_2_definition,
	(hs_function_definition*)&unit_suspended_2_definition,
	(hs_function_definition*)&unit_add_equipment_4_definition,
	(hs_function_definition*)&weapon_hold_trigger_3_definition,
	(hs_function_definition*)&weapon_enable_warthog_chaingun_light_1_definition,
	(hs_function_definition*)&device_set_never_appears_locked_2_definition,
	(hs_function_definition*)&device_set_power_2_definition,
	(hs_function_definition*)&device_get_power_1_definition,
	(hs_function_definition*)&device_set_position_2_definition,
	(hs_function_definition*)&device_get_position_1_definition,
	(hs_function_definition*)&device_set_position_immediate_2_definition,
	(hs_function_definition*)&device_group_get_1_definition,
	(hs_function_definition*)&device_group_set_3_definition,
	(hs_function_definition*)&device_group_set_immediate_2_definition,
	(hs_function_definition*)&device_one_sided_set_2_definition,
	(hs_function_definition*)&device_ignore_player_set_2_definition,
	(hs_function_definition*)&device_operates_automatically_set_2_definition,
	(hs_function_definition*)&device_closes_automatically_set_2_definition,
	(hs_function_definition*)&device_group_change_only_once_more_set_2_definition,
	(hs_function_definition*)&device_set_position_track_3_definition,
	(hs_function_definition*)&device_set_overlay_track_2_definition,
	(hs_function_definition*)&device_animate_position_6_definition,
	(hs_function_definition*)&device_animate_overlay_5_definition,
	(hs_function_definition*)&cheat_all_powerups_0_definition,
	(hs_function_definition*)&cheat_all_weapons_0_definition,
	(hs_function_definition*)&cheat_all_vehicles_0_definition,
	(hs_function_definition*)&cheat_teleport_to_camera_0_definition,
	(hs_function_definition*)&cheat_active_camouflage_1_definition,
	(hs_function_definition*)&cheat_active_camouflage_by_player_2_definition,
	(hs_function_definition*)&cheats_load_0_definition,
	(hs_function_definition*)&drop_safe_1_definition,
	(hs_function_definition*)&drop_1_definition,
	(hs_function_definition*)&drop_variant_2_definition,
	(hs_function_definition*)&ai_enable_1_definition,
	(hs_function_definition*)&ai_enabled_0_definition,
	(hs_function_definition*)&ai_grenades_1_definition,
	(hs_function_definition*)&ai_dialogue_enable_1_definition,
	(hs_function_definition*)&ai_player_dialogue_enable_1_definition,
	(hs_function_definition*)&ai_infection_suppress_1_definition,
	(hs_function_definition*)&ai_fast_and_dumb_1_definition,
	(hs_function_definition*)&ai_dialogue_log_reset_0_definition,
	(hs_function_definition*)&ai_dialogue_log_dump_1_definition,
	(hs_function_definition*)&ai_get_object_1_definition,
	(hs_function_definition*)&ai_get_unit_1_definition,
	(hs_function_definition*)&ai_get_squad_1_definition,
	(hs_function_definition*)&ai_get_turret_ai_2_definition,
	(hs_function_definition*)&ai_random_smart_point_4_definition,
	(hs_function_definition*)&ai_nearest_point_2_definition,
	(hs_function_definition*)&ai_get_point_count_1_definition,
	(hs_function_definition*)&ai_point_set_get_point_2_definition,
	(hs_function_definition*)&ai_place_1_definition,
	(hs_function_definition*)&ai_place_2_definition,
	(hs_function_definition*)&ai_place_in_vehicle_2_definition,
	(hs_function_definition*)&ai_cannot_die_2_definition,
	(hs_function_definition*)&ai_vitality_pinned_1_definition,
	(hs_function_definition*)&ai_index_from_spawn_formation_2_definition,
	(hs_function_definition*)&ai_resurrect_1_definition,
	(hs_function_definition*)&ai_kill_1_definition,
	(hs_function_definition*)&ai_kill_silent_1_definition,
	(hs_function_definition*)&ai_erase_1_definition,
	(hs_function_definition*)&ai_erase_all_0_definition,
	(hs_function_definition*)&ai_disposable_2_definition,
	(hs_function_definition*)&ai_select_1_definition,
	(hs_function_definition*)&ai_deselect_0_definition,
	(hs_function_definition*)&ai_set_deaf_2_definition,
	(hs_function_definition*)&ai_set_blind_2_definition,
	(hs_function_definition*)&ai_set_weapon_up_2_definition,
	(hs_function_definition*)&ai_flood_disperse_2_definition,
	(hs_function_definition*)&ai_magically_see_2_definition,
	(hs_function_definition*)&ai_magically_see_object_2_definition,
	(hs_function_definition*)&ai_set_active_camo_2_definition,
	(hs_function_definition*)&ai_suppress_combat_2_definition,
	(hs_function_definition*)&ai_engineer_explode_1_definition,
	(hs_function_definition*)&ai_grunt_kamikaze_1_definition,
	(hs_function_definition*)&ai_migrate_2_definition,
	(hs_function_definition*)&ai_allegiance_2_definition,
	(hs_function_definition*)&ai_allegiance_remove_2_definition,
	(hs_function_definition*)&ai_allegiance_break_2_definition,
	(hs_function_definition*)&ai_braindead_2_definition,
	(hs_function_definition*)&ai_braindead_by_unit_2_definition,
	(hs_function_definition*)&ai_disregard_2_definition,
	(hs_function_definition*)&ai_prefer_target_2_definition,
	(hs_function_definition*)&ai_prefer_target_team_2_definition,
	(hs_function_definition*)&ai_prefer_target_ai_3_definition,
	(hs_function_definition*)&ai_set_targeting_group_2_definition,
	(hs_function_definition*)&ai_set_targeting_group_3_definition,
	(hs_function_definition*)&ai_teleport_to_starting_location_if_outside_bsp_1_definition,
	(hs_function_definition*)&ai_teleport_to_spawn_point_if_outside_bsp_1_definition,
	(hs_function_definition*)&ai_teleport_2_definition,
	(hs_function_definition*)&ai_bring_forward_2_definition,
	(hs_function_definition*)&ai_migrate_form_3_definition,
	(hs_function_definition*)&ai_morph_2_definition,
	(hs_function_definition*)&biped_morph_1_definition,
	(hs_function_definition*)&ai_renew_1_definition,
	(hs_function_definition*)&ai_force_active_2_definition,
	(hs_function_definition*)&ai_force_active_by_unit_2_definition,
	(hs_function_definition*)&ai_playfight_2_definition,
	(hs_function_definition*)&ai_reconnect_0_definition,
	(hs_function_definition*)&ai_berserk_2_definition,
	(hs_function_definition*)&ai_set_team_2_definition,
	(hs_function_definition*)&ai_allow_dormant_2_definition,
	(hs_function_definition*)&ai_is_attacking_1_definition,
	(hs_function_definition*)&ai_fighting_count_1_definition,
	(hs_function_definition*)&ai_living_count_1_definition,
	(hs_function_definition*)&ai_living_fraction_1_definition,
	(hs_function_definition*)&ai_in_vehicle_count_1_definition,
	(hs_function_definition*)&ai_body_count_1_definition,
	(hs_function_definition*)&ai_strength_1_definition,
	(hs_function_definition*)&ai_swarm_count_1_definition,
	(hs_function_definition*)&ai_nonswarm_count_1_definition,
	(hs_function_definition*)&ai_actors_1_definition,
	(hs_function_definition*)&ai_allegiance_broken_2_definition,
	(hs_function_definition*)&ai_spawn_count_1_definition,
	(hs_function_definition*)&object_get_ai_1_definition,
	(hs_function_definition*)&ai_rotate_scenario_4_definition,
	(hs_function_definition*)&ai_translate_scenario_3_definition,
	(hs_function_definition*)&scenario_duplicate_bsp_objects_2_definition,
	(hs_function_definition*)&scenario_duplicate_bsp_objects_3_definition,
	(hs_function_definition*)&scenario_rotate_4_definition,
	(hs_function_definition*)&scenario_reflect_4_definition,
	(hs_function_definition*)&scenario_translate_3_definition,
	(hs_function_definition*)&scenario_rotate_6_definition,
	(hs_function_definition*)&scenario_reflect_6_definition,
	(hs_function_definition*)&scenario_translate_5_definition,
	(hs_function_definition*)&ai_set_task_3_definition,
	(hs_function_definition*)&ai_set_objective_2_definition,
	(hs_function_definition*)&ai_task_status_1_definition,
	(hs_function_definition*)&ai_set_task_condition_2_definition,
	(hs_function_definition*)&ai_leadership_1_definition,
	(hs_function_definition*)&ai_leadership_all_1_definition,
	(hs_function_definition*)&ai_task_count_1_definition,
	(hs_function_definition*)&ai_reset_objective_1_definition,
	(hs_function_definition*)&ai_squad_patrol_objective_disallow_2_definition,
	(hs_function_definition*)&generate_pathfinding_0_definition,
	(hs_function_definition*)&ai_render_paths_all_0_definition,
	(hs_function_definition*)&ai_activity_set_2_definition,
	(hs_function_definition*)&ai_activity_abort_1_definition,
	(hs_function_definition*)&ai_vehicle_get_1_definition,
	(hs_function_definition*)&ai_vehicle_get_from_starting_location_1_definition,
	(hs_function_definition*)&ai_vehicle_get_from_spawn_point_1_definition,
	(hs_function_definition*)&ai_vehicle_get_squad_count_1_definition,
	(hs_function_definition*)&ai_vehicle_get_from_squad_2_definition,
	(hs_function_definition*)&ai_vehicle_reserve_seat_3_definition,
	(hs_function_definition*)&ai_vehicle_reserve_2_definition,
	(hs_function_definition*)&ai_player_get_vehicle_squad_1_definition,
	(hs_function_definition*)&ai_vehicle_count_1_definition,
	(hs_function_definition*)&ai_carrying_player_1_definition,
	(hs_function_definition*)&player_in_vehicle_1_definition,
	(hs_function_definition*)&ai_player_needs_vehicle_1_definition,
	(hs_function_definition*)&ai_player_any_needs_vehicle_0_definition,
	(hs_function_definition*)&ai_vehicle_enter_3_definition,
	(hs_function_definition*)&ai_vehicle_enter_2_definition,
	(hs_function_definition*)&ai_vehicle_enter_immediate_3_definition,
	(hs_function_definition*)&ai_vehicle_enter_immediate_2_definition,
	(hs_function_definition*)&ai_enter_squad_vehicles_1_definition,
	(hs_function_definition*)&ai_vehicle_exit_2_definition,
	(hs_function_definition*)&ai_vehicle_exit_1_definition,
	(hs_function_definition*)&vehicle_overturned_1_definition,
	(hs_function_definition*)&vehicle_flip_1_definition,
	(hs_function_definition*)&ai_combat_status_1_definition,
	(hs_function_definition*)&flock_start_1_definition,
	(hs_function_definition*)&flock_stop_1_definition,
	(hs_function_definition*)&flock_create_1_definition,
	(hs_function_definition*)&flock_delete_1_definition,
	(hs_function_definition*)&flock_definition_set_2_definition,
	(hs_function_definition*)&flock_unperch_1_definition,
	(hs_function_definition*)&drop_ai_2_definition,
	(hs_function_definition*)&ai_verify_tags_0_definition,
	(hs_function_definition*)&ai_wall_lean_1_definition,
	(hs_function_definition*)&ai_play_line_2_definition,
	(hs_function_definition*)&ai_play_line_at_player_2_definition,
	(hs_function_definition*)&ai_play_line_on_object_2_definition,
	(hs_function_definition*)&ai_play_line_on_object_for_team_3_definition,
	(hs_function_definition*)&ai_play_line_on_point_set_3_definition,
	(hs_function_definition*)&ai_play_line_on_point_set_4_definition,
	(hs_function_definition*)&campaign_metagame_time_pause_1_definition,
	(hs_function_definition*)&campaign_metagame_award_points_2_definition,
	(hs_function_definition*)&campaign_metagame_award_primary_skull_2_definition,
	(hs_function_definition*)&campaign_metagame_award_secondary_skull_2_definition,
	(hs_function_definition*)&campaign_metagame_award_event_2_definition,
	(hs_function_definition*)&campaign_metagame_enabled_0_definition,
	(hs_function_definition*)&campaign_survival_enabled_0_definition,
	(hs_function_definition*)&campaign_is_finished_easy_0_definition,
	(hs_function_definition*)&campaign_is_finished_normal_0_definition,
	(hs_function_definition*)&campaign_is_finished_heroic_0_definition,
	(hs_function_definition*)&campaign_is_finished_legendary_0_definition,
	(hs_function_definition*)&cs_run_command_script_2_definition,
	(hs_function_definition*)&cs_queue_command_script_2_definition,
	(hs_function_definition*)&cs_stack_command_script_2_definition,
	(hs_function_definition*)&vs_reserve_2_definition,
	(hs_function_definition*)&vs_reserve_3_definition,
	(hs_function_definition*)&vs_cast_4_definition,
	(hs_function_definition*)&vs_cast_5_definition,
	(hs_function_definition*)&vs_cast_6_definition,
	(hs_function_definition*)&vs_cast_7_definition,
	(hs_function_definition*)&vs_cast_8_definition,
	(hs_function_definition*)&vs_cast_9_definition,
	(hs_function_definition*)&vs_cast_10_definition,
	(hs_function_definition*)&vs_role_1_definition,
	(hs_function_definition*)&vs_abort_on_alert_1_definition,
	(hs_function_definition*)&vs_abort_on_damage_1_definition,
	(hs_function_definition*)&vs_abort_on_combat_status_1_definition,
	(hs_function_definition*)&vs_abort_on_vehicle_exit_1_definition,
	(hs_function_definition*)&cs_abort_on_alert_thread_1_definition,
	(hs_function_definition*)&cs_abort_on_damage_thread_1_definition,
	(hs_function_definition*)&cs_abort_on_combat_status_thread_1_definition,
	(hs_function_definition*)&cs_abort_on_vehicle_exit_thread_1_definition,
	(hs_function_definition*)&cs_abort_on_alert_1_definition,
	(hs_function_definition*)&vs_abort_on_alert_2_definition,
	(hs_function_definition*)&cs_abort_on_damage_1_definition,
	(hs_function_definition*)&vs_abort_on_damage_2_definition,
	(hs_function_definition*)&cs_abort_on_combat_status_1_definition,
	(hs_function_definition*)&vs_abort_on_combat_status_2_definition,
	(hs_function_definition*)&cs_abort_on_vehicle_exit_1_definition,
	(hs_function_definition*)&vs_abort_on_vehicle_exit_2_definition,
	(hs_function_definition*)&vs_set_cleanup_script_1_definition,
	(hs_function_definition*)&vs_release_1_definition,
	(hs_function_definition*)&vs_release_all_0_definition,
	(hs_function_definition*)&cs_command_script_running_2_definition,
	(hs_function_definition*)&cs_command_script_queued_2_definition,
	(hs_function_definition*)&cs_number_queued_1_definition,
	(hs_function_definition*)&cs_moving_0_definition,
	(hs_function_definition*)&cs_moving_1_definition,
	(hs_function_definition*)&vs_running_atom_1_definition,
	(hs_function_definition*)&vs_running_atom_movement_1_definition,
	(hs_function_definition*)&vs_running_atom_action_1_definition,
	(hs_function_definition*)&vs_running_atom_dialogue_1_definition,
	(hs_function_definition*)&cs_fly_to_1_definition,
	(hs_function_definition*)&vs_fly_to_3_definition,
	(hs_function_definition*)&cs_fly_to_2_definition,
	(hs_function_definition*)&vs_fly_to_4_definition,
	(hs_function_definition*)&cs_fly_to_and_face_2_definition,
	(hs_function_definition*)&vs_fly_to_and_face_4_definition,
	(hs_function_definition*)&cs_fly_to_and_face_3_definition,
	(hs_function_definition*)&vs_fly_to_and_face_5_definition,
	(hs_function_definition*)&cs_fly_by_1_definition,
	(hs_function_definition*)&vs_fly_by_3_definition,
	(hs_function_definition*)&cs_fly_by_2_definition,
	(hs_function_definition*)&vs_fly_by_4_definition,
	(hs_function_definition*)&cs_go_to_1_definition,
	(hs_function_definition*)&vs_go_to_3_definition,
	(hs_function_definition*)&cs_go_to_2_definition,
	(hs_function_definition*)&vs_go_to_4_definition,
	(hs_function_definition*)&cs_go_by_2_definition,
	(hs_function_definition*)&vs_go_by_4_definition,
	(hs_function_definition*)&cs_go_by_3_definition,
	(hs_function_definition*)&vs_go_by_5_definition,
	(hs_function_definition*)&cs_go_to_and_face_2_definition,
	(hs_function_definition*)&vs_go_to_and_face_4_definition,
	(hs_function_definition*)&cs_go_to_and_posture_2_definition,
	(hs_function_definition*)&vs_go_to_and_posture_4_definition,
	(hs_function_definition*)&cs_go_to_nearest_1_definition,
	(hs_function_definition*)&vs_go_to_nearest_3_definition,
	(hs_function_definition*)&cs_move_in_direction_3_definition,
	(hs_function_definition*)&vs_move_in_direction_5_definition,
	(hs_function_definition*)&cs_move_towards_1_definition,
	(hs_function_definition*)&vs_move_towards_3_definition,
	(hs_function_definition*)&cs_move_towards_2_definition,
	(hs_function_definition*)&vs_move_towards_4_definition,
	(hs_function_definition*)&cs_move_towards_point_2_definition,
	(hs_function_definition*)&vs_move_towards_point_4_definition,
	(hs_function_definition*)&cs_swarm_to_2_definition,
	(hs_function_definition*)&vs_swarm_to_4_definition,
	(hs_function_definition*)&cs_swarm_from_2_definition,
	(hs_function_definition*)&vs_swarm_from_4_definition,
	(hs_function_definition*)&cs_pause_1_definition,
	(hs_function_definition*)&vs_pause_3_definition,
	(hs_function_definition*)&cs_grenade_2_definition,
	(hs_function_definition*)&vs_grenade_4_definition,
	(hs_function_definition*)&cs_equipment_1_definition,
	(hs_function_definition*)&vs_equipment_3_definition,
	(hs_function_definition*)&cs_jump_2_definition,
	(hs_function_definition*)&vs_jump_4_definition,
	(hs_function_definition*)&cs_jump_to_point_2_definition,
	(hs_function_definition*)&vs_jump_to_point_4_definition,
	(hs_function_definition*)&cs_vocalize_1_definition,
	(hs_function_definition*)&vs_vocalize_3_definition,
	(hs_function_definition*)&cs_play_sound_1_definition,
	(hs_function_definition*)&vs_play_sound_3_definition,
	(hs_function_definition*)&cs_play_sound_2_definition,
	(hs_function_definition*)&vs_play_sound_4_definition,
	(hs_function_definition*)&cs_play_sound_3_definition,
	(hs_function_definition*)&vs_play_sound_5_definition,
	(hs_function_definition*)&cs_action_2_definition,
	(hs_function_definition*)&vs_action_4_definition,
	(hs_function_definition*)&cs_action_at_object_2_definition,
	(hs_function_definition*)&vs_action_at_object_4_definition,
	(hs_function_definition*)&cs_action_at_player_1_definition,
	(hs_function_definition*)&vs_action_at_player_3_definition,
	(hs_function_definition*)&cs_custom_animation_3_definition,
	(hs_function_definition*)&vs_custom_animation_5_definition,
	(hs_function_definition*)&cs_custom_animation_4_definition,
	(hs_function_definition*)&vs_custom_animation_6_definition,
	(hs_function_definition*)&cs_custom_animation_death_3_definition,
	(hs_function_definition*)&vs_custom_animation_death_5_definition,
	(hs_function_definition*)&cs_custom_animation_death_4_definition,
	(hs_function_definition*)&vs_custom_animation_death_6_definition,
	(hs_function_definition*)&cs_custom_animation_loop_3_definition,
	(hs_function_definition*)&vs_custom_animation_loop_4_definition,
	(hs_function_definition*)&cs_custom_animation_loop_4_definition,
	(hs_function_definition*)&vs_custom_animation_loop_5_definition,
	(hs_function_definition*)&cs_play_line_1_definition,
	(hs_function_definition*)&vs_play_line_3_definition,
	(hs_function_definition*)&cs_die_1_definition,
	(hs_function_definition*)&vs_die_3_definition,
	(hs_function_definition*)&cs_deploy_turret_1_definition,
	(hs_function_definition*)&vs_deploy_turret_3_definition,
	(hs_function_definition*)&cs_approach_4_definition,
	(hs_function_definition*)&vs_approach_6_definition,
	(hs_function_definition*)&cs_approach_player_3_definition,
	(hs_function_definition*)&vs_approach_player_5_definition,
	(hs_function_definition*)&cs_go_to_vehicle_1_definition,
	(hs_function_definition*)&vs_go_to_vehicle_3_definition,
	(hs_function_definition*)&cs_go_to_vehicle_2_definition,
	(hs_function_definition*)&vs_go_to_vehicle_4_definition,
	(hs_function_definition*)&cs_set_style_1_definition,
	(hs_function_definition*)&vs_set_style_2_definition,
	(hs_function_definition*)&cs_force_combat_status_1_definition,
	(hs_function_definition*)&vs_force_combat_status_2_definition,
	(hs_function_definition*)&cs_enable_targeting_1_definition,
	(hs_function_definition*)&vs_enable_targeting_2_definition,
	(hs_function_definition*)&cs_enable_looking_1_definition,
	(hs_function_definition*)&vs_enable_looking_2_definition,
	(hs_function_definition*)&cs_enable_moving_1_definition,
	(hs_function_definition*)&vs_enable_moving_2_definition,
	(hs_function_definition*)&cs_enable_dialogue_1_definition,
	(hs_function_definition*)&vs_enable_dialogue_2_definition,
	(hs_function_definition*)&cs_suppress_activity_termination_1_definition,
	(hs_function_definition*)&vs_suppress_activity_termination_2_definition,
	(hs_function_definition*)&cs_suppress_dialogue_global_1_definition,
	(hs_function_definition*)&vs_suppress_dialogue_global_2_definition,
	(hs_function_definition*)&cs_look_2_definition,
	(hs_function_definition*)&vs_look_3_definition,
	(hs_function_definition*)&cs_look_player_1_definition,
	(hs_function_definition*)&vs_look_player_2_definition,
	(hs_function_definition*)&cs_look_object_2_definition,
	(hs_function_definition*)&vs_look_object_3_definition,
	(hs_function_definition*)&cs_aim_2_definition,
	(hs_function_definition*)&vs_aim_3_definition,
	(hs_function_definition*)&cs_aim_player_1_definition,
	(hs_function_definition*)&vs_aim_player_2_definition,
	(hs_function_definition*)&cs_aim_object_2_definition,
	(hs_function_definition*)&vs_aim_object_3_definition,
	(hs_function_definition*)&cs_face_2_definition,
	(hs_function_definition*)&vs_face_3_definition,
	(hs_function_definition*)&cs_face_player_1_definition,
	(hs_function_definition*)&vs_face_player_2_definition,
	(hs_function_definition*)&cs_face_object_2_definition,
	(hs_function_definition*)&vs_face_object_3_definition,
	(hs_function_definition*)&cs_shoot_1_definition,
	(hs_function_definition*)&vs_shoot_2_definition,
	(hs_function_definition*)&cs_shoot_2_definition,
	(hs_function_definition*)&vs_shoot_3_definition,
	(hs_function_definition*)&cs_shoot_point_2_definition,
	(hs_function_definition*)&vs_shoot_point_3_definition,
	(hs_function_definition*)&cs_shoot_secondary_trigger_1_definition,
	(hs_function_definition*)&vs_shoot_secondary_trigger_2_definition,
	(hs_function_definition*)&cs_lower_weapon_1_definition,
	(hs_function_definition*)&vs_lower_weapon_2_definition,
	(hs_function_definition*)&cs_vehicle_speed_1_definition,
	(hs_function_definition*)&vs_vehicle_speed_2_definition,
	(hs_function_definition*)&cs_vehicle_speed_instantaneous_1_definition,
	(hs_function_definition*)&vs_vehicle_speed_instantaneous_2_definition,
	(hs_function_definition*)&cs_vehicle_boost_1_definition,
	(hs_function_definition*)&vs_vehicle_boost_2_definition,
	(hs_function_definition*)&cs_turn_sharpness_2_definition,
	(hs_function_definition*)&vs_turn_sharpness_3_definition,
	(hs_function_definition*)&cs_enable_pathfinding_failsafe_1_definition,
	(hs_function_definition*)&vs_enable_pathfinding_failsafe_2_definition,
	(hs_function_definition*)&cs_set_pathfinding_radius_1_definition,
	(hs_function_definition*)&vs_set_pathfinding_radius_2_definition,
	(hs_function_definition*)&cs_ignore_obstacles_1_definition,
	(hs_function_definition*)&vs_ignore_obstacles_2_definition,
	(hs_function_definition*)&cs_approach_stop_0_definition,
	(hs_function_definition*)&vs_approach_stop_1_definition,
	(hs_function_definition*)&cs_movement_mode_1_definition,
	(hs_function_definition*)&vs_movement_mode_2_definition,
	(hs_function_definition*)&cs_crouch_1_definition,
	(hs_function_definition*)&vs_crouch_2_definition,
	(hs_function_definition*)&cs_crouch_2_definition,
	(hs_function_definition*)&vs_crouch_3_definition,
	(hs_function_definition*)&cs_walk_1_definition,
	(hs_function_definition*)&vs_walk_2_definition,
	(hs_function_definition*)&cs_posture_set_2_definition,
	(hs_function_definition*)&vs_posture_set_3_definition,
	(hs_function_definition*)&cs_posture_exit_0_definition,
	(hs_function_definition*)&vs_posture_exit_1_definition,
	(hs_function_definition*)&cs_stow_0_definition,
	(hs_function_definition*)&vs_stow_1_definition,
	(hs_function_definition*)&cs_draw_0_definition,
	(hs_function_definition*)&vs_draw_1_definition,
	(hs_function_definition*)&cs_teleport_2_definition,
	(hs_function_definition*)&vs_teleport_3_definition,
	(hs_function_definition*)&cs_stop_custom_animation_0_definition,
	(hs_function_definition*)&vs_stop_custom_animation_1_definition,
	(hs_function_definition*)&cs_stop_sound_1_definition,
	(hs_function_definition*)&vs_stop_sound_2_definition,
	(hs_function_definition*)&cs_player_melee_1_definition,
	(hs_function_definition*)&vs_player_melee_3_definition,
	(hs_function_definition*)&cs_melee_direction_1_definition,
	(hs_function_definition*)&vs_melee_direction_3_definition,
	(hs_function_definition*)&cs_smash_direction_1_definition,
	(hs_function_definition*)&vs_smash_direction_3_definition,
	(hs_function_definition*)&camera_control_1_definition,
	(hs_function_definition*)&camera_set_2_definition,
	(hs_function_definition*)&camera_set_relative_3_definition,
	(hs_function_definition*)&camera_set_animation_2_definition,
	(hs_function_definition*)&camera_set_animation_relative_4_definition,
	(hs_function_definition*)&camera_set_animation_with_speed_3_definition,
	(hs_function_definition*)&camera_set_animation_relative_with_speed_5_definition,
	(hs_function_definition*)&camera_set_animation_relative_with_speed_loop_6_definition,
	(hs_function_definition*)&camera_set_animation_relative_with_speed_loop_offset_7_definition,
	(hs_function_definition*)&camera_predict_resources_at_frame_5_definition,
	(hs_function_definition*)&camera_predict_resources_at_point_1_definition,
	(hs_function_definition*)&camera_set_first_person_1_definition,
	(hs_function_definition*)&camera_set_cinematic_0_definition,
	(hs_function_definition*)&camera_set_cinematic_scene_3_definition,
	(hs_function_definition*)&camera_place_relative_1_definition,
	(hs_function_definition*)&camera_place_worldspace_0_definition,
	(hs_function_definition*)&camera_time_0_definition,
	(hs_function_definition*)&camera_set_field_of_view_2_definition,
	(hs_function_definition*)&cinematic_camera_set_easing_in_6_definition,
	(hs_function_definition*)&cinematic_camera_set_easing_out_1_definition,
	(hs_function_definition*)&cinematic_print_1_definition,
	(hs_function_definition*)&camera_set_pan_2_definition,
	(hs_function_definition*)&camera_pan_7_definition,
	(hs_function_definition*)&debug_camera_save_0_definition,
	(hs_function_definition*)&debug_camera_load_0_definition,
	(hs_function_definition*)&debug_camera_save_name_1_definition,
	(hs_function_definition*)&debug_camera_load_name_1_definition,
	(hs_function_definition*)&director_debug_camera_1_definition,
	(hs_function_definition*)&game_difficulty_get_0_definition,
	(hs_function_definition*)&game_difficulty_get_real_0_definition,
	(hs_function_definition*)&game_insertion_point_get_0_definition,
	(hs_function_definition*)&game_insertion_point_set_1_definition,
	(hs_function_definition*)&pvs_set_object_1_definition,
	(hs_function_definition*)&pvs_set_camera_1_definition,
	(hs_function_definition*)&pvs_clear_0_definition,
	(hs_function_definition*)&pvs_reset_0_definition,
	(hs_function_definition*)&players_unzoom_all_0_definition,
	(hs_function_definition*)&player_enable_input_1_definition,
	(hs_function_definition*)&player_disable_movement_1_definition,
	(hs_function_definition*)&player_disable_weapon_pickup_1_definition,
	(hs_function_definition*)&player_disable_sprint_1_definition,
	(hs_function_definition*)&player_disable_equipment_use_1_definition,
	(hs_function_definition*)&player_flashlight_on_0_definition,
	(hs_function_definition*)&player_active_camouflage_on_0_definition,
	(hs_function_definition*)&player_camera_control_1_definition,
	(hs_function_definition*)&player_action_test_reset_0_definition,
	(hs_function_definition*)&player_action_test_jump_0_definition,
	(hs_function_definition*)&player_action_test_primary_trigger_0_definition,
	(hs_function_definition*)&player_action_test_grenade_trigger_0_definition,
	(hs_function_definition*)&player_action_test_vision_trigger_0_definition,
	(hs_function_definition*)&player_action_test_rotate_weapons_0_definition,
	(hs_function_definition*)&player_action_test_rotate_grenades_0_definition,
	(hs_function_definition*)&player_action_test_melee_0_definition,
	(hs_function_definition*)&player_action_test_action_0_definition,
	(hs_function_definition*)&player_action_test_accept_0_definition,
	(hs_function_definition*)&player_action_test_cancel_0_definition,
	(hs_function_definition*)&player_action_test_look_relative_up_0_definition,
	(hs_function_definition*)&player_action_test_look_relative_down_0_definition,
	(hs_function_definition*)&player_action_test_look_relative_left_0_definition,
	(hs_function_definition*)&player_action_test_look_relative_right_0_definition,
	(hs_function_definition*)&player_action_test_look_relative_all_directions_0_definition,
	(hs_function_definition*)&player_action_test_move_relative_all_directions_0_definition,
	(hs_function_definition*)&player_action_test_back_0_definition,
	(hs_function_definition*)&player_action_test_dpad_left_0_definition,
	(hs_function_definition*)&player_action_test_dpad_right_0_definition,
	(hs_function_definition*)&player_action_test_dpad_up_0_definition,
	(hs_function_definition*)&player_action_test_dpad_down_0_definition,
	(hs_function_definition*)&player_action_test_x_0_definition,
	(hs_function_definition*)&player_action_test_y_0_definition,
	(hs_function_definition*)&player_action_test_left_shoulder_0_definition,
	(hs_function_definition*)&player_action_test_right_shoulder_0_definition,
	(hs_function_definition*)&player_action_test_waypoint_activate_1_definition,
	(hs_function_definition*)&unit_action_test_reset_1_definition,
	(hs_function_definition*)&unit_action_test_primary_trigger_1_definition,
	(hs_function_definition*)&unit_action_test_vision_trigger_1_definition,
	(hs_function_definition*)&unit_action_test_rotate_weapons_1_definition,
	(hs_function_definition*)&unit_action_test_rotate_grenades_1_definition,
	(hs_function_definition*)&unit_action_test_melee_1_definition,
	(hs_function_definition*)&unit_action_test_action_1_definition,
	(hs_function_definition*)&unit_action_test_accept_1_definition,
	(hs_function_definition*)&unit_action_test_cancel_1_definition,
	(hs_function_definition*)&unit_action_test_look_relative_up_1_definition,
	(hs_function_definition*)&unit_action_test_look_relative_down_1_definition,
	(hs_function_definition*)&unit_action_test_look_relative_left_1_definition,
	(hs_function_definition*)&unit_action_test_look_relative_right_1_definition,
	(hs_function_definition*)&unit_action_test_look_relative_all_directions_1_definition,
	(hs_function_definition*)&unit_action_test_move_relative_all_directions_1_definition,
	(hs_function_definition*)&unit_action_test_back_1_definition,
	(hs_function_definition*)&unit_action_test_dpad_left_1_definition,
	(hs_function_definition*)&unit_action_test_dpad_right_1_definition,
	(hs_function_definition*)&unit_action_test_dpad_up_1_definition,
	(hs_function_definition*)&unit_action_test_dpad_down_1_definition,
	(hs_function_definition*)&unit_action_test_x_1_definition,
	(hs_function_definition*)&unit_action_test_y_1_definition,
	(hs_function_definition*)&unit_action_test_left_shoulder_1_definition,
	(hs_function_definition*)&unit_action_test_right_shoulder_1_definition,
	(hs_function_definition*)&player0_looking_up_0_definition,
	(hs_function_definition*)&player0_looking_down_0_definition,
	(hs_function_definition*)&player0_set_pitch_2_definition,
	(hs_function_definition*)&player1_set_pitch_2_definition,
	(hs_function_definition*)&player2_set_pitch_2_definition,
	(hs_function_definition*)&player3_set_pitch_2_definition,
	(hs_function_definition*)&player_action_test_lookstick_forward_0_definition,
	(hs_function_definition*)&player_action_test_lookstick_backward_0_definition,
	(hs_function_definition*)&debug_teleport_player_2_definition,
	(hs_function_definition*)&map_reset_0_definition,
	(hs_function_definition*)&map_reset_random_0_definition,
	(hs_function_definition*)&switch_bsp_1_definition,
	(hs_function_definition*)&switch_zone_set_1_definition,
	(hs_function_definition*)&current_zone_set_0_definition,
	(hs_function_definition*)&current_zone_set_fully_active_0_definition,
	(hs_function_definition*)&switch_map_and_zone_set_2_definition,
	(hs_function_definition*)&crash_1_definition,
	(hs_function_definition*)&version_0_definition,
	(hs_function_definition*)&status_0_definition,
	(hs_function_definition*)&record_movie_4_definition,
	(hs_function_definition*)&record_movie_distributed_5_definition,
	(hs_function_definition*)&screenshot_1_definition,
	(hs_function_definition*)&screenshot_debug_0_definition,
	(hs_function_definition*)&screenshot_big_2_definition,
	(hs_function_definition*)&screenshot_big_raw_2_definition,
	(hs_function_definition*)&screenshot_size_3_definition,
	(hs_function_definition*)&screenshot_simple_1_definition,
	(hs_function_definition*)&screenshot_cubemap_1_definition,
	(hs_function_definition*)&screenshot_webmap_1_definition,
	(hs_function_definition*)&screenshot_unknown344_0_definition,
	(hs_function_definition*)&screenshot_capture_0_definition,
	(hs_function_definition*)&screenshot_set_path_1_definition,
	(hs_function_definition*)&screenshot_unknown347_5_definition,
	(hs_function_definition*)&main_menu_0_definition,
	(hs_function_definition*)&main_halt_0_definition,
	(hs_function_definition*)&map_name_1_definition,
	(hs_function_definition*)&game_multiplayer_1_definition,
	(hs_function_definition*)&game_set_variant_1_definition,
	(hs_function_definition*)&game_splitscreen_1_definition,
	(hs_function_definition*)&game_difficulty_1_definition,
	(hs_function_definition*)&game_active_primary_skulls_1_definition,
	(hs_function_definition*)&game_active_secondary_skulls_1_definition,
	(hs_function_definition*)&game_coop_players_1_definition,
	(hs_function_definition*)&game_initial_zone_set_1_definition,
	(hs_function_definition*)&game_tick_rate_1_definition,
	(hs_function_definition*)&game_start_1_definition,
	(hs_function_definition*)&game_start_when_ready_0_definition,
	(hs_function_definition*)&game_start_when_joined_1_definition,
	(hs_function_definition*)&game_rate_3_definition,
	(hs_function_definition*)&texture_cache_flush_0_definition,
	(hs_function_definition*)&geometry_cache_flush_0_definition,
	(hs_function_definition*)&sound_cache_flush_0_definition,
	(hs_function_definition*)&animation_cache_flush_0_definition,
	(hs_function_definition*)&font_cache_flush_0_definition,
	(hs_function_definition*)&language_set_1_definition,
	(hs_function_definition*)&texture_cache_test_malloc_0_definition,
	(hs_function_definition*)&debug_memory_0_definition,
	(hs_function_definition*)&debug_memory_by_file_0_definition,
	(hs_function_definition*)&debug_memory_for_file_1_definition,
	(hs_function_definition*)&debug_tags_0_definition,
	(hs_function_definition*)&tags_verify_all_0_definition,
	(hs_function_definition*)&profiler_enable_1_definition,
	(hs_function_definition*)&profiler_set_thread_1_definition,
	(hs_function_definition*)&profiler_set_sort_method_1_definition,
	(hs_function_definition*)&profiler_set_range_1_definition,
	(hs_function_definition*)&profiler_set_attribute_1_definition,
	(hs_function_definition*)&trace_next_frame_1_definition,
	(hs_function_definition*)&trace_next_frame_to_file_2_definition,
	(hs_function_definition*)&trace_tick_1_definition,
	(hs_function_definition*)&collision_log_enable_1_definition,
	(hs_function_definition*)&damage_control_get_1_definition,
	(hs_function_definition*)&damage_control_set_2_definition,
	(hs_function_definition*)&ai_lines_0_definition,
	(hs_function_definition*)&ai_dialogue_break_on_vocalization_1_definition,
	(hs_function_definition*)&fade_in_4_definition,
	(hs_function_definition*)&fade_out_4_definition,
	(hs_function_definition*)&cinematic_start_0_definition,
	(hs_function_definition*)&cinematic_stop_0_definition,
	(hs_function_definition*)&cinematic_skip_start_internal_0_definition,
	(hs_function_definition*)&cinematic_skip_stop_internal_0_definition,
	(hs_function_definition*)&cinematic_show_letterbox_1_definition,
	(hs_function_definition*)&cinematic_show_letterbox_immediate_1_definition,
	(hs_function_definition*)&cinematic_set_title_1_definition,
	(hs_function_definition*)&cinematic_set_title_delayed_2_definition,
	(hs_function_definition*)&cinematic_suppress_bsp_object_creation_1_definition,
	(hs_function_definition*)&cinematic_subtitle_2_definition,
	(hs_function_definition*)&cinematic_set_1_definition,
	(hs_function_definition*)&cinematic_set_shot_2_definition,
	(hs_function_definition*)&cinematic_get_shot_0_definition,
	(hs_function_definition*)&cinematic_set_early_exit_1_definition,
	(hs_function_definition*)&cinematic_get_early_exit_0_definition,
	(hs_function_definition*)&cinematic_set_active_camera_1_definition,
	(hs_function_definition*)&cinematic_object_create_1_definition,
	(hs_function_definition*)&cinematic_object_create_cinematic_anchor_2_definition,
	(hs_function_definition*)&cinematic_object_destroy_1_definition,
	(hs_function_definition*)&cinematic_destroy_0_definition,
	(hs_function_definition*)&cinematic_clips_initialize_for_shot_1_definition,
	(hs_function_definition*)&cinematic_clips_destroy_0_definition,
	(hs_function_definition*)&cinematic_lights_initialize_for_shot_1_definition,
	(hs_function_definition*)&cinematic_lights_destroy_0_definition,
	(hs_function_definition*)&cinematic_lights_destroy_shot_0_definition,
	(hs_function_definition*)&cinematic_light_object_4_definition,
	(hs_function_definition*)&cinematic_light_object_off_1_definition,
	(hs_function_definition*)&cinematic_lighting_rebuild_all_0_definition,
	(hs_function_definition*)&cinematic_lighting_update_dynamic_light_direction_5_definition,
	(hs_function_definition*)&cinematic_lighting_update_sh_light_8_definition,
	(hs_function_definition*)&cinematic_object_get_1_definition,
	(hs_function_definition*)&cinematic_reset_0_definition,
	(hs_function_definition*)&camera_set_briefing_1_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_cinematic_1_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_scene_2_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_effect_4_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_dialogue_4_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_music_4_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_music_looping_4_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_animation_3_definition,
	(hs_function_definition*)&cinematic_scripting_object_coop_flags_valid_3_definition,
	(hs_function_definition*)&cinematic_scripting_fade_out_4_definition,
	(hs_function_definition*)&cinematic_scripting_create_object_3_definition,
	(hs_function_definition*)&cinematic_scripting_create_cinematic_object_3_definition,
	(hs_function_definition*)&cinematic_scripting_start_animation_4_definition,
	(hs_function_definition*)&cinematic_scripting_destroy_object_3_definition,
	(hs_function_definition*)&cinematic_scripting_start_effect_5_definition,
	(hs_function_definition*)&cinematic_scripting_start_music_4_definition,
	(hs_function_definition*)&cinematic_scripting_start_dialogue_5_definition,
	(hs_function_definition*)&cinematic_scripting_stop_music_4_definition,
	(hs_function_definition*)&cinematic_scripting_create_and_animate_object_5_definition,
	(hs_function_definition*)&cinematic_scripting_create_and_animate_cinematic_object_5_definition,
	(hs_function_definition*)&cinematic_scripting_create_and_animate_object_no_animation_4_definition,
	(hs_function_definition*)&cinematic_scripting_create_and_animate_cinematic_object_no_animation_4_definition,
	(hs_function_definition*)&cinematic_scripting_play_cortana_effect_4_definition,
	(hs_function_definition*)&attract_mode_start_0_definition,
	(hs_function_definition*)&attract_mode_set_seconds_1_definition,
	(hs_function_definition*)&game_level_advance_1_definition,
	(hs_function_definition*)&game_won_0_definition,
	(hs_function_definition*)&game_lost_1_definition,
	(hs_function_definition*)&game_revert_0_definition,
	(hs_function_definition*)&game_is_cooperative_0_definition,
	(hs_function_definition*)&game_is_playtest_0_definition,
	(hs_function_definition*)&game_can_use_flashlights_1_definition,
	(hs_function_definition*)&game_save_and_quit_0_definition,
	(hs_function_definition*)&game_save_unsafe_0_definition,
	(hs_function_definition*)&game_insertion_point_unlock_1_definition,
	(hs_function_definition*)&game_insertion_point_lock_1_definition,
	(hs_function_definition*)&saved_games_enumerate_1_definition,
	(hs_function_definition*)&saved_games_delete_campaign_save_1_definition,
	(hs_function_definition*)&saved_games_save_last_film_1_definition,
	(hs_function_definition*)&saved_games_autosave_free_up_space_0_definition,
	(hs_function_definition*)&content_catalogue_display_status_3_definition,
	(hs_function_definition*)&achievement_grant_to_player_2_definition,
	(hs_function_definition*)&achievement_grant_to_all_players_1_definition,
	(hs_function_definition*)&achievements_enable_1_definition,
	(hs_function_definition*)&achievements_skip_validation_checks_1_definition,
	(hs_function_definition*)&debug_spawning_influencers_1_definition,
	(hs_function_definition*)&debug_spawning_respawn_zones_1_definition,
	(hs_function_definition*)&debug_spawning_proximity_forbid_1_definition,
	(hs_function_definition*)&debug_spawning_moving_vehicle_1_definition,
	(hs_function_definition*)&debug_spawning_weapon_influences_1_definition,
	(hs_function_definition*)&debug_spawning_dangerous_projectiles_1_definition,
	(hs_function_definition*)&debug_spawning_deployed_equipment_1_definition,
	(hs_function_definition*)&debug_spawning_proximity_enemy_1_definition,
	(hs_function_definition*)&debug_spawning_teammates_1_definition,
	(hs_function_definition*)&debug_spawning_random_influence_1_definition,
	(hs_function_definition*)&debug_spawning_nominal_weight_1_definition,
	(hs_function_definition*)&debug_spawning_natural_weight_1_definition,
	(hs_function_definition*)&debug_spawning_target_1_definition,
	(hs_function_definition*)&debug_spawning_use_history_1_definition,
	(hs_function_definition*)&debug_initial_spawn_point_objects_1_definition,
	(hs_function_definition*)&debug_respawn_point_objects_1_definition,
	(hs_function_definition*)&game_export_variant_settings_1_definition,
	(hs_function_definition*)&game_engine_event_test_general_1_definition,
	(hs_function_definition*)&game_engine_event_test_flavor_1_definition,
	(hs_function_definition*)&game_engine_event_test_slayer_1_definition,
	(hs_function_definition*)&game_engine_event_test_ctf_1_definition,
	(hs_function_definition*)&game_engine_event_test_oddball_1_definition,
	(hs_function_definition*)&game_engine_event_test_king_1_definition,
	(hs_function_definition*)&game_engine_event_test_vip_1_definition,
	(hs_function_definition*)&game_engine_event_test_juggernaut_1_definition,
	(hs_function_definition*)&game_engine_event_test_territories_1_definition,
	(hs_function_definition*)&game_engine_event_test_assault_1_definition,
	(hs_function_definition*)&game_engine_event_test_infection_1_definition,
	(hs_function_definition*)&core_load_0_definition,
	(hs_function_definition*)&core_load_name_1_definition,
	(hs_function_definition*)&core_save_0_definition,
	(hs_function_definition*)&core_save_name_1_definition,
	(hs_function_definition*)&core_load_game_0_definition,
	(hs_function_definition*)&core_load_game_name_1_definition,
	(hs_function_definition*)&core_regular_upload_to_debug_server_1_definition,
	(hs_function_definition*)&core_set_upload_option_1_definition,
	(hs_function_definition*)&force_debugger_not_present_1_definition,
	(hs_function_definition*)&force_debugger_always_present_1_definition,
	(hs_function_definition*)&game_safe_to_save_0_definition,
	(hs_function_definition*)&game_safe_to_speak_0_definition,
	(hs_function_definition*)&game_all_quiet_0_definition,
	(hs_function_definition*)&game_save_0_definition,
	(hs_function_definition*)&game_save_cancel_0_definition,
	(hs_function_definition*)&game_save_no_timeout_0_definition,
	(hs_function_definition*)&game_save_immediate_0_definition,
	(hs_function_definition*)&game_saving_0_definition,
	(hs_function_definition*)&game_reverted_0_definition,
	(hs_function_definition*)&sound_ui_screen_init_0_definition,
	(hs_function_definition*)&sound_ui_screen_term_0_definition,
	(hs_function_definition*)&sound_set_tag_parameter_unsafe_8_definition,
	(hs_function_definition*)&sound_impulse_predict_1_definition,
	(hs_function_definition*)&sound_impulse_trigger_4_definition,
	(hs_function_definition*)&sound_impulse_start_3_definition,
	(hs_function_definition*)&sound_impulse_start_cinematic_5_definition,
	(hs_function_definition*)&sound_impulse_start_effect_4_definition,
	(hs_function_definition*)&sound_impulse_start_with_subtitle_5_definition,
	(hs_function_definition*)&sound_impulse_language_time_1_definition,
	(hs_function_definition*)&sound_impulse_stop_1_definition,
	(hs_function_definition*)&sound_impulse_start_3d_3_definition,
	(hs_function_definition*)&sound_impulse_mark_as_outro_1_definition,
	(hs_function_definition*)&sound_impulse_start_naked_3_definition,
	(hs_function_definition*)&sound_looping_predict_1_definition,
	(hs_function_definition*)&sound_looping_start_3_definition,
	(hs_function_definition*)&sound_looping_stop_1_definition,
	(hs_function_definition*)&sound_looping_stop_immediately_1_definition,
	(hs_function_definition*)&sound_looping_set_scale_2_definition,
	(hs_function_definition*)&sound_looping_set_alternate_2_definition,
	(hs_function_definition*)&sound_loop_spam_0_definition,
	(hs_function_definition*)&sound_class_show_channel_2_definition,
	(hs_function_definition*)&sound_class_debug_sound_start_2_definition,
	(hs_function_definition*)&debug_sounds_enable_2_definition,
	(hs_function_definition*)&sound_class_set_gain_3_definition,
	(hs_function_definition*)&sound_class_set_gain_db_3_definition,
	(hs_function_definition*)&sound_class_enable_ducker_2_definition,
	(hs_function_definition*)&debug_sound_environment_parameter_2_definition,
	(hs_function_definition*)&sound_set_global_effect_2_definition,
	(hs_function_definition*)&sound_set_global_effect_scale_2_definition,
	(hs_function_definition*)&vehicle_auto_turret_5_definition,
	(hs_function_definition*)&vehicle_hover_2_definition,
	(hs_function_definition*)&vehicle_count_bipeds_killed_1_definition,
	(hs_function_definition*)&biped_ragdoll_1_definition,
	(hs_function_definition*)&water_float_reset_1_definition,
	(hs_function_definition*)&hud_show_training_text_1_definition,
	(hs_function_definition*)&hud_set_training_text_1_definition,
	(hs_function_definition*)&hud_enable_training_1_definition,
	(hs_function_definition*)&player_training_activate_flashlight_0_definition,
	(hs_function_definition*)&player_training_activate_crouch_0_definition,
	(hs_function_definition*)&player_training_activate_stealth_0_definition,
	(hs_function_definition*)&player_training_activate_equipment_0_definition,
	(hs_function_definition*)&player_training_activate_jump_0_definition,
	(hs_function_definition*)&player_training_reset_1_definition,
	(hs_function_definition*)&hud_activate_team_nav_point_flag_3_definition,
	(hs_function_definition*)&hud_deactivate_team_nav_point_flag_2_definition,
	(hs_function_definition*)&chud_cortana_suck_3_definition,
	(hs_function_definition*)&chud_texture_cam_1_definition,
	(hs_function_definition*)&chud_cortana_set_range_multiplier_1_definition,
	(hs_function_definition*)&play_cortana_effect_1_definition,
	(hs_function_definition*)&chud_show_1_definition,
	(hs_function_definition*)&chud_show_weapon_stats_1_definition,
	(hs_function_definition*)&chud_show_crosshair_1_definition,
	(hs_function_definition*)&chud_show_shield_1_definition,
	(hs_function_definition*)&chud_show_grenades_1_definition,
	(hs_function_definition*)&chud_show_messages_1_definition,
	(hs_function_definition*)&chud_show_motion_sensor_1_definition,
	(hs_function_definition*)&chud_show_spike_grenades_1_definition,
	(hs_function_definition*)&chud_show_fire_grenades_1_definition,
	(hs_function_definition*)&chud_show_compass_1_definition,
	(hs_function_definition*)&chud_show_stamina_1_definition,
	(hs_function_definition*)&chud_show_energy_meters_1_definition,
	(hs_function_definition*)&chud_show_consumables_1_definition,
	(hs_function_definition*)&chud_cinematic_fade_2_definition,
	(hs_function_definition*)&chud_bonus_round_show_timer_1_definition,
	(hs_function_definition*)&chud_bonus_round_start_timer_1_definition,
	(hs_function_definition*)&chud_bonus_round_set_timer_1_definition,
	(hs_function_definition*)&cls_0_definition,
	(hs_function_definition*)&events_spam_suppression_enable_1_definition,
	(hs_function_definition*)&error_geometry_show_1_definition,
	(hs_function_definition*)&error_geometry_hide_1_definition,
	(hs_function_definition*)&error_geometry_show_all_0_definition,
	(hs_function_definition*)&error_geometry_hide_all_0_definition,
	(hs_function_definition*)&error_geometry_list_0_definition,
	(hs_function_definition*)&player_effect_set_max_translation_3_definition,
	(hs_function_definition*)&player_effect_set_max_rotation_3_definition,
	(hs_function_definition*)&player_effect_set_max_rumble_2_definition,
	(hs_function_definition*)&player_effect_start_2_definition,
	(hs_function_definition*)&player_effect_stop_1_definition,
	(hs_function_definition*)&player_effect_set_max_translation_for_player_4_definition,
	(hs_function_definition*)&player_effect_set_max_rotation_for_player_4_definition,
	(hs_function_definition*)&player_effect_set_max_rumble_for_player_3_definition,
	(hs_function_definition*)&player_effect_start_for_player_3_definition,
	(hs_function_definition*)&player_effect_stop_for_player_2_definition,
	(hs_function_definition*)&time_code_show_1_definition,
	(hs_function_definition*)&time_code_start_1_definition,
	(hs_function_definition*)&time_code_reset_0_definition,
	(hs_function_definition*)&script_screen_effect_set_value_2_definition,
	(hs_function_definition*)&cinematic_screen_effect_start_1_definition,
	(hs_function_definition*)&cinematic_screen_effect_set_crossfade_1_definition,
	(hs_function_definition*)&cinematic_screen_effect_set_crossfade_2_definition,
	(hs_function_definition*)&cinematic_screen_effect_stop_0_definition,
	(hs_function_definition*)&cinematic_set_near_clip_distance_1_definition,
	(hs_function_definition*)&cinematic_set_far_clip_distance_1_definition,
	(hs_function_definition*)&render_atmosphere_fog_1_definition,
	(hs_function_definition*)&atmosphere_fog_override_fade_3_definition,
	(hs_function_definition*)&motion_blur_1_definition,
	(hs_function_definition*)&render_weather_1_definition,
	(hs_function_definition*)&render_patchy_fog_1_definition,
	(hs_function_definition*)&cinematic_set_environment_map_attenuation_3_definition,
	(hs_function_definition*)&cinematic_set_environment_map_bitmap_1_definition,
	(hs_function_definition*)&cinematic_reset_environment_map_bitmap_0_definition,
	(hs_function_definition*)&cinematic_set_environment_map_tint_8_definition,
	(hs_function_definition*)&cinematic_reset_environment_map_tint_0_definition,
	(hs_function_definition*)&cinematic_layer_3_definition,
	(hs_function_definition*)&player_has_skills_0_definition,
	(hs_function_definition*)&player_has_mad_secret_skills_1_definition,
	(hs_function_definition*)&controller_invert_look_0_definition,
	(hs_function_definition*)&controller_look_speed_1_definition,
	(hs_function_definition*)&controller_set_look_invert_1_definition,
	(hs_function_definition*)&controller_get_look_invert_0_definition,
	(hs_function_definition*)&controller_unlock_solo_levels_1_definition,
	(hs_function_definition*)&controller_set_look_inverted_2_definition,
	(hs_function_definition*)&controller_set_vibration_enabled_2_definition,
	(hs_function_definition*)&controller_set_flight_stick_aircraft_controls_2_definition,
	(hs_function_definition*)&controller_set_auto_center_look_2_definition,
	(hs_function_definition*)&controller_set_crouch_lock_2_definition,
	(hs_function_definition*)&controller_set_button_preset_2_definition,
	(hs_function_definition*)&controller_set_joystick_preset_2_definition,
	(hs_function_definition*)&controller_set_look_sensitivity_2_definition,
	(hs_function_definition*)&controller_unlock_single_player_levels_1_definition,
	(hs_function_definition*)&controller_lock_single_player_levels_1_definition,
	(hs_function_definition*)&controller_unlock_skulls_1_definition,
	(hs_function_definition*)&controller_lock_skulls_1_definition,
	(hs_function_definition*)&controller_unlock_models_1_definition,
	(hs_function_definition*)&controller_lock_models_1_definition,
	(hs_function_definition*)&controller_set_single_player_level_completed_5_definition,
	(hs_function_definition*)&controller_set_player_character_type_2_definition,
	(hs_function_definition*)&controller_set_emblem_info_5_definition,
	(hs_function_definition*)&controller_set_voice_output_setting_2_definition,
	(hs_function_definition*)&controller_set_voice_mask_2_definition,
	(hs_function_definition*)&controller_set_subtitle_setting_2_definition,
	(hs_function_definition*)&controller_set_campaign_solo_high_score_3_definition,
	(hs_function_definition*)&controller_set_campaign_coop_high_score_4_definition,
	(hs_function_definition*)&controller_set_survival_solo_high_score_4_definition,
	(hs_function_definition*)&controller_set_survival_coop_high_score_5_definition,
	(hs_function_definition*)&controller_clear_high_scores_1_definition,
	(hs_function_definition*)&controller_set_unsignedin_user_2_definition,
	(hs_function_definition*)&controller_display_storage_device_selection_1_definition,
	(hs_function_definition*)&font_cache_bitmap_save_1_definition,
	(hs_function_definition*)&ui_debug_load_main_menu_0_definition,
	(hs_function_definition*)&ui_debug_text_bounds_1_definition,
	(hs_function_definition*)&ui_debug_text_font_1_definition,
	(hs_function_definition*)&ui_debug_show_title_safe_bounds_1_definition,
	(hs_function_definition*)&ui_debug_element_bounds_1_definition,
	(hs_function_definition*)&ui_memory_dump_1_definition,
	(hs_function_definition*)&ui_time_scale_step_1_definition,
	(hs_function_definition*)&xoverlapped_debug_render_1_definition,
	(hs_function_definition*)&gui_load_screen_1_definition,
	(hs_function_definition*)&gui_reset_0_definition,
	(hs_function_definition*)&gui_start_0_definition,
	(hs_function_definition*)&gui_stop_0_definition,
	(hs_function_definition*)&gui_error_post_3_definition,
	(hs_function_definition*)&gui_error_post_toast_3_definition,
	(hs_function_definition*)&gui_error_resolve_2_definition,
	(hs_function_definition*)&gui_error_clear_2_definition,
	(hs_function_definition*)&gui_dialog_show_1_definition,
	(hs_function_definition*)&gui_spartan_milestone_show_1_definition,
	(hs_function_definition*)&gui_spartan_rank_show_1_definition,
	(hs_function_definition*)&gui_print_active_screens_0_definition,
	(hs_function_definition*)&gui_print_active_screen_strings_0_definition,
	(hs_function_definition*)&gui_debug_screen_name_3_definition,
	(hs_function_definition*)&gui_debug_screen_animation_3_definition,
	(hs_function_definition*)&gui_debug_screen_bounds_3_definition,
	(hs_function_definition*)&gui_debug_screen_rotation_3_definition,
	(hs_function_definition*)&gui_debug_group_name_3_definition,
	(hs_function_definition*)&gui_debug_group_animation_3_definition,
	(hs_function_definition*)&gui_debug_group_bounds_3_definition,
	(hs_function_definition*)&gui_debug_group_rotation_3_definition,
	(hs_function_definition*)&gui_debug_list_name_3_definition,
	(hs_function_definition*)&gui_debug_list_animation_3_definition,
	(hs_function_definition*)&gui_debug_list_bounds_3_definition,
	(hs_function_definition*)&gui_debug_list_rotation_3_definition,
	(hs_function_definition*)&gui_debug_list_item_name_3_definition,
	(hs_function_definition*)&gui_debug_list_item_animation_3_definition,
	(hs_function_definition*)&gui_debug_list_item_bounds_3_definition,
	(hs_function_definition*)&gui_debug_list_item_rotation_3_definition,
	(hs_function_definition*)&gui_debug_text_name_2_definition,
	(hs_function_definition*)&gui_debug_text_animation_2_definition,
	(hs_function_definition*)&gui_debug_text_bounds_2_definition,
	(hs_function_definition*)&gui_debug_text_rotation_2_definition,
	(hs_function_definition*)&gui_debug_bitmap_name_2_definition,
	(hs_function_definition*)&gui_debug_bitmap_animation_2_definition,
	(hs_function_definition*)&gui_debug_bitmap_bounds_2_definition,
	(hs_function_definition*)&gui_debug_bitmap_rotation_2_definition,
	(hs_function_definition*)&gui_debug_music_state_1_definition,
	(hs_function_definition*)&cc_enable_1_definition,
	(hs_function_definition*)&cc_test_1_definition,
	(hs_function_definition*)&objectives_clear_0_definition,
	(hs_function_definition*)&objectives_show_up_to_1_definition,
	(hs_function_definition*)&objectives_finish_up_to_1_definition,
	(hs_function_definition*)&objectives_show_1_definition,
	(hs_function_definition*)&objectives_finish_1_definition,
	(hs_function_definition*)&objectives_unavailable_1_definition,
	(hs_function_definition*)&objectives_secondary_show_1_definition,
	(hs_function_definition*)&objectives_secondary_finish_1_definition,
	(hs_function_definition*)&objectives_secondary_unavailable_1_definition,
	(hs_function_definition*)&input_suppress_rumble_1_definition,
	(hs_function_definition*)&input_disable_claw_button_combos_1_definition,
	(hs_function_definition*)&update_remote_camera_0_definition,
	(hs_function_definition*)&net_build_network_config_0_definition,
	(hs_function_definition*)&net_build_game_variant_1_definition,
	(hs_function_definition*)&net_verify_game_variant_1_definition,
	(hs_function_definition*)&net_load_and_use_game_variant_1_definition,
	(hs_function_definition*)&net_use_hopper_directory_1_definition,
	(hs_function_definition*)&net_quality_dump_0_definition,
	(hs_function_definition*)&net_quality_clear_0_definition,
	(hs_function_definition*)&net_quality_set_connection_badness_history_2_definition,
	(hs_function_definition*)&net_quality_set_squad_host_badness_history_2_definition,
	(hs_function_definition*)&net_quality_set_squad_client_badness_history_2_definition,
	(hs_function_definition*)&net_quality_set_group_host_badness_history_2_definition,
	(hs_function_definition*)&net_quality_set_group_client_badness_history_2_definition,
	(hs_function_definition*)&net_quality_set_estimated_bandwidth_2_definition,
	(hs_function_definition*)&net_join_friend_1_definition,
	(hs_function_definition*)&net_join_squad_to_friend_1_definition,
	(hs_function_definition*)&net_join_sessionid_1_definition,
	(hs_function_definition*)&net_join_squad_to_sessionid_1_definition,
	(hs_function_definition*)&net_enable_join_friend_loop_1_definition,
	(hs_function_definition*)&net_set_maximum_player_count_1_definition,
	(hs_function_definition*)&net_set_campaign_insertion_point_1_definition,
	(hs_function_definition*)&net_status_filter_1_definition,
	(hs_function_definition*)&net_sim_reset_0_definition,
	(hs_function_definition*)&net_sim_spike_now_0_definition,
	(hs_function_definition*)&net_sim_dropspike_now_0_definition,
	(hs_function_definition*)&net_test_ping_0_definition,
	(hs_function_definition*)&net_test_channel_delete_0_definition,
	(hs_function_definition*)&net_test_delegate_host_1_definition,
	(hs_function_definition*)&net_test_delegate_leader_1_definition,
	(hs_function_definition*)&net_test_map_name_1_definition,
	(hs_function_definition*)&net_test_campaign_difficulty_1_definition,
	(hs_function_definition*)&net_test_reset_objects_0_definition,
	(hs_function_definition*)&net_test_fatal_error_0_definition,
	(hs_function_definition*)&net_set_machine_name_1_definition,
	(hs_function_definition*)&events_enabled_1_definition,
	(hs_function_definition*)&events_disable_suppression_1_definition,
	(hs_function_definition*)&event_global_display_category_1_definition,
	(hs_function_definition*)&event_global_log_category_1_definition,
	(hs_function_definition*)&event_global_remote_log_category_1_definition,
	(hs_function_definition*)&event_display_category_2_definition,
	(hs_function_definition*)&event_force_display_category_2_definition,
	(hs_function_definition*)&event_log_category_2_definition,
	(hs_function_definition*)&event_remote_log_category_2_definition,
	(hs_function_definition*)&event_debugger_break_category_2_definition,
	(hs_function_definition*)&event_halt_category_2_definition,
	(hs_function_definition*)&event_list_categories_1_definition,
	(hs_function_definition*)&events_suppress_console_display_1_definition,
	(hs_function_definition*)&play_bink_movie_1_definition,
	(hs_function_definition*)&play_bink_movie_from_tag_1_definition,
	(hs_function_definition*)&play_credits_skip_to_menu_0_definition,
	(hs_function_definition*)&bink_time_0_definition,
	(hs_function_definition*)&set_global_doppler_factor_1_definition,
	(hs_function_definition*)&set_global_mixbin_headroom_2_definition,
	(hs_function_definition*)&debug_sound_environment_source_parameter_2_definition,
	(hs_function_definition*)&data_mine_set_mission_segment_1_definition,
	(hs_function_definition*)&data_mine_insert_1_definition,
	(hs_function_definition*)&data_mine_upload_0_definition,
	(hs_function_definition*)&data_mine_enable_1_definition,
	(hs_function_definition*)&data_mine_flush_0_definition,
	(hs_function_definition*)&data_mine_debug_menu_setting_1_definition,
	(hs_function_definition*)&data_mine_open_debug_menu_0_definition,
	(hs_function_definition*)&data_mine_set_display_mission_segment_1_definition,
	(hs_function_definition*)&unknown4F4_1_definition,
	(hs_function_definition*)&test_memory_allocators_5_definition,
	(hs_function_definition*)&test_memory_allocators_6_definition,
	(hs_function_definition*)&display_video_standard_0_definition,
	(hs_function_definition*)&test_xcr_monkey_enable_1_definition,
	(hs_function_definition*)&test_show_guide_status_0_definition,
	(hs_function_definition*)&test_show_users_xuids_0_definition,
	(hs_function_definition*)&test_show_are_users_friends_2_definition,
	(hs_function_definition*)&test_invite_friend_2_definition,
	(hs_function_definition*)&test_get_squad_session_id_0_definition,
	(hs_function_definition*)&test_auto_get_screens_1_definition,
	(hs_function_definition*)&test_auto_get_screen_widgets_2_definition,
	(hs_function_definition*)&test_auto_screen_get_datasources_2_definition,
	(hs_function_definition*)&test_auto_screen_get_data_columns_3_definition,
	(hs_function_definition*)&test_auto_screen_get_data_4_definition,
	(hs_function_definition*)&test_auto_screen_invoke_list_item_by_name_5_definition,
	(hs_function_definition*)&test_auto_screen_invoke_list_item_by_text_5_definition,
	(hs_function_definition*)&test_auto_screen_invoke_list_item_by_handle_4_definition,
	(hs_function_definition*)&test_auto_screen_send_button_press_3_definition,
	(hs_function_definition*)&test_download_storage_file_2_definition,
	(hs_function_definition*)&test_game_results_save_to_file_1_definition,
	(hs_function_definition*)&test_game_results_load_from_file_1_definition,
	(hs_function_definition*)&test_roster_save_to_file_1_definition,
	(hs_function_definition*)&test_roster_load_from_file_1_definition,
	(hs_function_definition*)&test_fragment_utility_drive_3_definition,
	(hs_function_definition*)&test_metagame_results_save_to_file_1_definition,
	(hs_function_definition*)&test_metagame_results_load_from_file_1_definition,
	(hs_function_definition*)&clear_webcache_0_definition,
	(hs_function_definition*)&force_manifest_redownload_0_definition,
	(hs_function_definition*)&online_files_retry_0_definition,
	(hs_function_definition*)&online_files_upload_2_definition,
	(hs_function_definition*)&online_files_throttle_bandwidth_1_definition,
	(hs_function_definition*)&online_marketplace_refresh_0_definition,
	(hs_function_definition*)&webstats_disable_1_definition,
	(hs_function_definition*)&webstats_test_submit_1_definition,
	(hs_function_definition*)&webstats_test_submit_multiplayer_0_definition,
	(hs_function_definition*)&webstats_test_submit_campaign_0_definition,
	(hs_function_definition*)&webstats_throttle_bandwidth_1_definition,
	(hs_function_definition*)&netdebug_prefer_internet_1_definition,
	(hs_function_definition*)&flag_new_2_definition,
	(hs_function_definition*)&flag_new_at_look_2_definition,
	(hs_function_definition*)&flags_clear_0_definition,
	(hs_function_definition*)&bug_now_1_definition,
	(hs_function_definition*)&bug_now_lite_1_definition,
	(hs_function_definition*)&bug_now_auto_1_definition,
	(hs_function_definition*)&object_list_children_2_definition,
	(hs_function_definition*)&voice_set_outgoing_channel_count_2_definition,
	(hs_function_definition*)&voice_set_voice_repeater_peer_index_1_definition,
	(hs_function_definition*)&voice_set_mute_3_definition,
	(hs_function_definition*)&net_leaderboard_clear_hopper_1_definition,
	(hs_function_definition*)&net_leaderboard_clear_global_arbitrated_1_definition,
	(hs_function_definition*)&net_leaderboard_clear_global_unarbitrated_1_definition,
	(hs_function_definition*)&net_leaderboard_refresh_0_definition,
	(hs_function_definition*)&dump_loaded_tags_0_definition,
	(hs_function_definition*)&interpolator_start_3_definition,
	(hs_function_definition*)&interpolator_start_smooth_3_definition,
	(hs_function_definition*)&interpolator_stop_1_definition,
	(hs_function_definition*)&interpolator_restart_1_definition,
	(hs_function_definition*)&interpolator_is_active_1_definition,
	(hs_function_definition*)&interpolator_is_finished_1_definition,
	(hs_function_definition*)&interpolator_set_current_value_2_definition,
	(hs_function_definition*)&interpolator_get_current_value_2_definition,
	(hs_function_definition*)&interpolator_get_start_value_2_definition,
	(hs_function_definition*)&interpolator_get_final_value_2_definition,
	(hs_function_definition*)&interpolator_get_current_phase_1_definition,
	(hs_function_definition*)&interpolator_get_current_time_fraction_1_definition,
	(hs_function_definition*)&interpolator_get_start_time_1_definition,
	(hs_function_definition*)&interpolator_get_final_time_1_definition,
	(hs_function_definition*)&interpolator_evaluate_at_3_definition,
	(hs_function_definition*)&interpolator_evaluate_at_time_fraction_3_definition,
	(hs_function_definition*)&interpolator_evaluate_at_time_3_definition,
	(hs_function_definition*)&interpolator_evaluate_at_time_delta_3_definition,
	(hs_function_definition*)&interpolator_stop_all_0_definition,
	(hs_function_definition*)&interpolator_restart_all_0_definition,
	(hs_function_definition*)&interpolator_flip_0_definition,
	(hs_function_definition*)&set_pc_runtime_language_1_definition,
	(hs_function_definition*)&animation_cache_stats_reset_0_definition,
	(hs_function_definition*)&cinematic_clone_players_weapon_3_definition,
	(hs_function_definition*)&cinematic_move_attached_objects_4_definition,
	(hs_function_definition*)&vehicle_enable_ghost_effects_1_definition,
	(hs_function_definition*)&set_global_sound_environment_6_definition,
	(hs_function_definition*)&reset_dsp_image_0_definition,
	(hs_function_definition*)&game_save_cinematic_skip_0_definition,
	(hs_function_definition*)&cinematic_outro_start_0_definition,
	(hs_function_definition*)&cinematic_enable_ambience_details_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_reset_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_blur_amount_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_threshold_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_brightness_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_box_factor_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_max_factor_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_silver_bullet_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_only_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_high_res_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_brightness_alpha_1_definition,
	(hs_function_definition*)&rasterizer_bloom_override_max_factor_alpha_1_definition,
	(hs_function_definition*)&cache_block_for_one_frame_0_definition,
	(hs_function_definition*)&sound_suppress_ambience_update_on_revert_0_definition,
	(hs_function_definition*)&render_autoexposure_enable_1_definition,
	(hs_function_definition*)&render_exposure_full_5_definition,
	(hs_function_definition*)&render_exposure_fade_in_2_definition,
	(hs_function_definition*)&render_exposure_fade_out_1_definition,
	(hs_function_definition*)&render_exposure_2_definition,
	(hs_function_definition*)&render_autoexposure_instant_1_definition,
	(hs_function_definition*)&render_exposure_set_environment_darken_1_definition,
	(hs_function_definition*)&render_depth_of_field_enable_1_definition,
	(hs_function_definition*)&render_depth_of_field_4_definition,
	(hs_function_definition*)&render_dof_focus_depth_1_definition,
	(hs_function_definition*)&render_dof_blur_animate_2_definition,
	(hs_function_definition*)&render_debug_video_mode_0_definition,
	(hs_function_definition*)&cinematic_lightmap_shadow_disable_0_definition,
	(hs_function_definition*)&cinematic_lightmap_shadow_enable_0_definition,
	(hs_function_definition*)&predict_animation_2_definition,
	(hs_function_definition*)&mp_players_by_team_1_definition,
	(hs_function_definition*)&mp_active_player_count_by_team_1_definition,
	(hs_function_definition*)&deterministic_end_game_0_definition,
	(hs_function_definition*)&mp_game_won_1_definition,
	(hs_function_definition*)&mp_respawn_override_timers_1_definition,
	(hs_function_definition*)&mp_ai_allegiance_2_definition,
	(hs_function_definition*)&mp_allegiance_2_definition,
	(hs_function_definition*)&mp_round_started_0_definition,
	(hs_function_definition*)&give_medal_1_definition,
	(hs_function_definition*)&mp_scripts_reset_0_definition,
	(hs_function_definition*)&mp_ai_place_1_definition,
	(hs_function_definition*)&mp_ai_place_2_definition,
	(hs_function_definition*)&mp_ai_kill_1_definition,
	(hs_function_definition*)&mp_ai_kill_silent_1_definition,
	(hs_function_definition*)&mp_object_create_1_definition,
	(hs_function_definition*)&mp_object_create_clone_1_definition,
	(hs_function_definition*)&mp_object_create_anew_1_definition,
	(hs_function_definition*)&mp_object_destroy_1_definition,
	(hs_function_definition*)&tag_file_set_backend_1_definition,
	(hs_function_definition*)&mp_object_belongs_to_team_2_definition,
	(hs_function_definition*)&mp_weapon_belongs_to_team_2_definition,
	(hs_function_definition*)&mp_debug_goal_object_boundary_geometry_1_definition,
	(hs_function_definition*)&mp_dump_candy_monitor_state_0_definition,
	(hs_function_definition*)&set_camera_third_person_2_definition,
	(hs_function_definition*)&get_camera_third_person_1_definition,
	(hs_function_definition*)&determinism_debug_manager_enable_logging_1_definition,
	(hs_function_definition*)&determinism_debug_manager_set_trace_flags_1_definition,
	(hs_function_definition*)&determinism_debug_manager_enable_game_state_checksum_1_definition,
	(hs_function_definition*)&determinism_debug_manager_enable_trace_1_definition,
	(hs_function_definition*)&determinism_debug_manager_set_consumer_sample_level_2_definition,
	(hs_function_definition*)&unknown584_1_definition,
	(hs_function_definition*)&saved_film_play_2_definition,
	(hs_function_definition*)&saved_film_play_last_0_definition,
	(hs_function_definition*)&saved_film_disable_version_checking_1_definition,
	(hs_function_definition*)&saved_film_toggle_debug_saving_1_definition,
	(hs_function_definition*)&saved_films_delete_on_level_load_1_definition,
	(hs_function_definition*)&saved_films_show_timestamp_1_definition,
	(hs_function_definition*)&saved_film_manager_should_record_film_default_1_definition,
	(hs_function_definition*)&mover_set_program_2_definition,
	(hs_function_definition*)&determinism_log_compare_log_files_2_definition,
	(hs_function_definition*)&floating_point_exceptions_enable_1_definition,
	(hs_function_definition*)&determinism_debug_manager_enable_log_file_comparision_on_oos_1_definition,
	(hs_function_definition*)&event_logs_snapshot_0_definition,
	(hs_function_definition*)&tag_reload_force_1_definition,
	(hs_function_definition*)&tag_unload_force_1_definition,
	(hs_function_definition*)&tag_load_force_1_definition,
	(hs_function_definition*)&predict_bink_movie_1_definition,
	(hs_function_definition*)&predict_bink_movie_from_tag_1_definition,
	(hs_function_definition*)&profiler_dump_history_0_definition,
	(hs_function_definition*)&camera_set_mode_2_definition,
	(hs_function_definition*)&camera_set_flying_cam_at_point_2_definition,
	(hs_function_definition*)&camera_set_target_2_definition,
	(hs_function_definition*)&game_coop_player_count_0_definition,
	(hs_function_definition*)&player_force_mode_1_definition,
	(hs_function_definition*)&profiler_output_pulse_0_definition,
	(hs_function_definition*)&string_id_name_1_definition,
	(hs_function_definition*)&find_1_definition,
	(hs_function_definition*)&add_recycling_volume_3_definition,
	(hs_function_definition*)&tag_resources_set_per_frame_publish_1_definition,
	(hs_function_definition*)&object_recycling_clear_history_0_definition,
	(hs_function_definition*)&dump_cinematics_script_0_definition,
	(hs_function_definition*)&global_preferences_clear_0_definition,
	(hs_function_definition*)&network_storage_set_storage_subdirectory_1_definition,
	(hs_function_definition*)&network_storage_set_storage_user_1_definition,
	(hs_function_definition*)&status_line_dump_0_definition,
	(hs_function_definition*)&game_tick_get_0_definition,
	(hs_function_definition*)&loop_it_1_definition,
	(hs_function_definition*)&loop_clear_0_definition,
	(hs_function_definition*)&status_lines_enable_1_definition,
	(hs_function_definition*)&status_lines_disable_1_definition,
	(hs_function_definition*)&on_target_platform_0_definition,
	(hs_function_definition*)&profile_activate_1_definition,
	(hs_function_definition*)&profile_deactivate_1_definition,
	(hs_function_definition*)&net_game_set_player_standing_2_definition,
	(hs_function_definition*)&net_get_game_id_0_definition,
	(hs_function_definition*)&net_test_matchmaking_hopper_list_0_definition,
	(hs_function_definition*)&net_test_matchmaking_hopper_game_list_0_definition,
	(hs_function_definition*)&net_test_matchmaking_hopper_set_game_1_definition,
	(hs_function_definition*)&saved_film_set_playback_game_speed_1_definition,
	(hs_function_definition*)&saved_film_set_pending_playback_game_speed_1_definition,
	(hs_function_definition*)&noguchis_mystery_tour_1_definition,
	(hs_function_definition*)&designer_zone_sync_0_definition,
	(hs_function_definition*)&dump_designer_zone_1_definition,
	(hs_function_definition*)&designer_zone_activate_1_definition,
	(hs_function_definition*)&designer_zone_deactivate_1_definition,
	(hs_function_definition*)&object_set_always_active_2_definition,
	(hs_function_definition*)&saved_film_seek_to_film_tick_1_definition,
	(hs_function_definition*)&tag_is_active_1_definition,
	(hs_function_definition*)&tag_resources_enable_optional_caching_1_definition,
	(hs_function_definition*)&dump_active_resources_0_definition,
	(hs_function_definition*)&object_set_persistent_2_definition,
	(hs_function_definition*)&display_zone_size_estimates_1_definition,
	(hs_function_definition*)&report_zone_size_estimates_0_definition,
	(hs_function_definition*)&net_test_disconnect_squad_0_definition,
	(hs_function_definition*)&net_test_disconnect_group_0_definition,
	(hs_function_definition*)&net_test_clear_squad_session_parameter_1_definition,
	(hs_function_definition*)&net_test_clear_group_session_parameter_1_definition,
	(hs_function_definition*)&net_test_life_cycle_pause_2_definition,
	(hs_function_definition*)&net_test_life_cycle_display_states_0_definition,
	(hs_function_definition*)&overlapped_display_task_descriptions_0_definition,
	(hs_function_definition*)&overlapped_task_inject_error_2_definition,
	(hs_function_definition*)&net_leaderboard_clear_hopper_all_users_1_definition,
	(hs_function_definition*)&net_leaderboard_clear_global_arbitrated_all_users_1_definition,
	(hs_function_definition*)&net_leaderboard_clear_global_unarbitrated_all_users_1_definition,
	(hs_function_definition*)&test_sapien_crash_0_definition,
	(hs_function_definition*)&output_window_add_category_1_definition,
	(hs_function_definition*)&output_window_remove_category_1_definition,
	(hs_function_definition*)&output_window_list_categories_0_definition,
	(hs_function_definition*)&decorators_split_2_definition,
	(hs_function_definition*)&bandwidth_profiler_enable_1_definition,
	(hs_function_definition*)&bandwidth_profiler_set_context_1_definition,
	(hs_function_definition*)&overlapped_task_pause_2_definition,
	(hs_function_definition*)&net_banhammer_set_controller_cheat_flags_3_definition,
	(hs_function_definition*)&net_banhammer_set_controller_ban_flags_4_definition,
	(hs_function_definition*)&net_banhammer_dump_strings_0_definition,
	(hs_function_definition*)&net_banhammer_dump_repeated_play_list_0_definition,
	(hs_function_definition*)&net_leaderboard_set_user_stats_6_definition,
	(hs_function_definition*)&net_leaderboard_set_user_game_stats_5_definition,
	(hs_function_definition*)&net_build_map_variant_1_definition,
	(hs_function_definition*)&net_verify_map_variant_1_definition,
	(hs_function_definition*)&async_set_work_delay_milliseconds_1_definition,
	(hs_function_definition*)&sound_looping_start_with_effect_4_definition,
	(hs_function_definition*)&tag_resources_set_demand_throttle_to_io_1_definition,
	(hs_function_definition*)&set_performance_throttle_3_definition,
	(hs_function_definition*)&global_preferences_quality_set_all_1_definition,
	(hs_function_definition*)&get_performance_throttle_2_definition,
	(hs_function_definition*)&unknown5E4_1_definition,
	(hs_function_definition*)&cinematic_zone_activate_1_definition,
	(hs_function_definition*)&cinematic_zone_deactivate_1_definition,
	(hs_function_definition*)&cinematic_zone_activate_from_editor_1_definition,
	(hs_function_definition*)&cinematic_zone_deactivate_from_editor_1_definition,
	(hs_function_definition*)&tiling_current_0_definition,
	(hs_function_definition*)&unit_limit_lipsync_to_mouth_only_2_definition,
	(hs_function_definition*)&dump_active_zone_tags_0_definition,
	(hs_function_definition*)&calculate_tag_prediction_1_definition,
	(hs_function_definition*)&tag_resources_enable_fast_prediction_1_definition,
	(hs_function_definition*)&unit_start_first_person_custom_animation_4_definition,
	(hs_function_definition*)&unit_is_playing_custom_first_person_animation_1_definition,
	(hs_function_definition*)&unit_stop_first_person_custom_animation_1_definition,
	(hs_function_definition*)&prepare_to_switch_to_zone_set_1_definition,
	(hs_function_definition*)&cinematic_zone_activate_for_debugging_1_definition,
	(hs_function_definition*)&unit_play_random_ping_1_definition,
	(hs_function_definition*)&player_control_fade_out_all_input_1_definition,
	(hs_function_definition*)&player_control_fade_in_all_input_1_definition,
	(hs_function_definition*)&unit_control_fade_out_all_input_2_definition,
	(hs_function_definition*)&unit_control_fade_in_all_input_2_definition,
	(hs_function_definition*)&player_control_lock_gaze_3_definition,
	(hs_function_definition*)&player_control_unlock_gaze_1_definition,
	(hs_function_definition*)&player_control_scale_all_input_2_definition,
	(hs_function_definition*)&run_like_dvd_0_definition,
	(hs_function_definition*)&profiler_auto_core_save_1_definition,
	(hs_function_definition*)&run_no_hdd_0_definition,
	(hs_function_definition*)&cinematic_tag_reference_get_bink_1_definition,
	(hs_function_definition*)&voice_set_force_match_configurations_4_definition,
	(hs_function_definition*)&voice_set_force_hud_2_definition,
	(hs_function_definition*)&object_set_custom_animation_speed_2_definition,
	(hs_function_definition*)&scenery_animation_start_at_frame_loop_4_definition,
	(hs_function_definition*)&saved_film_set_repro_mode_1_definition,
	(hs_function_definition*)&cortana_tag_reference_get_scene_1_definition,
	(hs_function_definition*)&net_banhammer_force_download_2_definition,
	(hs_function_definition*)&font_set_emergency_0_definition,
	(hs_function_definition*)&biped_force_ground_fitting_on_2_definition,
	(hs_function_definition*)&cinematic_set_chud_objective_1_definition,
	(hs_function_definition*)&chud_show_cinematic_title_2_definition,
	(hs_function_definition*)&terminal_is_being_read_0_definition,
	(hs_function_definition*)&terminal_was_accessed_1_definition,
	(hs_function_definition*)&terminal_was_completed_1_definition,
	(hs_function_definition*)&unit_get_primary_weapon_1_definition,
	(hs_function_definition*)&dump_cortana_script_0_definition,
	(hs_function_definition*)&budget_resource_get_animation_graph_1_definition,
	(hs_function_definition*)&game_award_level_complete_achievements_0_definition,
	(hs_function_definition*)&budget_resource_get_looping_sound_1_definition,
	(hs_function_definition*)&game_safe_to_respawn_1_definition,
	(hs_function_definition*)&cortana_effect_kill_0_definition,
	(hs_function_definition*)&test_create_content_item_slayer_0_definition,
	(hs_function_definition*)&test_create_content_item_screenshot_0_definition,
	(hs_function_definition*)&cinematic_scripting_destroy_cortana_effect_cinematic_0_definition,
	(hs_function_definition*)&ai_migrate_infanty_2_definition,
	(hs_function_definition*)&render_cinematic_motion_blur_1_definition,
	(hs_function_definition*)&ai_dont_do_avoidance_2_definition,
	(hs_function_definition*)&cinematic_scripting_clean_up_1_definition,
	(hs_function_definition*)&ai_erase_inactive_2_definition,
	(hs_function_definition*)&ai_survival_cleanup_3_definition,
	(hs_function_definition*)&stop_bink_movie_0_definition,
	(hs_function_definition*)&play_credits_unskippable_0_definition,
	(hs_function_definition*)&budget_resource_get_sound_1_definition,
	(hs_function_definition*)&controller_set_single_player_level_unlocked_3_definition,
	(hs_function_definition*)&physical_memory_dump_0_definition,
	(hs_function_definition*)&tag_resources_validate_all_pages_1_definition,
	(hs_function_definition*)&cinematic_set_debug_mode_1_definition,
	(hs_function_definition*)&cinematic_scripting_get_object_3_definition,
	(hs_function_definition*)&dummy_function_0_definition,
	(hs_function_definition*)&gp_integer_get_1_definition,
	(hs_function_definition*)&gp_integer_set_2_definition,
	(hs_function_definition*)&gp_boolean_get_1_definition,
	(hs_function_definition*)&gp_boolean_set_2_definition,
	(hs_function_definition*)&gp_dump_1_definition,
	(hs_function_definition*)&gp_dump_debug_1_definition,
	(hs_function_definition*)&gp_startup_1_definition,
	(hs_function_definition*)&gp_reset_0_definition,
	(hs_function_definition*)&gp_commit_options_0_definition,
	(hs_function_definition*)&cinematic_scripting_start_screen_effect_4_definition,
	(hs_function_definition*)&cinematic_scripting_stop_screen_effect_4_definition,
	(hs_function_definition*)&game_level_prepare_1_definition,
	(hs_function_definition*)&prepare_game_level_1_definition,
	(hs_function_definition*)&game_start_with_network_session_3_definition,
	(hs_function_definition*)&levels_add_campaign_map_with_id_2_definition,
	(hs_function_definition*)&levels_add_campaign_map_1_definition,
	(hs_function_definition*)&sound_impulse_start_editor_3_definition,
	(hs_function_definition*)&sound_impulse_start_effect_editor_4_definition,
	(hs_function_definition*)&sound_impulse_start_3d_editor_3_definition,
	(hs_function_definition*)&sound_looping_start_editor_3_definition,
	(hs_function_definition*)&debug_sound_channels_log_start_0_definition,
	(hs_function_definition*)&debug_sound_channels_log_start_named_1_definition,
	(hs_function_definition*)&debug_sound_channels_log_stop_0_definition,
	(hs_function_definition*)&cinematic_scripting_set_user_input_constraints_4_definition,
	(hs_function_definition*)&is_skull_primary_enable_2_definition,
	(hs_function_definition*)&is_skull_secondary_enable_2_definition,
	(hs_function_definition*)&controller_set_popup_message_index_2_definition,
	(hs_function_definition*)&controller_set_vidmaster_seen_2_definition,
	(hs_function_definition*)&gui_enter_lobby_1_definition,
	(hs_function_definition*)&survival_mode_respawn_dead_players_0_definition,
	(hs_function_definition*)&survival_mode_lives_get_0_definition,
	(hs_function_definition*)&survival_mode_lives_set_1_definition,
	(hs_function_definition*)&survival_mode_set_get_0_definition,
	(hs_function_definition*)&survival_mode_set_set_1_definition,
	(hs_function_definition*)&survival_mode_round_get_0_definition,
	(hs_function_definition*)&survival_mode_round_set_1_definition,
	(hs_function_definition*)&survival_mode_wave_get_0_definition,
	(hs_function_definition*)&survival_mode_wave_set_1_definition,
	(hs_function_definition*)&survival_mode_set_multiplier_get_0_definition,
	(hs_function_definition*)&survival_mode_set_multiplier_set_1_definition,
	(hs_function_definition*)&survival_mode_bonus_multiplier_get_0_definition,
	(hs_function_definition*)&survival_mode_bonus_multiplier_set_1_definition,
	(hs_function_definition*)&debug_sound_channel_fadeout_time_1_definition,
	(hs_function_definition*)&survival_mode_set_rounds_per_set_1_definition,
	(hs_function_definition*)&survival_mode_set_waves_per_round_1_definition,
	(hs_function_definition*)&render_model_marker_name_filter_1_definition,
	(hs_function_definition*)&survival_mode_event_new_1_definition,
	(hs_function_definition*)&survival_mode_begin_new_set_0_definition,
	(hs_function_definition*)&survival_mode_begin_new_round_0_definition,
	(hs_function_definition*)&survival_mode_begin_new_wave_0_definition,
	(hs_function_definition*)&survival_mode_award_hero_medal_0_definition,
	(hs_function_definition*)&campaign_metagame_get_player_score_1_definition,
	(hs_function_definition*)&collision_debug_lightmaps_print_0_definition,
	(hs_function_definition*)&debug_coop_campaign_save_1_definition,
	(hs_function_definition*)&player_set_look_training_hack_2_definition,
	(hs_function_definition*)&device_arg_has_been_touched_by_unit_3_definition,
	(hs_function_definition*)&gui_hide_all_screens_1_definition,
	(hs_function_definition*)&debug_lightmap_inspect_0_definition,
	(hs_function_definition*)&achievements_display_debug_names_0_definition,
	(hs_function_definition*)&achievement_grant_to_controller_by_debug_name_2_definition,
	(hs_function_definition*)&achievements_grant_all_to_controller_1_definition,
	(hs_function_definition*)&achievements_award_all_vidmaster_achievements_1_definition,
	(hs_function_definition*)&chud_fade_for_first_person_cinematic_2_definition,
	(hs_function_definition*)&achievement_post_chud_progression_3_definition,
	(hs_function_definition*)&object_set_vision_mode_render_default_2_definition,
	(hs_function_definition*)&chud_show_navpoint_4_definition,
	(hs_function_definition*)&run_like_package_0_definition,
	(hs_function_definition*)&clear_map_slot_1_definition,
	(hs_function_definition*)&clear_map_type_1_definition,
	(hs_function_definition*)&unit_confirm_message_1_definition,
	(hs_function_definition*)&unit_confirm_cancel_message_1_definition,
	(hs_function_definition*)&unit_confirm_y_button_1_definition,
	(hs_function_definition*)&player_get_kills_by_type_3_definition,
	(hs_function_definition*)&unit_flashlight_on_1_definition,
	(hs_function_definition*)&clear_command_buffer_cache_from_script_1_definition,
	(hs_function_definition*)&sound_looping_resume_3_definition,
	(hs_function_definition*)&chud_bonus_round_set_target_score_1_definition,
	(hs_function_definition*)&unknown673_0_definition,
	(hs_function_definition*)&unknown674_3_definition,
	(hs_function_definition*)&unknown675_4_definition,
	(hs_function_definition*)&unknown676_0_definition,
	(hs_function_definition*)&unknown677_1_definition,
	(hs_function_definition*)&unknown678_0_definition,
	(hs_function_definition*)&unknown679_0_definition,
	(hs_function_definition*)&unknown67A_1_definition,
	(hs_function_definition*)&unknown67B_1_definition,
	(hs_function_definition*)&unknown67C_0_definition,
	(hs_function_definition*)&saber_ui_get_player_model_id_0_definition,
	(hs_function_definition*)&saber_ui_get_music_id_0_definition,
	(hs_function_definition*)&unknown67F_2_definition,
	(hs_function_definition*)&unit_set_armor_3_definition,
	(hs_function_definition*)&player_set_armor_2_definition,
	(hs_function_definition*)&change_network_privacy_to_system_link_0_definition,
	(hs_function_definition*)&ssao_enable_1_definition,
	(hs_function_definition*)&ssao_radius_1_definition,
	(hs_function_definition*)&ssao_intensity_1_definition,
	(hs_function_definition*)&ssao_sampleZThreshold_1_definition,
	(hs_function_definition*)&fxaa_enable_1_definition,
	(hs_function_definition*)&color_grading_enable_1_definition,
	(hs_function_definition*)&lightshafts_enable_1_definition,
	(hs_function_definition*)&unknown68A_2_definition,
	(hs_function_definition*)&reflections_enable_1_definition,
	(hs_function_definition*)&reflection_blur_enable_1_definition,
	(hs_function_definition*)&reflection_multisampling_enabled_1_definition,
	(hs_function_definition*)&reflection_multisample_passes_1_definition,
	(hs_function_definition*)&reflection_fxaa_enable_1_definition,
	(hs_function_definition*)&reflection_debug_enable_1_definition,
	(hs_function_definition*)&reflection_color_bilinear_filter_enable_1_definition,
	(hs_function_definition*)&end_match_wait_time_increment_1_definition,
	(hs_function_definition*)&unknown693_0_definition,
	(hs_function_definition*)&background_bitmap_set_1_definition,
	(hs_function_definition*)&local_player_earn_wp_event_1_definition,
	(hs_function_definition*)&global_screen_effect_enable_1_definition,
	(hs_function_definition*)&unknown697_3_definition,
	(hs_function_definition*)&unknown698_0_definition,
	(hs_function_definition*)&saber_mouse_inversion_get_0_definition,
	(hs_function_definition*)&saber_mouse_inversion_set_0_definition,
	(hs_function_definition*)&unknown69B_7_definition,
	(hs_function_definition*)&unknown69C_2_definition,
	(hs_function_definition*)&unknown69D_0_definition,
	(hs_function_definition*)&unknown69E_4_definition,
	(hs_function_definition*)&unknown69F_1_definition,
	(hs_function_definition*)&unknown6A0_0_definition,
	(hs_function_definition*)&object_copy_player_appearance_2_definition,
	(hs_function_definition*)&exit_game_0_definition,
	(hs_function_definition*)&load_preferences_from_file_1_definition,
	(hs_function_definition*)&load_customization_from_file_1_definition,
};
const int32 hs_function_table_count = NUMBEROF(hs_function_table);
static_assert(hs_function_table_count >= k_maximum_number_of_ms23_hs_functions);

template<typename t_return_type>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)())
{
	int32 result = 0;

	if constexpr (std::is_void_v<t_return_type>)
	{
		func();
	}
	else
	{
		*reinterpret_cast<t_return_type*>(&result) = func();
	}

	hs_return(thread_index, result);
}

template<typename t_return_type, typename t_arg0>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4);
		}

		hs_return(thread_index, result);
	}
}
template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);
		t_arg8 arg8 = *reinterpret_cast<t_arg8*>(&actual_parameters[8]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);
		t_arg8 arg8 = *reinterpret_cast<t_arg8*>(&actual_parameters[8]);
		t_arg9 arg9 = *reinterpret_cast<t_arg9*>(&actual_parameters[9]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);
		t_arg8 arg8 = *reinterpret_cast<t_arg8*>(&actual_parameters[8]);
		t_arg9 arg9 = *reinterpret_cast<t_arg9*>(&actual_parameters[9]);
		t_arg10 arg10 = *reinterpret_cast<t_arg10*>(&actual_parameters[10]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10, typename t_arg11>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10, t_arg11))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);
		t_arg8 arg8 = *reinterpret_cast<t_arg8*>(&actual_parameters[8]);
		t_arg9 arg9 = *reinterpret_cast<t_arg9*>(&actual_parameters[9]);
		t_arg10 arg10 = *reinterpret_cast<t_arg10*>(&actual_parameters[10]);
		t_arg11 arg11 = *reinterpret_cast<t_arg11*>(&actual_parameters[11]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
		}

		hs_return(thread_index, result);
	}
}

template<typename t_return_type, typename t_arg0, typename t_arg1, typename t_arg2, typename t_arg3, typename t_arg4, typename t_arg5, typename t_arg6, typename t_arg7, typename t_arg8, typename t_arg9, typename t_arg10, typename t_arg11, typename t_arg12>
constexpr void macro_function_evaluate(int16 function_index, int32 thread_index, bool initialize, t_return_type(*func)(t_arg0, t_arg1, t_arg2, t_arg3, t_arg4, t_arg5, t_arg6, t_arg7, t_arg8, t_arg9, t_arg10, t_arg11, t_arg12))
{
	int32 result = 0;

	if (int32* actual_parameters = hs_macro_function_evaluate(function_index, thread_index, initialize))
	{
		t_arg0 arg0 = *reinterpret_cast<t_arg0*>(&actual_parameters[0]);
		t_arg1 arg1 = *reinterpret_cast<t_arg1*>(&actual_parameters[1]);
		t_arg2 arg2 = *reinterpret_cast<t_arg2*>(&actual_parameters[2]);
		t_arg3 arg3 = *reinterpret_cast<t_arg3*>(&actual_parameters[3]);
		t_arg4 arg4 = *reinterpret_cast<t_arg4*>(&actual_parameters[4]);
		t_arg5 arg5 = *reinterpret_cast<t_arg5*>(&actual_parameters[5]);
		t_arg6 arg6 = *reinterpret_cast<t_arg6*>(&actual_parameters[6]);
		t_arg7 arg7 = *reinterpret_cast<t_arg7*>(&actual_parameters[7]);
		t_arg8 arg8 = *reinterpret_cast<t_arg8*>(&actual_parameters[8]);
		t_arg9 arg9 = *reinterpret_cast<t_arg9*>(&actual_parameters[9]);
		t_arg10 arg10 = *reinterpret_cast<t_arg10*>(&actual_parameters[10]);
		t_arg11 arg11 = *reinterpret_cast<t_arg11*>(&actual_parameters[11]);
		t_arg12 arg12 = *reinterpret_cast<t_arg12*>(&actual_parameters[12]);

		if constexpr (std::is_void_v<t_return_type>)
		{
			func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
		}
		else
		{
			*reinterpret_cast<t_return_type*>(&result) = func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
		}

		hs_return(thread_index, result);
	}
}

