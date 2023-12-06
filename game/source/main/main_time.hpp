#pragma once

#include "cseries/cseries.hpp"

struct s_main_time_globals
{
	// main_time_set_temporary_throttle(bool temporary_throttle)
	// __unknown0 = true;
	// __unknown1 = temporary_throttle;
	bool __unknown0;
	bool __unknown1;

	dword elapsed_ticks;
	__int64 last_milliseconds;
	__int64 absolute;
	__int64 __unknown18;
	__int64 target_display_vblank_index;
	__int64 input_collection_time;
	__int64 publishing_start_time;
	__int64 publishing_end_time;
};
static_assert(sizeof(s_main_time_globals) == 0x40);

extern bool& display_framerate;
extern bool debug_disable_frame_rate_throttle;
extern bool display_frame_deltas;

extern void __cdecl main_time_frame_rate_display();
extern __int64 __cdecl main_time_get_absolute_milliseconds();
extern __int64 __cdecl main_time_get_input_collection_time();
extern long __cdecl main_time_get_native_tick_rate();
extern __int64 __cdecl main_time_get_publishing_end_time();
extern __int64 __cdecl main_time_get_publishing_start_time();
extern __int64 __cdecl main_time_get_target_display_vblank_index();
extern long __cdecl main_time_get_time_since_input_collection(__int64 a1);
extern long __cdecl main_time_get_vblank_rate();
extern void __cdecl main_time_globals_post_copy_update(void* userdata);
extern void __cdecl main_time_initialize();
extern bool __cdecl main_time_is_throttled();
extern void __cdecl main_time_mark_input_collection_time();
extern void __cdecl main_time_mark_publishing_end_time();
extern void __cdecl main_time_mark_publishing_start_time();
extern void __cdecl main_time_reset();
extern void __cdecl main_time_restore(long a1);
extern void __cdecl main_time_set_temporary_throttle(bool temporary_throttle);
extern void __cdecl main_time_throttle(__int64 a1);
extern real __cdecl main_time_update();

