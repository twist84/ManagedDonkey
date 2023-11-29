#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

struct s_rasterizer_globals
{
	c_synchronized_int64 vblank_index;

	c_synchronized_int64 __unknown8;

	c_synchronized_long most_recent_delta;
	c_synchronized_long most_recent_swap_index;

	long frame_delta_index;
	long frame_deltas[16];

	long presentation_interval;

	byte __data[0x18];
};
static_assert(sizeof(s_rasterizer_globals) == 0x78);

extern s_rasterizer_globals& rasterizer_globals;

extern bool __cdecl rasterizer_get_is_widescreen();
extern long __cdecl rasterizer_lag_timing_get_gamestate_delay();
extern void __cdecl rasterizer_lag_timing_mark_input_adjustment(__int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_end();
extern void __cdecl rasterizer_lag_timing_mark_render_present(__int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_start();

