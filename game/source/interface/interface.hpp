#pragma once

#include "cseries/cseries.hpp"

struct c_draw_string;

extern void __cdecl interface_draw_fullscreen_overlays();
extern real_argb_color* __cdecl interface_get_real_argb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out);
extern real_argb_color* __cdecl interface_get_rgb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out);
extern void __cdecl interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, long font_id, long text_style, long text_justification, dword a5, short interface_tag_index, short color_block_index);
extern void __cdecl interface_splitscreen_render();

