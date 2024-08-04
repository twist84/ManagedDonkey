#include "rasterizer/rasterizer_globals.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "shell/shell_windows.hpp"

#include <math.h>
#include <windows.h>

REFERENCE_DECLARE(0x050DDA00, s_rasterizer_globals, rasterizer_globals);
REFERENCE_DECLARE(0x050DDA80, s_rasterizer_timing_globals, g_rasterizer_timing_globals);

HOOK_DECLARE(0x00A1FC90, rasterizer_get_is_widescreen);

bool __cdecl rasterizer_get_is_widescreen()
{
	//return INVOKE(0x00A1FC90, rasterizer_get_is_widescreen);
	//return ((real)c_rasterizer::render_globals.width / (real)c_rasterizer::render_globals.height) > 1.5f;

	RECT client_rect{};

	HWND window_handle = g_windows_params.window_handle;
	if (window_handle != NULL || (window_handle = g_windows_params.created_window_handle) != NULL)
	{
		GetClientRect(window_handle, &client_rect);
	}
	else
	{
		client_rect.right = c_rasterizer::render_globals.resolution_width;
		client_rect.bottom = c_rasterizer::render_globals.resolution_height;
	}

	if (client_rect.right <= 8)
		client_rect.right = 8;

	if (client_rect.bottom <= 8)
		client_rect.bottom = 8;

	if (render_debug_force_4x3_aspect_ratio)
	{
		if (fabsf(real((real)client_rect.right / (real)client_rect.bottom) - 1.3333334f /* 4/3 */) > _real_epsilon)
			return false;
	}

	return true;
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

