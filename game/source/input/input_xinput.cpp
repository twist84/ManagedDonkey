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

#define ADJUST_THUMB_AXIS_DEADZONE_SHORT(THUMB_AXIS, THUMB_DEADZONE) (short)input_xinput_adjust_thumb_axis_deadzone((THUMB_AXIS), (THUMB_DEADZONE))

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

word xinput_buttons[]
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
long const k_xinput_button_count = NUMBEROF(xinput_buttons);

c_static_array<debug_gamepad_data, 4> g_debug_gamepad_data = {};

double __cdecl input_xinput_adjust_thumb_axis_deadzone(double thumb_axis, double thumb_deadzone)
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

dword __cdecl input_xinput_get_state(dword user_index, _XINPUT_STATE* state)
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
		ASSERT2("please don't try to initialize xinput multiple times!");

	if (XInput_module = LoadLibrary(XINPUT_DLL))
	{
		XInputGetState_proxy = (XInputGetState_proxy_t*)GetProcAddress(XInput_module, "XInputGetState");
		XInputSetState_proxy = (XInputSetState_proxy_t*)GetProcAddress(XInput_module, "XInputSetState");
	}
	else
	{
		generate_event(_event_level_message, "system:input: XInput dll ('%s') not found, controllers will be unavailable", XINPUT_DLL_A);
	}

	return XInput_module != NULL;
#endif
}

dword __cdecl input_xinput_set_state(dword user_index, _XINPUT_VIBRATION* vibration)
{
	//return INVOKE(0x0065EF40, input_xinput_set_state, user_index, vibration);

	return XInputSetState_proxy(user_index, vibration);
}

bool __cdecl input_xinput_update_gamepad(dword gamepad_index, dword duration_ms, struct gamepad_state* gamepad_state, debug_gamepad_data* out_debug_gamepad_data)
{
	//bool result = INVOKE(0x0065EF60, input_xinput_update_gamepad, gamepad_index, duration_ms, gamepad_state, out_debug_gamepad_data);
	//if (result)
	//{
	//	if (!out_debug_gamepad_data)
	//		out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];
	//
	//	out_debug_gamepad_data->thumb_left = gamepad_state->thumb_left;
	//	out_debug_gamepad_data->thumb_right = gamepad_state->thumb_right;
	//}
	//return result;

	if (!input_xinput_available())
		return false;

	XINPUT_STATE state{};
	if (!XInputGetState_proxy(gamepad_index, &state))
	{
		for (short trigger_index = 0; trigger_index < 2; trigger_index++)
		{
			byte& trigger_msec_down = gamepad_state->trigger_msec_down[trigger_index];
			byte& max_trigger_msec_down = gamepad_state->max_trigger_msec_down[trigger_index];
			byte& button_frames_down = gamepad_state->button_frames_down[trigger_index];
			word& button_msec_down = gamepad_state->button_msec_down[trigger_index];

			trigger_msec_down = trigger_index ? state.Gamepad.bRightTrigger : state.Gamepad.bLeftTrigger;
			bool trigger_down = trigger_msec_down > max_trigger_msec_down;

			input_xinput_update_button(&button_frames_down, &button_msec_down, trigger_down, duration_ms);
			input_xinput_update_trigger(&trigger_msec_down, trigger_down, (byte)duration_ms);
		}

		for (long button_index = 0; button_index < k_xinput_button_count; button_index++)
		{
			byte& button_frames_down = gamepad_state->button_frames_down[_controller_button_dpad_up + button_index];
			word& button_msec_down = gamepad_state->button_msec_down[_controller_button_dpad_up + button_index];

			bool button_down = TEST_MASK(state.Gamepad.wButtons, xinput_buttons[button_index]);

			input_xinput_update_button(&button_frames_down, &button_msec_down, button_down, duration_ms);
		}

		// In Halo Online `out_debug_gamepad_data` is NULL, we "fix" that here
		if (!out_debug_gamepad_data)
			out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];

		ASSERT(out_debug_gamepad_data);
		out_debug_gamepad_data->thumb_left.x = state.Gamepad.sThumbLX;
		out_debug_gamepad_data->thumb_left.y = state.Gamepad.sThumbLY;
		out_debug_gamepad_data->thumb_right.x = state.Gamepad.sThumbRX;
		out_debug_gamepad_data->thumb_right.y = state.Gamepad.sThumbRY;

		input_xinput_update_thumbstick(true, &gamepad_state->thumb_left, state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
		input_xinput_update_thumbstick(false, &gamepad_state->thumb_right, state.Gamepad.sThumbRX, state.Gamepad.sThumbRY);

		return true;
	}

	csmemset(gamepad_state, 0, sizeof(struct gamepad_state));
	return false;
}

void __cdecl input_xinput_update_rumble_state(dword user_index, rumble_state const* state, bool suppressed)
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

void __cdecl input_xinput_update_thumbstick(bool left_thumb, int16_point2d* thumbstick, short thumb_x, short thumb_y)
{
	//INVOKE(0x0065F280, input_xinput_update_thumbstick, left_thumb, thumbstick, thumb_x, thumb_y);

	thumbstick->x = ADJUST_THUMB_AXIS_DEADZONE_SHORT(thumb_x, left_thumb ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	thumbstick->y = ADJUST_THUMB_AXIS_DEADZONE_SHORT(thumb_y, left_thumb ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void __cdecl input_xinput_update_button(byte* button_frames_down, word* button_msec_down, bool button_down, long duration_ms)
{
	//INVOKE(0x0065F380, input_xinput_update_button, button_frames_down, button_msec_down, button_down, duration_ms);

	*button_frames_down = button_down ? MIN(*button_frames_down + 1, UNSIGNED_CHAR_MAX) : 0;
	*button_msec_down = button_down ? MIN(*button_msec_down + (word)duration_ms, UNSIGNED_SHORT_MAX) : 0;
}

void __cdecl input_xinput_update_trigger(byte* trigger_msec_down, bool trigger_down, byte duration_ms)
{
	//INVOKE(0x0065F3D0, input_xinput_update_trigger, trigger_msec_down, trigger_down, duration_ms);

	if (trigger_down)
	{
		byte msec_down = CLAMP_LOWER(duration_ms, 0, 32);
		if (*trigger_msec_down <= msec_down)
			*trigger_msec_down = msec_down;
	}
	else
	{
		byte msec_down = CLAMP_UPPER(duration_ms, 64, 255);
		if (*trigger_msec_down >= msec_down)
			*trigger_msec_down = msec_down;
	}
}

