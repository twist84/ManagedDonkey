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

struct c_draw_string;
struct c_font_cache_base;

struct render_camera;
struct render_projection;

extern void __cdecl render_frame_begin();
extern void __cdecl render_fullscreen_text(s_render_fullscreen_text_context const* context, bool simple_font);
extern void __cdecl render_fullscreen_text_internal(s_render_fullscreen_text_context const* context, c_draw_string* draw_string, c_font_cache_base* font_cache);
extern void __cdecl render_prepare_for_window(long player_window_index, long output_user_index);
extern void __cdecl render_setup_window(render_camera* camera, render_projection* projection);

