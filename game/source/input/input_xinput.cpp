#include "input/input_xinput.hpp"

#include "input/input_windows.hpp"
#include "memory/module.hpp"

#include <Windows.h>
#include <XInput.h>

REFERENCE_DECLARE(0x02497CE4, s_xinput_globals, xinput_globals);

HOOK_DECLARE_CALL(0x005128FB, input_xinput_update_gamepad);

//HOOK_DECLARE(0x0065EE80, input_xinput_available);
//HOOK_DECLARE(0x0065EEB0, input_xinput_dispose);
//HOOK_DECLARE(0x0065EEE0, input_xinput_get_state);
//HOOK_DECLARE(0x0065EF00, input_xinput_initialize);
//HOOK_DECLARE(0x0065EF40, input_xinput_set_state);
////HOOK_DECLARE(0x0065EF60, input_xinput_update_gamepad);
//HOOK_DECLARE(0x0065F220, input_xinput_update_rumble_state);

c_static_array<debug_gamepad_data, 4> g_debug_gamepad_data = {};

bool __cdecl input_xinput_available()
{
	//return INVOKE(0x0065EE80, input_xinput_available);

	return xinput_globals.module_handle && xinput_globals.get_state && xinput_globals.set_state;
}

void __cdecl input_xinput_dispose()
{
	//INVOKE(0x0065EEB0, input_xinput_dispose);

	if (xinput_globals.module_handle)
	{
		FreeLibrary((HMODULE)xinput_globals.module_handle);
		xinput_globals.module_handle = NULL;
		xinput_globals.get_state = NULL;
		xinput_globals.set_state = NULL;
	}
}

dword __cdecl input_xinput_get_state(dword user_index, _XINPUT_STATE* state)
{
	//return INVOKE(0x0065EEE0, input_xinput_get_state, user_index, state);

	return xinput_globals.get_state(user_index, state);
}

void __cdecl input_xinput_initialize()
{
	//INVOKE(0x0065EF00, input_xinput_initialize);

	if (xinput_globals.module_handle = LoadLibraryA("xinput1_3.dll"))
	{
		xinput_globals.get_state = (decltype(xinput_globals.get_state))GetProcAddress((HMODULE)xinput_globals.module_handle, "XInputGetState");
		xinput_globals.set_state = (decltype(xinput_globals.set_state))GetProcAddress((HMODULE)xinput_globals.module_handle, "XInputSetState");
	}
}

dword __cdecl input_xinput_set_state(dword user_index, _XINPUT_VIBRATION* vibration)
{
	//return INVOKE(0x0065EF40, input_xinput_set_state, user_index, vibration);

	return xinput_globals.set_state(user_index, vibration);
}

bool __cdecl input_xinput_update_gamepad(dword gamepad_index, dword duration_ms, gamepad_state* state, debug_gamepad_data* out_debug_gamepad_data)
{
	bool result = INVOKE(0x0065EF60, input_xinput_update_gamepad, gamepad_index, duration_ms, state, out_debug_gamepad_data);
	if (result)
	{
		if (!out_debug_gamepad_data)
			out_debug_gamepad_data = &g_debug_gamepad_data[gamepad_index];

		out_debug_gamepad_data->thumb_left = state->thumb_left;
		out_debug_gamepad_data->thumb_right = state->thumb_right;
	}
	return result;
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

