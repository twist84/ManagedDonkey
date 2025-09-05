#include "render/render_water.hpp"

#include "geometry/geometry_definitions_new.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render_methods/render_method_submit.hpp"

//#define H3_WATER_RENDERER_BEHAVIOR

// H3 { 0.1f, 0.2f, 0.12f }
// HO { 1.0f, 0.0f,  0.0f }
REFERENCE_DECLARE(0x01692D88, real_point3d, k_default_underwater_fog_color); // can't set variables referenced from .rdata

REFERENCE_DECLARE(0x019146E0, real32, k_ripple_buffer_radius) = 10.0f;
REFERENCE_DECLARE(0x019146EC, bool, render_water_tessellation_enabled) = true;
REFERENCE_DECLARE(0x019146ED, bool, render_water_interaction_enabled) = true;
REFERENCE_DECLARE(0x019146EE, bool, render_water_enabled) = true;
REFERENCE_DECLARE(0x019146EF, bool, performance_throttles_water_interaction) = true;
REFERENCE_DECLARE(0x019146F0, bool, performance_throttles_water_tessellation) = true;
REFERENCE_DECLARE(0x019146F1, bool, render_water_tessllation_on) = true;
REFERENCE_DECLARE(0x019146F2, bool, render_water_interaction_on) = true;

REFERENCE_DECLARE(0x050FAAEC, int, g_is_underwater); // BOOL
REFERENCE_DECLARE(0x050FAB0C, int32, g_ripple_particle_count);
REFERENCE_DECLARE(0x050FAB10, int32, g_new_ripple_buffer_index);
REFERENCE_DECLARE(0x050FAB14, int32, g_water_new_interaction_event_count);
REFERENCE_DECLARE(0x050FAB18, bool, render_water_wireframe_enabled);
REFERENCE_DECLARE_ARRAY(0x050FAB20, int32, g_water_render_structure_part_indices, 512);
REFERENCE_DECLARE(0x050FB320, int32, g_water_render_structure_part_count);
REFERENCE_DECLARE(0x050FB324, real32, g_underwater_murkiness);
REFERENCE_DECLARE(0x050FB328, DirectX::XMMATRIX, g_xm_view_proj_inv);
REFERENCE_DECLARE(0x050FB368, DirectX::XMMATRIX, g_xm_view_proj);
REFERENCE_DECLARE(0x050FB3A8, real_point3d, g_underwater_fog_color);
REFERENCE_DECLARE(0x050FB3B4, real32, real_50FB3B4);
REFERENCE_DECLARE(0x050FB3B8, real32, real_50FB3B8);
REFERENCE_DECLARE(0x050FB3BC, real_vector3d, g_interaction_buffer_center);
REFERENCE_DECLARE(0x050FB3C8, real_point3d, g_camera_position);

HOOK_DECLARE_CLASS(0x00A363E0, c_water_renderer, render_shading);
HOOK_DECLARE_CLASS(0x00A36550, c_water_renderer, render_tessellation);
HOOK_DECLARE_CLASS(0x00A36580, c_water_renderer, render_underwater_fog);

HOOK_DECLARE_CLASS(0x00A36970, c_water_renderer, ripple_add);
HOOK_DECLARE_CLASS(0x00A37000, c_water_renderer, ripple_apply);
HOOK_DECLARE_CLASS(0x00A372B0, c_water_renderer, ripple_slope);
HOOK_DECLARE_CLASS(0x00A37350, c_water_renderer, ripple_update);

void __cdecl c_water_renderer::set_player_window(int32 window_index, int32 window_count, bool is_widescreen)
{
	INVOKE(0x006041F0, c_water_renderer::set_player_window, window_index, window_count, is_widescreen);
}

//.text:00A35A30 ; void __cdecl __tls_set_g_water_interaction_event_allocator(void*)
//.text:00A35A50 ; 
//.text:00A35A60 ; 
//.text:00A35A70 ; tls
//.text:00A35AB0 ; tls

void __cdecl c_water_renderer::dispose()
{
	INVOKE(0x00A35AE0, dispose);
}

void __cdecl c_water_renderer::dispose_from_old_map()
{
	INVOKE(0x00A35BA0, dispose_from_old_map);
}

void __cdecl c_water_renderer::game_interation_event_add(int32 water_ripple_definition_index, const real_point3d* position, const real_vector3d* object_velocity, const real_vector3d* water_velocity)
{
	INVOKE(0x00A35C40, c_water_renderer::game_interation_event_add, water_ripple_definition_index, position, object_velocity, water_velocity);
}

void __cdecl c_water_renderer::game_update()
{
	INVOKE(0x00A35CB0, game_update);
}

void __cdecl c_water_renderer::initialize()
{
	INVOKE(0x00A35D70, initialize);
}

void __cdecl c_water_renderer::initialize_for_new_map()
{
	INVOKE(0x00A36220, initialize_for_new_map);
}

bool __cdecl c_water_renderer::is_active_ripple_exist()
{
	return INVOKE(0x00A362B0, c_water_renderer::is_active_ripple_exist);
}

void __cdecl c_water_renderer::render_cluster_parts(e_entry_point entry_point, int32 mesh_part_mask)
{
	INVOKE(0x00A362D0, c_water_renderer::render_cluster_parts, entry_point, mesh_part_mask);
}

void __cdecl c_water_renderer::render_shading()
{
	//INVOKE(0x00A363E0, c_water_renderer::render_shading);

	g_accum_LDR_hack = true;
	{
		c_rasterizer::set_pixel_shader_constant(k_ps_water_view_xform_inverse, 4, (real_vector4d*)&g_xm_view_proj_inv);

		real32 z_near;
		real32 z_far;
		rasterizer_get_z_planes(&z_near, &z_far);

		real_vector4d depth_constants{};
		set_real_vector4d(&depth_constants, -((z_far / (z_far - z_near)) * z_near), z_far / (z_far - z_near), 0.0f, 0.0f);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_view_depth_constant, 1, (real_vector4d*)&depth_constants);

		c_rasterizer::set_vertex_shader_constant(k_vs_water_ripple_buffer_center, 1, (real_vector4d*)&g_interaction_buffer_center);
		c_rasterizer::set_vertex_shader_constant(k_vs_water_ripple_buffer_radius, 1, (real_vector4d*)&k_ripple_buffer_radius);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_player_view_constant, 1, &g_player_window_constants);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_camera_position, 1, (real_vector4d*)&g_camera_position);
		c_rasterizer::set_pixel_shader_constant_bool(k_ps_water_is_underwater, 1, &g_is_underwater);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_underwater_murkiness, 1, (real_vector4d*)&g_underwater_murkiness);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_underwater_fog_color, 1, (real_vector4d*)&g_underwater_fog_color);

#if defined(H3_WATER_RENDERER_BEHAVIOR)
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_read);
#else
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_write);
#endif

		if (g_is_underwater)
		{
			c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_ccw);
		}
		else
		{
			c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
		}

		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);

		if (render_water_wireframe_enabled)
		{
			c_rasterizer::set_fill_mode(c_rasterizer::_fill_mode_wireframe);
		}

		c_water_renderer::render_cluster_parts(_entry_point_static_lighting_per_vertex, 3);

#if defined(H3_WATER_RENDERER_BEHAVIOR)
		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_none);
		c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_write);
		c_water_renderer::render_cluster_parts(_entry_point_shadow_generate, 3);
		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);
		c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil_read_only);
#endif

		c_rasterizer::set_fill_mode(c_rasterizer::_fill_mode_solid);
	}
	g_accum_LDR_hack = false;
}

void __cdecl c_water_renderer::render_tessellation(bool is_screenshot)
{
	//INVOKE(0x00A36550, c_water_renderer::render_tessellation, is_screenshot);

	if (render_water_tessllation_on)
	{
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_off);
		c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);
		c_water_renderer::render_cluster_parts(_entry_point_water_tessellation, 3);
	}
}

void __cdecl c_water_renderer::render_underwater_fog()
{
	//INVOKE(0x00A36580, c_water_renderer::render_underwater_fog);

	if (g_is_underwater)
	{
		c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_off);
		c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
		c_rasterizer::set_color_write_enable(0, c_rasterizer::_color_write_enable_all);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_view_xform_inverse, 4, (real_vector4d*)&g_xm_view_proj_inv);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_player_view_constant, 1, (real_vector4d*)&g_player_window_constants);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_camera_position, 1, (real_vector4d*)&g_camera_position);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_underwater_murkiness, 1, (real_vector4d*)&g_underwater_murkiness);
		c_rasterizer::set_pixel_shader_constant(k_ps_water_underwater_fog_color, 1, (real_vector4d*)&g_underwater_fog_color);
		
		c_rasterizer::set_explicit_shaders(c_rasterizer_globals::_shader_water_ripple, _vertex_type_ripple, _transfer_vertex_none, _entry_point_shadow_apply); 

#if defined(H3_WATER_RENDERER_BEHAVIOR)
		c_rasterizer::set_surface_as_texture(0, c_rasterizer::_surface_post_LDR);
#else
		c_rasterizer::set_surface_as_texture(0, c_rasterizer::_surface_accum_LDR);
#endif

		c_rasterizer::set_sampler_address_mode(0, c_rasterizer::_sampler_address_clamp, c_rasterizer::_sampler_address_clamp, c_rasterizer::_sampler_address_clamp);
		c_rasterizer::set_sampler_filter_mode(0, c_rasterizer::_sampler_filter_bilinear);

		c_rasterizer::set_surface_as_texture(1, c_rasterizer::_surface_depth_stencil);
		c_rasterizer::set_sampler_address_mode(1, c_rasterizer::_sampler_address_clamp, c_rasterizer::_sampler_address_clamp, c_rasterizer::_sampler_address_clamp);

#if defined(H3_WATER_RENDERER_BEHAVIOR)
		c_rasterizer::set_sampler_filter_mode(1, c_rasterizer::_sampler_filter_bilinear);
#else
		c_rasterizer::set_sampler_filter_mode(1, c_rasterizer::_sampler_filter_point);
#endif


		c_rasterizer::draw_vertices(c_rasterizer_index_buffer::_primitive_type_quad_list, 0, 1);
	}
}

void __cdecl c_water_renderer::render_water_part(const s_render_geometry* geometry, int32 mesh_index, int32 part_index, e_entry_point entry_point, const geometry_material* materials)
{
	INVOKE(0x00A36660, c_water_renderer::render_water_part, geometry, mesh_index, part_index, entry_point, materials);
}

//.text:00A36930 ; 

void __cdecl c_water_renderer::ripple_add(uns32 valid_event_count)
{
	//INVOKE(0x00A36970, c_water_renderer::ripple_add, valid_event_count);

	c_rasterizer_profile_scope _ripple_add(_rasterizer_profile_element_total, L"ripple_add");

	HOOK_INVOKE_CLASS(, c_water_renderer, ripple_add, decltype(&c_water_renderer::ripple_add), valid_event_count);
}

void __cdecl c_water_renderer::ripple_apply()
{
	//INVOKE(0x00A37000, c_water_renderer::ripple_apply);

	if (g_main_render_timing_data->game_dt > 0.00001f)
	{
		c_rasterizer_profile_scope _ripple_apply(_rasterizer_profile_element_total, L"ripple_apply");
	
		HOOK_INVOKE_CLASS(, c_water_renderer, ripple_apply, decltype(&c_water_renderer::ripple_apply));
	}
}

uns32 __cdecl c_water_renderer::ripple_check_new()
{
	return INVOKE(0x00A37230, c_water_renderer::ripple_check_new);
}

void __cdecl c_water_renderer::ripple_slope()
{
	//INVOKE(0x00A372B0, c_water_renderer::ripple_slope);

	c_rasterizer_profile_scope _ripple_slope(_rasterizer_profile_element_total, L"ripple_slope");

	HOOK_INVOKE_CLASS(, c_water_renderer, ripple_slope, decltype(&c_water_renderer::ripple_slope));
}

void __cdecl c_water_renderer::ripple_update()
{
	//INVOKE(0x00A37350, c_water_renderer::ripple_update);

	c_rasterizer_profile_scope _ripple_update(_rasterizer_profile_element_total, L"ripple_update");

	HOOK_INVOKE_CLASS(, c_water_renderer, ripple_update, decltype(&c_water_renderer::ripple_update));
}

void __cdecl c_water_renderer::set_performance_throttles()
{
	INVOKE(0x00A373C0, c_water_renderer::set_performance_throttles);
}

bool __cdecl c_water_renderer::update_water_part_list()
{
	return INVOKE(0x00A373F0, c_water_renderer::update_water_part_list);
}

void __cdecl water_interaction_clear_all(int32 game_state_restore_flags)
{
	INVOKE(0x00A37B40, water_interaction_clear_all, game_state_restore_flags);
}

void c_water_renderer::frame_advance(real32 seconds_elapsed)
{
	c_water_renderer::set_performance_throttles();
	if (render_water_enabled && render_water_interaction_on)
	{
		uns32 ripple_index = c_water_renderer::ripple_check_new();
		if (c_water_renderer::is_active_ripple_exist())
		{
			c_rasterizer_profile_scope _ripple_frame_adavance(_rasterizer_profile_element_total, L"ripple_frame_adavance");

			c_water_renderer::ripple_add(ripple_index);
			c_water_renderer::ripple_update();
		}
	}
}

