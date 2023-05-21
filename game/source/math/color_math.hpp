#pragma once

#include "cseries/cseries.hpp"

extern real_argb_color* __cdecl pixel32_to_real_argb_color(argb_color color, real_argb_color* real_color);
extern real_rgb_color* __cdecl pixel32_to_real_rgb_color(rgb_color color, real_rgb_color* real_color);
extern dword __cdecl real_argb_color_to_pixel32(real_argb_color const* real_color);
extern dword __cdecl real_rgb_color_to_pixel32(real_rgb_color const* real_color);

