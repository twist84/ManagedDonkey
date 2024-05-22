#include "render/screen_postprocess.hpp"

#include "memory/module.hpp"

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

void __cdecl c_screen_postprocess::setup_rasterizer_for_postprocess(bool a1)
{
	INVOKE(0x00A63510, c_screen_postprocess::setup_rasterizer_for_postprocess, a1);
}

