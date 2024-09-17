#include "render/render_visibility.hpp"

#include "visibility/visibility_collection.hpp"

bool debug_pvs_activation = false;

long __cdecl render_visible_objects_iterate(void(*function)(long))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_root_objects_starting_index() == 0);

    return INVOKE(0x00A7C130, render_visible_objects_iterate, function);
}

void __cdecl render_invisible_objects_iterate(void(*function)(long))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_cluster_starting_index() == 0);

    INVOKE(0x00A7BC50, render_invisible_objects_iterate, function);
}

bool __cdecl render_sphere_visible(real_point3d const* center, real radius)
{
    return INVOKE(0x00A7BDA0, render_sphere_visible, center, radius);
}

