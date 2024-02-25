#include "render/render.hpp"

#include "config/version.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"

REFERENCE_DECLARE(0x01913474, dword, c_render_globals::m_frame_index);
REFERENCE_DECLARE(0x050E88F0, real, c_render_globals::m_frame_time);
REFERENCE_DECLARE(0x050E88F4, bool, c_render_globals::m_distortion_active);
REFERENCE_DECLARE(0x050E88F5, bool, c_render_globals::m_distortion_visible);
REFERENCE_DECLARE(0x050E88F6, bool, c_render_globals::m_distortion_history);
REFERENCE_DECLARE(0x050E88F7, bool, c_render_globals::m_depth_fade_active);
REFERENCE_DECLARE(0x050E88F9, bool, c_render_globals::m_weather_occlusion_available);

void __cdecl c_render_globals::advance_frame_time(real seconds_elapsed)
{
	//INVOKE(0x00A29410, advance_frame_time, seconds_elapsed);

	m_frame_time += seconds_elapsed;
}

bool __cdecl c_render_globals::get_depth_fade_active()
{
	//return INVOKE(0x00A29550, get_depth_fade_active);

	return m_depth_fade_active;
}

bool __cdecl c_render_globals::get_distortion_active()
{
	//return INVOKE(0x00A29560, get_distortion_active);

	return m_distortion_active;
}

bool __cdecl c_render_globals::get_distortion_history()
{
	//return INVOKE(0x00A29570, get_distortion_history);

	return m_distortion_history;
}

bool __cdecl c_render_globals::get_distortion_visible()
{
	//return INVOKE(0x00A29580, get_distortion_visible);

	return m_distortion_visible;
}

long __cdecl c_render_globals::get_frame_index()
{
	//return INVOKE(0x00A29590, get_frame_index);

	return m_frame_index;
}

real __cdecl c_render_globals::get_frame_time()
{
	//return INVOKE(0x00A295A0, get_frame_time);

	return m_frame_time;
}

void __cdecl c_render_globals::increment_frame_index()
{
	//INVOKE(0x00A295E0, increment_frame_index);

	m_frame_index++;
}

void __cdecl render_frame_begin()
{
	INVOKE(0x00A29760, render_frame_begin);
}

void __cdecl render_fullscreen_text(s_render_fullscreen_text_context const* context, bool simple_font)
{
	if (simple_font)
	{
		c_simple_font_draw_string draw_string;
		render_fullscreen_text_internal(context, &draw_string, nullptr);
	}
	else
	{
		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		draw_string.set_font(-2);
		render_fullscreen_text_internal(context, &draw_string, &font_cache);
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

//void __cdecl render_prepare_for_window(long player_window_index, e_output_user_index output_user_index)
void __cdecl render_prepare_for_window(long player_window_index, long output_user_index)
{
	INVOKE(0x00A29990, render_prepare_for_window, player_window_index, output_user_index);
}

void __cdecl render_setup_window(render_camera* camera, render_projection* projection)
{
	INVOKE(0x00A2A080, render_setup_window, camera, projection);
}

void __cdecl render_window_reset(long user_index)
{
	INVOKE(0x00A2A2E0, render_window_reset, user_index);
}

void __cdecl c_render_globals::set_depth_fade_active(bool depth_fade_active)
{
	//INVOKE(0x00A2A2F0, set_depth_fade_active, depth_fade_active);

	m_depth_fade_active = depth_fade_active;
}

void __cdecl c_render_globals::set_distortion_active(bool distortion_active)
{
	//INVOKE(0x00A2A300, set_distortion_active, distortion_active);

	m_distortion_active = distortion_active;
}

void __cdecl c_render_globals::set_distortion_history(bool distortion_history)
{
	//INVOKE(0x00A2A310, set_distortion_history, distortion_history);

	m_distortion_history = distortion_history;
}

void __cdecl c_render_globals::set_distortion_visible(bool distortion_visible)
{
	//INVOKE(0x00A2A320, set_distortion_visible, distortion_visible);

	m_distortion_visible = distortion_visible;
}

void __cdecl c_render_globals::set_frame_index(long frame_index)
{
	//INVOKE(0x00A2A330, set_frame_index, frame_index);

	m_frame_index = frame_index;
}

void __cdecl c_render_globals::set_frame_time(real frame_time)
{
	//INVOKE(0x00A2A340, set_frame_time, frame_time);

	m_frame_time = frame_time;
}

void __cdecl c_render_globals::set_weather_occlusion_available(bool weather_occlusion_available)
{
	//INVOKE(0x00A3B7E0, set_weather_occlusion_available, weather_occlusion_available);

	m_weather_occlusion_available = weather_occlusion_available;
}

bool __cdecl c_render_globals::get_weather_occlusion_available()
{
	//return INVOKE(0x00A4BFD0, get_weather_occlusion_available);

	return m_weather_occlusion_available;
}

real pregame_frame_scales[9] =
{
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f
};

s_render_fullscreen_text_context_colors pregame_frame_colors[9] =
{
	{
		.blank_frame = { 0.0f, 0.0f, 0.0f },
		.text_color = { 1.0f, 1.0f, 1.0f },
		.text_shadow_color = { 0.0f, 0.0f, 0.0f }
	},
	{
		.blank_frame = { 0.0f, 0.0f, 0.0f },
		.text_color = { 1.0f, 1.0f, 1.0f },
		.text_shadow_color = { 0.0f, 0.0f, 0.0f }
	},
	{
		.blank_frame = { 0.0f, 0.2f, 0.0f },
		.text_color = { 0.55f, 0.45f, 0.65f },
		.text_shadow_color = { 0.08f, 0.05f, 0.012f }
	},
	{
		.blank_frame = { 0.047058824f, 0.06666667f, 0.1254902f },
		.text_color = { 1.0f, 1.0f, 1.0f },
		.text_shadow_color = { 0.015686275f, 0.023529412f, 0.039215688f }
	},
	{
		//.blank_frame       = { 0.0f, 0.0f, 0.0f },
		.blank_frame = { 0.55f, 0.1f, 0.1f },
		.text_color = { 0.55f, 0.45f, 0.65f },
		.text_shadow_color = { 0.08f, 0.05f, 0.012f }
	},
	{
		//.blank_frame       = { 0.0f, 0.0f, 0.0f },
		.blank_frame = { 0.15f, 0.1f, 0.38f },
		.text_color = { 0.55f, 0.45f, 0.65f },
		.text_shadow_color = { 0.08f, 0.05f, 0.012f }
	},
	{
		.blank_frame = { 0.29f, 0.06f, 0.36f },
		.text_color = { 0.55f, 0.45f, 0.65f },
		.text_shadow_color = { 0.08f, 0.05f, 0.012f }
	},
	{
		.blank_frame = { 1.0f, 1.0f, 1.0f },
		.text_color = { 0.0f, 0.0f, 0.0f },
		.text_shadow_color = { 1.0f, 1.0f, 1.0f }
	},
	{
		.blank_frame = { 0.047058824f, 0.06666667f, 0.1254902f },
		.text_color = { 1.0f, 1.0f, 1.0f },
		.text_shadow_color = { 0.015686275f, 0.023529412f, 0.039215688f }
	}
};

