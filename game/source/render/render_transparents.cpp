#include "render/render_transparents.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "render/lighting_interface.hpp"
#include "render/render_debug.hpp"
#include "render/render_structure.hpp"
#include "rasterizer/rasterizer_profile.hpp"

REFERENCE_DECLARE(0x0191B560, int32, c_transparency_renderer::m_current_marker_index);
REFERENCE_DECLARE(0x0191B590, c_transparency_renderer::c_transparent_sorted_order, c_transparency_renderer::transparent_sorted_order);
REFERENCE_DECLARE(0x05216201, bool, c_transparency_renderer::m_using_active_camo);
REFERENCE_DECLARE(0x05216202, bool, c_transparency_renderer::m_needs_active_camo_ldr_resolve);
REFERENCE_DECLARE(0x05216204, int16, render_debug_transparent_sort_method);
REFERENCE_DECLARE(0x0521620C, c_transparency_renderer::c_markers, c_transparency_renderer::m_markers);
REFERENCE_DECLARE(0x05216230, int32, c_transparency_renderer::m_total_transparent_count);
REFERENCE_DECLARE(0x05216234, rectangle2d, c_transparency_renderer::m_active_camo_resolve_bounds);
REFERENCE_DECLARE(0x0521623C, real_vector4d, c_transparency_renderer::m_active_camo_distort_bounds);
REFERENCE_DECLARE(0x05216250, c_transparency_renderer::c_transparents, c_transparency_renderer::transparents);

HOOK_DECLARE_CLASS(0x00A73640, c_transparency_renderer, render);

bool render_debug_transparents = false;
bool render_debug_slow_transparents = false;
bool render_debug_transparent_cull = true;
bool render_debug_transparent_cull_flip = false;
//int16 render_debug_transparent_sort_method = 0;
bool render_transparents_enabled = true;

void __cdecl c_transparency_renderer::pop_marker()
{
	INVOKE(0x00A73580, c_transparency_renderer::pop_marker);
}

void __cdecl c_transparency_renderer::push_marker()
{
	INVOKE(0x00A735D0, c_transparency_renderer::push_marker);
}

//.text:00A73620 ; 

void __cdecl c_transparency_renderer::render(bool depth_test)
{
	//INVOKE(0x00A73640, c_transparency_renderer::render, depth_test);

	if (render_transparents_enabled)
	{
		if (depth_test)
		{
			c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_read);
		}

		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_color);

		c_rasterizer::e_cull_mode cull_mode = c_rasterizer::_cull_mode_off;
		if (render_debug_transparent_cull)
		{
			cull_mode = c_rasterizer::_cull_mode_cw;
			if (render_debug_transparent_cull_flip)
			{
				cull_mode = c_rasterizer::_cull_mode_ccw;
			}
		}
		c_rasterizer::set_cull_mode(cull_mode);

		//c_rasterizer::e_surface depth_stencil_surface = c_rasterizer::get_depth_stencil_surface();
		//c_rasterizer::e_surface only_depth_stencil_surface = c_rasterizer::get_read_only_depth_stencil_surface(depth_stencil_surface);
		//c_rasterizer::set_depth_stencil_surface(only_depth_stencil_surface);

		if (!c_structure_renderer::lightmaps_available_on_all_active_bsps() || rasterizer_force_default_lighting())
		{
			c_lighting_interface::setup_default_lighting();
		}

		int32 transparent_index = c_transparency_renderer::m_markers[c_transparency_renderer::m_current_marker_index][0].starting_transparent_index;
		for (int32 total_transparent_count = c_transparency_renderer::m_total_transparent_count; transparent_index < total_transparent_count; transparent_index++)
		{
			int32 order_count = c_transparency_renderer::transparent_sorted_order.m_order.get_count();
			ASSERT(transparent_index < order_count);

			uns16 order = c_transparency_renderer::transparent_sorted_order.m_order[transparent_index];

			s_transparent_types* transparent = &c_transparency_renderer::transparents[order];

			if (render_debug_transparents)
			{
				char sort_position[256]{};

				if (transparent->importance < k_real_epsilon)
				{
					csnzprintf(sort_position, sizeof(sort_position), "l%d i%d s%d",
						transparent->sort_layer,
						order,
						transparent_index);
				}
				else
				{
					csnzprintf(sort_position, sizeof(sort_position), "l%d i%d s%d w%5.3f",
						transparent->sort_layer,
						order,
						transparent_index,
						transparent->importance);
				}

				render_debug_string_at_point(&transparent->centroid, sort_position, global_real_argb_green);

				wchar_t sort_position_wide[256]{};
				ascii_string_to_wchar_string(sort_position, sort_position_wide, NUMBEROF(sort_position_wide), NULL);
				rasterizer_profile_begin_event(_rasterizer_profile_element_transparents, sort_position_wide);
			}

			transparent->render_callback(transparent->user_data, transparent->user_context);

			if (render_debug_transparents)
			{
				rasterizer_profile_end_event();
			}
		}

		//c_rasterizer::set_depth_stencil_surface(depth_stencil_surface);
	}
}

//.text:00A736E0 ; public: static void __cdecl c_transparency_renderer::reset()

void __cdecl c_transparency_renderer::set_active_camo_bounds(const rectangle2d* window_pixel_bounds, const rectangle2d* render_pixel_bounds)
{
	INVOKE(0x00A73710, c_transparency_renderer::set_active_camo_bounds, window_pixel_bounds, render_pixel_bounds);
}

//.text:00A73770 ; public: static void __cdecl c_transparency_renderer::resolve_framebuffer_for_active_camo()

void __cdecl c_transparency_renderer::sort()
{
	INVOKE(0x00A73880, c_transparency_renderer::sort);
}

