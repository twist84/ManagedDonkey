#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

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
