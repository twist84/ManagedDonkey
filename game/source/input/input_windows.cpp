#include "input/input_windows.hpp"

#include "cseries/cseries.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "input/input_abstraction.hpp"
#include "input/input_xinput.hpp"
#include "main/console.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell_windows.hpp"

//#include <windows.h> // for `key_to_virtual_table`

REFERENCE_DECLARE_ARRAY(0x01650918, uns8 const, key_to_virtual_table, k_key_code_count);
REFERENCE_DECLARE_ARRAY(0x01650980, int16 const, virtual_to_key_table, k_number_of_windows_input_virtual_codes);
REFERENCE_DECLARE_ARRAY(0x01650B80, uns8 const, key_to_ascii_table, k_key_code_count);
REFERENCE_DECLARE_ARRAY(0x01650BE8, int16 const, ascii_to_key_table, k_number_of_input_ascii_codes);
REFERENCE_DECLARE(0x0238DBE8, s_input_globals, input_globals);

HOOK_DECLARE(0x00512690, input_update);

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

	//if (shell_application_type() == _shell_application_game)
	//{
	//	if ((GetAsyncKeyState(VK_LBUTTON) & 0xFFFE) != 0 ||
	//		(GetAsyncKeyState(VK_RBUTTON) & 0xFFFE) != 0 ||
	//		(GetAsyncKeyState(VK_MBUTTON) & 0xFFFE) != 0 ||
	//		(GetAsyncKeyState(VK_XBUTTON1) & 0xFFFE) != 0 ||
	//		(GetAsyncKeyState(VK_XBUTTON2) & 0xFFFE) != 0 ||
	//		GetForegroundWindow() != window_globals.hWnd)
	//	{
	//		input_globals.raw_input_unknownAB5 = true;
	//		return;
	//	}
	//
	//	input_globals.raw_input_unknownAB5 = false;
	//}
	//
	//if (!input_globals.raw_input_unknownAB4 && !input_globals.raw_input_unknownAB6)
	//{
	//	sub_511410();
	//	if (sub_512450())
	//	{
	//		sub_5129B0();
	//		ShowCursor(FALSE);
	//		input_globals.raw_input_unknownAB4 = true;
	//	}
	//	else
	//	{
	//		sub_511760(DDERR_GENERIC, "Acquire (mouse)");
	//	}
	//}
}

void __cdecl sub_511550()
{
	INVOKE(0x00511550, sub_511550);

	//if (!input_globals.raw_input_mouse_state_update)
	//{
	//	sub_511AF0();
	//}
}

void __cdecl input_activate()
{
	INVOKE(0x005115A0, input_activate);

	//input_globals.update_time = system_milliseconds();
	//input_globals.mouse_acquired = true;
	//if (input_globals.raw_input_mouse_state_update)
	//{
	//	sub_5114A0();
	//}
}

void __cdecl input_clear_all_rumblers()
{
	INVOKE(0x005115C0, input_clear_all_rumblers);
}

void __cdecl sub_511620()
{
	INVOKE(0x00511620, sub_511620);

	//RECT client_rect{};
	//GetClientRect(window_globals.hWnd, &client_rect);
	//
	//POINT client_point0 = { .x = client_rect.left, .y = client_rect.top };
	//POINT client_point1 = { .x = client_rect.right, .y = client_rect.bottom };
	//ClientToScreen(window_globals.hWnd, &client_point0);
	//ClientToScreen(window_globals.hWnd, &client_point1);
	//
	//client_rect = { .left = client_point0.x, .top = client_point0.y, .right = client_point1.x, .bottom = client_point1.y };
	//ClipCursor(&client_rect);
}

void __cdecl input_deactivate()
{
	INVOKE(0x005116A0, input_deactivate);

	//if (input_globals.raw_input_mouse_state_update)
	//{
	//	sub_5125A0();
	//}
	//input_globals.mouse_acquired = false;
}

void __cdecl input_dispose()
{
	INVOKE(0x005116C0, input_dispose);

	//sub_511710();
	//input_xinput_dispose();
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

// Halo 3 (H3EK) uses DirectInput, it's possible so did Halo Online until RawInput was added
void __cdecl sub_511760(int error, const char* format, ...)
{
	va_list list;
	va_start(list, format);

	REFERENCE_DECLARE(0x0189D5B0, int, last_error);
	//static int last_error = NONE;

	if (error != last_error)
	{
		last_error = error;
		char error_message[4096]{};
		cvsnzprintf(error_message, sizeof(error_message), format, list);

		const char* error_string = "<unknown error>";
		//switch (error)
		//{
		//#define DIERR_ERROR_CASE(ERROR) case ERROR: error_string = #ERROR; break;
		//DIERR_ERROR_CASE(DIERR_BADDRIVERVER);
		//DIERR_ERROR_CASE(DIERR_ACQUIRED);
		//DIERR_ERROR_CASE(DIERR_OLDDIRECTINPUTVERSION);
		//DIERR_ERROR_CASE(DIERR_BETADIRECTINPUTVERSION);
		//DIERR_ERROR_CASE(DIERR_ALREADYINITIALIZED);
		//DIERR_ERROR_CASE(DIERR_INVALIDPARAM);
		//DIERR_ERROR_CASE(DIERR_OTHERAPPHASPRIO);
		//DIERR_ERROR_CASE(DIERR_NOTACQUIRED);
		//DIERR_ERROR_CASE(DIERR_OUTOFMEMORY);
		//DIERR_ERROR_CASE(DIERR_NOTINITIALIZED);
		//DIERR_ERROR_CASE(DIERR_INPUTLOST);
		//DIERR_ERROR_CASE(DIERR_OBJECTNOTFOUND);
		//DIERR_ERROR_CASE(DIERR_DEVICENOTREG);
		//DIERR_ERROR_CASE(DIERR_INSUFFICIENTPRIVS);
		//DIERR_ERROR_CASE(DIERR_DEVICEFULL);
		//DIERR_ERROR_CASE(DIERR_MOREDATA);
		//DIERR_ERROR_CASE(DIERR_NOTDOWNLOADED);
		//DIERR_ERROR_CASE(DIERR_HASEFFECTS);
		//DIERR_ERROR_CASE(DIERR_NOTEXCLUSIVEACQUIRED);
		//DIERR_ERROR_CASE(DIERR_INCOMPLETEEFFECT);
		//DIERR_ERROR_CASE(DIERR_NOTBUFFERED);
		//DIERR_ERROR_CASE(DIERR_EFFECTPLAYING);
		//DIERR_ERROR_CASE(DIERR_UNPLUGGED);
		//DIERR_ERROR_CASE(DIERR_REPORTFULL);
		//DIERR_ERROR_CASE(DIERR_MAPFILEFAIL);
		//DIERR_ERROR_CASE(DIERR_NOAGGREGATION);
		//DIERR_ERROR_CASE(DIERR_UNSUPPORTED);
		//DIERR_ERROR_CASE(DIERR_NOINTERFACE);
		//DIERR_ERROR_CASE(DIERR_GENERIC);
		//#undef DIERR_ERROR_CASE
		//}
		//event(_event_warning, "DirectInput: '%s' returned (%s#%d)", error_message, error_string, error);

		//event(_event_warning, "RawInput: '%s' returned (%s#%d)", error_message, error_string, error);
		c_console::write_line("RawInput: '%s' returned (%s#%d)", error_message, error_string, error);
	}

	va_end(list);
}

void __cdecl input_feedback_suppress(bool suppress_feedback)
{
	INVOKE(0x005117A0, input_feedback_suppress, suppress_feedback);

	//input_globals.feedback_suppressed = suppress_feedback;
}

void __cdecl input_flush()
{
	INVOKE(0x005117B0, input_flush);

	//input_globals.keys.clear();
	//
	//input_globals.buffered_key_read_index = 0;
	//input_globals.buffered_key_read_count = 0;
	//input_globals.buffered_keys.clear();
	//
	//uns8 raw_flags = input_globals.raw_mouse_state.raw_flags;
	//csmemset(&input_globals.raw_mouse_state, 0, sizeof(input_globals.raw_mouse_state));
	//input_globals.raw_mouse_state.raw_flags = raw_flags;
	//
	//input_globals.buffered_mouse_button_read_index = 0;
	//input_globals.buffered_mouse_button_read_count = 0;
	//input_globals.buffered_mouse_buttons.clear();
	//
	//input_globals.gamepad_states.clear();
}

const gamepad_state* __cdecl input_get_gamepad_state(int16 gamepad_index)
{
	//return INVOKE(0x00511840, input_get_gamepad_state, gamepad_index);

	if (!input_has_gamepad(gamepad_index))
	{
		return NULL;
	}
	
	if (input_globals.input_suppressed)
	{
		return &input_globals.suppressed_gamepad_state;
	}
	
	return &input_globals.gamepad_states[gamepad_index];
}

bool __cdecl input_get_key(s_key_state* key, e_input_type input_type)
{
	return INVOKE(0x005118C0, input_get_key, key, input_type);

	//if (!input_peek_key(key, input_type))
	//{
	//	return false;
	//}
	//input_globals.buffered_key_read_index++;
	//return true;
}

// Because of the way the game handles input this function won't actually run when the game is tabbed in
// In the window proc the `WM_INPUT` message is handled with a `RawInput` function
// RawInput is great but I want to get the damn mouse state
bool __cdecl input_get_mouse(s_mouse_state* mouse, e_input_type input_type)
{
	return INVOKE(0x00511990, input_get_mouse, mouse, input_type);

	//if (!input_peek_mouse(mouse, input_type))
	//{
	//	return false;
	//}
	//input_globals.buffered_mouse_button_read_index++;
	//return true;
}

mouse_state* __cdecl input_get_mouse_state(e_input_type input_type)
{
	return INVOKE(0x005119F0, input_get_mouse_state, input_type);

	//if (!input_globals.raw_input_mouse_state_update)
	//{
	//	return NULL;
	//}
	//
	//if (input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed)
	//{
	//	return &input_globals.suppressed_raw_mouse_state;
	//}
	//
	//return &input_globals.raw_mouse_state;
}

bool __cdecl input_has_gamepad(int16 gamepad_index)
{
	ASSERT(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	//return INVOKE(0x00511A40, input_has_gamepad, gamepad_index);

	return /*enable_pc_joystick &&*/ (!game_in_editor() || !input_globals.raw_input_mouse_state_update) && input_globals.gamepad_valid_mask.test(gamepad_index);
}

void __cdecl input_initialize()
{
	INVOKE(0x00511A90, input_initialize);

	//if (shell_application_type() == _shell_application_game)
	//{
	//	sub_511550();
	//}
	//
	//if (shell_application_type() == _shell_application_game || shell_tool_type() == _shell_tool_interactive)
	//{
	//	input_activate();
	//}
	//
	//input_xinput_initialize();
	//input_globals.initialized = true;
}

bool __cdecl sub_511AF0()
{
	return INVOKE(0x00511AF0, sub_511AF0);

	//input_globals.raw_input_unknownAB6 = false;
	//input_globals.raw_input_mouse_state_update = true;
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

	//return input_globals.mouse_acquired;
}

bool __cdecl input_type_suppressed(e_input_type input_type)
{
	return INVOKE(0x00511B50, input_type_suppressed, input_type);

	//return input_globals.input_type_suppressed[input_type];
}

uns8 __cdecl input_key_frames_down(e_input_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511B60, input_key_frames_down, key_code, input_type);

	//if (input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed)
	//	return 0;
	//
	//switch (key_code)
	//{
	//case _key_left_windows:
	//case _key_right_windows:
	//case _key_windows:
	//	return 0;
	//case _key_shift:
	//	return MAX(input_globals.keys[_key_left_shift].frames, input_globals.keys[_key_right_shift].frames);
	//case _key_control:
	//	return MAX(input_globals.keys[_key_left_control].frames, input_globals.keys[_key_right_control].frames);
	//case _key_alt:
	//	return MAX(input_globals.keys[_key_left_alt].frames, input_globals.keys[_key_right_alt].frames);
	//}
	//
	//return input_globals.keys[key_code].frames;
}

uns16 __cdecl input_key_msec_down(e_input_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511CE0, input_key_msec_down, key_code, input_type);

	//if (input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed)
	//	return 0;
	//
	//switch (key_code)
	//{
	//case _key_left_windows:
	//case _key_right_windows:
	//case _key_windows:
	//	return 0;
	//case _key_shift:
	//	return MAX(input_globals.keys[_key_left_shift].msec, input_globals.keys[_key_right_shift].msec);
	//case _key_control:
	//	return MAX(input_globals.keys[_key_left_control].msec, input_globals.keys[_key_right_control].msec);
	//case _key_alt:
	//	return MAX(input_globals.keys[_key_left_alt].msec, input_globals.keys[_key_right_alt].msec);
	//}
	//
	//return input_globals.keys[key_code].msec;
}

uns8 __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511DF0, input_mouse_frames_down, mouse_button, input_type);

	//bool suppressed = input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed;
	//if (!input_globals.raw_input_mouse_state_update || suppressed)
	//	return false;
	//
	//return input_globals.raw_mouse_state.frames_down[mouse_button];
}

uns16 __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511E30, input_mouse_msec_down, mouse_button, input_type);

	//bool suppressed = input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed;
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
bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type)
{
	return INVOKE(0x00511EC0, input_peek_mouse, mouse, input_type);

	//if (input_globals.input_type_suppressed[input_type] || input_globals.buffered_mouse_button_read_index >= input_globals.buffered_mouse_button_read_count)
	//	return false;
	//
	//*mouse = input_globals.buffered_mouse_buttons[input_globals.buffered_mouse_button_read_index];
	//return true;
}

bool __cdecl sub_512450()
{
	return INVOKE(0x00512450, sub_512450);

	//RAWINPUTDEVICE raw_input_device
	//{
	//	.usUsagePage = 1, // HID_USAGE_PAGE_GENERIC
	//	.usUsage     = 2, // HID_USAGE_GENERIC_MOUSE
	//	.dwFlags     = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE,
	//	.hwndTarget  = shell_application_type() == _shell_application_game ? window_globals.hWnd : window_globals.hWndPresentTarget
	//};
	//
	//return RegisterRawInputDevices(&raw_input_device, 1, sizeof(RAWINPUTDEVICE)) == TRUE;
}

void __cdecl input_set_gamepad_rumbler_state(int16 gamepad_index, uns16 left_motor_speed, uns16 right_motor_speed)
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
	//		input_globals.raw_input_unknownAB6 = false;
	//		if (!game_in_editor() && window_globals.hWnd == GetForegroundWindow())
	//			sub_5114A0();
	//	}
	//}
}

void __cdecl input_suppress()
{
	INVOKE(0x00512590, input_suppress);

	//input_globals.input_suppressed = true;
}

void __cdecl sub_5125A0()
{
	INVOKE(0x005125A0, sub_5125A0);

	//if (input_globals.raw_input_unknownAB4)
	//{
	//	input_globals.raw_input_unknownAB4 = false;
	//
	//	bool result = sub_512650();
	//	sub_5129B0();
	//	ShowCursor(TRUE);
	//	if (!result)
	//		sub_511760(DDERR_GENERIC, "Unacquire (mouse)");
	//}
}

bool __cdecl sub_512650()
{
	return INVOKE(0x00512650, sub_512650);

	//RAWINPUTDEVICE raw_input_device
	//{
	//	.usUsagePage = 1, // HID_USAGE_PAGE_GENERIC
	//	.usUsage     = 2, // HID_USAGE_GENERIC_MOUSE
	//	.dwFlags     = RIDEV_REMOVE,
	//	.hwndTarget  = NULL
	//};
	//return RegisterRawInputDevices(&raw_input_device, 1, sizeof(RAWINPUTDEVICE)) == TRUE;
}

void __cdecl input_update()
{
	//INVOKE(0x00512690, input_update);

	if (input_globals.initialized && (input_globals.mouse_acquired || game_in_editor() && sub_42E000()))
	{
		uns32 time = system_milliseconds();
		int32 elapsed_msec = CLAMP(time - input_globals.update_time, 0, 100);

		input_globals.input_suppressed = false;
		input_globals.update_time = time;
	
		input_update_device_connections();
		input_update_keyboard(elapsed_msec);
		input_update_mouse(elapsed_msec);
		input_update_gamepads(elapsed_msec);
		input_update_gamepads_rumble();

		input_handle_key_combos();
	}
}

void __cdecl sub_5129B0()
{
	INVOKE(0x005129B0, sub_5129B0);

	//if (!game_in_editor() && GetForegroundWindow() == window_globals.hWnd)
	//{
	//	if (input_globals.raw_input_unknownAB4 || game_options_valid() && !game_is_ui_shell())
	//		sub_511620();
	//	else
	//		ClipCursor(NULL);
	//}
}

void __cdecl input_update_device_connections()
{
}

void __cdecl input_update_keyboard(int32 elapsed_msec)
{
	bool window_has_focus = game_in_editor() || window_globals.hWnd == GetForegroundWindow();

	input_globals.buffered_key_read_index = 0;
	input_globals.buffered_key_read_count = 0;

	for (int32 key_index = 0; key_index < k_key_code_count; key_index++)
	{
		s_input_key_state& key = input_globals.keys[key_index];
		uns8 virtual_key = key_to_virtual_table[key_index];

		bool key_down = false;
		if (window_has_focus)
		{
			key_down = (GetKeyState(virtual_key) & 0x8000) != 0;
			if (key.msec == 1 && !key.frames && !key_down)
			{
				key_down = true;
			}
		}

		update_key(&key, key_down, elapsed_msec);
	}

	// Halo 3 handles `input_globals.buffered_keys` here

	// $TODO dedicated server debug console
}

void __cdecl input_update_mouse(int32 elapsed_msec)
{
	if (!input_globals.raw_input_mouse_state_update)
	{
		return;
	}

	input_globals.buffered_mouse_button_read_index = 0;
	input_globals.buffered_mouse_button_read_count = 0;

	input_globals.raw_mouse_state.x = input_globals.mouse_x_ticks * (input_globals.raw_mouse_state.relative_x / input_globals.mouse_relative_x);
	input_globals.raw_mouse_state.y = input_globals.mouse_y_ticks * (input_globals.raw_mouse_state.relative_y / input_globals.mouse_relative_y);

	if (input_globals.input_type_suppressed[_input_type_game])
	{
		input_globals.raw_mouse_wheel_update_time = 0;
	}
	else
	{
		if ((input_globals.update_time - input_globals.raw_mouse_wheel_update_time) > 700 /* wheel spam timeout */)
		{
			input_globals.raw_mouse_wheel_update_time = 0;
		}
		else
		{
			input_globals.raw_mouse_state.wheel_ticks = 0;
		}

		int32 wheel_ticks = input_globals.mouse_wheel_ticks * (input_globals.raw_mouse_state.wheel_delta / input_globals.mouse_wheel_delta);
		if (wheel_ticks && !input_globals.raw_mouse_wheel_update_time)
		{
			input_globals.raw_mouse_wheel_update_time = input_globals.update_time;
			input_globals.raw_mouse_state.wheel_ticks = wheel_ticks;
			return;
		}
	}

	input_globals.raw_mouse_state.relative_y %= input_globals.mouse_relative_y;
	input_globals.raw_mouse_state.relative_x %= input_globals.mouse_relative_x;
	input_globals.raw_mouse_state.wheel_delta %= input_globals.mouse_wheel_delta;
	input_globals.raw_mouse_state.hwheel_delta %= input_globals.mouse_wheel_delta;

	if (game_in_editor() || window_globals.hWnd == GetForegroundWindow())
	{
		for (int32 i = 0; i < k_mouse_button_count; i++)
		{
			uns8& frames_down = input_globals.raw_mouse_state.frames_down[i];
			uns16& msec_down = input_globals.raw_mouse_state.msec_down[i];
			bool key_down = input_globals.raw_mouse_state.raw_flags.test(e_mouse_button(i));
			update_button(&frames_down, &msec_down, key_down, elapsed_msec);
		}
	}
}

void __cdecl input_update_gamepads(int32 elapsed_msec)
{
	if (input_abstraction_get_controls_method() == 1)
	{
		for (uns32 gamepad_index = 0; gamepad_index < k_number_of_controllers; gamepad_index++)
		{
			gamepad_state& state = input_globals.gamepad_states[gamepad_index];
			debug_gamepad_data& debug_gamepad = g_debug_gamepad_data[gamepad_index];

			if (input_xinput_update_gamepad(gamepad_index, elapsed_msec, &state, &debug_gamepad))
			{
				input_globals.gamepad_valid_mask.set(gamepad_index, true);
			}
			else
			{
				input_globals.gamepad_valid_mask.set(gamepad_index, false);
				csmemset(&state, 0, sizeof(gamepad_state));
			}
		}
	}
}

void __cdecl input_update_gamepads_rumble()
{
	//INVOKE(0x005129F0, input_update_gamepads_rumble);

	bool suppressed = input_globals.feedback_suppressed || input_globals.input_suppressed || debugging_system_has_focus();
	if (game_in_progress())
	{
		if (game_time_get_paused())
		{
			suppressed = true;
		}
	}
	else
	{
		suppressed = true;
	}
	
	for (uns32 user_index = 0; user_index < k_number_of_controllers; user_index++)
	{
		//if (g_display_rumble_status_lines)
		//	g_rumble_status_lines[user_index].clear();
	
		input_xinput_update_rumble_state(user_index, &input_globals.rumble_states[user_index], suppressed);
	}
}

void __cdecl update_button(uns8* frames, uns16* msec, bool down, int32 elapsed_msec)
{
	INVOKE(0x00512B00, update_button, frames, msec, down, elapsed_msec);

	//*frames = down ? MIN(*frames + 1, UNSIGNED_CHAR_MAX) : 0;
	//*msec = down ? MIN(*msec + (uns16)elapsed_msec, UNSIGNED_SHORT_MAX) : 0;
}

void __cdecl update_key(s_input_key_state* key, bool down, int32 elapsed_msec)
{
	INVOKE(0x00512B50, update_key, key, down, elapsed_msec);

	//if (key->latched || !down)
	//{
	//	key->latched = false;
	//	update_button(&key->frames, &key->msec, down, elapsed_msec);
	//}
}

void input_handle_key_combos()
{
	if (!window_globals.editorWindowCreate)
	{
		if (input_key_frames_down(_key_alt, _input_type_ui) && input_key_frames_down(_key_return, _input_type_ui) == 1 ||
			input_key_frames_down(_key_alt, _input_type_ui) && input_key_frames_down(_keypad_enter, _input_type_ui) == 1)
		{
			global_preferences_set_fullscreen(!global_preferences_get_fullscreen());
			rasterizer_reset_device();
		}
	}

	if (input_key_frames_down(_key_control, _input_type_ui) && input_key_frames_down(_key_shift, _input_type_ui) && input_key_frames_down(_key_j, _input_type_ui) == 1)
	{
		c_console::toggle_window_visibility();
	}

	if (input_key_frames_down(_key_alt, _input_type_ui) && input_key_frames_down(_key_f4, _input_type_ui) == 1)
	{
		exit('zekk');
	}
}

void input_get_raw_data_string(char* buffer, int16 size)
{
	// For some reason buffer is displayed incorrectly for both H3EK and Donkey
	// commented out for now

	//ASSERT(buffer);
	//ASSERT(size > 0);
	//
	//if (buffer && size > 0)
	//{
	//	csnzprintf(buffer, size, "|n|n|n|ngamepad|tleft stick|tright stick|t|n");
	//	for (int16 gamepad_index = 0; gamepad_index < k_number_of_controllers; gamepad_index++)
	//	{
	//		if (input_globals.gamepad_valid_mask.test(gamepad_index))
	//		{
	//			csnzappendf(buffer, size, "gamepad %d|t(%d, %d)|t(%d, %d)|n",
	//				gamepad_index,
	//				g_debug_gamepad_data[gamepad_index].sticks[0].x,
	//				g_debug_gamepad_data[gamepad_index].sticks[0].y,
	//				g_debug_gamepad_data[gamepad_index].sticks[1].x,
	//				g_debug_gamepad_data[gamepad_index].sticks[1].y);
	//		}
	//	}
	//}
}

void input_mouse_state_get_raw_data_string(char* buffer, int16 size)
{
	ASSERT(buffer);
	ASSERT(size > 0);
	
	if (buffer && size > 0)
	{
		csnzappendf(buffer, size, "|n|n|n|ninput_mouse_state|n");

		csnzappendf(buffer, size, "x: %d|n", input_globals.raw_mouse_state.x);
		csnzappendf(buffer, size, "y: %d|n", input_globals.raw_mouse_state.y);
		csnzappendf(buffer, size, "wheel_ticks: %d|n", input_globals.raw_mouse_state.wheel_ticks);
		csnzappendf(buffer, size, "relative_x: %d|n", input_globals.raw_mouse_state.relative_x);
		csnzappendf(buffer, size, "relative_y: %d|n", input_globals.raw_mouse_state.relative_y);
		csnzappendf(buffer, size, "wheel_delta: %d|n", input_globals.raw_mouse_state.wheel_delta);
	}
}

//uns8 const key_to_virtual_table[k_key_code_count]
//{
//	VK_ESCAPE,           // _key_escape
//	VK_F1,               // _key_f1
//	VK_F2,               // _key_f2
//	VK_F3,               // _key_f3
//	VK_F4,               // _key_f4
//	VK_F5,               // _key_f5
//	VK_F6,               // _key_f6
//	VK_F7,               // _key_f7
//	VK_F8,               // _key_f8
//	VK_F9,               // _key_f9
//	VK_F10,              // _key_f10
//	VK_F11,              // _key_f11
//	VK_F12,              // _key_f12
//	VK_SNAPSHOT,         // _key_print_screen
//	VK_F14,              // _key_scroll_lock
//	VK_F15,              // _key_pause
//	VK_OEM_3,            // _key_backquote
//	'1',                 // _key_1
//	'2',                 // _key_2
//	'3',                 // _key_3
//	'4',                 // _key_4
//	'5',                 // _key_5
//	'6',                 // _key_6
//	'7',                 // _key_7
//	'8',                 // _key_8
//	'9',                 // _key_9
//	'0',                 // _key_0
//	VK_OEM_MINUS,        // _key_dash
//	VK_OEM_PLUS,         // _key_equal
//	VK_BACK,             // _key_backspace
//	VK_TAB,              // _key_tab
//	'Q',                 // _key_q
//	'W',                 // _key_w
//	'E',                 // _key_e
//	'R',                 // _key_r
//	'T',                 // _key_t
//	'Y',                 // _key_y
//	'U',                 // _key_u
//	'I',                 // _key_i
//	'O',                 // _key_o
//	'P',                 // _key_p
//	VK_OEM_4,            // _key_left_bracket
//	VK_OEM_6,            // _key_right_bracket
//	VK_OEM_5,            // _key_backslash
//	VK_CAPITAL,          // _key_caps_lock
//	'A',                 // _key_a
//	'S',                 // _key_s
//	'D',                 // _key_d
//	'F',                 // _key_f
//	'G',                 // _key_g
//	'H',                 // _key_h
//	'J',                 // _key_j
//	'K',                 // _key_k
//	'L',                 // _key_l
//	VK_OEM_1,            // _key_code_semicolon
//	VK_OEM_7,            // _key_apostrophe
//	VK_RETURN,           // _key_return
//	VK_LSHIFT,           // _key_left_shift
//	'Z',                 // _key_z
//	'X',                 // _key_x
//	'C',                 // _key_c
//	'V',                 // _key_v
//	'B',                 // _key_b
//	'N',                 // _key_n
//	'M',                 // _key_m
//	VK_OEM_COMMA,        // _key_comma
//	VK_OEM_PERIOD,       // _key_period
//	VK_OEM_2,            // _key_forwardslash
//	VK_RSHIFT,           // _key_right_shift
//	VK_LCONTROL,         // _key_left_control
//	VK_LWIN,             // _key_left_windows
//	VK_LMENU,            // _key_left_alt
//	VK_SPACE,            // _key_space
//	VK_RMENU,            // _key_right_alt
//	VK_RWIN,             // _key_right_windows
//	VK_APPS,             // _key_menu
//	VK_RCONTROL,         // _key_right_control
//	VK_UP,               // _key_up_arrow
//	VK_DOWN,             // _key_down_arrow
//	VK_LEFT,             // _key_left_arrow
//	VK_RIGHT,            // _key_right_arrow
//	VK_INSERT,           // _key_insert
//	VK_HOME,             // _key_home
//	VK_PRIOR,            // _key_page_up
//	VK_DELETE,           // _key_delete
//	VK_END,              // _key_end
//	VK_NEXT,             // _key_page_down
//	VK_NUMLOCK,          // _keypad_num_lock
//	VK_DIVIDE,           // _keypad_divide
//	VK_MULTIPLY,         // _keypad_multiply
//	VK_NUMPAD0,          // _keypad_0
//	VK_NUMPAD1,          // _keypad_1
//	VK_NUMPAD2,          // _keypad_2
//	VK_NUMPAD3,          // _keypad_3
//	VK_NUMPAD4,          // _keypad_4
//	VK_NUMPAD5,          // _keypad_5
//	VK_NUMPAD6,          // _keypad_6
//	VK_NUMPAD7,          // _keypad_7
//	VK_NUMPAD8,          // _keypad_8
//	VK_NUMPAD9,          // _keypad_9
//	VK_SUBTRACT,         // _keypad_subtract
//	VK_ADD,              // _keypad_add
//	VK_RETURN,           // _keypad_enter
//	VK_DECIMAL           // _keypad_decimal
//};
//
//int16 const virtual_to_key_table[k_number_of_windows_input_virtual_codes]
//{
//	_key_not_a_key,
//	_key_not_a_key,      // VK_LBUTTON
//	_key_not_a_key,      // VK_RBUTTON
//	_key_not_a_key,      // VK_CANCEL
//	_key_not_a_key,      // VK_MBUTTON
//	_key_not_a_key,      // VK_XBUTTON1
//	_key_not_a_key,      // VK_XBUTTON2
//	_key_not_a_key,      // reserved
//	_key_backspace,      // VK_BACK
//	_key_tab,            // VK_TAB
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // VK_CLEAR
//	_key_return,         // VK_RETURN
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_shift,          // VK_SHIFT
//	_key_control,        // VK_CONTROL
//	_key_left_alt,       // VK_MENU
//	_key_pause,          // VK_PAUSE
//	_key_caps_lock,      // VK_CAPITAL
//	_key_not_a_key,      // VK_KANA, VK_HANGEUL, VK_HANGUL
//	_key_not_a_key,      // VK_IME_ON
//	_key_not_a_key,      // VK_JUNJA
//	_key_not_a_key,      // VK_FINAL
//	_key_not_a_key,      // VK_HANJA, VK_KANJI
//	_key_not_a_key,      // VK_IME_OFF
//	_key_escape,         // VK_ESCAPE
//	_key_not_a_key,      // VK_CONVERT
//	_key_not_a_key,      // VK_NONCONVERT
//	_key_not_a_key,      // VK_ACCEPT
//	_key_not_a_key,      // VK_MODECHANGE
//	_key_space,          // VK_SPACE
//	_key_page_up,        // VK_PRIOR
//	_key_page_down,      // VK_NEXT
//	_key_end,            // VK_END
//	_key_home,           // VK_HOME
//	_key_left_arrow,     // VK_LEFT
//	_key_up_arrow,       // VK_UP
//	_key_right_arrow,    // VK_RIGHT
//	_key_down_arrow,     // VK_DOWN
//	_key_not_a_key,      // VK_SELECT
//	_key_not_a_key,      // VK_PRINT
//	_key_not_a_key,      // VK_EXECUTE
//	_key_print_screen,   // VK_SNAPSHOT
//	_key_insert,         // VK_INSERT
//	_key_delete,         // VK_DELETE
//	_key_not_a_key,      // VK_HELP
//	_key_0,              // VK_0
//	_key_1,              // VK_1
//	_key_2,              // VK_2
//	_key_3,              // VK_3
//	_key_4,              // VK_4
//	_key_5,              // VK_5
//	_key_6,              // VK_6
//	_key_7,              // VK_7
//	_key_8,              // VK_8
//	_key_9,              // VK_9
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_a,              // VK_A
//	_key_b,              // VK_B
//	_key_c,              // VK_C
//	_key_d,              // VK_D
//	_key_e,              // VK_E
//	_key_f,              // VK_F
//	_key_g,              // VK_G
//	_key_h,              // VK_H
//	_key_i,              // VK_I
//	_key_j,              // VK_J
//	_key_k,              // VK_K
//	_key_l,              // VK_L
//	_key_m,              // VK_M
//	_key_n,              // VK_N
//	_key_o,              // VK_O
//	_key_p,              // VK_P
//	_key_q,              // VK_Q
//	_key_r,              // VK_R
//	_key_s,              // VK_S
//	_key_t,              // VK_T
//	_key_u,              // VK_U
//	_key_v,              // VK_V
//	_key_w,              // VK_W
//	_key_x,              // VK_X
//	_key_y,              // VK_Y
//	_key_z,              // VK_Z
//	_key_left_windows,   // VK_LWIN
//	_key_right_windows,  // VK_RWIN
//	_key_menu,           // VK_APPS
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // VK_SLEEP
//	_keypad_0,           // VK_NUMPAD0
//	_keypad_1,           // VK_NUMPAD1
//	_keypad_2,           // VK_NUMPAD2
//	_keypad_3,           // VK_NUMPAD3
//	_keypad_4,           // VK_NUMPAD4
//	_keypad_5,           // VK_NUMPAD5
//	_keypad_6,           // VK_NUMPAD6
//	_keypad_7,           // VK_NUMPAD7
//	_keypad_8,           // VK_NUMPAD8
//	_keypad_9,           // VK_NUMPAD9
//	_keypad_multiply,    // VK_MULTIPLY
//	_keypad_add,         // VK_ADD
//	_key_not_a_key,      // VK_SEPARATOR
//	_keypad_subtract,    // VK_SUBTRACT
//	_keypad_decimal,     // VK_DECIMAL
//	_keypad_divide,      // VK_DIVIDE
//	_key_f1,             // VK_F1
//	_key_f2,             // VK_F2
//	_key_f3,             // VK_F3
//	_key_f4,             // VK_F4
//	_key_f5,             // VK_F5
//	_key_f6,             // VK_F6
//	_key_f7,             // VK_F7
//	_key_f8,             // VK_F8
//	_key_f9,             // VK_F9
//	_key_f10,            // VK_F10
//	_key_f11,            // VK_F11
//	_key_f12,            // VK_F12
//	_key_not_a_key,      // VK_F13
//	_key_not_a_key,      // VK_F14
//	_key_not_a_key,      // VK_F15
//	_key_not_a_key,      // VK_F16
//	_key_not_a_key,      // VK_F17
//	_key_not_a_key,      // VK_F18
//	_key_not_a_key,      // VK_F19
//	_key_not_a_key,      // VK_F20
//	_key_not_a_key,      // VK_F21
//	_key_not_a_key,      // VK_F22
//	_key_not_a_key,      // VK_F23
//	_key_not_a_key,      // VK_F24
//	_key_not_a_key,      // VK_NAVIGATION_VIEW
//	_key_not_a_key,      // VK_NAVIGATION_MENU
//	_key_not_a_key,      // VK_NAVIGATION_UP
//	_key_not_a_key,      // VK_NAVIGATION_DOWN
//	_key_not_a_key,      // VK_NAVIGATION_LEFT
//	_key_not_a_key,      // VK_NAVIGATION_RIGHT
//	_key_not_a_key,      // VK_NAVIGATION_ACCEPT
//	_key_not_a_key,      // VK_NAVIGATION_CANCEL
//	_keypad_num_lock,    // VK_NUMLOCK
//	_key_scroll_lock,    // VK_SCROLL
//	_key_not_a_key,      // VK_OEM_NEC_EQUAL, VK_OEM_FJ_JISHO
//	_key_not_a_key,      // VK_OEM_FJ_MASSHOU
//	_key_not_a_key,      // VK_OEM_FJ_TOUROKU
//	_key_not_a_key,      // VK_OEM_FJ_LOYA
//	_key_not_a_key,      // VK_OEM_FJ_ROYA
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // VK_LSHIFT
//	_key_not_a_key,      // VK_RSHIFT
//	_key_not_a_key,      // VK_LCONTROL
//	_key_not_a_key,      // VK_RCONTROL
//	_key_not_a_key,      // VK_LMENU
//	_key_not_a_key,      // VK_RMENU
//	_key_not_a_key,      // VK_BROWSER_BACK
//	_key_not_a_key,      // VK_BROWSER_FORWARD
//	_key_not_a_key,      // VK_BROWSER_REFRESH
//	_key_not_a_key,      // VK_BROWSER_STOP
//	_key_not_a_key,      // VK_BROWSER_SEARCH
//	_key_not_a_key,      // VK_BROWSER_FAVORITES
//	_key_not_a_key,      // VK_BROWSER_HOME
//	_key_not_a_key,      // VK_VOLUME_MUTE
//	_key_not_a_key,      // VK_VOLUME_DOWN
//	_key_not_a_key,      // VK_VOLUME_UP
//	_key_not_a_key,      // VK_MEDIA_NEXT_TRACK
//	_key_not_a_key,      // VK_MEDIA_PREV_TRACK
//	_key_not_a_key,      // VK_MEDIA_STOP
//	_key_not_a_key,      // VK_MEDIA_PLAY_PAUSE
//	_key_not_a_key,      // VK_LAUNCH_MAIL
//	_key_not_a_key,      // VK_LAUNCH_MEDIA_SELECT
//	_key_not_a_key,      // VK_LAUNCH_APP1
//	_key_not_a_key,      // VK_LAUNCH_APP2
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // reserved
//	_key_code_semicolon, // VK_OEM_1
//	_key_equal,          // VK_OEM_PLUS
//	_key_comma,          // VK_OEM_COMMA
//	_key_dash,           // VK_OEM_MINUS
//	_key_period,         // VK_OEM_PERIOD
//	_key_forwardslash,   // VK_OEM_2
//	_key_backquote,      // VK_OEM_3
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // VK_GAMEPAD_A
//	_key_not_a_key,      // VK_GAMEPAD_B
//	_key_not_a_key,      // VK_GAMEPAD_X
//	_key_not_a_key,      // VK_GAMEPAD_Y
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_SHOULDER
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_SHOULDER
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_TRIGGER
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_TRIGGER
//	_key_not_a_key,      // VK_GAMEPAD_DPAD_UP
//	_key_not_a_key,      // VK_GAMEPAD_DPAD_DOWN
//	_key_not_a_key,      // VK_GAMEPAD_DPAD_LEFT
//	_key_not_a_key,      // VK_GAMEPAD_DPAD_RIGHT
//	_key_not_a_key,      // VK_GAMEPAD_MENU
//	_key_not_a_key,      // VK_GAMEPAD_VIEW
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_THUMBSTICK_UP
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_THUMBSTICK_DOWN
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT
//	_key_not_a_key,      // VK_GAMEPAD_LEFT_THUMBSTICK_LEFT
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_THUMBSTICK_UP
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT
//	_key_not_a_key,      // VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT
//	_key_left_bracket,   // VK_OEM_4
//	_key_backslash,      // VK_OEM_5
//	_key_right_bracket,  // VK_OEM_6
//	_key_apostrophe,     // VK_OEM_7
//	_key_not_a_key,      // VK_OEM_8
//	_key_not_a_key,      // reserved
//	_key_not_a_key,      // VK_OEM_AX
//	_key_not_a_key,      // VK_OEM_102
//	_key_not_a_key,      // VK_ICO_HELP
//	_key_not_a_key,      // VK_ICO_00
//	_key_not_a_key,      // VK_PROCESSKEY
//	_key_not_a_key,      // VK_ICO_CLEAR
//	_key_not_a_key,      // VK_PACKET
//	_key_not_a_key,      // unassigned
//	_key_not_a_key,      // VK_OEM_RESET
//	_key_not_a_key,      // VK_OEM_JUMP
//	_key_not_a_key,      // VK_OEM_PA1
//	_key_not_a_key,      // VK_OEM_PA2
//	_key_not_a_key,      // VK_OEM_PA3
//	_key_not_a_key,      // VK_OEM_WSCTRL
//	_key_not_a_key,      // VK_OEM_CUSEL
//	_key_not_a_key,      // VK_OEM_ATTN
//	_key_not_a_key,      // VK_OEM_FINISH
//	_key_not_a_key,      // VK_OEM_COPY
//	_key_not_a_key,      // VK_OEM_AUTO
//	_key_not_a_key,      // VK_OEM_ENLW
//	_key_not_a_key,      // VK_OEM_BACKTAB
//	_key_not_a_key,      // VK_ATTN
//	_key_not_a_key,      // VK_CRSEL
//	_key_not_a_key,      // VK_EXSEL
//	_key_not_a_key,      // VK_EREOF
//	_key_not_a_key,      // VK_PLAY
//	_key_not_a_key,      // VK_ZOOM
//	_key_not_a_key,      // VK_NONAME
//	_key_not_a_key,      // VK_PA1
//	_key_not_a_key,      // VK_OEM_CLEAR
//	_key_not_a_key,      // reserved
//};
//
//uns8 const key_to_ascii_table[k_key_code_count]
//{
//	NONE,                // _key_escape
//	NONE,                // _key_f1
//	NONE,                // _key_f2
//	NONE,                // _key_f3
//	NONE,                // _key_f4
//	NONE,                // _key_f5
//	NONE,                // _key_f6
//	NONE,                // _key_f7
//	NONE,                // _key_f8
//	NONE,                // _key_f9
//	NONE,                // _key_f10
//	NONE,                // _key_f11
//	NONE,                // _key_f12
//	NONE,                // _key_print_screen
//	NONE,                // _key_scroll_lock
//	NONE,                // _key_pause
//	'`',                 // _key_backquote
//	'1',                 // _key_1
//	'2',                 // _key_2
//	'3',                 // _key_3
//	'4',                 // _key_4
//	'5',                 // _key_5
//	'6',                 // _key_6
//	'7',                 // _key_7
//	'8',                 // _key_8
//	'9',                 // _key_9
//	'0',                 // _key_0
//	'-',                 // _key_dash
//	'=',                 // _key_equal
//	'\x08',              // _key_backspace
//	'\x09',              // _key_tab
//	'Q',                 // _key_q
//	'W',                 // _key_w
//	'E',                 // _key_e
//	'R',                 // _key_r
//	'T',                 // _key_t
//	'Y',                 // _key_y
//	'U',                 // _key_u
//	'I',                 // _key_i
//	'O',                 // _key_o
//	'P',                 // _key_p
//	'[',                 // _key_left_bracket
//	']',                 // _key_right_bracket
//	'\\',                // _key_backslash
//	NONE,                // _key_caps_lock
//	'A',                 // _key_a
//	'S',                 // _key_s
//	'D',                 // _key_d
//	'F',                 // _key_f
//	'G',                 // _key_g
//	'H',                 // _key_h
//	'J',                 // _key_j
//	'K',                 // _key_k
//	'L',                 // _key_l
//	';',                 // _key_code_semicolon
//	'\'',                // _key_apostrophe
//	'\x0D',              // _key_return
//	NONE,                // _key_left_shift
//	'Z',                 // _key_z
//	'X',                 // _key_x
//	'C',                 // _key_c
//	'V',                 // _key_v
//	'B',                 // _key_b
//	'N',                 // _key_n
//	'M',                 // _key_m
//	',',                 // _key_comma
//	'.',                 // _key_period
//	'/',                 // _key_forwardslash
//	NONE,                // _key_right_shift
//	NONE,                // _key_left_control
//	NONE,                // _key_left_windows
//	NONE,                // _key_left_alt
//	' ',                 // _key_space
//	NONE,                // _key_right_alt
//	NONE,                // _key_right_windows
//	NONE,                // _key_menu
//	NONE,                // _key_right_control
//	NONE,                // _key_up_arrow
//	NONE,                // _key_down_arrow
//	NONE,                // _key_left_arrow
//	NONE,                // _key_right_arrow
//	NONE,                // _key_insert
//	NONE,                // _key_home
//	NONE,                // _key_page_up
//	NONE,                // _key_delete
//	NONE,                // _key_end
//	NONE,                // _key_page_down
//	NONE,                // _keypad_num_lock
//	'/',                 // _keypad_divide
//	'*',                 // _keypad_multiply
//	'0',                 // _keypad_0
//	'1',                 // _keypad_1
//	'2',                 // _keypad_2
//	'3',                 // _keypad_3
//	'4',                 // _keypad_4
//	'5',                 // _keypad_5
//	'6',                 // _keypad_6
//	'7',                 // _keypad_7
//	'8',                 // _keypad_8
//	'9',                 // _keypad_9
//	'-',                 // _keypad_subtract
//	'+',                 // _keypad_add
//	'\x0D',              // _keypad_enter
//	'.',                 // _keypad_decimal
//};
//
//int16 const ascii_to_key_table[k_number_of_input_ascii_codes]
//{
//	_key_not_a_key,      // NUL    Null
//	_key_not_a_key,      // SOH    Start of Heading
//	_key_not_a_key,      // STX    Start of Text
//	_key_not_a_key,      // ETX    End of Text
//	_key_not_a_key,      // EOT    End of Transmission
//	_key_not_a_key,      // ENQ    Enquiry
//	_key_not_a_key,      // ACK    Acknowledge
//	_key_not_a_key,      // BEL    Bell
//	_key_not_a_key,      // BS     Backspace
//	_key_not_a_key,      // HT     Horizontal Tab
//	_key_not_a_key,      // LF     Line Feed
//	_key_not_a_key,      // VT     Vertical Tab
//	_key_not_a_key,      // FF     Form Feed
//	_key_not_a_key,      // CR     Carriage Return
//	_key_not_a_key,      // SO     Shift Out
//	_key_not_a_key,      // SI     Shift In
//	_key_not_a_key,      // DLE    Data Link Escape
//	_key_not_a_key,      // DC1    Device Control 1
//	_key_not_a_key,      // DC2    Device Control 2
//	_key_not_a_key,      // DC3    Device Control 3
//	_key_not_a_key,      // DC4    Device Control 4
//	_key_not_a_key,      // NAK    Negative Acknowledge
//	_key_not_a_key,      // SYN    Synchronize
//	_key_not_a_key,      // ETB    End of Transmission Block
//	_key_not_a_key,      // CAN    Cancel
//	_key_not_a_key,      // EM     End of Medium
//	_key_not_a_key,      // SUB    Substitute
//	_key_not_a_key,      // ESC    Escape
//	_key_not_a_key,      // FS     File Separator
//	_key_not_a_key,      // GS     Group Separator
//	_key_not_a_key,      // RS     Record Separator
//	_key_not_a_key,      // US     Unit Separator
//	_key_space,          // space  Space
//	_key_1,              // !      exclamation mark
//	_key_apostrophe,     // "      double quote
//	_key_3,              // #      number
//	_key_4,              // $      dollar
//	_key_5,              // %      percent
//	_key_7,              // &      ampersand
//	_key_apostrophe,     // '      single quote
//	_key_9,              // (      left parenthesis
//	_key_0,              // )      right parenthesis
//	_key_8,              // *      asterisk
//	_key_equal,          // +      plus
//	_key_comma,          // ,      comma
//	_key_dash,           // -      minus
//	_key_period,         // .      period
//	_key_forwardslash,   // /      slash
//	_key_0,              // 0      zero
//	_key_1,              // 1      one
//	_key_2,              // 2      two
//	_key_3,              // 3      three
//	_key_4,              // 4      four
//	_key_5,              // 5      five
//	_key_6,              // 6      six
//	_key_7,              // 7      seven
//	_key_8,              // 8      eight
//	_key_9,              // 9      nine
//	_key_code_colon,     // :      colon
//	_key_code_semicolon, // ;      semicolon
//	_key_comma,          // <      less than
//	_key_equal,          // =      equality sign
//	_key_period,         // >      greater than
//	_key_forwardslash,   // ?      question mark
//	_key_2,              // @      at sign
//	_key_a,              // A
//	_key_b,              // B
//	_key_c,              // C
//	_key_d,              // D
//	_key_e,              // E
//	_key_f,              // F
//	_key_g,              // G
//	_key_h,              // H
//	_key_i,              // I
//	_key_j,              // J
//	_key_k,              // K
//	_key_l,              // L
//	_key_m,              // M
//	_key_n,              // N
//	_key_o,              // O
//	_key_p,              // P
//	_key_q,              // Q
//	_key_r,              // R
//	_key_s,              // S
//	_key_t,              // T
//	_key_u,              // U
//	_key_v,              // V
//	_key_w,              // W
//	_key_x,              // X
//	_key_y,              // Y
//	_key_z,              // Z
//	_key_left_bracket,   // [      left square bracket
//	_key_backslash,      // \      backslash
//	_key_right_bracket,  // ]      right square bracket
//	_key_6,              // ^      caret / circumflex
//	_key_dash,           // _      underscore
//	_key_backquote,      // `      grave / accent
//	_key_a,              // a
//	_key_b,              // b
//	_key_c,              // c
//	_key_d,              // d
//	_key_e,              // e
//	_key_f,              // f
//	_key_g,              // g
//	_key_h,              // h
//	_key_i,              // i
//	_key_j,              // j
//	_key_k,              // k
//	_key_l,              // l
//	_key_m,              // m
//	_key_n,              // n
//	_key_o,              // o
//	_key_p,              // p
//	_key_q,              // q
//	_key_r,              // r
//	_key_s,              // s
//	_key_t,              // t
//	_key_u,              // u
//	_key_v,              // v
//	_key_w,              // w
//	_key_x,              // x
//	_key_y,              // y
//	_key_z,              // z
//	_key_left_bracket,   // {      left curly bracket
//	_key_backslash,      // |      vertical bar
//	_key_right_bracket,  // }      right curly bracket
//	_key_backquote,      // ~      tilde
//	_key_delete          // DEL    delete
//};

