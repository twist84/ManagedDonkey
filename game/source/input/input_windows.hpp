#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

const long k_number_of_windows_input_virtual_codes = 256;
const long k_number_of_input_ascii_codes = 128;

enum e_key_code
{
	_key_code_escape = 0,         // VK_ESCAPE
	_key_code_f1,                 // VK_F1
	_key_code_f2,                 // VK_F2
	_key_code_f3,                 // VK_F3
	_key_code_f4,                 // VK_F4
	_key_code_f5,                 // VK_F5
	_key_code_f6,                 // VK_F6
	_key_code_f7,                 // VK_F7
	_key_code_f8,                 // VK_F8
	_key_code_f9,                 // VK_F9
	_key_code_f10,                // VK_F10
	_key_code_f11,                // VK_F11
	_key_code_f12,                // VK_F12
	_key_code_print_screen,       // VK_SNAPSHOT
	_key_code_f14,                // VK_F14
	_key_code_f15,                // VK_F15

	_key_code_backquote,          // VK_OEM_3         '`~' for US
	_key_code_1,                  // '1'
	_key_code_2,                  // '2'
	_key_code_3,                  // '3'
	_key_code_4,                  // '4'
	_key_code_5,                  // '5'
	_key_code_6,                  // '6'
	_key_code_7,                  // '7'
	_key_code_8,                  // '8'
	_key_code_9,                  // '9'
	_key_code_0,                  // '0'
	_key_code_minus,              // VK_OEM_MINUS     '-' any country
	_key_code_plus,               // VK_OEM_PLUS      '+' any country
	_key_code_backspace,          // VK_BACK

	_key_code_tab,                // VK_TAB
	_key_code_q,                  // 'Q'
	_key_code_w,                  // 'W'
	_key_code_e,                  // 'E'
	_key_code_r,                  // 'R'
	_key_code_t,                  // 'T'
	_key_code_y,                  // 'Y'
	_key_code_u,                  // 'U'
	_key_code_i,                  // 'I'
	_key_code_o,                  // 'O'
	_key_code_p,                  // 'P'
	_key_code_left_parenthesis,   // VK_OEM_4         '[{' for US
	_key_code_right_parenthesis,  // VK_OEM_6         ']}' for US
	_key_code_backslash,          // VK_OEM_5         '\|' for US

	_key_code_caps_lock,          // VK_CAPITAL
	_key_code_a,                  // 'A'
	_key_code_s,                  // 'S'
	_key_code_d,                  // 'D'
	_key_code_f,                  // 'F'
	_key_code_g,                  // 'G'
	_key_code_h,                  // 'H'
	_key_code_j,                  // 'J'
	_key_code_k,                  // 'K'
	_key_code_l,                  // 'L'

	_key_code_colon,              // VK_OEM_1         ';:' for US
	_key_code_semicolon = _key_code_colon,

	_key_code_apostrophe,         // VK_OEM_7         ''"' for US
	_key_code_enter,              // VK_RETURN

	_key_code_left_shift,         // VK_LSHIFT
	_key_code_z,                  // 'Z'
	_key_code_x,                  // 'X'
	_key_code_c,                  // 'C'
	_key_code_v,                  // 'V'
	_key_code_b,                  // 'B'
	_key_code_n,                  // 'N'
	_key_code_m,                  // 'M'
	_key_code_comma,              // VK_OEM_COMMA     ',' any country
	_key_code_dot,                // VK_OEM_PERIOD    '.' any country
	_key_code_slash,              // VK_OEM_2         '/?' for US
	_key_code_right_shift,        // VK_RSHIFT

	_key_code_left_control,       // VK_LCONTROL
	_key_code_left_windows,       // VK_LWIN
	_key_code_left_alt,           // VK_LMENU
	_key_code_spacebar,           // VK_SPACE
	_key_code_right_alt,          // VK_RMENU
	_key_code_right_windows,      // VK_RWIN
	_key_code_context_menu,       // VK_APPS
	_key_code_right_control,      // VK_RCONTROL

	_key_code_up,                 // VK_UP
	_key_code_down,               // VK_DOWN
	_key_code_left,               // VK_LEFT
	_key_code_right,              // VK_RIGHT

	_key_code_insert,             // VK_INSERT
	_key_code_home,               // VK_HOME
	_key_code_page_up,            // VK_PRIOR
	_key_code_delete,             // VK_DELETE
	_key_code_end,                // VK_END
	_key_code_page_down,          // VK_NEXT

	_key_code_keypad_num_lock,    // VK_NUMLOCK
	_key_code_keypad_divide,      // VK_DIVIDE
	_key_code_keypad_multiply,    // VK_MULTIPLY
	_key_code_keypad_0,           // VK_NUMPAD0
	_key_code_keypad_1,           // VK_NUMPAD1
	_key_code_keypad_2,           // VK_NUMPAD2
	_key_code_keypad_3,           // VK_NUMPAD3
	_key_code_keypad_4,           // VK_NUMPAD4
	_key_code_keypad_5,           // VK_NUMPAD5
	_key_code_keypad_6,           // VK_NUMPAD6
	_key_code_keypad_7,           // VK_NUMPAD7
	_key_code_keypad_8,           // VK_NUMPAD8
	_key_code_keypad_9,           // VK_NUMPAD9
	_key_code_keypad_subtract,    // VK_SUBTRACT
	_key_code_keypad_add,         // VK_ADD
	_key_code_keypad_enter,       // VK_RETURN
	_key_code_keypad_decimal,     // VK_DECIMAL

	k_key_code_count,

	_key_code_shift,
	_key_code_control,
	_key_code_windows,
	_key_code_alt,

	k_total_key_code_count,
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

	k_key_modifier_flags
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

	k_key_type_count,

	k_key_type_none = NONE
};

enum e_mouse_button
{
	_mouse_button_1 = 0,
	_mouse_button_left_click = _mouse_button_1,

	_mouse_button_2,
	_mouse_button_middle_click = _mouse_button_2,

	_mouse_button_3,
	_mouse_button_right_click = _mouse_button_3,

	// mouse 4
	_mouse_button_4,

	// mouse 5
	_mouse_button_5,

	k_mouse_button_count,
	_mouse_button_7,
	_mouse_button_8,

	_mouse_button_wheel_up,
	_mouse_button_wheel_down,

	k_total_mouse_button_count,
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
	k_controller_button_none = NONE
};

enum e_button_action
{
	// e_controller_button
	_button_action_left_trigger = 0,
	_button_action_right_trigger,
	_button_action_dpad_up,
	_button_action_dpad_down,
	_button_action_dpad_left,
	_button_action_dpad_right,
	_button_action_start,
	_button_action_back,
	_button_action_left_stick,
	_button_action_right_stick,
	_button_action_a,
	_button_action_b,
	_button_action_x,
	_button_action_y,
	_button_action_right_bumper,
	_button_action_left_bumper,

	_button_action_jump,
	_button_action_switch_grenades,
	_button_action_switch_weapons,
	_button_action_unknown19,
	_button_action_reload_right,
	_button_action_use,
	_button_action_reload_left,
	_button_action_pick_up_left,
	_button_action_melee,
	_button_action_throw_grenade,
	_button_action_fire_right,
	_button_action_fire_left,

	// fires a weapon with a trigger bound to the melee key
	_button_action_melee_fire,

	_button_action_crouch,
	_button_action_zoom,
	_button_action_unknown31,
	_button_action_lean_left,
	_button_action_lean_right, // sprint
	_button_action_flashlight,
	_button_action_unknown35,
	_button_action_unknown36,
	_button_action_unknown37,
	_button_action_unknown38,
	_button_action_general_chat,
	_button_action_team_chat,
	_button_action_voice_chat,
	_button_action_unknown42,
	_button_action_unknown43,
	_button_action_use_consumable_1,
	_button_action_use_consumable_2,
	_button_action_use_consumable_3,
	_button_action_use_consumable_4,
	_button_action_vehicle_boost,
	_button_action_vehicle_dive,
	_button_action_vehicle_raise,
	_button_action_vehicle_accelerate,
	_button_action_vehicle_reverse,
	_button_action_vehicle_fire,
	_button_action_vehicle_alt_fire,
	_button_action_vehicle_use,
	_button_action_next_player,
	_button_action_prev_player,
	_button_action_unknown58,

	k_button_action_count,

	_button_action_move_forward = k_button_action_count,
	_button_action_move_back,
	_button_action_move_left,
	_button_action_move_right,

	k_button_action_count_keyboard
};

struct key_state
{
	word msec_down;
	byte frames_down;

	// Set in `sub_511C50`, `sub_511C50` is only referenced by itself
	bool __unknown3;
};
static_assert(sizeof(key_state) == 0x4);

struct key_stroke
{
	c_flags<e_key_modifier_flags, byte, k_key_modifier_flags> modifier;
	c_enum<e_key_type, char, _key_type_down, k_key_type_count> key_type;
	c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count> key_code;
	bool was_key_down;
};
static_assert(sizeof(key_stroke) == 0x6);

// key_stroke
struct s_key_state
{
	c_flags<e_key_modifier_flags, byte, k_key_modifier_flags> modifier;
	c_enum<e_key_type, long, _key_type_down, k_key_type_count> key_type;
	c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count> key_code;

	// virtual-key code
	// LOWORD(wParam);
	union
	{
		word vk_code;
		char character[2];
	};

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
	c_enum<e_mouse_type, long, _mouse_type_move, k_mouse_type_count> mouse_type;
	dword x;
	dword y;
	dword wheel_delta;
	c_enum<e_mouse_button, char, _mouse_button_1, k_mouse_button_count> mouse_button;
};
static_assert(sizeof(s_mouse_state) == 0x14);

struct mouse_state
{
	long x;
	long y;
	long wheel_ticks;
	c_static_array<byte, k_mouse_button_count> frames_down;
	c_static_array<word, k_mouse_button_count> msec_down;

	c_flags<e_mouse_button, byte, k_mouse_button_count> raw_flags;
	long relative_x;
	long relative_y;
	long wheel_delta; // += TEST_FLAGS(usButtonFlags, RI_MOUSE_WHEEL) ? usButtonData : 0
};
static_assert(sizeof(mouse_state) == 0x2C);

enum e_xinput_gamepad
{
	_xinput_gamepad_dpad_up = 0,
	_xinput_gamepad_dpad_down,
	_xinput_gamepad_dpad_left,
	_xinput_gamepad_dpad_right,
	_xinput_gamepad_start,
	_xinput_gamepad_back,
	_xinput_gamepad_left_thumb,
	_xinput_gamepad_right_thumb,
	_xinput_gamepad_left_shoulder,
	_xinput_gamepad_right_shoulder,
	_xinput_gamepad_a,
	_xinput_gamepad_b,
	_xinput_gamepad_x,
	_xinput_gamepad_y,

	k_xinput_gamepad_count
};

// based on `XINPUT_STATE`
struct gamepad_state
{
	c_static_array<byte, 2> trigger_down_amount;
	c_static_array<byte, 2> max_trigger_down_amount;
	c_static_array<byte, 2> trigger_down_frames;
	c_static_array<byte, k_xinput_gamepad_count> buttons_down_frames;
	c_static_array<word, k_xinput_gamepad_count> buttons_down_msec;

	int16_point2d thumb_left;
	int16_point2d thumb_right;

	dword __unknown38;
};
static_assert(sizeof(gamepad_state) == 0x3C);

// based on `XINPUT_VIBRATION`
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
	bool input_suppressed;
	c_static_array<bool, k_input_type_count> input_type_suppressed;
	bool feedback_suppressed;
	dword update_time;

	c_static_array<key_state, k_key_code_count> keys;

	short buffered_key_read_index;
	short buffered_key_read_count;
	c_static_array<s_key_state, 64> buffered_keys;

	short buffered_mouse_button_read_index;
	short buffered_mouse_button_read_count;
	c_static_array<s_mouse_state, 64> buffered_mouse_buttons;

	bool raw_input_unknownAB4;
	bool raw_input_unknownAB5;
	bool raw_input_unknownAB6;
	bool raw_input_mouse_state_update;

	mouse_state raw_mouse_state;
	mouse_state suppressed_raw_mouse_state;

	// sub_511550
	// sub_511AF0
	long mouse_relative_x;  // 1
	long mouse_relative_y;  // 1
	long mouse_wheel_delta; // 120, WHEEL_DELTA
	long mouse_x_ticks;     // 1
	long mouse_y_ticks;     // 1
	long mouse_wheel_ticks; // 1

	c_static_flags<32> gamepad_valid_mask;
	c_static_array<gamepad_state, k_number_of_controllers> gamepad_states;
	gamepad_state suppressed_gamepad_state;

	c_static_array<rumble_state, k_number_of_controllers> rumble_states;

	long raw_mouse_wheel_update_time;
	long __unknownC6C;
};
static_assert(sizeof(s_input_globals) == 0xC70);

// key_to_virtual_table[_key_code_escape] = VK_ESCAPE
//extern c_static_array<byte const, k_key_code_count>& key_to_virtual_table;
extern byte const(&key_to_virtual_table)[k_key_code_count];
//extern byte const key_to_virtual_table[k_key_code_count];

// virtual_to_key_table[VK_ESCAPE] = _key_code_escape
//extern c_static_array<short const, k_number_of_windows_input_virtual_codes>& virtual_to_key_table;
extern short const(&virtual_to_key_table)[k_number_of_windows_input_virtual_codes];
//extern short const virtual_to_key_table[k_number_of_windows_input_virtual_codes];

// key_to_ascii_table[_key_code_spacebar] = ' '
//extern c_static_array<byte const, k_key_code_count>& key_to_ascii_table;
extern byte const(&key_to_ascii_table)[k_key_code_count];
//extern byte const key_to_ascii_table[k_key_code_count];

// key_to_ascii_table[' '] = _key_code_spacebar
//extern c_static_array<short const, k_number_of_input_ascii_codes>& ascii_to_key_table;
extern short const(&ascii_to_key_table)[k_number_of_input_ascii_codes];
//extern short const ascii_to_key_table[k_number_of_input_ascii_codes];

extern s_input_globals& input_globals;

extern void __cdecl sub_5113E0(int vKey, e_mouse_button mouse_button);
extern void __cdecl sub_511410();
extern void __cdecl sub_5114A0();
extern void __cdecl sub_511550();
extern void __cdecl sub_5115A0();
extern void __cdecl input_clear_all_rumblers();
extern void __cdecl sub_511620();
extern void __cdecl sub_5116A0();
extern void __cdecl input_dispose();
extern void __cdecl sub_511710();
extern void __cdecl sub_511760(int error, char const* format, ...);
extern void __cdecl input_feedback_suppress(bool suppress_feedback);
extern void __cdecl input_flush();
extern gamepad_state const* __cdecl input_get_gamepad_state(short gamepad_index);
extern bool __cdecl input_get_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_get_mouse(s_mouse_state* mouse, e_input_type input_type);
extern mouse_state* __cdecl input_get_mouse_state(e_input_type input_type);
extern bool __cdecl input_has_gamepad(short gamepad_index);
extern void __cdecl input_initialize();
extern bool __cdecl sub_511AF0();
extern bool __cdecl sub_511B40();
extern bool __cdecl input_type_suppressed(e_input_type input_type);
extern byte __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type);
extern word __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type);
extern byte __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type);
extern word __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type);
extern bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type);
extern bool __cdecl sub_512450();
extern void __cdecl input_set_gamepad_rumbler_state(short gamepad_index, word left_motor_speed, word right_motor_speed);
extern void __cdecl input_suppress_type(e_input_type input_type, bool suppress);
extern void __cdecl input_suppress();
extern void __cdecl sub_5125A0();
extern bool __cdecl sub_512650();
extern void __cdecl input_update();
extern void __cdecl sub_5129B0();
extern void __cdecl input_update_gamepads_rumble();
extern void __cdecl update_button(byte* frames_down, word* msec_down, bool key_down, long duration_ms);
extern void __cdecl update_key(key_state* key, bool key_down, long duration_ms);

extern void input_get_raw_data_string(char* buffer, short size);
extern void input_mouse_state_get_raw_data_string(char* buffer, short size);

