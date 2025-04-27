#include "game/game_save.hpp"

#include "ai/ai.hpp"
#include "cache/cache_files.hpp"
#include "effects/effects.hpp"
#include "game/game.hpp"
#include "game/game_allegiance.hpp"
#include "game/players.hpp"
#include "hs/hs_runtime.hpp"
#include "items/items.hpp"
#include "items/projectiles.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "units/units.hpp"
#include "units/vehicles.hpp"

HOOK_DECLARE(0x00682130, __tls_set_g_game_save_globals_allocator);
HOOK_DECLARE(0x006821F0, game_all_quiet);
HOOK_DECLARE(0x00682250, game_safe_to_save);
HOOK_DECLARE(0x00682260, game_safe_to_save_internal);
HOOK_DECLARE(0x006823A0, game_safe_to_speak);
HOOK_DECLARE(0x006823D0, game_save);
HOOK_DECLARE(0x00682420, game_save_cancel);
HOOK_DECLARE(0x00682440, game_save_cinematic_skip);
//HOOK_DECLARE(0x00682490, game_save_dispose);
//HOOK_DECLARE(0x006824A0, game_save_dispose_from_old_map);
HOOK_DECLARE(0x006824B0, game_save_immediate);
//HOOK_DECLARE(0x00682500, game_save_initialize);
//HOOK_DECLARE(0x00682560, game_save_initialize_for_new_map);
HOOK_DECLARE(0x006825B0, game_save_no_timeout);
HOOK_DECLARE(0x00682600, game_save_safe);
HOOK_DECLARE(0x00682650, game_save_update);
HOOK_DECLARE(0x00682770, game_saving);
HOOK_DECLARE(0x006827A0, not_enough_time_since_last_save);

bool debug_game_save = false;

void __cdecl __tls_set_g_game_save_globals_allocator(void* address)
{
	//INVOKE(0x00682130, __tls_set_g_game_save_globals_allocator, address);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	g_game_save_globals = (s_game_save_globals*)address;
}

//.text:00682150 ; 
//.text:00682190 ; 
//.text:006821C0 ; 

bool __cdecl game_all_quiet()
{
	//return INVOKE(0x006821F0, game_all_quiet);

	int32 unsafe_object_index = NONE;
	return !dangerous_projectiles_near_player(&unsafe_object_index)
		&& !dangerous_items_near_player(&unsafe_object_index)
		&& !dangerous_effects_near_player()
		&& !any_unit_is_dangerous(&unsafe_object_index)
		&& !ai_enemies_can_see_player(&unsafe_object_index);
}

bool __cdecl game_safe_to_save()
{
	//return INVOKE(0x00682250, game_safe_to_save);

	return game_safe_to_save_internal(false);
}

bool __cdecl game_safe_to_save_internal(bool a1)
{
	//return INVOKE(0x00682260, game_safe_to_save_internal, a1);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

#define DEBUG_SAFE_TO_SAVE(FUNCTION_NAME, ...) \
	if (FUNCTION_NAME(__VA_ARGS__)) \
	{ \
	    if (debug_game_save && !a1) \
	        console_warning("not safe to save: "#FUNCTION_NAME);\
	    return false; \
	}

	g_game_save_globals->unsafe_object_index = NONE;

	DEBUG_SAFE_TO_SAVE(not_enough_time_since_last_save);
	DEBUG_SAFE_TO_SAVE(game_allegiance_betrayal_exists);
	DEBUG_SAFE_TO_SAVE(ai_enemies_can_see_player, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(dangerous_projectiles_near_player, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(dangerous_items_near_player, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(dangerous_effects_near_player);
	DEBUG_SAFE_TO_SAVE(any_unit_is_dangerous, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(players_any_are_in_the_air, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(players_any_are_near_death, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(players_any_are_dead);
	DEBUG_SAFE_TO_SAVE(game_is_lost);
	DEBUG_SAFE_TO_SAVE(vehicle_moving_near_any_player, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(vehicle_about_to_detonate_near_any_player, &g_game_save_globals->unsafe_object_index);
	DEBUG_SAFE_TO_SAVE(player_is_reading_terminal);
	DEBUG_SAFE_TO_SAVE(hs_runtime_nondeterministic_threads_running);

#undef DEBUG_SAFE_TO_SAVE

	return true;
}

bool __cdecl game_safe_to_speak()
{
	//return INVOKE(0x006823A0, game_safe_to_speak);

	int32 unsafe_object_index = NONE;
	return !dangerous_projectiles_near_player(&unsafe_object_index) && !players_any_are_dead();
}

void __cdecl game_save(int32 save_priority)
{
	//INVOKE(0x006823D0, game_save, game_save_type);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	if (save_priority > g_game_save_globals->save_priority)
	{
		g_game_save_globals->save_priority = save_priority;
		g_game_save_globals->ticks_until_next_save_check = 0;
		g_game_save_globals->save_start_time = game_time_get();
		g_game_save_globals->safe_interval_count = 0;
	}
}

void __cdecl game_save_cancel()
{
	//INVOKE(0x00682420, game_save_cancel);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	if (debug_game_save)
		console_printf("game save cancelled");

	g_game_save_globals->save_priority = _game_save_priority_none;
}

void __cdecl game_save_cinematic_skip()
{
	//INVOKE(0x00682440, game_save_cinematic_skip);

	if (debug_game_save)
		console_printf("beginning cinematic skip game save");

	game_save(_game_save_priority_cinematic_skip);
}

void __cdecl game_save_dispose()
{
	//INVOKE(0x00682490, game_save_dispose);
}

void __cdecl game_save_dispose_from_old_map()
{
	//INVOKE(0x006824A0, game_save_dispose_from_old_map);
}

void __cdecl game_save_immediate()
{
	//INVOKE(0x006824B0, game_save_immediate);

	if (debug_game_save)
		console_printf("beginning immediate game save");

	game_save(_game_save_priority_immediate);
}

void __cdecl game_save_initialize()
{
	//INVOKE(0x00682500, game_save_initialize);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	g_game_save_globals = (s_game_save_globals*)g_game_save_globals_allocator.allocate(sizeof(s_game_save_globals), "game save globals");
}

void __cdecl game_save_initialize_for_new_map()
{
	//INVOKE(0x00682560, game_save_initialize_for_new_map);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	csmemset(g_game_save_globals, 0, sizeof(s_game_save_globals));
	g_game_save_globals->time_of_last_game_save = NONE;
	g_game_save_globals->unsafe_object_index = NONE;
	game_save_cancel();
}

void __cdecl game_save_no_timeout()
{
	//INVOKE(0x006825B0, game_save_no_timeout);

	if (debug_game_save)
		console_printf("beginning no-timeout game save");

	game_save(_game_save_priority_no_timeout);
}

void __cdecl game_save_safe()
{
	//INVOKE(0x00682600, game_save_safe);

	if (debug_game_save)
		console_printf("beginning no-timeout game save");

	game_save(_game_save_priority_safe);
}

void __cdecl game_save_update()
{
	//INVOKE(0x00682650, game_save_update);

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	bool perform_cancel = false;
	bool perform_save = false;
	bool cinematic_skip = false;

	if (cinematic_in_progress())
	{
		if (g_game_save_globals->save_priority != _game_save_priority_cinematic_skip)
			return;

	LABEL_0:;
		if (game_is_lost())
		{
			if (!debug_game_save)
			{
			LABEL_1:;
				g_game_save_globals->save_priority = _game_save_priority_none;
				return;
			}

			console_printf("game is lost, cannot immediate-save, cancelling");

		LABEL_2:;
			if (debug_game_save)
			{
				console_printf("canceling!");

				if (debug_game_save)
					console_printf("game save cancelled");
			}

			goto LABEL_1;
		}

		if (debug_game_save)
			console_printf("performing immediate save");

		cinematic_skip = g_game_save_globals->save_priority == _game_save_priority_cinematic_skip;
		goto LABEL_5;
	}

	if (g_game_save_globals->save_priority <= _game_save_priority_none)
		return;

	if (g_game_save_globals->save_priority >= _game_save_priority_immediate)
		goto LABEL_0;

	if (g_game_save_globals->ticks_until_next_save_check > 0)
	{
		g_game_save_globals->ticks_until_next_save_check--;

	LABEL_3:;
		if (g_game_save_globals->save_priority != _game_save_priority_no_timeout)
		{
			if (game_ticks_to_seconds(real32(game_time_get() - g_game_save_globals->save_start_time)) >= 8.0f)
			{
				if (debug_game_save)
					console_printf("timeout trying to save");

				perform_cancel = true;
			}
		}

		goto LABEL_4;
	}

	if (game_safe_to_save())
	{
		if (debug_game_save)
			console_printf("game currently safe to save");

		perform_save = ++g_game_save_globals->safe_interval_count >= 3;
	}
	else
	{
		if (debug_game_save)
			console_printf("game currently unsafe to save");

		g_game_save_globals->safe_interval_count = 0;
	}

	g_game_save_globals->ticks_until_next_save_check = game_seconds_to_ticks_round(0.33f);
	if (!perform_save)
		goto LABEL_3;

LABEL_4:;
	if (debug_game_save)
	{
		if (g_game_save_globals->unsafe_object_index != NONE)
		{
			real_point3d unsafe_object_position{};
			object_get_origin(g_game_save_globals->unsafe_object_index, &unsafe_object_position);

			real_point3d camera_position = unsafe_object_position;
			if (s_observer_result const* camera = observer_try_and_get_camera(player_mapping_first_active_output_user()))
			{
				camera_position.x = camera->position.x + (camera->forward.i * 0.05f);
				camera_position.y = camera->position.y + (camera->forward.j * 0.05f);
				camera_position.z = camera->position.z + (camera->forward.k * 0.05f);
			}

			render_debug_line(false, &unsafe_object_position, &camera_position, global_real_argb_red);
			render_debug_sphere(false, &unsafe_object_position, 0.5f, global_real_argb_red);

			if (object_datum* object = object_get(g_game_save_globals->unsafe_object_index))
				render_debug_string_at_point(&unsafe_object_position, tag_get_name(object->definition_index), global_real_argb_white);
		}
	}

	if (!perform_save)
	{
		if (!perform_cancel)
			return;

		goto LABEL_2;
	}

LABEL_5:;
	if (debug_game_save)
		console_printf("saving!");

	g_game_save_globals->save_priority = _game_save_priority_none;

	if (cinematic_skip)
	{
		director_reset();
	}
#if defined(_DEBUG)
	else
	{
		c_data_iterator<player_datum> player_iterator;
		player_iterator.begin(player_data);
		while (player_iterator.next())
		{
			int32 unit_index = player_iterator.get_datum()->unit_index;
			if (unit_index != NONE)
			{
				if (unit_datum* unit = UNIT_GET(unit_index))
				{
					//if (unit->object.current_body_damage < 1.0f)
					//{
					//	unit->object.current_body_damage = 1.0f;
					//	unit->object.body_stun_ticks = 0;
					//}

					object_datum* object = (object_datum*)unit;
					if (object->object.body_vitality < 1.0f)
					{
						object->object.body_vitality = 1.0f;
						object->object.body_stun_ticks = 0;
					}
				}
			}
		}
	}
#endif

	main_save_map();

	g_game_save_globals->time_of_last_game_save = game_time_get();
}

bool __cdecl game_saving()
{
	//return INVOKE(0x00682770, game_saving);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);
	return main_save_map_pending() || g_game_save_globals->save_priority;
}

bool __cdecl not_enough_time_since_last_save()
{
	//return INVOKE(0x006827A0, not_enough_time_since_last_save);

	TLS_DATA_GET_VALUE_REFERENCE(g_game_save_globals);

	bool result = false;
	if (g_game_save_globals->time_of_last_game_save != NONE)
	{
		if (game_time_get() <= g_game_save_globals->time_of_last_game_save + game_seconds_to_ticks_round(10.0f))
			result = true;
	}

	ASSERT(g_game_save_globals->time_of_last_game_save == NONE || g_game_save_globals->time_of_last_game_save <= game_time_get());

	return result;
}

//.text:00682800 ; 

