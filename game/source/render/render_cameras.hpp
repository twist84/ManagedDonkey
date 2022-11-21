#pragma once

#include "cseries/cseries.hpp"

struct render_camera
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	byte __unknown24;
	byte __unknown25;
	byte __unknown26;
	byte __unknown27;
	real vertical_field_of_view;
	real horizontal_field_of_view;
	short_rectangle2d window_pixel_bounds;
	short_rectangle2d window_title_safe_pixel_bounds;
	long __unknown40;
	short_rectangle2d render_pixel_bounds;
	short_rectangle2d render_title_safe_pixel_bounds;
	short_rectangle2d display_pixel_bounds;
	real z_near;
	real z_far;
	byte __data64[0x24];
};
static_assert(sizeof(render_camera) == 0x88);

struct render_projection
{
	byte __data[0xC0];
};
static_assert(sizeof(render_projection) == 0xC0);

