#include "interface/interface.hpp"

#include "text/draw_string.hpp"

void __cdecl interface_draw_fullscreen_overlays()
{
	//INVOKE(0x00AAE4C0, interface_draw_fullscreen_overlays);
	interface_splitscreen_render();
}

real_argb_color* __cdecl interface_get_real_argb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out)
{
	return INVOKE(0x00AAE4D0, interface_get_real_argb_color, interface_reference_index, color_block_index, color_out);
}

real_argb_color* __cdecl interface_get_rgb_color(short interface_reference_index, short color_block_index, real_argb_color* color_out)
{
	return INVOKE(0x00AAE4D0, interface_get_rgb_color, interface_reference_index, color_block_index, color_out);
}

void __cdecl interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, long font_id, long text_style, long text_justification, dword a5, short interface_tag_index, short a7)
{
	//INVOKE(0x00AAE630, interface_set_bitmap_text_draw_mode, draw_string, font_id, text_style, text_justification, a5, interface_tag_index, a7);

	real_argb_color color{};
	interface_get_real_argb_color(interface_tag_index, a7, &color);

	real_argb_color shadow_color{};
	shadow_color.color = *global_real_rgb_black;
	shadow_color.alpha = color.alpha;

	draw_string->set_font(font_id);
	draw_string->set_style(text_style);
	draw_string->set_justification(text_justification);
	draw_string->set_color(&color);
	draw_string->set_shadow_color(&shadow_color);
}

void __cdecl interface_splitscreen_render()
{
	INVOKE(0x00AAE6B0, interface_splitscreen_render);
}

