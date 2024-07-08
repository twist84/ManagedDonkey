#include "render/screen_postprocess.hpp"

#include "memory/module.hpp"

decltype(c_screen_postprocess::postprocess_player_view)* screen_postprocess_postprocess_player_view = c_screen_postprocess::postprocess_player_view;
HOOK_DECLARE_CALL(0x00A39F4E, screen_postprocess_postprocess_player_view);

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
	INVOKE(0x00A60460, c_screen_postprocess::copy, explicit_shader_index, surface_a, surface_b, sampler_filter_mode, sampler_address_mode, a6, a7, a8, a9, bounds);
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

