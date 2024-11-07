#include "rasterizer/rasterizer_stipple.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CALL(0x00A39B17, rasterizer_stipple_initialize);

//.text:00A7A910 ; void __cdecl rasterizer_stipple_deactivate_fade()

void __cdecl rasterizer_stipple_initialize()
{
	c_d3d_pix_event _stipple_initialize(g_rasterizer_profile_pix_colors[1], L"stipple initialize");

	INVOKE(0x00A7A930, rasterizer_stipple_initialize);
}

//.text:00A7AC20 ; rasterizer_stipple_set_fade_byte

