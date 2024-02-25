#include "game/player_control.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE_CALL(0x005D1389, evaluate_piecewise_linear_function);
HOOK_DECLARE_CALL(0x005D13B4, evaluate_piecewise_linear_function);

bool __cdecl player_control_get_machinima_camera_debug()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_debug;

	return false;
}

bool __cdecl player_control_get_machinima_camera_enabled()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (game_in_progress() && !game_is_ui_shell())
		return player_control_globals->machinima_camera_enabled;

	return false;
}

bool __cdecl player_control_get_machinima_camera_use_old_controls()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_use_old_controls;

	return false;
}

void __cdecl player_control_set_machinima_camera(bool enabled, bool debug, bool use_old_controls)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	player_control_globals->machinima_camera_enabled = enabled;
	player_control_globals->machinima_camera_debug = debug;
	player_control_globals->machinima_camera_use_old_controls = use_old_controls;
}

void __cdecl player_control_toggle_machinima_camera_enabled()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	player_control_globals->machinima_camera_enabled = !player_control_globals->machinima_camera_enabled;
}

void __cdecl player_control_toggle_machinima_camera_use_old_controls()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	player_control_globals->machinima_camera_use_old_controls = !player_control_globals->machinima_camera_use_old_controls;
}

void __cdecl player_control_toggle_machinima_camera_debug()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	player_control_globals->machinima_camera_debug = !player_control_globals->machinima_camera_debug;
}

void __cdecl player_control_toggle_player_input(long user_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	player_control_globals->input_user_states[user_index].player_input_locked = !player_control_globals->input_user_states[user_index].player_input_locked;
}

// crash hack fix for controller
// count is player_control[i]->look_function.count
// function is player_control[i]->look_function.address
// there is no check inplace of count
// count is used to determine index
real __cdecl evaluate_piecewise_linear_function(short count, real* const function, real a3)
{
	if (count <= 0)
		return a3;

	return INVOKE(0x005CFA50, evaluate_piecewise_linear_function, count, function, a3);
}

short __cdecl player_control_get_zoom_level(long user_index)
{
	return INVOKE(0x005D2D50, player_control_get_zoom_level, user_index);
}

//void __cdecl player_control_suppress_rotate_weapons(enum e_input_user_index)
void __cdecl player_control_suppress_rotate_weapons(long user_index)
{
	INVOKE(0x005D4860, player_control_suppress_rotate_weapons, user_index);
}

void __cdecl player_control_update(real world_seconds_elapsed, real game_seconds_elapsed)
{
	INVOKE(0x005D4990, player_control_update, world_seconds_elapsed, game_seconds_elapsed);
}

