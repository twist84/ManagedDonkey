#include "rasterizer/rasterizer_globals.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "shell/shell_windows.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x050DDA00, s_rasterizer_globals, rasterizer_globals);
REFERENCE_DECLARE(0x050DDA80, s_rasterizer_timing_globals, g_rasterizer_timing_globals);

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

void __cdecl rasterizer_set_presentation_interval(long presentation_interval)
{
	INVOKE(0x00A25E60, rasterizer_set_presentation_interval, presentation_interval);

	//rasterizer_globals.presentation_interval = presentation_interval;
}

