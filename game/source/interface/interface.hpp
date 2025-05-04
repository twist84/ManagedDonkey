#pragma once

struct c_draw_string;
union real_argb_color;

extern void __cdecl interface_dispose();
extern void __cdecl interface_dispose_from_old_map();
extern void __cdecl interface_draw_fullscreen_overlays();
extern real_argb_color* __cdecl interface_get_real_argb_color(int16 interface_reference_index, int16 color_block_index, real_argb_color* color_out);
extern real_argb_color* __cdecl interface_get_rgb_color(int16 interface_reference_index, int16 color_block_index, real_argb_color* color_out);
extern void __cdecl interface_initialize();
extern void __cdecl interface_initialize_for_new_map();
extern void __cdecl interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, e_font_id interface_font, e_text_style style, e_text_justification justification, uns32 flags, int16 color_table_index, int16 color_index);
extern void __cdecl interface_splitscreen_render();

