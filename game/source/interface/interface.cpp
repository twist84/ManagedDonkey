#include "interface/interface.hpp"

#include "text/draw_string.hpp"

void __cdecl interface_dispose()
{
	INVOKE(0x00AADC50, interface_dispose);
}

void __cdecl interface_dispose_from_old_map()
{
	INVOKE(0x00AADC60, interface_dispose_from_old_map);
}

//.text:00AADC70 ; void __cdecl interface_draw_bitmap_modulated_p32(bitmap_data const*, c_rasterizer_texture_ref const*, int16_point2d const*, real_rectangle2d const*, real, real, dword, short)
//.text:00AADF50 ; void __cdecl interface_draw_bitmap_modulated(bitmap_data const*, c_rasterizer_texture_ref const*, int16_point2d const*, real_rectangle2d const*, real, real, real_argb_color const*, short)
//.text:00AAE230 ; 

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

//.text:00AAE5D0 ; long __cdecl interface_get_tag_index(short)

void __cdecl interface_initialize()
{
	INVOKE(0x00AAE610, interface_initialize);
}

void __cdecl interface_initialize_for_new_map()
{
	INVOKE(0x00AAE620, interface_initialize_for_new_map);
}

void __cdecl interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, e_font_id interface_font, e_text_style style, e_text_justification justification, dword flags, short color_table_index, short color_index)
{
	//INVOKE(0x00AAE630, interface_set_bitmap_text_draw_mode, draw_string, interface_font, style, justification, flags, color_table_index, color_index);

	real_argb_color color{};
	interface_get_real_argb_color(color_table_index, color_index, &color);

	real_argb_color shadow_color{};
	shadow_color.color = *global_real_rgb_black;
	shadow_color.alpha = color.alpha;

	draw_string->set_font(interface_font);
	draw_string->set_style(style);
	draw_string->set_justification(justification);
	draw_string->set_color(&color);
	draw_string->set_shadow_color(&shadow_color);
}

void __cdecl interface_splitscreen_render()
{
	INVOKE(0x00AAE6B0, interface_splitscreen_render);
}

