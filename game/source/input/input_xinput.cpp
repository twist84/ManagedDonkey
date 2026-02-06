#include "input/input_xinput.hpp"

#include "cseries/cseries_events.hpp"
#include "input/input_windows.hpp"
#include "memory/module.hpp"

#include <windows.h>
#include <math.h>
#include <XInput.h>

#if defined(_DEBUG)
#define STATIC_XINPUT
#endif

#ifdef STATIC_XINPUT
#pragma comment(lib, "xinput.lib")
#endif

#define ADJUST_THUMB_AXIS_DEADZONE_SHORT(THUMB_AXIS, THUMB_DEADZONE) (int16)input_xinput_adjust_thumb_axis_deadzone((THUMB_AXIS), (THUMB_DEADZONE))

using XInputGetState_proxy_t = DWORD WINAPI(DWORD dwUserIndex, XINPUT_STATE* pState);
using XInputSetState_proxy_t = DWORD WINAPI(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

#if defined(STATIC_XINPUT)
XInputGetState_proxy_t* XInputGetState_proxy = XInputGetState;
XInputSetState_proxy_t* XInputSetState_proxy = XInputSetState;
#else
REFERENCE_DECLARE(0x02497CE4, XInputGetState_proxy_t*, XInputGetState_proxy);
REFERENCE_DECLARE(0x02497CE8, XInputSetState_proxy_t*, XInputSetState_proxy);
REFERENCE_DECLARE(0x02497CEC, HMODULE, XInput_module);
#endif

HOOK_DECLARE(0x0065EE00, input_xinput_adjust_thumb_axis_deadzone);
HOOK_DECLARE(0x0065EE80, input_xinput_available);
HOOK_DECLARE(0x0065EEB0, input_xinput_dispose);
HOOK_DECLARE(0x0065EEE0, input_xinput_get_state);
HOOK_DECLARE(0x0065EF00, input_xinput_initialize);
HOOK_DECLARE(0x0065EF40, input_xinput_set_state);
HOOK_DECLARE(0x0065EF60, input_xinput_update_gamepad);
HOOK_DECLARE(0x0065F220, input_xinput_update_rumble_state);
HOOK_DECLARE(0x0065F280, input_xinput_update_thumbstick);
HOOK_DECLARE(0x0065F380, input_xinput_update_button);
HOOK_DECLARE(0x0065F3D0, update_threshold);

enum
{
	NUMBER_OF_VIRTUAL_CODES = 256,
	NUMBER_OF_ASCII_CODES = 128,
	MAXIMUM_BUFFERED_KEYSTROKES = 64,
	k_key_was_down_bit = 30,
	k_trigger_dead_zone = 6,
	// k_thumbstick_dead_zone = 7864,
};

constexpr real32 k_thumbstick_dead_zone = 0.24f;

static constexpr WORD button_to_xinput_button_mask[] =
{
	0,                             // _gamepad_analog_button_left_trigger
	0,                             // _gamepad_analog_button_right_trigger
	XINPUT_GAMEPAD_DPAD_UP,        // _gamepad_binary_button_dpad_up
	XINPUT_GAMEPAD_DPAD_DOWN,      // _gamepad_binary_button_dpad_down
	XINPUT_GAMEPAD_DPAD_LEFT,      // _gamepad_binary_button_dpad_left
	XINPUT_GAMEPAD_DPAD_RIGHT,     // _gamepad_binary_button_dpad_right
	XINPUT_GAMEPAD_START,          // _gamepad_binary_button_start
	XINPUT_GAMEPAD_BACK,           // _gamepad_binary_button_back
	XINPUT_GAMEPAD_LEFT_THUMB,     // _gamepad_binary_button_left_thumb
	XINPUT_GAMEPAD_RIGHT_THUMB,    // _gamepad_binary_button_right_thumb
	XINPUT_GAMEPAD_A,              // _gamepad_binary_button_a
	XINPUT_GAMEPAD_B,              // _gamepad_binary_button_b
	XINPUT_GAMEPAD_X,              // _gamepad_binary_button_x
	XINPUT_GAMEPAD_Y,              // _gamepad_binary_button_y
	XINPUT_GAMEPAD_LEFT_SHOULDER,  // _gamepad_binary_button_left_bumper
	XINPUT_GAMEPAD_RIGHT_SHOULDER, // _gamepad_binary_button_right_bumper
};
COMPILE_ASSERT(NUMBEROF(button_to_xinput_button_mask) == NUMBER_OF_GAMEPAD_BUTTONS);

c_static_array<debug_gamepad_data, 4> g_debug_gamepad_data = {};

real64 __cdecl input_xinput_adjust_thumb_axis_deadzone(real64 thumb_axis, real64 thumb_deadzone)
{
	//return INVOKE(0x0065EE00, input_xinput_adjust_thumb_axis_deadzone, thumb_axis, thumb_deadzone);

	if (thumb_axis > thumb_deadzone)
		return (thumb_axis - thumb_deadzone) * 0x7FFF / (0x7FFF - thumb_deadzone);
	
	if (-thumb_deadzone > thumb_axis)
		return (thumb_axis + thumb_deadzone) * ~0x7FFF / (~0x7FFF + thumb_deadzone);
	
	return 0.0;
}

bool __cdecl input_xinput_available()
{
	//return INVOKE(0x0065EE80, input_xinput_available);

#if defined(STATIC_XINPUT)
	return XInputGetState_proxy && XInputSetState_proxy;
#else
	return XInput_module && XInputGetState_proxy && XInputSetState_proxy;
#endif
}

void __cdecl input_xinput_dispose()
{
	//INVOKE(0x0065EEB0, input_xinput_dispose);

#if defined(STATIC_XINPUT)
	XInputGetState_proxy = nullptr;
	XInputSetState_proxy = nullptr;
#else
	if (XInput_module)
	{
		FreeLibrary(XInput_module);
		XInput_module = nullptr;
		XInputGetState_proxy = nullptr;
		XInputSetState_proxy = nullptr;
	}
#endif
}

uns32 __cdecl input_xinput_get_state(uns32 user_index, _XINPUT_STATE* state)
{
	//return INVOKE(0x0065EEE0, input_xinput_get_state, user_index, state);

	return XInputGetState_proxy(user_index, state);
}

bool __cdecl input_xinput_initialize()
{
	//INVOKE(0x0065EF00, input_xinput_initialize);

#if defined(STATIC_XINPUT)
	XInputGetState_proxy = XInputGetState;
	XInputSetState_proxy = XInputSetState;

	return true;
#else
	VASSERT(XInput_module == nullptr, "please don't try to initialize xinput multiple times!");

	if (XInput_module = LoadLibrary(XINPUT_DLL))
	{
		XInputGetState_proxy = (XInputGetState_proxy_t*)GetProcAddress(XInput_module, "XInputGetState");
		XInputSetState_proxy = (XInputSetState_proxy_t*)GetProcAddress(XInput_module, "XInputSetState");
	}
	else
	{
		event(_event_message, "system:input: XInput dll ('%s') not found, controllers will be unavailable", XINPUT_DLL_A);
	}

	return XInput_module != nullptr;
#endif
}

uns32 __cdecl input_xinput_set_state(uns32 user_index, _XINPUT_VIBRATION* vibration)
{
	//return INVOKE(0x0065EF40, input_xinput_set_state, user_index, vibration);

	return XInputSetState_proxy(user_index, vibration);
}

void update_trigger(uns8 input, uns8* output)
{
	uns8 value = input;
	if (value < k_trigger_dead_zone)
	{
		value = 0;
	}

	*output = value;
}

void update_thumbstick(int16 input_x, int16 input_y, point2d* output)
{
	real32 scale_x = fabsf((real32)(input_x > 0 ? INT16_MAX : INT16_MIN));
	real32 scale_y = fabsf((real32)(input_y > 0 ? INT16_MAX : INT16_MIN));

	real_vector2d thumbstick;
	set_real_vector2d(
		&thumbstick,
		(real32)input_x / scale_x,
		(real32)input_y / scale_y);

	real_vector2d thumbstick_l_direction = thumbstick;
	real32 magnitude = normalize2d(&thumbstick_l_direction);
	if (magnitude > 1.0f)
	{
		thumbstick = thumbstick_l_direction;
	}
	else if (magnitude < k_thumbstick_dead_zone)
	{
		set_real_vector2d(
			&thumbstick,
			0.0f,
			0.0f);
	}
	else
	{
		thumbstick = thumbstick_l_direction;
		real32 adjusted_magnitude = (magnitude - k_thumbstick_dead_zone) / (1.0f - k_thumbstick_dead_zone);
		scale_vector2d(&thumbstick, adjusted_magnitude, &thumbstick);
	}

	output->x = (int16)(thumbstick.i * scale_x);
	output->y = (int16)(thumbstick.j * scale_y);
}

bool __cdecl input_xinput_update_gamepad(uns32 gamepad_index, uns32 elapsed_msec, gamepad_state* in_out_gamepad_state, debug_gamepad_data* out_debug_gamepad_data)
{
	//bool result = INVOKE(0x0065EF60, input_xinput_update_gamepad, gamepad_index, elapsed_msec, in_out_gamepad_state, out_debug_gamepad_data);
	//if (result)
	//{
	//	if (!out_debug_gamepad_data)
	//		out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];
	//
	//	out_debug_gamepad_data->thumb_left = in_out_gamepad_state->thumb_left;
	//	out_debug_gamepad_data->thumb_right = in_out_gamepad_state->thumb_right;
	//}
	//return result;

	if (!input_xinput_available())
	{
		return false;
	}

	XINPUT_STATE xinput_state{};
	if (XInputGetState_proxy(gamepad_index, &xinput_state) == ERROR_SUCCESS)
	{
		for (long button_index = FIRST_GAMEPAD_BINARY_BUTTON; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
		{
			bool binary_down = TEST_MASK(xinput_state.Gamepad.wButtons, button_to_xinput_button_mask[button_index]);
			update_button(
				&in_out_gamepad_state->button_frames[button_index],
				&in_out_gamepad_state->button_msec[button_index],
				binary_down,
				elapsed_msec);
		}

		update_thumbstick(xinput_state.Gamepad.sThumbLX, xinput_state.Gamepad.sThumbLY, &in_out_gamepad_state->sticks[_gamepad_stick_left]);
		update_thumbstick(xinput_state.Gamepad.sThumbRX, xinput_state.Gamepad.sThumbRY, &in_out_gamepad_state->sticks[_gamepad_stick_right]);

		update_trigger(xinput_state.Gamepad.bLeftTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_left_trigger]);
		update_trigger(xinput_state.Gamepad.bRightTrigger, &in_out_gamepad_state->analog_buttons[_gamepad_analog_button_right_trigger]);

		for (long button_index = 0; button_index < NUMBER_OF_GAMEPAD_ANALOG_BUTTONS; button_index++)
		{
			bool binary_down = in_out_gamepad_state->analog_buttons[button_index] > in_out_gamepad_state->analog_button_thresholds[button_index];
			update_button(
				&in_out_gamepad_state->button_frames[button_index],
				&in_out_gamepad_state->button_msec[button_index],
				binary_down,
				elapsed_msec);
			update_threshold(
				&in_out_gamepad_state->analog_button_thresholds[button_index],
				binary_down,
				in_out_gamepad_state->analog_buttons[button_index]);
		}
		return true;
	}

	csmemset(in_out_gamepad_state, 0, sizeof(struct gamepad_state));
	return false;
}

void __cdecl input_xinput_update_rumble_state(uns32 user_index, const rumble_state* state, bool suppressed)
{
	//INVOKE(0x0065F220, input_xinput_update_rumble_state, user_index, state, no_rumble);

	if (input_xinput_available())
	{
		XINPUT_VIBRATION vibration = {};
		if (!suppressed)
		{
			vibration.wLeftMotorSpeed = state->left_motor_speed;
			vibration.wRightMotorSpeed = state->right_motor_speed;
		}
		input_xinput_set_state(user_index, &vibration);
	}
}

void __cdecl input_xinput_update_thumbstick(bool left_thumb, point2d* thumbstick, int16 thumb_x, int16 thumb_y)
{
	//INVOKE(0x0065F280, input_xinput_update_thumbstick, left_thumb, thumbstick, thumb_x, thumb_y);

	thumbstick->x = ADJUST_THUMB_AXIS_DEADZONE_SHORT(thumb_x, left_thumb ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	thumbstick->y = ADJUST_THUMB_AXIS_DEADZONE_SHORT(thumb_y, left_thumb ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void __cdecl input_xinput_update_button(uns8* button_frames, uns16* button_msec, bool button_down, int32 duration_ms)
{
	//INVOKE(0x0065F380, input_xinput_update_button, button_frames, button_msec, button_down, elapsed_msec);

	*button_frames = button_down ? MIN(*button_frames + 1, UNSIGNED_CHAR_MAX) : 0;
	*button_msec = button_down ? MIN(*button_msec + (uns16)duration_ms, UNSIGNED_SHORT_MAX) : 0;
}

void __cdecl update_threshold(uns8* analog_buttons, bool trigger_down, uns8 duration_ms)
{
	//INVOKE(0x0065F3D0, update_threshold, analog_buttons, trigger_down, elapsed_msec);

	if (trigger_down)
	{
		uns8 msec_down = CLAMP_LOWER(duration_ms, 0, 32);
		if (*analog_buttons <= msec_down)
			*analog_buttons = msec_down;
	}
	else
	{
		uns8 msec_down = CLAMP_UPPER(duration_ms, 64, 255);
		if (*analog_buttons >= msec_down)
			*analog_buttons = msec_down;
	}
}

