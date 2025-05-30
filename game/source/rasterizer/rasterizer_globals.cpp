#include "rasterizer/rasterizer_globals.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "shell/shell_windows.hpp"

#include <windows.h>

static s_frame_rate_data g_frame_rate_data;

REFERENCE_DECLARE(0x0191068C, real32, global_z_near);
REFERENCE_DECLARE(0x01910690, real32, global_z_far);
REFERENCE_DECLARE(0x050DDA00, s_rasterizer_globals, rasterizer_globals);
REFERENCE_DECLARE(0x050DDA80, s_rasterizer_timing_globals, g_rasterizer_timing_globals);

//.text:00A259A0 ; real32 __cdecl rasterizer_get_average_frame_delta()
//.text:00A25A30 ; int32 __cdecl rasterizer_get_d3d_presentation_interval()
//.text:00A25A40 ; real32 __cdecl rasterizer_get_most_recent_delta()
//.text:00A25A60 ; uns32 __cdecl rasterizer_get_most_recent_swap_index()
//.text:00A25A70 ; int32 __cdecl rasterizer_get_presentation_interval()
//.text:00A25A80 ; int64 __cdecl rasterizer_get_vblank_index()
//.text:00A25A90 ; int32 __cdecl rasterizer_get_vblank_interval()

void __cdecl rasterizer_get_z_planes(real32* z_near, real32* z_far)
{
	INVOKE(0x00A25AA0, rasterizer_get_z_planes, z_near, z_far);
}

void __cdecl rasterizer_globals_initialize()
{
	INVOKE(0x00A25B10, rasterizer_globals_initialize);
}

int32 __cdecl rasterizer_lag_timing_get_gamestate_delay()
{
	return INVOKE(0x00A25CF0, rasterizer_lag_timing_get_gamestate_delay);
}

void __cdecl rasterizer_lag_timing_mark_input_adjustment(int64 a1)
{
	INVOKE(0x00A25D00, rasterizer_lag_timing_mark_input_adjustment, a1);
}

void __cdecl rasterizer_lag_timing_mark_render_end()
{
	INVOKE(0x00A25D20, rasterizer_lag_timing_mark_render_end);
}

void __cdecl rasterizer_lag_timing_mark_render_present(int64 a1)
{
	INVOKE(0x00A25D40, rasterizer_lag_timing_mark_render_present, a1);
}

void __cdecl rasterizer_lag_timing_mark_render_start()
{
	INVOKE(0x00A25DB0, rasterizer_lag_timing_mark_render_start);
}

void __cdecl rasterizer_set_presentation_interval(int32 presentation_interval)
{
	INVOKE(0x00A25E60, rasterizer_set_presentation_interval, presentation_interval);

	//rasterizer_globals.presentation_interval = presentation_interval;
}

