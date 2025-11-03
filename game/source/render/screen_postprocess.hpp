#pragma once

#include "rasterizer/rasterizer.hpp"

class c_camera_fx_values;
class c_camera_fx_settings;
struct render_projection;
struct render_camera;
struct s_screen_effect_settings;
struct s_observer_depth_of_field;
struct s_oriented_bounding_box;
struct s_lightshafts;

class c_screen_postprocess
{
public:
	struct s_settings
	{
		bool m_postprocess;
		int32 m_display_alpha;
		int32 m_accum;
		int32 m_bloom_source;
		int32 m_accum_filter;
		int32 m_bloom;
		int32 m_bling;
		bool m_persist;
		int32 m_downsample;
		real32 m_postprocess_exposure;
		bool m_allow_debug_display;
		bool m_tone_curve;
		real32 m_tone_curve_white_point;
		bool m_auto_exposure_lock;
		int32 m_debug_mode;
	};
	static_assert(sizeof(s_settings) == 0x38);

	static void __cdecl accept_edited_settings();

	static void __cdecl apply_binary_op_ex(
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
		real_rectangle2d* source_texture_rect);

	static void __cdecl blit(
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
		real_rectangle2d* dest_texture_rect);

	static c_rasterizer::e_surface __cdecl blur_display();

	static void __cdecl copy(
		int32 explicit_shader_index,
		c_rasterizer::e_surface source_surface,
		c_rasterizer::e_surface dest_surface,
		c_rasterizer::e_sampler_filter_mode sampler_filter_mode,
		c_rasterizer::e_sampler_address_mode sampler_address_mode,
		real32 scale_r,
		real32 scale_g,
		real32 scale_b,
		real32 scale_a,
		real_rectangle2d* dest_texture_rect);

	static void __cdecl sub_A60AF0(
		const render_projection* projection,
		const render_camera* camera,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static c_rasterizer::e_surface __cdecl downsample_generate(
		c_rasterizer::e_surface source_buffer,
		c_camera_fx_values& camera,
		c_rasterizer::e_surface dest_buffer_tiny,
		c_rasterizer::e_surface dest_buffer_mini);

	static void __cdecl gaussian_blur(
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		real32 horizontal_blur_size,
		real32 vertical_blur_size);

	static void __cdecl gaussian_blur_fixed(
		c_rasterizer::e_surface target_surface,
		c_rasterizer::e_surface temp_surface,
		real32 scale_r,
		real32 scale_g,
		real32 scale_b,
		real32 scale_a);

	static c_rasterizer::e_surface __cdecl postprocess_bloom_buffer(
		c_rasterizer::e_surface bloom_buffer,
		c_camera_fx_values& camera,
		c_rasterizer::e_splitscreen_res resolution);

	static bool __cdecl postprocess_final_composite(
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
		const s_observer_depth_of_field* observer_depth_of_field);

	static void __cdecl postprocess_player_view(
		c_camera_fx_values& fx_values,
		const render_projection* projection,
		const render_camera* camera,
		s_screen_effect_settings& screen_effect_settings,
		c_rasterizer::e_splitscreen_res splitscreen_res,
		const s_observer_depth_of_field* observer_dof,
		int32 user_index);

	static void __cdecl sub_A62710(
		const render_projection* projection,
		const render_camera* camera,
		const real_matrix4x3* matrix,
		real32 const(*projection_matrix)[4],
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static void __cdecl render_lightshafts(
		s_lightshafts* lightshafts,
		render_projection* projection,
		render_camera* camera,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b);

	static void __cdecl render_ssao(
		const c_camera_fx_values* fx_values,
		const render_projection* projection,
		const render_camera* camera);

	static void __cdecl postprocess_ssr(
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
		c_rasterizer::e_surface surface_l);

	static void __cdecl setup_rasterizer_for_postprocess(bool clear_targets);

	static const c_screen_postprocess::s_settings* const& x_settings;
	static c_screen_postprocess::s_settings& x_settings_internal;
	static c_screen_postprocess::s_settings& x_editable_settings;
};

