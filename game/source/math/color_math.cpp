#include "math/color_math.hpp"

real_argb_color* __cdecl pixel32_to_real_argb_color(argb_color color, real_argb_color* real_color)
{
	return INVOKE(0x005211C0, pixel32_to_real_argb_color, color, real_color);
}

real_rgb_color* __cdecl pixel32_to_real_rgb_color(rgb_color color, real_rgb_color* real_color)
{
	return INVOKE(0x00521300, pixel32_to_real_rgb_color, color, real_color);
}

dword __cdecl real_argb_color_to_pixel32(real_argb_color const* real_color)
{
	return INVOKE(0x00521810, real_argb_color_to_pixel32, real_color);
}

dword __cdecl real_rgb_color_to_pixel32(real_rgb_color const* real_color)
{
	return INVOKE(0x00521D90, real_rgb_color_to_pixel32, real_color);
}

