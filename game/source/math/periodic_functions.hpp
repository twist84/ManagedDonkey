#pragma once

enum e_transition_function
{
	_transition_function_linear = 0,
	_transition_function_early,
	_transition_function_very_early,
	_transition_function_late,
	_transition_function_very_late,
	_transition_function_cosine,
	_transition_function_one,
	_transition_function_zero,

	k_transition_functions_count,
};

extern real32 __cdecl transition_function_evaluate(int16 function_type, real32 value);

