#include "rasterizer/rasterizer_globals.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x050DDA00, s_rasterizer_globals, rasterizer_globals);

REFERENCE_DECLARE(0x019106C0, long, render_globals_width);
REFERENCE_DECLARE(0x019106C8, long, render_globals_height);

bool __cdecl rasterizer_get_is_widescreen()
{
	//return INVOKE(0x00A1FC90, rasterizer_get_is_widescreen);

	return (render_globals_width / render_globals_height) > real(720 / 480);
}

long __cdecl rasterizer_lag_timing_get_gamestate_delay()
{
	return INVOKE(0x00A25CF0, rasterizer_lag_timing_get_gamestate_delay);
}

void __cdecl rasterizer_lag_timing_mark_input_adjustment(__int64 a1)
{
	INVOKE(0x00A25D00, rasterizer_lag_timing_mark_input_adjustment, a1);
}

void __cdecl rasterizer_lag_timing_mark_render_end()
{
	INVOKE(0x00A25D20, rasterizer_lag_timing_mark_render_end);
}

void __cdecl rasterizer_lag_timing_mark_render_present(__int64 a1)
{
	INVOKE(0x00A25D40, rasterizer_lag_timing_mark_render_present, a1);
}

void __cdecl rasterizer_lag_timing_mark_render_start()
{
	INVOKE(0x00A25DB0, rasterizer_lag_timing_mark_render_start);
}

