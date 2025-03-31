#pragma once

#include "rasterizer/rasterizer.hpp"

enum e_splitscreen_res;

struct c_camera_fx_values;
struct c_camera_fx_settings;
struct render_projection;
struct render_camera;
struct s_screen_effect_settings;
struct s_observer_depth_of_field;
struct s_oriented_bounding_box;
struct s_lightshafts;

struct c_screen_postprocess
{
	struct s_settings
	{
		bool m_postprocess;
		long m_display_alpha;
		long m_accum;
		long m_bloom_source;
		long m_accum_filter;
		long m_bloom;
		long m_bling;
		bool m_persist;
		long m_downsample;
		real m_postprocess_exposure;
		bool m_allow_debug_display;
		bool m_tone_curve;
		real m_tone_curve_white_point;
		bool m_auto_exposure_lock;
		long m_debug_mode;
	};
	static_assert(sizeof(s_settings) == 0x38);

	static void __cdecl accept_edited_settings();

	static void __cdecl blit(
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
		real_rectangle2d* dest_texture_rect);

	static c_rasterizer::e_surface __cdecl blur_display();

	static void __cdecl copy(
		long explicit_shader_index,
		c_rasterizer::e_surface source_surface,
		c_rasterizer::e_surface dest_surface,
		c_rasterizer::e_sampler_filter_mode sampler_filter_mode,
		c_rasterizer::e_sampler_address_mode sampler_address_mode,
		real scale_r,
		real scale_g,
		real scale_b,
		real scale_a,
		real_rectangle2d* dest_texture_rect);

	static void __cdecl render_ssao_old(
		render_projection const* projection,
		render_camera const* camera,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static void __cdecl gaussian_blur(
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		real horizontal_blur_size,
		real vertical_blur_size);

	static void __cdecl postprocess_player_view(
		c_camera_fx_values& fx_values,
		render_projection const* projection,
		render_camera const* camera,
		s_screen_effect_settings& screen_effect_settings,
		e_splitscreen_res splitscreen_res,
		s_observer_depth_of_field const* observer_dof,
		long user_index);

	static void __cdecl sub_A62710(
		render_projection const* projection,
		render_camera const* camera,
		real_matrix4x3 const* matrix,
		real const(*projection_matrix)[4],
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static void __cdecl render_lightshafts(
		s_lightshafts* lightshafts,
		render_projection* projection,
		render_camera* camera,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b);

	static void __cdecl postprocess_ssr(
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
		c_rasterizer::e_surface surface_l);

	static void __cdecl setup_rasterizer_for_postprocess(bool a1);

	static c_screen_postprocess::s_settings const* const& x_settings;
	static c_screen_postprocess::s_settings& x_editable_settings;
};

extern void __cdecl render_ssao(c_camera_fx_settings* fx_settings,
	render_projection* projection,
	render_camera* camera);

