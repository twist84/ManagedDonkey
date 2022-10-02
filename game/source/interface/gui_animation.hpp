#pragma once

#include "cseries/cseries.hpp"

struct s_animation_transform
{
	byte __data0[0x38];
	real_argb_color tint_color;
	byte __data48[0x40];
};
static_assert(sizeof(s_animation_transform) == 0x88);
