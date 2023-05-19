#include "game/game_time.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "memory/thread_local.hpp"

game_time_globals_definition* game_time_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(game_time_globals);
	return game_time_globals;
}

long __cdecl game_seconds_integer_to_ticks(long seconds)
{
	return INVOKE(0x00564B40, game_seconds_integer_to_ticks, seconds);
}
real __cdecl game_seconds_to_ticks_real(real seconds)
{
	return INVOKE(0x00564B70, game_seconds_to_ticks_real, seconds);
}

long __cdecl game_seconds_to_ticks_round(real seconds)
{
	real tick_rate = game_time_globals_get()->tick_rate * seconds;
	if (tick_rate < 0.0)
		return static_cast<long>((-1.0 * 0.5) + tick_rate);
	else
		return static_cast<long>((1.0 * 0.5) + tick_rate);

	return INVOKE(0x00564BB0, game_seconds_to_ticks_round, seconds);
}

real __cdecl game_tick_length()
{
	return INVOKE(0x00564C20, game_tick_length);
}

long __cdecl game_tick_rate()
{
	return INVOKE(0x00564C40, game_tick_rate);
}

real __cdecl game_ticks_to_seconds(real ticks)
{
	return INVOKE(0x00564C60, game_ticks_to_seconds, ticks);
}

void __cdecl game_time_advance()
{
	INVOKE(0x00564C90, game_time_advance);
}

void __cdecl game_time_discard(long desired_ticks, long actual_ticks, real* elapsed_game_dt)
{
	INVOKE(0x00564CB0, game_time_discard, desired_ticks, actual_ticks, elapsed_game_dt);
}

void __cdecl game_time_dispose()
{
	INVOKE(0x00564D10, game_time_dispose);
}

void __cdecl game_time_dispose_from_old_map()
{
	INVOKE(0x00564D20, game_time_dispose_from_old_map);
}

long __cdecl game_time_get()
{
	return INVOKE(0x00564D50, game_time_get);
}

bool __cdecl game_time_get_paused()
{
	return INVOKE(0x00564D70, game_time_get_paused);
}

bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason)
{
	return INVOKE(0x00564E20, game_time_get_paused_for_reason, reason);
}

real __cdecl game_time_get_safe_in_seconds()
{
	return INVOKE(0x00564E60, game_time_get_safe_in_seconds);
}

real __cdecl game_time_get_speed()
{
	return INVOKE(0x00564EB0, game_time_get_speed);
}

void __cdecl game_time_initialize()
{
	INVOKE(0x00564ED0, game_time_initialize);
}

void __cdecl game_time_initialize_for_new_map()
{
	INVOKE(0x00564F30, game_time_initialize_for_new_map);
}

bool __cdecl game_time_initialized()
{
	return INVOKE(0x00564FA0, game_time_initialized);
}

void __cdecl game_time_set(long time)
{
	INVOKE(0x00564FE0, game_time_set, time);
}

void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason)
{
	INVOKE(0x00565000, game_time_set_paused, enable, reason);
}

void __cdecl game_time_set_rate_scale(real rate_scale0, real rate_scale1, real rate_scale2)
{
	INVOKE(0x00565060, game_time_set_rate_scale, rate_scale0, rate_scale1, rate_scale2);
}

void __cdecl game_time_set_rate_scale_direct(real rate_scale)
{
	INVOKE(0x00565110, game_time_set_rate_scale_direct, rate_scale);
}

void __cdecl game_time_set_speed(real speed)
{
	INVOKE(0x005651D0, game_time_set_speed, speed);
}

void __cdecl game_time_update(real world_seconds_elapsed, real* game_seconds_elapsed, long* tick_count)
{
	INVOKE(0x00565250, game_time_update, world_seconds_elapsed, game_seconds_elapsed, tick_count);
}

void __cdecl game_time_update_paused_flags()
{
	INVOKE(0x00565510, game_time_update_paused_flags);
}
