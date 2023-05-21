#include "render/render.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"

void __cdecl render_frame_begin()
{
	INVOKE(0x00A29760, render_frame_begin);
}

void __cdecl render_fullscreen_text(s_render_fullscreen_text_context const* context, bool simple_font)
{
	if (simple_font)
	{
		c_simple_font_draw_string simple_font_draw_string;
		render_fullscreen_text_internal(context, &simple_font_draw_string, nullptr);
	}
	else
	{
		c_rasterizer_draw_string rasterizer_draw_string;
		c_font_cache_mt_safe font_cache;

		rasterizer_draw_string.set_font(-2);
		render_fullscreen_text_internal(context, &rasterizer_draw_string, &font_cache);
	}
}

void __cdecl render_fullscreen_text_internal(s_render_fullscreen_text_context const* context, c_draw_string* draw_string, c_font_cache_base* font_cache)
{
	real_argb_color color = *global_real_argb_white;
	if (context->color)
	{
		color.color = *context->color;
	}
	
	if (context->shadow_color)
	{
		real_argb_color shadow_color;
		shadow_color.color = *context->shadow_color;
		shadow_color.alpha = 1.0f;
	
		draw_string->set_shadow_color(&shadow_color);
	}
	
	draw_string->set_color(&color);
	draw_string->set_scale(context->scale);
	draw_string->draw(font_cache, version_get_full_string());
	
	if (context->text)
	{
		draw_string->draw_more(font_cache, "\r\n");
		draw_string->draw_more(font_cache, context->text);
	}
}

void __cdecl render_prepare_for_window(long player_window_index, long output_user_index)
{
	INVOKE(0x00A29990, render_prepare_for_window, player_window_index, output_user_index);
}

void __cdecl render_setup_window(render_camera* camera, render_projection* projection)
{
	INVOKE(0x00A2A080, render_setup_window, camera, projection);
}

