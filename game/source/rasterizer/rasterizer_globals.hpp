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

	int32 frame_delta_index;
	int32 frame_deltas[16];

	int32 presentation_interval;

	uint8 __data[0x18];
};
static_assert(sizeof(s_rasterizer_globals) == 0x78);

struct s_rasterizer_timing_data :
	s_synchronized_list_entry
{
	int64 input_collection_time;
	int64 input_adjustment_time;
	int64 publishing_start_time;
	int64 publishing_end_time;
	int64 render_start_time;
	int64 render_end_time;
	int64 present_start_time;
	int64 present_end_time;
	int64 target_vblank_index;
	int64 throttle_vblank_overrun;
	int64 swap_vblank_overrun;
};
static_assert(sizeof(s_rasterizer_timing_data) == 0x60);

struct s_rasterizer_timing_globals
{
	s_rasterizer_timing_data published_time_set;
	t_event_queue<s_rasterizer_timing_data, 45> published_time_set_queue;
	//t_fifo_event_queue<s_rasterizer_timing_data, 4> pending_time_set_queue;
	uint8 __data[0x1A0];
	s_rasterizer_timing_data next_time_set;
};
static_assert(sizeof(s_rasterizer_timing_globals) == 0x1360);

struct s_frame_rate_data
{
	int32 starting_film_tick;
	int32 min;
	int32 max;
	uint32 measurement_sum;
	uint32 measurement_count;
};
static_assert(sizeof(s_frame_rate_data) == 0x14);

extern s_frame_rate_data g_frame_rate_data;

extern real32& global_z_near;
extern real32& global_z_far;
extern s_rasterizer_globals& rasterizer_globals;
extern s_rasterizer_timing_globals& g_rasterizer_timing_globals;

extern void __cdecl rasterizer_get_z_planes(real32* z_near, real32* z_far);
extern void __cdecl rasterizer_globals_initialize();
extern int32 __cdecl rasterizer_lag_timing_get_gamestate_delay();
extern void __cdecl rasterizer_lag_timing_mark_input_adjustment(int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_end();
extern void __cdecl rasterizer_lag_timing_mark_render_present(int64 a1);
extern void __cdecl rasterizer_lag_timing_mark_render_start();
extern void __cdecl rasterizer_set_presentation_interval(int32 presentation_interval);

