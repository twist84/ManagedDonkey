#pragma once

#include "cseries/cseries.hpp"

struct s_location;

struct s_game_cluster_bit_vectors
{
	c_static_array<c_static_flags<256>, 16> bit_vectors;
};
static_assert(sizeof(s_game_cluster_bit_vectors) == 0x200);

struct render_camera;
struct render_projection;
struct s_cluster_reference;
struct visibility_projection;

extern void __cdecl render_visibility_add_current_visible_clusters_to_frame_visible_clusters();
extern void __cdecl render_visibility_add_first_person_object_to_items(int32 user_index, int32 object_index, bool lit, bool shadow_casting);
extern int32 __cdecl render_visibility_add_instance(int32 structure_bsp_index, int32 structure_instance_index, int32 region_cluster_index);
extern int32 __cdecl render_visibility_add_object_to_items(int32 object_index, int32 player_window_index, bool lit, bool shadow_casting);
extern void __cdecl render_visibility_camera_collection_compute(const render_camera* camera, s_cluster_reference camera_cluster_reference, const render_projection* projection, int32 user_index, int32 player_window_index, bool single_cluster_only, bool a7);
extern bool __cdecl render_visibility_get_projection_depth_regions(real32* separation, const visibility_projection* projection);
extern s_game_cluster_bit_vectors* __cdecl render_visibility_get_visible_clusters_bitvector_frame();
extern bool __cdecl render_visibility_location_visible_in_frame(const s_location* location);
extern void __cdecl render_visibility_reset_visible_clusters_for_frame();

