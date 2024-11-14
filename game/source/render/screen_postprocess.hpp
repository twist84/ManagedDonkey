#pragma once

#include "rasterizer/rasterizer.hpp"

enum e_splitscreen_res;
enum e_output_user_index;

struct c_camera_fx_values;
struct c_camera_fx_settings;
struct render_projection;
struct render_camera;
struct s_screen_effect_settings;
struct s_observer_depth_of_field;
struct s_oriented_bounding_box;

struct c_screen_postprocess
{
	static void __cdecl accept_edited_settings();

	static void __cdecl blit(
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
		real_rectangle2d* a11);

	static c_rasterizer::e_surface __cdecl blur_display();

	static void __cdecl copy(
		long explicit_shader_index,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_sampler_filter_mode sampler_filter_mode,
		c_rasterizer::e_sampler_address_mode sampler_address_mode,
		real a6,
		real a7,
		real a8,
		real a9,
		real_rectangle2d* bounds);

	static void __cdecl sub_A60AF0(
		render_projection const* projection,
		render_camera const* camera,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static void __cdecl gaussian_blur(
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b);

	static void __cdecl postprocess_player_view(
		c_camera_fx_values& fx_values,
		render_projection const* projection,
		render_camera const* camera,
		s_screen_effect_settings& screen_effect_settings,
		e_splitscreen_res splitscreen_res,
		s_observer_depth_of_field const* observer_dof,
		e_output_user_index output_user_index);

	static void __cdecl sub_A62710(
		render_projection const* projection,
		render_camera const* camera,
		real_matrix4x3 const* matrix,
		s_oriented_bounding_box const* projection_matrix,
		c_rasterizer::e_surface surface_a,
		c_rasterizer::e_surface surface_b,
		c_rasterizer::e_surface surface_c);

	static void __cdecl sub_A61CD0(
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

	struct s_settings
	{
		bool __unknown0;
		byte __data1[0x37];
	};
	static_assert(sizeof(s_settings) == 0x38);

	static c_screen_postprocess::s_settings const* const& x_settings;

};

struct s_lightshafts;

extern void __cdecl sub_A62720(s_lightshafts* lightshafts,
	render_projection* projection, render_camera* camera,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b);

extern void __cdecl sub_A62D70(c_camera_fx_settings* fx_settings,
	render_projection* projection,
	render_camera* camera);

