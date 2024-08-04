#include "game/player_control.hpp"

#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE_CALL(0x005D1389, evaluate_piecewise_linear_function);
HOOK_DECLARE_CALL(0x005D13B4, evaluate_piecewise_linear_function);

e_input_user_index first_input_user()
{
	return _input_user_index0;
}

e_input_user_index next_input_user(e_input_user_index input_user_index)
{
	if (VALID_INDEX(input_user_index + 1, k_number_of_input_users))
		return e_input_user_index(input_user_index + 1);

	return k_input_user_none;
}

e_output_user_index first_output_user()
{
	return _output_user_index0;
}

e_output_user_index next_output_user(e_output_user_index output_user_index)
{
	if (VALID_INDEX(output_user_index + 1, k_number_of_output_users))
		return e_output_user_index(output_user_index + 1);

	return k_output_user_none;
}

//s_player_control_input_state* player_control_input_get(e_input_user_index)
s_player_control_input_state* player_control_input_get(e_input_user_index input_user_index)
{
	//ASSERT(VALID_INDEX(input_user_index, k_number_of_input_users));
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	return &player_control_globals->input_states[input_user_index];
}

//s_player_control_output_state* player_control_output_get(e_output_user_index)
s_player_control_output_state* player_control_output_get(e_output_user_index output_user_index)
{
	//ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));
	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	return &player_control_globals->output_state[output_user_index];
}

void player_control_update_debug_render()
{
	//if (debug_player_input)
	//{
	//	console_clear();
	//
	//	// #TODO: implement
	//}
}

//.text:005CF8A0 ; void __cdecl __tls_set_g_player_control_globals_allocator(void* address)
//.text:005CF8C0 ; void __cdecl __tls_set_g_player_control_globals_deterministic_allocator(void* address)
//.text:005CF8E0 ; 
//.text:005CF920 ; 
//.text:005CF990 ; void __cdecl clear_player_control_input(s_player_control_input* input)
//.text:005CF9C0 ; 
//.text:005CF9F0 ; 
//.text:005CFA20 ; public: real __cdecl c_abstract_button::down_amount(void)

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

//.text:005CFB30 ; 
//.text:005CFB50 ; 
//.text:005CFB60 ; 
//.text:005CFB90 ; 
//.text:005CFBC0 ; 
//.text:005CFBD0 ; 
//.text:005CFBF0 ; 
//.text:005CFC00 ; 
//.text:005CFC10 ; 
//.text:005CFC30 ; 
//.text:005CFC60 ; 
//.text:005CFC70 ; controller_get_look_invert
//.text:005CFC80 ; void __cdecl player0_controller_set_look_invert(bool)
//.text:005CFCC0 ; real __cdecl player0_get_looking_pitch()
//.text:005CFD10 ; bool __cdecl player0_looking_down()
//.text:005CFD60 ; bool __cdecl player0_looking_up()
//.text:005CFDB0 ; void __cdecl player0_set_pitch(real, long)
//.text:005CFDD0 ; void __cdecl player1_set_pitch(real, long)
//.text:005CFDF0 ; void __cdecl player2_set_pitch(real, long)
//.text:005CFE10 ; void __cdecl player3_set_pitch(real, long)
//.text:005CFE30 ; void __cdecl player_action_test_inhibit_button_flags_set_all_players(e_player_control_action_test_bit, bool)
//.text:005CFEB0 ; void __cdecl player_action_test_testing_for_action_flags_set_all_players(e_player_control_action_test_bit, bool)
//.text:005CFF30 ; bool __cdecl player_control_action_test_accept()
//.text:005CFF50 ; bool __cdecl player_control_action_test_action()
//.text:005CFF70 ; bool __cdecl player_control_action_test_any_players(e_player_control_action_test_bit)
//.text:005CFFE0 ; bool __cdecl player_control_action_test_any_players_range(e_player_control_action_test_bit, e_player_control_action_test_bit)
//.text:005D0050 ; bool __cdecl player_control_action_test_back()
//.text:005D0060 ; bool __cdecl player_control_action_test_cancel()
//.text:005D0080 ; bool __cdecl player_control_action_test_dpad_down()
//.text:005D0090 ; bool __cdecl player_control_action_test_dpad_left()
//.text:005D00A0 ; bool __cdecl player_control_action_test_dpad_right()
//.text:005D00B0 ; bool __cdecl player_control_action_test_dpad_up()
//.text:005D00C0 ; player_control_action_test_unknown1F_plus
//.text:005D00E0 ; bool __cdecl player_control_action_test_grenade_trigger()
//.text:005D00F0 ; void __cdecl player_control_action_test_input(e_input_user_index, s_game_input_state*, s_player_control_input*)
//.text:005D05A0 ; bool __cdecl player_control_action_test_jump()
//.text:005D05B0 ; bool __cdecl player_control_action_test_left_shoulder()
//.text:005D05C0 ; bool __cdecl player_control_action_test_look_invert()
//.text:005D05D0 ; bool __cdecl player_control_action_test_look_relative_all_directions()
//.text:005D05E0 ; bool __cdecl player_control_action_test_look_relative_down()
//.text:005D05F0 ; bool __cdecl player_control_action_test_look_relative_left()
//.text:005D0600 ; bool __cdecl player_control_action_test_look_relative_right()
//.text:005D0610 ; bool __cdecl player_control_action_test_look_relative_up()
//.text:005D0620 ; bool __cdecl player_control_action_test_lookstick_backward()
//.text:005D0630 ; bool __cdecl player_control_action_test_lookstick_forward()
//.text:005D0640 ; bool __cdecl player_control_action_test_melee()
//.text:005D0650 ; bool __cdecl player_control_action_test_move_relative_all_directions()
//.text:005D0660 ; bool __cdecl player_control_action_test_primary_trigger()
//.text:005D0670 ; void __cdecl player_control_action_test_reset()
//.text:005D0760 ; bool __cdecl player_control_action_test_right_shoulder()
//.text:005D0770 ; bool __cdecl player_control_action_test_rotate_grenades()
//.text:005D0780 ; bool __cdecl player_control_action_test_rotate_weapons()
//.text:005D0790 ; bool __cdecl player_control_action_test_x()
//.text:005D07A0 ; bool __cdecl player_control_action_test_y()
//.text:005D07B0 ; bool __cdecl player_control_action_test_zoom()

//void __cdecl player_control_build_action(long, e_input_user_index, s_player_action*)
void __cdecl player_control_build_action(long player_index, e_input_user_index input_user_index, s_player_action* action)
{
	INVOKE(0x005D07C0, player_control_build_action, player_index, input_user_index, action);
}

//.text:005D0860 ; bool __cdecl player_control_camera_control_is_active()
//.text:005D0880 ; bool __cdecl player_control_compute_input_inhibition(e_input_user_index, e_controller_index, s_game_input_state const*, s_player_control_input*)
//.text:005D08C0 ; void __cdecl player_control_copy_state_from_action(s_player_action const*, s_player_control_state*)
//.text:005D0980 ; void __cdecl player_control_copy_state_from_unit(long, s_player_control_state*)
//.text:005D0A40 ; void __cdecl player_control_dispose()
//.text:005D0A50 ; void __cdecl player_control_dispose_from_old_map()
//.text:005D0A60 ; void __cdecl player_control_fade_in_all_input(real)
//.text:005D0A90 ; void __cdecl player_control_fade_in_all_input_for_player(long, real)
//.text:005D0AF0 ; void __cdecl player_control_fade_out_all_input(real)
//.text:005D0B20 ; void __cdecl player_control_fade_out_all_input_for_player(long, real)
//.text:005D0B80 ; bool __cdecl player_control_flags_test_all_players(e_player_control_bit)

s_player_action_context const* __cdecl player_control_get_action_context(long user_index)
{
	return INVOKE(0x005D0BD0, player_control_get_action_context, user_index);
}

//.text:005D0C00 ; long __cdecl player_control_get_aiming_unit_index(e_output_user_index)

void __cdecl player_control_get_aiming_vector(e_output_user_index output_user_index, vector3d* aiming_vector)
{
	INVOKE(0x005D0C30, player_control_get_aiming_vector, output_user_index, aiming_vector);
}

//.text:005D0C90 ; void __cdecl player_control_get_controller_input(e_input_user_index, e_controller_index, real, real, s_game_input_state**, s_player_control_input*)
//.text:005D29E0 ; euler_angles2d const* __cdecl player_control_get_facing_angles(long)
//.text:005D2A10 ; real __cdecl player_control_get_field_of_view(long)
//.text:005D2AA0 ; real __cdecl player_control_get_field_of_view_change_time(long)

bool __cdecl player_control_get_machinima_camera_debug()
{
	//return INVOKE(0x005D2B40, player_control_get_machinima_camera_debug);

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_debug;

	return false;
}

bool __cdecl player_control_get_machinima_camera_enabled()
{
	//return INVOKE(0x005D2B90, player_control_get_machinima_camera_enabled);

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (player_control_machinima_available())
		return player_control_globals->machinima_camera_enabled;

	return false;
}

bool __cdecl player_control_get_machinima_camera_use_old_controls()
{
	//return INVOKE(0x005D2BE0, player_control_get_machinima_camera_use_old_controls);

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_use_old_controls;

	return false;
}

//.text:005D2C30 ; real __cdecl player_control_get_target_autoaim_level(long)
//.text:005D2C80 ; long __cdecl player_control_get_target_player_index(e_output_user_index)
//.text:005D2CE0 ; s_aim_assist_targeting_result const* __cdecl player_control_get_targeting(long)
//.text:005D2D10 ; void __cdecl player_control_get_unit_camera_info(long, s_unit_camera_info*)

short __cdecl player_control_get_zoom_level(long user_index)
{
	return INVOKE(0x005D2D50, player_control_get_zoom_level, user_index);
}

//void __cdecl player_control_suppress_rotate_weapons(enum e_input_user_index)
void __cdecl player_control_suppress_rotate_weapons(long user_index)
{
	INVOKE(0x005D4860, player_control_suppress_rotate_weapons, user_index);
}

//.text:005D2EA0 ; void __cdecl player_control_handle_weapon_put_away(long, short)
//.text:005D2EE0 ; void __cdecl player_control_initialize()
//.text:005D3120 ; void __cdecl player_control_initialize_for_new_map()
//.text:005D3290 ; void __cdecl player_control_input_set_unit_index(e_input_user_index, long)
//.text:005D3360 ; void __cdecl player_control_lock_gaze(e_input_user_index, long, real)

bool __cdecl player_control_machinima_available()
{
	//return INVOKE(0x005D3400, player_control_machinima_available);

	return game_in_progress() && !game_is_ui_shell() && game_options_get()->game_network_type != _network_game_simulation_sync_server;
}

//.text:005D3430 ; void __cdecl player_control_modify_desired_angles_default(e_input_user_index, real, real, real, real)
//.text:005D4350 ; s_player_control_non_deterministic_input_user_state* __cdecl player_control_non_deterministic_input_user_state_get(e_input_user_index)
//.text:005D4380 ; void __cdecl player_control_output_set_unit_index(e_output_user_index, long)
//.text:005D43E0 ; void __cdecl player_control_permanent_impulse(long, euler_angles2d const*)

//void __cdecl player_control_propagate_output(e_input_user_index)
void __cdecl player_control_propagate_output(e_input_user_index input_user_index)
{
	//INVOKE(0x005D4430, player_control_propagate_output, input_user_index);

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	s_player_control_input_state* player_control_input = player_control_input_get(input_user_index);

	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		s_player_control_output_state* player_control_output = player_control_output_get(user_index);
		if (player_control_output->unit_index == player_control_input->unit_index)
			csmemcpy(&player_control_output->output, &player_control_input->state, sizeof(s_player_control_state));
	}
}

//.text:005D44E0 ; void __cdecl player_control_scale_all_input(real, real)
//.text:005D45E0 ; void __cdecl player_control_scale_all_input_for_player(long, real, real)
//.text:005D4690 ; void __cdecl player_control_set_deterministic_action_test_flags(long, qword)
//.text:005D46C0 ; void __cdecl player_control_set_external_action(e_output_user_index, s_player_action const*)

void __cdecl player_control_set_facing(e_input_user_index input_user_index, vector3d const* facing)
{
	INVOKE(0x005D4700, player_control_set_facing, input_user_index, facing);
}

//.text:005D4760 ; void __cdecl player_control_state_build_action(s_player_control_state const*, s_player_action*)
//.text:005D4820 ; void __cdecl player_control_state_clear(s_player_control_state*)
//.text:005D4860 ; void __cdecl player_control_suppress_rotate_weapons(e_input_user_index)
//.text:005D48A0 ; void __cdecl player_control_unlock_gaze(e_input_user_index)
//.text:005D48E0 ; void __cdecl player_control_unzoom(e_input_user_index)
//.text:005D4920 ; void __cdecl player_control_unzoom_all()

void __cdecl player_control_update(real world_seconds_elapsed, real game_seconds_elapsed)
{
	//INVOKE(0x005D4990, player_control_update, world_seconds_elapsed, game_seconds_elapsed);

	//collision_log_begin_period(5);
	//player_control_update_debug_begin();

	dword player_mask = 0;
	c_static_array<s_player_action, 4> actions{};

	for (e_input_user_index user_index = first_input_user(); user_index != k_input_user_none; user_index = next_input_user(user_index))
	{
		long player_index = player_mapping_get_player_by_input_user(user_index);
		if (player_index != NONE)
		{
			e_controller_index controller_index = player_mapping_get_input_controller(player_index);
			player_control_update_player(player_index, user_index, controller_index, world_seconds_elapsed, game_seconds_elapsed);
			player_control_build_action(player_index, user_index, &actions[user_index]);
			player_control_propagate_output(user_index);

			player_mask |= FLAG(user_index);
		}
	}

	simulation_process_input(player_mask, actions);
	//collision_log_end_period();
	main_time_mark_input_collection_time();
	player_control_update_debug_render();
	player_control_update_machinima();
}

void __cdecl player_control_update_machinima()
{
	//INVOKE(0x005D4A60, player_control_update_machinima);

	TLS_DATA_GET_VALUE_REFERENCE(player_control_globals);

	if (!player_control_get_machinima_camera_enabled())
		return;

	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		e_controller_index controller_index = controller_index_from_output_user_index(user_index);
		s_game_input_state* input_state = NULL;

		if (VALID_CONTROLLER(controller_index))
			input_abstraction_get_input_state(controller_index, &input_state);

		if (input_state)
		{
			if (input_state->get_button(_button_action_crouch).down_frames() && input_state->get_button(_button_action_zoom).down_frames())
			{
				if (input_state->get_button(_button_action_unknown37).down_frames() == 1) // dpad left
				{
					player_control_globals->machinima_camera_use_old_controls = !player_control_globals->machinima_camera_use_old_controls;
					user_interface_play_sound(_user_interface_sound_effect_x_button, NONE);
				}

				if (input_state->get_button(_button_action_unknown38).down_frames() == 1) // dpad right
				{
					player_control_globals->machinima_camera_debug = !player_control_globals->machinima_camera_debug;
					user_interface_play_sound(_user_interface_sound_effect_x_button, NONE);
				}
			}
		}
	}
}

//void __cdecl player_control_update_player(long, e_input_user_index, e_controller_index, real, real)
void __cdecl player_control_update_player(long player_index, e_input_user_index input_user_index, e_controller_index controller_index, real world_seconds_elapsed, real game_seconds_elapsed)
{
	INVOKE(0x005D4BF0, player_control_update_player, player_index, input_user_index, controller_index, world_seconds_elapsed, game_seconds_elapsed);
}

//.text:005D5590 ; void __cdecl player_set_pitch_internal(e_input_user_index, real, long)
//.text:005D5670 ; bool __cdecl sub_5D5670(e_controller_index, s_player_control_input* input)
//.text:005D56E0 ; 
//.text:005D5720 ; 
//.text:005D5760 ; 
//.text:005D57F0 ; bool __cdecl scripted_player_control_set_camera_control(bool)
//.text:005D5840 ; 
//.text:005D5870 ; 
//.text:005D58C0 ; 
//.text:005D58F0 ; 
//.text:005D5910 ; real __cdecl signed_angular_difference(real, real)
//.text:005D5970 ; 
//.text:005D59B0 ; 
//.text:005D59D0 ; 
//.text:005D5A20 ; void __cdecl player_action_test_inhibit_button_flags_set(long, e_player_control_action_test_bit, bool)
//.text:005D5AB0 ; void __cdecl player_action_test_testing_for_action_flags_set(long, e_player_control_action_test_bit, bool)
//.text:005D5B40 ; bool __cdecl player_control_action_test(long, e_player_control_action_test_bit)
//.text:005D5BC0 ; bool __cdecl player_control_action_test_accept(long)
//.text:005D5C60 ; bool __cdecl player_control_action_test_action(long)
//.text:005D5D00 ; bool __cdecl player_control_action_test_back(long)
//.text:005D5D20 ; bool __cdecl player_control_action_test_cancel(long)
//.text:005D5DC0 ; bool __cdecl player_control_action_test_dpad_down(long)
//.text:005D5DE0 ; bool __cdecl player_control_action_test_dpad_left(long)
//.text:005D5E00 ; bool __cdecl player_control_action_test_dpad_right(long)
//.text:005D5E20 ; bool __cdecl player_control_action_test_dpad_up(long)
//.text:005D5E40 ; bool __cdecl player_control_action_test_grenade_trigger(long)
//.text:005D5E60 ; bool __cdecl player_control_action_test_left_shoulder(long)
//.text:005D5E80 ; bool __cdecl player_control_action_test_look_invert(long)
//.text:005D5EA0 ; bool __cdecl player_control_action_test_look_relative_all_directions(long)
//.text:005D5EC0 ; bool __cdecl player_control_action_test_look_relative_down(long)
//.text:005D5EE0 ; bool __cdecl player_control_action_test_look_relative_left(long)
//.text:005D5F00 ; bool __cdecl player_control_action_test_look_relative_right(long)
//.text:005D5F20 ; bool __cdecl player_control_action_test_look_relative_up(long)
//.text:005D5F40 ; bool __cdecl player_control_action_test_lookstick_backward(long)
//.text:005D5F60 ; bool __cdecl player_control_action_test_lookstick_forward(long)
//.text:005D5F80 ; bool __cdecl player_control_action_test_melee(long)
//.text:005D5FA0 ; bool __cdecl player_control_action_test_move_relative_all_directions(long)
//.text:005D5FC0 ; bool __cdecl player_control_action_test_range(long, e_player_control_action_test_bit, e_player_control_action_test_bit)
//.text:005D6060 ; void __cdecl player_control_action_test_reset(long)
//.text:005D60D0 ; bool __cdecl player_control_action_right_shoulder(long)
//.text:005D60F0 ; bool __cdecl player_control_action_test_rotate_grenades(long)
//.text:005D6110 ; bool __cdecl player_control_action_test_rotate_weapons(long)
//.text:005D6130 ; bool __cdecl player_control_action_test_vision_trigger(long)
//.text:005D6150 ; bool __cdecl player_control_action_x(long)
//.text:005D6170 ; bool __cdecl player_control_action_y(long)
//.text:005D6190 ; void __cdecl player_control_lock_accept_button_until_pressed(long)
//.text:005D61F0 ; void __cdecl player_control_lock_cancel_button_until_pressed(long)
//.text:005D6250 ; void __cdecl player_control_lock_y_button_until_pressed(long)
//.text:005D62B0 ; 
//.text:005D62C0 ; bool __cdecl user_currently_piloting_aircraft(enum e_input_user_index)

