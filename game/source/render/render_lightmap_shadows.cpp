#include "render/render_lightmap_shadows.hpp"

#include "cache/cache_files.hpp"
#include "cutscene/cinematics.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "rasterizer/rasterizer_performance_throttles.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "visibility/visibility_collection_objects.hpp"

REFERENCE_DECLARE(0x01918160, bool, c_lightmap_shadows_view::g_debug_shadow_screenspace);
REFERENCE_DECLARE(0x01918164, real32, shadow_fade_distance_scale);
REFERENCE_DECLARE(0x01918168, real32, shadow_drastic_distance_scale);
REFERENCE_DECLARE(0x0191816C, real32, shadow_reduce_distance_scale);
REFERENCE_DECLARE(0x01918170, real32, shadow_decrease_distance_blend);
REFERENCE_DECLARE(0x05115B3E, bool, c_lightmap_shadows_view::g_debug_shadow_bounds);
REFERENCE_DECLARE(0x05115B3F, bool, c_lightmap_shadows_view::g_debug_shadow_bounds_solid);
REFERENCE_DECLARE(0x05115B40, bool, c_lightmap_shadows_view::g_debug_shadow_opaque);            // unused
REFERENCE_DECLARE(0x05115B41, bool, c_lightmap_shadows_view::g_debug_shadow_histencil);
REFERENCE_DECLARE(0x05115B42, bool, c_lightmap_shadows_view::g_debug_shadow_force_hi_res);      // unused
REFERENCE_DECLARE(0x05115B43, bool, c_lightmap_shadows_view::g_debug_objectspace_stencil_clip);
REFERENCE_DECLARE(0x05115B44, bool, c_lightmap_shadows_view::g_debug_force_fancy_shadows);      // unused
REFERENCE_DECLARE(0x05115B45, bool, c_lightmap_shadows_view::g_debug_force_old_shadows);        // unused

HOOK_DECLARE_CLASS_MEMBER(0x00A68C70, c_lightmap_shadows_view, render);

// $TODO remove this once all performance throttles are added back
#define USE_GLOBAL_PREFERENCES

#if defined(USE_GLOBAL_PREFERENCES)
static int32 shadow_generate_counts[k_quality_setting_count]
{
	0,
	3,
	5
};
static real32 shadow_quality_lods[k_quality_setting_count]
{
	0.0f,
	0.7f,
	1.0f
};
#endif

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

void c_lightmap_shadows_view::compute_visibility(int32 object_index, int32 forced_shadow_receiver_object_index)
{
	INVOKE_CLASS_MEMBER(0x00A682D0, c_lightmap_shadows_view, compute_visibility, object_index, forced_shadow_receiver_object_index);
}

int32 __cdecl c_visible_items::get_camera_root_objects_count()
{
	return INVOKE(0x00A68470, c_visible_items::get_camera_root_objects_count);
}

//.text:00A68490 ; public: static int32 __cdecl c_lightmap_shadows_view::get_shadow_apply_shader(c_lightmap_shadows_view::e_shadow_mode)
//.text:00A684A0 ; void __cdecl hs_disable_cinematic_lightmap_shadow()
//.text:00A684B0 ; void __cdecl hs_enable_cinematic_lightmap_shadow()
//.text:00A684C0 ; bool __cdecl obb_visible(s_oriented_bounding_box* obb)
//.text:00A68640 ; static void __cdecl c_lightmap_shadows_view::object_shadow_get_potential_bounds(const real_point3d* bounding_sphere_center, real32 bounding_sphere_radius, real_point3d* out_shadow_bounds_center, real32* out_shadow_bounds_radius)

bool __cdecl c_lightmap_shadows_view::object_shadow_visible(int32 object_index, s_oriented_bounding_box* obb, light_geometry* geometry)
{
	return INVOKE(0x00A68670, c_lightmap_shadows_view::object_shadow_visible, object_index, obb, geometry);
}

void c_lightmap_shadows_view::render(int32 ignore_object_index)
{
	//INVOKE_CLASS_MEMBER(0x00A68C70, c_lightmap_shadows_view, render, ignore_object_index);

#if defined(USE_GLOBAL_PREFERENCES)
	e_quality_setting shadow_quality = global_preferences_get_shadow_quality();
	int32 shadow_generate_count = shadow_generate_counts[shadow_quality];
	real32 shadow_quality_lod = shadow_quality_lods[shadow_quality];
#else
	s_performance_throttles* performance_throttles = c_performance_throttles::get_current_performance_throttles();
	int32 shadow_generate_count = performance_throttles->shadow_generate_count;
	real32 shadow_quality_lod = performance_throttles->shadow_quality_lod;
#endif

	int32 shadows_to_generate = 0;
	if (shadow_generate_count > 0 && render_objects_enabled)
	{
		s_visible_items& global_items = get_global_items();
		int32 root_object_count = c_visible_items::get_camera_root_objects_count();
		for (int32 root_object_index = 0; root_object_index < root_object_count; root_object_index++)
		{
			s_visible_object_hierarchy& root_object = global_items.root_objects[root_object_index];
			if (!TEST_BIT(root_object.flags, 1) || !root_object.visibility_object_index_count)
			{
				continue;
			}

			int32 object_index = global_items.objects[root_object.first_visibility_object_index].object_index;
			object_datum* object = OBJECT_GET(object_datum, object_index);

			bool should_be_visible = render_object_should_be_visible(object_index);
			if (object_index == ignore_object_index
				|| object->object.parent_object_index != NONE
				|| !should_be_visible
				|| !global_items.root_objects.data[root_object_index].lod_transparency.shadow_alpha)
			{
				continue;
			}

			real32 shadow_alpha = (real32)global_items.root_objects[root_object_index].lod_transparency.shadow_alpha / 255.0f;
			real32 shadow_resolution = 0.0f;
			if (c_player_view::x_current_player_view)
			{
				const render_projection* render_projection = c_player_view::x_current_player_view->get_render_projection();
				real32 sphere_diameter_in_pixels = render_projection_sphere_diameter_in_pixels(
					render_projection,
					&object->object.bounding_sphere_center,
					object->object.scale * object->object.bounding_sphere_radius);

				shadow_resolution = (sphere_diameter_in_pixels * shadow_quality_lod) / (real32)c_rasterizer::render_globals.k_shadow_resolution;
			}

			c_visible_items::push_marker();
			{
				c_lightmap_shadows_view::compute_visibility(object_index, NONE);

				struct object_definition* object_definition = TAG_GET_SAFE(OBJECT_TAG, struct object_definition, object->definition_index);
				if (object_definition
					&& object_definition->object.lightmap_shadow_mode == _lightmap_shadow_mode_blur
					&& object_definition->object.model.index != NONE)
				{
					c_lightmap_shadows_view::render_ambient(shadow_alpha, shadow_resolution, object_index, object_definition->object.model.index);
				}
				else
				{
					c_lightmap_shadows_view::submit_visibility_and_render(shadow_alpha, shadow_resolution);
				}
			}
			c_visible_items::pop_marker();
		}

		if (!cinematic_in_progress())
		{
			if (shadows_to_generate > 2 * shadow_generate_count)
			{
				shadow_fade_distance_scale = shadow_fade_distance_scale * shadow_drastic_distance_scale;
			}
			else if (shadows_to_generate > shadow_generate_count)
			{
				shadow_fade_distance_scale = shadow_fade_distance_scale * shadow_reduce_distance_scale;
			}
			else if (shadows_to_generate < shadow_generate_count - 1)
			{
				shadow_fade_distance_scale = (shadow_decrease_distance_blend * shadow_fade_distance_scale) + (1.0f - shadow_decrease_distance_blend);
			}

			if (shadow_fade_distance_scale <= 1.0f)
			{
				shadow_fade_distance_scale = 1.0f;
			}
		}
	}
	//c_render_information::add_shadow_count(shadows_to_generate);
}

void c_lightmap_shadows_view::render_ambient(real32 shadow_alpha, real32 shadow_resolution, int32 object_index, int32 model_index)
{
	INVOKE_CLASS_MEMBER(0x00A68F30, c_lightmap_shadows_view, render_ambient, shadow_alpha, shadow_resolution, object_index, model_index);
}

//.text:00A6B0C0 ; void c_lightmap_shadows_view::render_first_person(int32 object_index, int32 player_window_index)
//.text:00A6B180 ; void __cdecl render_lightmap_shadow_calculate_radius(int32, const real_point3d*, const real_vector3d*, real32*, real32*, bool, bool)
//.text:00A6B310 ; void c_lightmap_shadows_view::render_setup_internal() const 
//.text:00A6B340 ; void c_lightmap_shadows_view::render_submit_visibility()
//.text:00A6B360 ; 
//.text:00A6B370 ; void c_lightmap_shadows_view::setup_camera()

void c_lightmap_shadows_view::submit_visibility_and_render(real32 shadow_alpha, real32 shadow_resolution)
{
	INVOKE_CLASS_MEMBER(0x00A6B4B0, c_lightmap_shadows_view, submit_visibility_and_render, shadow_alpha, shadow_resolution);
}

