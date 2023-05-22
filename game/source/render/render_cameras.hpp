#pragma once

#include "cseries/cseries.hpp"

struct render_camera
{
	real_point3d position;
	vector3d forward;
	vector3d up;
	byte __unknown24;
	byte __unknown25;
	byte __unknown26;
	byte __unknown27;
	real vertical_field_of_view;
	real horizontal_field_of_view;
	short_rectangle2d window_pixel_bounds;
	short_rectangle2d window_title_safe_pixel_bounds;
	__int16 __unknown40;
	__int16 __unknown42;
	short_rectangle2d render_pixel_bounds;
	short_rectangle2d render_title_safe_pixel_bounds;
	short_rectangle2d display_pixel_bounds;
	real z_near;
	real z_far;
	plane3d __plane64;
	byte __data74[0x14];
};
static_assert(sizeof(render_camera) == 0x88);

struct render_projection
{
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;
	real_rectangle2d projection_bounds;
	real projection_matrix[4][4];
	real __unknownB8;
	real __unknownBC;
};
static_assert(sizeof(render_projection) == 0xC0);

struct s_observer_result;
extern void __cdecl render_camera_build(render_camera* camera, s_observer_result const* result);
extern void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real a4);
extern void __cdecl render_camera_build_viewport_frustum_bounds(render_camera const* camera, real_rectangle2d* frustum_bounds);
extern real __cdecl render_projection_sphere_diameter_in_pixels(render_projection const* projection, real_point3d const* point, real scale);
extern void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera);

