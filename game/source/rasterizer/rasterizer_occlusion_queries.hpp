#pragma once

enum e_occlusion_query_types
{
	k_occlusion_query_type_lens_flare = 0,
	k_occlusion_query_type_distortion = 1,
	k_number_of_occlusion_query_types = 2,
};

extern void __cdecl rasterizer_occlusion_dispose();
extern void __cdecl rasterizer_occlusion_initialize();
extern int32 __cdecl rasterizer_occlusion_submit(e_occlusion_query_types type, int32 flags, int32 user_index, int32 player_window_index, void(__cdecl* render_callback)(int32));
extern void __cdecl rasterizer_occlusion_window_reset();
extern bool __cdecl rasterizer_occlusions_get_result(e_occlusion_query_types type, int32 user_index, int32 player_window_index, int32* pixels_visible, int32* conditional_rendering_index);
extern void __cdecl rasterizer_occlusions_retrieve(int32 player_window_index);

