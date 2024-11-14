#pragma once

#include "rasterizer/rasterizer.hpp"

struct s_screen_effect_shader_sample_result;

extern void __cdecl render_screen_shaders(
	s_screen_effect_shader_sample_result* result,
	long a2,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	real_rectangle2d const* rect);

