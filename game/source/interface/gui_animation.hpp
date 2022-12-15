#pragma once

#include "cseries/cseries.hpp"

struct s_animation_transform
{
	byte __data0[0x38];
	real_argb_color tint_color_direct;
	real_point3d __unknown48;
	vector2d __unknown54;
	real_point2d __unknown5C;
	real __unknown64;
	real __unknown68;
	real __unknown6C;
	real_point2d __unknown70;
	real_point2d __unknown80;
	short __unknown82;
	short __unknown84;
	long custom_font;
};
static_assert(sizeof(s_animation_transform) == 0x88);
