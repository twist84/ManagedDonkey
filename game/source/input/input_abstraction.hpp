#pragma once

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"
#include "tag_files/tag_groups.hpp"

struct s_keyboard_input_preferences
{
	int16 control_to_keyboard_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];
	int16 control_to_keyboard_alternative_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];

	uns8 control_to_mouse_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];
	uns8 control_to_mouse_alternative_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];

	int16 joystick_preset;
};
COMPILE_ASSERT(sizeof(s_keyboard_input_preferences) == 0x17C);

struct s_gamepad_input_preferences
{
	real32 yaw_rate;
	real32 pitch_rate;

	uns8 control_to_gamepad_button_mapping[NUMBER_OF_CONTROL_BUTTONS];
	uns8 control_to_gamepad_button_alternative_mapping[NUMBER_OF_CONTROL_BUTTONS];

	int16 control_to_keyboard_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];
	int16 control_to_keyboard_alternative_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];

	uns8 control_to_mouse_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];
	uns8 control_to_mouse_alternative_mapping[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];

	int16 joystick_preset;
	bool controller_look_inverted;
	bool controller_flight_stick_aircraft_controls;
	int16 camera_panning;
	real32 camera_flying_movement;
	real32 camera_flying_thrust;
};
COMPILE_ASSERT(sizeof(s_gamepad_input_preferences) == 0x208);

class c_abstract_button
{
public:
	enum
	{
		_abstract_button_latch_bit = 0,
		k_abstract_button_flag_count,
	};

public:
	c_abstract_button();

	bool access_valid() const;
	real32 down_amount();
	uns8 down_frames() const;
	uns16 down_msec();
	bool is_down();
	void latch();
	bool latched() const;
	void lock();
	bool locked();
	void set_accessor(int32 accessor);
	void set_latch_bit(bool set_bit);
	void update(uns16 down_msec, uns16 down_frames, uns8 down_amount);
	void unlock();

protected:
	uns16 m_down_msec;
	uns8 m_down_frames;
	uns8 m_flags;
	real32 m_down_amount;
	int8 m_button_accessor_index;
	int8 m_button_locked_index;
};
COMPILE_ASSERT(sizeof(c_abstract_button) == 0xC);

struct s_game_input_state
{
	c_abstract_button& get_button(int32 button_index);
	const c_abstract_button& get_button(int32 button_index) const;

	// player_control_get_controller_input
	// if (unit->unit.seat_index != NONE && unit->object.parent_object_index != NONE)
	// {
	//   input_state->yaw = input_state->vehicle_yaw;
	//   input_state->pitch = input_state->vehicle_pitch;
	//   input_state->aircraft_pitch1 = input_state->vehicle_pitch1;
	// }
	// 
	// real32 pitch = user_currently_piloting_aircraft(input_user_index) ? input_state->aircraft_pitch : input_state->pitch

	// sub_60D620
	// input_state->aircraft_pitch = input_state->aircraft_pitch1


	c_abstract_button abstract_gamepad_buttons[NUMBER_OF_KEYBOARD_CONTROL_BUTTONS];
	point2d abstract_sticks[NUMBER_OF_GAMEPAD_STICKS];

	real32 forward_movement;
	real32 strafe;
	real32 vehicle_forward_movement;

	real32 yaw;
	real32 pitch;
	real32 aircraft_pitch;

	real32 vehicle_yaw;
	real32 vehicle_pitch;

	// overrides?
	real32 aircraft_pitch1;
	real32 vehicle_pitch1;

	bool aim_instantaneously;
};
COMPILE_ASSERT(sizeof(s_game_input_state) == 0x328);

struct s_input_globals_definition
{
	int32 __unknown0;
	int32 __unknown4;
	s_tag_data __unknown8;
	s_tag_data __unknown1C;
	int32 __unknown30;
};
COMPILE_ASSERT(sizeof(s_input_globals_definition) == 0x34);

struct s_input_abstraction_globals
{
	s_gamepad_input_preferences preferences[k_number_of_controllers];
	s_game_input_state input_states[k_number_of_controllers];
	uns32 controller_detection_timer;
	bool input_has_gamepad[k_number_of_controllers];
	int32 controls_method;
	bool input_device_changed;

	// 'inpg'
	s_input_globals_definition* input_globals;
};
COMPILE_ASSERT(sizeof(s_input_abstraction_globals) == 0x14D4);

extern s_input_abstraction_globals& input_abstraction_globals;
extern bool use_mean_look_rate;

extern void __cdecl input_abstraction_dispose();
extern void __cdecl input_abstraction_dispose_from_old_map();
extern void __cdecl sub_60B3D0(gamepad_state* state, s_gamepad_input_preferences* preferences, s_game_input_state* input_state, int32 gamepad_index);
extern int16 __cdecl input_abstraction_get_abstract_stick_pitch(int32 controller_index);
extern int32 __cdecl input_abstraction_get_controls_method();
extern void __cdecl input_abstraction_get_controller_preferences(int32 controller_index, s_gamepad_input_preferences* preferences);
extern void __cdecl sub_60BEA0(s_gamepad_input_preferences* preferences, void* bindings);
extern void __cdecl input_abstraction_get_default_preferences(s_gamepad_input_preferences* preferences);
//.text:0060BFC0 ; gamepad_state *sub_60BFC0()
extern void __cdecl input_abstraction_get_input_state(int32 controller_index, s_game_input_state** input_state);
extern void __cdecl input_abstraction_get_player_look_angular_velocity(int32 controller_index, real_euler_angles2d* angular_velocity);
extern void __cdecl input_abstraction_get_preset_preferences(int32 keyboard_preset, s_gamepad_input_preferences* preferences);
extern void __cdecl input_abstraction_initialize();
extern void __cdecl input_abstraction_initialize_for_new_map();
extern void __cdecl sub_60C4A0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
extern void __cdecl sub_60C6D0(s_gamepad_input_preferences* preferences, s_game_input_state* input_state); // this is for setting editor controls
extern void __cdecl input_abstraction_latch_all_buttons(int32 controller_index);
extern void __cdecl sub_60CE70(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
extern void __cdecl sub_60D160(mouse_state* state, s_game_input_state* input_state, int32 a3);
extern void __cdecl sub_60D620(s_gamepad_input_preferences* preferences, s_game_input_state* input_state);
extern void __cdecl input_abstraction_reset_controller_detection_timer();
extern void __cdecl input_should_suppress_rumble(int32 controls_method);
extern void __cdecl input_abstraction_set_controller_preferences(int32 controller_index, s_gamepad_input_preferences* preferences);
extern void __cdecl input_abstraction_update();
extern void __cdecl input_abstraction_update_device_changes(uns32 flags);

extern void input_abstraction_get_raw_data_string(char* buffer, int16 size);

