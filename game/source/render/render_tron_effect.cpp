#include "render/render_tron_effect.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE(0x00A7B5F0, sub_A7B5F0);

void __cdecl sub_A7B5F0(long player_index, short_rectangle2d* bounds)
{
	//INVOKE(0x00A7B5F0, sub_A7B5F0, player_index, bounds);

	c_rasterizer_profile_scope _decals(_rasterizer_profile_element_effects, L"tron_effect");

	HOOK_INVOKE(, sub_A7B5F0, player_index, bounds);
}

