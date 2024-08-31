#pragma once

#include "cseries/cseries.hpp"

extern real& render_debug_depth_render_scale_r;
extern real& render_debug_depth_render_scale_g;
extern real& render_debug_depth_render_scale_b;
extern long& render_debug_depth_render;

extern void __cdecl render_texture_camera_dispose();
extern void __cdecl render_texture_camera_dispose_from_old_map();
extern void __cdecl render_texture_camera_initialize();
extern void __cdecl render_texture_camera_initialize_for_new_map();

