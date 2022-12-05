#include "interface/interface.hpp"

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

//00AAE630 ; interface_set_bitmap_text_draw_mode

void __cdecl interface_splitscreen_render()
{
	INVOKE(0x00AAE6B0, interface_splitscreen_render);
}

