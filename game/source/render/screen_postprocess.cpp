#include "render/screen_postprocess.hpp"

#include "game/game.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/render.hpp"

REFERENCE_DECLARE(0x01917D50, long, g_ssao_enable);

decltype(c_screen_postprocess::postprocess_player_view)* screen_postprocess_postprocess_player_view = c_screen_postprocess::postprocess_player_view;
HOOK_DECLARE_CALL(0x00A39F4E, screen_postprocess_postprocess_player_view);
HOOK_DECLARE_CALL(0x00A3A171, sub_A62D70);
HOOK_DECLARE_CALL(0x00A61BBD, sub_A62720);

HOOK_DECLARE_CLASS(0x00A60460, c_screen_postprocess, copy);
HOOK_DECLARE_CLASS(0x00A601E0, c_screen_postprocess, blit);
HOOK_DECLARE_CLASS(0x00A60D60, c_screen_postprocess, gaussian_blur);

void __cdecl sub_A62720(s_lightshafts* lightshafts, render_projection* projection, render_camera* camera, c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b)
{
	c_d3d_pix_event _lightshafts(g_rasterizer_profile_pix_colors[1], L"lightshafts");

	INVOKE(0x00A62720, sub_A62720, lightshafts, projection, camera, surface_a, surface_b);
}

void __cdecl sub_A62D70(c_camera_fx_settings* fx_settings, render_projection* projection, render_camera* camera)
{
	if (players_get_active_and_in_game_count(true) > 1)
		return;

	bool ssao_enable = TEST_BIT(fx_settings->ssao.flags, 1);
	if (g_ssao_enable != NONE)
		ssao_enable = g_ssao_enable == 1;

	if (ssao_enable && global_preferences_get_postprocessing_quality())
	{
		c_d3d_pix_event _ssao(g_rasterizer_profile_pix_colors[1], L"ssao");

		INVOKE(0x00A62D70, sub_A62D70, fx_settings, projection, camera);
	}
}

void __cdecl c_screen_postprocess::accept_edited_settings()
{
	INVOKE(0x00604150, c_screen_postprocess::accept_edited_settings);
}

void __cdecl c_screen_postprocess::blit(
	long explicit_shader_index,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	c_rasterizer::e_sampler_filter_mode sampler_filter_mode,
	c_rasterizer::e_sampler_address_mode sampler_address_mode,
	real a6,
	real a7,
	real a8,
	real a9,
	real_rectangle2d* a10,
	real_rectangle2d* a11)
{
	//INVOKE(0x00A601E0, c_screen_postprocess::blit, explicit_shader_index, surface_a, surface_b, sampler_filter_mode, sampler_address_mode, a6, a7, a8, a9, a10, a11);

	c_d3d_pix_event _blit(g_rasterizer_profile_pix_colors[1], L"blit");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, blit, decltype(&c_screen_postprocess::blit), explicit_shader_index, surface_a, surface_b, sampler_filter_mode, sampler_address_mode, a6, a7, a8, a9, a10, a11);
}

c_rasterizer::e_surface __cdecl c_screen_postprocess::blur_display()
{
	return INVOKE(0x00A60310, c_screen_postprocess::blur_display);
}

void __cdecl c_screen_postprocess::copy(
	long explicit_shader_index,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	c_rasterizer::e_sampler_filter_mode sampler_filter_mode,
	c_rasterizer::e_sampler_address_mode sampler_address_mode,
	real a6,
	real a7,
	real a8,
	real a9,
	real_rectangle2d* bounds)
{
	//INVOKE(0x00A60460, c_screen_postprocess::copy, explicit_shader_index, surface_a, surface_b, sampler_filter_mode, sampler_address_mode, a6, a7, a8, a9, bounds);

	c_d3d_pix_event _copy(g_rasterizer_profile_pix_colors[1], L"copy");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, copy, decltype(&c_screen_postprocess::copy), explicit_shader_index, surface_a, surface_b, sampler_filter_mode, sampler_address_mode, a6, a7, a8, a9, bounds);
}

void __cdecl c_screen_postprocess::gaussian_blur(c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b)
{
	//INVOKE(0x00A60D60, c_screen_postprocess::gaussian_blur, surface_a, surface_b);

	c_d3d_pix_event _gaussian_blur(g_rasterizer_profile_pix_colors[1], L"gaussian_blur");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, gaussian_blur, decltype(&c_screen_postprocess::gaussian_blur), surface_a, surface_b);
}

void __cdecl c_screen_postprocess::postprocess_player_view(
	c_camera_fx_values& fx_values,
	render_camera const* camera,
	s_screen_effect_settings& screen_effect_settings,
	e_splitscreen_res splitscreen_res,
	s_observer_depth_of_field const* observer_dof,
	e_output_user_index output_user_index,
	c_rasterizer::e_surface surface)
{
	INVOKE(0x00A61770, c_screen_postprocess::postprocess_player_view, fx_values, camera, screen_effect_settings, splitscreen_res, observer_dof, output_user_index, surface);

	if (render_debug_toggle_default_lightmaps_texaccum == 3)
	{
		c_screen_postprocess::setup_rasterizer_for_postprocess(false);
		c_screen_postprocess::copy(
			c_rasterizer_globals::_explicit_shader_copy_surface,
			c_rasterizer::_surface_accum_LDR,
			c_rasterizer::_surface_disable,
			c_rasterizer::_sampler_filter_mode_unknown1,
			c_rasterizer::_sampler_address_mode_unknown1,
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			NULL);
	}
}

void __cdecl c_screen_postprocess::setup_rasterizer_for_postprocess(bool a1)
{
	INVOKE(0x00A63510, c_screen_postprocess::setup_rasterizer_for_postprocess, a1);
}

