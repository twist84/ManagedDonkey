#pragma once

#include "cseries/cseries.hpp"

extern bool debug_pvs_activation;

extern long __cdecl render_visible_objects_iterate(void(*function)(long));
extern void __cdecl render_invisible_objects_iterate(void(*function)(long));
extern bool __cdecl render_sphere_visible(real_point3d const* center, real radius);

extern void visibility_profile_display(char* buffer, long buffer_size);

