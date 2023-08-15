#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl rasterizer_debug_line(real_point3d const* p0, real_point3d const* p1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl rasterizer_debug_line2d(real_point2d const* p0, real_point2d const* p1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl rasterizer_debug_triangle(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);

extern void __cdecl render_debug_string_immediate(bool draw_immediately, short const* tab_stops, short tab_stop_count, char const* string);
extern void __cdecl render_debug_string_at_point_immediate(real_point3d const* point, char const* string, real_argb_color const* color, real scale);
extern void __cdecl render_debug_string_at_point(real_point3d const* point, char const* string, union real_argb_color const* color);
extern void __cdecl render_debug_point(bool a1, real_point3d const* point, real scale, real_argb_color const* color);
extern void __cdecl render_debug_point2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* point, real a6, real_argb_color const* color, real a8);
extern void __cdecl render_debug_line(bool a1, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_line_shaded(bool a1, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl render_debug_line2d(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_line2d_shaded(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl render_debug_vector(bool a1, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color);
extern void __cdecl render_debug_triangle(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);
extern void __cdecl render_debug_triangle_outline(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);
extern void __cdecl render_debug_quadrilateral(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3, real_argb_color const* color);
extern real __cdecl build_height_matrix(real_point3d const* base, vector3d const* height, real_matrix4x3* out_matrix);
extern void __cdecl render_debug_build_circle_points(real radius, real_point2d* points, long total_point_count);
extern void __cdecl render_debug_build_pill_points(real_point3d const* base, vector3d const* height, real radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5);
extern void __cdecl render_debug_sphere(bool a1, real_point3d const* center, real radius, real_argb_color const* color);
extern void __cdecl render_debug_cylinder(bool a1, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color);
extern void __cdecl render_debug_pill(bool a1, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color);
extern void __cdecl render_debug_polygon(real_point3d const* points, short total_point_count, real_argb_color const* color);
extern void __cdecl render_debug_polygon_edges(real_point3d const* points, short total_point_count, real_argb_color const* color);
extern void __cdecl render_debug_polygon_fan(real_point3d const* points, short total_point_count, real_argb_color const* color);

