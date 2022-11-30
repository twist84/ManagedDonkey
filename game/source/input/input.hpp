#pragma once

#include "cseries/cseries.hpp"

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
	// uMsg == WM_KEYDOWN
	// Msg == WM_SYSKEYDOWN
	_key_type_down = 0,

	// uMsg == WM_KEYUP
	// uMsg == WM_SYSKEYUP
	_key_type_up,

	// uMsg == WM_CHAR
	// uMsg == WM_SYSCHAR
	_key_type_char,

	k_key_type_count
};

enum e_mouse_button
{
	// left-click
	_mouse_button_1 = 0,

	// middle-click
	_mouse_button_2,

	// right-click
	_mouse_button_3,

	// mouse 4
	_mouse_button_4,

	// mouse 5
	_mouse_button_5,

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
	// uMsg == WM_MOUSEMOVE
	_mouse_type_move = 0,

	// uMsg == WM_LBUTTONDOWN
	// uMsg == WM_RBUTTONDOWN
	// uMsg == WM_MBUTTONDOWN
	// uMsg == WM_XBUTTONDOWN
	_mouse_type_down,

	// uMsg == WM_LBUTTONUP
	// uMsg == WM_RBUTTONUP
	// uMsg == WM_MBUTTONUP
	// uMsg == WM_XBUTTONUP
	_mouse_type_up,

	// uMsg == WM_MOUSEWHEEL
	_mouse_type_wheel,

	k_mouse_type_count
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

// key_stroke
struct s_key_state
{
	c_flags<e_key_modifier_flags, byte, k_key_modifier_flag_count> modifier;
	c_enum<e_key_type, long, k_key_type_count> key_type;
	c_enum<e_key_code, short, k_key_code_count> key_code;

	// virtual-key code
	// LOWORD(wParam);
	word vk_code;

	// uMsg == WM_KEYDOWN
	// uMsg == WM_SYSKEYDOWN
	// uMsg == WM_CHAR
	// uMsg == WM_SYSCHAR
	// = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
	bool was_key_down;
};
static_assert(sizeof(s_key_state) == 0x10);

struct s_mouse_state
{
	c_enum<e_mouse_type, long, k_mouse_type_count> mouse_type;
	dword x;
	dword y;
	dword wheel_delta;
	c_enum<e_mouse_button, char, _mouse_button_5 + 1> mouse_button;
};
static_assert(sizeof(s_mouse_state) == 0x14);

struct mouse_state
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	c_static_array<byte, 5> frames_down;
	c_static_array<word, 5> msec_down;
	byte_flags flags;
	dword x;
	dword y;
	dword wheel_delta;
};
static_assert(sizeof(mouse_state) == 0x2C);

struct gamepad_state
{
	byte __unknown0[2];
	byte __unknown2[2];
	byte __unknown4[2];

	byte __data[0x36];
};
static_assert(sizeof(gamepad_state) == 0x3C);

struct rumble_state
{
	word left_motor_speed;
	word right_motor_speed;
};
static_assert(sizeof(rumble_state) == 0x4);

struct s_input_globals
{
	bool initialized;
	bool mouse_acquired;
	bool suppressed;
	c_static_array<bool, k_input_type_count> input_type_suppressed;
	bool feedback_suppressed;
	dword time;

	struct
	{
		word __unknown0;
		byte __unknown2;
		byte __unknown3;
	} __unknownC[104];

	short buffered_key_read_index;
	short buffered_key_read_count;
	c_static_array<s_key_state, 64> buffered_keys;

	short buffered_mouse_button_read_index;
	short buffered_mouse_button_read_count;
	c_static_array<s_mouse_state, 64> buffered_mouse_buttons;

	bool __unknownAB4;
	bool __unknownAB5;
	bool __unknownAB6;

	bool raw_input_enabled;
	mouse_state raw_input_mouse_state;
	mouse_state suppressed_mouse_state;

	long __unknownB10;
	long __unknownB14;
	long __unknownB18;
	long __unknownB1C;
	long __unknownB20;
	long __unknownB24;

	long __unknownB28[1];

	c_static_array<gamepad_state, 4> gamepad_states;
	gamepad_state suppressed_gamepad_state;

	c_static_array<rumble_state, 4> rumble_states;

	long __unknownC68;
	long __unknownC6C;
};
static_assert(sizeof(s_input_globals) == 0xC70);

extern bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type);
extern bool __cdecl input_has_gamepad(short gamepad_index);
extern byte __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type);
extern word __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type);
extern byte __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type);
extern word __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type);

extern short const(&virtual_to_key_table)[256];
extern short const(&ascii_to_key_table)[256];
extern s_input_globals& input_globals;

