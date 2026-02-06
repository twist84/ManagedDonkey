#include "game/game_time.hpp"

#include "cutscene/cinematics.hpp"
#include "input/controllers.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"

#include <math.h>

HOOK_DECLARE(0x00564B40, game_seconds_integer_to_ticks);
HOOK_DECLARE(0x00564B70, game_seconds_to_ticks_real);
HOOK_DECLARE(0x00564BB0, game_seconds_to_ticks_round);
HOOK_DECLARE(0x00564C20, game_tick_length);
HOOK_DECLARE(0x00564C40, game_tick_rate);
HOOK_DECLARE(0x00564C60, game_ticks_to_seconds);
HOOK_DECLARE(0x00564C90, game_time_advance);
HOOK_DECLARE(0x00564CB0, game_time_discard);
//HOOK_DECLARE(0x00564D10, game_time_dispose);
//HOOK_DECLARE(0x00564D20, game_time_dispose_from_old_map);
HOOK_DECLARE(0x00564D50, game_time_get);
HOOK_DECLARE(0x00564D70, game_time_get_paused);
HOOK_DECLARE(0x00564E20, game_time_get_paused_for_reason);
HOOK_DECLARE(0x00564E60, game_time_get_safe_in_seconds);
HOOK_DECLARE(0x00564EB0, game_time_get_speed);
//HOOK_DECLARE(0x00564ED0, game_time_initialize);
//HOOK_DECLARE(0x00564F30, game_time_initialize_for_new_map);
HOOK_DECLARE(0x00564FA0, game_time_initialized);
HOOK_DECLARE(0x00564FE0, game_time_set);
HOOK_DECLARE(0x00565000, game_time_set_paused);
HOOK_DECLARE(0x00565060, game_time_set_rate_scale);
HOOK_DECLARE(0x00565110, game_time_set_rate_scale_direct);
HOOK_DECLARE(0x005651D0, game_time_set_speed);
HOOK_DECLARE(0x00565250, game_time_update);
HOOK_DECLARE(0x00565510, game_time_update_paused_flags);

real32 debug_game_speed = 1.0f;
bool debug_game_time_statistics = false;
bool debug_game_time_lock = false;
bool debug_pause_game_active = false;
bool debug_pause_game = false;

FILE* game_time_statistics_file = nullptr;
bool game_time_statistics_started;
bool game_time_statistics_update_time;
bool game_time_statistics_write_header;
uns32 game_time_statistics_time;

void __cdecl __tls_set_g_game_time_globals_allocator(void* address)
{
	//INVOKE(0x00564A70, __tls_set_g_game_time_globals_allocator, address);

	game_time_globals = (game_time_globals_definition*)address;
}

int32 __cdecl game_seconds_integer_to_ticks(int32 seconds)
{
	//return INVOKE(0x00564B40, game_seconds_integer_to_ticks, seconds);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return seconds * game_time_globals->tick_rate;
}

real32 __cdecl game_seconds_to_ticks_real(real32 seconds)
{
	//return INVOKE(0x00564B70, game_seconds_to_ticks_real, seconds);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return seconds * game_time_globals->tick_rate;
}

int32 __cdecl game_seconds_to_ticks_round(real32 seconds)
{
	//return INVOKE(0x00564BB0, game_seconds_to_ticks_round, seconds);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	real32 tick_rate = game_time_globals->tick_rate * seconds;
	return int32(tick_rate + ((tick_rate < 0.0f ? -1.0f : 1.0f) / 2));
}

real32 __cdecl game_tick_length()
{
	//return INVOKE(0x00564C20, game_tick_length);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_length;
}

int32 __cdecl game_tick_rate()
{
	//return INVOKE(0x00564C40, game_tick_rate);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_rate;
}

real32 __cdecl game_ticks_to_seconds(real32 ticks)
{
	//return INVOKE(0x00564C60, game_ticks_to_seconds, ticks);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	return game_time_globals->tick_length * ticks;
}

void __cdecl game_time_advance()
{
	//INVOKE(0x00564C90, game_time_advance);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	game_time_globals->time++;
}

void __cdecl game_time_discard(int32 desired_ticks, int32 actual_ticks, real32* elapsed_game_dt)
{
	//INVOKE(0x00564CB0, game_time_discard, desired_ticks, actual_ticks, elapsed_game_dt);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	ASSERT(desired_ticks > 0);
	ASSERT(actual_ticks < desired_ticks);
	ASSERT(elapsed_game_dt);
	if (actual_ticks)
	{
		real32 lost_dt = game_ticks_to_seconds(real32(desired_ticks - actual_ticks));
		ASSERT(*elapsed_game_dt - lost_dt > -_real_epsilon);
		*elapsed_game_dt = MAX(*elapsed_game_dt - lost_dt, 0.0f);
	}
	else
	{
		*elapsed_game_dt = 0.0f;
	}
}

void __cdecl game_time_dispose()
{
	//INVOKE(0x00564D10, game_time_dispose);
}

void __cdecl game_time_dispose_from_old_map()
{
	//INVOKE(0x00564D20, game_time_dispose_from_old_map);

	if (game_time_globals && game_time_globals->initialized)
	{
		game_time_statistics_stop();
		game_time_globals->initialized = false;
	}
}

int32 __cdecl game_time_get()
{
	//return INVOKE(0x00564D50, game_time_get);

	ASSERT(game_time_globals && game_time_globals->initialized);
	return game_time_globals->time;
}

bool __cdecl game_time_get_paused()
{
	//return INVOKE(0x00564D70, game_time_get_paused);

	ASSERT(game_time_globals);
	
	bool result = false;
	if (game_time_globals->initialized)
	{
		c_flags<e_game_time_pause_reason, uns16, k_game_time_pause_reason_count>& pause_flags = game_time_globals->flags;
	
		bool v1 = game_is_campaign() && !(game_is_playback() && !game_is_authoritative_playback());
		result = TEST_FLAG(pause_flags, _game_time_pause_debug);
		if (TEST_FLAG(pause_flags, _game_time_pause_ui) ||
			TEST_FLAG(pause_flags, _game_time_pause_recursion_lock_internal))
			result |= v1;
	
		if (TEST_FLAG(pause_flags, _game_time_pause_controller0_removal) ||
			TEST_FLAG(pause_flags, _game_time_pause_controller1_removal) ||
			TEST_FLAG(pause_flags, _game_time_pause_controller2_removal) ||
			TEST_FLAG(pause_flags, _game_time_pause_controller3_removal))
			result |= v1;
	
		if (TEST_FLAG(pause_flags, _game_time_pause_xbox_guide_ui))
			result |= v1;
	}
	return result;
}

bool __cdecl game_time_get_paused_for_reason(e_game_time_pause_reason reason)
{
	//return INVOKE(0x00564E20, game_time_get_paused_for_reason, reason);

	return game_time_globals->initialized && game_time_globals->flags.test(reason);
}

real32 __cdecl game_time_get_safe_in_seconds()
{
	//return INVOKE(0x00564E60, game_time_get_safe_in_seconds);

	if (game_time_globals && game_time_globals->initialized)
		return game_time_globals->time * game_time_globals->tick_length;
	return 0.0f;
}

real32 __cdecl game_time_get_speed()
{
	//return INVOKE(0x00564EB0, game_time_get_speed);

	return game_time_globals->speed;
}

void __cdecl game_time_initialize()
{
	//INVOKE(0x00564ED0, game_time_initialize);

	game_time_globals = (game_time_globals_definition*)g_game_time_globals_allocator.allocate(sizeof(game_time_globals_definition), "game time globals");
	csmemset(game_time_globals, 0, sizeof(game_time_globals_definition));
}

void __cdecl game_time_initialize_for_new_map()
{
	//INVOKE(0x00564F30, game_time_initialize_for_new_map);

	csmemset(game_time_globals, 0, sizeof(game_time_globals_definition));
	game_time_globals->tick_rate = game_options_get()->game_tick_rate;
	game_time_globals->tick_length = 1.0f / game_time_globals->tick_rate;
	game_time_globals->speed = 1.0f;
	debug_game_speed = game_time_globals->speed;
	game_time_statistics_start();
	game_time_globals->initialized = true;
}

bool __cdecl game_time_initialized()
{
	//return INVOKE(0x00564FA0, game_time_initialized);

	return game_time_globals && game_time_globals->initialized;
}

void __cdecl game_time_render_debug()
{
	//INVOKE(0x00564FD0, game_time_render_debug);
}

void __cdecl game_time_set(int32 game_time)
{
	//INVOKE(0x00564FE0, game_time_set, game_time);

	ASSERT(game_time_globals && game_time_globals->initialized);
	ASSERT(game_is_networked() && !simulation_in_progress());
	ASSERT(game_time >= 0);
	game_time_globals->time = game_time;
}

void __cdecl game_time_set_paused(bool enable, e_game_time_pause_reason reason)
{
	//INVOKE(0x00565000, game_time_set_paused, enable, reason);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	game_time_globals->flags.set(reason, enable);
	game_time_update_paused_flags();
	cinematic_game_pause_handler(game_time_get_paused());
	sound_game_pause_handler(game_time_get_paused());
}

void __cdecl game_time_set_rate_scale(real32 world_seconds_elapsed, real32 game_seconds_elapsed, real32 shell_seconds_elapsed)
{
	//INVOKE(0x00565060, game_time_set_rate_scale, world_seconds_elapsed, game_seconds_elapsed, shell_seconds_elapsed);

	world_seconds_elapsed = CLAMP(world_seconds_elapsed, 0.2f, 5.0f);
	game_seconds_elapsed = CLAMP(game_seconds_elapsed, 0.2f, 5.0f);
	if (shell_seconds_elapsed > 0.0f)
	{
		game_time_globals->rate_scale_timer = 0.0f;
		game_time_globals->rate_scale_duration = shell_seconds_elapsed;
		game_time_globals->rate_scale_initial = world_seconds_elapsed;
		game_time_globals->rate_scale_final = game_seconds_elapsed;
	}
	game_time_set_rate_scale_direct(shell_seconds_elapsed > 0.0f ? world_seconds_elapsed : game_seconds_elapsed);
}

void __cdecl game_time_set_rate_scale_direct(real32 rate_scale)
{
	//INVOKE(0x00565110, game_time_set_rate_scale_direct, rate_scale);

	real32 game_tick_rate = game_options_get()->game_tick_rate;
	real32 tick_rate = game_tick_rate / MAX(rate_scale, 0.01f);
	tick_rate += ((tick_rate < 0.0f ? -1.0f : 1.0f) / 2);
	game_time_globals->tick_rate = int16(tick_rate);
	game_time_globals->tick_length = 1.0f / tick_rate;
	game_time_globals->speed = game_tick_rate / tick_rate;
	debug_game_speed = game_time_globals->speed;
}

void __cdecl game_time_set_speed(real32 speed)
{
	//INVOKE(0x005651D0, game_time_set_speed, speed);

	ASSERT(game_time_globals);
	ASSERT(game_time_globals->initialized);
	game_time_globals->speed = speed;
}

void game_time_set_cinematic_rate()
{
	if (!game_in_progress())
		return;

	real32 tick_rate = game_options_get()->game_tick_rate;
	if (game_is_campaign() && cinematic_in_progress())
		tick_rate = 30.0f;
	game_time_globals->tick_rate = int16(tick_rate);
	game_time_globals->tick_length = 1.0f / tick_rate;
}

bool __cdecl game_time_update(real32 world_seconds_elapsed, real32* game_seconds_elapsed, int32* game_ticks_elapsed_)
{
	//return INVOKE(0x00565250, game_time_update, world_seconds_elapsed, game_seconds_elapsed, game_ticks_elapsed_);

	uns32 game_ticks_target = 0;
	bool result = false;
	uns32 game_ticks_limit = 0;
	uns32 game_ticks_elapsed = 0;
	bool discontinuity = false;
	real32 elapsed_game_dt = 0.0f;
	real32 real_desired_ticks = 0.0f;
	real32 game_ticks_leftover = 0.0f;
	
	if (debug_pause_game != debug_pause_game_active)
	{
		game_time_set_paused(debug_pause_game, _game_time_pause_debug);
		debug_pause_game_active = debug_pause_game;
	}
	
	if (debug_game_speed != game_time_get_speed())
	{
		game_time_set_speed(debug_game_speed);
	}
	
	game_time_update_paused_flags();
	
	if (game_is_playback())
	{
		real32 playback_game_speed = saved_film_manager_get_playback_game_speed();
		game_time_set_speed(playback_game_speed);
	}

	game_time_set_cinematic_rate();

	if (game_in_progress())
	{
		if (game_time_globals->rate_scale_duration > 0.0f)
		{
			game_time_globals->rate_scale_timer += world_seconds_elapsed;
			real32 unknown18 = game_time_globals->rate_scale_timer;
			real32 rate_scale_duration = game_time_globals->rate_scale_duration;
			if (rate_scale_duration <= unknown18)
			{
				game_time_set_rate_scale_direct(game_time_globals->rate_scale_final);
				game_time_globals->rate_scale_duration = 0.0f;
			}
			else
			{
				real32 rate_scale = 
					((1.0f - (unknown18 / rate_scale_duration)) * game_time_globals->rate_scale_initial)
					+ ((unknown18 / rate_scale_duration) * game_time_globals->rate_scale_final);
				game_time_set_rate_scale_direct(rate_scale);
			}
		}
	}
	
	if (game_in_progress() && !game_time_get_paused() && game_time_get_speed() > 0.0)
	{
		bool match_remote_time = false;
		bool v36 = cinematic_in_progress() && debug_game_speed == 1.0f;
	
		if (game_is_networked() || game_is_playback())
		{
			game_ticks_limit = simulation_time_get_maximum_available(&match_remote_time);

			int32 seek_time_available = 0;
			if (saved_film_manager_seeking(&seek_time_available) && game_ticks_limit > (uns32)seek_time_available)
			{
				game_ticks_limit = (uns32)seek_time_available;
			}
		}
		else
		{
			game_ticks_limit = 5 * game_time_globals->tick_rate;
		}
	
		if (!match_remote_time && !v36)
		{
			real32 tick_rate = (2.0f * CLAMP(game_time_get_speed(), 1.0f, 5.0f));
			game_ticks_limit = (uns32)MAX(game_ticks_limit, ((tick_rate < 0.0f ? -1.0f : 1.0f) / 2));
		}
	
		if (thread_is_being_traced(k_thread_main))
		{
			game_ticks_limit = MIN(1, game_ticks_limit);
		}
	
		if (debug_game_time_lock /*|| movie_recording()*/)
		{
			real_desired_ticks = 1.0f;
			game_ticks_target = 1;
			elapsed_game_dt = game_time_globals->tick_length;
		}
		else
		{
			elapsed_game_dt = game_time_get_speed() * world_seconds_elapsed;
			real_desired_ticks = (game_time_globals->tick_rate * elapsed_game_dt) + game_time_globals->leftover_ticks;
			game_ticks_target = (int32)real_desired_ticks;
		}
	
		if (match_remote_time)
		{
			if (game_ticks_target > game_ticks_limit)
			{
				game_ticks_elapsed = MAX(0, game_ticks_limit - 1);
				discontinuity = true;
			}
			else if (game_ticks_target + 7 < game_ticks_limit)
			{
				game_ticks_elapsed = MAX(0, game_ticks_limit - 1);
				discontinuity = true;
			}
			else if (game_ticks_target + 1 < game_ticks_limit)
			{
				game_ticks_elapsed = game_ticks_target + 1;
				discontinuity = true;
			}
			else if (game_ticks_target + 1 > game_ticks_limit)
			{
				game_ticks_elapsed = MAX(0, game_ticks_limit - 1);
				discontinuity = true;
			}
			else
			{
				game_ticks_elapsed = game_ticks_target;
			}
		}
		else if (game_ticks_target > game_ticks_limit)
		{
			game_ticks_elapsed = game_ticks_limit;
			discontinuity = true;
		}
		else
		{
			game_ticks_elapsed = game_ticks_target;
		}
		ASSERT(game_ticks_elapsed >= 0 && game_ticks_elapsed <= game_ticks_limit);
	
		if (discontinuity)
		{
			game_ticks_elapsed = game_ticks_limit;
			elapsed_game_dt = game_time_globals->tick_length * game_ticks_limit;
		}
		else
		{
			game_ticks_leftover = real_desired_ticks - (real32)game_ticks_elapsed;
			ASSERT(abs(game_ticks_elapsed - game_ticks_target) <= 1);
			ASSERT(game_ticks_leftover >= -1.0f && game_ticks_leftover <= 2.0f);
		}
	
		result = true;
	}
	
	game_time_statistics_frame(
		world_seconds_elapsed,
		elapsed_game_dt,
		real_desired_ticks,
		game_ticks_target,
		game_ticks_limit,
		0,
		game_ticks_elapsed,
		game_ticks_leftover,
		discontinuity);
	
	game_time_globals->leftover_ticks = game_ticks_leftover;
	
	if (game_seconds_elapsed)
	{
		*game_seconds_elapsed = elapsed_game_dt;
	}
	
	if (game_ticks_elapsed_)
	{
		*game_ticks_elapsed_ = game_ticks_elapsed;
	}
	
	return result;
}

void __cdecl game_time_update_paused_flags()
{
	//INVOKE(0x00565510, game_time_update_paused_flags);

	if (!game_time_get_paused_for_reason(_game_time_pause_recursion_lock_internal))
	{
		game_time_globals->flags.set(_game_time_pause_recursion_lock_internal, true);
	
		bool v1 = true;
		if (game_is_campaign() && game_is_or_was_cooperative())
			v1 = game_is_server() && user_interface_squad_get_machine_count() <= 1;
	
		if (bink_playback_active())
			v1 = false;
	
		if (game_time_get_paused_for_reason(_game_time_pause_ui) && !v1)
			game_time_set_paused(false, _game_time_pause_ui);
	
		for (e_controller_index controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
		{
			c_controller_interface* controller = controller_get(controller_index);
			if (controller->in_use() && !controller->is_attached() && v1 && controller->get_user_index() != NONE)
			{
				game_time_set_paused(true, k_controller_pause_reasons[controller_index]);
			}
			else
			{
				if (game_time_get_paused_for_reason(k_controller_pause_reasons[controller_index]))
					game_time_set_paused(false, k_controller_pause_reasons[controller_index]);
			}
		}
	
		if (user_interface_xbox_guide_is_active())
		{
			if (v1 && !game_time_get_paused_for_reason(_game_time_pause_xbox_guide_ui))
				game_time_set_paused(true, _game_time_pause_xbox_guide_ui);
		}
		else if (game_time_get_paused_for_reason(_game_time_pause_xbox_guide_ui))
		{
			game_time_set_paused(false, _game_time_pause_xbox_guide_ui);
		}
	
		game_time_globals->flags.set(_game_time_pause_recursion_lock_internal, false);
	}
}

void game_time_statistics_start()
{
	if (debug_game_time_statistics)
	{
		if (!game_time_statistics_file)
			fopen_s(&game_time_statistics_file, "game_time_statistics.txt", "w");

		game_time_statistics_started = true;
		game_time_statistics_update_time = true;
		game_time_statistics_write_header = true;
	}
}

void game_time_statistics_frame(
	real32 world_seconds_elapsed,
	real32 game_seconds_elapsed,
	real32 real_desired_ticks,
	int32 game_ticks_target,
	int32 game_ticks_limit,
	int32 game_ticks_available,
	int32 game_ticks_elapsed,
	real32 game_ticks_leftover,
	bool discontinuity)
{
	if (game_time_statistics_started)
	{
		if (game_time_statistics_update_time)
		{
			game_time_statistics_time = system_milliseconds();
			game_time_statistics_update_time = false;
		}

		uns32 time = system_milliseconds();
		uns32 milliseconds_elapsed = time - game_time_statistics_time;
		game_time_statistics_time = time;

		if (game_time_statistics_file)
		{
			if (game_time_statistics_write_header)
			{
				char date_and_time[256]{};
				system_get_date_and_time(date_and_time, sizeof(date_and_time), false);

				fprintf(game_time_statistics_file, "\nSTATISTICS FOR %s %s\n",
					tag_name_strip_path(game_options_get()->scenario_path.get_string()),
					date_and_time);

				fprintf(game_time_statistics_file, "  time= game time, msec= milliseconds elapsed, w-dt= elapsed world dt, g-dt= elapsed game dt\n");
				fprintf(game_time_statistics_file, "  ticks= real desired ticks, tgt= target updates, lim= limit of updates, avl= currently available updates\n");
				fprintf(game_time_statistics_file, "  ela= actual elapsed ticks, left= leftover fractional tick, d= discontinuity\n");
				fprintf(game_time_statistics_file, "\n");
				fprintf(game_time_statistics_file, "  time msec  w-dt  g-dt    ticks tgt lim avl     ela   left (d)\n");

				game_time_statistics_write_header = false;
			}

			fprintf(game_time_statistics_file, "%6d %4d %5.3f %5.3f    %5.2f %3d %3d %3d     %3d %6.3f  %s\n",
				game_time_globals->time,
				milliseconds_elapsed,
				world_seconds_elapsed,
				game_seconds_elapsed,
				real_desired_ticks,
				game_ticks_target,
				game_ticks_limit,
				game_ticks_available,
				game_ticks_elapsed,
				game_ticks_leftover,
				discontinuity ? "*" : " ");
		}
	}
}

void __cdecl game_time_statistics_stop()
{
	game_time_statistics_started = false;
}

e_game_time_pause_reason const k_controller_pause_reasons[k_number_of_controllers]
{
	_game_time_pause_controller0_removal,
	_game_time_pause_controller1_removal,
	_game_time_pause_controller2_removal,
	_game_time_pause_controller3_removal
};

