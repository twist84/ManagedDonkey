#pragma once

#include "cseries/cseries.hpp"

enum e_font_id;

struct s_animation_transform
{
	byte __data0[0x38];

	real_argb_color color;
	real_point3d position;
	real_vector2d scale;
	real_point2d local_scale_origin;
	real rotation_angle_radians;
	real sine_rotation_angle;
	real cosine_rotation_angle;
	real_point2d local_rotation_origin;
	real_point2d texture_uv;
	short bitmap_sprite_sequence;
	short bitmap_sprite_frame;
	e_font_id font;
};
static_assert(sizeof(s_animation_transform) == 0x88);

union gui_real_rectangle2d
{
	gui_real_rectangle2d* apply_projection_transform(real depth, rectangle2d const* window_bounds);
	real_point2d* get_centroid(real_point2d* centroid) const;
	real get_height() const;
	real get_width() const;
	gui_real_rectangle2d* offset(real dx, real dy);
	gui_real_rectangle2d* pin(real_rectangle2d const* pin_bounds);
	gui_real_rectangle2d* rotate_about_local_point(real_point2d const* local_point, real angle_radians);
	gui_real_rectangle2d* rotate_about_local_point(real_point2d const* local_point, real sine_angle, real cosine_angle);
	gui_real_rectangle2d* scale_about_local_point(real_point2d const* local_point, real_vector2d const* scale);
	void set(real_rectangle2d const* source);
	void set(rectangle2d const* source);
	static void transform_real_point3d(real_point3d* point, rectangle2d const* window_bounds);

	real_point2d vertex[4];
	struct
	{
		real_point2d top_left;
		real_point2d bottom_left;
		real_point2d top_right;
		real_point2d bottom_right;
	};
};
static_assert(sizeof(gui_real_rectangle2d) == 0x20);

