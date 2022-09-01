#pragma once

#include "cseries/cseries.hpp"

struct s_geometry_sample
{
	real_point3d __unknown0;
	byte __dataC[0x90];
	byte __data9C[0x90];
	byte __data12C[0x90];
	real_vector3d __vector1BC;
	real_vector3d up;
	real_vector3d __vector1D4;
	real_rgb_color color;
	float __unknown1EC;
	byte __data1F0[8];
};
static_assert(sizeof(s_geometry_sample) == 0x1F8);

struct s_effect_lightprobe
{
	byte_flags flags;
	s_geometry_sample geometry_sample;
};
static_assert(sizeof(s_effect_lightprobe) == 0x1FC);
