#include "render/render_visibility_collection.hpp"

#include "cseries/location.hpp"
#include "render/render_cameras.hpp"
#include "render/render_visibility.hpp"
#include "visibility/visibility_collection.hpp"

void __cdecl render_visibility_add_current_visible_clusters_to_frame_visible_clusters()
{
	INVOKE(0x00A52C20, render_visibility_add_current_visible_clusters_to_frame_visible_clusters);
}

void __cdecl render_visibility_add_first_person_object_to_items(int32 user_index, int32 object_index, bool lit, bool shadow_casting)
{
	INVOKE(0x00A52C90, render_visibility_add_first_person_object_to_items, user_index, object_index, lit, shadow_casting);
}

int32 __cdecl render_visibility_add_instance(int32 structure_bsp_index, int32 structure_instance_index, int32 region_cluster_index)
{
	return INVOKE(0x00A53050, render_visibility_add_instance, structure_bsp_index, structure_instance_index, region_cluster_index);
}

int32 __cdecl render_visibility_add_object_to_items(int32 object_index, int32 player_window_index, bool lit, bool shadow_casting)
{
	return INVOKE(0x00A53160, render_visibility_add_object_to_items, object_index, player_window_index, lit, shadow_casting);
}

void __cdecl render_visibility_camera_collection_compute(const render_camera* camera, s_cluster_reference camera_cluster_reference, const render_projection* projection, int32 user_index, int32 player_window_index, bool single_cluster_only, bool a7)
{
	INVOKE(0x00A53250, render_visibility_camera_collection_compute, camera, camera_cluster_reference, projection, user_index, player_window_index, single_cluster_only, a7);

	//visibility_projection visibility_projection{};
	//render_visibility_build_projection(camera, projection, camera_cluster_reference, &visibility_projection);
	//
	//int32 flags = (debug_pvs_render_all ? FLAG(6) : 0) | (single_cluster_only ? FLAG(8) : 0);
	//
	//// Not in Halo 3
	//if (a7)
	//	flags |= (FLAG(4) | FLAG(5));
	//
	//c_visibility_collection::compute(
	//	get_global_camera_collection(),
	//	0,
	//	flags,
	//	&visibility_projection,
	//	0,
	//	&visibility_projection.basis_to_world.origin,
	//	1024.0f,
	//	0,
	//	camera_cluster_reference,
	//	user_index,
	//	player_window_index);
}

//.text:00A532F0 ; 

bool __cdecl render_visibility_get_projection_depth_regions(real32* separation, const visibility_projection* projection)
{
	return INVOKE(0x00A53310, render_visibility_get_projection_depth_regions, separation, projection);
}

s_game_cluster_bit_vectors* __cdecl render_visibility_get_visible_clusters_bitvector_frame()
{
	return INVOKE(0x00A53320, render_visibility_get_visible_clusters_bitvector_frame);
}

bool __cdecl render_visibility_location_visible_in_frame(const s_location* location)
{
	return INVOKE(0x00A53330, render_visibility_location_visible_in_frame, location);
}

void __cdecl render_visibility_reset_visible_clusters_for_frame()
{
	INVOKE(0x00A53380, render_visibility_reset_visible_clusters_for_frame);
}

