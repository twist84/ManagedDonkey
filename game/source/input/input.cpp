#include "input/input.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE_STATIC_ARRAY(0x01650918, char const, k_key_code_count, key_to_virtual_table);
REFERENCE_DECLARE_STATIC_ARRAY(0x01650980, short const, k_number_of_windows_input_virtual_codes, virtual_to_key_table);
REFERENCE_DECLARE_STATIC_ARRAY(0x01650B80, char const, k_key_code_count, key_to_ascii_table);
REFERENCE_DECLARE_STATIC_ARRAY(0x01650BE8, short const, k_number_of_input_ascii_codes, ascii_to_key_table);
REFERENCE_DECLARE(0x0238DBE8, s_input_globals, input_globals);

c_static_array<debug_gamepad_data, 4> g_debug_gamepad_data = {};

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
}

word __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511E30, input_mouse_msec_down, mouse_button, input_type);
}

bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type)
{
	return INVOKE(0x00511E70, input_peek_key, key, input_type);
}

// Because of the way the game handles input this function won't actually run when the game is tabbed in
// In the window proc the `WM_INPUT` message is handled with a `RawInput` function
// Raw input is great but I want to peek the damn mouse state
bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type)
{
	return INVOKE(0x00511EC0, input_peek_mouse, mouse, input_type);
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

