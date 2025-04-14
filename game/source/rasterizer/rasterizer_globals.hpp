#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/event_queue.hpp"

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

struct s_rasterizer_timing_data :
	s_synchronized_list_entry
{
	__int64 input_collection_time;
	__int64 input_adjustment_time;
	__int64 publishing_start_time;
	__int64 publishing_end_time;
	__int64 render_start_time;
	__int64 render_end_time;
	__int64 present_start_time;
	__int64 present_end_time;
	__int64 target_vblank_index;
	__int64 throttle_vblank_overrun;
	__int64 swap_vblank_overrun;
};
static_assert(sizeof(s_rasterizer_timing_data) == 0x60);

struct s_rasterizer_timing_globals
{
	s_rasterizer_timing_data published_time_set;
	t_event_queue<s_rasterizer_timing_data, 45> published_time_set_queue;
	//t_fifo_event_queue<s_rasterizer_timing_data, 4> pending_time_set_queue;
	byte __data[0x1A0];
	s_rasterizer_timing_data next_time_set;
};
static_assert(sizeof(s_rasterizer_timing_globals) == 0x1360);

extern s_rasterizer_globals& rasterizer_globals;
extern s_rasterizer_timing_globals& g_rasterizer_timing_globals;

extern long __cdecl rasterizer_lag_timing_get_gamestate_delay();
extern void __cdecl rasterizer_lag_timing_mark_input_adjustment(__int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_end();
extern void __cdecl rasterizer_lag_timing_mark_render_present(__int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_start();
extern void __cdecl rasterizer_set_presentation_interval(long presentation_interval);

