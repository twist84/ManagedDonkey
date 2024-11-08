#include "render/views/render_player_view.hpp"

#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_occlusion_queries.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_flags.hpp"
#include "render/render_lens_flares.hpp"
#include "render/render_transparents.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"
#include "render_methods/render_method_submit.hpp"

REFERENCE_DECLARE(0x019147BC, real, render_debug_depth_render_scale_r);
REFERENCE_DECLARE(0x019147C0, real, render_debug_depth_render_scale_g);
REFERENCE_DECLARE(0x019147C4, real, render_debug_depth_render_scale_b);
REFERENCE_DECLARE(0x050FB3FC, long, render_debug_depth_render);
REFERENCE_DECLARE(0x01694EC8, c_screen_postprocess::s_settings const* const, c_screen_postprocess::x_settings);

HOOK_DECLARE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);
HOOK_DECLARE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);
HOOK_DECLARE_CLASS_MEMBER(0x00A39960, c_player_view, render_);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);
HOOK_DECLARE_CLASS(0x00A3A310, c_player_view, render_albedo_decals);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A6C0, c_player_view, render_lens_flares);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B380, c_player_view, render_transparents);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

void __thiscall c_player_view::render_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);

	c_rasterizer_profile_scope _distortion_apply(_rasterizer_profile_element_distortions, L"distortion_apply");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_distortions);
}

void c_player_view::create_frame_textures(long player_index)
{
	INVOKE_CLASS_MEMBER(0x00A38D70, c_player_view, create_frame_textures, player_index);
}

void __cdecl c_player_view::get_player_render_camera_orientation(real_matrix4x3* camera)
{
	//INVOKE(0x00A39640, get_player_render_camera_orientation, camera);

	ASSERT(camera);
	ASSERT(c_player_view::x_current_player_view, "cannot get the render camera when not in the player view render loop");

	render_camera const* rasterizer_camera = c_player_view::x_current_player_view->get_rasterizer_camera();

	camera->position = rasterizer_camera->position;
	camera->forward = rasterizer_camera->forward;
	camera->up = rasterizer_camera->up;
	cross_product3d(&camera->up, &camera->forward, &camera->left);
	camera->scale = 1.0f;
}

void __thiscall c_player_view::queue_patchy_fog()
{
	//INVOKE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);

	if (game_is_splitscreen_deterministic())
		return;

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, queue_patchy_fog);
}

void __thiscall c_player_view::render_()
{
	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_);
}

bool __thiscall c_player_view::render_albedo()
{
	//return INVOKE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);

	c_rasterizer_profile_scope _albedo(_rasterizer_profile_element_texaccum, L"albedo");

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_player_view, render_albedo);
	return result;
}

void __cdecl c_player_view::render_albedo_decals(bool a1, bool a2)
{
	//INVOKE(0x00A3A310, c_player_view::render_albedo_decals, a1, a2);

	c_rasterizer_profile_scope _decorators(_rasterizer_profile_element_total, L"decorators");

	HOOK_INVOKE_CLASS(, c_player_view, render_albedo_decals, decltype(&c_player_view::render_albedo_decals), a1, a2);
}

//.text:00A3A3C0 ; 

void __thiscall c_player_view::render_effects(e_effect_pass pass)
{
	//INVOKE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects, pass);

	c_rasterizer_profile_scope _render_effects(_rasterizer_profile_element_effects, L"render_effects");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_effects, pass);
}

void __thiscall c_player_view::render_first_person(bool a1)
{
	//INVOKE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);

	c_rasterizer_profile_scope _first_person(_rasterizer_profile_element_total, L"first_person");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person, a1);
}

void __thiscall c_player_view::render_first_person_albedo()
{
	//INVOKE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);

	c_rasterizer_profile_scope _first_person_albedo(_rasterizer_profile_element_total, L"first_person_albedo");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person_albedo);
}

//.text:00A3A6B0 ; 

void __thiscall c_player_view::render_lens_flares()
{
	//INVOKE_CLASS_MEMBER(0x00A3A6C0, c_player_view::render_lens_flares);

	c_rasterizer::set_using_albedo_sampler(false);
	rasterizer_occlusions_retrieve(m_player_index);

	{
		c_rasterizer_profile_scope _lens_flares(_rasterizer_profile_element_transparents, L"lens flares");
		lens_flares_render(m_player_view_user_index);
	}

	c_rasterizer::set_using_albedo_sampler(c_rasterizer::surface_valid(c_rasterizer::_surface_albedo));
}

void __thiscall c_player_view::render_lightmap_shadows()
{
	//INVOKE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);

	c_rasterizer_profile_scope _lightmap_shadows(_rasterizer_profile_element_total, L"lightmap_shadows");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_lightmap_shadows);
}

void __cdecl c_player_view::render_misc_transparents()
{
	c_rasterizer_profile_scope _render_misc_transparents(_rasterizer_profile_element_widgets, L"render_misc_transparents");

	INVOKE(0x00A3A790, c_player_view::render_misc_transparents);
}

//.text:00A3A7A0 ; public: void __cdecl c_player_view::render_patchy_fog()
//.text:00A3A7F0 ; render_patchy_fog_callback
//.text:00A3A850 ; public: virtual void __cdecl c_player_view::render_setup()

void __thiscall c_player_view::render_static_lighting()
{
	//INVOKE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);

	c_rasterizer_profile_scope _static_lighting(_rasterizer_profile_element_static_lighting, L"static_lighting");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_static_lighting);
}

//.text:00A3AAA0 ; public: virtual void __cdecl c_player_view::render_submit_visibility()
//.text:00A3AB20 ; 

void __cdecl render_texture_camera_dispose()
{
	INVOKE(0x00A3AB90, render_texture_camera_dispose);
}

void __cdecl render_texture_camera_dispose_from_old_map()
{
	INVOKE(0x00A3ABA0, render_texture_camera_dispose_from_old_map);
}

//.text:00A3ABB0 ; void __cdecl render_texture_camera_enable_dynamic_lights(bool)
//.text:00A3ABD0 ; double __cdecl render_texture_camera_get_aspect_ratio()
//.text:00A3ABF0 ; 
//.text:00A3AC10 ; 
//.text:00A3AC30 ; 
//.text:00A3AC70 ; 
//.text:00A3ACB0 ; 

void __cdecl render_texture_camera_initialize()
{
	INVOKE(0x00A3ACD0, render_texture_camera_initialize);
}

void __cdecl render_texture_camera_initialize_for_new_map()
{
	INVOKE(0x00A3AD30, render_texture_camera_initialize_for_new_map);
}

//.text:00A3AE50 ; void __cdecl render_texture_camera_off()
//.text:00A3AE70 ; void __cdecl render_texture_camera_on()
//.text:00A3AE90 ; void __cdecl render_texture_camera_position_world_offset(real, real, real)
//.text:00A3AEE0 ; void __cdecl render_texture_camera_render_mode(long)
//.text:00A3AF20 ; void __cdecl render_texture_camera_set_aspect_ratio(real)
//.text:00A3AF50 ; void __cdecl render_texture_camera_set_fov(real)
//.text:00A3AFB0 ; void __cdecl render_texture_camera_set_fov_frame_target(real)
//.text:00A3AFF0 ; void __cdecl render_texture_camera_set_object_marker(long, long, real)
//.text:00A3B0F0 ; void __cdecl render_texture_camera_set_position(real, real, real)
//.text:00A3B150 ; void __cdecl render_texture_camera_set_position_and_direction(bool, real_point3d const*, vector3d const*, vector3d const*, real)
//.text:00A3B240 ; 
//.text:00A3B280 ; void __cdecl render_texture_camera_set_resolution(long, long)
//.text:00A3B2E0 ; void __cdecl render_texture_camera_set_target(real, real, real)
//.text:00A3B330 ; void __cdecl render_texture_camera_target_object(long, long)

// #TODO: move this
bool __cdecl screenshot_in_progress()
{
	return INVOKE(0x00610310, screenshot_in_progress);
}

void __thiscall c_player_view::render_transparents()
{
	//INVOKE_CLASS_MEMBER(0x00A3B380, c_player_view, render_transparents);

	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_z, 0);
	render_method_submit_extern_texture_static(_render_method_extern_scene_ldr_texture, 0);
	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_texaccum, 1);
	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_normal, 1);
	m_lights_view.submit_simple_light_draw_list_to_shader();

	{
		c_rasterizer_profile_scope _transparents(_rasterizer_profile_element_transparents, L"transparents");

		c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), true);
		c_transparency_renderer::set_active_camo_bounds(&m_rasterizer_camera.window_pixel_bounds, &m_rasterizer_camera.render_pixel_bounds);
		c_rasterizer::set_using_albedo_sampler(false);

		{
			c_rasterizer_profile_scope _transparents_sky(_rasterizer_profile_element_transparents, L"transparents: sky");
			c_object_renderer::submit_and_render_sky(2, m_player_index);
		}

		{
			c_rasterizer_profile_scope _transparents_misc(_rasterizer_profile_element_transparents, L"transparents: misc");
			render_misc_transparents();
		}

		c_player_view::render_effects(_effect_pass_transparents);

		{
			c_rasterizer_profile_scope _transparents_other(_rasterizer_profile_element_transparents, L"transparents: other");
			c_transparency_renderer::sort();
			c_transparency_renderer::render(true);
			c_rasterizer::set_using_albedo_sampler(c_rasterizer::surface_valid(c_rasterizer::_surface_albedo));
		}
	}

	render_method_clear_extern(_render_method_extern_texture_global_target_texaccum);
	render_method_clear_extern(_render_method_extern_texture_global_target_normal);
}

void __thiscall c_player_view::render_water()
{
	//INVOKE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);

	c_rasterizer_profile_scope _water_render(_rasterizer_profile_element_water, L"water_render");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_water);
}

void __thiscall c_player_view::render_weather_occlusion()
{
	//INVOKE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);

	c_rasterizer_profile_scope _weather_occlusion(_rasterizer_profile_element_total, L"weather_occlusion");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_weather_occlusion);
}

void c_player_view::setup_camera(long player_index, long window_count, long window_arrangement, e_output_user_index output_user_index, s_observer_result const* result, bool render_freeze)
{
	INVOKE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera, player_index, window_count, window_arrangement, output_user_index, result, render_freeze);
}

//.text:00A3B990 ; public: void __cdecl c_player_view::setup_camera_fx_parameters()
//.text:00A3BBA0 ; protected: void __cdecl c_player_view::setup_cinematic_clip_planes()
//.text:00A3BD10 ; protected: void __cdecl c_player_view::static_lighting_setup_internals()
//.text:00A3BDB0 ; protected: void __cdecl c_player_view::submit_attachments()

void __thiscall c_player_view::distortion_generate()
{
	//INVOKE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

	c_rasterizer_profile_scope _distortion_generate(_rasterizer_profile_element_distortions, L"distortion_generate");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, distortion_generate);
}

void c_player_view::frame_advance()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	effects_frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	effects_frame_advance_gpu(g_main_render_timing_data->game_seconds_elapsed);
	c_water_renderer::frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	c_patchy_fog::frame_advance_all(g_main_render_timing_data->game_seconds_elapsed);
}

