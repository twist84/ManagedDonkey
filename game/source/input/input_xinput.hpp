#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct debug_gamepad_data
{
	int16_point2d thumb_left;
	int16_point2d thumb_right;
};
static_assert(sizeof(debug_gamepad_data) == 0x8);

extern c_static_array<debug_gamepad_data, k_number_of_controllers> g_debug_gamepad_data;

struct _XINPUT_STATE;
struct _XINPUT_VIBRATION;
struct gamepad_state;
struct rumble_state;

extern double __cdecl input_xinput_adjust_thumb_axis_deadzone(double thumb_axis, double thumb_deadzone);
extern bool __cdecl input_xinput_available();
extern void __cdecl input_xinput_dispose();
extern dword __cdecl input_xinput_get_state(dword user_index, _XINPUT_STATE* state);
extern bool __cdecl input_xinput_initialize();
extern dword __cdecl input_xinput_set_state(dword user_index, _XINPUT_VIBRATION* state);
extern bool __cdecl input_xinput_update_gamepad(dword gamepad_index, dword duration_ms, gamepad_state* state, debug_gamepad_data* out_debug_gamepad_data);
extern void __cdecl input_xinput_update_rumble_state(dword user_index, rumble_state const* state, bool suppressed);
extern void __cdecl input_xinput_update_thumbstick(bool left_thumb, int16_point2d* thumbstick, short thumb_x, short thumb_y);
extern void __cdecl input_xinput_update_button(byte* trigger_down_frames, word* trigger_down_msec, bool trigger_down, long duration_ms);
extern void __cdecl input_xinput_update_trigger(byte* trigger_down_msec, bool trigger_down, byte duration_ms);

