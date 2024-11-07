#pragma once

class c_d3d_pix_event
{
public:
	c_d3d_pix_event(unsigned long color, wchar_t const* name);
	~c_d3d_pix_event();
};

extern unsigned long g_rasterizer_profile_pix_colors[17];

extern void d3d_pix_begin_event(unsigned long color, wchar_t const* name);
extern void d3d_pix_end_event();

