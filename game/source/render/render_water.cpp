#include "render/render_water.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_profile.hpp"

REFERENCE_DECLARE(0x019146EE, bool, render_water_enabled);
REFERENCE_DECLARE(0x019146F1, bool, render_water_tessellation_enabled);
REFERENCE_DECLARE(0x019146F2, bool, render_water_interaction_enabled);
REFERENCE_DECLARE(0x050FAAEC, bool, render_underwater_fog_enabled);
REFERENCE_DECLARE(0x050FAB18, bool, render_water_wireframe_enabled);

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
//.text:00A35A70 ; 
//.text:00A35AB0 ; 

void __cdecl c_water_renderer::dispose()
{
	INVOKE(0x00A35AE0, dispose);
}

void __cdecl c_water_renderer::dispose_from_old_map()
{
	INVOKE(0x00A35BA0, dispose_from_old_map);
}

//.text:00A35C40 ; public: static void __cdecl c_water_renderer::game_interation_event_add(int32, const real_point3d*, const real_vector3d*, const real_vector3d*)

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

//.text:00A362D0 ; private: static void __cdecl c_water_renderer::render_cluster_parts(e_entry_point, int32)

void __cdecl c_water_renderer::render_shading()
{
	INVOKE(0x00A363E0, c_water_renderer::render_shading);
}

void __cdecl c_water_renderer::render_tessellation(bool is_screenshot)
{
	INVOKE(0x00A36550, c_water_renderer::render_tessellation, is_screenshot);
}

void __cdecl c_water_renderer::render_underwater_fog()
{
	INVOKE(0x00A36580, c_water_renderer::render_underwater_fog);
}

//.text:00A36660 ; private: static void __cdecl c_water_renderer::render_water_part(const s_render_geometry*, int32, int32, e_entry_point, const geometry_material*)
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

//.text:00A37B40 ; void __cdecl water_interaction_clear_all(int32)

void c_water_renderer::frame_advance(real32 seconds_elapsed)
{
	c_water_renderer::set_performance_throttles();
	if (render_water_enabled && render_water_interaction_enabled)
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

