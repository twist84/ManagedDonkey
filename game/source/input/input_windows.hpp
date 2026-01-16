#pragma once

#include "cseries/cseries.hpp"

const int32 k_number_of_windows_input_virtual_codes = 256;
const int32 k_number_of_input_ascii_codes = 128;

enum e_input_key_code
{
	_key_escape = 0,         // VK_ESCAPE
	_key_f1,                 // VK_F1
	_key_f2,                 // VK_F2
	_key_f3,                 // VK_F3
	_key_f4,                 // VK_F4
	_key_f5,                 // VK_F5
	_key_f6,                 // VK_F6
	_key_f7,                 // VK_F7
	_key_f8,                 // VK_F8
	_key_f9,                 // VK_F9
	_key_f10,                // VK_F10
	_key_f11,                // VK_F11
	_key_f12,                // VK_F12
	_key_print_screen,       // VK_SNAPSHOT
	_key_scroll_lock,        // VK_F14
	_key_pause,              // VK_F15

	_key_backquote,          // VK_OEM_3         ` for US
	_key_1,                  // '1'
	_key_2,                  // '2'
	_key_3,                  // '3'
	_key_4,                  // '4'
	_key_5,                  // '5'
	_key_6,                  // '6'
	_key_7,                  // '7'
	_key_8,                  // '8'
	_key_9,                  // '9'
	_key_0,                  // '0'
	_key_dash,               // VK_OEM_MINUS     - any country
	_key_equal,              // VK_OEM_PLUS      = any country
	_key_backspace,          // VK_BACK

	_key_tab,                // VK_TAB
	_key_q,                  // 'Q'
	_key_w,                  // 'W'
	_key_e,                  // 'E'
	_key_r,                  // 'R'
	_key_t,                  // 'T'
	_key_y,                  // 'Y'
	_key_u,                  // 'U'
	_key_i,                  // 'I'
	_key_o,                  // 'O'
	_key_p,                  // 'P'
	_key_left_bracket,       // VK_OEM_4         [ for US
	_key_right_bracket,      // VK_OEM_6         ] for US
	_key_backslash,          // VK_OEM_5         \ for US

	_key_caps_lock,          // VK_CAPITAL
	_key_a,                  // 'A'
	_key_s,                  // 'S'
	_key_d,                  // 'D'
	_key_f,                  // 'F'
	_key_g,                  // 'G'
	_key_h,                  // 'H'
	_key_j,                  // 'J'
	_key_k,                  // 'K'
	_key_l,                  // 'L'

	_key_code_semicolon,     // VK_OEM_1         ; for US

	_key_apostrophe,         // VK_OEM_7         ' for US
	_key_return,             // VK_RETURN

	_key_left_shift,         // VK_LSHIFT
	_key_z,                  // 'Z'
	_key_x,                  // 'X'
	_key_c,                  // 'C'
	_key_v,                  // 'V'
	_key_b,                  // 'B'
	_key_n,                  // 'N'
	_key_m,                  // 'M'
	_key_comma,              // VK_OEM_COMMA     , any country
	_key_period,             // VK_OEM_PERIOD    . any country
	_key_forwardslash,       // VK_OEM_2         /' for US
	_key_right_shift,        // VK_RSHIFT

	_key_left_control,       // VK_LCONTROL
	_key_left_windows,       // VK_LWIN
	_key_left_alt,           // VK_LMENU
	_key_space,              // VK_SPACE
	_key_right_alt,          // VK_RMENU
	_key_right_windows,      // VK_RWIN
	_key_menu,               // VK_APPS
	_key_right_control,      // VK_RCONTROL

	_key_up_arrow,           // VK_UP
	_key_down_arrow,         // VK_DOWN
	_key_left_arrow,         // VK_LEFT
	_key_right_arrow,        // VK_RIGHT

	_key_insert,             // VK_INSERT
	_key_home,               // VK_HOME
	_key_page_up,            // VK_PRIOR
	_key_delete,             // VK_DELETE
	_key_end,                // VK_END
	_key_page_down,          // VK_NEXT

	_keypad_num_lock,    // VK_NUMLOCK
	_keypad_divide,      // VK_DIVIDE
	_keypad_multiply,    // VK_MULTIPLY
	_keypad_0,           // VK_NUMPAD0
	_keypad_1,           // VK_NUMPAD1
	_keypad_2,           // VK_NUMPAD2
	_keypad_3,           // VK_NUMPAD3
	_keypad_4,           // VK_NUMPAD4
	_keypad_5,           // VK_NUMPAD5
	_keypad_6,           // VK_NUMPAD6
	_keypad_7,           // VK_NUMPAD7
	_keypad_8,           // VK_NUMPAD8
	_keypad_9,           // VK_NUMPAD9
	_keypad_subtract,    // VK_SUBTRACT
	_keypad_add,         // VK_ADD
	_keypad_enter,       // VK_RETURN
	_keypad_decimal,     // VK_DECIMAL

	k_key_code_count,

	_key_shift,
	_key_control,
	_key_windows,
	_key_alt,

	NUMBER_OF_KEYS,

	_key_question_mark = _key_forwardslash,
	_key_tilde = _key_backquote,

	_key_not_a_key = NONE
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
	_mouse_button_6,
	_mouse_button_7,

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

enum e_gamepad_stick
{
	_gamepad_stick_left = 0,
	_gamepad_stick_right,

	NUMBER_OF_GAMEPAD_STICKS,
};

enum e_gamepad_button
{
	FIRST_GAMEPAD_ANALOG_BUTTON = 0x0,
	_gamepad_analog_button_left_trigger = FIRST_GAMEPAD_ANALOG_BUTTON,
	_gamepad_analog_button_right_trigger,
	NUMBER_OF_GAMEPAD_ANALOG_BUTTONS,

	FIRST_GAMEPAD_BINARY_BUTTON = NUMBER_OF_GAMEPAD_ANALOG_BUTTONS,
	_gamepad_binary_button_dpad_up = FIRST_GAMEPAD_BINARY_BUTTON,
	_gamepad_binary_button_dpad_down,
	_gamepad_binary_button_dpad_left,
	_gamepad_binary_button_dpad_right,
	_gamepad_binary_button_start,
	_gamepad_binary_button_back,
	_gamepad_binary_button_left_thumb,
	_gamepad_binary_button_right_thumb,
	_gamepad_binary_button_a,
	_gamepad_binary_button_b,
	_gamepad_binary_button_x,
	_gamepad_binary_button_y,
	_gamepad_binary_button_left_bumper,
	_gamepad_binary_button_right_bumper,
	NUMBER_OF_GAMEPAD_BUTTONS,

	NUMBER_OF_GAMEPAD_BINARY_BUTTONS = NUMBER_OF_GAMEPAD_BUTTONS - FIRST_GAMEPAD_BINARY_BUTTON,
	_gamepad_button_unused = 0xFF
};

enum
{
	// e_gamepad_button
	_button_left_trigger = 0,
	_button_right_trigger,
	_button_dpad_up,
	_button_dpad_down,
	_button_dpad_left,
	_button_dpad_right,
	_button_start,
	_button_back,
	_button_left_stick,
	_button_right_stick,
	_button_a,
	_button_b,
	_button_x,
	_button_y,
	_button_right_bumper,
	_button_left_bumper,

	_button_jump,
	_button_switch_grenade,
	_button_switch_weapon,
	_button_unknown19,
	_button_reload_right,
	_button_use,
	_button_reload_left,
	_button_pick_up_left,
	_button_melee_attack,
	_button_throw_grenade,
	_button_fire_primary,
	_button_fire_secondary,
	_button_fire_tertiary,
	_button_crouch,
	_button_scope_zoom,
	_button_unknown31,
	_button_lean_left,
	_button_lean_right,
	_button_flashlight,
	_button_machinima_lower_weapon_toggle,
	_button_machinima_camera_enable_toggle,
	_button_machinima_camera_control_toggle,
	_button_machinima_camera_debug_toggle,
	_button_general_chat,
	_button_team_chat,
	_button_voice_chat,
	_button_unknown42,
	_button_unknown43,
	_button_use_consumable_1,
	_button_use_consumable_2,
	_button_use_consumable_3,
	_button_use_consumable_4,
	_button_vehicle_boost,
	_button_vehicle_dive,
	_button_vehicle_raise,
	_button_vehicle_accelerate,
	_button_vehicle_reverse,
	_button_vehicle_fire,
	_button_vehicle_alt_fire,
	_button_vehicle_use,
	_button_next_player,
	_button_prev_player,
	_button_unknown58,

	NUMBER_OF_CONTROL_BUTTONS,

	_button_move_forward = NUMBER_OF_CONTROL_BUTTONS,
	_button_move_backward,
	_button_move_left,
	_button_move_right,

	NUMBER_OF_KEYBOARD_CONTROL_BUTTONS
};

struct s_input_key_state
{
	uns16 msec;
	uns8 frames;
	bool latched;
};
COMPILE_ASSERT(sizeof(s_input_key_state) == 0x4);

struct key_stroke
{
	uns8 modifier_flags;
	int8 key_type;
	int16 ascii_code;
	bool repeating;
};
COMPILE_ASSERT(sizeof(key_stroke) == 0x6);

// key_stroke
struct s_key_state
{
	uns8 modifier_flags;
	int32 key_type;
	int16 ascii_code;

	// virtual-key code
	// LOWORD(wParam);
	union
	{
		uns16 vk_code;
		char character[2];
	};

	// uMsg == WM_KEYDOWN
	// uMsg == WM_SYSKEYDOWN
	// uMsg == WM_CHAR
	// uMsg == WM_SYSCHAR
	// = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
	bool repeating;
};
COMPILE_ASSERT(sizeof(s_key_state) == 0x10);

struct s_mouse_state
{
	c_enum<e_mouse_type, int32, _mouse_type_move, k_mouse_type_count> mouse_type;
	uns32 x;
	uns32 y;
	uns32 wheel_delta;
	c_enum<e_mouse_button, int8, _mouse_button_1, k_mouse_button_count> mouse_button;
};
COMPILE_ASSERT(sizeof(s_mouse_state) == 0x14);

struct mouse_state
{
	int32 x;
	int32 y;
	int32 wheel_ticks;
	c_static_array<uns8, k_mouse_button_count> frames_down;
	c_static_array<uns16, k_mouse_button_count> msec_down;

	c_flags<e_mouse_button, uns8, k_mouse_button_count> raw_flags;
	int32 relative_x;
	int32 relative_y;
	int16 wheel_delta; // += TEST_FLAGS(usButtonFlags, RI_MOUSE_WHEEL) ? usButtonData : 0
	int16 hwheel_delta; // += TEST_FLAGS(usButtonFlags, RI_MOUSE_HWHEEL) ? usButtonData : 0
};
COMPILE_ASSERT(sizeof(mouse_state) == 0x2C);

// based on `XINPUT_STATE`
struct gamepad_state
{
	uns8 analog_buttons[NUMBER_OF_GAMEPAD_ANALOG_BUTTONS];
	uns8 analog_button_thresholds[NUMBER_OF_GAMEPAD_ANALOG_BUTTONS];
	uns8 button_frames[NUMBER_OF_GAMEPAD_BUTTONS];
	uns16 button_msec[NUMBER_OF_GAMEPAD_BUTTONS];
	point2d sticks[2];
};
COMPILE_ASSERT(sizeof(gamepad_state) == 0x3C);

// based on `XINPUT_VIBRATION`
struct rumble_state
{
	uns16 left_motor_speed;
	uns16 right_motor_speed;
};
COMPILE_ASSERT(sizeof(rumble_state) == 0x4);

struct s_input_globals
{
	bool initialized;
	bool mouse_acquired;
	bool input_suppressed;
	c_static_array<bool, k_input_type_count> input_type_suppressed;
	bool feedback_suppressed;
	bool focus_mouse; // unused padding
	uns32 update_time;

	c_static_array<s_input_key_state, k_key_code_count> keys;

	int16 buffered_key_read_index;
	int16 buffered_key_read_count;
	c_static_array<s_key_state, 64> buffered_keys;

	int16 buffered_mouse_button_read_index;
	int16 buffered_mouse_button_read_count;
	c_static_array<s_mouse_state, 64> buffered_mouse_buttons;

	bool raw_input_unknownAB4;
	bool raw_input_unknownAB5;
	bool raw_input_unknownAB6;
	bool raw_input_mouse_state_update;

	mouse_state raw_mouse_state;
	mouse_state suppressed_raw_mouse_state;

	// sub_511550
	// sub_511AF0
	int32 mouse_relative_x;  // 1
	int32 mouse_relative_y;  // 1
	int16 mouse_wheel_delta; // 120, WHEEL_DELTA
	int32 mouse_x_ticks;     // 1
	int32 mouse_y_ticks;     // 1
	int32 mouse_wheel_ticks; // 1

	c_static_flags<32> gamepad_valid_mask;
	c_static_array<gamepad_state, k_number_of_controllers> gamepad_states;
	gamepad_state suppressed_gamepad_state;

	c_static_array<rumble_state, k_number_of_controllers> rumble_states;

	uns32 raw_mouse_wheel_update_time;
	int32 __unknownC6C;
};
COMPILE_ASSERT(sizeof(s_input_globals) == 0xC70);

// key_to_virtual_table[_key_escape] = VK_ESCAPE
//extern c_static_array<uns8 const, k_key_code_count>& key_to_virtual_table;
extern uns8 const(&key_to_virtual_table)[k_key_code_count];
//extern uns8 const key_to_virtual_table[k_key_code_count];

// virtual_to_key_table[VK_ESCAPE] = _key_escape
//extern c_static_array<int16 const, k_number_of_windows_input_virtual_codes>& virtual_to_key_table;
extern int16 const(&virtual_to_key_table)[k_number_of_windows_input_virtual_codes];
//extern int16 const virtual_to_key_table[k_number_of_windows_input_virtual_codes];

// key_to_ascii_table[_key_space] = ' '
//extern c_static_array<uns8 const, k_key_code_count>& key_to_ascii_table;
extern uns8 const(&key_to_ascii_table)[k_key_code_count];
//extern uns8 const key_to_ascii_table[k_key_code_count];

// key_to_ascii_table[' '] = _key_space
//extern c_static_array<int16 const, k_number_of_input_ascii_codes>& ascii_to_key_table;
extern int16 const(&ascii_to_key_table)[k_number_of_input_ascii_codes];
//extern int16 const ascii_to_key_table[k_number_of_input_ascii_codes];

extern s_input_globals& input_globals;

extern void __cdecl sub_5113E0(int vKey, e_mouse_button mouse_button);
extern void __cdecl sub_511410();
extern void __cdecl sub_5114A0();
extern void __cdecl sub_511550();
extern void __cdecl input_activate();
extern void __cdecl input_clear_all_rumblers();
extern void __cdecl sub_511620();
extern void __cdecl input_deactivate();
extern void __cdecl input_dispose();
extern void __cdecl sub_511710();
extern void __cdecl sub_511760(int error, const char* format, ...);
extern void __cdecl input_feedback_suppress(bool suppress_feedback);
extern void __cdecl input_flush();
extern const gamepad_state* __cdecl input_get_gamepad_state(int16 gamepad_index);
extern bool __cdecl input_get_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_get_mouse(s_mouse_state* mouse, e_input_type input_type);
extern mouse_state* __cdecl input_get_mouse_state(e_input_type input_type);
extern bool __cdecl input_has_gamepad(int16 gamepad_index);
extern void __cdecl input_initialize();
extern bool __cdecl sub_511AF0();
extern bool __cdecl sub_511B40();
extern bool __cdecl input_type_suppressed(e_input_type input_type);
extern uns8 __cdecl input_key_frames_down(e_input_key_code key_code, e_input_type input_type);
extern uns16 __cdecl input_key_msec_down(e_input_key_code key_code, e_input_type input_type);
extern uns8 __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type);
extern uns16 __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type);
extern bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type);
extern bool __cdecl sub_512450();
extern void __cdecl input_set_gamepad_rumbler_state(int16 gamepad_index, uns16 left_motor_speed, uns16 right_motor_speed);
extern void __cdecl input_suppress_type(e_input_type input_type, bool suppress);
extern void __cdecl input_suppress();
extern void __cdecl sub_5125A0();
extern bool __cdecl sub_512650();
extern void __cdecl input_update();
extern void __cdecl sub_5129B0();
extern void __cdecl input_update_device_connections();
extern void __cdecl input_update_keyboard(int32 elapsed_msec);
extern void __cdecl input_update_mouse(int32 elapsed_msec);
extern void __cdecl input_update_gamepads(int32 elapsed_msec);
extern void __cdecl input_update_gamepads_rumble();
extern void __cdecl update_button(uns8* frames, uns16* msec, bool down, int32 elapsed_msec);
extern void __cdecl update_key(s_input_key_state* key, bool down, int32 elapsed_msec);

extern void input_handle_key_combos();
extern void input_get_raw_data_string(char* buffer, int16 size);
extern void input_mouse_state_get_raw_data_string(char* buffer, int16 size);

