#include "render/render_lightmap_shadows.hpp"

#include "memory/module.hpp"
#include "render/views/render_view.hpp"
#include "render/render_debug.hpp"

REFERENCE_DECLARE(0x01918160, bool, c_lightmap_shadows_view::g_debug_shadow_screenspace);
REFERENCE_DECLARE(0x05115B3E, bool, c_lightmap_shadows_view::g_debug_shadow_bounds);            // unused
REFERENCE_DECLARE(0x05115B3F, bool, c_lightmap_shadows_view::g_debug_shadow_bounds_solid);
REFERENCE_DECLARE(0x05115B40, bool, c_lightmap_shadows_view::g_debug_shadow_opaque);            // unused
REFERENCE_DECLARE(0x05115B41, bool, c_lightmap_shadows_view::g_debug_shadow_histencil);
REFERENCE_DECLARE(0x05115B42, bool, c_lightmap_shadows_view::g_debug_shadow_force_hi_res);      // unused
REFERENCE_DECLARE(0x05115B43, bool, c_lightmap_shadows_view::g_debug_objectspace_stencil_clip);
REFERENCE_DECLARE(0x05115B44, bool, c_lightmap_shadows_view::g_debug_force_fancy_shadows);      // unused
REFERENCE_DECLARE(0x05115B45, bool, c_lightmap_shadows_view::g_debug_force_old_shadows);        // unused

bool __cdecl lightmap_shadows_view_object_shadow_visible_hook_for_compute_visibility(int32 object_index, s_oriented_bounding_box* obb, light_geometry* geometry)
{
	bool result = c_lightmap_shadows_view::object_shadow_visible(object_index, obb, geometry);
	if (result && c_lightmap_shadows_view::g_debug_shadow_bounds)
	{
		real_rectangle3d bounds{};
		set_real_rectangle3d(&bounds,
			/* x0, x1 */ -obb->m_extents.i, obb->m_extents.i,
			/* y0, y1 */ -obb->m_extents.j, obb->m_extents.j,
			/* z0, z1 */ -obb->m_extents.k, obb->m_extents.k);
		render_debug_box_outline_oriented(true, &bounds, &obb->m_transform, global_real_argb_yellow);
	}
	return result;
}
HOOK_DECLARE_CALL(0x00A68316, lightmap_shadows_view_object_shadow_visible_hook_for_compute_visibility);

//.text:00A682D0 ; void c_lightmap_shadows_view::compute_visibility(int32 object_index, int32 forced_shadow_receiver_object_index)
//.text:00A68640 ; static void __cdecl c_lightmap_shadows_view::object_shadow_get_potential_bounds(const real_point3d* bounding_sphere_center, real32 bounding_sphere_radius, real_point3d* out_shadow_bounds_center, real32* out_shadow_bounds_radius)

bool __cdecl c_lightmap_shadows_view::object_shadow_visible(int32 object_index, s_oriented_bounding_box* obb, light_geometry* geometry)
{
	return INVOKE(0x00A68670, c_lightmap_shadows_view::object_shadow_visible, object_index, obb, geometry);
}

//.text:00A68C70 ; void c_lightmap_shadows_view::render(int32 ignore_object_index)
//.text:00A68F30 ; void c_lightmap_shadows_view::render_ambient(real32 shadow_alpha, real32 shadow_resolution, int32 object_index, int32 model_index)
//.text:00A6B0C0 ; void c_lightmap_shadows_view::render_first_person(int32 object_index, int32 player_window_index)
//.text:00A6B180 ; void __cdecl render_lightmap_shadow_calculate_radius(int32, const real_point3d*, const real_vector3d*, real32*, real32*, bool, bool)
//.text:00A6B310 ; void c_lightmap_shadows_view::render_setup_internal() const 
//.text:00A6B340 ; void c_lightmap_shadows_view::render_submit_visibility()
//.text:00A6B360 ; 
//.text:00A6B370 ; void c_lightmap_shadows_view::setup_camera()
//.text:00A6B4B0 ; void c_lightmap_shadows_view::submit_visibility_and_render(real32 shadow_alpha, real32 shadow_resolution)

