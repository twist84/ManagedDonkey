#include "render/render.hpp"

#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "config/version.hpp"
#include "game/game.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "objects/lights.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_memory.hpp"
#include "render/old_render_debug.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"
#include "render/render_debug_commands.hpp"
#include "render/render_flags.hpp"
#include "render/render_lens_flares.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/render_sky.hpp"
#include "render/render_visibility_collection.hpp"
#include "text/draw_string.hpp"

REFERENCE_DECLARE(0x01913474, uns32, c_render_globals::m_frame_index);
REFERENCE_DECLARE(0x0191347D, bool, render_pc_specular);
REFERENCE_DECLARE(0x050E88F0, real32, c_render_globals::m_frame_time);
REFERENCE_DECLARE(0x050E88F4, bool, c_render_globals::m_distortion_active);
REFERENCE_DECLARE(0x050E88F5, bool, c_render_globals::m_distortion_visible);
REFERENCE_DECLARE(0x050E88F6, bool, c_render_globals::m_distortion_history);
REFERENCE_DECLARE(0x050E88F7, bool, c_render_globals::m_depth_fade_active);
REFERENCE_DECLARE(0x050E88F9, bool, c_render_globals::m_weather_occlusion_available);
REFERENCE_DECLARE(0x050E88FA, bool, c_render_globals::m_render_pc_albedo_lighting);

REFERENCE_DECLARE(0x050E88FC, void*, g_restricted_render_memory);
REFERENCE_DECLARE(0x050E8904, c_restricted_section, g_restricted_render_memory_section);

HOOK_DECLARE(0x00A29760, render_frame_begin);
HOOK_DECLARE(0x00A29970, render_initialize_for_new_map);
HOOK_DECLARE(0x00A2A080, render_setup_window);

void __cdecl c_render_globals::advance_frame_time(real32 seconds_elapsed)
{
	//INVOKE(0x00A29410, advance_frame_time, seconds_elapsed);

	m_frame_time += seconds_elapsed;
}

void __cdecl combine_projection_and_view_matrix(const real_matrix4x3* view_matrix, real32 const (* const projection_matrix)[4], real32* const projection_and_view_matrix)
{
	INVOKE(0x00A29440, combine_projection_and_view_matrix, view_matrix, projection_matrix, projection_and_view_matrix);
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

int32 __cdecl c_render_globals::get_frame_index()
{
	//return INVOKE(0x00A29590, get_frame_index);

	return m_frame_index;
}

real32 __cdecl c_render_globals::get_frame_time()
{
	//return INVOKE(0x00A295A0, get_frame_time);

	return m_frame_time;
}

void __cdecl c_render_globals::increment_frame_index()
{
	//INVOKE(0x00A295E0, increment_frame_index);

	m_frame_index++;
}

void __cdecl render_dispose()
{
	INVOKE(0x00A296F0, render_dispose);
}

void __cdecl render_dispose_from_old_map()
{
	INVOKE(0x00A29730, render_dispose_from_old_map);
}

void __cdecl render_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00A29750, render_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl render_frame_begin()
{
	//INVOKE(0x00A29760, render_frame_begin);

	render_update_debug_commands();
	rasterizer_memory_reset();
	render_visibility_reset_visible_clusters_for_frame();
	//c_render_information::clear_frame();
	c_render_flags::prepare_for_frame();
	lens_flares_garbage_collect();
	c_rasterizer::g_tiling_force_4x_msaa = game_options_valid() && game_is_ui_shell();
}

void __cdecl render_fullscreen_text(const s_render_fullscreen_text_context* context, bool simple_font)
{
	//INVOKE(0x00A297A0, render_fullscreen_text, context, simple_font);

	if (simple_font)
	{
		c_simple_font_draw_string draw_string;
		render_fullscreen_text_internal(context, &draw_string, nullptr);
	}
	else
	{
		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		draw_string.set_font(_font_id_fallback);
		render_fullscreen_text_internal(context, &draw_string, &font_cache);
	}
}

void __cdecl render_fullscreen_text_internal(const s_render_fullscreen_text_context* context, c_draw_string* draw_string, c_font_cache_base* font_cache)
{
	//INVOKE(0x00A29860, render_fullscreen_text_internal, context, draw_string, font_cache);

	real_argb_color color = *global_real_argb_white;
	if (context->text_color)
	{
		color.rgb = *context->text_color;
	}
	
	if (context->shadow_color)
	{
		real_argb_color shadow_color;
		shadow_color.rgb = *context->shadow_color;
		shadow_color.alpha = 1.0f;
	
		draw_string->set_shadow_color(&shadow_color);
	}
	
	draw_string->set_color(&color);
	draw_string->set_scale(context->font_scale);
	draw_string->draw(font_cache, version_get_full_string());
	
	if (context->string)
	{
		draw_string->draw_more(font_cache, "\r\n");
		draw_string->draw_more(font_cache, context->string);
	}
}

void __cdecl render_initialize()
{
	INVOKE(0x00A29900, render_initialize);

	render_debug_initialize();

	//g_restricted_render_memory = VirtualAlloc(0, 0xA0000, MEM_COMMIT, PAGE_READWRITE);
	//g_restricted_render_memory_section.initialize(g_restricted_render_memory, 0xA0000, k_crit_section_global_render_subsection);
	//
	//restricted_region_create(k_global_render_data_region, &g_restricted_render_memory_section, k_crit_section_global_render_region, 0);
	//restricted_region_lock_primary(k_global_render_data_region);
	//
	//c_render_globals::set_frame_index(0);
	//c_render_globals::set_frame_time(0.0);
	//
	//render_objects_initialize();
	//render_debug_initialize();
	//c_object_renderer::initialize();
	//vision_mode_initialize();
}

void __cdecl render_initialize_for_new_map()
{
	//INVOKE(0x00A29970, render_initialize_for_new_map);

	debug_render_freeze = false;
	c_render_globals::set_frame_time(0.0f);
	render_objects_initialize_for_new_map();
}

void __cdecl render_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00A29980, render_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	//c_structure_renderer::reset();
}

void __cdecl render_prepare_for_window(int32 player_window_index, int32 user_index)
{
	//INVOKE(0x00A29990, render_prepare_for_window, player_window_index, user_index);

	render_objects_prepare_for_window(user_index);
	render_sky_prepare_for_window();
	lights_prepare_for_window(player_window_index);
	c_render_flags::prepare_for_player_window(player_window_index);
}

void __cdecl render_setup_window(render_camera* camera, render_projection* projection)
{
	//INVOKE(0x00A2A080, render_setup_window, camera, projection);

	ASSERT(camera && projection);

	real_vector4d pc_only_shader_constant{};
	real32 pc_specular{};
	real32 pc_albedo_lighting{};

	if (render_pc_specular)
		pc_specular = 1.0f;
	else
		pc_specular = 0.0f;

	set_real_vector4d(&pc_only_shader_constant, pc_specular, pc_specular, pc_specular, pc_specular);
	c_rasterizer::set_pixel_shader_constant(95, 1, &pc_only_shader_constant);

	if (c_render_globals::m_render_pc_albedo_lighting)
		pc_albedo_lighting = 1.0f;
	else
		pc_albedo_lighting = 0.0f;

	set_real_vector4d(&pc_only_shader_constant, pc_albedo_lighting, pc_albedo_lighting, pc_albedo_lighting, pc_albedo_lighting);
	c_rasterizer::set_pixel_shader_constant(96, 1, &pc_only_shader_constant);

	real_vector4d combined_matrix[4]{};
	combine_projection_and_view_matrix(&projection->world_to_view, projection->projection_matrix, (real32*)combined_matrix);
	c_rasterizer::set_vertex_shader_constant(0, 4, combined_matrix);

	real_vector4d position{};
	real_vector4d forward{};
	real_vector4d up{};
	set_real_vector4d(&position, camera->position.x, camera->position.y, camera->position.z, 0.0f);
	set_real_vector4d(&forward, camera->forward.i, camera->forward.j, camera->forward.k, 0.0f);
	set_real_vector4d(&up, camera->up.i, camera->up.j, camera->up.k, 0.0f);
	//ASSERT(valid_real(up.i) && valid_real(up.j) && valid_real(up.k));
	//ASSERT(valid_real(forward.i) && valid_real(forward.j) && valid_real(forward.k));

	real_vector4d left{};
	cross_product3d((real_vector3d*)&up, (real_vector3d*)&forward, (real_vector3d*)&left);
	//ASSERT(valid_real(left.i) && valid_real(left.j) && valid_real(left.k));
	
	normalize3d((real_vector3d*)&left);
	left.l = 0.0f;
	
	c_rasterizer::set_vertex_shader_constant(4, 1, &forward);
	c_rasterizer::set_vertex_shader_constant(5, 1, &left);
	c_rasterizer::set_vertex_shader_constant(6, 1, &up);
	c_rasterizer::set_vertex_shader_constant(7, 1, &position);
	c_rasterizer::set_pixel_shader_constant(16, 1, &position);
}

void __cdecl render_window_reset(int32 user_index)
{
	//INVOKE(0x00A2A2E0, c_render_globals::render_window_reset, user_index);

	c_rasterizer::window_reset();
}

void __cdecl c_render_globals::set_depth_fade_active(bool depth_fade_active)
{
	//INVOKE(0x00A2A2F0, c_render_globals::set_depth_fade_active, depth_fade_active);

	m_depth_fade_active = depth_fade_active;
}

void __cdecl c_render_globals::set_distortion_active(bool distortion_active)
{
	//INVOKE(0x00A2A300, c_render_globals::set_distortion_active, distortion_active);

	m_distortion_active = distortion_active;
}

void __cdecl c_render_globals::set_distortion_history(bool distortion_history)
{
	//INVOKE(0x00A2A310, c_render_globals::set_distortion_history, distortion_history);

	m_distortion_history = distortion_history;
}

void __cdecl c_render_globals::set_distortion_visible(bool distortion_visible)
{
	//INVOKE(0x00A2A320, c_render_globals::set_distortion_visible, distortion_visible);

	m_distortion_visible = distortion_visible;
}

void __cdecl c_render_globals::set_frame_index(int32 frame_index)
{
	//INVOKE(0x00A2A330, c_render_globals::set_frame_index, frame_index);

	m_frame_index = frame_index;
}

void __cdecl c_render_globals::set_frame_time(real32 frame_time)
{
	//INVOKE(0x00A2A340, c_render_globals::set_frame_time, frame_time);

	m_frame_time = frame_time;
}

void __cdecl c_render_globals::set_weather_occlusion_available(bool weather_occlusion_available)
{
	//INVOKE(0x00A3B7E0, c_render_globals::set_weather_occlusion_available, weather_occlusion_available);

	m_weather_occlusion_available = weather_occlusion_available;
}

bool __cdecl c_render_globals::get_weather_occlusion_available()
{
	//return INVOKE(0x00A4BFD0, c_render_globals::get_weather_occlusion_available);

	return m_weather_occlusion_available;
}

real32 pregame_frame_scales[9] =
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

