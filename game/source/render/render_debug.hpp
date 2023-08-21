#pragma once

#include "cseries/cseries.hpp"

#define CIRCLE_DIVISIONS 16

extern void __cdecl rasterizer_debug_line(real_point3d const* p0, real_point3d const* p1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl rasterizer_debug_line2d(real_point2d const* p0, real_point2d const* p1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl rasterizer_debug_triangle(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);

extern void __cdecl render_debug_begin(bool a1);
extern void __cdecl render_debug_end(bool a1);
extern void __cdecl render_debug_structure_draw();
extern void __cdecl render_debug_clients(long user_index);

real_argb_color const* __cdecl render_debug_random_color(real_argb_color* color);
real_argb_color const* __cdecl render_debug_unique_color(long index, real_argb_color* color);
//render_debug_spray
extern void __cdecl render_debug_point2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* point, real scale, real_argb_color const* color, real a8);
extern void __cdecl render_debug_line2d(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_line2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* p0, real_point2d const* p1, real_argb_color const* color, real a8);
extern void __cdecl render_debug_vector2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* point, vector2d const* vector, real scale, real_argb_color const* color, real a9);
extern void __cdecl render_debug_circle(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* center, real radius, real_argb_color const* color, real a8);
extern void __cdecl render_debug_circle(bool draw_immediately, real_point3d const* center, vector3d const* normal, real radius, real_argb_color const* color);
extern void __cdecl render_debug_polygon_regular(bool draw_immediately, real_point3d const* center, long point_count, vector3d const* normal, real radius, real_argb_color const* color);
extern void __cdecl render_debug_point(bool draw_immediately, real_point3d const* point, real scale, real_argb_color const* color);
extern void __cdecl render_debug_line(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_line_shaded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl render_debug_line2d_shaded(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color0, real_argb_color const* color1);
extern void __cdecl render_debug_line_unclipped(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_line_non_occluded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color);
extern void __cdecl render_debug_vector(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color);
extern void __cdecl render_debug_tick(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color);
extern void __cdecl render_debug_line_offset(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color, real scale);
extern void __cdecl render_debug_vectors(bool draw_immediately, real_point3d const* point, vector3d const* forward, vector3d const* up, real radius);
extern void __cdecl render_debug_quaternion(bool draw_immediately, real_point3d const* point, real_quaternion const* quaternion, real radius);
extern void __cdecl render_debug_matrix(bool draw_immediately, real_matrix4x3 const* matrix, real radius);
extern void __cdecl render_debug_matrix3x3(bool draw_immediately, matrix3x3 const* matrix, real_point3d const* point, real radius);
extern void __cdecl render_debug_triangle(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);
extern void __cdecl render_debug_triangle_outline(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color);
extern void __cdecl render_debug_quadrilateral(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3, real_argb_color const* color);
extern void __cdecl render_debug_sphere(bool draw_immediately, real_point3d const* center, real radius, real_argb_color const* color);
extern void __cdecl render_debug_cylinder(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color);
extern void __cdecl render_debug_pill(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color);
extern void __cdecl render_debug_box2d_outline(bool draw_immediately, real_rectangle2d const* bounds, real_argb_color const* color);
extern void __cdecl render_debug_box(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color);
extern void __cdecl render_debug_box_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color);
extern void __cdecl render_debug_box_outline(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color);
extern void __cdecl render_debug_box_outline_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color);
extern void __cdecl render_debug_polygon(real_point3d const* points, short total_point_count, real_argb_color const* color);
extern void __cdecl render_debug_polygon_edges(real_point3d const* points, short total_point_count, real_argb_color const* color);
extern void __cdecl render_debug_k_graph(real_point3d const* points, short total_point_count, real_argb_color const* color);
extern void __cdecl render_debug_cone_outline(bool draw_immediately, real_point3d const* point, vector3d const* direction, real radius, real cone_angle, real_argb_color const* color);
extern void __cdecl render_debug_string(char const* string);
extern void __cdecl render_debug_string_at_point(real_point3d const* point, char const* string, union real_argb_color const* color);

extern void __cdecl render_debug_string_immediate(bool draw_immediately, short const* tab_stops, short tab_stop_count, char const* string);
extern void __cdecl render_debug_string_at_point_immediate(real_point3d const* point, char const* string, real_argb_color const* color, real scale);

extern bool __cdecl render_debug_draw_immediately(real_argb_color const* color);
extern void __cdecl render_debug_add_cache_entry(short type, ...);
extern void __cdecl render_debug_cache_draw(bool a1);

extern void __cdecl render_debug_polygon_fan(real_point3d const* points, short total_point_count, real_argb_color const* color);

extern real __cdecl build_height_matrix(real_point3d const* base, vector3d const* height, real_matrix4x3* out_matrix);
extern void __cdecl render_debug_build_circle_points(real radius, real_point2d* points, long total_point_count);
extern void __cdecl render_debug_build_pill_points(real_point3d const* base, vector3d const* height, real radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5);

extern word __cdecl _random(dword* seed, char const* string, char const* file, dword line);

