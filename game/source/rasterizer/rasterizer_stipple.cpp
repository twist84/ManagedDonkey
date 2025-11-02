#include "rasterizer/rasterizer_stipple.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_profile.hpp"

#include <d3d9.h>

enum
{
	k_ps_stencil_stipple_block_size = 80,
	k_ps_stencil_stipple_odd_bits = 1,
};

REFERENCE_DECLARE(0x0524B6F8, bool, g_disable_default_stipple_stencil);

HOOK_DECLARE(0x00A7A910, rasterizer_stipple_deactivate_fade);
HOOK_DECLARE(0x00A7A930, rasterizer_stipple_initialize);
HOOK_DECLARE(0x00A7AC20, rasterizer_stipple_set_fade_byte);

void __cdecl rasterizer_stipple_deactivate_fade()
{
	//INVOKE(0x00A7A910, rasterizer_stipple_deactivate_fade);

	if (g_disable_default_stipple_stencil)
		return;

	c_rasterizer::set_stencil_mode(c_rasterizer::_stencil_mode_off);
}

void __cdecl rasterizer_stipple_initialize()
{
	//INVOKE(0x00A7A930, rasterizer_stipple_initialize);

	c_rasterizer_profile_scope _stipple_initialize(_rasterizer_profile_element_total, L"stipple initialize");

	c_rasterizer::set_render_target(0, c_rasterizer::_surface_albedo, 0xFFFFFFFF);
	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
	c_rasterizer::clear(D3DCLEAR_STENCIL, 0, 0.0f, 0);

	if (c_rasterizer::set_explicit_shaders(
		c_rasterizer_globals::_shader_8x8_dither_pattern,
		_vertex_type_screen,
		_transfer_vertex_none,
		_entry_point_default))
	{
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_off);
		c_rasterizer::set_stencil_mode(c_rasterizer::_stencil_mode_old_stipple);

		int32 surface_height = c_rasterizer::get_surface_height(c_rasterizer::_surface_albedo);
		int32 surface_width = c_rasterizer::get_surface_width(c_rasterizer::_surface_albedo);

		real_vector4d block_size[1]{};
		int odd_pass[4]{};
		int pass_index = 0;

		constexpr int32 num_block_sizes = 3;
		constexpr real32 block_sizes[] = { 4.0f, 2.0f, 1.0f };
		static_assert(NUMBEROF(block_sizes) == num_block_sizes);
		for (int32 block_size_index = 0; block_size_index < num_block_sizes; block_size_index++)
		{
			csmemset(block_size, 0, sizeof(block_size));
			set_real_vector4d(block_size, block_sizes[block_size_index], block_sizes[block_size_index], block_sizes[block_size_index], block_sizes[block_size_index]);
			c_rasterizer::set_pixel_shader_constant(k_ps_stencil_stipple_block_size, NUMBEROF(block_size), block_size);

			for (int& v : odd_pass) v = FALSE;
			c_rasterizer::set_pixel_shader_constant_bool(k_ps_stencil_stipple_odd_bits, NUMBEROF(odd_pass), odd_pass);
			c_rasterizer::set_stencil_write_mask(FLAG(pass_index++));
			c_rasterizer::draw_fullscreen_quad(surface_width, surface_height);

			for (int& v : odd_pass) v = TRUE;
			c_rasterizer::set_pixel_shader_constant_bool(k_ps_stencil_stipple_odd_bits, NUMBEROF(odd_pass), odd_pass);
			c_rasterizer::set_stencil_write_mask(FLAG(pass_index++));
			c_rasterizer::draw_fullscreen_quad(surface_width, surface_height);
		}

		c_rasterizer::set_stencil_mode(c_rasterizer::_stencil_mode_off);
	}
}

void __cdecl rasterizer_stipple_set_fade_byte(int32 alpha, int32 write_heat_flag, int32 write_decal_occlusion_flag, int32 first_person)
{
	//INVOKE(0x00A7AC20, rasterizer_stipple_set_fade_byte, alpha, write_heat_flag, write_decal_occlusion_flag, first_person);

	if (g_disable_default_stipple_stencil)
		return;

	c_rasterizer::set_stencil_mode_with_value(
		c_rasterizer::_stencil_mode_object_rendering,
		(write_decal_occlusion_flag != 0 ? FLAG(5) : 0)
		+ (write_heat_flag != 0 ? FLAG(6) : 0)
		+ (first_person ? FLAG(7) : 0)
		+ ((alpha >> 3) & 0x1F));
}

//.text:00A7AC90 ; void __cdecl sub_A7AC90(void(*)(int32, int32), int32, int32)

void __cdecl sub_A7ACE0()
{
	INVOKE(0x00A7ACE0, sub_A7ACE0);
}

//.text:00A7AD00 ; void __cdecl sub_A7AD00()

void __cdecl sub_A7AD20()
{
	INVOKE(0x00A7AD20, sub_A7AD20);
}

