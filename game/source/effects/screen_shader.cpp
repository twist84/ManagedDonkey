#include "effects/screen_shader.hpp"

#include "rasterizer/rasterizer_profile.hpp"

void __cdecl render_screen_shaders(
	s_screen_effect_shader_sample_result* result,
	int32 a2,
	c_rasterizer::e_surface surface_a,
	c_rasterizer::e_surface surface_b,
	real_rectangle2d const* rect)
{
	c_rasterizer_profile_scope _screen_shaders(_rasterizer_profile_element_total, L"screen_shaders");

	INVOKE(0x014E6F60, render_screen_shaders, result, a2, surface_a, surface_b, rect);
}

