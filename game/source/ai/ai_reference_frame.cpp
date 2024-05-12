#include "ai/ai_reference_frame.hpp"

real_point3d* __cdecl ai_point_get_position(c_ai_point3d const* ai_point, real_point3d* position)
{
    return INVOKE(0x01451A10, ai_point_get_position, ai_point, position);
}

