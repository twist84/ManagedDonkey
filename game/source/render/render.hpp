#pragma once

#include "cseries/cseries.hpp"

struct s_render_fullscreen_text_context
{
	char const* text;
	real_rgb_color* color;
	real_rgb_color* shadow_color;
	real scale;
};
static_assert(sizeof(s_render_fullscreen_text_context) == 0x10);

extern void render_fullscreen_text(s_render_fullscreen_text_context const* context, bool simple_font);

