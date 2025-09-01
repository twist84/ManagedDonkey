#include "render/views/render_player_view.hpp"

#include "effects/screen_shader.hpp"
#include "game/game_engine_display.hpp"
#include "interface/c_controller.hpp"
#include "interface/interface_constants.hpp"
#include "interface/overhead_map.hpp"
#include "interface/user_interface.hpp"
#include "main/main_render.hpp"
#include "main/main_screenshot.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_occlusion_queries.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "rasterizer/rasterizer_render_targets.hpp"
#include "rasterizer/rasterizer_stipple.hpp"
#include "rasterizer/rasterizer_synchronization.hpp"
#include "render/render.hpp"
#include "render/render_debug_commands.hpp"
#include "render/render_flags.hpp"
#include "render/render_lens_flares.hpp"
#include "render/render_transparents.hpp"
#include "render/render_tron_effect.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"
#include "render/views/split_screen_config.hpp"
#include "render_methods/render_method_submit.hpp"

enum
{
	k_vs_decal_sprite = 228,
	k_vs_decal_pixel_kill_enabled = 2,
	k_ps_decal_fade = 32,
};

REFERENCE_DECLARE(0x019147B8, real32, g_particle_hack_near_fade_scale);
REFERENCE_DECLARE(0x019147BC, real32, render_debug_depth_render_scale_r);
REFERENCE_DECLARE(0x019147C0, real32, render_debug_depth_render_scale_g);
REFERENCE_DECLARE(0x019147C4, real32, render_debug_depth_render_scale_b);
REFERENCE_DECLARE(0x050FB3FC, int32, render_debug_depth_render);
REFERENCE_DECLARE(0x01694EC8, const c_screen_postprocess::s_settings* const, c_screen_postprocess::x_settings);
REFERENCE_DECLARE(0x019147CC, int32, g_distortion_conditional_rendering_index);

HOOK_DECLARE_CLASS_MEMBER(0x00A38040, c_player_view, apply_distortions);
HOOK_DECLARE_CLASS_MEMBER(0x00A39560, c_player_view, generate_distortions);
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
HOOK_DECLARE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BDF0, c_player_view, submit_distortions);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests);

void c_player_view::animate_water()
{
	//INVOKE_CLASS_MEMBER(0x00A38030, c_player_view, animate_water);

	c_rasterizer_profile_scope _water_animate(_rasterizer_profile_element_distortions, L"water_animate");

	c_water_renderer::ripple_apply();
	c_water_renderer::ripple_slope();
}

void __thiscall c_player_view::apply_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A38040, c_player_view, apply_distortions);

	c_rasterizer_profile_scope _distortion_apply(_rasterizer_profile_element_distortions, L"distortion_apply");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, apply_distortions);
}

void c_player_view::create_frame_textures(int32 player_index)
{
	INVOKE_CLASS_MEMBER(0x00A38D70, c_player_view, create_frame_textures, player_index);
}

void __thiscall c_player_view::generate_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A39560, c_player_view, generate_distortions);

	//bool depth_test = true;
	//c_player_render_camera_iterator player_camera_iterator{};
	//if (c_rasterizer::get_is_tiling_enabled() || player_camera_iterator.get_window_count() != 1)
	//{
	//	depth_test = false;
	//}
	bool depth_test = false;
	c_transparency_renderer::render(depth_test);

	c_rasterizer_profile_scope _chud_distortion(_rasterizer_profile_element_distortions, L"chud distortion");

	int32 user_index = m_camera_user_data.user_index;
	int32 surface_height = c_rasterizer::get_surface_height(c_rasterizer::_surface_distortion);
	int32 surface_width = c_rasterizer::get_surface_width(c_rasterizer::_surface_distortion);
	chud_apply_distortion(user_index, surface_width, surface_height);
}

void __cdecl c_player_view::generate_distortions_callback(int32 __formal)
{
	//INVOKE(0x00A395D0, c_player_view::generate_distortions_callback, __formal);

	c_player_view::x_current_player_view->generate_distortions();
}

void __cdecl c_player_view::get_player_render_camera_orientation(real_matrix4x3* camera)
{
	//INVOKE(0x00A39640, get_player_render_camera_orientation, camera);

	ASSERT(camera);
	ASSERT(c_player_view::x_current_player_view, "cannot get the render camera when not in the player view render loop");

	const render_camera* rasterizer_camera = c_player_view::x_current_player_view->get_rasterizer_camera();

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

// $TODO move this
REFERENCE_DECLARE_ARRAY(0x0510D8D0, c_render_surface_group, c_render_surfaces_interface::m_render_surface_groups, 60);

// $TODO move this
void __cdecl sub_14E56A0(int32 player_index, c_player_view* player_view)
{
	INVOKE(0x014E56A0, sub_14E56A0, player_index, player_view);
}

// $TODO move this
void __cdecl vision_mode_render(int32 player_index, const c_player_view* player_view, real32 a3, real32 a4, real32 a5, int32 a6, int32 a7)
{
	INVOKE(0x014E3EE0, vision_mode_render, player_index, player_view, a3, a4, a5, a6, a7);
}

void __thiscall c_player_view::render_()
{
	//HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_);

	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	s_screen_effect_settings screen_effect_settings{};
	s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
	screen_effect_sample(
		&m_rasterizer_camera.position,
		&m_rasterizer_camera.forward,
		&screen_effect_settings,
		&screen_effect_shader_sample_result,
		m_camera_user_data.user_index);

	c_rasterizer::set_current_splitscreen_res(m_camera_user_data.m_splitscreen_res_index);

	if (!game_engine_suppress_render_scene(m_camera_user_data.user_index))
	{
		render_method_clear_externs();

		s_cluster_reference cluster_reference{};
		c_world_view::get_starting_cluster(&cluster_reference);

		c_atmosphere_fog_interface::compute_cluster_weights(cluster_reference, &rasterizer_camera->position);

		{
			real32 horizontal_field_of_view = observer_get_camera(m_camera_user_data.user_index)->horizontal_field_of_view;
			if (horizontal_field_of_view < k_real_epsilon)
			{
				horizontal_field_of_view = k_real_epsilon;
			}
			g_particle_hack_near_fade_scale = 1.0f / horizontal_field_of_view;
		}

		c_player_view::submit_attachments();

		bool water_updated = c_water_renderer::update_water_part_list();
		if (water_updated && render_water_interaction_enabled && c_water_renderer::is_active_ripple_exist())
		{
			c_player_view::animate_water();
		}

		{
			c_rasterizer_profile_scope _chud_turbulence(_rasterizer_profile_element_interface_hud, L"chud turbulence");
			chud_draw_turbulence(m_camera_user_data.user_index);
		}

		c_player_view::setup_camera_fx_parameters(screen_effect_settings.exposure_boost);
		c_player_view::setup_cinematic_clip_planes();
		m_lights_view.clear_simple_light_draw_list(m_camera_user_data.user_index);
		m_lights_view.build_simple_light_draw_list(m_camera_user_data.player_window_index);

		{
			c_atmosphere_fog_interface::invalidate_atmosphere_constants();

			s_weighted_atmosphere_parameters weighted_atmosphere_parameters{};
			c_atmosphere_fog_interface::populate_atmosphere_parameters(cluster_reference, &weighted_atmosphere_parameters);

			c_atmosphere_fog_interface::set_default_atmosphere_constants(&weighted_atmosphere_parameters);
			c_atmosphere_fog_interface::restore_atmosphere_constants();
		}

		rasterizer_stipple_initialize();
		c_decal_system::submit_all();

		bool rendering_albedo = c_player_view::render_albedo();

		if (screenshot_allow_postprocess())
		{
			c_screen_postprocess::sub_A62710(
				&m_rasterizer_projection,
				&m_rasterizer_camera,
				&m_last_frame_motion_blur_state.view_matrix,
				m_last_frame_motion_blur_state.projection_matrix,
				c_rasterizer::_surface_color_half_fp16_0,
				c_rasterizer::_surface_depth_fp32,
				c_rasterizer::_surface_color_half_fp16_1);
		}

		if (rendering_albedo)
		{
			c_rasterizer::set_using_albedo_sampler(true);
			c_player_view::submit_occlusion_tests(false, true);

			if (m_stall_cpu_to_wait_for_gpu)
			{
				c_cpu_gpu_synchronizer::wait_for_gpu_to_catch_up();
				m_stall_cpu_to_wait_for_gpu = false;
			}

			if (render_debug_toggle_default_static_lighting)
			{
				c_player_view::render_static_lighting();
			}

			if (render_debug_toggle_default_dynamic_lighting)
			{
				c_player_view::render_lightmap_shadows();

				c_rasterizer::setup_targets_static_lighting(
					m_render_exposure,
					m_illum_render_scale,
					true,
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					false);

				IDirect3DSurface9* accum_LDR_surface = c_render_surfaces_interface::get_render_surface(c_rasterizer::_surface_accum_LDR)->m_d3d_surface;
				IDirect3DSurface9* accum_HDR_surface = c_render_surfaces_interface::get_render_surface(c_rasterizer::_surface_accum_HDR)->m_d3d_surface;
				IDirect3DSurface9* depth_stencil_surface = c_render_surfaces_interface::get_render_surface(c_rasterizer::_surface_depth_stencil)->m_d3d_surface;

				m_lights_view.render(
					m_camera_user_data.user_index,
					m_camera_user_data.player_window_index,
					accum_LDR_surface,
					accum_HDR_surface,
					depth_stencil_surface);
			}

			if (render_debug_toggle_default_sfx)
			{
				c_rasterizer::setup_targets_static_lighting(
					m_render_exposure,
					m_illum_render_scale,
					c_screen_postprocess::x_settings->m_postprocess || screenshot_in_progress(),
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					true);

				if (!render_debug_depth_render)
				{
					c_player_view::render_first_person(false);
				}

				if (render_debug_toggle_default_sfx)
				{
					if (c_rasterizer::get_is_tiling_enabled())
					{
						c_tron_effect::resolve_and_process_z_camera(m_camera_user_data.player_window_index, &rasterizer_camera->window_pixel_bounds, false);
					}

					c_screen_postprocess::render_ssao_old(
						&m_rasterizer_projection,
						&m_rasterizer_camera,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_accum_HDR,
						c_rasterizer::_surface_color_half_fp16_0);

					if (screenshot_allow_postprocess())
					{
						c_screen_postprocess::postprocess_ssr(
							&m_rasterizer_projection,
							&m_rasterizer_camera,
							c_rasterizer::_surface_accum_LDR,
							c_rasterizer::_surface_accum_HDR,
							c_rasterizer::_surface_normal,
							c_rasterizer::_surface_depth_fp32,
							c_rasterizer::_surface_gbuf,
							c_rasterizer::_surface_post_LDR,
							c_rasterizer::_surface_post_HDR,
							c_rasterizer::_surface_depth_half_fp32,
							c_rasterizer::_surface_color_half_fp16_0,
							c_rasterizer::_surface_color_half_fp16_1,
							c_rasterizer::_surface_normal_half,
							c_rasterizer::_surface_post_half_LDR);
					}

					c_rasterizer::setup_targets_static_lighting(
						m_render_exposure,
						m_illum_render_scale,
						c_screen_postprocess::x_settings->m_postprocess || screenshot_in_progress(),
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);

					if (water_updated || render_underwater_fog_enabled)
					{
						c_player_view::render_water();
					}

					if (g_rasterizer_game_states->patchy_fog)
					{
						c_player_view::queue_patchy_fog();
					}

					c_player_view::render_transparents();

					c_player_view::submit_distortions();
					bool distortion_history = c_render_globals::get_distortion_visible();
					if (distortion_history || water_updated || render_underwater_fog_enabled)
					{
						distortion_history = true;
					}
					c_render_globals::set_distortion_history(distortion_history);

					{
						c_rasterizer_profile_scope _game_engine(_rasterizer_profile_element_game_engine, L"game_engine");
						game_engine_render(m_camera_user_data.user_index);
					}

					c_player_view::apply_distortions();

					if (!render_debug_depth_render)
					{
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->m_postprocess || screenshot_in_progress(), true);
						c_player_view::render_first_person(true);
					}

					{
						c_rasterizer_profile_scope _setup_targets_static_lighting_alpha_blend(_rasterizer_profile_element_total, L"setup_targets_static_lighting_alpha_blend");
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->m_postprocess || screenshot_in_progress(), false);
					}

					c_player_view::render_lens_flares();
					c_player_view::submit_occlusion_tests(true, false);

					{
						c_rasterizer_profile_scope _chud_draw_screen_LDR(_rasterizer_profile_element_total, L"chud_draw_screen_LDR");

						if (chud_generate_damage_flash_texture(m_camera_user_data.user_index))
						{
							c_rasterizer::setup_targets_static_lighting_alpha_blend(false, true);
							c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f, true);
							c_rasterizer::set_render_target(1, c_rasterizer::_surface_none, 0xFFFFFFFF);
							chud_draw_screen_LDR(m_camera_user_data.user_index);
						}
					}

					render_setup_window(&m_rasterizer_camera, &m_rasterizer_projection);
					c_rasterizer::setup_targets_static_lighting(
						m_render_exposure,
						m_illum_render_scale,
						true,
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);
					c_player_view::render_weather_occlusion();
				}
			}

			if (screenshot_allow_postprocess())
			{
				c_screen_postprocess::postprocess_player_view(
					m_window_game_state->m_camera_fx_values,
					&m_rasterizer_projection,
					&m_rasterizer_camera,
					screen_effect_settings,
					m_camera_user_data.m_splitscreen_res_index,
					&m_observer_depth_of_field,
					0);

				if (render_debug_toggle_default_lightmaps_texaccum == 3)
				{
					c_screen_postprocess::setup_rasterizer_for_postprocess(false);
					c_screen_postprocess::copy(
						c_rasterizer_globals::_shader_copy_scaled,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_disable,
						c_rasterizer::_sampler_filter_point,
						c_rasterizer::_sampler_address_clamp,
						1.0f,
						1.0f,
						1.0f,
						1.0f,
						NULL);
				}
			}
		}
	}

	if (screenshot_allow_ui_render())
	{
		c_rasterizer_profile_scope _interface_and_hud(_rasterizer_profile_element_interface_hud, L"interface_and_hud");

		c_rasterizer::restore_last_viewport();
		c_rasterizer::restore_last_scissor_rect();
		c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f, true);

		s_screen_effect_settings screen_effect_settings{};
		s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
		screen_effect_sample(
			&m_rasterizer_camera.position,
			&m_rasterizer_camera.forward,
			&screen_effect_settings,
			&screen_effect_shader_sample_result,
			m_camera_user_data.user_index);

		{
			c_rasterizer_profile_scope _vision_mode(_rasterizer_profile_element_total, L"vision_mode");

			if (screen_effect_settings.vision_mode > 0.0f)
			{
				vision_mode_render(
					m_camera_user_data.player_window_index,
					this,
					screen_effect_settings.vision_mode,
					g_main_render_timing_data->game_dt,
					screen_effect_settings.vision_noise,
					0,
					0);
			}
			else
			{
				sub_14E56A0(
					m_camera_user_data.player_window_index,
					this);
			}
		}

		render_screen_shaders(&screen_effect_shader_sample_result, 1, c_rasterizer::get_display_surface(), c_rasterizer::_surface_none, NULL);

		{
			c_rasterizer_profile_scope _chud_draw_screen(_rasterizer_profile_element_total, L"chud_draw_screen");

			// added by us
			c_rasterizer::set_viewport(m_rasterizer_camera.window_pixel_bounds, 0.0f, 1.0f);
			c_rasterizer::set_scissor_rect(&m_rasterizer_camera.window_pixel_bounds);

			chud_draw_screen(m_camera_user_data.user_index);

			// added by us
			c_rasterizer::restore_last_viewport();
			c_rasterizer::restore_last_scissor_rect();
		}
		{
			c_rasterizer_profile_scope _fade_to_black(_rasterizer_profile_element_total, L"fade_to_black");
			game_engine_render_fade_to_black(m_camera_user_data.user_index);
		}
		{
			c_rasterizer_profile_scope _user_interface_render(_rasterizer_profile_element_total, L"user_interface_render");

			rectangle2d display_pixel_bounds{};
			interface_get_current_window_settings(NULL, NULL, &display_pixel_bounds, NULL);
			user_interface_render(
				m_camera_user_data.controller_index,
				m_camera_user_data.user_index,
				(e_window_index)m_camera_user_data.player_window_index,
				&display_pixel_bounds,
				c_rasterizer::get_display_surface(),
				false);
		}
		{
			c_rasterizer_profile_scope _watermarks(_rasterizer_profile_element_total, L"watermarks");
			game_engine_render_watermarks();
		}
		{
			c_rasterizer_profile_scope _chud_draw_screen_saved_film(_rasterizer_profile_element_total, L"chud_draw_screen_saved_film");
			chud_draw_screen_saved_film(m_camera_user_data.user_index);
		}
		{
			c_rasterizer_profile_scope _player_training(_rasterizer_profile_element_total, L"player_training");
			player_training_render_screen(m_camera_user_data.user_index);
		}
		{
			c_rasterizer_profile_scope _debug_render(_rasterizer_profile_element_total, L"debug_render");
		}
		{
			c_rasterizer_profile_scope _overhead_map(_rasterizer_profile_element_total, L"overhead_map");
			overhead_map_render();
		}
		{
			c_rasterizer_profile_scope _player_effect_render(_rasterizer_profile_element_total, L"player_effect_render");
			player_effect_render(m_camera_user_data.user_index);
		}
	}

	game_engine_render_debug(m_camera_user_data.user_index);

	if (!screenshot_sub_610260())
	{
		render_debug_window_render(m_camera_user_data.user_index);
	}

	c_rasterizer::end();
}

bool __thiscall c_player_view::render_albedo()
{
	//return INVOKE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);

	c_rasterizer_profile_scope _albedo(_rasterizer_profile_element_texaccum, L"albedo");

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_player_view, render_albedo);
	return result;
}

void __cdecl c_player_view::render_albedo_decals(bool render_object_decals, bool render_structure_decals)
{
	//INVOKE(0x00A3A310, c_player_view::render_albedo_decals, render_object_decals, render_structure_decals);

	c_rasterizer_profile_scope _decorators(_rasterizer_profile_element_total, L"decorators");

	HOOK_INVOKE_CLASS(, c_player_view, render_albedo_decals, decltype(&c_player_view::render_albedo_decals), render_object_decals, render_structure_decals);

	//c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_read);
	//c_rasterizer::begin_high_quality_blend();
	//
	//if (render_object_decals)
	//{
	//	int pixel_kill_enabled = 1;
	//	c_rasterizer::set_vertex_shader_constant_bool(k_vs_decal_pixel_kill_enabled, 1, &pixel_kill_enabled);
	//
	//	real_vector4d fade{};
	//	set_real_vector4d(&fade, 1.0f, 1.0f, 1.0f, 1.0f);
	//	c_rasterizer::set_pixel_shader_constant(k_ps_decal_fade, 1, &fade);
	//
	//	real_vector4d sprite{};
	//	set_real_vector4d(&sprite, 0.0f, 0.0f, 1.0f, 1.0f);
	//	c_rasterizer::set_vertex_shader_constant(k_vs_decal_sprite, 1, &sprite);
	//
	//	c_object_renderer::render_albedo_decals();
	//}
	//
	//if (render_structure_decals)
	//{
	//	c_decal_system::render_all(_pass_post_albedo);
	//}
}

//.text:00A3A3C0 ; protected: void __cdecl c_player_view::render_dynamic_lights(IDirect3DSurface9* ldr_surface, IDirect3DSurface9* hdr_surface, IDirect3DSurface9* depth_surface)

void __thiscall c_player_view::render_effects(e_effect_pass effect_pass)
{
	//INVOKE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects, effect_pass);

	c_rasterizer_profile_scope _render_effects(_rasterizer_profile_element_effects, L"render_effects");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_effects, effect_pass);

	//if (!game_is_ui_shell() || user_interface_should_render_fancy())
	//{
	//	effects_render(m_camera_user_data.user_index, effect_pass);
	//}
}

void __thiscall c_player_view::render_first_person(bool render_only_transparents)
{
	//INVOKE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);

	c_rasterizer_profile_scope _first_person(_rasterizer_profile_element_total, L"first_person");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person, render_only_transparents);
}

void __thiscall c_player_view::render_first_person_albedo()
{
	//INVOKE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);

	c_rasterizer_profile_scope _first_person_albedo(_rasterizer_profile_element_total, L"first_person_albedo");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person_albedo);

	//if (player_control_get_zoom_level(m_camera_user_data.user_index) == 0xFFFF)
	//{
	//	g_rendering_first_person = true;
	//	c_visible_items::push_marker();
	//	c_object_renderer::push_marker();
	//
	//	*m_first_person_view.get_render_camera_modifiable() = m_render_camera;
	//	*m_first_person_view.get_rasterizer_camera_modifiable() = m_rasterizer_camera;
	//	m_first_person_view.m_default_rasterizer_camera = &m_rasterizer_camera;
	//
	//	m_first_person_view.compute_visibility(m_camera_user_data.user_index);
	//	m_first_person_view.render_submit_visibility(m_camera_user_data.user_index, false);
	//
	//	c_view::begin(&m_first_person_view);
	//	m_first_person_view.override_projection(true);
	//	m_first_person_view.render_albedo(m_camera_user_data.user_index);
	//	c_view::end();
	//
	//	c_visible_items::pop_marker();
	//	c_object_renderer::pop_marker();
	//	g_rendering_first_person = false;
	//}
}

//.text:00A3A6B0 ; 

void __thiscall c_player_view::render_lens_flares()
{
	//INVOKE_CLASS_MEMBER(0x00A3A6C0, c_player_view::render_lens_flares);

	c_rasterizer::set_using_albedo_sampler(false);
	rasterizer_occlusions_retrieve(m_camera_user_data.player_window_index);

	{
		c_rasterizer_profile_scope _lens_flares(_rasterizer_profile_element_transparents, L"lens flares");
		lens_flares_render(m_camera_user_data.user_index);
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

void __cdecl render_texture_camera_align_to_attached_object()
{
	INVOKE(0x00A3AB00, render_texture_camera_align_to_attached_object);

	//g_render_texture_camera_globals->orientation_type = _texture_camera_orientation_attached_object_orient;
}

void __cdecl render_texture_camera_attach_to_object(int32 object_index, int32 marker_id)
{
	INVOKE(0x00A3AB20, render_texture_camera_attach_to_object, object_index, marker_id);

	//if (object_try_and_get_and_verify_type(object_index, NONE))
	//{
	//	g_render_texture_camera_globals->position_type = _texture_camera_type_marker_id;
	//	g_render_texture_camera_globals->position.object_index = object_index;
	//	g_render_texture_camera_globals->position.marker_id = marker_id;
	//	g_render_texture_camera_globals->position_offset.i = 0.0;
	//	g_render_texture_camera_globals->position_offset.j = 0.0;
	//	g_render_texture_camera_globals->position_offset.k = 0.0;
	//}
	//else
	//{
	//	event(_event_warning, "texture camera can't find object to attach to");
	//}
}

void __cdecl render_texture_camera_dispose()
{
	INVOKE(0x00A3AB90, render_texture_camera_dispose);
}

void __cdecl render_texture_camera_dispose_from_old_map()
{
	INVOKE(0x00A3ABA0, render_texture_camera_dispose_from_old_map);
}

void __cdecl render_texture_camera_enable_dynamic_lights(bool enable)
{
	INVOKE(0x00A3ABB0, render_texture_camera_enable_dynamic_lights, enable);

	//g_render_texture_camera_globals->dynamic_lights = enable;
}

real32 __cdecl render_texture_camera_get_aspect_ratio()
{
	return INVOKE(0x00A3ABD0, render_texture_camera_get_aspect_ratio);

	//return g_render_texture_camera_globals->aspect_ratio;
}

int32 __cdecl render_texture_camera_get_height()
{
	return INVOKE(0x00A3ABF0, render_texture_camera_get_height);

	//return g_render_texture_camera_globals->height;
}

int32 __cdecl render_texture_camera_get_render_mode()
{
	return INVOKE(0x00A3AC10, render_texture_camera_get_render_mode);

	//return g_render_texture_camera_globals->render_mode;
}

int32 __cdecl render_texture_camera_get_sub_A3AC30()
{
	return INVOKE(0x00A3AC30, render_texture_camera_get_sub_A3AC30);

	//if (!object_try_and_get_and_verify_type(g_render_texture_camera_globals->__unknown40_object_index, NONE))
	//{
	//	g_render_texture_camera_globals->__unknown40_object_index = NONE;
	//}
	//
	//return g_render_texture_camera_globals->__unknown40_object_index;
}

int32 __cdecl render_texture_camera_get_target_object_index()
{
	return INVOKE(0x00A3AC70, render_texture_camera_get_target_object_index);

	//if (g_render_texture_camera_globals->target_type == _texture_camera_type_position)
	//{
	//	return NONE;
	//}
	//
	//int32 object_index = g_render_texture_camera_globals->target.object_index;
	//if (!object_try_and_get_and_verify_type(object_index, NONE))
	//{
	//	return NONE;
	//}
	//
	//return object_index;
}

int32 __cdecl render_texture_camera_get_width()
{
	return INVOKE(0x00A3ACB0, render_texture_camera_get_width);

	//return g_render_texture_camera_globals->width;
}

void __cdecl render_texture_camera_initialize()
{
	INVOKE(0x00A3ACD0, render_texture_camera_initialize);

	//g_render_texture_camera_globals = (s_render_texture_camera_globals*)g_render_texture_camera_globals_allocator.allocate(
	//	sizeof(s_render_texture_camera_globals),
	//	"render texture globals");
}

void __cdecl render_texture_camera_initialize_for_new_map()
{
	INVOKE(0x00A3AD30, render_texture_camera_initialize_for_new_map);

	//csmemset(g_render_texture_camera_globals, 0, sizeof(s_render_texture_camera_globals));
	//g_render_texture_camera_globals->active = false;
	//g_render_texture_camera_globals->dynamic_lights = false;
	//g_render_texture_camera_globals->render_mode = _texture_camera_render_default;
	//g_render_texture_camera_globals->position_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->target_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->orientation_type = _texture_camera_orientation_target_auto;
	//g_render_texture_camera_globals->projection_type = _texture_camera_projection_projective;
	//g_render_texture_camera_globals->fov_type = _texture_camera_fov_frame_target;
	//g_render_texture_camera_globals->__unknown40_object_index = NONE;
	//g_render_texture_camera_globals->position.position.x = 0.0f;
	//g_render_texture_camera_globals->position.position.y = 0.0f;
	//g_render_texture_camera_globals->position.position.z = 0.0f;
	//g_render_texture_camera_globals->position_offset.i = 0.0f;
	//g_render_texture_camera_globals->position_offset.j = 0.0f;
	//g_render_texture_camera_globals->position_offset.k = 0.0f;
	//g_render_texture_camera_globals->target.position.x = 1.0f;
	//g_render_texture_camera_globals->target.position.y = 0.0f;
	//g_render_texture_camera_globals->target.position.z = 0.0f;
	//g_render_texture_camera_globals->fov.target_scale = 1.0f;
	//g_render_texture_camera_globals->aspect_ratio = 1.7777778f;
	//g_render_texture_camera_globals->width = 384;
	//g_render_texture_camera_globals->height = 384;
}

void __cdecl render_texture_camera_off()
{
	INVOKE(0x00A3AE50, render_texture_camera_off);

	//g_render_texture_camera_globals->active = false;
}

void __cdecl render_texture_camera_on()
{
	INVOKE(0x00A3AE70, render_texture_camera_on);

	//g_render_texture_camera_globals->active = true;
}

void __cdecl render_texture_camera_position_world_offset(real32 offset_x, real32 offset_y, real32 offset_z)
{
	INVOKE(0x00A3AE90, render_texture_camera_position_world_offset, offset_x, offset_y, offset_z);

	//g_render_texture_camera_globals->position_offset.i = offset_x;
	//g_render_texture_camera_globals->position_offset.j = offset_x;
	//g_render_texture_camera_globals->position_offset.k = offset_y;
}

void __cdecl render_texture_camera_render_mode(int32 mode)
{
	INVOKE(0x00A3AEE0, render_texture_camera_render_mode, mode);

	//g_render_texture_camera_globals->render_mode = (e_texture_camera_render_mode)MAX(_texture_camera_render_default, MIN(mode, _texture_camera_render_target_only));
}

void __cdecl render_texture_camera_set_aspect_ratio(real32 aspect_ratio)
{
	INVOKE(0x00A3AF20, render_texture_camera_set_aspect_ratio, aspect_ratio);

	//g_render_texture_camera_globals->aspect_ratio = aspect_ratio;
}

void __cdecl render_texture_camera_set_fov(real32 vertical_field_of_view_degrees)
{
	INVOKE(0x00A3AF50, render_texture_camera_set_fov, vertical_field_of_view_degrees);

	//g_render_texture_camera_globals->fov_type = _texture_camera_fov_fixed;
	//g_render_texture_camera_globals->fov.vertical_fov_radians = CLAMP(vertical_field_of_view_degrees, 1.0f, 180.0f) * DEG;
}

void __cdecl render_texture_camera_set_fov_frame_target(real32 target_zoom)
{
	INVOKE(0x00A3AFB0, render_texture_camera_set_fov_frame_target, target_zoom);

	//g_render_texture_camera_globals->fov_type = _texture_camera_fov_frame_target;
	//g_render_texture_camera_globals->fov.target_scale = target_zoom;
}

void __cdecl render_texture_camera_set_object_marker(int32 object_index, int32 marker_id, real32 vertical_field_of_view_degrees)
{
	INVOKE(0x00A3AFF0, render_texture_camera_set_object_marker, object_index, marker_id, vertical_field_of_view_degrees);

	//object_marker marker[1]{};
	//if (object_index != NONE
	//	&& object_try_and_get_and_verify_type(object_index, NONE)
	//	&& object_get_markers_by_string_id(object_index, marker_id, marker, NUMBEROF(marker) & 0xFFFF))
	//{
	//	g_render_texture_camera_globals->active = true;
	//	g_render_texture_camera_globals->position_type = _texture_camera_type_marker_id;
	//	g_render_texture_camera_globals->target_type = _texture_camera_type_position;
	//	g_render_texture_camera_globals->orientation_type = _texture_camera_orientation_position_marker;
	//	g_render_texture_camera_globals->projection_type = _texture_camera_projection_projective;
	//	g_render_texture_camera_globals->position.object_index = object_index;
	//	g_render_texture_camera_globals->position.marker_id = marker_id;
	//	g_render_texture_camera_globals->position_offset.i = 0.0f;
	//	g_render_texture_camera_globals->position_offset.j = 0.0f;
	//	g_render_texture_camera_globals->position_offset.k = 0.0f;
	//	render_texture_camera_set_fov(vertical_field_of_view_degrees);
	//}
	//else
	//{
	//	g_render_texture_camera_globals->active = false;
	//
	//	if (object_index != NONE)
	//	{
	//		event(_event_error, "### WARNING texture camera can't attach to object! detaching ...");
	//	}
	//}
}

void __cdecl render_texture_camera_set_position(real32 position_x, real32 position_y, real32 position_z)
{
	INVOKE(0x00A3B0F0, render_texture_camera_set_position, position_x, position_y, position_z);

	//g_render_texture_camera_globals->position_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->position.position.x = position_x;
	//g_render_texture_camera_globals->position.position.y = position_y;
	//g_render_texture_camera_globals->position.position.z = position_z;
	//g_render_texture_camera_globals->position_offset.i = 0.0f;
	//g_render_texture_camera_globals->position_offset.j = 0.0f;
	//g_render_texture_camera_globals->position_offset.k = 0.0f;
}

void __cdecl render_texture_camera_set_position_and_direction(bool enabled, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, real32 vertical_field_of_view_degrees)
{
	INVOKE(0x00A3B150, render_texture_camera_set_position_and_direction, enabled, position, forward, up, vertical_field_of_view_degrees);

	//g_render_texture_camera_globals->position_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->target_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->orientation_type = _texture_camera_orientation_explicit;
	//g_render_texture_camera_globals->projection_type = _texture_camera_projection_projective;
	//g_render_texture_camera_globals->position.position = *position;
	//g_render_texture_camera_globals->position_offset.i = 0.0f;
	//g_render_texture_camera_globals->position_offset.j = 0.0f;
	//g_render_texture_camera_globals->position_offset.k = 0.0f;
	//g_render_texture_camera_globals->orientation.forward = *forward;
	//g_render_texture_camera_globals->orientation.up = *up;
	//render_texture_camera_set_fov(vertical_field_of_view_degrees);
}

//.text:00A3B240 ; render_texture_camera_set_?
void __cdecl render_texture_camera_set_sub_A3B240(int32 object_index)
{
	INVOKE(0x00A3B240, render_texture_camera_set_sub_A3B240, object_index);

	//if (object_try_and_get_and_verify_type(object_index, NONE))
	//{
	//	g_render_texture_camera_globals->__unknown40_object_index = object_index;
	//}
}

void __cdecl render_texture_camera_set_resolution(int32 width, int32 height)
{
	INVOKE(0x00A3B280, render_texture_camera_set_resolution, width, height);

	//g_render_texture_camera_globals->width = MAX(1, MIN(width, 512));
	//g_render_texture_camera_globals->height = MAX(1, MIN(height, 288));
}

void __cdecl render_texture_camera_set_target(real32 position_x, real32 position_y, real32 position_z)
{
	INVOKE(0x00A3B2E0, render_texture_camera_set_target, position_x, position_y, position_z);

	//g_render_texture_camera_globals->target_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->target.position.x = position_x;
	//g_render_texture_camera_globals->target.position.y = position_y;
	//g_render_texture_camera_globals->target.position.z = position_z;
}

void __cdecl render_texture_camera_target_object(int32 object_index, int32 marker_id)
{
	INVOKE(0x00A3B330, render_texture_camera_target_object, object_index, marker_id);

	//if (object_try_and_get_and_verify_type(object_index, NONE))
	//{
	//	g_render_texture_camera_globals->target_type = _texture_camera_type_marker_id;
	//	g_render_texture_camera_globals->target.object_index = object_index;
	//	g_render_texture_camera_globals->target.marker_id = marker_id;
	//}
	//else
	//{
	//	event(_event_warning, "texture camera can't find object to target");
	//}
}

void __thiscall c_player_view::render_transparents()
{
	//INVOKE_CLASS_MEMBER(0x00A3B380, c_player_view, render_transparents);

	render_method_submit_single_extern(_render_method_extern_texture_global_target_z, false);
	render_method_submit_single_extern(_render_method_extern_scene_ldr_texture, false);
	render_method_submit_single_extern(_render_method_extern_texture_global_target_texaccum, true);
	render_method_submit_single_extern(_render_method_extern_texture_global_target_normal, true);
	m_lights_view.submit_simple_light_draw_list_to_shader();

	{
		c_rasterizer_profile_scope _transparents(_rasterizer_profile_element_transparents, L"transparents");

		c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->m_postprocess || screenshot_in_progress(), true);
		c_transparency_renderer::set_active_camo_bounds(&m_rasterizer_camera.window_pixel_bounds, &m_rasterizer_camera.render_pixel_bounds);
		c_rasterizer::set_using_albedo_sampler(false);

		{
			c_rasterizer_profile_scope _transparents_sky(_rasterizer_profile_element_transparents, L"transparents: sky");
			c_object_renderer::submit_and_render_sky(2, m_camera_user_data.player_window_index);
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

	c_rasterizer::resolve_surface(c_rasterizer::_surface_accum_LDR, 0, NULL, 0, 0);
	c_rasterizer::stretch_rect(c_rasterizer::_surface_accum_LDR, c_rasterizer::_surface_post_LDR);

	render_method_submit_single_extern(_render_method_extern_scene_ldr_texture, false);
	render_method_submit_single_extern(_render_method_extern_texture_global_target_z, false);

	c_water_renderer::render_underwater_fog();
	c_water_renderer::render_tessellation(screenshot_in_progress());
	c_water_renderer::render_shading();

	render_method_clear_extern(_render_method_extern_scene_ldr_texture);
	render_method_clear_extern(_render_method_extern_texture_global_target_z);
}

void __thiscall c_player_view::render_weather_occlusion()
{
	//INVOKE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);

	c_rasterizer_profile_scope _weather_occlusion(_rasterizer_profile_element_total, L"weather_occlusion");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_weather_occlusion);
}

void c_player_view::restore_to_display_surface()
{
	c_rasterizer::e_splitscreen_res current_splitscreen_res = c_rasterizer::g_current_splitscreen_res;
	if (current_splitscreen_res)
	{
		INVOKE_CLASS_MEMBER(0x00A3B5E0, c_player_view, restore_to_display_surface);
		c_rasterizer::set_current_splitscreen_res(current_splitscreen_res);
	}
}

void c_player_view::setup_camera(int32 player_window_index, int32 player_window_count, int32 player_window_arrangement, int32 user_index, const s_observer_result* observer, bool freeze_render_camera)
{
	//INVOKE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera, player_window_index, player_window_count, player_window_arrangement, user_index, observer, freeze_render_camera);

	ASSERT(VALID_INDEX(user_index, k_number_of_users));

	render_camera* rasterizer_camera = c_view::get_rasterizer_camera_modifiable();
	if (game_in_progress() && game_is_ui_shell() && user_interface_should_render_at_origin())
	{
		s_observer_result fake_observer = *observer;
		fake_observer.position = *global_origin3d;
		observer = &fake_observer;
		fake_observer.forward = *global_down3d;
		fake_observer.up = *global_forward3d;
	}
	render_view_compute_all_bounds(player_window_index, player_window_count, rasterizer_camera);

	m_camera_user_data.m_splitscreen_resolve_surface = c_rasterizer::_surface_none;
	m_camera_user_data.m_splitscreen_res_index = (c_rasterizer::e_splitscreen_res)c_splitscreen_config::get_view_bounds(player_window_index, player_window_count, c_rasterizer::get_is_widescreen())->m_render_target_resolution;

	render_camera_build(rasterizer_camera, observer);

	if (observer)
	{
		m_observer_depth_of_field = observer->depth_of_field;
	}
	else
	{
		m_observer_depth_of_field.flags = 0;
	}

	real_rectangle2d frustum_bounds{};
	render_camera_build_viewport_frustum_bounds(rasterizer_camera, &frustum_bounds);

	render_projection* rasterizer_projection = c_view::get_rasterizer_projection_modifiable();
	render_camera_build_projection(rasterizer_camera, &frustum_bounds, rasterizer_projection, 0.0f);

	if (!freeze_render_camera)
	{
		m_render_camera = *rasterizer_camera;
		m_render_projection = *rasterizer_projection;
	}

	m_camera_user_data.player_window_count = player_window_count;
	m_camera_user_data.player_window_arrangement = player_window_arrangement;
	m_camera_user_data.player_window_index = player_window_index;
	m_camera_user_data.user_index = user_index;
	m_camera_user_data.controller_index = controller_index_from_user_index(user_index);
	m_window_game_state = get_render_player_window_game_state(player_window_index);
}

void __thiscall c_player_view::setup_camera_fx_parameters(real32 exposure_boost)
{
	INVOKE_CLASS_MEMBER(0x00A3B990, c_player_view, setup_camera_fx_parameters, exposure_boost);
}

void __thiscall c_player_view::setup_cinematic_clip_planes()
{
	INVOKE_CLASS_MEMBER(0x00A3BBA0, c_player_view, setup_cinematic_clip_planes);
}

//.text:00A3BD10 ; protected: void __cdecl c_player_view::static_lighting_setup_internals()

void __thiscall c_player_view::submit_attachments()
{
	INVOKE_CLASS_MEMBER(0x00A3BDB0, c_player_view, submit_attachments);
}

void __thiscall c_player_view::submit_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A3BDF0, c_player_view, submit_distortions);

	c_rasterizer_profile_scope _distortion_generate(_rasterizer_profile_element_distortions, L"distortion_generate");

	if (chud_contain_turbulence)
	{
		c_render_globals::set_distortion_active(true);
		c_render_globals::set_distortion_visible(true);
	}

	if (c_render_globals::get_distortion_active() && c_screen_postprocess::x_settings->m_postprocess)
	{
		c_transparency_renderer::push_marker();

		{
			c_rasterizer_profile_scope _submit(_rasterizer_profile_element_distortions, L"submit");
			c_player_view::render_effects(_effect_pass_distortion);
		}

		if (c_render_globals::get_distortion_visible())
		{
			c_rasterizer_profile_scope _generate(_rasterizer_profile_element_distortions, L"generate");

			rectangle2d _distortion_pixel_bounds = m_rasterizer_camera.window_pixel_bounds;
			//_distortion_pixel_bounds.y0 /= 2;
			//_distortion_pixel_bounds.x0 /= 2;
			//_distortion_pixel_bounds.y1 /= 2;
			//_distortion_pixel_bounds.x1 /= 2;

			render_method_submit_single_extern(_render_method_extern_texture_global_target_z, false);

			//bool depth_test = true;
			//c_player_render_camera_iterator player_camera_iterator{};
			//if (c_rasterizer::get_is_tiling_enabled() || player_camera_iterator.get_window_count() != 1)
			//{
			//	depth_test = false;
			//}
			bool depth_test = false;
			c_rasterizer::setup_targets_distortion(&_distortion_pixel_bounds, depth_test);

			rasterizer_occlusion_submit(
				k_occlusion_query_type_distortion,
				6,
				0,
				m_camera_user_data.player_window_index,
				c_player_view::generate_distortions_callback);

			int32 pixels_visible_unused = 0;
			rasterizer_occlusions_get_result(
				k_occlusion_query_type_distortion,
				0,
				m_camera_user_data.player_window_index,
				&pixels_visible_unused,
				&g_distortion_conditional_rendering_index);

			c_rasterizer::resolve_surface(
				c_rasterizer::_surface_distortion,
				0,
				&_distortion_pixel_bounds,
				_distortion_pixel_bounds.x0,
				_distortion_pixel_bounds.y0);
		}

		c_transparency_renderer::pop_marker();
	}
}

void __thiscall c_player_view::submit_occlusion_tests(bool occlusion, bool conditional)
{
	//INVOKE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests, occlusion, conditional);

	c_rasterizer_profile_scope _occlusion_tests(_rasterizer_profile_element_occlusions, L"occlusion_tests");

	if (!screenshot_in_progress())
	{
		c_rasterizer::setup_occlusion_state();

		if (occlusion)
		{
			lens_flares_submit_occlusions(m_camera_user_data.user_index, _lens_flare_occlusion_type_occlusion);
		}

		if (conditional)
		{
			lens_flares_submit_occlusions(m_camera_user_data.user_index, _lens_flare_occlusion_type_conditional);
		}
	}
}

void c_player_view::frame_advance()
{
	effects_frame_advance(g_main_render_timing_data->game_dt);
	effects_frame_advance_gpu(g_main_render_timing_data->game_dt);
	c_water_renderer::frame_advance(g_main_render_timing_data->game_dt);
	c_patchy_fog::frame_advance_all(g_main_render_timing_data->game_dt);
}

