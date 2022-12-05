#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl interface_draw_fullscreen_overlays();
extern real_argb_color* __cdecl interface_get_real_argb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out);
extern real_argb_color* __cdecl interface_get_rgb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out);
extern void __cdecl interface_splitscreen_render();

