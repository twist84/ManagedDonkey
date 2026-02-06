#include "game/player_control.hpp"

#include "game/cheats.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/mover.hpp"
#include "simulation/simulation.hpp"
#include "units/bipeds.hpp"

HOOK_DECLARE_CALL(0x005D1389, evaluate_piecewise_linear_function);
HOOK_DECLARE_CALL(0x005D13B4, evaluate_piecewise_linear_function);

int32 first_input_user()
{
	return 0;
}

int32 next_input_user(int32 input_user_index)
{
	if (VALID_INDEX(input_user_index + 1, k_number_of_users))
		return int32(input_user_index + 1);

	return NONE;
}

int32 first_output_user()
{
	return 0;
}

int32 next_output_user(int32 user_index)
{
	if (VALID_INDEX(user_index + 1, k_number_of_users))
		return int32(user_index + 1);

	return NONE;
}

//s_player_control_input_state* player_control_input_get(int32)
s_player_control_input_state* player_control_input_get(int32 input_user_index)
{
	//ASSERT(VALID_INDEX(input_user_index, k_number_of_users));

	return &player_control_globals->input_states[input_user_index];
}

//s_player_control_output_state* player_control_output_get(int32)
s_player_control_output_state* player_control_output_get(int32 user_index)
{
	//ASSERT(VALID_INDEX(user_index, k_number_of_users));

	return &player_control_globals->output_state[user_index];
}

void player_control_update_debug_render()
{
	//if (debug_player_input)
	//{
	//	console_clear();
	//
	//	// $IMPLEMENT
	//}
}

//.text:005CF8A0 ; void __cdecl __tls_set_g_player_control_globals_allocator(void* address)
//.text:005CF8C0 ; void __cdecl __tls_set_g_player_control_globals_deterministic_allocator(void* address)
//.text:005CF8E0 ; 
//.text:005CF920 ; 
//.text:005CF990 ; void __cdecl clear_player_control_input(s_player_control_input* input)
//.text:005CF9C0 ; 
//.text:005CF9F0 ; 
//.text:005CFA20 ; public: real32 __cdecl c_abstract_button::down_amount(void)

// crash hack fix for controller
// count is player_control[i]->look_function.count
// function is player_control[i]->look_function.address
// there is no check inplace of count
// count is used to determine index
real32 __cdecl evaluate_piecewise_linear_function(int16 count, real32* const function, real32 a3)
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
//.text:005CFC70 ; bool __cdecl player0_controller_get_look_invert()
//.text:005CFC80 ; void __cdecl player0_controller_set_look_invert(bool)
//.text:005CFCC0 ; real32 __cdecl player0_get_looking_pitch()
//.text:005CFD10 ; bool __cdecl player0_looking_down()
//.text:005CFD60 ; bool __cdecl player0_looking_up()
//.text:005CFDB0 ; void __cdecl player0_set_pitch(real32, int32)
//.text:005CFDD0 ; void __cdecl player1_set_pitch(real32, int32)
//.text:005CFDF0 ; void __cdecl player2_set_pitch(real32, int32)
//.text:005CFE10 ; void __cdecl player3_set_pitch(real32, int32)
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
//.text:005D00F0 ; void __cdecl player_control_action_test_input(int32, s_game_input_state*, s_player_control_input*)
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

//void __cdecl player_control_build_action(int32, int32, player_action*)
void __cdecl player_control_build_action(int32 player_index, int32 input_user_index, player_action* action)
{
	INVOKE(0x005D07C0, player_control_build_action, player_index, input_user_index, action);
}

//.text:005D0860 ; bool __cdecl player_control_camera_control_is_active()
//.text:005D0880 ; bool __cdecl player_control_compute_input_inhibition(int32, e_controller_index, const s_game_input_state*, s_player_control_input*)
//.text:005D08C0 ; void __cdecl player_control_copy_state_from_action(const player_action*, s_player_control_state*)
//.text:005D0980 ; void __cdecl player_control_copy_state_from_unit(int32, s_player_control_state*)

void __cdecl player_control_dispose()
{
	INVOKE(0x005D0A40, player_control_dispose);
}

void __cdecl player_control_dispose_from_old_map()
{
	INVOKE(0x005D0A50, player_control_dispose_from_old_map);
}

//.text:005D0A60 ; void __cdecl player_control_fade_in_all_input(real32)
//.text:005D0A90 ; void __cdecl player_control_fade_in_all_input_for_player(int32, real32)
//.text:005D0AF0 ; void __cdecl player_control_fade_out_all_input(real32)
//.text:005D0B20 ; void __cdecl player_control_fade_out_all_input_for_player(int32, real32)
//.text:005D0B80 ; bool __cdecl player_control_flags_test_all_players(e_player_control_bit)

const s_player_action_context* __cdecl player_control_get_action_context(int32 user_index)
{
	return INVOKE(0x005D0BD0, player_control_get_action_context, user_index);
}

//.text:005D0C00 ; int32 __cdecl player_control_get_aiming_unit_index(int32)

void __cdecl player_control_get_aiming_vector(int32 user_index, real_vector3d* aiming_vector)
{
	INVOKE(0x005D0C30, player_control_get_aiming_vector, user_index, aiming_vector);
}

void __cdecl player_control_get_controller_input(int32 input_user_index, e_controller_index controller_index, real32 world_seconds_elapsed, real32 game_seconds_elapsed, s_game_input_state** input_states, s_player_control_input* input)
{
	INVOKE(0x005D0C90, player_control_get_controller_input, input_user_index, controller_index, world_seconds_elapsed, game_seconds_elapsed, input_states, input);
}

//.text:005D29E0 ; const real_euler_angles2d* __cdecl player_control_get_facing_angles(int32)
//.text:005D2A10 ; real32 __cdecl player_control_get_field_of_view(int32)
//.text:005D2AA0 ; real32 __cdecl player_control_get_field_of_view_change_time(int32)

bool __cdecl player_control_get_machinima_camera_debug()
{
	//return INVOKE(0x005D2B40, player_control_get_machinima_camera_debug);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_debug;

	return false;
}

bool __cdecl player_control_get_machinima_camera_enabled()
{
	//return INVOKE(0x005D2B90, player_control_get_machinima_camera_enabled);

	if (player_control_machinima_available())
		return player_control_globals->machinima_camera_enabled;

	return false;
}

bool __cdecl player_control_get_machinima_camera_use_old_controls()
{
	//return INVOKE(0x005D2BE0, player_control_get_machinima_camera_use_old_controls);

	if (player_control_get_machinima_camera_enabled())
		return player_control_globals->machinima_camera_old_controls;

	return false;
}

//.text:005D2C30 ; real32 __cdecl player_control_get_target_autoaim_level(int32)
//.text:005D2C80 ; int32 __cdecl player_control_get_target_player_index(int32)
//.text:005D2CE0 ; const s_aim_assist_targeting_result* __cdecl player_control_get_targeting(int32)
//.text:005D2D10 ; void __cdecl player_control_get_unit_camera_info(int32, s_unit_camera_info*)

int16 __cdecl player_control_get_zoom_level(int32 user_index)
{
	return INVOKE(0x005D2D50, player_control_get_zoom_level, user_index);
}

void __cdecl player_control_suppress_rotate_weapons(int32 user_index)
{
	INVOKE(0x005D4860, player_control_suppress_rotate_weapons, user_index);
}

//.text:005D2EA0 ; void __cdecl player_control_handle_weapon_put_away(int32, int16)

void __cdecl player_control_initialize()
{
	INVOKE(0x005D2EE0, player_control_initialize);
}

void __cdecl player_control_initialize_for_new_map()
{
	INVOKE(0x005D3120, player_control_initialize_for_new_map);
}

//.text:005D3290 ; void __cdecl player_control_input_set_unit_index(int32, int32)
//.text:005D3360 ; void __cdecl player_control_lock_gaze(int32, int32, real32)

bool __cdecl player_control_machinima_available()
{
	//return INVOKE(0x005D3400, player_control_machinima_available);

	return game_in_progress() && !game_is_ui_shell() && game_options_get()->game_network_type != _network_game_simulation_sync_server;
}

//.text:005D3430 ; void __cdecl player_control_modify_desired_angles_default(int32, real32, real32, real32, real32)
//.text:005D4350 ; s_player_control_non_deterministic_input_user_state* __cdecl player_control_non_deterministic_input_user_state_get(int32)
//.text:005D4380 ; void __cdecl player_control_output_set_unit_index(int32, int32)
//.text:005D43E0 ; void __cdecl player_control_permanent_impulse(int32, const real_euler_angles2d*)

//void __cdecl player_control_propagate_output(int32)
void __cdecl player_control_propagate_output(int32 input_user_index)
{
	//INVOKE(0x005D4430, player_control_propagate_output, input_user_index);

	s_player_control_input_state* player_control_input = player_control_input_get(input_user_index);

	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		s_player_control_output_state* player_control_output = player_control_output_get(user_index);
		if (player_control_output->unit_index == player_control_input->output.unit_index)
			csmemcpy(&player_control_output->output, &player_control_input->output.output, sizeof(s_player_control_state));
	}
}

//.text:005D44E0 ; void __cdecl player_control_scale_all_input(real32, real32)
//.text:005D45E0 ; void __cdecl player_control_scale_all_input_for_player(int32, real32, real32)
//.text:005D4690 ; void __cdecl player_control_set_deterministic_action_test_flags(int32, uns64)
//.text:005D46C0 ; void __cdecl player_control_set_external_action(int32, const player_action*)

void __cdecl player_control_set_facing(int32 input_user_index, const real_vector3d* facing)
{
	INVOKE(0x005D4700, player_control_set_facing, input_user_index, facing);
}

//.text:005D4760 ; void __cdecl player_control_state_build_action(const s_player_control_state*, player_action*)
//.text:005D4820 ; void __cdecl player_control_state_clear(s_player_control_state*)
//.text:005D4860 ; void __cdecl player_control_suppress_rotate_weapons(int32)
//.text:005D48A0 ; void __cdecl player_control_unlock_gaze(int32)
//.text:005D48E0 ; void __cdecl player_control_unzoom(int32)
//.text:005D4920 ; void __cdecl player_control_unzoom_all()

void __cdecl player_control_update(real32 world_seconds_elapsed, real32 game_seconds_elapsed)
{
	//INVOKE(0x005D4990, player_control_update, world_seconds_elapsed, game_seconds_elapsed);

	//collision_log_begin_period(5);
	//player_control_update_debug_begin();

	uns32 player_mask = 0;
	player_action actions[k_number_of_users]{};

	for (int32 user_index = first_input_user(); user_index != NONE; user_index = next_input_user(user_index))
	{
		int32 player_index = player_mapping_get_player_by_input_user(user_index);
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

	if (!player_control_get_machinima_camera_enabled())
	{
		return;
	}

	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		e_controller_index controller_index = controller_index_from_user_index(user_index);
		s_game_input_state* input_state = nullptr;

		if (VALID_CONTROLLER(controller_index))
		{
			input_abstraction_get_input_state(controller_index, &input_state);
		}

		if (!input_state)
		{
			continue;
		}

		if (input_state->get_button(_button_crouch).down_frames()
			&& input_state->get_button(_button_scope_zoom).down_frames())
		{
			if (input_state->get_button(_button_machinima_camera_control_toggle).down_frames() == 1)
			{
				player_control_globals->machinima_camera_old_controls = !player_control_globals->machinima_camera_old_controls;
				user_interface_play_sound(_ui_global_sound_effect_button_x, NONE);
			}

			if (input_state->get_button(_button_machinima_camera_debug_toggle).down_frames() == 1)
			{
				player_control_globals->machinima_camera_debug = !player_control_globals->machinima_camera_debug;
				user_interface_play_sound(_ui_global_sound_effect_button_x, NONE);
			}
		}
	}
}

//void __cdecl player_control_update_player(int32, int32, e_controller_index, real32, real32)
void __cdecl player_control_update_player(int32 player_index, int32 input_user_index, e_controller_index controller_index, real32 world_seconds_elapsed, real32 game_seconds_elapsed)
{
	INVOKE(0x005D4BF0, player_control_update_player, player_index, input_user_index, controller_index, world_seconds_elapsed, game_seconds_elapsed);
}

//.text:005D5590 ; void __cdecl player_set_pitch_internal(int32, real32, int32)
//.text:005D5670 ; bool __cdecl process_user_interface_input(e_controller_index, s_player_control_input*)
//.text:005D56E0 ; 
//.text:005D5720 ; 
//.text:005D5760 ; 
//.text:005D57F0 ; bool __cdecl scripted_player_control_set_camera_control(bool)
//.text:005D5840 ; 
//.text:005D5870 ; 
//.text:005D58C0 ; 
//.text:005D58F0 ; 
//.text:005D5910 ; real32 __cdecl signed_angular_difference(real32, real32)
//.text:005D5970 ; 
//.text:005D59B0 ; 
//.text:005D59D0 ; 
//.text:005D5A20 ; void __cdecl player_action_test_inhibit_button_flags_set(int32, e_player_control_action_test_bit, bool)
//.text:005D5AB0 ; void __cdecl player_action_test_testing_for_action_flags_set(int32, e_player_control_action_test_bit, bool)
//.text:005D5B40 ; bool __cdecl player_control_action_test(int32, e_player_control_action_test_bit)
//.text:005D5BC0 ; bool __cdecl player_control_action_test_accept(int32)
//.text:005D5C60 ; bool __cdecl player_control_action_test_action(int32)
//.text:005D5D00 ; bool __cdecl player_control_action_test_back(int32)
//.text:005D5D20 ; bool __cdecl player_control_action_test_cancel(int32)
//.text:005D5DC0 ; bool __cdecl player_control_action_test_dpad_down(int32)
//.text:005D5DE0 ; bool __cdecl player_control_action_test_dpad_left(int32)
//.text:005D5E00 ; bool __cdecl player_control_action_test_dpad_right(int32)
//.text:005D5E20 ; bool __cdecl player_control_action_test_dpad_up(int32)
//.text:005D5E40 ; bool __cdecl player_control_action_test_grenade_trigger(int32)
//.text:005D5E60 ; bool __cdecl player_control_action_test_left_shoulder(int32)
//.text:005D5E80 ; bool __cdecl player_control_action_test_look_invert(int32)
//.text:005D5EA0 ; bool __cdecl player_control_action_test_look_relative_all_directions(int32)
//.text:005D5EC0 ; bool __cdecl player_control_action_test_look_relative_down(int32)
//.text:005D5EE0 ; bool __cdecl player_control_action_test_look_relative_left(int32)
//.text:005D5F00 ; bool __cdecl player_control_action_test_look_relative_right(int32)
//.text:005D5F20 ; bool __cdecl player_control_action_test_look_relative_up(int32)
//.text:005D5F40 ; bool __cdecl player_control_action_test_lookstick_backward(int32)
//.text:005D5F60 ; bool __cdecl player_control_action_test_lookstick_forward(int32)
//.text:005D5F80 ; bool __cdecl player_control_action_test_melee(int32)
//.text:005D5FA0 ; bool __cdecl player_control_action_test_move_relative_all_directions(int32)
//.text:005D5FC0 ; bool __cdecl player_control_action_test_range(int32, e_player_control_action_test_bit, e_player_control_action_test_bit)
//.text:005D6060 ; void __cdecl player_control_action_test_reset(int32)
//.text:005D60D0 ; bool __cdecl player_control_action_right_shoulder(int32)
//.text:005D60F0 ; bool __cdecl player_control_action_test_rotate_grenades(int32)
//.text:005D6110 ; bool __cdecl player_control_action_test_rotate_weapons(int32)
//.text:005D6130 ; bool __cdecl player_control_action_test_vision_trigger(int32)
//.text:005D6150 ; bool __cdecl player_control_action_x(int32)
//.text:005D6170 ; bool __cdecl player_control_action_y(int32)
//.text:005D6190 ; void __cdecl player_control_lock_accept_button_until_pressed(int32)
//.text:005D61F0 ; void __cdecl player_control_lock_cancel_button_until_pressed(int32)
//.text:005D6250 ; void __cdecl player_control_lock_y_button_until_pressed(int32)
//.text:005D62B0 ; 
//.text:005D62C0 ; bool __cdecl user_currently_piloting_aircraft(int32)

void __cdecl player_control_get_controller_input_for_jetpack(int32 input_user_index, e_controller_index controller_index, real32 world_seconds_elapsed, real32 game_seconds_elapsed, s_game_input_state** input_states, s_player_control_input* input)
{
	player_control_get_controller_input(input_user_index, controller_index, world_seconds_elapsed, game_seconds_elapsed, input_states, input);

	if (!cheat.jetpack)
		return;

	int32 unit_index = player_control_globals->input_states[controller_index].output.unit_index;
	biped_datum* biped = BIPED_GET(unit_index);
	if (!biped)
		return;

	bool v0 = false;
	if (input_states[controller_index]->get_button(_button_jump).down_frames() && mover_get_motor_program(unit_index) == 1)
	{
		bool v1 = TEST_BIT(biped->unit.unit_control_flags, _unit_control_jump_bit);
		real32 jump_control_ticks = (real32)biped->biped.jump_control_ticks;
		real32 v2 = (real32)game_tick_rate() / 15.0f;
		bool v3 = v2 > jump_control_ticks;
		if ((v1 && v3) || TEST_BIT(biped->unit.unit_control_flags, _unit_control_jetpack_bit))
			v0 = true;
	}

	SET_BIT(input->unit_control_flags, _unit_control_jetpack_bit, v0);
}
HOOK_DECLARE_CALL(0x005D4C66, player_control_get_controller_input_for_jetpack);

