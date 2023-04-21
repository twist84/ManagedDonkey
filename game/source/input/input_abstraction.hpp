#pragma once

#include "cseries/cseries.hpp"
#include "input/input.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"

struct s_gamepad_input_preferences
{
	real look_sensitivity_x;
	real look_sensitivity_y;

	c_static_array<c_enum<e_controller_button, char, _controller_button_left_trigger, k_controller_button_count>, k_gamepad_button_count> gamepad_buttons;
	c_static_array<bool, k_gamepad_button_count> gamepad_buttons_held;

	c_static_array<c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count>, k_gamepad_button_count_keyboard> keys_primary;
	c_static_array<c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count>, k_gamepad_button_count_keyboard> keys_secondary;
	c_static_array<c_enum<e_mouse_button, char, _mouse_button_1, k_total_mouse_button_count>, k_gamepad_button_count_keyboard> mouse_buttons_primary;
	c_static_array<c_enum<e_mouse_button, char, _mouse_button_1, k_total_mouse_button_count>, k_gamepad_button_count_keyboard> mouse_buttons_secondary;

	short joystick_preset;
	bool controller_look_inverted;
	bool controller_flight_stick_aircraft_controls;
	short camera_panning;
	real camera_flying_movement;
	real camera_flying_thrust;
};
static_assert(sizeof(s_gamepad_input_preferences) == 0x208);

struct c_abstract_button
{
public:
	
	c_abstract_button();
	void update(word down_msec, word down_frames, byte down_amount);
	void set_accessor(long accessor); // e_button_action
	void unlock();
	bool locked();
	void lock();
	real down_amount();
	bool access_valid() const;
	word down_msec();
	bool latched() const;
	byte down_frames() const;
	void set_latch_bit(bool set_bit);
	void latch();

protected:
	word m_down_msec;
	byte m_down_frames;
	byte_flags m_flags;
	real m_down_amount;
	byte m_accessor;
	byte m_locked;
};
static_assert(sizeof(c_abstract_button) == 0xC);

struct s_game_input_state
{
	c_abstract_button abstract_buttons[k_gamepad_button_count_keyboard];
	int16_point2d abstract_sticks[2];
	real forward_movement;
	real strafe;
	real yaw;
	real pitch;
	real __unknown30C;
	real __unknown310;
	real __unknown314;
	real __unknown318;
	real __unknown31C;
	real __unknown320;
	bool __unknown324;
};
static_assert(sizeof(s_game_input_state) == 0x328);

struct s_input_globals_definition
{
	long __unknown0;
	long __unknown4;
	s_tag_data __unknown8;
	s_tag_data __unknown1C;
	long __unknown30;
};
static_assert(sizeof(s_input_globals_definition) == 0x34);

struct s_input_abstraction_globals
{
	s_gamepad_input_preferences preferences[k_number_of_controllers];
	s_game_input_state input_states[k_number_of_controllers];
	dword controller_detection_timer;
	bool input_has_gamepad[k_number_of_controllers];
	long controls_method;
	bool input_device_changed;

	// 'inpg'
	s_input_globals_definition* input_globals;
};
static_assert(sizeof(s_input_abstraction_globals) == 0x14D4);

extern s_input_abstraction_globals& input_abstraction_globals;

extern void __cdecl input_abstraction_dispose();
extern void __cdecl input_abstraction_dispose_from_old_map();
extern void __cdecl sub_60B3D0(struct gamepad_state* state, s_gamepad_input_preferences* preferences, s_game_input_state* input_state, long gamepad_index);
extern short __cdecl input_abstraction_get_abstract_stick_pitch(long controller_index);
extern long __cdecl input_abstraction_get_controls_method();
extern void __cdecl sub_60BEA0(s_gamepad_input_preferences* preferences, void* bindings);
extern void __cdecl input_abstraction_get_controller_preferences(long controller_index, s_gamepad_input_preferences* preferences);
extern void __cdecl input_abstraction_get_default_preferences(s_gamepad_input_preferences* preferences);
extern void input_abstraction_get_input_state(long controller_index, s_game_input_state** input_state);
extern void __cdecl input_abstraction_get_player_look_angular_velocity(long controller_index, euler_angles2d* angular_velocity);
extern void __cdecl sub_60C040(int keyboard_preset, s_gamepad_input_preferences* preferences);
extern void __cdecl input_abstraction_initialize();
//extern void __cdecl input_abstraction_initialize_for_new_map();
//extern void __cdecl sub_60C4A0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
//extern void __cdecl sub_60C6D0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);

void __cdecl input_abstraction_latch_all_buttons(long controller_index);
//extern void __cdecl sub_60CE70(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
//extern void __cdecl sub_60D160(mouse_state* state, s_game_input_state* input_state, long a3);
//extern void __cdecl sub_60D620(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
extern void __cdecl input_abstraction_reset_controller_detection_timer();
//extern void __cdecl input_should_suppress_rumble(long controls_method);
extern void __cdecl input_abstraction_set_controller_preferences(long controller_index, s_gamepad_input_preferences* preferences);
//extern void __cdecl input_abstraction_update();
//extern void __cdecl input_abstraction_update_device_changes(dword_flags flags);
