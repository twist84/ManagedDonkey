#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

// mapped using halo 3/odst/halo reach
// only `_game_time_pause_reason_debug` works in multiplayer
enum e_game_time_pause_reason
{
	// set/unset at `game_time_update` begin/end
	_game_time_pause_reason_unknown0 = 0,

	// game_time_update
	// - debug_pause_game != debug_pause_game_active
	_game_time_pause_reason_debug,

	// halo 3: c_start_menu_game_campaign::initialize
	// odst: c_start_menu_screen_widget::update
	// halo reach: c_start_menu_pause_component::update_pause
	// game_state_call_after_load_procs
	//	- game_state_set_revert_time
	_game_time_pause_reason_ui,

	// game_time_update
	// - controllers
	_game_time_pause_reason_controller0,
	_game_time_pause_reason_controller1,
	_game_time_pause_reason_controller2,
	_game_time_pause_reason_controller3,

	// user_interface_xbox_guide_is_active
	_game_time_pause_reason_xbox_guide,

	// metagame: load postgame carnage report
	_game_time_pause_reason_postgame,

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

	void accum(s_game_tick_time_samples const* samples)
	{
		flags = samples->flags;
		shell_dt += samples->shell_dt;
		world_dt += samples->world_dt;
		game_dt += samples->game_dt;
		elapsed_game_ticks = samples->elapsed_game_ticks;
	}

	c_flags<e_game_tick_publishing_flags, dword, k_game_tick_publishing_flag_count> flags;
	real shell_dt;
	real world_dt;
	real game_dt;
	long elapsed_game_ticks;
};
static_assert(sizeof(s_game_tick_time_samples) == 0x14);

struct game_time_globals_definition
{
	bool initialized;

	// halo 3: bool paused
	byte : 8; // alignment

	c_flags<e_game_time_pause_reason, word, k_game_time_pause_reason_count> flags;
	short tick_rate;

	word : 16; // alignment

	real tick_length;
	long elapsed_ticks;
	real speed;
	real ticks_leftover;

	real __unknown18;
	real shell_seconds_elapsed;
	real world_seconds_elapsed;
	real game_seconds_elapsed;
	dword game_ticks_elapsed;
};
static_assert(sizeof(game_time_globals_definition) == 0x2C);

extern real debug_game_speed;
extern bool debug_game_time_statistics;
extern bool debug_game_time_lock;
extern bool debug_pause_game_active;
extern bool debug_pause_game;
extern e_game_time_pause_reason const k_controller_pause_reasons[k_number_of_controllers];

extern void __cdecl __tls_set_g_game_time_globals_allocator(void* address);
extern long __cdecl game_seconds_integer_to_ticks(long seconds);
extern real __cdecl game_seconds_to_ticks_real(real seconds);
extern long __cdecl game_seconds_to_ticks_round(real seconds);
extern real __cdecl game_tick_length();
extern long __cdecl game_tick_rate();
extern real __cdecl game_ticks_to_seconds(real ticks);
extern void __cdecl game_time_advance();
extern void __cdecl game_time_discard(long desired_ticks, long actual_ticks, real* elapsed_game_dt);
extern void __cdecl game_time_dispose();
extern void __cdecl game_time_dispose_from_old_map();
extern long __cdecl game_time_get();
extern bool __cdecl game_time_get_paused();
extern bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason);
extern real __cdecl game_time_get_safe_in_seconds();
extern real __cdecl game_time_get_speed();
extern void __cdecl game_time_initialize();
extern void __cdecl game_time_initialize_for_new_map();
extern bool __cdecl game_time_initialized();
extern void __cdecl game_time_set(long time);
extern void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason);
extern void __cdecl game_time_set_rate_scale(real rate_scale0, real rate_scale1, real rate_scale2);
extern void __cdecl game_time_set_rate_scale_direct(real rate_scale);
extern void __cdecl game_time_set_speed(real speed);
extern bool __cdecl game_time_update(real world_seconds_elapsed, real* game_seconds_elapsed, long* game_ticks_elapsed);
extern void __cdecl game_time_update_paused_flags();

extern void __cdecl game_time_statistics_start();
extern void __cdecl game_time_statistics_frame(
	real world_seconds_elapsed,
	real game_seconds_elapsed,
	real real_desired_ticks,
	long game_ticks_target,
	long game_ticks_limit,
	long game_ticks_available,
	long game_ticks_elapsed,
	real game_ticks_leftover,
	bool discontinuity);
extern void __cdecl game_time_statistics_stop();

