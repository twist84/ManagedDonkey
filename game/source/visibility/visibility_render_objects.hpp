#pragma once

#include "cseries/cseries.hpp"

struct s_lod_transparency;
struct s_model_level_of_detail;
struct s_render_object_info;

extern long __cdecl visibility_render_objects_compute_level_of_detail(long object_index, long player_window_index, s_model_level_of_detail const* level_of_detail_definition, real32 lod_distance, char last_lod);
extern void __cdecl visibility_render_objects_compute_lod_transparency(long object_index, real32 shadow_fade_distance, s_lod_transparency* desired_alpha);
extern bool __cdecl visibility_render_objects_find_geometry(long object_index, long render_model_index, long player_window_index, short* desired_level_of_detail, char last_level_of_detail, uint8* mesh_indices, long* region_count, bool* using_old_permutation, uint16* a9);
extern short __cdecl visibility_render_objects_find_lightmap_object_index(long object_index);
extern bool __cdecl visibility_render_objects_geometry_in_cache(long render_model_index, long region_count, char const* region_permutation_indices, char* last_region_permutation_indices, long level_of_detail, bool desired_geometry, uint8* mesh_indices, bool* using_old_permutation);
extern char __cdecl visibility_render_objects_get_last_level_of_detail_rendered(long object_index, long player_window_index);
extern bool __cdecl visibility_render_objects_get_model_info(long object_index, real32 lod_distance, real32 screen_pixels, long player_window_index, s_render_object_info* info);
extern void __cdecl visibility_render_objects_get_region_zsort_offsets(long object_index, long model_definition_index, s_render_object_info* info);
extern void __cdecl visibility_render_objects_set_last_level_of_detail_rendered(long object_index, long player_window_index, char lod);

