#include "render/render_tron_effect.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CLASS(0x00A7B5F0, c_tron_effect, resolve_and_process_z_camera);

void __cdecl c_tron_effect::resolve_and_process_z_camera(long player_window_index, rectangle2d* resolve_rectangle, bool use_fullsize_depth_texture)
{
	//INVOKE(0x00A7B5F0, sub_A7B5F0, player_window_index, resolve_rectangle);

	c_rasterizer_profile_scope _decals(_rasterizer_profile_element_effects, L"tron_effect");

	HOOK_INVOKE_CLASS(, c_tron_effect, resolve_and_process_z_camera, decltype(&c_tron_effect::resolve_and_process_z_camera), player_window_index, resolve_rectangle, use_fullsize_depth_texture);
}

