#include "visibility/visibility_collection_objects.hpp"

#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "visibility/visibility_collection.hpp"

HOOK_DECLARE_CLASS(0x009732A0, c_visibility_collection, expand_sky_object);

bool render_objects_enabled = true;

int32 __cdecl c_visibility_collection::add_root_object(int32 object_index, const real_point3d* object_center, real32 object_radius, int32 player_window_index, bool lit, bool shadow_casting, bool fully_contained, int32 region_cluster_memory, s_lod_transparency lod_transparency, bool calculate_lod, bool ignore_first_person_objects, int32 ignore_first_person_user_index, uns16* a13)
{
	return INVOKE(0x00972E50, c_visibility_collection::add_root_object, object_index, object_center, object_radius, player_window_index, lit, shadow_casting, fully_contained, region_cluster_memory, lod_transparency, calculate_lod, ignore_first_person_objects, ignore_first_person_user_index, a13);
}

void __cdecl c_visibility_collection::expand_sky_object(int32 player_window_index)
{
	//INVOKE(0x009732A0, c_visibility_collection::expand_sky_object, player_window_index);

	if (get_global_items().skies.m_count)
	{
		object_marker_begin();
	
		c_visibility_collection::add_root_object(
			get_global_items().skies[0],
			global_origin3d,
			999999.0f,
			player_window_index,
			true,
			true,
			true,
			NONE,
			(s_lod_transparency)NONE,
			false,
			false,
			NONE,
			get_global_camera_collection()->__unknown4);
	
		c_visibility_collection::generate_objects_visible_subparts();
		object_marker_end();
	}
}

void __cdecl c_visibility_collection::generate_objects_visible_subparts()
{
	INVOKE(0x00973390, c_visibility_collection::generate_objects_visible_subparts);
}

