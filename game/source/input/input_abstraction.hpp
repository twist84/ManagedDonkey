#pragma once

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"
#include "tag_files/tag_groups.hpp"

struct s_keyboard_input_preferences
{
	c_static_array<c_enum<e_input_key_code, int16, _key_escape, NUMBER_OF_KEYS>, k_button_action_count_keyboard> keys_primary;
	c_static_array<c_enum<e_input_key_code, int16, _key_escape, NUMBER_OF_KEYS>, k_button_action_count_keyboard> keys_alternative;
	c_static_array<c_enum<e_mouse_button, int8, _mouse_button_1, k_total_mouse_button_count>, k_button_action_count_keyboard> mouse_buttons_primary;
	c_static_array<c_enum<e_mouse_button, int8, _mouse_button_1, k_total_mouse_button_count>, k_button_action_count_keyboard> mouse_buttons_alternative;

	int16 joystick_preset;
};
static_assert(sizeof(s_keyboard_input_preferences) == 0x17C);

struct s_gamepad_input_preferences
{
	real32 look_sensitivity_x;
	real32 look_sensitivity_y;

	c_static_array<c_enum<e_controller_button, int8, _controller_button_left_trigger, k_controller_button_count>, k_button_action_count> gamepad_buttons;
	c_static_array<bool, k_button_action_count> gamepad_buttons_held;

	s_keyboard_input_preferences keyboard_preferences;

	bool controller_look_inverted;
	bool controller_flight_stick_aircraft_controls;
	int16 camera_panning;
	real32 camera_flying_movement;
	real32 camera_flying_thrust;
};
static_assert(sizeof(s_gamepad_input_preferences) == 0x208);

class c_abstract_button
{
public:
	c_abstract_button();
	
	void update(uns16 down_msec, uns16 down_frames, uns8 down_amount);
	void set_accessor(e_button_action accessor);
	void unlock();
	bool locked();
	void lock();
	real32 down_amount();
	bool access_valid() const;
	uns16 down_msec();
	bool latched() const;
	uns8 down_frames() const;
	void set_latch_bit(bool set_bit);
	void latch();
	bool is_down();

protected:
	uns16 m_down_msec;
	uns8 m_down_frames;
	uns8 m_flags;
	real32 m_down_amount;
	uns8 m_accessor;
	uns8 m_locked;
};
static_assert(sizeof(c_abstract_button) == 0xC);

struct s_game_input_state
{
	c_abstract_button& get_button(e_button_action button_index);
	const c_abstract_button& get_button(e_button_action button_index) const;

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


	c_abstract_button abstract_buttons[k_button_action_count_keyboard];
	point2d abstract_sticks[2];

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

	// apply_mouse_acceleration?
	bool __unknown324;
};
static_assert(sizeof(s_game_input_state) == 0x328);

struct s_input_globals_definition
{
	int32 __unknown0;
	int32 __unknown4;
	s_tag_data __unknown8;
	s_tag_data __unknown1C;
	int32 __unknown30;
};
static_assert(sizeof(s_input_globals_definition) == 0x34);

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
static_assert(sizeof(s_input_abstraction_globals) == 0x14D4);

extern s_input_abstraction_globals& input_abstraction_globals;
extern bool use_mean_look_sensitivity;

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
extern void __cdecl sub_60C040(int32 keyboard_preset, s_gamepad_input_preferences* preferences);
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

