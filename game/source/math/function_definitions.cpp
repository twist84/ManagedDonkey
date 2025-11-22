#include "math/function_definitions.hpp"

#include "cseries/cseries.hpp"

real32 c_function_definition::evaluate_scalar(real32 input, real32 range) const
{
    real64 result = DECLFUNC(0x00949CE0, real64, __thiscall, const c_function_definition*, real32, real32)(this, input, range);
    return (real32)result;
}

