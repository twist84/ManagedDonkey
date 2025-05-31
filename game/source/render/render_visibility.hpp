#pragma once

#include "cseries/cseries.hpp"

struct render_camera;
struct render_projection;
struct visibility_projection;

extern void __cdecl render_invisible_objects_iterate(void(*function)(int32));
extern bool __cdecl render_sphere_visible(const real_point3d* center, real32 radius);
extern void __cdecl render_visibility_build_projection(const render_camera* camera, const render_projection* projection, s_cluster_reference camera_cluster_reference, struct visibility_projection* visibility_projection);
extern int32 __cdecl render_visible_objects_iterate(void(*function)(int32));

extern void visibility_profile_display(char* buffer, int32 buffer_size);

