#include "render/screen_postprocess.hpp"

#include "game/game.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/render.hpp"

REFERENCE_DECLARE(0x01917D50, long, g_ssao_enable);
REFERENCE_DECLARE(0x05115B00, c_screen_postprocess::s_settings, c_screen_postprocess::x_editable_settings);

decltype(c_screen_postprocess::postprocess_player_view)* screen_postprocess_postprocess_player_view = c_screen_postprocess::postprocess_player_view;
HOOK_DECLARE_CALL(0x00A39F4E, screen_postprocess_postprocess_player_view);
HOOK_DECLARE_CALL(0x00A3A171, render_ssao);

HOOK_DECLARE_CLASS(0x00A60460, c_screen_postprocess, copy);
HOOK_DECLARE_CLASS(0x00A601E0, c_screen_postprocess, blit);
HOOK_DECLARE_CLASS(0x00A60D60, c_screen_postprocess, gaussian_blur);
HOOK_DECLARE_CLASS(0x00A61BBD, c_screen_postprocess, render_lightshafts);

void __cdecl render_ssao(c_camera_fx_settings* fx_settings, render_projection* projection, render_camera* camera)
{
	if (game_is_splitscreen_deterministic())
		return;

	bool ssao_enable = TEST_BIT(fx_settings->ssao.flags, 1);
	if (g_ssao_enable != NONE)
		ssao_enable = g_ssao_enable == 1;

	if (ssao_enable && global_preferences_get_postprocessing_quality())
	{
		c_rasterizer_profile_scope _ssao(_rasterizer_profile_element_total, L"ssao");

		INVOKE(0x00A62D70, render_ssao, fx_settings, projection, camera);
	}
}

void __cdecl c_screen_postprocess::accept_edited_settings()
{
	INVOKE(0x00604150, c_screen_postprocess::accept_edited_settings);
}

void __cdecl c_screen_postprocess::blit(
	long explicit_shader_index,
	c_rasterizer::e_surface source_surface,
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_sampler_filter_mode filter_mode,
	c_rasterizer::e_sampler_address_mode address_mode,
	real scale_r,
	real scale_g,
	real scale_b,
	real scale_a,
	real_rectangle2d* source_texture_rect,
	real_rectangle2d* dest_texture_rect)
{
	//INVOKE(0x00A601E0, c_screen_postprocess::blit, explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, source_texture_rect, dest_texture_rect);

	c_rasterizer_profile_scope _blit(_rasterizer_profile_element_total, L"blit");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, blit, decltype(&c_screen_postprocess::blit), explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, source_texture_rect, dest_texture_rect);
}

c_rasterizer::e_surface __cdecl c_screen_postprocess::blur_display()
{
	return INVOKE(0x00A60310, c_screen_postprocess::blur_display);
}

void __cdecl c_screen_postprocess::copy(
	long explicit_shader_index,
	c_rasterizer::e_surface source_surface,
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_sampler_filter_mode filter_mode,
	c_rasterizer::e_sampler_address_mode address_mode,
	real scale_r,
	real scale_g,
	real scale_b,
	real scale_a,
	real_rectangle2d* dest_texture_rect)
{
	//INVOKE(0x00A60460, c_screen_postprocess::copy, explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, );

	c_rasterizer_profile_scope _copy(_rasterizer_profile_element_total, L"copy");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, copy, decltype(&c_screen_postprocess::copy), explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, dest_texture_rect);
}

//.text:00A605B0 ; 

// nullsub
void __cdecl c_screen_postprocess::render_ssao_old(render_projection const* projection, render_camera const* camera, c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b, c_rasterizer::e_surface surface_c)
{
	//INVOKE(0x00A60AF0, c_screen_postprocess::render_ssao_old, projection, camera, surface_a, surface_b, surface_c);
}

void __cdecl c_screen_postprocess::gaussian_blur(c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b)
{
	//INVOKE(0x00A60D60, c_screen_postprocess::gaussian_blur, surface_a, surface_b);

	c_rasterizer_profile_scope _gaussian_blur(_rasterizer_profile_element_total, L"gaussian_blur");

	HOOK_INVOKE_CLASS(, c_screen_postprocess, gaussian_blur, decltype(&c_screen_postprocess::gaussian_blur), surface_a, surface_b);
}

void __cdecl c_screen_postprocess::postprocess_player_view(
	c_camera_fx_values& fx_values,
	render_projection const* projection,
	render_camera const* camera,
	s_screen_effect_settings& screen_effect_settings,
	e_splitscreen_res splitscreen_res,
	s_observer_depth_of_field const* observer_dof,
	long user_index)
{
	INVOKE(0x00A61770, c_screen_postprocess::postprocess_player_view, fx_values, projection, camera, screen_effect_settings, splitscreen_res, observer_dof, user_index);
}

void __cdecl c_screen_postprocess::postprocess_ssr(
	render_projection const* projection,
	render_camera const* camera,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	c_rasterizer::e_surface surface_c,
	c_rasterizer::e_surface surface_d,
	c_rasterizer::e_surface surface_e,
	c_rasterizer::e_surface surface_f,
	c_rasterizer::e_surface surface_g,
	c_rasterizer::e_surface surface_h,
	c_rasterizer::e_surface surface_i,
	c_rasterizer::e_surface surface_j,
	c_rasterizer::e_surface surface_k,
	c_rasterizer::e_surface surface_l)
{
	INVOKE(0x00A61CD0, c_screen_postprocess::postprocess_ssr,
		projection,
		camera,
		surface_a,
		surface_b,
		surface_c,
		surface_d,
		surface_e,
		surface_f,
		surface_g,
		surface_h,
		surface_i,
		surface_j,
		surface_k,
		surface_l);
}

// nullsub
void __cdecl c_screen_postprocess::sub_A62710(
	render_projection const* projection,
	render_camera const* camera,
	real_matrix4x3 const* matrix,
	real const(*projection_matrix)[4],
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	c_rasterizer::e_surface surface_c)
{
	//INVOKE(0x00A62710, c_screen_postprocess::sub_A62710, projection, camera, matrix, bounding_box, surface_a, surface_b, surface_c);
}

void __cdecl c_screen_postprocess::render_lightshafts(s_lightshafts* lightshafts, render_projection* projection, render_camera* camera, c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b)
{
	c_rasterizer_profile_scope _lightshafts(_rasterizer_profile_element_total, L"lightshafts");

	INVOKE(0x00A62720, c_screen_postprocess::render_lightshafts, lightshafts, projection, camera, surface_a, surface_b);
}

void __cdecl c_screen_postprocess::setup_rasterizer_for_postprocess(bool a1)
{
	INVOKE(0x00A63510, c_screen_postprocess::setup_rasterizer_for_postprocess, a1);
}

