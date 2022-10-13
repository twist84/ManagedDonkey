#include "game/game_time.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/thread_local.hpp"

game_time_globals_definition* game_time_globals_get()
{
	FUNCTION_BEGIN(true);

	s_thread_local_storage* tls = get_tls();
	if (!tls || !tls->game_time_globals)
		return nullptr;

	return tls->game_time_globals;
}

long __cdecl game_seconds_integer_to_ticks(long seconds)
{
	FUNCTION_BEGIN(false);

	return INVOKE(0x00564B40, game_seconds_integer_to_ticks, seconds);
}
real __cdecl game_seconds_to_ticks_real(real seconds)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564B70, game_seconds_to_ticks_real, seconds);
}

long __cdecl game_seconds_to_ticks_round(real seconds)
{
	FUNCTION_BEGIN(true);

	real tick_rate = game_time_globals_get()->tick_rate * seconds;
	if (tick_rate < 0.0)
		return static_cast<long>((-1.0 * 0.5) + tick_rate);
	else
		return static_cast<long>((1.0 * 0.5) + tick_rate);

	return INVOKE(0x00564BB0, game_seconds_to_ticks_round, seconds);
}

real __cdecl game_tick_length()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564C20, game_tick_length);
}

long __cdecl game_tick_rate()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564C40, game_tick_rate);
}

real __cdecl game_ticks_to_seconds(real ticks)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564C60, game_ticks_to_seconds, ticks);
}

void __cdecl game_time_advance()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564C90, game_time_advance);
}

void __cdecl game_time_discard(long desired_ticks, long actual_ticks, real* elapsed_game_dt)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564CB0, game_time_discard, desired_ticks, actual_ticks, elapsed_game_dt);
}

void __cdecl game_time_dispose()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564D10, game_time_dispose);
}

void __cdecl game_time_dispose_from_old_map()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564D20, game_time_dispose_from_old_map);
}

long __cdecl game_time_get()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564D50, game_time_get);
}

bool __cdecl game_time_get_paused()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564D70, game_time_get_paused);
}

bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564E20, game_time_get_paused_for_reason, reason);
}

real __cdecl game_time_get_safe_in_seconds()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564E60, game_time_get_safe_in_seconds);
}

real __cdecl game_time_get_speed()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564EB0, game_time_get_speed);
}

void __cdecl game_time_initialize()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564ED0, game_time_initialize);
}

void __cdecl game_time_initialize_for_new_map()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564F30, game_time_initialize_for_new_map);
}

bool __cdecl game_time_initialized()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00564FA0, game_time_initialized);
}

void __cdecl game_time_set(long time)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00564FE0, game_time_set, time);
}

void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00565000, game_time_set_paused, enable, reason);
}

void __cdecl game_time_set_rate_scale(real rate_scale0, real rate_scale1, real rate_scale2)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00565060, game_time_set_rate_scale, rate_scale0, rate_scale1, rate_scale2);
}

void __cdecl game_time_set_speed(real speed)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00565110, game_time_set_speed, speed);
}

void __cdecl game_time_update(real world_seconds_elapsed, real* game_seconds_elapsed, long* tick_count)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00565250, game_time_update, world_seconds_elapsed, game_seconds_elapsed, tick_count);
}

void __cdecl game_time_update_paused_flags()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00565510, game_time_update_paused_flags);
}
