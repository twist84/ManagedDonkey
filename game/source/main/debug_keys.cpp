#include "main/debug_keys.hpp"

#include "units/units.hpp"

debug_key global_debug_key_list[]
{
	//{ "select this actor", _key_code_f1 }, // #TODO
	//{ "select prev encounter", _key_code_f2 }, // #TODO
	//{ "select next encounter", _key_code_f3 }, // #TODO
	//{ "select next actor", _key_code_f4 }, // #TODO
	//{ "select prev actor", _key_code_f4 }, // #TODO
	//{ "show actor spray", _key_code_f5 }, // #TODO
	//{ "erase all actors", _key_code_f6 }, // #TODO
	{ "rotate units", _key_code_backslash, 0, debug_rotate_units_callback, false, true, false, nullptr },
	{ "rotate all units", _key_code_right_parenthesis, 0, debug_rotate_all_units_callback, false, true, false, nullptr },
	//{ "ninja rope", _key_code_left_parenthesis }, // #TODO
	//{ "breakpoint", _key_code_b }, // #TODO
	//{ "play animation", _key_code_k }, // #TODO
	//{ "profile summary", _key_code_f10 }, // #TODO
	//{ "profile summary off", _key_code_f10 }, // #TODO
	//{ "profile off", _key_code_f7 }, // #TODO
	//{ "profile next thread", _key_code_f7 }, // #TODO
	//{ "profile prev thread", _key_code_f7 }, // #TODO
	//{ "profile next attrib", _key_code_f8 }, // #TODO
	//{ "profile prev attrib", _key_code_f8 }, // #TODO
	//{ "profile next sort", _key_code_f9 }, // #TODO
	//{ "profile prev sort", _key_code_f9 }, // #TODO
	//{ "profile disp type", _key_code_f9 }, // #TODO
	//{ "profile dump frame", _key_code_f11 }, // #TODO
	//{ "input debug", _key_code_i }, // #TODO
	//{ "weapon debug", _key_code_w }, // #TODO
	//{ "4x3 view in widescreen", _key_code_f11 }, // #TODO
	//{ "exit game", _key_code_escape }, // #TODO
	//{ "toggle mouse focus", _key_code_m }, // #TODO
	//{ "clear screen", _key_code_enter }, // #TODO
	//{ "deathless player", _key_code_c }, // #TODO
	//{ "infinite ammo", _key_code_c }, // #TODO
	//{ "display framerate", _key_code_f }, // #TODO
	//{ "infinite framerate", _key_code_f }, // #TODO
	//{ "render model vertex", _key_code_r }, // #TODO
	//{ "render model names", _key_code_r }, // #TODO
	//{ "save camera", _key_code_j }, // #TODO
	//{ "load camera", _key_code_k }, // #TODO
	//{ "teleport to camera", _key_code_l }, // #TODO
	//{ "texture cache usage", _key_code_6 }, // #TODO
	//{ "texture cache debug mip", _key_code_6 }, // #TODO
	//{ "texture cache status", _key_code_7 }, // #TODO
	//{ "texture cache graph", _key_code_7 }, // #TODO
	//{ "texture cache list", _key_code_7 }, // #TODO
	//{ "geometry cache status", _key_code_8 }, // #TODO
	//{ "geometry cache graph", _key_code_8 }, // #TODO
	//{ "geometry cache list", _key_code_8 }, // #TODO
	//{ "toggle pause", _key_code_f15 }, // #TODO
	//{ "print screen", _key_code_print_screen }, // #TODO
	//{ "game speed minor +", _key_code_plus }, // #TODO
	//{ "game speed minor -", _key_code_minus }, // #TODO
	//{ "game speed major +", _key_code_plus }, // #TODO
	//{ "game speed major -", _key_code_minus }, // #TODO
	//{ "game speed minor +", _key_code_keypad_add }, // #TODO
	//{ "game speed minor -", _key_code_keypad_subtract }, // #TODO
	//{ "game speed major +", _key_code_keypad_add }, // #TODO
	//{ "game speed major -", _key_code_keypad_subtract }, // #TODO
	//{ "dump asserts", _key_code_a }, // #TODO
	//{ "time stats display", _key_code_t }, // #TODO
	//{ "time stats pause", _key_code_t } // #TODO
};

long* global_debug_key_down;
dword_flags g_debug_button_down_flags;

s_debug_button g_debug_button_list[]
{
	{ "drop flag at camera", _controller_button_y, nullptr, false, false, false, nullptr },
	{ "drop flag as bullet", _controller_button_x, nullptr, false, false, false, nullptr }
};

void __cdecl debug_keys_initialize()
{
	long key_down_count = 0;
	for (debug_key& key : global_debug_key_list)
	{
		if (key.key_down)
			*key.key_down = false;
		++key_down_count;
	}

	if (!global_debug_key_down)
	{
		long key_down_size = 4 * ((key_down_count + 31) >> 5);
		global_debug_key_down = (long*)malloc(key_down_size /*, __FILE__, __LINE__ */);
		ASSERT(global_debug_key_down);
		csmemset(global_debug_key_down, 0, key_down_size);
	}

	g_debug_button_down_flags = 0;
}

void __cdecl debug_keys_dispose()
{
	if (global_debug_key_down)
	{
		free(global_debug_key_down);
		global_debug_key_down = nullptr;
	}
}

void __cdecl debug_keys_update()
{
	// #TODO: create an actual implementation using `global_debug_key_list` and `g_debug_button_list`

	for (debug_key& key : global_debug_key_list)
	{
		bool key_down = (input_key_frames_down(static_cast<e_key_code>(key.key_code), _input_type_game) == 1);

		// Extra logic

		key.callback(key_down);
	}
}

