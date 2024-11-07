#include "rasterizer/rasterizer_profile.hpp"

#include "math/color_math.hpp"
#include "render/views/render_view.hpp"

#include <d3dx9.h>

unsigned long g_rasterizer_profile_pix_colors[]
{
	real_argb_color_to_pixel32(global_real_argb_black),
	real_argb_color_to_pixel32(global_real_argb_grey),
	real_argb_color_to_pixel32(global_real_argb_red),
	real_argb_color_to_pixel32(global_real_argb_green),
	real_argb_color_to_pixel32(global_real_argb_blue),
	real_argb_color_to_pixel32(global_real_argb_yellow),
	real_argb_color_to_pixel32(global_real_argb_cyan),
	real_argb_color_to_pixel32(global_real_argb_magenta),
	real_argb_color_to_pixel32(global_real_argb_pink),
	real_argb_color_to_pixel32(global_real_argb_lightblue),
	real_argb_color_to_pixel32(global_real_argb_orange),
	real_argb_color_to_pixel32(global_real_argb_purple),
	real_argb_color_to_pixel32(global_real_argb_aqua),
	real_argb_color_to_pixel32(global_real_argb_darkgreen),
	real_argb_color_to_pixel32(global_real_argb_salmon),
	real_argb_color_to_pixel32(global_real_argb_red),
	0,
};

void d3d_pix_begin_event(unsigned long color, wchar_t const* name)
{
	if (render_debug_pix_events)
	{
		D3DPERF_BeginEvent(color, name);
		//D3DPERF_SetMarker(color, name);
	}
}

void d3d_pix_end_event()
{
	if (render_debug_pix_events)
		D3DPERF_EndEvent();
}

c_d3d_pix_event::c_d3d_pix_event(unsigned long color, wchar_t const* name)
{
	d3d_pix_begin_event(color, name);
}

c_d3d_pix_event::~c_d3d_pix_event()
{
	D3DPERF_EndEvent();
}

