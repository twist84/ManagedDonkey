#pragma once

#include "cseries/cseries.hpp"
#include "input/input.hpp"

struct debug_key
{
	const char* name;
	//c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count> key_code;
	short key_code;
	short modifier; // NUMBEROF(modifier_down) == 6
	void(__cdecl* callback)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* key_down;
};
static_assert(sizeof(debug_key) == 0x14);

struct s_debug_button
{
	const char* name;
	//c_enum<e_controller_button, short, _controller_button_left_trigger, k_controller_button_count> button;
	short button;
	void(__cdecl* callback)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* button_down;
};
static_assert(sizeof(s_debug_button) == 0x14);

extern debug_key global_debug_key_list[];
extern long* global_debug_key_down;
extern dword_flags g_debug_button_down_flags;
extern s_debug_button g_debug_button_list[];

extern void __cdecl debug_keys_initialize();
extern void __cdecl debug_keys_dispose();
extern void __cdecl debug_keys_update();

