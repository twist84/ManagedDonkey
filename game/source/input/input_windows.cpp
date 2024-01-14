#include "input/input_windows.hpp"

#include "cseries/cseries.hpp"
#include "editor/editor_stubs.hpp"
#include "memory/module.hpp"
#include "shell/shell_windows.hpp"

//#include <Windows.h> // for `key_to_virtual_table`

REFERENCE_DECLARE_ARRAY(0x01650918, byte const, key_to_virtual_table, k_key_code_count);
REFERENCE_DECLARE_ARRAY(0x01650980, short const, virtual_to_key_table, k_number_of_windows_input_virtual_codes);
REFERENCE_DECLARE_ARRAY(0x01650B80, byte const, key_to_ascii_table, k_key_code_count);
REFERENCE_DECLARE_ARRAY(0x01650BE8, short const, ascii_to_key_table, k_number_of_input_ascii_codes);
REFERENCE_DECLARE(0x0238DBE8, s_input_globals, input_globals);

c_static_array<debug_gamepad_data, 4> g_debug_gamepad_data = {};

void __cdecl sub_5113E0(int vKey, e_mouse_button mouse_button)
{
	INVOKE(0x005113E0, sub_5113E0, vKey, mouse_button);

	//input_globals.raw_mouse_state.raw_flags.set(mouse_button, (GetAsyncKeyState(vKey) & 0xFFFE) != 0);
}

void __cdecl sub_511410()
{
	INVOKE(0x00511410, sub_511410);

	//sub_5113E0(VK_LBUTTON, _mouse_button_1);
	//sub_5113E0(VK_RBUTTON, _mouse_button_2);
	//sub_5113E0(VK_MBUTTON, _mouse_button_3);
	//sub_5113E0(VK_XBUTTON1, _mouse_button_4);
	//sub_5113E0(VK_XBUTTON2, _mouse_button_5);
}

void __cdecl sub_5114A0()
{
	INVOKE(0x005114A0, sub_5114A0);
}

void __cdecl sub_511550()
{
	INVOKE(0x00511550, sub_511550);

	//if (!input_globals.raw_input_mouse_state_update)
	//	sub_511AF0();
}

void __cdecl sub_5115A0()
{
	INVOKE(0x005115A0, sub_5115A0);

	//input_globals.update_time = system_milliseconds();
	//input_globals.mouse_acquired = true;
	//if (input_globals.raw_input_mouse_state_update)
	//	sub_5114A0();
}

void __cdecl input_clear_all_rumblers()
{
	INVOKE(0x005115C0, input_clear_all_rumblers);
}

void __cdecl sub_511620()
{
	INVOKE(0x00511620, sub_511620);
}

void __cdecl sub_5116A0()
{
	INVOKE(0x005116A0, sub_5116A0);

	//if (input_globals.raw_input_mouse_state_update)
	//	sub_5125A0();
	//input_globals.mouse_acquired = false;
}

void __cdecl input_dispose()
{
	INVOKE(0x005116C0, input_dispose);

	//sub_511710();
	//sub_65EEB0();
	//input_globals.initialized = false;
}

void __cdecl sub_511710()
{
	INVOKE(0x00511710, sub_511710);

	//if (input_globals.raw_input_mouse_state_update)
	//{
	//	input_globals.raw_input_unknownAB4 = false;
	//	sub_512650();
	//	input_globals.raw_input_mouse_state_update = false;
	//}
}

//.text:00511760

void __cdecl input_feedback_suppress(bool suppress_feedback)
{
	INVOKE(0x005117A0, input_feedback_suppress, suppress_feedback);
}

void __cdecl input_flush()
{
	INVOKE(0x005117B0, input_flush);
}

gamepad_state const* __cdecl input_get_gamepad_state(short gamepad_index)
{
	return INVOKE(0x00511840, input_get_gamepad_state, gamepad_index);
}

bool __cdecl input_get_key(s_key_state* key, e_input_type input_type)
{
	return INVOKE(0x005118C0, input_get_key, key, input_type);
}

// Because of the way the game handles input this function won't actually run when the game is tabbed in
// In the window proc the `WM_INPUT` message is handled with a `RawInput` function
// Raw input is great but I want to peek the damn mouse state
bool __cdecl input_get_mouse(s_mouse_state* mouse, e_input_type input_type)
{
	return INVOKE(0x00511990, input_get_mouse, mouse, input_type);
}

mouse_state* __cdecl input_get_mouse_state(e_input_type input_type)
{
	return INVOKE(0x005119F0, input_get_mouse_state, input_type);
}

bool __cdecl input_has_gamepad(short gamepad_index)
{
	ASSERT(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	return INVOKE(0x00511A40, input_has_gamepad, gamepad_index);
}

void __cdecl input_initialize()
{
	INVOKE(0x00511A90, input_initialize);

	//if (shell_application_type() == _shell_application_type_client)
	//	sub_511550();
	//
	//if (shell_application_type() == _shell_application_type_client || shell_tool_type() == _shell_tool_type_guerilla)
	//	sub_5115A0();
	//
	//sub_65EF00();
	//input_globals.initialized = true;
}

bool __cdecl sub_511AF0()
{
	return INVOKE(0x00511AF0, sub_511AF0);

	//input_globals.raw_input_unknownAB6 = 0;
	//input_globals.raw_input_mouse_state_update = 1;
	//input_globals.mouse_relative_x = 1;
	//input_globals.mouse_relative_y = 1;
	//input_globals.mouse_wheel_delta = WHEEL_DELTA;
	//input_globals.mouse_x_ticks = 1;
	//input_globals.mouse_y_ticks = 1;
	//input_globals.mouse_wheel_ticks = 1;
}

bool __cdecl sub_511B40()
{
	return INVOKE(0x00511B40, sub_511B40);
}

bool __cdecl input_type_suppressed(e_input_type input_type)
{
	return INVOKE(0x00511B50, input_type_suppressed, input_type);

	//return input_globals.input_type_suppressed[input_type];
}

byte __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511B60, input_key_frames_down, key_code, input_type);
}

word __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511CE0, input_key_msec_down, key_code, input_type);
}

byte __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511DF0, input_mouse_frames_down, mouse_button, input_type);

	//bool suppressed = input_globals.input_type_suppressed[input_type] || input_globals.suppressed;
	//if (!input_globals.raw_input_mouse_state_update || suppressed)
	//	return false;
	//
	//return input_globals.raw_mouse_state.frames_down[mouse_button];
}

word __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511E30, input_mouse_msec_down, mouse_button, input_type);

	//bool suppressed = input_globals.input_type_suppressed[input_type] || input_globals.suppressed;
	//if (!input_globals.raw_input_mouse_state_update || suppressed)
	//	return false;
	//
	//return input_globals.raw_mouse_state.msec_down[mouse_button];
}

bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type)
{
	return INVOKE(0x00511E70, input_peek_key, key, input_type);

	//if (input_globals.input_type_suppressed[input_type] || input_globals.buffered_key_read_index >= input_globals.buffered_key_read_count)
	//	return false;
	//
	//*key = input_globals.buffered_keys[input_globals.buffered_key_read_index];
	//return true;
}

// Because of the way the game handles input this function won't actually run when the game is tabbed in
// In the window proc the `WM_INPUT` message is handled with a `RawInput` function
// Raw input is great but I want to peek the damn mouse state
bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type)
{
	return INVOKE(0x00511EC0, input_peek_mouse, mouse, input_type);
}

void __cdecl input_set_gamepad_rumbler_state(short gamepad_index, word left_motor_speed, word right_motor_speed)
{
	INVOKE(0x005124F0, input_set_gamepad_rumbler_state, gamepad_index, left_motor_speed, right_motor_speed);

	//if (user_interface_controller_get_rumble_enabled())
	//{
	//	input_globals.rumble_states[gamepad_index].left_motor_speed = left_motor_speed;
	//	input_globals.rumble_states[gamepad_index].right_motor_speed = right_motor_speed;
	//}
	//else
	//{
	//	input_globals.rumble_states[gamepad_index].left_motor_speed = 0;
	//	input_globals.rumble_states[gamepad_index].right_motor_speed = 0;
	//}
}

void __cdecl input_suppress_type(e_input_type input_type, bool suppress)
{
	INVOKE(0x00512530, input_suppress_type, input_type, suppress);

	//bool suppressed = input_globals.input_type_suppressed[input_type] == suppress;
	//input_globals.input_type_suppressed[input_type] = suppress;
	//
	//if (input_type == _input_type_game && !suppressed)
	//{
	//	if (suppress)
	//	{
	//		input_globals.raw_input_unknownAB6 = true;
	//		sub_5125A0();
	//	}
	//	else
	//	{
	//		input_globals.raw_input_unknownAB6 = 0;
	//		if (!game_in_editor() && g_windows_params.created_window_handle == GetForegroundWindow())
	//			sub_5114A0();
	//	}
	//}
}

void __cdecl input_suppress()
{
	INVOKE(0x00512590, input_suppress);

	//input_globals.suppressed = true;
}

void __cdecl sub_5125A0()
{
	INVOKE(0x005125A0, sub_5125A0);
}

void __cdecl sub_512650()
{
	INVOKE(0x00512650, sub_512650);
}

void __cdecl input_update()
{
	INVOKE(0x00512690, input_update);
}

void __cdecl sub_65EEB0()
{
	INVOKE(0x0065EE80, sub_65EEB0);
}

//.text:0065EEE0 ; DWORD __cdecl sub_65EEE0(DWORD dwUserIndex, XINPUT_STATE *pState)

void __cdecl sub_65EF00()
{
	INVOKE(0x0065EF00, sub_65EF00);
}

bool __cdecl input_xinput_update_gamepad(dword gamepad_index, dword a2, gamepad_state* state, debug_gamepad_data* out_debug_gamepad_data)
{
	bool result = INVOKE(0x0065EF60, input_xinput_update_gamepad, gamepad_index, a2, state, out_debug_gamepad_data);
	if (result)
	{
		if (!out_debug_gamepad_data)
			out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];

		out_debug_gamepad_data->thumb_left = state->thumb_left;
		out_debug_gamepad_data->thumb_right = state->thumb_right;
	}
	return result;
}
HOOK_DECLARE_CALL(0x005128FB, input_xinput_update_gamepad);

void input_get_raw_data_string(char* buffer, short size)
{
	// For some reason buffer is displayed incorrectly for both H3EK and Donkey
	// commented out for now

	//ASSERT(buffer);
	//ASSERT(size > 0);
	//
	//if (buffer && size > 0)
	//{
	//	csnzprintf(buffer, size, "|n|n|n|ngamepad|tleft stick|tright stick|t|n");
	//	for (short gamepad_index = 0; gamepad_index < k_number_of_controllers; gamepad_index++)
	//	{
	//		if (input_globals.gamepad_valid_mask.test(gamepad_index))
	//		{
	//			csnzappendf(buffer, size, "gamepad %d|t(%d, %d)|t(%d, %d)|n",
	//				gamepad_index,
	//				g_debug_gamepad_data[gamepad_index].thumb_left.x,
	//				g_debug_gamepad_data[gamepad_index].thumb_left.y,
	//				g_debug_gamepad_data[gamepad_index].thumb_right.x,
	//				g_debug_gamepad_data[gamepad_index].thumb_right.y);
	//		}
	//	}
	//}
}

void input_mouse_state_get_raw_data_string(char* buffer, short size)
{
	ASSERT(buffer);
	ASSERT(size > 0);
	
	if (buffer && size > 0)
	{
		csnzappendf(buffer, size, "|ninput_mouse_state|n");
		//raw_data_string[0] = 0;

		csnzappendf(buffer, size, "x: ", input_globals.raw_mouse_state.x);
		csnzappendf(buffer, size, "y: ", input_globals.raw_mouse_state.y);
		csnzappendf(buffer, size, "wheel_ticks: ", input_globals.raw_mouse_state.wheel_ticks);
		csnzappendf(buffer, size, "relative_x: ", input_globals.raw_mouse_state.relative_x);
		csnzappendf(buffer, size, "relative_y: ", input_globals.raw_mouse_state.relative_y);
		csnzappendf(buffer, size, "wheel_delta: ", input_globals.raw_mouse_state.wheel_delta);
	}
}

//byte const key_to_virtual_table[k_key_code_count]
//{
//	VK_ESCAPE,                   // _key_code_escape
//	VK_F1,                       // _key_code_f1
//	VK_F2,                       // _key_code_f2
//	VK_F3,                       // _key_code_f3
//	VK_F4,                       // _key_code_f4
//	VK_F5,                       // _key_code_f5
//	VK_F6,                       // _key_code_f6
//	VK_F7,                       // _key_code_f7
//	VK_F8,                       // _key_code_f8
//	VK_F9,                       // _key_code_f9
//	VK_F10,                      // _key_code_f10
//	VK_F11,                      // _key_code_f11
//	VK_F12,                      // _key_code_f12
//	VK_SNAPSHOT,                 // _key_code_print_screen
//	VK_F14,                      // _key_code_f14
//	VK_F15,                      // _key_code_f15
//	VK_OEM_3,                    // _key_code_backquote
//	'1',                         // _key_code_1
//	'2',                         // _key_code_2
//	'3',                         // _key_code_3
//	'4',                         // _key_code_4
//	'5',                         // _key_code_5
//	'6',                         // _key_code_6
//	'7',                         // _key_code_7
//	'8',                         // _key_code_8
//	'9',                         // _key_code_9
//	'0',                         // _key_code_0
//	VK_OEM_MINUS,                // _key_code_minus
//	VK_OEM_PLUS,                 // _key_code_plus
//	VK_BACK,                     // _key_code_backspace
//	VK_TAB,                      // _key_code_tab
//	'Q',                         // _key_code_q
//	'W',                         // _key_code_w
//	'E',                         // _key_code_e
//	'R',                         // _key_code_r
//	'T',                         // _key_code_t
//	'Y',                         // _key_code_y
//	'U',                         // _key_code_u
//	'I',                         // _key_code_i
//	'O',                         // _key_code_o
//	'P',                         // _key_code_p
//	VK_OEM_4,                    // _key_code_left_parenthesis
//	VK_OEM_6,                    // _key_code_right_parenthesis
//	VK_OEM_5,                    // _key_code_backslash
//	VK_CAPITAL,                  // _key_code_caps_lock
//	'A',                         // _key_code_a
//	'S',                         // _key_code_s
//	'D',                         // _key_code_d
//	'F',                         // _key_code_f
//	'G',                         // _key_code_g
//	'H',                         // _key_code_h
//	'J',                         // _key_code_j
//	'K',                         // _key_code_k
//	'L',                         // _key_code_l
//	VK_OEM_1,                    // _key_code_semicolon
//	VK_OEM_7,                    // _key_code_apostrophe
//	VK_RETURN,                   // _key_code_enter
//	VK_LSHIFT,                   // _key_code_left_shift
//	'Z',                         // _key_code_z
//	'X',                         // _key_code_x
//	'C',                         // _key_code_c
//	'V',                         // _key_code_v
//	'B',                         // _key_code_b
//	'N',                         // _key_code_n
//	'M',                         // _key_code_m
//	VK_OEM_COMMA,                // _key_code_comma
//	VK_OEM_PERIOD,               // _key_code_dot
//	VK_OEM_2,                    // _key_code_slash
//	VK_RSHIFT,                   // _key_code_right_shift
//	VK_LCONTROL,                 // _key_code_left_control
//	VK_LWIN,                     // _key_code_left_windows
//	VK_LMENU,                    // _key_code_left_alt
//	VK_SPACE,                    // _key_code_spacebar
//	VK_RMENU,                    // _key_code_right_alt
//	VK_RWIN,                     // _key_code_right_windows
//	VK_APPS,                     // _key_code_context_menu
//	VK_RCONTROL,                 // _key_code_right_control
//	VK_UP,                       // _key_code_up
//	VK_DOWN,                     // _key_code_down
//	VK_LEFT,                     // _key_code_left
//	VK_RIGHT,                    // _key_code_right
//	VK_INSERT,                   // _key_code_insert
//	VK_HOME,                     // _key_code_home
//	VK_PRIOR,                    // _key_code_page_up
//	VK_DELETE,                   // _key_code_delete
//	VK_END,                      // _key_code_end
//	VK_NEXT,                     // _key_code_page_down
//	VK_NUMLOCK,                  // _key_code_keypad_num_lock
//	VK_DIVIDE,                   // _key_code_keypad_divide
//	VK_MULTIPLY,                 // _key_code_keypad_multiply
//	VK_NUMPAD0,                  // _key_code_keypad_0
//	VK_NUMPAD1,                  // _key_code_keypad_1
//	VK_NUMPAD2,                  // _key_code_keypad_2
//	VK_NUMPAD3,                  // _key_code_keypad_3
//	VK_NUMPAD4,                  // _key_code_keypad_4
//	VK_NUMPAD5,                  // _key_code_keypad_5
//	VK_NUMPAD6,                  // _key_code_keypad_6
//	VK_NUMPAD7,                  // _key_code_keypad_7
//	VK_NUMPAD8,                  // _key_code_keypad_8
//	VK_NUMPAD9,                  // _key_code_keypad_9
//	VK_SUBTRACT,                 // _key_code_keypad_subtract
//	VK_ADD,                      // _key_code_keypad_add
//	VK_RETURN,                   // _key_code_keypad_enter
//	VK_DECIMAL                   // _key_code_keypad_decimal
//};
//
//short const virtual_to_key_table[k_number_of_windows_input_virtual_codes]
//{
//	k_key_code_none,
//	k_key_code_none,             // VK_LBUTTON
//	k_key_code_none,             // VK_RBUTTON
//	k_key_code_none,             // VK_CANCEL
//	k_key_code_none,             // VK_MBUTTON
//	k_key_code_none,             // VK_XBUTTON1
//	k_key_code_none,             // VK_XBUTTON2
//	k_key_code_none,             // reserved
//	_key_code_backspace,         // VK_BACK
//	_key_code_tab,               // VK_TAB
//	k_key_code_none,             // reserved
//	k_key_code_none,             // reserved
//	k_key_code_none,             // VK_CLEAR
//	_key_code_enter,             // VK_RETURN
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	_key_code_shift,             // VK_SHIFT
//	_key_code_control,           // VK_CONTROL
//	_key_code_left_alt,          // VK_MENU
//	_key_code_f15,               // VK_PAUSE
//	_key_code_caps_lock,         // VK_CAPITAL
//	k_key_code_none,             // VK_KANA, VK_HANGEUL, VK_HANGUL
//	k_key_code_none,             // VK_IME_ON
//	k_key_code_none,             // VK_JUNJA
//	k_key_code_none,             // VK_FINAL
//	k_key_code_none,             // VK_HANJA, VK_KANJI
//	k_key_code_none,             // VK_IME_OFF
//	_key_code_escape,            // VK_ESCAPE
//	k_key_code_none,             // VK_CONVERT
//	k_key_code_none,             // VK_NONCONVERT
//	k_key_code_none,             // VK_ACCEPT
//	k_key_code_none,             // VK_MODECHANGE
//	_key_code_spacebar,          // VK_SPACE
//	_key_code_page_up,           // VK_PRIOR
//	_key_code_page_down,         // VK_NEXT
//	_key_code_end,               // VK_END
//	_key_code_home,              // VK_HOME
//	_key_code_left,              // VK_LEFT
//	_key_code_up,                // VK_UP
//	_key_code_right,             // VK_RIGHT
//	_key_code_down,              // VK_DOWN
//	k_key_code_none,             // VK_SELECT
//	k_key_code_none,             // VK_PRINT
//	k_key_code_none,             // VK_EXECUTE
//	_key_code_print_screen,      // VK_SNAPSHOT
//	_key_code_insert,            // VK_INSERT
//	_key_code_delete,            // VK_DELETE
//	k_key_code_none,             // VK_HELP
//	_key_code_0,                 // VK_0
//	_key_code_1,                 // VK_1
//	_key_code_2,                 // VK_2
//	_key_code_3,                 // VK_3
//	_key_code_4,                 // VK_4
//	_key_code_5,                 // VK_5
//	_key_code_6,                 // VK_6
//	_key_code_7,                 // VK_7
//	_key_code_8,                 // VK_8
//	_key_code_9,                 // VK_9
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	_key_code_a,                 // VK_A
//	_key_code_b,                 // VK_B
//	_key_code_c,                 // VK_C
//	_key_code_d,                 // VK_D
//	_key_code_e,                 // VK_E
//	_key_code_f,                 // VK_F
//	_key_code_g,                 // VK_G
//	_key_code_h,                 // VK_H
//	_key_code_i,                 // VK_I
//	_key_code_j,                 // VK_J
//	_key_code_k,                 // VK_K
//	_key_code_l,                 // VK_L
//	_key_code_m,                 // VK_M
//	_key_code_n,                 // VK_N
//	_key_code_o,                 // VK_O
//	_key_code_p,                 // VK_P
//	_key_code_q,                 // VK_Q
//	_key_code_r,                 // VK_R
//	_key_code_s,                 // VK_S
//	_key_code_t,                 // VK_T
//	_key_code_u,                 // VK_U
//	_key_code_v,                 // VK_V
//	_key_code_w,                 // VK_W
//	_key_code_x,                 // VK_X
//	_key_code_y,                 // VK_Y
//	_key_code_z,                 // VK_Z
//	_key_code_left_windows,      // VK_LWIN
//	_key_code_right_windows,     // VK_RWIN
//	_key_code_context_menu,      // VK_APPS
//	k_key_code_none,             // reserved
//	k_key_code_none,             // VK_SLEEP
//	_key_code_keypad_0,          // VK_NUMPAD0
//	_key_code_keypad_1,          // VK_NUMPAD1
//	_key_code_keypad_2,          // VK_NUMPAD2
//	_key_code_keypad_3,          // VK_NUMPAD3
//	_key_code_keypad_4,          // VK_NUMPAD4
//	_key_code_keypad_5,          // VK_NUMPAD5
//	_key_code_keypad_6,          // VK_NUMPAD6
//	_key_code_keypad_7,          // VK_NUMPAD7
//	_key_code_keypad_8,          // VK_NUMPAD8
//	_key_code_keypad_9,          // VK_NUMPAD9
//	_key_code_keypad_multiply,   // VK_MULTIPLY
//	_key_code_keypad_add,        // VK_ADD
//	k_key_code_none,             // VK_SEPARATOR
//	_key_code_keypad_subtract,   // VK_SUBTRACT
//	_key_code_keypad_decimal,    // VK_DECIMAL
//	_key_code_keypad_divide,     // VK_DIVIDE
//	_key_code_f1,                // VK_F1
//	_key_code_f2,                // VK_F2
//	_key_code_f3,                // VK_F3
//	_key_code_f4,                // VK_F4
//	_key_code_f5,                // VK_F5
//	_key_code_f6,                // VK_F6
//	_key_code_f7,                // VK_F7
//	_key_code_f8,                // VK_F8
//	_key_code_f9,                // VK_F9
//	_key_code_f10,               // VK_F10
//	_key_code_f11,               // VK_F11
//	_key_code_f12,               // VK_F12
//	k_key_code_none,             // VK_F13
//	k_key_code_none,             // VK_F14
//	k_key_code_none,             // VK_F15
//	k_key_code_none,             // VK_F16
//	k_key_code_none,             // VK_F17
//	k_key_code_none,             // VK_F18
//	k_key_code_none,             // VK_F19
//	k_key_code_none,             // VK_F20
//	k_key_code_none,             // VK_F21
//	k_key_code_none,             // VK_F22
//	k_key_code_none,             // VK_F23
//	k_key_code_none,             // VK_F24
//	k_key_code_none,             // VK_NAVIGATION_VIEW
//	k_key_code_none,             // VK_NAVIGATION_MENU
//	k_key_code_none,             // VK_NAVIGATION_UP
//	k_key_code_none,             // VK_NAVIGATION_DOWN
//	k_key_code_none,             // VK_NAVIGATION_LEFT
//	k_key_code_none,             // VK_NAVIGATION_RIGHT
//	k_key_code_none,             // VK_NAVIGATION_ACCEPT
//	k_key_code_none,             // VK_NAVIGATION_CANCEL
//	_key_code_keypad_num_lock,   // VK_NUMLOCK
//	_key_code_f14,               // VK_SCROLL
//	k_key_code_none,             // VK_OEM_NEC_EQUAL, VK_OEM_FJ_JISHO
//	k_key_code_none,             // VK_OEM_FJ_MASSHOU
//	k_key_code_none,             // VK_OEM_FJ_TOUROKU
//	k_key_code_none,             // VK_OEM_FJ_LOYA
//	k_key_code_none,             // VK_OEM_FJ_ROYA
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // VK_LSHIFT
//	k_key_code_none,             // VK_RSHIFT
//	k_key_code_none,             // VK_LCONTROL
//	k_key_code_none,             // VK_RCONTROL
//	k_key_code_none,             // VK_LMENU
//	k_key_code_none,             // VK_RMENU
//	k_key_code_none,             // VK_BROWSER_BACK
//	k_key_code_none,             // VK_BROWSER_FORWARD
//	k_key_code_none,             // VK_BROWSER_REFRESH
//	k_key_code_none,             // VK_BROWSER_STOP
//	k_key_code_none,             // VK_BROWSER_SEARCH
//	k_key_code_none,             // VK_BROWSER_FAVORITES
//	k_key_code_none,             // VK_BROWSER_HOME
//	k_key_code_none,             // VK_VOLUME_MUTE
//	k_key_code_none,             // VK_VOLUME_DOWN
//	k_key_code_none,             // VK_VOLUME_UP
//	k_key_code_none,             // VK_MEDIA_NEXT_TRACK
//	k_key_code_none,             // VK_MEDIA_PREV_TRACK
//	k_key_code_none,             // VK_MEDIA_STOP
//	k_key_code_none,             // VK_MEDIA_PLAY_PAUSE
//	k_key_code_none,             // VK_LAUNCH_MAIL
//	k_key_code_none,             // VK_LAUNCH_MEDIA_SELECT
//	k_key_code_none,             // VK_LAUNCH_APP1
//	k_key_code_none,             // VK_LAUNCH_APP2
//	k_key_code_none,             // reserved
//	k_key_code_none,             // reserved
//	_key_code_semicolon,         // VK_OEM_1
//	_key_code_plus,              // VK_OEM_PLUS
//	_key_code_comma,             // VK_OEM_COMMA
//	_key_code_minus,             // VK_OEM_MINUS
//	_key_code_dot,               // VK_OEM_PERIOD
//	_key_code_slash,             // VK_OEM_2
//	_key_code_backquote,         // VK_OEM_3
//	k_key_code_none,             // reserved
//	k_key_code_none,             // reserved
//	k_key_code_none,             // VK_GAMEPAD_A
//	k_key_code_none,             // VK_GAMEPAD_B
//	k_key_code_none,             // VK_GAMEPAD_X
//	k_key_code_none,             // VK_GAMEPAD_Y
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_SHOULDER
//	k_key_code_none,             // VK_GAMEPAD_LEFT_SHOULDER
//	k_key_code_none,             // VK_GAMEPAD_LEFT_TRIGGER
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_TRIGGER
//	k_key_code_none,             // VK_GAMEPAD_DPAD_UP
//	k_key_code_none,             // VK_GAMEPAD_DPAD_DOWN
//	k_key_code_none,             // VK_GAMEPAD_DPAD_LEFT
//	k_key_code_none,             // VK_GAMEPAD_DPAD_RIGHT
//	k_key_code_none,             // VK_GAMEPAD_MENU
//	k_key_code_none,             // VK_GAMEPAD_VIEW
//	k_key_code_none,             // VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON
//	k_key_code_none,             // VK_GAMEPAD_LEFT_THUMBSTICK_UP
//	k_key_code_none,             // VK_GAMEPAD_LEFT_THUMBSTICK_DOWN
//	k_key_code_none,             // VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT
//	k_key_code_none,             // VK_GAMEPAD_LEFT_THUMBSTICK_LEFT
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_THUMBSTICK_UP
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT
//	k_key_code_none,             // VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT
//	_key_code_left_parenthesis,  // VK_OEM_4
//	_key_code_backslash,         // VK_OEM_5
//	_key_code_right_parenthesis, // VK_OEM_6
//	_key_code_apostrophe,        // VK_OEM_7
//	k_key_code_none,             // VK_OEM_8
//	k_key_code_none,             // reserved
//	k_key_code_none,             // VK_OEM_AX
//	k_key_code_none,             // VK_OEM_102
//	k_key_code_none,             // VK_ICO_HELP
//	k_key_code_none,             // VK_ICO_00
//	k_key_code_none,             // VK_PROCESSKEY
//	k_key_code_none,             // VK_ICO_CLEAR
//	k_key_code_none,             // VK_PACKET
//	k_key_code_none,             // unassigned
//	k_key_code_none,             // VK_OEM_RESET
//	k_key_code_none,             // VK_OEM_JUMP
//	k_key_code_none,             // VK_OEM_PA1
//	k_key_code_none,             // VK_OEM_PA2
//	k_key_code_none,             // VK_OEM_PA3
//	k_key_code_none,             // VK_OEM_WSCTRL
//	k_key_code_none,             // VK_OEM_CUSEL
//	k_key_code_none,             // VK_OEM_ATTN
//	k_key_code_none,             // VK_OEM_FINISH
//	k_key_code_none,             // VK_OEM_COPY
//	k_key_code_none,             // VK_OEM_AUTO
//	k_key_code_none,             // VK_OEM_ENLW
//	k_key_code_none,             // VK_OEM_BACKTAB
//	k_key_code_none,             // VK_ATTN
//	k_key_code_none,             // VK_CRSEL
//	k_key_code_none,             // VK_EXSEL
//	k_key_code_none,             // VK_EREOF
//	k_key_code_none,             // VK_PLAY
//	k_key_code_none,             // VK_ZOOM
//	k_key_code_none,             // VK_NONAME
//	k_key_code_none,             // VK_PA1
//	k_key_code_none,             // VK_OEM_CLEAR
//	k_key_code_none,             // reserved
//};
//
//byte const key_to_ascii_table[k_key_code_count]
//{
//	NONE,                        // _key_code_escape
//	NONE,                        // _key_code_f1
//	NONE,                        // _key_code_f2
//	NONE,                        // _key_code_f3
//	NONE,                        // _key_code_f4
//	NONE,                        // _key_code_f5
//	NONE,                        // _key_code_f6
//	NONE,                        // _key_code_f7
//	NONE,                        // _key_code_f8
//	NONE,                        // _key_code_f9
//	NONE,                        // _key_code_f10
//	NONE,                        // _key_code_f11
//	NONE,                        // _key_code_f12
//	NONE,                        // _key_code_print_screen
//	NONE,                        // _key_code_f14
//	NONE,                        // _key_code_f15
//	'`',                         // _key_code_backquote
//	'1',                         // _key_code_1
//	'2',                         // _key_code_2
//	'3',                         // _key_code_3
//	'4',                         // _key_code_4
//	'5',                         // _key_code_5
//	'6',                         // _key_code_6
//	'7',                         // _key_code_7
//	'8',                         // _key_code_8
//	'9',                         // _key_code_9
//	'0',                         // _key_code_0
//	'-',                         // _key_code_minus
//	'=',                         // _key_code_plus
//	'\x08',                      // _key_code_backspace
//	'\x09',                      // _key_code_tab
//	'Q',                         // _key_code_q
//	'W',                         // _key_code_w
//	'E',                         // _key_code_e
//	'R',                         // _key_code_r
//	'T',                         // _key_code_t
//	'Y',                         // _key_code_y
//	'U',                         // _key_code_u
//	'I',                         // _key_code_i
//	'O',                         // _key_code_o
//	'P',                         // _key_code_p
//	'[',                         // _key_code_left_parenthesis
//	']',                         // _key_code_right_parenthesis
//	'\\',                        // _key_code_backslash
//	NONE,                        // _key_code_caps_lock
//	'A',                         // _key_code_a
//	'S',                         // _key_code_s
//	'D',                         // _key_code_d
//	'F',                         // _key_code_f
//	'G',                         // _key_code_g
//	'H',                         // _key_code_h
//	'J',                         // _key_code_j
//	'K',                         // _key_code_k
//	'L',                         // _key_code_l
//	';',                         // _key_code_semicolon
//	'\'',                        // _key_code_apostrophe
//	'\x0D',                      // _key_code_enter
//	NONE,                        // _key_code_left_shift
//	'Z',                         // _key_code_z
//	'X',                         // _key_code_x
//	'C',                         // _key_code_c
//	'V',                         // _key_code_v
//	'B',                         // _key_code_b
//	'N',                         // _key_code_n
//	'M',                         // _key_code_m
//	',',                         // _key_code_comma
//	'.',                         // _key_code_dot
//	'/',                         // _key_code_slash
//	NONE,                        // _key_code_right_shift
//	NONE,                        // _key_code_left_control
//	NONE,                        // _key_code_left_windows
//	NONE,                        // _key_code_left_alt
//	' ',                         // _key_code_spacebar
//	NONE,                        // _key_code_right_alt
//	NONE,                        // _key_code_right_windows
//	NONE,                        // _key_code_context_menu
//	NONE,                        // _key_code_right_control
//	NONE,                        // _key_code_up
//	NONE,                        // _key_code_down
//	NONE,                        // _key_code_left
//	NONE,                        // _key_code_right
//	NONE,                        // _key_code_insert
//	NONE,                        // _key_code_home
//	NONE,                        // _key_code_page_up
//	NONE,                        // _key_code_delete
//	NONE,                        // _key_code_end
//	NONE,                        // _key_code_page_down
//	NONE,                        // _key_code_keypad_num_lock
//	'/',                         // _key_code_keypad_divide
//	'*',                         // _key_code_keypad_multiply
//	'0',                         // _key_code_keypad_0
//	'1',                         // _key_code_keypad_1
//	'2',                         // _key_code_keypad_2
//	'3',                         // _key_code_keypad_3
//	'4',                         // _key_code_keypad_4
//	'5',                         // _key_code_keypad_5
//	'6',                         // _key_code_keypad_6
//	'7',                         // _key_code_keypad_7
//	'8',                         // _key_code_keypad_8
//	'9',                         // _key_code_keypad_9
//	'-',                         // _key_code_keypad_subtract
//	'+',                         // _key_code_keypad_add
//	'\x0D',                      // _key_code_keypad_enter
//	'.',                         // _key_code_keypad_decimal
//};
//
//short const ascii_to_key_table[k_number_of_input_ascii_codes]
//{
//	k_key_code_none,             // NUL    Null
//	k_key_code_none,             // SOH    Start of Heading
//	k_key_code_none,             // STX    Start of Text
//	k_key_code_none,             // ETX    End of Text
//	k_key_code_none,             // EOT    End of Transmission
//	k_key_code_none,             // ENQ    Enquiry
//	k_key_code_none,             // ACK    Acknowledge
//	k_key_code_none,             // BEL    Bell
//	k_key_code_none,             // BS     Backspace
//	k_key_code_none,             // HT     Horizontal Tab
//	k_key_code_none,             // LF     Line Feed
//	k_key_code_none,             // VT     Vertical Tab
//	k_key_code_none,             // FF     Form Feed
//	k_key_code_none,             // CR     Carriage Return
//	k_key_code_none,             // SO     Shift Out
//	k_key_code_none,             // SI     Shift In
//	k_key_code_none,             // DLE    Data Link Escape
//	k_key_code_none,             // DC1    Device Control 1
//	k_key_code_none,             // DC2    Device Control 2
//	k_key_code_none,             // DC3    Device Control 3
//	k_key_code_none,             // DC4    Device Control 4
//	k_key_code_none,             // NAK    Negative Acknowledge
//	k_key_code_none,             // SYN    Synchronize
//	k_key_code_none,             // ETB    End of Transmission Block
//	k_key_code_none,             // CAN    Cancel
//	k_key_code_none,             // EM     End of Medium
//	k_key_code_none,             // SUB    Substitute
//	k_key_code_none,             // ESC    Escape
//	k_key_code_none,             // FS     File Separator
//	k_key_code_none,             // GS     Group Separator
//	k_key_code_none,             // RS     Record Separator
//	k_key_code_none,             // US     Unit Separator
//	_key_code_spacebar,          // space  Space
//	_key_code_1,                 // !      exclamation mark
//	_key_code_apostrophe,        // "      double quote
//	_key_code_3,                 // #      number
//	_key_code_4,                 // $      dollar
//	_key_code_5,                 // %      percent
//	_key_code_7,                 // &      ampersand
//	_key_code_apostrophe,        // '      single quote
//	_key_code_9,                 // (      left parenthesis
//	_key_code_0,                 // )      right parenthesis
//	_key_code_8,                 // *      asterisk
//	_key_code_plus,              // +      plus
//	_key_code_comma,             // ,      comma
//	_key_code_minus,             // -      minus
//	_key_code_dot,               // .      period
//	_key_code_slash,             // /      slash
//	_key_code_0,                 // 0      zero
//	_key_code_1,                 // 1      one
//	_key_code_2,                 // 2      two
//	_key_code_3,                 // 3      three
//	_key_code_4,                 // 4      four
//	_key_code_5,                 // 5      five
//	_key_code_6,                 // 6      six
//	_key_code_7,                 // 7      seven
//	_key_code_8,                 // 8      eight
//	_key_code_9,                 // 9      nine
//	_key_code_colon,             // :      colon
//	_key_code_semicolon,         // ;      semicolon
//	_key_code_comma,             // <      less than
//	_key_code_plus,              // =      equality sign
//	_key_code_dot,               // >      greater than
//	_key_code_slash,             // ?      question mark
//	_key_code_2,                 // @      at sign
//	_key_code_a,                 // A
//	_key_code_b,                 // B
//	_key_code_c,                 // C
//	_key_code_d,                 // D
//	_key_code_e,                 // E
//	_key_code_f,                 // F
//	_key_code_g,                 // G
//	_key_code_h,                 // H
//	_key_code_i,                 // I
//	_key_code_j,                 // J
//	_key_code_k,                 // K
//	_key_code_l,                 // L
//	_key_code_m,                 // M
//	_key_code_n,                 // N
//	_key_code_o,                 // O
//	_key_code_p,                 // P
//	_key_code_q,                 // Q
//	_key_code_r,                 // R
//	_key_code_s,                 // S
//	_key_code_t,                 // T
//	_key_code_u,                 // U
//	_key_code_v,                 // V
//	_key_code_w,                 // W
//	_key_code_x,                 // X
//	_key_code_y,                 // Y
//	_key_code_z,                 // Z
//	_key_code_left_parenthesis,  // [      left square bracket
//	_key_code_backslash,         // \      backslash
//	_key_code_right_parenthesis, // ]      right square bracket
//	_key_code_6,                 // ^      caret / circumflex
//	_key_code_minus,             // _      underscore
//	_key_code_backquote,         // `      grave / accent
//	_key_code_a,                 // a
//	_key_code_b,                 // b
//	_key_code_c,                 // c
//	_key_code_d,                 // d
//	_key_code_e,                 // e
//	_key_code_f,                 // f
//	_key_code_g,                 // g
//	_key_code_h,                 // h
//	_key_code_i,                 // i
//	_key_code_j,                 // j
//	_key_code_k,                 // k
//	_key_code_l,                 // l
//	_key_code_m,                 // m
//	_key_code_n,                 // n
//	_key_code_o,                 // o
//	_key_code_p,                 // p
//	_key_code_q,                 // q
//	_key_code_r,                 // r
//	_key_code_s,                 // s
//	_key_code_t,                 // t
//	_key_code_u,                 // u
//	_key_code_v,                 // v
//	_key_code_w,                 // w
//	_key_code_x,                 // x
//	_key_code_y,                 // y
//	_key_code_z,                 // z
//	_key_code_left_parenthesis,  // {      left curly bracket
//	_key_code_backslash,         // |      vertical bar
//	_key_code_right_parenthesis, // }      right curly bracket
//	_key_code_backquote,         // ~      tilde
//	_key_code_delete             // DEL    delete
//};

