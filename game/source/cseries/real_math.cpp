#include "cseries/real_math.hpp"

#include "cseries/cseries.hpp"

euler_angles2d* euler_angles2d_from_vector3d(euler_angles2d* facing, vector3d const* forward)
{
    return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

