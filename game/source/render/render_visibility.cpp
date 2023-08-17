#include "render/render_visibility.hpp"

bool debug_pvs_activation = false;

bool __cdecl render_sphere_visible(real_point3d const* center, real radius)
{
    return INVOKE(0x00A7BDA0, render_sphere_visible, center, radius);
}

