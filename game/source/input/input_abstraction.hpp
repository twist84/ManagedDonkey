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
	_key_code_f13, // print screen
	_key_code_f14,
	_key_code_f15,

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

	_key_code_insert,
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

enum e_input_type
{
	_input_type_ui = 0,
	_input_type_game,
	_input_type_special,

	k_input_type_count
};

enum e_key_modifier_flags
{
	// GetKeyState(VK_SHIFT)
	_key_modifier_flag_shift_key_bit = 0,

	// GetKeyState(VK_CONTROL)
	_key_modifier_flag_control_key_bit,

	// GetKeyState(VK_MENU)
	_key_modifier_flag_alt_key_bit,

	k_key_modifier_flag_count
};

enum e_key_type
{
	// message == WM_KEYDOWN
	// message == WM_SYSKEYDOWN
	_key_type_down = 0,

	// message == WM_KEYUP
	// message == WM_SYSKEYUP
	_key_type_up,

	// message == WM_CHAR
	// message == WM_SYSCHAR
	_key_type_char,

	k_key_type_count
};

// key_stroke
struct s_key_state
{
	c_flags<e_key_modifier_flags, byte, k_key_modifier_flag_count> modifier;
	c_enum<e_key_type, long, k_key_type_count> key_type;
	c_enum<e_key_code, short, k_key_code_count> key_code;

	// virtual-key code
	// LOWORD(wParam);
	word vk_code;

	// if WM_KEYDOWN or WM_SYSKEYDOWN or WM_CHAR or WM_SYSCHAR
	// = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
	bool was_key_down;
};
static_assert(sizeof(s_key_state) == 0x10);

enum e_mouse_button
{
	_mouse_button_1 = 0, // left-click
	_mouse_button_2,     // middle-click
	_mouse_button_3,     // right-click
	_mouse_button_4,     // mouse 4
	_mouse_button_5,     // mouse 5
	_mouse_button_6,
	_mouse_button_7,
	_mouse_button_8,

	_mouse_button_wheel_up,
	_mouse_button_wheel_down,

	k_mouse_button_count,
	k_mouse_button_none = -1
};

enum e_mouse_type
{
	// WM_MOUSEMOVE
	_mouse_type_move = 0,

	// WM_LBUTTONDOWN
	// WM_RBUTTONDOWN
	// WM_MBUTTONDOWN
	// WM_XBUTTONDOWN
	_mouse_type_down,

	// WM_LBUTTONUP
	// WM_RBUTTONUP
	// WM_MBUTTONUP
	// WM_XBUTTONUP
	_mouse_type_up,

	// WM_MOUSEWHEEL
	_mouse_type_wheel,

	k_mouse_type_count
};

struct s_mouse_state
{
	c_enum<e_mouse_type, long, k_mouse_type_count> mouse_type;
	dword x;
	dword y;
	dword wheel_delta;
	c_enum<e_mouse_button, char, k_mouse_button_count> mouse_button;
};
static_assert(sizeof(s_mouse_state) == 0x14);

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

struct s_input_globals
{
	long __unknown0;
	long __unknown4;
	tag_data __unknown8;
	tag_data __unknown1C;
	long __unknown30;
};
static_assert(sizeof(s_input_globals) == 0x34);

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

extern void __cdecl input_abstraction_dispose();
extern void __cdecl input_abstraction_dispose_from_old_map();
extern void __cdecl sub_60B3D0(struct gamepad_state* state, s_gamepad_input_preferences* preferences, s_game_input_state* input_state, long gamepad_index);
extern short __cdecl input_abstraction_get_abstract_stick_pitch(long controller_index);
extern long __cdecl input_abstraction_get_controls_method();
extern void __cdecl sub_60BEA0(s_gamepad_input_preferences* preferences, void* bindings);
extern void __cdecl input_abstraction_get_controller_preferences(long controller_index, s_gamepad_input_preferences* preferences);
extern void __cdecl input_abstraction_get_default_preferences(s_gamepad_input_preferences* preferences);
extern void input_abstraction_get_input_state(long controller_index, s_game_input_state** input_state);
extern void __cdecl input_abstraction_get_player_look_angular_velocity(long controller_index, real_euler_angles2d* angular_velocity);
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
