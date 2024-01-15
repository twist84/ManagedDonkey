#pragma once

#include "cseries/cseries.hpp"

struct real_linear_rgb_color;
struct c_hue_saturation_control
{
public:
	static void __cdecl copy_from_gamestate();
	static void __cdecl copy_to_gamestate();

	void reset();

	//static c_hue_saturation_control& x_default;
	//static c_hue_saturation_control& x_exposed_to_hs;
	//thread_local c_hue_saturation_control* x_in_gamestate;

	real m_hue;
	real m_saturation;
	real m_filter_red;
	real m_filter_green;
	real m_filter_blue;
};
static_assert(sizeof(c_hue_saturation_control) == 0x14);

extern c_hue_saturation_control& g_hue_saturation_control_default;
extern c_hue_saturation_control& g_hue_saturation_control_exposed_to_hs;

extern void __cdecl render_postprocess_reset_color_tweaking();

