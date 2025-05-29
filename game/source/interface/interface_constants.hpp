#pragma once

#include "cseries/cseries.hpp"

struct rasterizer_vertex_screen;

extern void __cdecl calculate_aspect_ratio_scaling();
extern real_vector2d __cdecl interface_get_aspect_ratio_scaling();
extern real_vector2d __cdecl interface_get_authoring_aspect_conversion_scaling(int32 destination_aspect);
extern real_vector2d __cdecl interface_get_authoring_to_target_dimensions_scaling();
extern int32 __cdecl interface_get_closest_authored_ui_aspect_ratio();
extern int32 __cdecl interface_get_closest_authoring_screen_dimensions();
extern void __cdecl interface_get_current_display_or_window_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds);
extern int32 __cdecl interface_get_current_display_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds);
extern void __cdecl interface_get_current_window_settings(rectangle2d* out_render_window_bounds, rectangle2d* out_render_window_title_safe_bounds, rectangle2d* out_display_window_bounds, rectangle2d* out_display_window_title_safe_bounds);
extern void __cdecl interface_scale_points_for_xenon_scaler(point2d* points, int32 point_count, real_vector2d const* aspect_ratio_scale);
extern void __cdecl interface_scale_points_for_xenon_scaler(real_point2d* points, int32 point_count, real_vector2d const* aspect_ratio_scale);
extern void __cdecl interface_scale_real_rectangle2d_for_xenon_scaler(real_rectangle2d* rectangle, real_vector2d const* aspect_ratio_scale);
extern void __cdecl interface_scale_rectangle2d_for_xenon_scaler(rectangle2d* rectangle, real_vector2d const* aspect_ratio_scale);
extern void __cdecl interface_scale_screenspace_vertices_for_xenon_scaler(rasterizer_vertex_screen* vertices, int32 vertex_count, real_vector2d const* aspect_ratio_scale);

