#pragma once

#include "rasterizer/rasterizer.hpp"

enum e_splitscreen_res;
enum e_output_user_index;

struct c_camera_fx_values;
struct render_camera;
struct s_screen_effect_settings;
struct s_observer_depth_of_field;

struct c_screen_postprocess
{
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

	static void __cdecl setup_rasterizer_for_postprocess(bool a1);
};

