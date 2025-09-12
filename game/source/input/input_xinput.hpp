#pragma once

#include "cseries/cseries.hpp"

struct debug_gamepad_data
{
	point2d sticks[2];
};
static_assert(sizeof(debug_gamepad_data) == 0x8);

extern c_static_array<debug_gamepad_data, k_number_of_controllers> g_debug_gamepad_data;

struct _XINPUT_STATE;
struct _XINPUT_VIBRATION;
struct gamepad_state;
struct rumble_state;

extern real64 __cdecl input_xinput_adjust_thumb_axis_deadzone(real64 thumb_axis, real64 thumb_deadzone);
extern bool __cdecl input_xinput_available();
extern void __cdecl input_xinput_dispose();
extern uns32 __cdecl input_xinput_get_state(uns32 user_index, _XINPUT_STATE* state);
extern bool __cdecl input_xinput_initialize();
extern uns32 __cdecl input_xinput_set_state(uns32 user_index, _XINPUT_VIBRATION* state);
extern bool __cdecl input_xinput_update_gamepad(uns32 gamepad_index, uns32 elapsed_msec, gamepad_state* in_out_gamepad_state, debug_gamepad_data* out_debug_gamepad_data);
extern void __cdecl input_xinput_update_rumble_state(uns32 user_index, const rumble_state* state, bool suppressed);
extern void __cdecl input_xinput_update_thumbstick(bool left_thumb, point2d* thumbstick, int16 thumb_x, int16 thumb_y);
extern void __cdecl input_xinput_update_button(uns8* trigger_down_frames, uns16* trigger_down_msec, bool trigger_down, int32 duration_ms);
extern void __cdecl update_threshold(uns8* trigger_down_msec, bool trigger_down, uns8 duration_ms);

