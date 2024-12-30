#pragma once

#include "cseries/cseries.hpp"

extern real& render_debug_depth_render_scale_r;
extern real& render_debug_depth_render_scale_g;
extern real& render_debug_depth_render_scale_b;
extern long& render_debug_depth_render;

extern void __cdecl render_texture_camera_align_to_attached_object();
extern void __cdecl render_texture_camera_attach_to_object(long object_index, long marker_id);
extern void __cdecl render_texture_camera_dispose();
extern void __cdecl render_texture_camera_dispose_from_old_map();
extern void __cdecl render_texture_camera_enable_dynamic_lights(bool enable);
extern real __cdecl render_texture_camera_get_aspect_ratio();
extern long __cdecl render_texture_camera_get_height();
extern long __cdecl render_texture_camera_get_render_mode();
extern long __cdecl render_texture_camera_get_sub_A3AC30();
extern long __cdecl render_texture_camera_get_target_object_index();
extern long __cdecl render_texture_camera_get_width();
extern void __cdecl render_texture_camera_initialize();
extern void __cdecl render_texture_camera_initialize_for_new_map();
extern void __cdecl render_texture_camera_off();
extern void __cdecl render_texture_camera_on();
extern void __cdecl render_texture_camera_position_world_offset(real offset_x, real offset_y, real offset_z);
extern void __cdecl render_texture_camera_render_mode(long mode);
extern void __cdecl render_texture_camera_set_aspect_ratio(real aspect_ratio);
extern void __cdecl render_texture_camera_set_fov(real vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_fov_frame_target(real target_zoom);
extern void __cdecl render_texture_camera_set_object_marker(long object_index, long marker_id, real vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_position(real position_x, real position_y, real position_z);
extern void __cdecl render_texture_camera_set_position_and_direction(bool enabled, real_point3d const* position, real_vector3d const* forward, real_vector3d const* up, real vertical_field_of_view_degrees);
extern void __cdecl render_texture_camera_set_sub_A3B240(long object_index);
extern void __cdecl render_texture_camera_set_resolution(long width, long height);
extern void __cdecl render_texture_camera_set_target(real position_x, real position_y, real position_z);
extern void __cdecl render_texture_camera_target_object(long object_index, long marker_id);

