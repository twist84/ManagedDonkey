#include "render/screen_postprocess.hpp"

#include "game/game.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/render.hpp"

REFERENCE_DECLARE(0x01917D18, c_screen_postprocess::s_settings, c_screen_postprocess::x_settings_internal);
REFERENCE_DECLARE(0x01917D50, int32, g_ssao_enable);
REFERENCE_DECLARE(0x01917D54, real32, g_ssao_radius);
REFERENCE_DECLARE(0x01917D58, real32, g_ssao_intensity);
REFERENCE_DECLARE(0x01917D5C, real32, g_ssao_sample_z_threshold);
REFERENCE_DECLARE(0x01917D60, bool, g_use_fxaa);
REFERENCE_DECLARE(0x01917D61, bool, g_lightshafts_enable);
REFERENCE_DECLARE(0x01917D62, bool, g_ssr_enabled);
REFERENCE_DECLARE(0x01917D63, bool, g_ssr_use_blur);
REFERENCE_DECLARE(0x01917D64, bool, g_ssr_use_multisampling);
REFERENCE_DECLARE(0x01917D68, int32, g_ssr_multisample_passes);
REFERENCE_DECLARE(0x05115AFC, bool, g_ssr_debug_enabled);
REFERENCE_DECLARE(0x05115AFD, bool, g_ssr_use_bilinear_filtering);
REFERENCE_DECLARE(0x05115AFE, bool, g_ssr_use_fxaa);
REFERENCE_DECLARE(0x05115B00, c_screen_postprocess::s_settings, c_screen_postprocess::x_editable_settings);

decltype(c_screen_postprocess::render_ssao)* screen_postprocess_render_ssao = c_screen_postprocess::render_ssao;
HOOK_DECLARE_CALL(0x00A3A171, screen_postprocess_render_ssao);

decltype(c_screen_postprocess::render_lightshafts)* screen_postprocess_render_lightshafts = c_screen_postprocess::render_lightshafts;
HOOK_DECLARE_CALL(0x00A61BBD, screen_postprocess_render_lightshafts);

HOOK_DECLARE_CLASS(0x00A60060, c_screen_postprocess, apply_binary_op_ex);
HOOK_DECLARE_CLASS(0x00A601E0, c_screen_postprocess, blit);
HOOK_DECLARE_CLASS(0x00A60460, c_screen_postprocess, copy);
HOOK_DECLARE_CLASS(0x00A60D60, c_screen_postprocess, gaussian_blur);
HOOK_DECLARE_CLASS(0x00A60DE0, c_screen_postprocess, gaussian_blur_fixed);
HOOK_DECLARE_CLASS(0x00A63510, c_screen_postprocess, setup_rasterizer_for_postprocess);

void __cdecl c_screen_postprocess::accept_edited_settings()
{
	//INVOKE(0x00604150, c_screen_postprocess::accept_edited_settings);

	c_screen_postprocess::x_settings_internal = c_screen_postprocess::x_editable_settings;
}

void __cdecl c_screen_postprocess::apply_binary_op_ex(
	int32 explicit_shader_index,
	c_rasterizer::e_surface source_surface_0,
	c_rasterizer::e_surface source_surface_1,
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_sampler_filter_mode filter_mode,
	c_rasterizer::e_sampler_address_mode address_mode,
	real32 scale_r,
	real32 scale_g,
	real32 scale_b,
	real32 scale_a,
	real_rectangle2d* source_texture_rect)
{
	//INVOKE(0x00A60060, c_screen_postprocess::apply_binary_op_ex, explicit_shader_index, source_surface_0, source_surface_1, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, source_texture_rect);

	c_rasterizer::set_render_target(0, dest_surface, 0xFFFFFFFF);
	if (c_rasterizer::set_explicit_shaders(explicit_shader_index, _vertex_type_screen, _transfer_vertex_none, _entry_point_default))
	{
		c_rasterizer::set_surface_as_texture(0, source_surface_0);
		c_rasterizer::set_sampler_texture_direct(0, c_rasterizer::_surface_none);
		c_rasterizer::set_sampler_address_mode(0, address_mode, address_mode, address_mode);
		c_rasterizer::set_sampler_filter_mode(0, filter_mode);
		c_rasterizer::set_surface_as_texture(1, source_surface_1);
		c_rasterizer::set_sampler_address_mode(1, address_mode, address_mode, address_mode);
		c_rasterizer::set_sampler_filter_mode(1, filter_mode);
		c_rasterizer::set_sampler_texture(2, c_rasterizer_texture_ref());

		int32 source_surface_0_width = c_rasterizer::get_surface_width(source_surface_0);
		int32 source_surface_0_height = c_rasterizer::get_surface_height(source_surface_0);
		{
			real_vector4d constant_data[]
			{
				{ 1.0f / source_surface_0_width, 1.0f / source_surface_0_height, 0.0f, 0.0f }
			};
			c_rasterizer::set_pixel_shader_constant(1, NUMBEROF(constant_data), constant_data);
		}
		{
			real_vector4d constant_data[]
			{
				{ scale_r, scale_g, scale_b, scale_a }
			};
			c_rasterizer::set_pixel_shader_constant(2, NUMBEROF(constant_data), constant_data);
		}

		int32 dest_surface_width = c_rasterizer::get_surface_width(dest_surface);
		int32 dest_surface_height = c_rasterizer::get_surface_height(dest_surface);
		c_rasterizer::draw_fullscreen_quad(dest_surface_width, dest_surface_height);
		c_rasterizer::set_sampler_texture(0, c_rasterizer_texture_ref());
		c_rasterizer::set_sampler_texture(1, c_rasterizer_texture_ref());
	}
}

void __cdecl c_screen_postprocess::blit(
	int32 explicit_shader_index,
	c_rasterizer::e_surface source_surface,
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_sampler_filter_mode filter_mode,
	c_rasterizer::e_sampler_address_mode address_mode,
	real32 scale_r,
	real32 scale_g,
	real32 scale_b,
	real32 scale_a,
	real_rectangle2d* source_texture_rect,
	real_rectangle2d* dest_texture_rect)
{
	//INVOKE(0x00A601E0, c_screen_postprocess::blit, explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, source_texture_rect, dest_texture_rect);

	c_rasterizer_profile_scope _blit(_rasterizer_profile_element_total, L"blit");

	c_rasterizer::set_surface_as_texture(0, source_surface);
	c_rasterizer::set_sampler_address_mode(0, address_mode, address_mode, address_mode);
	c_rasterizer::set_sampler_filter_mode(0, filter_mode);
	c_rasterizer::set_render_target(0, dest_surface, 0xFFFFFFFF);
	
	if (c_rasterizer::set_explicit_shaders(explicit_shader_index, _vertex_type_screen, _transfer_vertex_none, _entry_point_default))
	{
		int32 source_surface_width = c_rasterizer::get_surface_width(source_surface);
		int32 source_surface_height = c_rasterizer::get_surface_height(source_surface);
		{
			real_vector4d constant_data[]
			{
				{ 1.0f / source_surface_width, 1.0f / source_surface_height, 0.0f, 0.0f }
			};
			c_rasterizer::set_pixel_shader_constant(1, NUMBEROF(constant_data), constant_data);
		}
		{
			real_vector4d constant_data[]
			{
				{ scale_r, scale_g, scale_b, scale_a }
			};
			c_rasterizer::set_pixel_shader_constant(2, NUMBEROF(constant_data), constant_data);
		}

		int32 dest_surface_width = c_rasterizer::get_surface_width(dest_surface);
		int32 dest_surface_height = c_rasterizer::get_surface_height(dest_surface);
		c_rasterizer::draw_screen_quad_with_texture_transform(
			dest_surface_width,
			dest_surface_height,
			dest_texture_rect,
			source_texture_rect);
		c_rasterizer::set_sampler_texture(0, c_rasterizer_texture_ref());
	}
}

c_rasterizer::e_surface __cdecl c_screen_postprocess::blur_display()
{
	c_rasterizer::e_surface result = INVOKE(0x00A60310, c_screen_postprocess::blur_display);
	return result;
}

void __cdecl c_screen_postprocess::copy(
	int32 explicit_shader_index,
	c_rasterizer::e_surface source_surface,
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_sampler_filter_mode filter_mode,
	c_rasterizer::e_sampler_address_mode address_mode,
	real32 scale_r,
	real32 scale_g,
	real32 scale_b,
	real32 scale_a,
	real_rectangle2d* dest_texture_rect)
{
	//INVOKE(0x00A60460, c_screen_postprocess::copy, explicit_shader_index, source_surface, dest_surface, filter_mode, address_mode, scale_r, scale_g, scale_b, scale_a, dest_texture_rect);

	c_rasterizer_profile_scope _copy(_rasterizer_profile_element_total, L"copy");

	c_rasterizer::set_render_target(0, dest_surface, 0xFFFFFFFF);
	if (c_rasterizer::set_explicit_shaders(explicit_shader_index, _vertex_type_screen, _transfer_vertex_none, _entry_point_default))
	{
		c_rasterizer::set_surface_as_texture(0, source_surface);
		c_rasterizer::set_sampler_address_mode(0, address_mode, address_mode, address_mode);
		c_rasterizer::set_sampler_filter_mode(0, filter_mode);
	
		int32 source_surface_width = c_rasterizer::get_surface_width(source_surface);
		int32 source_surface_height = c_rasterizer::get_surface_height(source_surface);
		{
			real_vector4d constant_data[]
			{
				{ 1.0f / source_surface_width, 1.0f / source_surface_height, 0.0f, 0.0f }
			};
			c_rasterizer::set_pixel_shader_constant(1, NUMBEROF(constant_data), constant_data);
		}
		{
			real_vector4d constant_data[]
			{
				{ scale_r, scale_g, scale_b, scale_a }
			};
			c_rasterizer::set_pixel_shader_constant(2, NUMBEROF(constant_data), constant_data);
		}
	
		int32 dest_surface_width = c_rasterizer::get_surface_width(dest_surface);
		int32 dest_surface_height = c_rasterizer::get_surface_height(dest_surface);
		if (dest_texture_rect)
		{
			c_rasterizer::draw_fullscreen_quad_with_texture_xform(dest_surface_width, dest_surface_height, dest_texture_rect);
		}
		else
		{
			c_rasterizer::draw_fullscreen_quad(dest_surface_width, dest_surface_height);
		}
	}
}

void __cdecl copy_accumulation_target(
	c_rasterizer::e_surface dest_surface,
	c_rasterizer::e_surface star_buffer,
	real32 natural_intensity,
	real32 bloom_intensity,
	real32 bling_intensity,
	real32 persist_intensity,
	bool use_depth_of_field,
	const s_observer_depth_of_field* observer_depth_of_field)
{
	INVOKE(0x00A605B0, copy_accumulation_target, dest_surface, star_buffer, natural_intensity, bloom_intensity, bling_intensity, persist_intensity, use_depth_of_field, observer_depth_of_field);
}

// nullsub
void __cdecl c_screen_postprocess::sub_A60AF0(const render_projection* projection, const render_camera* camera, c_rasterizer::e_surface surface_a, c_rasterizer::e_surface surface_b, c_rasterizer::e_surface surface_c)
{
	//INVOKE(0x00A60AF0, c_screen_postprocess::sub_A60AF0, projection, camera, surface_a, surface_b, surface_c);
}

c_rasterizer::e_surface __cdecl c_screen_postprocess::downsample_generate(
	c_rasterizer::e_surface source_buffer,
	c_camera_fx_values& camera,
	c_rasterizer::e_surface dest_buffer_tiny,
	c_rasterizer::e_surface dest_buffer_mini)
{
	c_rasterizer::e_surface result = INVOKE(0x00A60B00, c_screen_postprocess::downsample_generate, source_buffer, camera, dest_buffer_tiny, dest_buffer_mini);
	return result;
}

//.text:00A60D50 ; void __cdecl fxaa_enable(bool)

void __cdecl c_screen_postprocess::gaussian_blur(
	c_rasterizer::e_surface target_surface,
	c_rasterizer::e_surface temp_surface,
	real32 horizontal_blur_size,
	real32 vertical_blur_size)
{
	//INVOKE(0x00A60D60, c_screen_postprocess::gaussian_blur, target_surface, temp_surface, horizontal_blur_size, vertical_blur_size);

	c_rasterizer_profile_scope _gaussian_blur(_rasterizer_profile_element_total, L"gaussian_blur");

	c_screen_postprocess::copy(
		c_rasterizer_globals::_shader_bloom_blur_kernel_11_horizontal,
		target_surface,
		temp_surface,
		c_rasterizer::_sampler_filter_bilinear,
		c_rasterizer::_sampler_address_clamp,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		NULL);
	c_screen_postprocess::copy(
		c_rasterizer_globals::_shader_bloom_blur_kernel_11_vertical,
		temp_surface,
		target_surface,
		c_rasterizer::_sampler_filter_bilinear,
		c_rasterizer::_sampler_address_clamp,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		NULL);
}

//REFERENCE_DECLARE_ARRAY(0x01694F08, real_vector4d const, kernel_vertical, 5);
void __cdecl c_screen_postprocess::gaussian_blur_fixed(
	c_rasterizer::e_surface target_surface,
	c_rasterizer::e_surface temp_surface,
	real32 scale_r,
	real32 scale_g,
	real32 scale_b,
	real32 scale_a)
{
	//INVOKE(0x00A60DE0, c_screen_postprocess::gaussian_blur_fixed, target_surface, temp_surface, scale_r, scale_g, scale_b, scale_a);

	c_screen_postprocess::copy(
		c_rasterizer_globals::_shader_bloom_blur_kernel_11_horizontal,
		target_surface,
		temp_surface,
		c_rasterizer::_sampler_filter_bilinear,
		c_rasterizer::_sampler_address_clamp,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		NULL);
	c_rasterizer::resolve_surface(temp_surface, 0, 0, 0, 0);

	{
		static real_vector4d const kernel_vertical[]
		{
			{ 0.5f, -4.1f, 0.01953125f, 0.0f },
			{ 0.5f, -2.3f,   0.234375f, 0.0f },
			{ 0.5f, -0.5f,  0.4921875f, 0.0f },
			{ 0.5f,  1.3f,   0.234375f, 0.0f },
			{ 0.5f,  3.1f, 0.01953125f, 0.0f },
		};
		COMPILE_ASSERT(NUMBEROF(kernel_vertical) == 5);
		c_rasterizer::set_pixel_shader_constant(3, NUMBEROF(kernel_vertical), kernel_vertical);
	}

	c_screen_postprocess::copy(
		c_rasterizer_globals::_shader_convolve_kernel_5,
		temp_surface,
		target_surface,
		c_rasterizer::_sampler_filter_bilinear,
		c_rasterizer::_sampler_address_clamp,
		scale_r,
		scale_g,
		scale_b,
		scale_a,
		NULL);
}

c_rasterizer::e_surface __cdecl c_screen_postprocess::postprocess_bloom_buffer(
	c_rasterizer::e_surface bloom_buffer,
	c_camera_fx_values& camera,
	c_rasterizer::e_splitscreen_res resolution)
{
	c_rasterizer::e_surface result = INVOKE(0x00A60F90, c_screen_postprocess::postprocess_bloom_buffer, bloom_buffer, camera, resolution);
	return result;
}

bool __cdecl c_screen_postprocess::postprocess_final_composite(
	int32 explicit_shader_index,
	c_rasterizer::e_surface display_surface,
	c_rasterizer::e_surface bloom_surface,
	real32 inherent_scale,
	bool use_tone_curve,
	bool true_sRGB_output,
	bool use_depth_of_field,
	real32 z_near,
	real32 z_far,
	const rectangle2d* target_rect,
	const real_rectangle2d* window_rect,
	const s_observer_depth_of_field* observer_depth_of_field)
{
	bool result = INVOKE(0x00A61200, c_screen_postprocess::postprocess_final_composite, explicit_shader_index, display_surface, bloom_surface, inherent_scale, use_tone_curve, true_sRGB_output, use_depth_of_field, z_near, z_far, target_rect, window_rect, observer_depth_of_field);
	return result;
}

void __cdecl c_screen_postprocess::postprocess_player_view(
	c_camera_fx_values& fx_values,
	const render_projection* projection,
	const render_camera* camera,
	s_screen_effect_settings& screen_effect_settings,
	c_rasterizer::e_splitscreen_res splitscreen_res,
	const s_observer_depth_of_field* observer_dof,
	int32 user_index)
{
	INVOKE(0x00A61770, c_screen_postprocess::postprocess_player_view, fx_values, projection, camera, screen_effect_settings, splitscreen_res, observer_dof, user_index);
}

void __cdecl c_screen_postprocess::postprocess_ssr(
	const render_projection* projection,
	const render_camera* camera,
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
	g_ssr_enabled;
	g_ssr_use_blur;
	g_ssr_use_multisampling;
	g_ssr_multisample_passes;
	g_ssr_debug_enabled;
	g_ssr_use_bilinear_filtering;
	g_ssr_use_fxaa;

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
	const render_projection* projection,
	const render_camera* camera,
	const real_matrix4x3* matrix,
	real32 const(*projection_matrix)[4],
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

void __cdecl c_screen_postprocess::render_ssao(const c_camera_fx_values* fx_values, const render_projection* projection, const render_camera* camera)
{
	if (!game_is_splitscreen_deterministic())
	{
		bool use_ssao = g_ssao_enable != NONE ? g_ssao_enable == 1 : TEST_BIT(fx_values->ssao.m_flags, 1);
		if (use_ssao && global_preferences_get_postprocessing_quality())
		{
			c_rasterizer_profile_scope _ssao(_rasterizer_profile_element_total, L"ssao");

			INVOKE(0x00A62D70, render_ssao, fx_values, projection, camera);
		}
	}
}

void __cdecl c_screen_postprocess::setup_rasterizer_for_postprocess(bool clear_targets)
{
	//INVOKE(0x00A63510, c_screen_postprocess::setup_rasterizer_for_postprocess, clear_targets);

	c_rasterizer_profile_scope _setup_postprocess(_rasterizer_profile_element_total, L"setup_postprocess");

	c_rasterizer::clear_sampler_textures(0xFFFFFFFF);
	if (clear_targets)
	{
		c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_none);
		c_rasterizer::set_render_target(1, c_rasterizer::_surface_none, 0xFFFFFFFF);
		c_rasterizer::set_render_target(2, c_rasterizer::_surface_none, 0xFFFFFFFF);
		c_rasterizer::set_render_target(3, c_rasterizer::_surface_none, 0xFFFFFFFF);
	}
	c_rasterizer::set_alpha_blend_mode(c_rasterizer::_alpha_blend_opaque);
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
	c_rasterizer::set_scissor_rect(NULL);
	c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_off);
	c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);
}

