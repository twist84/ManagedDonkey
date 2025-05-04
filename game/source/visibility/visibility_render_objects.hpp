#pragma once

#include "cseries/cseries.hpp"

struct s_lod_transparency;
struct s_model_level_of_detail;
struct s_render_object_info;

extern int32 __cdecl visibility_render_objects_compute_level_of_detail(int32 object_index, int32 player_window_index, s_model_level_of_detail const* level_of_detail_definition, real32 lod_distance, int8 last_lod);
extern void __cdecl visibility_render_objects_compute_lod_transparency(int32 object_index, real32 shadow_fade_distance, s_lod_transparency* desired_alpha);
extern bool __cdecl visibility_render_objects_find_geometry(int32 object_index, int32 render_model_index, int32 player_window_index, int16* desired_level_of_detail, int8 last_level_of_detail, uns8* mesh_indices, int32* region_count, bool* using_old_permutation, uns16* a9);
extern int16 __cdecl visibility_render_objects_find_lightmap_object_index(int32 object_index);
extern bool __cdecl visibility_render_objects_geometry_in_cache(int32 render_model_index, int32 region_count, int8 const* region_permutation_indices, int8* last_region_permutation_indices, int32 level_of_detail, bool desired_geometry, uns8* mesh_indices, bool* using_old_permutation);
extern int8 __cdecl visibility_render_objects_get_last_level_of_detail_rendered(int32 object_index, int32 player_window_index);
extern bool __cdecl visibility_render_objects_get_model_info(int32 object_index, real32 lod_distance, real32 screen_pixels, int32 player_window_index, s_render_object_info* info);
extern void __cdecl visibility_render_objects_get_region_zsort_offsets(int32 object_index, int32 model_definition_index, s_render_object_info* info);
extern void __cdecl visibility_render_objects_set_last_level_of_detail_rendered(int32 object_index, int32 player_window_index, int8 lod);

