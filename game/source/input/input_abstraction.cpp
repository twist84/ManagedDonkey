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
HOOK_DECLARE(0x0060C040, input_abstraction_get_preset_preferences);
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

bool use_mean_look_rate = false;

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
	int16 joystick_preset = input_abstraction_globals.preferences[controller_index].joystick_preset;

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

	printf("");

	//preferences->yaw_rate = 120.0f;
	//preferences->pitch_rate = 60.0f;
	//
	//csmemset(preferences->control_to_gamepad_button_mapping, NONE, sizeof(preferences->control_to_gamepad_button_mapping));
	//csmemset(preferences->control_to_gamepad_button_alternative_mapping, 0, sizeof(preferences->control_to_gamepad_button_alternative_mapping));
	//
	//for (int32 gamepad_button_index = 0; gamepad_button_index < NUMBER_OF_GAMEPAD_BUTTONS; gamepad_button_index++)
	//	preferences->control_to_gamepad_button_mapping[gamepad_button_index] = gamepad_button_index;
	//
	//// $TODO 
	//preferences->control_to_gamepad_button_mapping[] = ;
	//
	//preferences->joystick_preset = 0;
	//preferences->controller_look_inverted = 0;
	//preferences->controller_flight_stick_aircraft_controls = 0;
	//preferences->camera_panning = 0;
	//preferences->camera_flying_movement = 1.0f;
	//preferences->camera_flying_thrust = 1.0f;
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

	real32 yaw_rate = input_abstraction_globals.preferences[controller_index].yaw_rate;
	real32 pitch_rate = input_abstraction_globals.preferences[controller_index].pitch_rate;

	if (use_mean_look_rate)
	{
		real32 mean_look_rate = (yaw_rate + pitch_rate) / 2;
		yaw_rate = mean_look_rate;
		pitch_rate = mean_look_rate;
	}

	angular_velocity->yaw = yaw_rate * DEG;
	angular_velocity->pitch = pitch_rate * DEG;
}

void __cdecl input_abstraction_get_preset_preferences(int32 keyboard, s_gamepad_input_preferences* preferences)
{
	HOOK_INVOKE(, input_abstraction_get_preset_preferences, keyboard, preferences);

	int16& reload_left_button = preferences->control_to_keyboard_mapping[_button_reload_left];
	int16& pick_up_left_button = preferences->control_to_keyboard_mapping[_button_pick_up_left];
	uns8& fire_left_button = preferences->control_to_mouse_mapping[_button_fire_secondary];

	reload_left_button = _key_z;
	pick_up_left_button = _key_z;
	fire_left_button = _mouse_button_right_click;

	//for (int32 i = 0; i < NUMBER_OF_KEYBOARD_CONTROL_BUTTONS; i++)
	//{
	//	preferences->control_to_keyboard_mapping[i] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[i] = k_mouse_button_none;
	//	preferences->control_to_keyboard_mapping[i] = _key_not_a_key;
	//	preferences->control_to_mouse_alternative_mapping[i] = k_mouse_button_none;
	//}
	//
	//preferences->control_to_keyboard_mapping[             _button_machinima_lower_weapon_toggle   ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_machinima_camera_enable_toggle  ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_machinima_camera_control_toggle ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_machinima_camera_debug_toggle   ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_a                               ] = _key_space;
	//preferences->control_to_keyboard_mapping[             _button_b                               ] = _key_q;
	//preferences->control_to_keyboard_mapping[             _button_start                           ] = _key_escape;
	//preferences->control_to_keyboard_mapping[             _button_back                            ] = _key_tab;
	//preferences->control_to_keyboard_mapping[             _button_voice_chat                      ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_unknown42                       ] = _key_space;
	//preferences->control_to_keyboard_mapping[             _button_dpad_up                         ] = _key_up_arrow;
	//preferences->control_to_keyboard_mapping[             _button_dpad_down                       ] = _key_down_arrow;
	//preferences->control_to_keyboard_mapping[             _button_dpad_left                       ] = _key_left_arrow;
	//preferences->control_to_keyboard_mapping[             _button_dpad_right                      ] = _key_right_arrow;
	//preferences->control_to_keyboard_mapping[             _button_unknown31                       ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_lean_left                       ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_general_chat                    ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_team_chat                       ] = _key_not_a_key;
	//preferences->control_to_keyboard_mapping[             _button_vehicle_boost                   ] = _key_left_shift;
	//preferences->control_to_keyboard_mapping[             _button_vehicle_dive                    ] = _key_left_control;
	//preferences->control_to_keyboard_alternative_mapping[ _button_vehicle_accelerate              ] = _key_up_arrow;
	//preferences->control_to_keyboard_alternative_mapping[ _button_vehicle_reverse                 ] = _key_down_arrow;
	//preferences->control_to_keyboard_mapping[             _button_vehicle_raise                   ] = _key_space;
	//preferences->control_to_mouse_alternative_mapping[    _button_vehicle_dive                    ] = _mouse_button_wheel_down;
	//preferences->control_to_mouse_alternative_mapping[    _button_vehicle_raise                   ] = _mouse_button_wheel_up;
	//preferences->control_to_mouse_mapping[                _button_vehicle_fire                    ] = _mouse_button_left_click;
	//preferences->control_to_mouse_mapping[                _button_vehicle_alt_fire                ] = _mouse_button_right_click;
	//
	//switch (keyboard)
	//{
	//case _keyboard_preset_default:
	//case _keyboard_preset_custom:
	//{
	//	preferences->control_to_keyboard_alternative_mapping[ _button_melee_attack     ] = _key_v;
	//	preferences->control_to_keyboard_mapping[             _button_vehicle_use      ] = _key_e;
	//	preferences->control_to_keyboard_mapping[             _button_fire_tertiary    ] = _key_q;
	//	preferences->control_to_keyboard_mapping[             _button_lean_right       ] = _key_left_shift;
	//	preferences->control_to_mouse_mapping[                _button_throw_grenade    ] = preferences->control_to_mouse_mapping[_button_fire_secondary];
	//	preferences->control_to_keyboard_alternative_mapping[ _button_fire_tertiary    ] = _key_v;
	//	preferences->control_to_keyboard_mapping[             _button_unknown58        ] = _key_space;
	//	preferences->control_to_keyboard_mapping[             _button_fire_primary     ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[             _button_fire_secondary   ] = _key_f;
	//	preferences->control_to_keyboard_mapping[             _button_jump             ] = _key_space;
	//	preferences->control_to_keyboard_mapping[             _button_switch_grenade   ] = _key_g;
	//	preferences->control_to_keyboard_mapping[             _button_melee_attack     ] = _key_q;
	//	preferences->control_to_keyboard_mapping[             _button_throw_grenade    ] = _key_f;
	//	preferences->control_to_mouse_mapping[                _button_fire_primary     ] = _mouse_button_left_click;
	//	preferences->control_to_mouse_mapping[                _button_switch_weapon    ] = _mouse_button_wheel_up;
	//	preferences->control_to_mouse_alternative_mapping[    _button_switch_weapon    ] = _mouse_button_wheel_down;
	//	preferences->control_to_mouse_alternative_mapping[    _button_fire_secondary   ] = _mouse_button_middle_click;
	//	preferences->control_to_keyboard_mapping[             _button_reload_right     ] = _key_r;
	//	preferences->control_to_keyboard_mapping[             _button_use              ] = _key_e;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_reload_right     ] = _key_not_a_key;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_use              ] = _key_not_a_key;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_vehicle_use      ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[                _button_reload_right     ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[                _button_use              ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[                _button_vehicle_use      ] = _key_not_a_key;
	//	preferences->control_to_mouse_alternative_mapping[    _button_reload_right     ] = _key_not_a_key;
	//	preferences->control_to_mouse_alternative_mapping[    _button_use              ] = _key_not_a_key;
	//	preferences->control_to_mouse_alternative_mapping[    _button_vehicle_use      ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[             _button_crouch           ] = _key_left_control;
	//	preferences->control_to_keyboard_mapping[             _button_scope_zoom       ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[             _button_use_consumable_1 ] = _key_1;
	//	preferences->control_to_keyboard_mapping[             _button_use_consumable_2 ] = _key_2;
	//	preferences->control_to_keyboard_mapping[             _button_use_consumable_3 ] = _key_3;
	//	preferences->control_to_keyboard_mapping[             _button_use_consumable_4 ] = _key_4;
	//	preferences->control_to_keyboard_mapping[             _button_team_chat        ] = _key_y;
	//	preferences->control_to_keyboard_mapping[             _button_voice_chat       ] = _key_caps_lock;
	//	preferences->control_to_keyboard_mapping[             _button_general_chat     ] = _key_t;
	//	preferences->control_to_mouse_mapping[                _button_scope_zoom       ] = _mouse_button_right_click;
	//	preferences->control_to_keyboard_mapping[             _button_move_forward     ] = _key_w;
	//	preferences->control_to_keyboard_mapping[             _button_move_backward    ] = _key_s;
	//	preferences->control_to_keyboard_mapping[             _button_move_left        ] = _key_a;
	//	preferences->control_to_keyboard_mapping[             _button_move_right       ] = _key_d;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_move_forward     ] = _key_up_arrow;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_move_backward    ] = _key_down_arrow;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_move_left        ] = _key_left_arrow;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_move_right       ] = _key_right_arrow;
	//	preferences->control_to_keyboard_alternative_mapping[ _button_throw_grenade    ] = preferences->control_to_keyboard_alternative_mapping[_button_fire_secondary];
	//	preferences->control_to_mouse_alternative_mapping[    _button_throw_grenade    ] = _mouse_button_middle_click;
	//	preferences->control_to_keyboard_mapping[             _button_next_player      ] = _key_d;
	//	preferences->control_to_keyboard_mapping[             _button_prev_player      ] = _key_a;
	//}
	//break;
	//case _keyboard_preset_skirmish:
	//{
	//	preferences->control_to_keyboard_mapping[ _button_reload_right   ] = _key_t;
	//	preferences->control_to_keyboard_mapping[ _button_switch_grenade ] = _key_e;
	//	preferences->control_to_keyboard_mapping[ _button_switch_weapon  ] = _key_tab;
	//	preferences->control_to_keyboard_mapping[ _button_melee_attack   ] = _key_f;
	//	preferences->control_to_keyboard_mapping[ _button_throw_grenade  ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[    _button_fire_primary   ] = _mouse_button_left_click;
	//	preferences->control_to_mouse_mapping[    _button_fire_secondary ] = _mouse_button_right_click;
	//	preferences->control_to_keyboard_mapping[ _button_flashlight     ] = _key_x;
	//	preferences->control_to_keyboard_mapping[ _button_scope_zoom     ] = _key_left_shift;
	//	preferences->control_to_keyboard_mapping[ _button_crouch         ] = _key_z;
	//	preferences->control_to_keyboard_mapping[ _button_move_forward   ] = _key_w;
	//	preferences->control_to_keyboard_mapping[ _button_move_backward  ] = _key_s;
	//	preferences->control_to_keyboard_mapping[ _button_move_left      ] = _key_a;
	//	preferences->control_to_keyboard_mapping[ _button_move_right     ] = _key_d;
	//	preferences->control_to_keyboard_mapping[ _button_fire_tertiary  ] = _key_f;
	//	preferences->control_to_keyboard_mapping[ _button_jump           ] = _key_space;
	//	preferences->control_to_keyboard_mapping[ _button_reload_left    ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[    _button_switch_weapon  ] = _mouse_button_7;
	//	preferences->control_to_keyboard_mapping[ _button_fire_primary   ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[ _button_fire_secondary ] = _key_not_a_key;
	//}
	//break;
	//case _keyboard_preset_southpaw:
	//{
	//	preferences->control_to_keyboard_mapping[ _button_reload_right   ] = _key_u;
	//	preferences->control_to_keyboard_mapping[ _button_switch_grenade ] = _key_g;
	//	preferences->control_to_keyboard_mapping[ _button_switch_weapon  ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[ _button_melee_attack   ] = _key_h;
	//	preferences->control_to_keyboard_mapping[ _button_throw_grenade  ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[    _button_fire_primary   ] = _mouse_button_right_click;
	//	preferences->control_to_mouse_mapping[    _button_fire_secondary ] = _mouse_button_left_click;
	//	preferences->control_to_keyboard_mapping[ _button_flashlight     ] = _key_alt;
	//	preferences->control_to_keyboard_mapping[ _button_scope_zoom     ] = _key_left_shift;
	//	preferences->control_to_keyboard_mapping[ _button_crouch         ] = _key_n;
	//	preferences->control_to_keyboard_mapping[ _button_move_forward   ] = _key_i;
	//	preferences->control_to_keyboard_mapping[ _button_move_backward  ] = _key_k;
	//	preferences->control_to_keyboard_mapping[ _button_move_left      ] = _key_j;
	//	preferences->control_to_keyboard_mapping[ _button_move_right     ] = _key_l;
	//	preferences->control_to_keyboard_mapping[ _button_fire_tertiary  ] = _key_h;
	//	preferences->control_to_keyboard_mapping[ _button_jump           ] = _key_space;
	//	preferences->control_to_keyboard_mapping[ _button_reload_left    ] = _key_not_a_key;
	//	preferences->control_to_mouse_mapping[    _button_switch_weapon  ] = _mouse_button_7;
	//	preferences->control_to_keyboard_mapping[ _button_fire_primary   ] = _key_not_a_key;
	//	preferences->control_to_keyboard_mapping[ _button_fire_secondary ] = _key_not_a_key;
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

		for (int32 button_index = 0; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
		{
			input_abstraction_globals.input_states[controller_index].get_button(button_index).unlock();
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

	for (int32 i = 0; i < NUMBER_OF_KEYBOARD_CONTROL_BUTTONS * 2; i++)
	{
		int32 key_index = i % NUMBER_OF_KEYBOARD_CONTROL_BUTTONS;

		int16 key_code = (i < NUMBER_OF_KEYBOARD_CONTROL_BUTTONS ? preferences->control_to_keyboard_mapping : preferences->control_to_keyboard_alternative_mapping)[key_index];
		if (key_code != 0xFF)
		{
			e_input_type input_type = _input_type_game;
			if (i == _key_f6 || i == _key_f7)
				input_type = _input_type_special;

			if (i == _key_left_shift || i == _key_return)
				input_type = _input_type_ui;

			if (i == _key_z)
				input_type = _input_type_ui;

			uns16 down_msec = MAX(input_state->get_button(key_index).down_msec(), input_key_msec_down((e_input_key_code)key_code, input_type));
			uns8 down_frames = MAX(input_state->get_button(key_index).down_frames(), input_key_frames_down((e_input_key_code)key_code, input_type));
			uns8 down_amount = MAX((uns8)input_state->get_button(key_index).down_amount(), input_key_frames_down((e_input_key_code)key_code, input_type));

			input_state->get_button(key_index).update(down_msec, down_frames, -(down_amount != 0));
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
//	input_state->get_button(_button_##ACTION).update(msec_down, frames_down, -(frames_down != 0)); \
//}
//#define UPDATE_BUTTON_MOUSE(TYPE, MOUSE, ACTION) \
//{ \
//	uns16 msec_down = input_mouse_msec_down(_mouse_button_##MOUSE, _input_type_##TYPE); \
//	uns8 frames_down = input_mouse_frames_down(_mouse_button_##MOUSE, _input_type_##TYPE); \
//	input_state->get_button(_button_##ACTION).update(msec_down, frames_down, -(frames_down != 0)); \
//}
//
//#define UPDATE_BUTTON_MOUSE_AND_KEY(TYPE, MOUSE, KEY, ACTION) \
//{ \
//	uns16 msec_down = FLOOR(input_mouse_msec_down(_mouse_button_##MOUSE, _input_type_##TYPE), input_key_msec_down(_key_code_##KEY, _input_type_##TYPE)); \
//	uns8 frames_down = FLOOR(input_mouse_frames_down(_mouse_button_##MOUSE, _input_type_##TYPE), input_key_frames_down(_key_code_##KEY, _input_type_##TYPE)); \
//	input_state->get_button(_button_##ACTION).update(msec_down, frames_down, -(frames_down != 0)); \
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
//	for (int32 i = _button_jump; i < NUMBER_OF_CONTROL_BUTTONS; i++)
//		input_state->get_button(i).set_accessor(preferences->gamepad_buttons[i]);
//
//#undef UPDATE_BUTTON_MOUSE_AND_KEY
//#undef UPDATE_BUTTON_MOUSE
//#undef UPDATE_BUTTON_KEY
}

void __cdecl input_abstraction_latch_all_buttons(int32 controller_index)
{
	//INVOKE(0x0060CE40, input_abstraction_latch_all_buttons, controller_index);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);

	for (int32 button_index = 0; button_index < NUMBER_OF_CONTROL_BUTTONS; button_index++)
	{
		input_abstraction_globals.input_states[controller_index].get_button(button_index).latch();
	}
}

void __cdecl sub_60CE70(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
{
	//INVOKE(0x0060CE70, sub_60CE70, preferences, input_state);

	//for (int32 i = 0; i < NUMBER_OF_CONTROL_BUTTONS * 2; i++)
	//{
	//	int32 mouse_button_index = i % NUMBER_OF_CONTROL_BUTTONS;
	//
	//	e_mouse_button mouse_button = (i < NUMBER_OF_CONTROL_BUTTONS ? preferences->control_to_mouse_mapping : preferences->control_to_mouse_alternative_mapping)[mouse_button_index];
	//	if (mouse_button < k_mouse_button_count)
	//	{
	//		uns16 down_msec = MAX(input_state->get_button(mouse_button_index).down_msec(), input_mouse_msec_down(mouse_button, _input_type_game));
	//		uns8 down_frames = MAX(input_state->get_button(mouse_button_index).down_frames(), input_mouse_frames_down(mouse_button, _input_type_game));
	//		uns8 down_amount = MAX((uns8)input_state->get_button(mouse_button_index).down_amount(), input_mouse_frames_down(mouse_button, _input_type_game));
	//
	//		input_state->get_button(mouse_button_index).update(down_msec, down_frames, down_amount);
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
	//		uns8 down_frames = MAX(input_state->get_button(mouse_button_index).down_frames(), 1);
	//		uns16 down_msec = MAX(input_state->get_button(mouse_button_index).down_msec(), 1);
	//		uns8 down_amount = 255;
	//
	//		input_state->get_button(mouse_button_index).update(down_msec, down_frames, down_amount);
	//	}
	//}

	for (int32 i = 0; i < NUMBER_OF_CONTROL_BUTTONS * 2; i++)
	{
		int32 mouse_button_index = (i < NUMBER_OF_CONTROL_BUTTONS) ? i : i - NUMBER_OF_CONTROL_BUTTONS;

		uns8 mouse_button = preferences->control_to_mouse_mapping[mouse_button_index];
		if (i >= NUMBER_OF_CONTROL_BUTTONS)
		{
			mouse_button = preferences->control_to_mouse_alternative_mapping[mouse_button_index];
		}

		c_abstract_button& abstract_button = input_state->get_button(mouse_button_index);

		if (mouse_button < k_mouse_button_count)
		{
			uns16 down_msec = abstract_button.down_msec();
			uns16 frames_down = input_mouse_frames_down((e_mouse_button)mouse_button, _input_type_game);
			uns16 msec_down = input_mouse_msec_down((e_mouse_button)mouse_button, _input_type_game);
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

	uns8 forward_frames = input_state->get_button(_button_move_forward).down_frames();
	uns8 back_frames = input_state->get_button(_button_move_backward).down_frames();
	uns8 left_frames = input_state->get_button(_button_move_left).down_frames();
	uns8 right_frames = input_state->get_button(_button_move_right).down_frames();
	uns8 vehicle_accelerate_frames = input_state->get_button(_button_vehicle_accelerate).down_frames();
	uns8 vehicle_reverse_frames = input_state->get_button(_button_vehicle_reverse).down_frames();

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
	ASSERT(preferences->control_to_gamepad_button_mapping[_button_start] == _gamepad_binary_button_start && preferences->control_to_gamepad_button_mapping[_button_back] == _gamepad_binary_button_back, "invalid controller preferences; can't remap start & back buttons");

	for (int32 control_index = 0; control_index < NUMBER_OF_CONTROL_BUTTONS; control_index++)
	{
		uns8 gamepad_button_index = preferences->control_to_gamepad_button_mapping[control_index];
		ASSERT((gamepad_button_index>=0 && gamepad_button_index<NUMBER_OF_GAMEPAD_BUTTONS) || gamepad_button_index == _gamepad_button_unused);
	}

	for (int32 gamepad_button_index = 0; gamepad_button_index < NUMBER_OF_GAMEPAD_BUTTONS; gamepad_button_index++)
	{
		input_abstraction_globals.input_states[controller_index].get_button(gamepad_button_index).unlock();
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
	m_button_accessor_index(NONE),
	m_button_locked_index(NONE)
{
}

bool c_abstract_button::access_valid() const
{
	return m_button_locked_index == NONE || m_button_locked_index == m_button_accessor_index;
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
	m_button_locked_index = m_button_accessor_index;
}

bool c_abstract_button::locked()
{
	return (m_button_locked_index + 1) == 0;
}

void c_abstract_button::set_accessor(int32 accessor)
{
	m_button_accessor_index = (uns8)accessor;
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
	m_button_locked_index = NONE;
}

c_abstract_button& s_game_input_state::get_button(int32 button_index)
{
	return abstract_gamepad_buttons[button_index];
}

const c_abstract_button& s_game_input_state::get_button(int32 button_index) const
{
	return abstract_gamepad_buttons[button_index];
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

