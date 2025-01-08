#include "render/views/render_player_view.hpp"

#include "effects/screen_shader.hpp"
#include "game/game_engine_display.hpp"
#include "interface/interface_constants.hpp"
#include "interface/overhead_map.hpp"
#include "interface/user_interface.hpp"
#include "main/main_screenshot.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_occlusion_queries.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "rasterizer/rasterizer_render_targets.hpp"
#include "rasterizer/rasterizer_stipple.hpp"
#include "rasterizer/rasterizer_synchronization.hpp"
#include "render/render.hpp"
#include "render/render_flags.hpp"
#include "render/render_lens_flares.hpp"
#include "render/render_transparents.hpp"
#include "render/render_tron_effect.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"
#include "render_methods/render_method_submit.hpp"

REFERENCE_DECLARE(0x019147B8, real, g_particle_hack_near_fade_scale);
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
HOOK_DECLARE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests);

void c_player_view::animate_water()
{
	//INVOKE_CLASS_MEMBER(0x00A38030, c_player_view, animate_water);

	c_rasterizer_profile_scope _water_animate(_rasterizer_profile_element_distortions, L"water_animate");

	c_water_renderer::ripple_apply();
	c_water_renderer::ripple_slope();
}

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

// #TODO: move this
struct s_surface_group_description
{
	dword_flags flags;
	IDirect3DTexture9* Texture;
	real DepthWidth;
	real __unknownC;
	dword DepthFormat;
	byte __unknown14[4];
	dword Srgb;
	dword MultiSample;
	real __unknown20;
	real __unknown24;
	dword __unknown28;
	dword __unknown2C;
	dword __unknown30;
	dword __unknown34;
	dword __unknown38;
	dword __unknown3C;
	dword Alias;
	dword __unknown44;
	dword index_unknown_of_size_54;
};

// #TODO: move this
struct c_render_surface_group
{
	s_surface_group_description description;
	long m_count_specializations;
	c_render_surface* m_default_surface;
};

// #TODO: move this
struct c_render_surfaces_interface
{
	static c_render_surface* get_render_surface_default(c_rasterizer::e_surface surface)
	{
		return m_render_surface_groups[surface].m_default_surface;
	}

	static c_render_surface_group(&m_render_surface_groups)[60];
};

// #TODO: move this
REFERENCE_DECLARE_ARRAY(0x0510D8D0, c_render_surface_group, c_render_surfaces_interface::m_render_surface_groups, 60);

// #TODO: move this
void __cdecl sub_14E56A0(long player_index, c_player_view* player_view)
{
	INVOKE(0x014E56A0, sub_14E56A0, player_index, player_view);
}

// #TODO: move this
void __cdecl vision_mode_render(long player_index, c_player_view const* player_view, real a3, real a4, real a5, long a6, long a7)
{
	INVOKE(0x014E3EE0, vision_mode_render, player_index, player_view, a3, a4, a5, a6, a7);
}

void __thiscall c_player_view::render_()
{
	//HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_);
	
	c_player_view::render_1st_pass();
	c_player_view::render_2nd_pass();
	c_player_view::render_3rd_pass();
	c_player_view::render_4th_pass();
}

void __thiscall c_player_view::render_1st_pass()
{
	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	c_rasterizer::set_current_splitscreen_res(m_camera_user_data.m_splitscreen_res_index);

	if (!game_engine_suppress_render_scene(m_camera_user_data.user_index))
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_game_states);

		render_method_clear_externs();

		s_cluster_reference cluster_reference{};
		c_world_view::get_starting_cluster(&cluster_reference);

		c_atmosphere_fog_interface::compute_cluster_weights(cluster_reference, &rasterizer_camera->position);

		{
			real horizontal_field_of_view = observer_get_camera(m_camera_user_data.user_index)->horizontal_field_of_view;
			if (horizontal_field_of_view < k_real_epsilon)
				horizontal_field_of_view = k_real_epsilon;
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

		s_screen_effect_settings screen_effect_settings{};
		s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
		screen_effect_sample(
			&rasterizer_camera->position,
			&rasterizer_camera->forward,
			&screen_effect_settings,
			&screen_effect_shader_sample_result,
			m_camera_user_data.user_index);

		c_player_view::setup_camera_fx_parameters(screen_effect_settings.exposure_boost);
		c_player_view::setup_cinematic_clip_planes();
		m_lights_view.clear_simple_light_draw_list(m_camera_user_data.user_index);
		m_lights_view.build_simple_light_draw_list(m_camera_user_data.player_window_index);

		{
			c_atmosphere_fog_interface::sub_A397D0();

			s_weighted_atmosphere_parameters weighted_atmosphere_parameters{};
			c_atmosphere_fog_interface::populate_atmosphere_parameters(cluster_reference, &weighted_atmosphere_parameters);

			c_atmosphere_fog_interface::set_default_atmosphere_constants(&weighted_atmosphere_parameters);
			c_atmosphere_fog_interface::restore_atmosphere_constants();
		}

		rasterizer_stipple_initialize();
		c_decal_system::submit_all();

		bool rendering_albedo = c_player_view::render_albedo();

		if (screenshot_allow_postprocess())
			c_screen_postprocess::sub_A62710(
				&m_rasterizer_projection,
				&m_rasterizer_camera,
				&m_last_frame_motion_blur_state.view_matrix,
				m_last_frame_motion_blur_state.projection_matrix,
				c_rasterizer::_surface_color_half_fp16_0,
				c_rasterizer::_surface_depth_fp32,
				c_rasterizer::_surface_color_half_fp16_1);

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
				c_player_view::render_static_lighting();

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

				IDirect3DSurface9* accum_LDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_LDR)->m_d3d_surface;
				IDirect3DSurface9* accum_HDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_HDR)->m_d3d_surface;
				IDirect3DSurface9* depth_stencil_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_depth_stencil)->m_d3d_surface;

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
					c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					true);

				if (!render_debug_depth_render)
					c_player_view::render_first_person(false);

				if (render_debug_toggle_default_sfx)
				{
					if (c_rasterizer::get_is_tiling_enabled())
						c_tron_effect::resolve_and_process_z_camera(m_camera_user_data.player_window_index, &rasterizer_camera->window_pixel_bounds, false);

					c_screen_postprocess::render_ssao(
						&m_rasterizer_projection,
						&m_rasterizer_camera,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_accum_HDR,
						c_rasterizer::_surface_color_half_fp16_0);
					
					if (screenshot_allow_postprocess())
						c_screen_postprocess::render_ssr(
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

					c_rasterizer::setup_targets_static_lighting(
						m_render_exposure,
						m_illum_render_scale,
						c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);

					if (water_updated || render_underwater_fog_enabled)
						c_player_view::render_water();

					if (g_rasterizer_game_states->patchy_fog)
						c_player_view::queue_patchy_fog();

					c_player_view::render_transparents();

					c_player_view::distortion_generate();
					bool distortion_history = c_render_globals::get_distortion_visible();
					if (distortion_history || water_updated || render_underwater_fog_enabled)
						distortion_history = true;
					c_render_globals::set_distortion_history(distortion_history);

					{
						c_rasterizer_profile_scope _game_engine(_rasterizer_profile_element_game_engine, L"game_engine");
						game_engine_render(m_camera_user_data.user_index);
					}

					c_player_view::render_distortions();

					if (!render_debug_depth_render)
					{
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), true);
						c_player_view::render_first_person(true);
					}

					{
						c_rasterizer_profile_scope _setup_targets_static_lighting_alpha_blend(_rasterizer_profile_element_total, L"setup_targets_static_lighting_alpha_blend");
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), false);
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
				c_rasterizer_profile_scope _postprocess_copy(_rasterizer_profile_element_total, L"postprocess copy");
				c_screen_postprocess::copy(
					c_rasterizer_globals::_shader_copy_scaled,
					c_rasterizer::_surface_accum_LDR,
					c_rasterizer::_surface_albedo,
					c_rasterizer::_sampler_filter_mode_point,
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

void __thiscall c_player_view::render_2nd_pass()
{
	if (bool rendering_albedo = c_player_view::render_albedo())
	{
		if (screenshot_allow_postprocess())
		{
			s_screen_effect_settings screen_effect_settings{};
			s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
			screen_effect_sample(
				&m_rasterizer_camera.position,
				&m_rasterizer_camera.forward,
				&screen_effect_settings,
				&screen_effect_shader_sample_result,
				m_camera_user_data.user_index);

			c_screen_postprocess::postprocess_player_view(
				m_window_game_state->camera_fx_values,
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
					c_rasterizer::_sampler_filter_mode_point,
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

void __thiscall c_player_view::render_3rd_pass()
{
	if (screenshot_allow_ui_render())
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);

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

		render_screen_shaders(&screen_effect_shader_sample_result, 1, c_rasterizer::sub_A48770(), c_rasterizer::_surface_none, NULL);

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

			short_rectangle2d display_pixel_bounds{};
			interface_get_current_window_settings(NULL, NULL, &display_pixel_bounds, NULL);
			user_interface_render(
				m_camera_user_data.controller_index,
				m_camera_user_data.user_index,
				m_camera_user_data.player_window_index,
				&display_pixel_bounds,
				c_rasterizer::sub_A48770(),
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
}

void __thiscall c_player_view::render_4th_pass()
{
	game_engine_render_debug(m_camera_user_data.user_index);

	if (!sub_610260())
		render_debug_window_render(m_camera_user_data.user_index);

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

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->orientation_type = _texture_camera_orientation_attached_object_orient;
}

void __cdecl render_texture_camera_attach_to_object(long object_index, long marker_id)
{
	INVOKE(0x00A3AB20, render_texture_camera_attach_to_object, object_index, marker_id);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
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
	//	WARNING_EVENT("texture camera can't find object to attach to");
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

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->dynamic_lights = enable;
}

real __cdecl render_texture_camera_get_aspect_ratio()
{
	return INVOKE(0x00A3ABD0, render_texture_camera_get_aspect_ratio);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//return g_render_texture_camera_globals->aspect_ratio;
}

long __cdecl render_texture_camera_get_height()
{
	return INVOKE(0x00A3ABF0, render_texture_camera_get_height);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//return g_render_texture_camera_globals->height;
}

long __cdecl render_texture_camera_get_render_mode()
{
	return INVOKE(0x00A3AC10, render_texture_camera_get_render_mode);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//return g_render_texture_camera_globals->render_mode;
}

long __cdecl render_texture_camera_get_sub_A3AC30()
{
	return INVOKE(0x00A3AC30, render_texture_camera_get_sub_A3AC30);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//if (!object_try_and_get_and_verify_type(g_render_texture_camera_globals->__unknown40_object_index, NONE))
	//	g_render_texture_camera_globals->__unknown40_object_index = NONE;
	//
	//return g_render_texture_camera_globals->__unknown40_object_index;
}

long __cdecl render_texture_camera_get_target_object_index()
{
	return INVOKE(0x00A3AC70, render_texture_camera_get_target_object_index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//if (g_render_texture_camera_globals->target_type == _texture_camera_type_position)
	//	return NONE;
	//
	//long object_index = g_render_texture_camera_globals->target.object_index;
	//if (!object_try_and_get_and_verify_type(object_index, NONE))
	//	return NONE;
	//
	//return object_index;
}

long __cdecl render_texture_camera_get_width()
{
	return INVOKE(0x00A3ACB0, render_texture_camera_get_width);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//return g_render_texture_camera_globals->width;
}

void __cdecl render_texture_camera_initialize()
{
	INVOKE(0x00A3ACD0, render_texture_camera_initialize);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals = (s_render_texture_camera_globals*)g_render_texture_camera_globals_allocator.allocate(
	//	sizeof(s_render_texture_camera_globals),
	//	"render texture globals");
}

void __cdecl render_texture_camera_initialize_for_new_map()
{
	INVOKE(0x00A3AD30, render_texture_camera_initialize_for_new_map);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
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

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->active = false;
}

void __cdecl render_texture_camera_on()
{
	INVOKE(0x00A3AE70, render_texture_camera_on);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->active = true;
}

void __cdecl render_texture_camera_position_world_offset(real offset_x, real offset_y, real offset_z)
{
	INVOKE(0x00A3AE90, render_texture_camera_position_world_offset, offset_x, offset_y, offset_z);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->position_offset.i = offset_x;
	//g_render_texture_camera_globals->position_offset.j = offset_x;
	//g_render_texture_camera_globals->position_offset.k = offset_y;
}

void __cdecl render_texture_camera_render_mode(long mode)
{
	INVOKE(0x00A3AEE0, render_texture_camera_render_mode, mode);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->render_mode = (e_texture_camera_render_mode)MAX(_texture_camera_render_default, MIN(mode, _texture_camera_render_target_only));
}

void __cdecl render_texture_camera_set_aspect_ratio(real aspect_ratio)
{
	INVOKE(0x00A3AF20, render_texture_camera_set_aspect_ratio, aspect_ratio);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->aspect_ratio = aspect_ratio;
}

void __cdecl render_texture_camera_set_fov(real vertical_field_of_view_degrees)
{
	INVOKE(0x00A3AF50, render_texture_camera_set_fov, vertical_field_of_view_degrees);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->fov_type = _texture_camera_fov_fixed;
	//g_render_texture_camera_globals->fov.vertical_fov_radians = CLAMP(vertical_field_of_view_degrees, 1.0f, 180.0f) * DEG;
}

void __cdecl render_texture_camera_set_fov_frame_target(real target_zoom)
{
	INVOKE(0x00A3AFB0, render_texture_camera_set_fov_frame_target, target_zoom);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->fov_type = _texture_camera_fov_frame_target;
	//g_render_texture_camera_globals->fov.target_scale = target_zoom;
}

void __cdecl render_texture_camera_set_object_marker(long object_index, long marker_id, real vertical_field_of_view_degrees)
{
	INVOKE(0x00A3AFF0, render_texture_camera_set_object_marker, object_index, marker_id, vertical_field_of_view_degrees);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
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
	//		ERROR_EVENT("### WARNING texture camera can't attach to object! detaching ...");
	//}
}

void __cdecl render_texture_camera_set_position(real position_x, real position_y, real position_z)
{
	INVOKE(0x00A3B0F0, render_texture_camera_set_position, position_x, position_y, position_z);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->position_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->position.position.x = position_x;
	//g_render_texture_camera_globals->position.position.y = position_y;
	//g_render_texture_camera_globals->position.position.z = position_z;
	//g_render_texture_camera_globals->position_offset.i = 0.0f;
	//g_render_texture_camera_globals->position_offset.j = 0.0f;
	//g_render_texture_camera_globals->position_offset.k = 0.0f;
}

void __cdecl render_texture_camera_set_position_and_direction(bool enabled, real_point3d const* position, real_vector3d const* forward, real_vector3d const* up, real vertical_field_of_view_degrees)
{
	INVOKE(0x00A3B150, render_texture_camera_set_position_and_direction, enabled, position, forward, up, vertical_field_of_view_degrees);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
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
void __cdecl render_texture_camera_set_sub_A3B240(long object_index)
{
	INVOKE(0x00A3B240, render_texture_camera_set_sub_A3B240, object_index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//if (object_try_and_get_and_verify_type(object_index, NONE))
	//	g_render_texture_camera_globals->__unknown40_object_index = object_index;
}

void __cdecl render_texture_camera_set_resolution(long width, long height)
{
	INVOKE(0x00A3B280, render_texture_camera_set_resolution, width, height);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->width = MAX(1, MIN(width, 512));
	//g_render_texture_camera_globals->height = MAX(1, MIN(height, 288));
}

void __cdecl render_texture_camera_set_target(real position_x, real position_y, real position_z)
{
	INVOKE(0x00A3B2E0, render_texture_camera_set_target, position_x, position_y, position_z);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//g_render_texture_camera_globals->target_type = _texture_camera_type_position;
	//g_render_texture_camera_globals->target.position.x = position_x;
	//g_render_texture_camera_globals->target.position.y = position_y;
	//g_render_texture_camera_globals->target.position.z = position_z;
}

void __cdecl render_texture_camera_target_object(long object_index, long marker_id)
{
	INVOKE(0x00A3B330, render_texture_camera_target_object, object_index, marker_id);

	//TLS_DATA_GET_VALUE_REFERENCE(g_render_texture_camera_globals);
	//if (object_try_and_get_and_verify_type(object_index, NONE))
	//{
	//	g_render_texture_camera_globals->target_type = _texture_camera_type_marker_id;
	//	g_render_texture_camera_globals->target.object_index = object_index;
	//	g_render_texture_camera_globals->target.marker_id = marker_id;
	//}
	//else
	//{
	//	WARNING_EVENT("texture camera can't find object to target");
	//}
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

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_water);
}

void __thiscall c_player_view::render_weather_occlusion()
{
	//INVOKE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);

	c_rasterizer_profile_scope _weather_occlusion(_rasterizer_profile_element_total, L"weather_occlusion");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_weather_occlusion);
}

void c_player_view::setup_camera(long player_index, long window_count, long window_arrangement, long user_index, s_observer_result const* result, bool render_freeze)
{
	INVOKE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera, player_index, window_count, window_arrangement, user_index, result, render_freeze);
}

void __thiscall c_player_view::setup_camera_fx_parameters(real a1)
{
	INVOKE_CLASS_MEMBER(0x00A3B990, c_player_view, setup_camera_fx_parameters, a1);
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

void __thiscall c_player_view::distortion_generate()
{
	//INVOKE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

	c_rasterizer_profile_scope _distortion_generate(_rasterizer_profile_element_distortions, L"distortion_generate");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, distortion_generate);
}

void __thiscall c_player_view::submit_occlusion_tests(bool a1, bool a2)
{
	//INVOKE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests, a1, a2);

	c_rasterizer_profile_scope _occlusion_tests(_rasterizer_profile_element_occlusions, L"occlusion_tests");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, submit_occlusion_tests, a1, a2);
}

void c_player_view::frame_advance()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	effects_frame_advance(g_main_render_timing_data->game_dt);
	effects_frame_advance_gpu(g_main_render_timing_data->game_dt);
	c_water_renderer::frame_advance(g_main_render_timing_data->game_dt);
	c_patchy_fog::frame_advance_all(g_main_render_timing_data->game_dt);
}

