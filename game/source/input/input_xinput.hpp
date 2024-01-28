#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct debug_gamepad_data
{
	int16_point2d thumb_left;
	int16_point2d thumb_right;
};
static_assert(sizeof(debug_gamepad_data) == 0x8);

struct _XINPUT_STATE;
struct _XINPUT_VIBRATION;
struct s_xinput_globals
{
	//DWORD(WINAPI* GetState)(DWORD dwUserIndex, XINPUT_STATE* pState);
	dword(__stdcall* get_state)(dword user_index, _XINPUT_STATE* state);
	//DWORD(WINAPI* SetState)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
	dword(__stdcall* set_state)(dword user_index, _XINPUT_VIBRATION* vibration);
	//HMODULE hModule;
	void* module_handle;
};
static_assert(sizeof(s_xinput_globals) == 0xC);

extern c_static_array<debug_gamepad_data, k_number_of_controllers> g_debug_gamepad_data;
extern s_xinput_globals& xinput_globals;

struct gamepad_state;
struct rumble_state;

extern bool __cdecl input_xinput_available();
extern void __cdecl input_xinput_dispose();
extern dword __cdecl input_xinput_get_state(dword user_index, _XINPUT_STATE* state);
extern void __cdecl input_xinput_initialize();
extern dword __cdecl input_xinput_set_state(dword user_index, _XINPUT_VIBRATION* state);
extern bool __cdecl input_xinput_update_gamepad(dword gamepad_index, dword a2, gamepad_state* state, debug_gamepad_data* out_debug_gamepad_data);
extern void __cdecl input_xinput_update_rumble_state(dword user_index, rumble_state const* state, bool suppressed);

