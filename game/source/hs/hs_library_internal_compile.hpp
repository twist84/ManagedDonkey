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
	_hs_function_sleep_for_ticks,
	_hs_function_sleep_forever,
	_hs_function_sleep_until,
	_hs_function_wake,
	_hs_function_inspect,
	_hs_function_object_to_unit,

	_hs_function_debug_string__ai_debug_communication_suppress = 27,
	_hs_function_debug_string__ai_debug_communication_ignore = 28,
	_hs_function_debug_string__ai_debug_communication_focus = 29,

	_hs_function_debug_string__first = 27,
	_hs_function_debug_string__last = 29,
};

extern bool hs_parse_begin(short function_index, long expression_index);
extern bool hs_parse_if(short function_index, long expression_index);
extern bool hs_parse_cond(short function_index, long expression_index);
extern bool hs_parse_set(short function_index, long expression_index);
extern bool hs_parse_logical(short function_index, long expression_index);
extern bool hs_parse_arithmetic(short function_index, long expression_index);
extern bool hs_parse_equality(short function_index, long expression_index);
extern bool hs_parse_inequality(short function_index, long expression_index);
extern bool hs_parse_sleep(short function_index, long expression_index);
extern bool hs_parse_sleep_for_ticks(short function_index, long expression_index);
extern bool hs_parse_sleep_forever(short function_index, long expression_index);
extern bool hs_parse_sleep_until(short function_index, long expression_index);
extern bool hs_parse_wake(short function_index, long expression_index);
extern bool hs_parse_inspect(short function_index, long expression_index);
extern bool hs_parse_object_cast_up(short function_index, long expression_index);

