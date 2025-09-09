#include "math/color_math.hpp"

#include <math.h>

real_rgb_color interpolate_real_rgb_color(const real_rgb_color* a, const real_rgb_color* b, real32 t)
{
	real_rgb_color result;
	result.red = interpolate_linear(a->red, b->red, t);
	result.green = interpolate_linear(a->green, b->green, t);
	result.blue = interpolate_linear(a->blue, b->blue, t);
	return result;
}

real_argb_color* __cdecl pixel32_to_real_argb_color(argb_color color, real_argb_color* real_color)
{
	return INVOKE(0x005211C0, pixel32_to_real_argb_color, color, real_color);
}

real_rgb_color* __cdecl pixel32_to_real_rgb_color(rgb_color color, real_rgb_color* real_color)
{
	return INVOKE(0x00521300, pixel32_to_real_rgb_color, color, real_color);
}

uns32 __cdecl real_argb_color_to_pixel32(const real_argb_color* real_color)
{
	return INVOKE(0x00521810, real_argb_color_to_pixel32, real_color);
}

uns32 __cdecl real_rgb_color_to_pixel32(const real_rgb_color* real_color)
{
	return INVOKE(0x00521D90, real_rgb_color_to_pixel32, real_color);
}

real32 __cdecl value_degamma(real32 value)
{
	real32 v3 = value;
	if (value <= 0.0f)
		v3 = 0.0f;

	real32 v4 = value;
	if (value <= 0.0f)
		v4 = 0.0f;

	real32 v5 = v4;
	if (v3 >= 1.0f)
		v5 = 1.0f;

	return powf(v5, 2.2f);
}

real32 __cdecl value_regamma(real32 value)
{
	real32 v3 = value;
	if (value <= 0.0f)
		v3 = 0.0f;

	real32 v4 = value;
	if (value <= 0.0f)
		v4 = 0.0f;

	real32 v5 = v4;
	if (v3 >= 1.0f)
		v5 = 1.0f;

	return powf(v5, 0.45454544f);
}

void __cdecl color_degamma(const real_rgb_color* gamma_color, real_linear_rgb_color* linear_color)
{
	ASSERT(gamma_color && gamma_color->red >= 0.f && gamma_color->green >= 0.f && gamma_color->blue >= 0.f);

	linear_color->red = value_degamma(gamma_color->red);
	linear_color->green = value_degamma(gamma_color->green);
	linear_color->blue = value_degamma(gamma_color->blue);
}

void __cdecl color_regamma(const real_linear_rgb_color* linear_color, real_rgb_color* gamma_color)
{
	ASSERT(linear_color && linear_color->red >= 0.f && linear_color->green >= 0.f && linear_color->blue >= 0.f);

	gamma_color->red = value_regamma(linear_color->red);
	gamma_color->green = value_regamma(linear_color->green);
	gamma_color->blue = value_regamma(linear_color->blue);
}

bool valid_real_argb_color(const real_argb_color* color)
{
	//return INVOKE(0x00523B20, valid_real_argb_color, color);

	return valid_real(color->alpha)
		&& IN_RANGE_INCLUSIVE(color->alpha, 0.0f, 1.0f)
		&& valid_real_rgb_color(&color->rgb);
}

bool valid_real_rgb_color(const real_rgb_color* color)
{
	//return INVOKE(0x00523B70, valid_real_rgb_color, color);

	return valid_real(color->red)
		&& valid_real(color->green)
		&& valid_real(color->blue)
		&& IN_RANGE_INCLUSIVE(color->red, 0.0f, 1.0f)
		&& IN_RANGE_INCLUSIVE(color->green, 0.0f, 1.0f)
		&& IN_RANGE_INCLUSIVE(color->blue, 0.0f, 1.0f);
}

