#include "game/game_grief.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x024B06B4, s_game_grief_globals, game_grief_globals);

void __cdecl game_grief_record_reprieve()
{
	INVOKE(0x00678F10, game_grief_record_reprieve);

	//if (game_is_playback())
	//	return;
	//
	//game_grief_globals.reprieve_time = system_milliseconds();
	//generate_event(_event_level_status, "betrayal forgiveness granted");
}

bool __cdecl game_grief_can_eject(long player_index)
{
	return INVOKE(0x00678F30, game_grief_can_eject, player_index);

	//if (game_is_playback())
	//	return;
	//
	//return (!current_game_variant() || current_game_variant()->get_active_variant()->get_social_options()->get_betrayal_booting_enabled())
	//	&& global_preferences_get_eviction_count() <= g_network_configuration.maximum_griefer_eviction_count
	//	&& game_grief_globals.reprieve_time <= 0;
}

void __cdecl game_grief_dispose()
{
	INVOKE(0x00678F90, game_grief_dispose);
}

void __cdecl game_grief_dispose_from_old_map()
{
	INVOKE(0x00678FA0, game_grief_dispose_from_old_map);
}

bool __cdecl game_grief_get_ui_active_for_local_user(e_controller_index controller_index)
{
	return INVOKE(0x00678FB0, game_grief_get_ui_active_for_local_user, controller_index);

	//if (game_is_playback())
	//	return false;
	//
	//return game_grief_globals.ui_active[controller_index];
}

void __cdecl game_grief_initialize()
{
	INVOKE(0x00678FD0, game_grief_initialize);

	//csmemset(&game_grief_globals, 0, sizeof(game_grief_globals));
}

void __cdecl game_grief_initialize_for_new_map()
{
	INVOKE(0x00678FF0, game_grief_initialize_for_new_map);

	//long constexpr k_size = sizeof(game_grief_globals) - OFFSETOF(s_game_grief_globals, reprieve_time);
	//csmemset(&game_grief_globals.reprieve_time, 0, k_size);
}

void __cdecl game_grief_record_betrayal(long player_index)
{
	INVOKE(0x00679010, game_grief_record_betrayal, player_index);

	//if (game_is_playback())
	//	return;
	//
	//e_controller_index controller = player_mapping_get_input_controller(player_index);
	//bool is_griefer = false;
	//real betrayal_count = global_preferences_get_betrayal_count() + 1;
	//game_grief_globals.betrayal_time = system_milliseconds();
	//if (betrayal_count >= g_network_configuration.maximum_griefer_betrayal_count && controller != k_no_controller)
	//{
	//	user_interface_controller_set_griefer(controller, true);
	//	user_interface_controller_update_network_properties(controller);
	//	is_griefer = true;
	//}
	//
	//generate_event(_event_level_status, "local absolute player %d controller %d betrayed (count %d, %s)!",
	//	player_index,
	//	controller,
	//	betrayal_count,
	//	is_griefer ? "GRIEFER" : "not yet griefer");
	//
	//global_preferences_set_betrayal_count(betrayal_count);
}

void __cdecl game_grief_record_ejection(long player_index)
{
	INVOKE(0x00679070, game_grief_record_ejection, player_index);

	//if (game_is_playback())
	//	return;
	//
	//long eviction_count = global_preferences_get_eviction_count();
	//game_grief_globals.ejection_time = system_milliseconds();
	//
	//generate_event(_event_level_status, "ejection recorded for player %d", player_index);
	//
	//global_preferences_set_eviction_count(eviction_count + 1);
}

void __cdecl sub_6790A0(e_controller_index controller_index, bool ui_active)
{
	INVOKE(0x006790A0, sub_6790A0, controller_index, ui_active);

	//if (game_is_playback())
	//	return;
	//
	//ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	//
	//game_grief_globals.ui_active[controller_index] = ui_active;
	//if (ui_active)
	//	game_grief_globals.reprieve_time = 0;
}

void __cdecl game_grief_update()
{
	INVOKE(0x006790D0, game_grief_update);

	//if (game_is_playback())
	//	return;
	//
	//unsigned long time = system_milliseconds();
	//
	//real betrayal_count = global_preferences_get_betrayal_count();
	//if (betrayal_count > 0.0f && (time - game_grief_globals.betrayal_time) > g_network_configuration.griefer_betrayal_timeout)
	//{
	//	generate_event(_event_level_status, "ejection count decremented!");
	//
	//	game_grief_globals.ejection_time = time;
	//	global_preferences_set_betrayal_count(MAX(betrayal_count - 1.0f, 0.0f));
	//
	//	if (g_network_configuration.maximum_griefer_betrayal_count > betrayal_count)
	//	{
	//		for (long controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
	//		{
	//			user_interface_controller_set_griefer(controller_index, false);
	//			user_interface_controller_update_network_properties(controller_index);
	//		}
	//	}
	//}
	//
	//long eviction_count = global_preferences_get_eviction_count();
	//if (eviction_count > 0 && (time - game_grief_globals.ejection_time) > g_network_configuration.griefer_ejection_timeout)
	//{
	//	generate_event(_event_level_status, "ejection count decremented!");
	//
	//	game_grief_globals.ejection_time = time;
	//	global_preferences_set_eviction_count(MAX(eviction_count - 1, 0));
	//}
	//
	//if (game_grief_globals.reprieve_time > 0 && time - game_grief_globals.reprieve_time >= 10000)
	//{
	//	generate_event(_event_level_status, "forgiveness grace period expired");
	//
	//	game_grief_globals.reprieve_time = 0;
	//}
	//
	//game_grief_globals.update_time = time;
}

