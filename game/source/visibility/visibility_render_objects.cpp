#include "visibility/visibility_render_objects.hpp"

#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "visibility/visibility_collection.hpp"

//HOOK_DECLARE_CALL(0x00A44961, visibility_render_objects_compute_lod_transparency);

int32 __cdecl visibility_render_objects_compute_level_of_detail(int32 object_index, int32 player_window_index, s_model_level_of_detail const* level_of_detail_definition, real32 lod_distance, int8 last_lod)
{
	return INVOKE(0x00A19490, visibility_render_objects_compute_level_of_detail, object_index, player_window_index, level_of_detail_definition, lod_distance, last_lod);
}

void __cdecl visibility_render_objects_compute_lod_transparency(int32 object_index, real32 lod_distance, s_lod_transparency* desired_transparency)
{
	INVOKE(0x00A195E0, visibility_render_objects_compute_lod_transparency, object_index, lod_distance, desired_transparency);
}

bool __cdecl visibility_render_objects_find_geometry(int32 object_index, int32 render_model_index, int32 player_window_index, int16* desired_level_of_detail, int8 last_level_of_detail, uns8* mesh_indices, int32* region_count, bool* using_old_permutation, uns16* a9)
{
	return INVOKE(0x00A19A40 , visibility_render_objects_find_geometry, object_index, render_model_index, player_window_index, desired_level_of_detail, last_level_of_detail, mesh_indices, region_count, using_old_permutation, a9);
}

int16 __cdecl visibility_render_objects_find_lightmap_object_index(int32 object_index)
{
	return INVOKE(0x00A19C20, visibility_render_objects_find_lightmap_object_index, object_index);
}

bool __cdecl visibility_render_objects_geometry_in_cache(int32 render_model_index, int32 region_count, int8 const* region_permutation_indices, int8* last_region_permutation_indices, int32 level_of_detail, bool desired_geometry, uns8* mesh_indices, bool* using_old_permutation)
{
	return INVOKE(0x00A19C60, visibility_render_objects_geometry_in_cache, render_model_index, region_count, region_permutation_indices, last_region_permutation_indices, level_of_detail, desired_geometry, mesh_indices, using_old_permutation);
}

int8 __cdecl visibility_render_objects_get_last_level_of_detail_rendered(int32 object_index, int32 player_window_index)
{
	return INVOKE(0x00A19D40, visibility_render_objects_get_last_level_of_detail_rendered, object_index, player_window_index);
}

bool __cdecl visibility_render_objects_get_model_info(int32 object_index, real32 lod_distance, real32 screen_pixels, int32 player_window_index, s_render_object_info* info)
{
	return INVOKE(0x00A19D60, visibility_render_objects_get_model_info, object_index, lod_distance, screen_pixels, player_window_index, info);
}

void __cdecl visibility_render_objects_get_region_zsort_offsets(int32 object_index, int32 model_definition_index, s_render_object_info* info)
{
	INVOKE(0x00A1A000, visibility_render_objects_get_region_zsort_offsets, object_index, model_definition_index, info);
}

void __cdecl visibility_render_objects_set_last_level_of_detail_rendered(int32 object_index, int32 player_window_index, int8 lod)
{
	INVOKE(0x00A1A0D0, visibility_render_objects_set_last_level_of_detail_rendered, object_index, player_window_index, lod);
}

