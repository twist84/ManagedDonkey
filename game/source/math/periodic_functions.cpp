#include "math/periodic_functions.hpp"

#include "cseries/cseries.hpp"

real32 __cdecl transition_function_evaluate(int16 function_type, real32 value)
{
	return INVOKE(0x00601950, transition_function_evaluate, function_type, value);
}

