#pragma once

#include "cseries/cseries.hpp"

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

	k_game_time_pause_reason_count,
};

enum e_game_tick_publishing_flags
{
	_game_tick_publishing_unknown_bit0 = 0,
	_game_tick_publishing_unknown_bit1,
	_game_tick_publishing_unknown_bit2,
	_game_tick_publishing_unknown_bit3,
	_game_tick_publishing_unknown_bit4,
	_game_tick_publishing_unknown_bit5,
	_game_tick_publishing_unknown_bit6,
	_game_tick_publishing_unknown_bit7,
	_game_tick_publishing_unknown_bit8,
	_game_tick_publishing_unknown_bit9,
	_game_tick_publishing_unknown_bit10,
	_game_tick_publishing_unknown_bit11,

	k_game_tick_publishing_flags
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
		shell_seconds_elapsed = 0.0f;
		world_seconds_elapsed = 0.0f;
		game_seconds_elapsed = 0.0f;
		game_ticks_elapsed = 0;
	}

	void accum(s_game_tick_time_samples const* samples)
	{
		flags = samples->flags;
		shell_seconds_elapsed += samples->shell_seconds_elapsed;
		world_seconds_elapsed += samples->world_seconds_elapsed;
		game_seconds_elapsed += samples->game_seconds_elapsed;
		game_ticks_elapsed = samples->game_ticks_elapsed;
	}

	c_flags<e_game_tick_publishing_flags, dword, k_game_tick_publishing_flags> flags;
	real shell_seconds_elapsed;
	real world_seconds_elapsed;
	real game_seconds_elapsed;
	dword game_ticks_elapsed;
};
static_assert(sizeof(s_game_tick_time_samples) == 0x14);

struct game_time_globals_definition
{
	bool initialized;
	byte : 8; // halo 3: bool paused
	c_flags<e_game_time_pause_reason, word, k_game_time_pause_reason_count> flags;
	short tick_rate;
	word : 16;
	real seconds_per_tick;
	long elapsed_ticks;
	real speed;
	real __unknown14;
	s_game_tick_time_samples time_samples;
};
static_assert(sizeof(game_time_globals_definition) == 0x2C);

extern real debug_game_speed;

extern game_time_globals_definition* game_time_globals_get();

extern long __cdecl game_seconds_integer_to_ticks(long seconds); // 0x00564B40
extern real __cdecl game_seconds_to_ticks_real(real seconds); // 0x00564B70
extern long __cdecl game_seconds_to_ticks_round(real seconds); // 0x00564BB0
extern real __cdecl game_tick_length(); // 0x00564C20
extern long __cdecl game_tick_rate(); // 0x00564C40
extern real __cdecl game_ticks_to_seconds(real ticks); // 0x00564C60
extern void __cdecl game_time_advance(); // 0x00564C90
extern void __cdecl game_time_discard(long desired_ticks, long actual_ticks, real* elapsed_game_dt); // 0x00564CB0
extern void __cdecl game_time_dispose(); // 0x00564D10
extern void __cdecl game_time_dispose_from_old_map(); // 0x00564D20
extern long __cdecl game_time_get(); // 0x00564D50
extern bool __cdecl game_time_get_paused(); // 0x00564D70
extern bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason); // 0x00564E20
extern real __cdecl game_time_get_safe_in_seconds(); // 0x00564E60
extern real __cdecl game_time_get_speed(); // 0x00564EB0
extern void __cdecl game_time_initialize(); // 0x00564ED0
extern void __cdecl game_time_initialize_for_new_map(); // 0x00564F30
extern bool __cdecl game_time_initialized(); // 0x00564FA0
extern void __cdecl game_time_set(long time); // 0x00564FE0
extern void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason); // 0x00565000
extern void __cdecl game_time_set_rate_scale(real rate_scale0, real rate_scale1, real rate_scale2); // 0x00565060
extern void __cdecl game_time_set_rate_scale_direct(real rate_scale); // 0x00565110
extern void __cdecl game_time_set_speed(real speed); // 0x005651D0
extern bool __cdecl game_time_update(real world_seconds_elapsed, real* game_seconds_elapsed, long* game_ticks_elapsed); // 0x00565250
extern void __cdecl game_time_update_paused_flags(); // 0x00565510