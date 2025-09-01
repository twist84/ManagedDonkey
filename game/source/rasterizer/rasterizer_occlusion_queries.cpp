#include "rasterizer/rasterizer_occlusion_queries.hpp"

#include "cseries/cseries.hpp"

void __cdecl rasterizer_occlusion_dispose()
{
	INVOKE(0x00A75420, rasterizer_occlusion_dispose);
}

void __cdecl rasterizer_occlusion_initialize()
{
	INVOKE(0x00A754C0, rasterizer_occlusion_initialize);
}

int32 __cdecl rasterizer_occlusion_submit(e_occlusion_query_types type, int32 flags, int32 user_index, int32 player_window_index, void(__cdecl* render_callback)(int32))
{
	return INVOKE(0x00A75570, rasterizer_occlusion_submit, type, flags, user_index, player_window_index, render_callback);
}

void __cdecl rasterizer_occlusion_window_reset()
{
	INVOKE(0x00A75650, rasterizer_occlusion_window_reset);
}

bool __cdecl rasterizer_occlusions_get_result(e_occlusion_query_types type, int32 user_index, int32 player_window_index, int32* pixels_visible, int32* conditional_rendering_index)
{
	return INVOKE(0x00A75660, rasterizer_occlusions_get_result, type, user_index, player_window_index, pixels_visible, conditional_rendering_index);
}

void __cdecl rasterizer_occlusions_retrieve(int32 player_window_index)
{
	INVOKE(0x00A756D0, rasterizer_occlusions_retrieve, player_window_index);
}

