#include "input/input_xinput.hpp"

#include "cseries/cseries_events.hpp"
#include "input/input_windows.hpp"
#include "memory/module.hpp"

#include <windows.h>
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
HOOK_DECLARE(0x0065F3D0, input_xinput_update_trigger);

uns16 xinput_buttons[]
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER
};
int32 const k_xinput_button_count = NUMBEROF(xinput_buttons);

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
	XInputGetState_proxy = NULL;
	XInputSetState_proxy = NULL;
#else
	if (XInput_module)
	{
		FreeLibrary(XInput_module);
		XInput_module = NULL;
		XInputGetState_proxy = NULL;
		XInputSetState_proxy = NULL;
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
	if (XInput_module != NULL)
		VASSERT("please don't try to initialize xinput multiple times!");

	if (XInput_module = LoadLibrary(XINPUT_DLL))
	{
		XInputGetState_proxy = (XInputGetState_proxy_t*)GetProcAddress(XInput_module, "XInputGetState");
		XInputSetState_proxy = (XInputSetState_proxy_t*)GetProcAddress(XInput_module, "XInputSetState");
	}
	else
	{
		event(_event_message, "system:input: XInput dll ('%s') not found, controllers will be unavailable", XINPUT_DLL_A);
	}

	return XInput_module != NULL;
#endif
}

uns32 __cdecl input_xinput_set_state(uns32 user_index, _XINPUT_VIBRATION* vibration)
{
	//return INVOKE(0x0065EF40, input_xinput_set_state, user_index, vibration);

	return XInputSetState_proxy(user_index, vibration);
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

	XINPUT_STATE state{};
	if (!XInputGetState_proxy(gamepad_index, &state))
	{
		for (int16 trigger_index = 0; trigger_index < 2; trigger_index++)
		{
			uns8& analog_buttons = in_out_gamepad_state->analog_buttons[trigger_index];
			uns8& analog_button_thresholds = in_out_gamepad_state->analog_button_thresholds[trigger_index];
			uns8& button_frames = in_out_gamepad_state->button_frames[trigger_index];
			uns16& button_msec = in_out_gamepad_state->button_msec[trigger_index];

			analog_buttons = trigger_index ? state.Gamepad.bRightTrigger : state.Gamepad.bLeftTrigger;
			bool trigger_down = analog_buttons > analog_button_thresholds;

			input_xinput_update_button(&button_frames, &button_msec, trigger_down, elapsed_msec);
			input_xinput_update_trigger(&analog_buttons, trigger_down, (uns8)elapsed_msec);
		}

		for (int32 button_index = 0; button_index < k_xinput_button_count; button_index++)
		{
			uns8& button_frames = in_out_gamepad_state->button_frames[_controller_button_dpad_up + button_index];
			uns16& button_msec = in_out_gamepad_state->button_msec[_controller_button_dpad_up + button_index];

			bool button_down = TEST_MASK(state.Gamepad.wButtons, xinput_buttons[button_index]);

			input_xinput_update_button(&button_frames, &button_msec, button_down, elapsed_msec);
		}

		// In Halo Online `out_debug_gamepad_data` is NULL, we "fix" that here
		if (!out_debug_gamepad_data)
		{
			out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];
		}

		ASSERT(out_debug_gamepad_data);
		out_debug_gamepad_data->sticks[0].x = state.Gamepad.sThumbLX;
		out_debug_gamepad_data->sticks[0].y = state.Gamepad.sThumbLY;
		out_debug_gamepad_data->sticks[1].x = state.Gamepad.sThumbRX;
		out_debug_gamepad_data->sticks[1].y = state.Gamepad.sThumbRY;

		input_xinput_update_thumbstick(true, &in_out_gamepad_state->thumb_left, state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
		input_xinput_update_thumbstick(false, &in_out_gamepad_state->thumb_right, state.Gamepad.sThumbRX, state.Gamepad.sThumbRY);

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

void __cdecl input_xinput_update_trigger(uns8* analog_buttons, bool trigger_down, uns8 duration_ms)
{
	//INVOKE(0x0065F3D0, input_xinput_update_trigger, analog_buttons, trigger_down, elapsed_msec);

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

