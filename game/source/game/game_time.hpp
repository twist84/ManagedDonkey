#pragma once

#include "cseries/cseries.hpp"

// mapped using halo 3/odst/halo reach
// only `_game_time_pause_reason_debug` works in multiplayer
enum e_game_time_pause_reason
{
	_game_time_pause_recursion_lock_internal = 0,
	_game_time_pause_debug,
	//_game_time_pause_debug_menu,
	_game_time_pause_ui,
	_game_time_pause_controller0_removal,
	_game_time_pause_controller1_removal,
	_game_time_pause_controller2_removal,
	_game_time_pause_controller3_removal,
	_game_time_pause_xbox_guide_ui,
	_game_time_pause_postgame,

	k_game_time_pause_reason_count
};

enum e_game_tick_publishing_flags
{
	_game_published_new_game_tick = 0,
	_game_published_shell_paused,
	_game_published_game_time_unchanged,
	_game_published_game_time_paused,
	_game_published_game_paused,
	_game_published_pregame,
	_game_published_main_time_halted,
	_game_published_game_speed_slowed,
	_game_published_framerate_infinite,
	_game_published_ui_request,
	_game_published_network_playback_client,
	_game_published_maintain_minimal_framerate,

	k_game_tick_publishing_flag_count
};

struct s_game_tick_time_samples
{
	void initialize()
	{
		reset();
	}

	void reset()
	{
		flags.clear();
		shell_dt = 0.0f;
		world_dt = 0.0f;
		game_dt = 0.0f;
		elapsed_game_ticks = 0;
	}

	void accum(const s_game_tick_time_samples* samples)
	{
		flags = samples->flags;
		shell_dt += samples->shell_dt;
		world_dt += samples->world_dt;
		game_dt += samples->game_dt;
		elapsed_game_ticks = samples->elapsed_game_ticks;
	}

	c_flags<e_game_tick_publishing_flags, uns32, k_game_tick_publishing_flag_count> flags;
	real32 shell_dt;
	real32 world_dt;
	real32 game_dt;
	int32 elapsed_game_ticks;
};
COMPILE_ASSERT(sizeof(s_game_tick_time_samples) == 0x14);

struct game_time_globals_definition
{
	bool initialized;
	c_flags<e_game_time_pause_reason, uns16, k_game_time_pause_reason_count> flags;
	int16 tick_rate;
	real32 tick_length;
	int32 time;
	real32 speed;
	real32 leftover_ticks;
	real32 rate_scale_timer;
	real32 rate_scale_duration;
	real32 rate_scale_initial;
	real32 rate_scale_final;
	int32 game_message_tick;
};
COMPILE_ASSERT(sizeof(game_time_globals_definition) == 0x2C);

extern real32 debug_game_speed;
extern bool debug_game_time_statistics;
extern bool debug_game_time_lock;
extern bool debug_pause_game_active;
extern bool debug_pause_game;
extern e_game_time_pause_reason const k_controller_pause_reasons[k_number_of_controllers];

extern void __cdecl __tls_set_g_game_time_globals_allocator(void* address);
extern int32 __cdecl game_seconds_integer_to_ticks(int32 seconds);
extern real32 __cdecl game_seconds_to_ticks_real(real32 seconds);
extern int32 __cdecl game_seconds_to_ticks_round(real32 seconds);
extern real32 __cdecl game_tick_length();
extern int32 __cdecl game_tick_rate();
extern real32 __cdecl game_ticks_to_seconds(real32 ticks);
extern void __cdecl game_time_advance();
extern void __cdecl game_time_discard(int32 desired_ticks, int32 actual_ticks, real32* elapsed_game_dt);
extern void __cdecl game_time_dispose();
extern void __cdecl game_time_dispose_from_old_map();
extern int32 __cdecl game_time_get();
extern bool __cdecl game_time_get_paused();
extern bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason);
extern real32 __cdecl game_time_get_safe_in_seconds();
extern real32 __cdecl game_time_get_speed();
extern void __cdecl game_time_initialize();
extern void __cdecl game_time_initialize_for_new_map();
extern bool __cdecl game_time_initialized();
extern void __cdecl game_time_render_debug();
extern void __cdecl game_time_set(int32 time);
extern void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason);
extern void __cdecl game_time_set_rate_scale(real32 rate_scale0, real32 rate_scale1, real32 rate_scale2);
extern void __cdecl game_time_set_rate_scale_direct(real32 rate_scale);
extern void __cdecl game_time_set_speed(real32 speed);
extern bool __cdecl game_time_update(real32 world_seconds_elapsed, real32* game_seconds_elapsed, int32* game_ticks_elapsed);
extern void __cdecl game_time_update_paused_flags();

extern void __cdecl game_time_statistics_start();
extern void __cdecl game_time_statistics_frame(
	real32 world_seconds_elapsed,
	real32 game_seconds_elapsed,
	real32 real_desired_ticks,
	int32 game_ticks_target,
	int32 game_ticks_limit,
	int32 game_ticks_available,
	int32 game_ticks_elapsed,
	real32 game_ticks_leftover,
	bool discontinuity);
extern void __cdecl game_time_statistics_stop();

