#pragma once

#include "cseries/cseries.hpp"

extern real_rgb_color interpolate_real_rgb_color(const real_rgb_color* a, const real_rgb_color* b, real32 t);

extern real_argb_color* __cdecl pixel32_to_real_argb_color(argb_color color, real_argb_color* real_color);
extern real_rgb_color* __cdecl pixel32_to_real_rgb_color(rgb_color color, real_rgb_color* real_color);
extern uns32 __cdecl real_argb_color_to_pixel32(const real_argb_color* real_color);
extern uns32 __cdecl real_rgb_color_to_pixel32(const real_rgb_color* real_color);

extern real32 __cdecl value_degamma(real32 value);
extern real32 __cdecl value_regamma(real32 value);
extern void __cdecl color_degamma(const real_rgb_color* gamma_color, real_linear_rgb_color* linear_color);
extern void __cdecl color_regamma(const real_linear_rgb_color* linear_color, real_rgb_color* gamma_color);

extern bool valid_real_argb_color(const real_argb_color* color);
extern bool valid_real_rgb_color(const real_rgb_color* color);

