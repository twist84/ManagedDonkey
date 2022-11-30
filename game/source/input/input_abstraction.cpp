#include "input/input_abstraction.hpp"

#include "cseries/cseries_windows.hpp"
#include "input/controllers.hpp"
#include "input/input.hpp"
#include "memory/module.hpp"

#include <assert.h>

REFERENCE_DECLARE(0x0244C9D0, s_input_abstraction_globals, input_abstraction_globals);

HOOK_DECLARE(0x0060B3A0, input_abstraction_dispose);
HOOK_DECLARE(0x0060B3C0, input_abstraction_dispose_from_old_map);
//HOOK_DECLARE(0x0060B3D0, sub_60B3D0);
HOOK_DECLARE(0x0060BE20, input_abstraction_get_abstract_stick_pitch);
HOOK_DECLARE(0x0060BE60, input_abstraction_get_controls_method);
//HOOK_DECLARE(0x0060BEA0, sub_60BEA0);
HOOK_DECLARE(0x0060BF00, input_abstraction_get_default_preferences);
HOOK_DECLARE(0x0060BFE0, input_abstraction_get_input_state);
HOOK_DECLARE(0x0060C000, input_abstraction_get_player_look_angular_velocity);
//HOOK_DECLARE(0x0060C040, sub_60C040);
HOOK_DECLARE(0x0060C390, input_abstraction_initialize);
//HOOK_DECLARE(0x0060C430, input_abstraction_initialize_for_new_map);
//HOOK_DECLARE(0x0060C4A0, sub_60C4A0);
//HOOK_DECLARE(0x0060C6D0, sub_60C6D0);
HOOK_DECLARE(0x0060CE40, input_abstraction_latch_all_buttons);
//HOOK_DECLARE(0x0060CE70, sub_60CE70);
//HOOK_DECLARE(0x0060D160, sub_60D160);
//HOOK_DECLARE(0x0060D620, sub_60D620);
HOOK_DECLARE(0x0060D7A0, input_abstraction_reset_controller_detection_timer);
//HOOK_DECLARE(0x0060D7B0, input_should_suppress_rumble);
HOOK_DECLARE(0x0060D830, input_abstraction_set_controller_preferences);
//HOOK_DECLARE(0x0060D880, input_abstraction_update);
//HOOK_DECLARE(0x0060D9E0, input_abstraction_update_device_changes);

void __cdecl input_abstraction_dispose()
{
	csmemset(&input_abstraction_globals, 0, sizeof(s_input_abstraction_globals));
}

void __cdecl input_abstraction_dispose_from_old_map()
{
	input_abstraction_globals.input_globals = nullptr;
}

//void __cdecl sub_60B3D0(struct gamepad_state* state, s_gamepad_input_preferences* preferences, s_game_input_state* input_state, long gamepad_index)
//{
//	HOOK_INVOKE(, sub_60B3D0, state, preferences, input_state, gamepad_index);
//}

short __cdecl input_abstraction_get_abstract_stick_pitch(long controller_index)
{
	assert(controller_index >= 0 && controller_index < k_number_of_controllers);

	int16_point2d* abstract_sticks = input_abstraction_globals.input_states[controller_index].abstract_sticks;
	short joystick_preset = input_abstraction_globals.preferences[controller_index].joystick_preset;

	return abstract_sticks[(joystick_preset == 1 || joystick_preset == 3) ? 0 : 1].y;
}

long __cdecl input_abstraction_get_controls_method()
{
	return input_abstraction_globals.controls_method;
}

// global_preferences
//void __cdecl sub_60BEA0(s_gamepad_input_preferences* preferences, void* bindings)
//{
//	HOOK_INVOKE(, sub_60BEA0, preferences, bindings);
//}

void __cdecl input_abstraction_get_controller_preferences(long controller_index, s_gamepad_input_preferences* preferences)
{
	assert(controller_index >= 0 && controller_index < k_number_of_controllers);
	assert(preferences);

	csmemcpy(preferences, &input_abstraction_globals.preferences[controller_index], sizeof(s_gamepad_input_preferences));
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
	//for (long gamepad_button_index = 0; gamepad_button_index < 16; gamepad_button_index++)
	//	preferences->gamepad_buttons[gamepad_button_index] = gamepad_button_index;
	//
	//// #TODO: 
	//preferences->gamepad_buttons[] = ;
	//
	//preferences->joystick_preset = 0;
	//preferences->controller_look_inverted = 0;
	//preferences->controller_flight_stick_aircraft_controls = 0;
	//preferences->camera_panning = 0;
	//preferences->camera_flying_movement = 1.0;
	//preferences->camera_flying_thrust = 1.0;
}

void __cdecl input_abstraction_get_input_state(long controller_index, s_game_input_state** input_state)
{
	assert(input_state);
	assert(controller_index >= 0 && controller_index < k_number_of_controllers);

	*input_state = &input_abstraction_globals.input_states[controller_index];
}

void __cdecl input_abstraction_get_player_look_angular_velocity(long controller_index, euler_angles2d* angular_velocity)
{
	angular_velocity->yaw = static_cast<real>(input_abstraction_globals.preferences[controller_index].look_sensitivity_x * RAD);
	angular_velocity->pitch = static_cast<real>(input_abstraction_globals.preferences[controller_index].look_sensitivity_y * RAD);
}

//void __cdecl sub_60C040(int keyboard_preset, s_gamepad_input_preferences* preferences)
//{
//	HOOK_INVOKE(, sub_60C040, keyboard_preset, preferences);
//}

void __cdecl input_abstraction_initialize()
{
	csmemset(&input_abstraction_globals, 0, sizeof(s_input_abstraction_globals));

	for (long controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
	{
		input_abstraction_get_default_preferences(&input_abstraction_globals.preferences[controller_index]);
		input_abstraction_globals.input_has_gamepad[controller_index] = input_has_gamepad(static_cast<short>(controller_index));

		for (long button_index = 0; button_index < 16; button_index++)
			input_abstraction_globals.input_states[controller_index].abstract_buttons[button_index].unlock();
	}

	input_abstraction_reset_controller_detection_timer();
	input_abstraction_globals.input_device_changed = true;
}

//void __cdecl input_abstraction_initialize_for_new_map()
//void __cdecl sub_60C4A0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
//void __cdecl sub_60C6D0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)

void __cdecl input_abstraction_latch_all_buttons(long controller_index)
{
	assert(controller_index >= 0 && controller_index < k_number_of_controllers);

	for (long button_index = 0; button_index < k_gamepad_button_count; button_index++)
		input_abstraction_globals.input_states[controller_index].abstract_buttons[button_index].latch();
}

//void __cdecl sub_60CE70(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)
//void __cdecl sub_60D160(mouse_state* state, s_game_input_state* input_state, long a3)
//void __cdecl sub_60D620(s_gamepad_input_preferences* preferences, s_game_input_state* input_state)

void __cdecl input_abstraction_reset_controller_detection_timer()
{
	input_abstraction_globals.controller_detection_timer = system_milliseconds();
}

//void __cdecl input_should_suppress_rumble(long controls_method)

void __cdecl input_abstraction_set_controller_preferences(long controller_index, s_gamepad_input_preferences* preferences)
{
	assert(controller_index >= 0 && controller_index < k_number_of_controllers);
	assert(preferences);
	//assert(preferences->gamepad_buttons[_gamepad_button_start] != _controller_button_start || preferences->gamepad_buttons[_gamepad_button_back] != _controller_button_back);

	for (long button_index = 0; button_index < 16; button_index++)
		input_abstraction_globals.input_states[controller_index].abstract_buttons[button_index].unlock();

	csmemcpy(&input_abstraction_globals.preferences[controller_index], preferences, sizeof(s_gamepad_input_preferences));
}

//void __cdecl input_abstraction_update()
//void __cdecl input_abstraction_update_device_changes(dword_flags flags)

c_abstract_button::c_abstract_button() :
	m_down_msec(),
	m_down_frames(),
	m_flags(),
	m_down_amount(),
	m_accessor(-1),
	m_locked(-1)
{
}

void c_abstract_button::update(word down_msec, word down_frames, byte down_amount)
{
	m_down_msec = down_msec;
	m_down_frames = static_cast<byte>(down_frames);
	m_down_amount = down_amount / 255.0f;

	if (!m_down_frames && latched())
		set_latch_bit(false);
}

void c_abstract_button::set_accessor(long accessor)
{
	m_accessor = static_cast<byte>(accessor);
}

void c_abstract_button::unlock()
{
	m_locked = -1;
}

bool c_abstract_button::locked()
{
	return (m_locked + 1) == 0;
}

void c_abstract_button::lock()
{
	assert(access_valid());
	m_locked = m_accessor;
}

real c_abstract_button::down_amount()
{
	if (!latched() && access_valid())
		return m_down_amount;

	return 0.0f;
}

bool c_abstract_button::access_valid() const
{
	return m_locked == 0xFF || m_locked == m_accessor;
}

word c_abstract_button::down_msec()
{
	if (!latched() && access_valid())
		return m_down_msec;

	return 0;
}

bool c_abstract_button::latched() const
{
	return TEST_BIT(m_flags, 0);
}

byte c_abstract_button::down_frames() const
{
	if (!latched() && access_valid())
		return m_down_frames;

	return 0;
}

void c_abstract_button::set_latch_bit(bool set_bit)
{
	if (set_bit)
		m_flags |= (1 << 0);
	else
		m_flags |= ~(1 << 0);
}

void c_abstract_button::latch()
{
	set_latch_bit(true);
}

