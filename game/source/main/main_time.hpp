#pragma once

#include "cseries/cseries.hpp"

struct s_main_time_globals
{
	bool temporary_throttle_control;
	bool temporary_throttle;
	int32 last_game_time;
	int64 last_milliseconds;
	int64 last_vblank_index;
	int64 last_initial_vblank_index;
	int64 target_display_vblank_index;
	int64 last_input_timestamp;
	int64 last_publish_start_timestamp;
	int64 last_publish_end_timestamp;
};
COMPILE_ASSERT(sizeof(s_main_time_globals) == 0x40);

extern bool& display_framerate;
extern bool debug_disable_frame_rate_throttle;
extern bool debug_frame_rate_based_on_system_time;
extern bool debug_frame_rate_stabilization;
extern bool display_frame_deltas;

extern void __cdecl __tls_set_g_main_time_globals_allocator(void* address);
extern void __cdecl main_time_frame_rate_debug();
extern void __cdecl main_time_frame_rate_display();
extern int64 __cdecl main_time_get_absolute_milliseconds();
extern int64 __cdecl main_time_get_input_collection_time();
extern int32 __cdecl main_time_get_native_tick_rate();
extern int64 __cdecl main_time_get_publishing_end_time();
extern int64 __cdecl main_time_get_publishing_start_time();
extern int64 __cdecl main_time_get_target_display_vblank_index();
extern int32 __cdecl main_time_get_time_since_input_collection(int64 timestamp);
extern int32 __cdecl main_time_get_vblank_rate();
extern void __cdecl main_time_globals_post_copy_update(void* new_address);
extern void __cdecl main_time_initialize();
extern bool __cdecl main_time_is_throttled();
extern void __cdecl main_time_mark_input_collection_time();
extern void __cdecl main_time_mark_publishing_end_time();
extern void __cdecl main_time_mark_publishing_start_time();
extern void __cdecl main_time_reset();
extern void __cdecl main_time_restore(int32 game_state_flags);
extern void __cdecl main_time_set_temporary_throttle(bool throttle);
extern void __cdecl main_time_throttle(int64 target_display_vblank_index);
extern real32 __cdecl main_time_update();
extern void __cdecl main_time_update_framerate_datamining();

