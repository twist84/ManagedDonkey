#pragma once

#include "cseries/cseries.hpp"

struct s_animation_transform
{
	byte __data0[0x38];

	real_argb_color tint_color_direct;

	// gui_real_rectangle2d::offset
	// bounding_rect_from_sequence
	real_point3d __unknown48;

	vector2d __unknown54;

	// gui_real_rectangle2d::scale_about_local_point
	// local_point
	real_point2d __unknown5C;

	real __unknown64;
	real __unknown68;
	real __unknown6C;

	// gui_real_rectangle2d::rotate_about_local_point
	// local_point
	real_point2d __unknown70;

	real_point2d __unknown80;

	short sequence_index;
	short frame_index;
	long custom_font;
};
static_assert(sizeof(s_animation_transform) == 0x88);
