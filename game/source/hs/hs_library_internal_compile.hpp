#pragma once

#include "hs/hs.hpp"
#include "hs/hs_compile.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_runtime.hpp"

enum
{
	_hs_function_begin = 0,
	_hs_function_begin_random,
	_hs_function_if,
	_hs_function_cond,
	_hs_function_set,
	_hs_function_and,
	_hs_function_or,
	_hs_function_plus,
	_hs_function_minus,
	_hs_function_times,
	_hs_function_divide,
	_hs_function_modulo,
	_hs_function_min,
	_hs_function_max,
	_hs_function_equal,
	_hs_function_not_equal,
	_hs_function_gt,
	_hs_function_lt,
	_hs_function_gte,
	_hs_function_lte,
	_hs_function_sleep,
	_hs_function_sleep_forever,
	_hs_function_sleep_until,
	_hs_function_wake,
	_hs_function_inspect,
	_hs_function_object_to_unit,

	_hs_function_debug_string__ai_debug_communication_suppress = 26,
	_hs_function_debug_string__ai_debug_communication_ignore = 27,
	_hs_function_debug_string__ai_debug_communication_focus = 28,

	_hs_function_debug_string__first = 26,
	_hs_function_debug_string__last = 28,
};

extern bool hs_parse_begin(int16 function_index, int32 expression_index);
extern bool hs_parse_if(int16 function_index, int32 expression_index);
extern bool hs_parse_cond(int16 function_index, int32 expression_index);
extern bool hs_parse_set(int16 function_index, int32 expression_index);
extern bool hs_parse_logical(int16 function_index, int32 expression_index);
extern bool hs_parse_arithmetic(int16 function_index, int32 expression_index);
extern bool hs_parse_equality(int16 function_index, int32 expression_index);
extern bool hs_parse_inequality(int16 function_index, int32 expression_index);
extern bool hs_parse_sleep(int16 function_index, int32 expression_index);
extern bool hs_parse_sleep_forever(int16 function_index, int32 expression_index);
extern bool hs_parse_sleep_until(int16 function_index, int32 expression_index);
extern bool hs_parse_wake(int16 function_index, int32 expression_index);
extern bool hs_parse_inspect(int16 function_index, int32 expression_index);
extern bool hs_parse_object_cast_up(int16 function_index, int32 expression_index);

