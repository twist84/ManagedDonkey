#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"

enum e_key_code
{
	_key_code_escape = 0,
	_key_code_f1,
	_key_code_f2,
	_key_code_f3,
	_key_code_f4,
	_key_code_f5,
	_key_code_f6,
	_key_code_f7,
	_key_code_f8,
	_key_code_f9,
	_key_code_f10,
	_key_code_f11,
	_key_code_f12,

	_key_code_print_screen,
	_key_code_scroll_lock,
	_key_code_pause,
	_key_code_backquote,

	_key_code_1,
	_key_code_2,
	_key_code_3,
	_key_code_4,
	_key_code_5,
	_key_code_6,
	_key_code_7,
	_key_code_8,
	_key_code_9,
	_key_code_0,
	_key_code_minus,
	_key_code_equal,
	_key_code_backspace,

	_key_code_tab,
	_key_code_q,
	_key_code_w,
	_key_code_e,
	_key_code_r,
	_key_code_t,
	_key_code_y,
	_key_code_u,
	_key_code_i,
	_key_code_o,
	_key_code_p,
	_key_code_left_parenthesis,
	_key_code_right_parenthesis,
	_key_code_backslash,

	_key_code_caps_lock,
	_key_code_a,
	_key_code_s,
	_key_code_d,
	_key_code_f,
	_key_code_g,
	_key_code_h,
	_key_code_j,
	_key_code_k,
	_key_code_l,
	_key_code_semicolon,
	_key_code_apostrophe,
	_key_code_enter,

	_key_code_left_shift,
	_key_code_z,
	_key_code_x,
	_key_code_c,
	_key_code_v,
	_key_code_b,
	_key_code_n,
	_key_code_m,
	_key_code_comma,
	_key_code_dot,
	_key_code_slash,
	_key_code_right_shift,

	_key_code_left_control,
	_key_code_left_windows,
	_key_code_left_alt,
	_key_code_spacebar,
	_key_code_right_alt,
	_key_code_right_windows,
	_key_code_context_menu,
	_key_code_right_control,

	_key_code_up,
	_key_code_down,
	_key_code_left,
	_key_code_right,

	_key_code_ins,
	_key_code_home,
	_key_code_page_up,
	_key_code_delete,
	_key_code_end,
	_key_code_page_down,

	_key_code_keypad_num_lock,
	_key_code_keypad_divide,
	_key_code_keypad_multiply,
	_key_code_keypad_0,
	_key_code_keypad_1,
	_key_code_keypad_2,
	_key_code_keypad_3,
	_key_code_keypad_4,
	_key_code_keypad_5,
	_key_code_keypad_6,
	_key_code_keypad_7,
	_key_code_keypad_8,
	_key_code_keypad_9,
	_key_code_keypad_subtract,
	_key_code_keypad_add,
	_key_code_keypad_enter,
	_key_code_keypad_decimal,

	_key_code_unknown104,

	_key_code_shift,
	_key_code_control,
	_key_code_windows,
	_key_code_alt,

	k_key_code_count,
	k_key_code_none = -1
};

enum e_mouse_button
{
	_mouse_button_1 = 0, // left
	_mouse_button_2,     // middle
	_mouse_button_3,     // right
	_mouse_button_4,     // wheel
	_mouse_button_5,
	_mouse_button_6,
	_mouse_button_7,
	_mouse_button_8,

	_mouse_button_up,
	_mouse_button_down,

	k_mouse_button_count,
	k_mouse_button_none = -1
};

enum e_controller_button
{
	_controller_button_left_trigger = 0,
	_controller_button_right_trigger,
	_controller_button_dpad_up,
	_controller_button_dpad_down,
	_controller_button_dpad_left,
	_controller_button_dpad_right,
	_controller_button_start,
	_controller_button_back,
	_controller_button_left_stick,
	_controller_button_right_stick,
	_controller_button_a,
	_controller_button_b,
	_controller_button_x,
	_controller_button_y,
	_controller_button_right_bumper,
	_controller_button_left_bumper,

	k_controller_button_count,
	k_controller_button_none
};

enum e_gamepad_button
{
	// e_controller_button
	_gamepad_button_left_trigger = 0,
	_gamepad_button_right_trigger,
	_gamepad_button_dpad_up,
	_gamepad_button_dpad_down,
	_gamepad_button_dpad_left,
	_gamepad_button_dpad_right,
	_gamepad_button_start,
	_gamepad_button_back,
	_gamepad_button_left_stick,
	_gamepad_button_right_stick,
	_gamepad_button_a,
	_gamepad_button_b,
	_gamepad_button_x,
	_gamepad_button_y,
	_gamepad_button_right_bumper,
	_gamepad_button_left_bumper,

	_gamepad_button_jump,
	_gamepad_button_switch_grenades,
	_gamepad_button_switch_weapons,
	_gamepad_button_unknown19,
	_gamepad_button_reload_right,
	_gamepad_button_use,
	_gamepad_button_reload_left,
	_gamepad_button_pick_up_left,
	_gamepad_button_melee,
	_gamepad_button_throw_grenade,
	_gamepad_button_fire_right,
	_gamepad_button_fire_left,

	// fires a weapon with a trigger bound to the melee key
	_gamepad_button_melee_fire,

	_gamepad_button_crouch,
	_gamepad_button_zoom,
	_gamepad_button_unknown31,
	_gamepad_button_unknown32,
	_gamepad_button_sprint,
	_gamepad_button_flashlight,
	_gamepad_button_unknown35,
	_gamepad_button_unknown36,
	_gamepad_button_unknown37,
	_gamepad_button_unknown38,
	_gamepad_button_general_chat,
	_gamepad_button_team_chat,
	_gamepad_button_voice_chat,
	_gamepad_button_unknown42,
	_gamepad_button_unknown43,
	_gamepad_button_use_consumable_1,
	_gamepad_button_use_consumable_2,
	_gamepad_button_use_consumable_3,
	_gamepad_button_use_consumable_4,
	_gamepad_button_vehicle_boost,
	_gamepad_button_vehicle_dive,
	_gamepad_button_vehicle_raise,
	_gamepad_button_vehicle_accelerate,
	_gamepad_button_vehicle_reverse,
	_gamepad_button_vehicle_fire,
	_gamepad_button_vehicle_alt_fire,
	_gamepad_button_vehicle_use,
	_gamepad_button_next_player,
	_gamepad_button_prev_player,
	_gamepad_button_unknown58,

	k_gamepad_button_count,

	_gamepad_button_move_forward = k_gamepad_button_count,
	_gamepad_button_move_back,
	_gamepad_button_move_left,
	_gamepad_button_move_right,

	k_gamepad_button_count_keyboard
};

struct s_gamepad_input_preferences
{
	real look_sensitivity_x;
	real look_sensitivity_y;

	c_static_array<c_enum<e_controller_button, char, k_controller_button_count>, k_gamepad_button_count> gamepad_buttons;
	c_static_array<bool, k_gamepad_button_count> gamepad_buttons_held;

	c_static_array<c_enum<e_key_code, short, k_key_code_count>, k_gamepad_button_count_keyboard> keys_primary;
	c_static_array<c_enum<e_key_code, short, k_key_code_count>, k_gamepad_button_count_keyboard> keys_secondary;
	c_static_array<c_enum<e_mouse_button, char, k_mouse_button_count>, k_gamepad_button_count_keyboard> mouse_buttons_primary;
	c_static_array<c_enum<e_mouse_button, char, k_mouse_button_count>, k_gamepad_button_count_keyboard> mouse_buttons_secondary;

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
	point2d abstract_sticks[2];
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

struct s_input_globals;
struct s_input_abstraction_globals
{
	s_gamepad_input_preferences preferences[k_number_of_controllers];
	s_game_input_state input_states[k_number_of_controllers];
	dword controller_detection_timer;
	bool input_has_gamepad[k_number_of_controllers];
	long controls_method;
	bool input_device_changed;

	// 'inpg'
	s_input_globals* input_globals;
};
static_assert(sizeof(s_input_abstraction_globals) == 0x14D4);

extern s_input_abstraction_globals& input_abstraction_globals;

struct s_input_globals
{
	long __unknown0;
	long __unknown4;
	tag_data __unknown8;
	tag_data __unknown1C;
	long __unknown30;
};
static_assert(sizeof(s_input_globals) == 0x34);

