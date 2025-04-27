#pragma once

#include "cseries/cseries.hpp"

extern real32& render_debug_depth_render_scale_r;
extern real32& render_debug_depth_render_scale_g;
extern real32& render_debug_depth_render_scale_b;
extern int32& render_debug_depth_render;

extern void __cdecl render_texture_camera_align_to_attached_object();
extern void __cdecl render_texture_camera_attach_to_object(int32 object_index, int32 marker_id);
extern void __cdecl render_texture_camera_dispose();
extern void __cdecl render_texture_camera_dispose_from_old_map();
extern void __cdecl render_texture_camera_enable_dynamic_lights(bool enable);
extern real32 __cdecl render_texture_camera_get_aspect_ratio();
extern int32 __cdecl render_texture_camera_get_height();
extern int32 __cdecl render_texture_camera_get_render_mode();
extern int32 __cdecl render_texture_camera_get_sub_A3AC30();
extern int32 __cdecl render_texture_camera_get_target_object_index();
extern int32 __cdecl render_texture_camera_get_width();
extern void __cdecl render_texture_camera_initialize();
extern void __cdecl render_texture_camera_initialize_for_new_map();
extern void __cdecl render_texture_camera_off();
extern void __cdecl render_texture_camera_on();
extern void __cdecl render_texture_camera_position_world_offset(real32 offset_x, real32 offset_y, real32 offset_z);
extern void __cdecl render_texture_camera_render_mode(int32 mode);
extern void __cdecl render_texture_camera_set_aspect_ratio(real32 aspect_ratio);
extern void __cdecl render_texture_camera_set_fov(real32 vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_fov_frame_target(real32 target_zoom);
extern void __cdecl render_texture_camera_set_object_marker(int32 object_index, int32 marker_id, real32 vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_position(real32 position_x, real32 position_y, real32 position_z);
extern void __cdecl render_texture_camera_set_position_and_direction(bool enabled, real_point3d const* position, real_vector3d const* forward, real_vector3d const* up, real32 vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_sub_A3B240(int32 object_index);
extern void __cdecl render_texture_camera_set_resolution(int32 width, int32 height);
extern void __cdecl render_texture_camera_set_target(real32 position_x, real32 position_y, real32 position_z);
extern void __cdecl render_texture_camera_target_object(int32 object_index, int32 marker_id);

