#include "input/input_abstraction.hpp"

#include "cseries/cseries_windows.hpp"
#include "input/controllers.hpp"
#include "input/input_windows.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0244C9D0, s_input_abstraction_globals, input_abstraction_globals);

//HOOK_DECLARE(0x0060B3A0, input_abstraction_dispose);
//HOOK_DECLARE(0x0060B3C0, input_abstraction_dispose_from_old_map);
//HOOK_DECLARE(0x0060B3D0, sub_60B3D0);
HOOK_DECLARE(0x0060BE20, input_abstraction_get_abstract_stick_pitch);
HOOK_DECLARE(0x0060BE60, input_abstraction_get_controls_method);
//HOOK_DECLARE(0x0060BE70, input_abstraction_get_controller_preferences);
//HOOK_DECLARE(0x0060BEA0, sub_60BEA0);
HOOK_DECLARE(0x0060BF00, input_abstraction_get_default_preferences);
HOOK_DECLARE(0x0060BFE0, input_abstraction_get_input_state);
HOOK_DECLARE(0x0060C000, input_abstraction_get_player_look_angular_velocity);
HOOK_DECLARE(0x0060C040, sub_60C040);
//HOOK_DECLARE(0x0060C390, input_abstraction_initialize);
//HOOK_DECLARE(0x0060C430, input_abstraction_initialize_for_new_map);
//HOOK_DECLARE(0x0060C4A0, sub_60C4A0);
//HOOK_DECLARE(0x0060C6D0, sub_60C6D0);
HOOK_DECLARE(0x0060CE40, input_abstraction_latch_all_buttons);
//HOOK_DECLARE(0x0060CE70, sub_60CE70);
//HOOK_DECLARE_CALL(0x0060D9AA, sub_60D160); //HOOK_DECLARE(0x0060D160, sub_60D160);
HOOK_DECLARE(0x0060D620, sub_60D620);
HOOK_DECLARE(0x0060D7A0, input_abstraction_reset_controller_detection_timer);
//HOOK_DECLARE(0x0060D7B0, input_should_suppress_rumble);
HOOK_DECLARE(0x0060D830, input_abstraction_set_controller_preferences);
//HOOK_DECLARE(0x0060D880, input_abstraction_update);
//HOOK_DECLARE(0x0060D9E0, input_abstraction_update_device_changes);

bool use_mean_look_sensitivity = false;

void __cdecl input_abstraction_dispose()
{
	//INVOKE(0x0060B3A0, input_abstraction_dispose);

	csmemset(&input_abstraction_globals, 0, sizeof(s_input_abstraction_globals));
}

void __cdecl input_abstraction_dispose_from_old_map()
{
	//INVOKE(0x0060B3C0, input_abstraction_dispose_from_old_map);

	input_abstraction_globals.input_globals = nullptr;
}

void __cdecl sub_60B3D0(gamepad_state* state, s_gamepad_input_preferences* preferences, s_game_input_state* input_state, int32 gamepad_index)
{
	INVOKE(0x0060B3D0, sub_60B3D0, state, preferences, input_state, gamepad_index);
}

int16 __cdecl input_abstraction_get_abstract_stick_pitch(int32 controller_index)
{
	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);

	point2d* abstract_sticks = input_abstraction_globals.input_states[controller_index].abstract_sticks;
	int16 joystick_preset = input_abstraction_globals.preferences[controller_index].keyboard_preferences.joystick_preset;

	return abstract_sticks[(joystick_preset == 1 || joystick_preset == 3) ? 0 : 1].y;
}

int32 __cdecl input_abstraction_get_controls_method()
{
	return input_abstraction_globals.controls_method;
}

void __cdecl input_abstraction_get_controller_preferences(int32 controller_index, s_gamepad_input_preferences* preferences)
{
	//INVOKE(0x0060BE70, input_abstraction_get_controller_preferences, controller_index, preferences);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
	ASSERT(preferences);

	csmemcpy(preferences, &input_abstraction_globals.preferences[controller_index], sizeof(s_gamepad_input_preferences));
}

void __cdecl sub_60BEA0(s_gamepad_input_preferences* preferences, void* bindings)
{
	INVOKE(0x0060BEA0, sub_60BEA0, preferences, bindings);
}

void __cdecl input_abstraction_get_default_preferences(s_gamepad_input_preferences* preferences)
{
	HOOK_INVOKE(, input_abstraction_get_default_preferences, preferences);

	//preferences->look_sensitivity_x = 120.0f;
	//preferences->look_sensitivity_y = 60.0f;
	//
	//csmemset(preferences->gamepad_buttons.m_storage, 0xFF, sizeof(preferences->gamepad_buttons));
	//csmemset(preferences->gamepad_buttons_held.m_storage, false, sizeof(preferences->gamepad_buttons_held));
	//
	//for (int32 gamepad_button_index = 0; gamepad_button_index < 16; gamepad_button_index++)
	//	preferences->gamepad_buttons[gamepad_button_index] = gamepad_button_index;
	//
	//// $TODO 
	//preferences->gamepad_buttons[] = ;
	//
	//preferences->joystick_preset = 0;
	//preferences->controller_look_inverted = 0;
	//preferences->controller_flight_stick_aircraft_controls = 0;
	//preferences->camera_panning = 0;
	//preferences->camera_flying_movement = 1.0;
	//preferences->camera_flying_thrust = 1.0;
}

//.text:0060BFC0 ; gamepad_state *sub_60BFC0()

void __cdecl input_abstraction_get_input_state(int32 controller_index, s_game_input_state** input_state)
{
	//INVOKE(0x0060BFE0, input_abstraction_get_input_state, controller_index, input_state);

	ASSERT(input_state);
	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);

	*input_state = &input_abstraction_globals.input_states[controller_index];
}

void __cdecl input_abstraction_get_player_look_angular_velocity(int32 controller_index, real_euler_angles2d* angular_velocity)
{
	//INVOKE(0x0060C000, input_abstraction_get_player_look_angular_velocity, controller_index, angular_velocity);

	real32 look_sensitivity_x = input_abstraction_globals.preferences[controller_index].look_sensitivity_x;
	real32 look_sensitivity_y = input_abstraction_globals.preferences[controller_index].look_sensitivity_y;

	if (use_mean_look_sensitivity)
	{
		real32 mean_look_sensitivity = (look_sensitivity_x + look_sensitivity_y) / 2;
		look_sensitivity_x = mean_look_sensitivity;
		look_sensitivity_y = mean_look_sensitivity;
	}

	angular_velocity->yaw = look_sensitivity_x * DEG;
	angular_velocity->pitch = look_sensitivity_y * DEG;
}

void __cdecl sub_60C040(int32 keyboard_preset, s_gamepad_input_preferences* preferences)
{
	HOOK_INVOKE(, sub_60C040, keyboard_preset, preferences);

	c_enum<e_input_key_code, int16, _key_escape, NUMBER_OF_KEYS>& reload_left_button = preferences->keyboard_preferences.keys_primary[_button_action_reload_left];
	c_enum<e_input_key_code, int16, _key_escape, NUMBER_OF_KEYS>& pick_up_left_button = preferences->keyboard_preferences.keys_primary[_button_action_pick_up_left];
	c_enum<e_mouse_button, int8, _mouse_button_1, k_total_mouse_button_count>& fire_left_button = preferences->keyboard_preferences.mouse_buttons_primary[_button_action_fire_left];

	reload_left_button = _key_z;
	pick_up_left_button = _key_z;
	fire_left_button = _mouse_button_right_click;

	printf("");

	//for (int32 i = 0; i < k_button_action_count_keyboard; i++)
	//{
	//	preferences->keyboard_preferences.keys_primary[i] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[i] = k_mouse_button_none;
	//	preferences->keyboard_preferences.keys_primary[i] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[i] = k_mouse_button_none;
	//}
	//
	//preferences->keyboard_preferences.keys_primary[         _button_machinima_lower_weapon_toggle] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[        _button_machinima_camera_enable_toggle] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[_button_action_machinima_camera_control_toggle] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[  _button_action_machinima_camera_debug_toggle] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                              _button_action_a] = _key_space;
	//preferences->keyboard_preferences.keys_primary[                              _button_action_b] = _key_q;
	//preferences->keyboard_preferences.keys_primary[                          _button_action_start] = _key_escape;
	//preferences->keyboard_preferences.keys_primary[                           _button_action_back] = _key_tab;
	//preferences->keyboard_preferences.keys_primary[                     _button_action_voice_chat] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_unknown42] = _key_space;
	//preferences->keyboard_preferences.keys_primary[                        _button_action_dpad_up] = _key_up_arrow;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_dpad_down] = _key_down_arrow;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_dpad_left] = _key_left_arrow;
	//preferences->keyboard_preferences.keys_primary[                     _button_action_dpad_right] = _key_right_arrow;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_unknown31] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_lean_left] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                   _button_action_general_chat] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                      _button_action_team_chat] = _key_not_a_key;
	//preferences->keyboard_preferences.keys_primary[                  _button_action_vehicle_boost] = _key_left_shift;
	//preferences->keyboard_preferences.keys_primary[                   _button_action_vehicle_dive] = _key_left_control;
	//preferences->keyboard_preferences.keys_alternative[         _button_action_vehicle_accelerate] = _key_up_arrow;
	//preferences->keyboard_preferences.keys_alternative[            _button_action_vehicle_reverse] = _key_down_arrow;
	//preferences->keyboard_preferences.keys_primary[                  _button_action_vehicle_raise] = _key_space;
	//preferences->keyboard_preferences.mouse_buttons_alternative[      _button_action_vehicle_dive] = _mouse_button_wheel_down;
	//preferences->keyboard_preferences.mouse_buttons_alternative[     _button_action_vehicle_raise] = _mouse_button_wheel_up;
	//preferences->keyboard_preferences.mouse_buttons_primary[          _button_action_vehicle_fire] = _mouse_button_left_click;
	//preferences->keyboard_preferences.mouse_buttons_primary[      _button_action_vehicle_alt_fire] = _mouse_button_right_click;
	//
	//switch (keyboard_preset)
	//{
	//case 0:
	//case 3:
	//{
	//	preferences->keyboard_preferences.keys_alternative[                  _button_action_melee] = _key_v;
	//	preferences->keyboard_preferences.keys_primary[                _button_action_vehicle_use] = _key_e;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_melee_fire] = _key_q;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_lean_right] = _key_left_shift;
	//	preferences->keyboard_preferences.mouse_buttons_primary[     _button_action_throw_grenade] = preferences->keyboard_preferences.mouse_buttons_primary[_button_action_fire_left];
	//	preferences->keyboard_preferences.keys_alternative[             _button_action_melee_fire] = _key_v;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_unknown58] = _key_space;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_fire_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_fire_left] = _key_f;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_jump] = _key_space;
	//	preferences->keyboard_preferences.keys_primary[            _button_action_switch_grenades] = _key_g;
	//	preferences->keyboard_preferences.keys_primary[                      _button_action_melee] = _key_q;
	//	preferences->keyboard_preferences.keys_primary[              _button_action_throw_grenade] = _key_f;
	//	preferences->keyboard_preferences.mouse_buttons_primary[        _button_action_fire_right] = _mouse_button_left_click;
	//	preferences->keyboard_preferences.mouse_buttons_primary[    _button_action_switch_weapons] = _mouse_button_wheel_up;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[_button_action_switch_weapons] = _mouse_button_wheel_down;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[     _button_action_fire_left] = _mouse_button_middle_click;
	//	preferences->keyboard_preferences.keys_primary[               _button_action_reload_right] = _key_r;
	//	preferences->keyboard_preferences.keys_primary[                        _button_action_use] = _key_e;
	//	preferences->keyboard_preferences.keys_alternative[           _button_action_reload_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_alternative[                    _button_action_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_alternative[            _button_action_vehicle_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[      _button_action_reload_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[               _button_action_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[       _button_action_vehicle_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[  _button_action_reload_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[           _button_action_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_alternative[   _button_action_vehicle_use] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[                     _button_action_crouch] = _key_left_control;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_zoom] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[           _button_action_use_consumable_1] = _key_1;
	//	preferences->keyboard_preferences.keys_primary[           _button_action_use_consumable_2] = _key_2;
	//	preferences->keyboard_preferences.keys_primary[           _button_action_use_consumable_3] = _key_3;
	//	preferences->keyboard_preferences.keys_primary[           _button_action_use_consumable_4] = _key_4;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_team_chat] = _key_y;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_voice_chat] = _key_caps_lock;
	//	preferences->keyboard_preferences.keys_primary[               _button_action_general_chat] = _key_t;
	//	preferences->keyboard_preferences.mouse_buttons_primary[              _button_action_zoom] = _mouse_button_right_click;
	//	preferences->keyboard_preferences.keys_primary[               _button_action_move_forward] = _key_w;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_back] = _key_s;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_left] = _key_a;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_move_right] = _key_d;
	//	preferences->keyboard_preferences.keys_alternative[           _button_action_move_forward] = _key_up_arrow;
	//	preferences->keyboard_preferences.keys_alternative[              _button_action_move_back] = _key_down_arrow;
	//	preferences->keyboard_preferences.keys_alternative[              _button_action_move_left] = _key_left_arrow;
	//	preferences->keyboard_preferences.keys_alternative[             _button_action_move_right] = _key_right_arrow;
	//	preferences->keyboard_preferences.keys_alternative[          _button_action_throw_grenade] = preferences->keyboard_preferences.keys_alternative[_button_action_fire_left];
	//	preferences->keyboard_preferences.mouse_buttons_alternative[ _button_action_throw_grenade] = _mouse_button_middle_click;
	//	preferences->keyboard_preferences.keys_primary[                _button_action_next_player] = _key_d;
	//	preferences->keyboard_preferences.keys_primary[                _button_action_prev_player] = _key_a;
	//}
	//break;
	//case 1:
	//{
	//	preferences->keyboard_preferences.keys_primary[               _button_action_reload_right] = _key_t;
	//	preferences->keyboard_preferences.keys_primary[            _button_action_switch_grenades] = _key_e;
	//	preferences->keyboard_preferences.keys_primary[             _button_action_switch_weapons] = _key_tab;
	//	preferences->keyboard_preferences.keys_primary[                      _button_action_melee] = _key_f;
	//	preferences->keyboard_preferences.keys_primary[              _button_action_throw_grenade] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[        _button_action_fire_right] = _mouse_button_left_click;
	//	preferences->keyboard_preferences.mouse_buttons_primary[         _button_action_fire_left] = _mouse_button_right_click;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_flashlight] = _key_x;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_zoom] = _key_left_shift;
	//	preferences->keyboard_preferences.keys_primary[                     _button_action_crouch] = _key_z;
	//	preferences->keyboard_preferences.keys_primary[               _button_action_move_forward] = _key_w;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_back] = _key_s;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_left] = _key_a;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_move_right] = _key_d;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_melee_fire] = _key_f;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_jump] = _key_space;
	//	preferences->keyboard_preferences.keys_primary[                _button_action_reload_left] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[    _button_action_switch_weapons] = _mouse_button_7;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_fire_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_fire_left] = _key_not_a_key;
	//}
	//break;
	//case 2:
	//{
	//	preferences->keyboard_preferences.keys_primary[               _button_action_reload_right] = _key_u;
	//	preferences->keyboard_preferences.keys_primary[            _button_action_switch_grenades] = _key_g;
	//	preferences->keyboard_preferences.keys_primary[             _button_action_switch_weapons] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[                      _button_action_melee] = _key_h;
	//	preferences->keyboard_preferences.keys_primary[              _button_action_throw_grenade] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[        _button_action_fire_right] = _mouse_button_right_click;
	//	preferences->keyboard_preferences.mouse_buttons_primary[         _button_action_fire_left] = _mouse_button_left_click;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_flashlight] = _key_alt;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_zoom] = _key_left_shift;
	//	preferences->keyboard_preferences.keys_primary[                     _button_action_crouch] = _key_n;
	//	preferences->keyboard_preferences.keys_primary[               _button_action_move_forward] = _key_i;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_back] = _key_k;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_move_left] = _key_j;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_move_right] = _key_l;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_melee_fire] = _key_h;
	//	preferences->keyboard_preferences.keys_primary[                       _button_action_jump] = _key_space;
	//	preferences->keyboard_preferences.keys_primary[                _button_action_reload_left] = _key_not_a_key;
	//	preferences->keyboard_preferences.mouse_buttons_primary[    _button_action_switch_weapons] = _mouse_button_7;
	//	preferences->keyboard_preferences.keys_primary[                 _button_action_fire_right] = _key_not_a_key;
	//	preferences->keyboard_preferences.keys_primary[                  _button_action_fire_left] = _key_not_a_key;
	//}
	//break;
	//}
}

void __cdecl input_abstraction_initialize()
{
	//INVOKE(0x0060C390, input_abstraction_initialize);

	csmemset(&input_abstraction_globals, 0, sizeof(s_input_abstraction_globals));

	for (e_controller_index controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
	{
		input_abstraction_get_default_preferences(&input_abstraction_globals.preferences[controller_index]);
		input_abstraction_globals.input_has_gamepad[controller_index] = input_has_gamepad(static_cast<int16>(controller_index));

		for (int32 button_index = 0; button_index < k_controller_button_count; button_index++)
		{
			e_button_action button_action = static_cast<e_button_action>(button_index);
			input_abstraction_globals.input_states[controller_index].get_button(button_action).unlock();
		}
	}

	input_abstraction_reset_controller_detection_timer();
	input_abstraction_globals.input_device_changed = true;
}

void __cdecl input_abstraction_initialize_for_new_map()
{
	INVOKE(0x0060C430, input_abstraction_initialize_for_new_map);
}

void __cdecl sub_60C4A0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
{
	//INVOKE(0x0060C4A0, sub_60C4A0, preferences, input_state);

	for (int32 i = 0; i < k_button_action_count_keyboard * 2; i++)
	{
		int32 key_index = i % k_button_action_count_keyboard;

		e_input_key_code key_code = (i < k_button_action_count_keyboard ? preferences->keyboard_preferences.keys_primary : preferences->keyboard_preferences.keys_alternative)[key_index];
		if ((int16)key_code != 0xFF)
		{
			e_input_type input_type = _input_type_game;
			if (i == _key_f6 || i == _key_f7)
				input_type = _input_type_special;

			if (i == _key_left_shift || i == _key_return)
				input_type = _input_type_ui;

			if (i == _key_z)
				input_type = _input_type_ui;

			uns16 down_msec = MAX(input_state->abstract_buttons[key_index].down_msec(), input_key_msec_down(key_code, input_type));
			uns8 down_frames = MAX(input_state->abstract_buttons[key_index].down_frames(), input_key_frames_down(key_code, input_type));
			uns8 down_amount = MAX((uns8)input_state->abstract_buttons[key_index].down_amount(), input_key_frames_down(key_code, input_type));

			input_state->abstract_buttons[key_index].update(down_msec, down_frames, -(down_amount != 0));
		}
	}
}

void __cdecl sub_60C6D0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state) // this is for setting editor controls
{
	INVOKE(0x0060C6D0, sub_60C6D0, preferences, input_state);

//#define UPDATE_BUTTON_KEY(TYPE, KEY, ACTION) \
//{ \
//	uns16 msec_down = input_key_msec_down(_key_code_##KEY, _input_type_##TYPE); \
//	uns8 frames_down = input_key_frames_down(_key_code_##KEY, _input_type_##TYPE); \
//	input_state->abstract_buttons[_button_action_##ACTION].update(msec_down, frames_down, -(frames_down != 0)); \
//}
//#define UPDATE_BUTTON_MOUSE(TYPE, MOUSE, ACTION) \
//{ \
//	uns16 msec_down = input_mouse_msec_down(_mouse_button_##MOUSE, _input_type_##TYPE); \
//	uns8 frames_down = input_mouse_frames_down(_mouse_button_##MOUSE, _input_type_##TYPE); \
//	input_state->abstract_buttons[_button_action_##ACTION].update(msec_down, frames_down, -(frames_down != 0)); \
//}
//
//#define UPDATE_BUTTON_MOUSE_AND_KEY(TYPE, MOUSE, KEY, ACTION) \
//{ \
//	uns16 msec_down = FLOOR(input_mouse_msec_down(_mouse_button_##MOUSE, _input_type_##TYPE), input_key_msec_down(_key_code_##KEY, _input_type_##TYPE)); \
//	uns8 frames_down = FLOOR(input_mouse_frames_down(_mouse_button_##MOUSE, _input_type_##TYPE), input_key_frames_down(_key_code_##KEY, _input_type_##TYPE)); \
//	input_state->abstract_buttons[_button_action_##ACTION].update(msec_down, frames_down, -(frames_down != 0)); \
//}
//
//	UPDATE_BUTTON_KEY(          ui,            w,   move_forward);
//	UPDATE_BUTTON_KEY(          ui,            s,      move_back);
//	UPDATE_BUTTON_KEY(          ui,            a,      move_left);
//	UPDATE_BUTTON_KEY(          ui,            d,     move_right);
//	UPDATE_BUTTON_KEY(          ui,     spacebar,              a);
//	UPDATE_BUTTON_KEY(          ui,            v,              b);
//	UPDATE_BUTTON_KEY(          ui,            f,              x);
//	UPDATE_BUTTON_KEY(          ui,            r,              y);
//	UPDATE_BUTTON_KEY(          ui,            x,      dpad_left);
//	UPDATE_BUTTON_KEY(          ui,            c,     dpad_right);
//	UPDATE_BUTTON_KEY(          ui,            q,   right_bumper);
//	UPDATE_BUTTON_KEY(          ui,            e,    left_bumper);
//	UPDATE_BUTTON_KEY(          ui,      control,     left_stick);
//	UPDATE_BUTTON_MOUSE_AND_KEY(ui, middle_click, z, right_stick);
//	UPDATE_BUTTON_MOUSE(        ui,  right_click,   left_trigger);
//	UPDATE_BUTTON_MOUSE(        ui,   left_click,  right_trigger);
//	UPDATE_BUTTON_KEY(     special,    page_down,          start);
//	UPDATE_BUTTON_KEY(     special,      page_up,           back);
//
//	for (int32 i = _button_action_jump; i < k_button_action_count; i++)
//		input_state->abstract_buttons[i].set_accessor(preferences->gamepad_buttons[i]);
//
//#undef UPDATE_BUTTON_MOUSE_AND_KEY
//#undef UPDATE_BUTTON_MOUSE
//#undef UPDATE_BUTTON_KEY
}

void __cdecl input_abstraction_latch_all_buttons(int32 controller_index)
{
	//INVOKE(0x0060CE40, input_abstraction_latch_all_buttons, controller_index);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);

	for (int32 button_index = 0; button_index < k_button_action_count; button_index++)
	{
		e_button_action button_action = static_cast<e_button_action>(button_index);
		input_abstraction_globals.input_states[controller_index].get_button(button_action).latch();
	}
}

void __cdecl sub_60CE70(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
{
	//INVOKE(0x0060CE70, sub_60CE70, preferences, input_state);

	//for (int32 i = 0; i < k_button_action_count * 2; i++)
	//{
	//	int32 mouse_button_index = i % k_button_action_count;
	//
	//	e_mouse_button mouse_button = (i < k_button_action_count ? preferences->keyboard_preferences.mouse_buttons_primary : preferences->keyboard_preferences.mouse_buttons_alternative)[mouse_button_index];
	//	if (mouse_button < k_mouse_button_count)
	//	{
	//		uns16 down_msec = MAX(input_state->abstract_buttons[mouse_button_index].down_msec(), input_mouse_msec_down(mouse_button, _input_type_game));
	//		uns8 down_frames = MAX(input_state->abstract_buttons[mouse_button_index].down_frames(), input_mouse_frames_down(mouse_button, _input_type_game));
	//		uns8 down_amount = MAX((uns8)input_state->abstract_buttons[mouse_button_index].down_amount(), input_mouse_frames_down(mouse_button, _input_type_game));
	//
	//		input_state->abstract_buttons[mouse_button_index].update(down_msec, down_frames, down_amount);
	//	}
	//	else
	//	{
	//		mouse_state* state = input_get_mouse_state(_input_type_ui);
	//		if (state && !input_type_suppressed(_input_type_game))
	//		{
	//			if (mouse_button == _mouse_button_wheel_up)
	//			{
	//				if (state->wheel_ticks <= 0)
	//					continue;
	//			}
	//			else if (mouse_button != _mouse_button_wheel_down || state->wheel_ticks >= 0)
	//				continue;
	//		}
	//
	//		uns8 down_frames = MAX(input_state->abstract_buttons[mouse_button_index].down_frames(), 1);
	//		uns16 down_msec = MAX(input_state->abstract_buttons[mouse_button_index].down_msec(), 1);
	//		uns8 down_amount = 255;
	//
	//		input_state->abstract_buttons[mouse_button_index].update(down_msec, down_frames, down_amount);
	//	}
	//}

	for (int32 i = 0; i < k_button_action_count * 2; i++)
	{
		e_button_action mouse_button_index = e_button_action((i < k_button_action_count) ? i : i - k_button_action_count);

		e_mouse_button mouse_button = preferences->keyboard_preferences.mouse_buttons_primary[mouse_button_index];
		if (i >= k_button_action_count)
		{
			mouse_button = preferences->keyboard_preferences.mouse_buttons_alternative[mouse_button_index];
		}

		c_abstract_button& abstract_button = input_state->get_button(mouse_button_index);

		if (mouse_button < k_mouse_button_count)
		{
			uns16 down_msec = abstract_button.down_msec();
			uns16 frames_down = input_mouse_frames_down(mouse_button, _input_type_game);
			uns16 msec_down = input_mouse_msec_down(mouse_button, _input_type_game);
			real32 down_amount = abstract_button.down_amount();

			if (down_msec <= msec_down)
			{
				down_msec = msec_down;
			}

			if (abstract_button.down_frames() > frames_down)
			{
				abstract_button.update(down_msec, abstract_button.down_frames(), -static_cast<uns8>(down_amount != 0));
			}
			else
			{
				abstract_button.update(down_msec, frames_down, 255);
			}
		}
		else
		{
			mouse_state* state = input_get_mouse_state(_input_type_ui);
			if (state && !input_type_suppressed(_input_type_game))
			{
				if ((mouse_button == _mouse_button_wheel_up && state->wheel_ticks > 0) ||
					(mouse_button == _mouse_button_wheel_down && state->wheel_ticks < 0))
				{
					uns16 down_frames = abstract_button.is_down() ? abstract_button.down_frames() : 1;
					uns16 down_msec = abstract_button.down_msec() ? abstract_button.down_msec() : 1;

					abstract_button.update(down_msec, down_frames, 255);
				}
			}
		}
	}
}

void __cdecl sub_60D160(mouse_state* state, s_game_input_state* input_state, int32 a3)
{
	INVOKE(0x0060D160, sub_60D160, state, input_state, a3);
}

void __cdecl sub_60D620(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
{
	//INVOKE(0x0060D620, sub_60D620, preferences, input_state);

	input_state->aim_instantaneously = true;
	input_state->aircraft_pitch = input_state->aircraft_pitch1;

	uns8 forward_frames = input_state->abstract_buttons[_button_action_move_forward].down_frames();
	uns8 back_frames = input_state->abstract_buttons[_button_action_move_back].down_frames();
	uns8 left_frames = input_state->abstract_buttons[_button_action_move_left].down_frames();
	uns8 right_frames = input_state->abstract_buttons[_button_action_move_right].down_frames();
	uns8 vehicle_accelerate_frames = input_state->abstract_buttons[_button_action_vehicle_accelerate].down_frames();
	uns8 vehicle_reverse_frames = input_state->abstract_buttons[_button_action_vehicle_reverse].down_frames();

	input_state->forward_movement = real32((forward_frames != 0) - (back_frames != 0));
	input_state->strafe = real32((left_frames != 0) - (right_frames != 0));
	input_state->vehicle_forward_movement = real32((vehicle_accelerate_frames != 0) - (vehicle_reverse_frames != 0));
}

void __cdecl input_abstraction_reset_controller_detection_timer()
{
	input_abstraction_globals.controller_detection_timer = system_milliseconds();
}

void __cdecl input_should_suppress_rumble(int32 controls_method)
{
	INVOKE(0x0060D7B0, input_should_suppress_rumble, controls_method);
}

void __cdecl input_abstraction_set_controller_preferences(int32 controller_index, s_gamepad_input_preferences* preferences)
{
	//INVOKE(0x0060D830, input_abstraction_set_controller_preferences, controller_index, preferences);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
	ASSERT(preferences);
	ASSERT(preferences->gamepad_buttons[_button_action_start] == _controller_button_start && preferences->gamepad_buttons[_button_action_back] == _controller_button_back, "invalid controller preferences; can't remap start & back buttons");

	for (int32 button_index = 0; button_index < 16; button_index++)
	{
		e_button_action button_action = static_cast<e_button_action>(button_index);
		input_abstraction_globals.input_states[controller_index].get_button(button_action).unlock();
	}

	csmemcpy(&input_abstraction_globals.preferences[controller_index], preferences, sizeof(s_gamepad_input_preferences));
}

void __cdecl input_abstraction_update()
{
	INVOKE(0x0060D880, input_abstraction_update);
}

void __cdecl input_abstraction_update_device_changes(uns32 flags)
{
	INVOKE(0x0060D9E0, input_abstraction_update_device_changes, flags);
}

c_abstract_button::c_abstract_button() :
	m_down_msec(),
	m_down_frames(),
	m_flags(),
	m_down_amount(),
	m_accessor(-1),
	m_locked(-1)
{
}

bool c_abstract_button::access_valid() const
{
	return m_locked == 0xFF || m_locked == m_accessor;
}

real32 c_abstract_button::down_amount()
{
	if (c_abstract_button::latched() || !c_abstract_button::access_valid())
	{
		return 0.0f;
	}

	return m_down_amount;
}

uns8 c_abstract_button::down_frames() const
{
	if (c_abstract_button::latched() || !c_abstract_button::access_valid())
	{
		return 0;
	}

	return m_down_frames;
}

uns16 c_abstract_button::down_msec()
{
	if (c_abstract_button::latched() || !c_abstract_button::access_valid())
	{
		return 0;
	}

	return m_down_msec;
}

bool c_abstract_button::is_down()
{
	return c_abstract_button::down_frames() != 0;
}

void c_abstract_button::latch()
{
	c_abstract_button::set_latch_bit(true);
}

bool c_abstract_button::latched() const
{
	return TEST_BIT(m_flags, _abstract_button_latch_bit);
}

void c_abstract_button::lock()
{
	ASSERT(access_valid());
	m_locked = m_accessor;
}

bool c_abstract_button::locked()
{
	return (m_locked + 1) == 0;
}

void c_abstract_button::set_accessor(e_button_action accessor)
{
	m_accessor = (uns8)(accessor);
}

void c_abstract_button::set_latch_bit(bool set_bit)
{
	SET_BIT(m_flags, _abstract_button_latch_bit, set_bit);
}

void c_abstract_button::update(uns16 down_msec, uns16 down_frames, uns8 down_amount)
{
	m_down_msec = down_msec;
	m_down_frames = (uns8)(down_frames);
	m_down_amount = (real32)(down_amount / 255);

	if (!m_down_frames && c_abstract_button::latched())
	{
		c_abstract_button::set_latch_bit(false);
	}
}

void c_abstract_button::unlock()
{
	m_locked = -1;
}

c_abstract_button& s_game_input_state::get_button(e_button_action button_index)
{
	return abstract_buttons[button_index];
}

const c_abstract_button& s_game_input_state::get_button(e_button_action button_index) const
{
	return abstract_buttons[button_index];
}

void input_abstraction_get_raw_data_string(char* buffer, int16 size)
{
	ASSERT(buffer);
	ASSERT(size > 0);

	if (!buffer || size <= 0)
	{
		return;
	}

	csnzappendf(buffer, size, "|n|n|n|ninput_abstraction|n");
	for (int16 i = 0; i < k_number_of_controllers; i++)
	{
		s_game_input_state& input_state = input_abstraction_globals.input_states[i];

		if (!input_state.aim_instantaneously)
		{
			continue;
		}

		csnzappendf(buffer, size, "%hd,   sticks: (left: %hd, %hd), (right: %hd, %hd)]|n",
			i,
			input_state.abstract_sticks[0].x, input_state.abstract_sticks[0].y,
			input_state.abstract_sticks[1].x, input_state.abstract_sticks[1].y);

		csnzappendf(buffer, size, "%hd, movement: (forward: %f, strafe: %f, vehicle forward: %f)|n",
			i,
			input_state.forward_movement,
			input_state.strafe,
			input_state.vehicle_forward_movement);

		csnzappendf(buffer, size, "%hd,      yaw: (%f, vehicle: %f)|n",
			i,
			input_state.yaw, input_state.vehicle_yaw);

		csnzappendf(buffer, size, "%hd,    pitch: (%f, vehicle: %f, %f, aircraft: %f, %f)|n",
			i, input_state.pitch,
			input_state.vehicle_pitch, input_state.vehicle_pitch1,
			input_state.aircraft_pitch, input_state.aircraft_pitch1);
	}
}

