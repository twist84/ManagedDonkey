#include "game/game_time.hpp"

#include "memory/thread_local.hpp"

game_time_globals_definition* game_time_globals_get()
{
	s_thread_local_storage* tls = get_tls();
	if (!tls || !tls->game_time_globals)
		return nullptr;

	return tls->game_time_globals;
}

long __cdecl game_seconds_integer_to_ticks(long seconds)
{
	return reinterpret_cast<decltype(game_seconds_integer_to_ticks)*>(0x00564B40)(seconds);
}
real __cdecl game_seconds_to_ticks_real(real seconds)
{
	return reinterpret_cast<decltype(game_seconds_to_ticks_real)*>(0x00564B70)(seconds);
}

long __cdecl game_seconds_to_ticks_round(real seconds)
{
	real tick_rate = game_time_globals_get()->tick_rate * seconds;
	if (tick_rate < 0.0)
		return static_cast<long>((-1.0 * 0.5) + tick_rate);
	else
		return static_cast<long>((1.0 * 0.5) + tick_rate);

	return reinterpret_cast<decltype(game_seconds_to_ticks_round)*>(0x00564BB0)(seconds);
}

real __cdecl game_tick_length()
{
	return reinterpret_cast<decltype(game_tick_length)*>(0x00564C20)();
}

long __cdecl game_tick_rate()
{
	return reinterpret_cast<decltype(game_tick_rate)*>(0x00564C40)();
}

real __cdecl game_ticks_to_seconds(real ticks)
{
	return reinterpret_cast<decltype(game_ticks_to_seconds)*>(0x00564C60)(ticks);
}

void __cdecl game_time_advance()
{
	reinterpret_cast<decltype(game_time_advance)*>(0x00564C90)();
}

void __cdecl game_time_discard(long desired_ticks, long actual_ticks, real* elapsed_game_dt)
{
	reinterpret_cast<decltype(game_time_discard)*>(0x00564CB0)(desired_ticks, actual_ticks, elapsed_game_dt);
}

void __cdecl game_time_dispose()
{
	reinterpret_cast<decltype(game_time_dispose)*>(0x00564D10)();
}

void __cdecl game_time_dispose_from_old_map()
{
	reinterpret_cast<decltype(game_time_dispose_from_old_map)*>(0x00564D20)();
}

long __cdecl game_time_get()
{
	return reinterpret_cast<decltype(game_time_get)*>(0x00564D50)();
}

bool __cdecl game_time_get_paused()
{
	return reinterpret_cast<decltype(game_time_get_paused)*>(0x00564D70)();
}

bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason)
{
	return reinterpret_cast<decltype(game_time_get_paused_for_reason)*>(0x00564E20)(reason);
}

real __cdecl game_time_get_safe_in_seconds()
{
	return reinterpret_cast<decltype(game_time_get_safe_in_seconds)*>(0x00564E60)();
}

real __cdecl game_time_get_speed()
{
	return reinterpret_cast<decltype(game_time_get_speed)*>(0x00564EB0)();
}

void __cdecl game_time_initialize()
{
	reinterpret_cast<decltype(game_time_initialize)*>(0x00564ED0)();
}

void __cdecl game_time_initialize_for_new_map()
{
	reinterpret_cast<decltype(game_time_initialize_for_new_map)*>(0x00564F30)();
}

bool __cdecl game_time_initialized()
{
	return reinterpret_cast<decltype(game_time_initialized)*>(0x00564FA0)();
}

void __cdecl game_time_set(long time)
{
	reinterpret_cast<decltype(game_time_set)*>(0x00564FE0)(time);
}

void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason)
{
	reinterpret_cast<decltype(game_time_set_paused)*>(0x00565000)(enable, reason);
}

void __cdecl game_time_set_rate_scale(real rate_scale0, real rate_scale1, real rate_scale2)
{
	reinterpret_cast<decltype(game_time_set_rate_scale)*>(0x00565060)(rate_scale0, rate_scale1, rate_scale2);
}

void __cdecl game_time_set_speed(real speed)
{
	reinterpret_cast<decltype(game_time_set_speed)*>(0x00565110)(speed);
}

void __cdecl game_time_update(real world_seconds_elapsed, real* game_seconds_elapsed, long* tick_count) // 
{
	reinterpret_cast<decltype(game_time_update)*>(0x00565250)(world_seconds_elapsed, game_seconds_elapsed, tick_count);
}