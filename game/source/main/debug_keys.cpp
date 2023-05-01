#include "main/debug_keys.hpp"

#include "camera/director.hpp"
#include "cseries/cseries_console.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "main/main.hpp"
#include "main/main_time.hpp"
#include "memory/thread_local.hpp"
#include "units/units.hpp"

// Modifier Table
// 
// Index, Shift,  Control, Windows
// 0,     No,     No,      No
// 1,     Yes,    No,      No
// 2,     No,     Yes,     No
// 3,     Ignore, No,      Yes
// 4,     Yes,    Yes,     Yes
// 5,     No,     Yes,     Yes

debug_key global_debug_key_list[]
{
	{
		.name = "Select This Actor",
		.key_code = _key_code_f1,
		.modifier = 0,
		.callback = debug_key_select_this_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Select Prev Encounter",
		.key_code = _key_code_f2,
		.modifier = 0,
		.callback = debug_key_select_prev_encounter,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Select Next Encounter",
		.key_code = _key_code_f3,
		.modifier = 0,
		.callback = debug_key_select_next_encounter,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Select Next Actor",
		.key_code = _key_code_f4,
		.modifier = 0,
		.callback = debug_key_select_next_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Select Prev Actor",
		.key_code = _key_code_f4,
		.modifier = 1,
		.callback = debug_key_select_prev_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Show Actor Spray",
		.key_code = _key_code_f5,
		.modifier = 0,
		.callback = debug_key_render_spray,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Erase All Actors",
		.key_code = _key_code_f6,
		.modifier = 0,
		.callback = debug_key_erase_all_actors,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Rotate Units",
		.key_code = _key_code_backslash,
		.modifier = 0,
		.callback = debug_key_rotate_units,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Rotate All Units",
		.key_code = _key_code_right_parenthesis,
		.modifier = 0,
		.callback = debug_key_rotate_all_units,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Ninja Rope",
		.key_code = _key_code_left_parenthesis,
		.modifier = 0,
		.callback = debug_key_ninja_rope,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	//{
	//	.name = "Breakpoint",
	//	.key_code = _key_code_b,
	//	.modifier = 0,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr // #TODO `bool breakpoint`
	//},
	{
		.name = "Play Animation",
		.key_code = _key_code_k,
		.modifier = 0,
		.callback = debug_key_play_animation,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Summary",
		.key_code = _key_code_f10,
		.modifier = 0,
		.callback = debug_key_profile_summary,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr // #TODO
	},
	{
		.name = "Profile Summary Off",
		.key_code = _key_code_f10,
		.modifier = 2,
		.callback = debug_key_profile_summary_off,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Off",
		.key_code = _key_code_f7,
		.modifier = 2,
		.callback = debug_key_profile_off,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Next Thread",
		.key_code = _key_code_f7,
		.modifier = 0,
		.callback = debug_key_profile_next_thread,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Prev Thread",
		.key_code = _key_code_f7,
		.modifier = 1,
		.callback = debug_key_profile_prev_thread,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Next Attrib",
		.key_code = _key_code_f8,
		.modifier = 0,
		.callback = debug_key_profile_next_attribute,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Prev Attrib",
		.key_code = _key_code_f8,
		.modifier = 1,
		.callback = debug_key_profile_prev_attribute,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Next Sort",
		.key_code = _key_code_f9,
		.modifier = 0,
		.callback = debug_key_profile_next_sort,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Prev Sort",
		.key_code = _key_code_f9,
		.modifier = 1,
		.callback = debug_key_profile_prev_sort,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Disp Type",
		.key_code = _key_code_f9,
		.modifier = 2,
		.callback = debug_key_profile_next_display,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Profile Dump Frame",
		.key_code = _key_code_f11,
		.modifier = 0,
		.callback = debug_key_profile_dump_frame,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "Input debug",
		.key_code = _key_code_i,
		.modifier = 1,
		.callback = debug_player_input_toggle,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = true,
		.variable = nullptr
	},
	{
		.name = "Weapon Debug",
		.key_code = _key_code_w,
		.modifier = 1,
		.callback = debug_key_toggle_weapons,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = true,
		.variable = nullptr
	},
	//{
	//	.name = "4x3 view in widescreen",
	//	.key_code = _key_code_f11,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = true,
	//	.allow_in_editor = true,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `bool render_debug_show_4x3_bounds`
	//},
	{
		.name = "exit game",
		.key_code = _key_code_escape,
		.modifier = 1,
		.callback = debug_key_exit_game,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "toggle mouse focus",
		.key_code = _key_code_m,
		.modifier = 1,
		.callback = debug_key_mouse_focus,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "clear screen",
		.key_code = _key_code_enter,
		.modifier = 1,
		.callback = debug_key_clear_screen,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = nullptr
	},
	//{
	//	.name = "deathless player",
	//	.key_code = _key_code_c,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `cheat.deathless_player`
	//},
	//{
	//	.name = "infinite ammo",
	//	.key_code = _key_code_c,
	//	.modifier = 4,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `cheat.infinite_ammo`
	//},
	{
		.name = "display framerate",
		.key_code = _key_code_f,
		.modifier = 2,
		.callback = nullptr,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = &display_framerate
	},
	//{
	//	.name = "infinite framerate",
	//	.key_code = _key_code_f,
	//	.modifier = 4,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `bool render_debug_infinite_framerate`
	//},
	//{
	//	.name = "render model vertex",
	//	.key_code = _key_code_r,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `bool debug_render_model_vertex_counts`
	//},
	//{
	//	.name = "render model names",
	//	.key_code = _key_code_r,
	//	.modifier = 4,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr // #TODO `bool debug_render_model_names`
	//},
	{
		.name = "save camera",
		.key_code = _key_code_j,
		.modifier = 2,
		.callback = debug_key_save_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "load camera",
		.key_code = _key_code_k,
		.modifier = 2,
		.callback = debug_key_load_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "teleport to Camera",
		.key_code = _key_code_l,
		.modifier = 2,
		.callback = debug_key_teleport_to_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	//{
	//	.name = "texture cache usage",
	//	.key_code = _key_code_6,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "texture cache debug mip",
	//	.key_code = _key_code_6,
	//	.modifier = 1,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "texture cache status",
	//	.key_code = _key_code_7,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "texture cache graph",
	//	.key_code = _key_code_7,
	//	.modifier = 1,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "texture cache list",
	//	.key_code = _key_code_7,
	//	.modifier = 4,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "geometry cache status",
	//	.key_code = _key_code_8,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "geometry cache graph",
	//	.key_code = _key_code_8,
	//	.modifier = 1,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "geometry cache list",
	//	.key_code = _key_code_8,
	//	.modifier = 4,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO
	//},
	{
		.name = "toggle pause",
		.key_code = _key_code_f15,
		.modifier = 0,
		.callback = debug_key_toggle_pause,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "print screen",
		.key_code = _key_code_print_screen,
		.modifier = 0,
		.callback = debug_key_print_screen,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed minor +",
		.key_code = _key_code_plus,
		.modifier = 0,
		.callback = debug_key_increment_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed minor -",
		.key_code = _key_code_minus,
		.modifier = 0,
		.callback = debug_key_decrement_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed major +",
		.key_code = _key_code_plus,
		.modifier = 1,
		.callback = debug_key_increment_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed major -",
		.key_code = _key_code_minus,
		.modifier = 1,
		.callback = debug_key_decrement_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed minor +",
		.key_code = _key_code_keypad_add,
		.modifier = 0,
		.callback = debug_key_increment_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed minor -",
		.key_code = _key_code_keypad_subtract,
		.modifier = 0,
		.callback = debug_key_decrement_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed major +",
		.key_code = _key_code_keypad_add,
		.modifier = 1,
		.callback = debug_key_increment_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "game speed major -",
		.key_code = _key_code_keypad_subtract,
		.modifier = 1,
		.callback = debug_key_decrement_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "dump asserts",
		.key_code = _key_code_a,
		.modifier = 2,
		.callback = debug_dump_assert_log,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "time stats display",
		.key_code = _key_code_t,
		.modifier = 2,
		.callback = debug_time_stats_display,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "time stats pause",
		.key_code = _key_code_t,
		.modifier = 4,
		.callback = debug_time_stats_pause,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = nullptr,
		.key_code = k_key_code_none,
		.modifier = 0,
		.callback = nullptr,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	}
};

c_static_flags<ALIGN(NUMBEROF(global_debug_key_list), 4), NUMBEROF(global_debug_key_list)> global_debug_key_down; // 16 bit aligned
//long global_debug_key_down[((NUMBEROF(global_debug_key_list) - 1) >> 5) + 1]{};
dword_flags g_debug_button_down_flags;

s_debug_button g_debug_button_list[]
{
	{
		.name = "drop flag at camera",
		.button = _controller_button_y,
		.callback = debug_button_drop_flag_at_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "drop flag as bullet",
		.button = _controller_button_x,
		.callback = debug_button_drop_flag_as_projectile,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	}
};

void __cdecl debug_keys_initialize()
{
	debug_key* key = global_debug_key_list;
	long key_down_count = 0;
	for (key_down_count = 0; key->name; key_down_count++)
	{
		if (key->variable)
			*key->variable = false;

		key++;
	}

	//if (!global_debug_key_down)
	//{
	//	long key_down_size = 4 * ((key_down_count + 31) >> 5);
	//	global_debug_key_down = (long*)malloc(key_down_size /*, __FILE__, __LINE__ */);
	//	ASSERT(global_debug_key_down);
	//	csmemset(global_debug_key_down, 0, key_down_size);
	//}

	global_debug_key_down.clear();
	g_debug_button_down_flags = 0;
}

void __cdecl debug_keys_dispose()
{
	//if (global_debug_key_down)
	//{
	//	free(global_debug_key_down);
	//	global_debug_key_down = nullptr;
	//}
}

bool __cdecl debug_key_update(long key_index, debug_key* key, bool* modifier_down, long force_key_down)
{
	bool result = false;
	if ((game_in_progress() || key->allow_out_of_game) && (!game_in_editor() || key->allow_in_editor) && input_globals.mouse_acquired)
	{
		ASSERT(VALID_INDEX(key->modifier, 6)); // NUMBEROF(modifier_down) == 6
		bool key_down = input_key_frames_down(static_cast<e_key_code>(key->key_code), _input_type_game) && modifier_down[key->modifier];

		if (force_key_down)
			key_down = force_key_down == 1;

		if (!key->toggle_variable)
		{
			if (key->variable)
				*key->variable = key_down;
		}

		if (global_debug_key_down.test(key_index))
		{
			if (!key_down)
			{
				result = true;
				global_debug_key_down.set(key_index, false);

				if (key->toggle_variable)
				{
					if (key->variable)
					{
						c_console::write_line("%s = %s", key->name, *key->variable ? "OFF" : "ON");
						*key->variable = !*key->variable;
					}
				}

				if (key->callback)
					key->callback(false);
			}
		}
		else if (key_down)
		{
			result = true;
			global_debug_key_down.set(key_index, true);

			if (key->callback)
				key->callback(true);
		}
	}

	return result;
}

// name assumption
bool __cdecl debug_key_execute(char const* name, bool key_down)
{
	long v4 = 2; // debug_key not found

	bool modifier_down[6];
	csmemset(modifier_down, 0, NUMBEROF(modifier_down));

	debug_key* key = global_debug_key_list;
	char const* key_name = key->name;

	if (key_name)
	{
		long key_index = NONE;
		for (; csstricmp(name, key_name); key++, key_index++)
		{
			key_name = (key + 1)->name;
			if (!key_name)
				break;
		}

		if (key_name)
			v4 = !debug_key_update(key_index, key, modifier_down, (key_down ^ 1) + 1);
	}

	switch (v4)
	{
	case 0:
	{
		c_console::write_line("debug: evaluate debug_key '%s' executed with key_down=%s", key->name, key_down ? "true" : "false");
	}
	break;
	case 1:
	{
		c_console::write_line("debug: evaluate debug_key '%s' DID NOT execute with key_down=%s. allow_out_of_game=%d allow_in_editor=%d toggle_variable=%d", key->name, key_down ? "true" : "false", key->allow_out_of_game, key->allow_in_editor, key->toggle_variable);
	}
	break;
	case 2:
	{
		c_console::write_line("debug: evaluate debug_key '%s' was NOT found", key->name);
	}
	break;
	}

	return v4 == 0;
}

void __cdecl debug_keys_update()
{
	bool shift_down = input_key_frames_down(_key_code_shift, _input_type_game);
	bool control_down = input_key_frames_down(_key_code_control, _input_type_game) != 0;
	bool windows_down = input_key_frames_down(_key_code_windows, _input_type_game) != 0;

	//random_seed_allow_use();
	bool modifier_down[6]{};

	modifier_down[0] = !shift_down && !control_down && !windows_down;
	modifier_down[1] = shift_down && !control_down && !windows_down;
	modifier_down[2] = !shift_down && control_down && !windows_down;
	modifier_down[3] = !control_down && windows_down;
	modifier_down[4] = shift_down && control_down && windows_down;
	modifier_down[5] = !shift_down && control_down && windows_down;

	debug_key* key = global_debug_key_list;
	if (key->name)
	{
		for (long key_index = 0; key->name; key_index++)
			debug_key_update(key_index, key++, modifier_down, 0);
	}

	// #TODO: add implementation for `g_debug_button_list`

	//for (s_debug_button& button : g_debug_button_list)
	//{
	//	bool button_down = ?;
	//
	//	// Extra logic
	//
	//	if (button.callback)
	//		button.callback(button_down);
	//}

	//random_seed_disallow_use();
}

void __cdecl debug_key_select_this_actor(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_select_prev_encounter(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_select_next_encounter(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_select_next_actor(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_select_prev_actor(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_render_spray(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_erase_all_actors(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_rotate_units(bool enabled)
{
	if (enabled && game_in_progress() && !game_is_ui_shell())
	{
		long active_user = players_first_active_user();
		if (active_user != NONE)
		{
			TLS_REFERENCE(player_data);

			long player_index = player_index_from_user_index(active_user);
			player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);
			long unit_index = player->unit_index;
			if (unit_index != NONE)
			{
				long next_unit = units_debug_get_closest_unit(unit_index);
				if (next_unit != NONE)
					player_set_unit_index(player_index, next_unit);
			}
		}
	}
}

void __cdecl debug_key_rotate_all_units(bool enabled)
{
	if (enabled && game_in_progress() && !game_is_ui_shell())
	{
		long active_user = players_first_active_user();
		if (active_user != NONE)
		{
			TLS_REFERENCE(player_data);

			long player_index = player_index_from_user_index(active_user);
			player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);
			long unit_index = player->unit_index;
			if (unit_index != NONE)
			{
				long next_unit = units_debug_get_next_unit(unit_index);
				if (next_unit != NONE)
					player_set_unit_index(player_index, next_unit);
			}
		}
	}
}

void __cdecl debug_key_ninja_rope(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_play_animation(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_summary(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_summary_off(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_off(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_next_thread(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_prev_thread(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_next_attribute(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_prev_attribute(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_next_sort(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_prev_sort(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_next_display(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_profile_dump_frame(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_player_input_toggle(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_toggle_weapons(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_exit_game(bool enabled)
{
	if (enabled)
		main_exit_game();
}

void __cdecl debug_key_mouse_focus(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_clear_screen(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_save_camera(bool enabled)
{
	if (enabled)
		director_save_camera();
}

void __cdecl debug_key_load_camera(bool enabled)
{
	if (enabled)
		director_load_camera();
}

void __cdecl debug_key_teleport_to_camera(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_toggle_pause(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_print_screen(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_increment_game_speed_minor(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_decrement_game_speed_minor(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_increment_game_speed_major(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_key_decrement_game_speed_major(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_dump_assert_log(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_time_stats_display(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_time_stats_pause(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}
void __cdecl debug_button_drop_flag_at_camera(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

void __cdecl debug_button_drop_flag_as_projectile(bool enabled)
{
	if (enabled)
	{
		c_console::write_line(__FUNCTION__);
	}
}

