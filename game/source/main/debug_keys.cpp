#include "main/debug_keys.hpp"

#include "ai/ai_debug.hpp"
#include "ai/ai_script.hpp"
#include "camera/director.hpp"
#include "editor/editor_flags.hpp"
#include "editor/editor_stubs.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "interface/terminal.hpp"
#include "items/weapons.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "main/main_time.hpp"
#include "memory/thread_local.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_debug_commands.hpp"
#include "units/units.hpp"

#include <math.h>

// Modifier Table
// 
// Index, Shift,  Control, Alt
// 0,     No,     No,      No
// 1,     Yes,    No,      No
// 2,     No,     Yes,     No
// 3,     Ignore, No,      Yes
// 4,     Yes,    Yes,     Yes
// 5,     No,     Yes,     Yes

// ai debug
bool breakpoint = false;

debug_key global_debug_key_list[]
{
	{
		.name = "Force Respawn",
		.key_code = _key_8,
		.modifier = 4,
		.function = debug_key_force_respawn,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = true,
		.variable = NULL
	},
	{
		.name = "Select This Actor",
		.key_code = _key_f1,
		.modifier = 0,
		.function = debug_key_select_this_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Select Prev Encounter",
		.key_code = _key_f2,
		.modifier = 0,
		.function = debug_key_select_prev_encounter,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Select Next Encounter",
		.key_code = _key_f3,
		.modifier = 0,
		.function = debug_key_select_next_encounter,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Select Next Actor",
		.key_code = _key_f4,
		.modifier = 0,
		.function = debug_key_select_next_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Select Prev Actor",
		.key_code = _key_f4,
		.modifier = 1,
		.function = debug_key_select_prev_actor,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Show Actor Spray",
		.key_code = _key_f5,
		.modifier = 0,
		.function = debug_key_render_spray,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Erase All Actors",
		.key_code = _key_f6,
		.modifier = 0,
		.function = debug_key_erase_all_actors,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Rotate Units",
		.key_code = _key_backslash,
		.modifier = 2,
		.function = debug_key_rotate_units,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Rotate All Units",
		.key_code = _key_right_bracket,
		.modifier = 2,
		.function = debug_key_rotate_all_units,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Ninja Rope",
		.key_code = _key_left_bracket,
		.modifier = 0,
		.function = debug_key_ninja_rope,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Breakpoint",
		.key_code = _key_b,
		.modifier = 0,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = &breakpoint
	},
	{
		.name = "Play Animation",
		.key_code = _key_k,
		.modifier = 0,
		.function = debug_key_play_animation,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Summary",
		.key_code = _key_f10,
		.modifier = 0,
		.function = debug_key_profile_summary,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL // $TODO
	},
	{
		.name = "Profile Summary Off",
		.key_code = _key_f10,
		.modifier = 2,
		.function = debug_key_profile_summary_off,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Off",
		.key_code = _key_f7,
		.modifier = 2,
		.function = debug_key_profile_off,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Next Thread",
		.key_code = _key_f7,
		.modifier = 0,
		.function = debug_key_profile_next_thread,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Prev Thread",
		.key_code = _key_f7,
		.modifier = 1,
		.function = debug_key_profile_prev_thread,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Next Attrib",
		.key_code = _key_f8,
		.modifier = 0,
		.function = debug_key_profile_next_attribute,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Prev Attrib",
		.key_code = _key_f8,
		.modifier = 1,
		.function = debug_key_profile_prev_attribute,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Next Sort",
		.key_code = _key_f9,
		.modifier = 0,
		.function = debug_key_profile_next_sort,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Prev Sort",
		.key_code = _key_f9,
		.modifier = 1,
		.function = debug_key_profile_prev_sort,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Disp Type",
		.key_code = _key_f9,
		.modifier = 2,
		.function = debug_key_profile_next_display,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Profile Dump Frame",
		.key_code = _key_f11,
		.modifier = 0,
		.function = debug_key_profile_dump_frame,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "Input debug",
		.key_code = _key_i,
		.modifier = 1,
		.function = debug_player_input_toggle,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = true,
		.variable = NULL
	},
	{
		.name = "Weapon Debug",
		.key_code = _key_w,
		.modifier = 1,
		.function = debug_key_toggle_weapons,
		.allow_out_of_game = false,
		.allow_in_editor = true,
		.toggle_variable = true,
		.variable = NULL
	},
	//{
	//	.name = "4x3 view in widescreen",
	//	.key_code = _key_f11,
	//	.modifier = 2,
	//	.function = NULL,
	//	.allow_out_of_game = true,
	//	.allow_in_editor = true,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO `bool render_debug_show_4x3_bounds`
	//},
	{
		.name = "exit game",
		.key_code = _key_escape,
		.modifier = 1,
		.function = debug_key_exit_game,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "toggle mouse focus",
		.key_code = _key_m,
		.modifier = 1,
		.function = debug_key_mouse_focus,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "clear screen",
		.key_code = _key_return,
		.modifier = 1,
		.function = debug_key_clear_screen,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = NULL
	},
	{
		.name = "deathless player",
		.key_code = _key_c,
		.modifier = 2,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = &cheat.deathless_player
	},
	{
		.name = "infinite ammo",
		.key_code = _key_c,
		.modifier = 4,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = &cheat.infinite_ammo
	},
	{
		.name = "display framerate",
		.key_code = _key_f,
		.modifier = 2,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = &display_framerate
	},
	{
		.name = "infinite framerate",
		.key_code = _key_f,
		.modifier = 4,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = &render_debug_infinite_framerate
	},
	//{
	//	.name = "render model vertex",
	//	.key_code = _key_r,
	//	.modifier = 2,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO `bool debug_render_model_vertex_counts`
	//},
	//{
	//	.name = "render model names",
	//	.key_code = _key_r,
	//	.modifier = 4,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = NULL // $TODO `bool debug_render_model_names`
	//},
	{
		.name = "save camera",
		.key_code = _key_j,
		.modifier = 2,
		.function = debug_key_save_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "load camera",
		.key_code = _key_k,
		.modifier = 2,
		.function = debug_key_load_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "teleport to Camera",
		.key_code = _key_l,
		.modifier = 2,
		.function = debug_key_teleport_to_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	//{
	//	.name = "texture cache usage",
	//	.key_code = _key_6,
	//	.modifier = 2,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "texture cache debug mip",
	//	.key_code = _key_6,
	//	.modifier = 1,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "texture cache status",
	//	.key_code = _key_7,
	//	.modifier = 2,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "texture cache graph",
	//	.key_code = _key_7,
	//	.modifier = 1,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "texture cache list",
	//	.key_code = _key_7,
	//	.modifier = 4,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "geometry cache status",
	//	.key_code = _key_8,
	//	.modifier = 2,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "geometry cache graph",
	//	.key_code = _key_8,
	//	.modifier = 1,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	//{
	//	.name = "geometry cache list",
	//	.key_code = _key_8,
	//	.modifier = 4,
	//	.function = NULL,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = NULL // $TODO
	//},
	{
		.name = "toggle pause",
		.key_code = _key_pause,
		.modifier = 0,
		.function = debug_key_toggle_pause,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "print screen",
		.key_code = _key_print_screen,
		.modifier = 0,
		.function = debug_key_print_screen,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed minor +",
		.key_code = _key_equal,
		.modifier = 0,
		.function = debug_key_increment_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed minor -",
		.key_code = _key_dash,
		.modifier = 0,
		.function = debug_key_decrement_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed major +",
		.key_code = _key_equal,
		.modifier = 1,
		.function = debug_key_increment_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed major -",
		.key_code = _key_dash,
		.modifier = 1,
		.function = debug_key_decrement_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed minor +",
		.key_code = _keypad_add,
		.modifier = 0,
		.function = debug_key_increment_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed minor -",
		.key_code = _keypad_subtract,
		.modifier = 0,
		.function = debug_key_decrement_game_speed_minor,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed major +",
		.key_code = _keypad_add,
		.modifier = 1,
		.function = debug_key_increment_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "game speed major -",
		.key_code = _keypad_subtract,
		.modifier = 1,
		.function = debug_key_decrement_game_speed_major,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "dump asserts",
		.key_code = _key_a,
		.modifier = 2,
		.function = debug_dump_assert_log,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "time stats display",
		.key_code = _key_t,
		.modifier = 2,
		.function = debug_time_stats_display,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "time stats pause",
		.key_code = _key_t,
		.modifier = 4,
		.function = debug_time_stats_pause,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "toggle controls method",
		.key_code = _key_c,
		.modifier = 5,
		.function = debug_toggle_controls_method,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = NULL,
		.key_code = _key_not_a_key,
		.modifier = 0,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	}
};

c_static_flags<ALIGN(NUMBEROF(global_debug_key_list), 4)> global_debug_key_down; // 16 bit aligned
//int32 global_debug_key_down[((NUMBEROF(global_debug_key_list) - 1) >> 5) + 1]{};
uns32 g_debug_button_down_flags;

s_debug_button g_debug_button_list[]
{
	{
		.name = "drop flag at camera",
		.gamepad_abstract_button = _gamepad_binary_button_y,
		.function = debug_button_drop_flag_at_camera,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "drop flag as bullet",
		.gamepad_abstract_button = _gamepad_binary_button_x,
		.function = debug_button_drop_flag_as_projectile,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	}
};

void __cdecl debug_keys_initialize()
{
	debug_key* key = global_debug_key_list;
	int32 key_down_count = 0;
	for (key_down_count = 0; key->name; key_down_count++)
	{
		if (key->variable)
			*key->variable = false;

		key++;
	}

	//if (!global_debug_key_down)
	//{
	//	int32 key_down_size = 4 * ((key_down_count + 31) >> 5);
	//	global_debug_key_down = (int32*)malloc(key_down_size /*, __FILE__, __LINE__ */);
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
	//	global_debug_key_down = NULL;
	//}
}

bool __cdecl debug_key_update(int32 key_index, debug_key* key, bool* modifier_down, int32 force_key_down)
{
	bool result = false;
	if ((game_in_progress() || key->allow_out_of_game) && (!game_in_editor() || key->allow_in_editor) && input_globals.mouse_acquired)
	{
		ASSERT(VALID_INDEX(key->modifier, 6)); // NUMBEROF(modifier_down) == 6
		bool key_down = input_key_frames_down(static_cast<e_input_key_code>(key->key_code), _input_type_game) && modifier_down[key->modifier];

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
						console_printf("%s = %s", key->name, *key->variable ? "OFF" : "ON");
						*key->variable = !*key->variable;
					}
				}

				if (key->function)
					key->function(false);
			}
		}
		else if (key_down)
		{
			result = true;
			global_debug_key_down.set(key_index, true);

			if (key->function)
				key->function(true);
		}
	}

	return result;
}

// name assumption
bool __cdecl debug_key_execute(const char* name, bool key_down)
{
	int32 v4 = 2; // debug_key not found

	bool modifier_down[6];
	csmemset(modifier_down, 0, NUMBEROF(modifier_down));

	debug_key* key = global_debug_key_list;
	const char* key_name = key->name;

	if (key_name)
	{
		int32 key_index = NONE;
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
		event(_event_status, "debug: evaluate debug_key '%s' executed with key_down=%s", key->name, key_down ? "true" : "false");
	}
	break;
	case 1:
	{
		event(_event_warning, "debug: evaluate debug_key '%s' DID NOT execute with key_down=%s. allow_out_of_game=%d allow_in_editor=%d toggle_variable=%d", key->name, key_down ? "true" : "false", key->allow_out_of_game, key->allow_in_editor, key->toggle_variable);
	}
	break;
	case 2:
	{
		event(_event_error, "debug: evaluate debug_key '%s' was NOT found", key->name);
	}
	break;
	}

	return v4 == 0;
}

void __cdecl debug_keys_update()
{
	bool shift_down = input_key_frames_down(_key_shift, _input_type_game);
	bool control_down = input_key_frames_down(_key_control, _input_type_game) != 0;
	bool alt_down = input_key_frames_down(_key_alt, _input_type_game) != 0;

	//random_seed_allow_use();
	bool modifier_down[6]{};

	modifier_down[0] = !shift_down   && !control_down && !alt_down;
	modifier_down[1] = shift_down    && !control_down && !alt_down;
	modifier_down[2] = !shift_down   && control_down  && !alt_down;
	modifier_down[3] = !control_down && alt_down;
	modifier_down[4] = shift_down    && control_down  && alt_down;
	modifier_down[5] = !shift_down   && control_down  && alt_down;

	debug_key* key = global_debug_key_list;
	if (key->name)
	{
		for (int32 key_index = 0; key->name; key_index++)
			debug_key_update(key_index, key++, modifier_down, 0);
	}

	// $IMPLEMENT `g_debug_button_list`

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

void __cdecl debug_key_select_this_actor(bool key_is_down)
{
	//if (key_is_down && game_in_progress())
	//	ai_debug.ai_select_this_actor = true;

	if (key_is_down)
		cheat_all_weapons();
}

void __cdecl debug_key_select_prev_encounter(bool key_is_down)
{
	//if (key_is_down && game_in_progress())
	//{
	//	ai_debug_change_selected_squad(false);
	//}

	if (key_is_down)
		cheat_all_vehicles();
}

void __cdecl debug_key_select_next_encounter(bool key_is_down)
{
	//if (key_is_down && game_in_progress())
	//{
	//	ai_debug_change_selected_squad(true);
	//}

	if (key_is_down)
		cheat_all_chars();
}

void __cdecl debug_key_select_next_actor(bool key_is_down)
{
	if (key_is_down && game_in_progress())
	{
		ai_debug_change_selected_actor(true);
	}
}

void __cdecl debug_key_select_prev_actor(bool key_is_down)
{
	if (key_is_down && game_in_progress())
	{
		ai_debug_change_selected_actor(false);
	}
}

void __cdecl debug_key_render_spray(bool key_is_down)
{
	if (key_is_down && game_in_progress())
	{
		ai_profile_change_render_spray();
	}
}

void __cdecl debug_key_erase_all_actors(bool key_is_down)
{
	if (key_is_down && game_in_progress())
		ai_scripting_erase_all();
}

void __cdecl debug_key_rotate_units(bool key_is_down)
{
	if (key_is_down && game_in_progress() && !game_is_ui_shell())
	{
		int32 active_input_user = player_mapping_first_active_input_user();
		if (active_input_user != k_number_of_users)
		{
			int32 player_index = player_mapping_get_player_by_input_user(active_input_user);
			player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);
			int32 closest_unit = units_debug_get_closest_unit(player->unit_index);
			if (closest_unit != NONE)
				player_set_unit_index(player_index, closest_unit);
		}
	}
}

void __cdecl debug_key_rotate_all_units(bool key_is_down)
{
	if (key_is_down && game_in_progress() && !game_is_ui_shell())
	{
		int32 active_input_user = player_mapping_first_active_input_user();
		if (active_input_user != k_number_of_users)
		{
			int32 player_index = player_mapping_get_player_by_input_user(active_input_user);
			player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);
			int32 unit_index = player->unit_index;
			if (unit_index != NONE)
			{
				int32 next_unit = units_debug_get_next_unit(unit_index);
				if (next_unit != NONE)
					player_set_unit_index(player_index, next_unit);
			}
		}
	}
}

void __cdecl debug_key_ninja_rope(bool key_is_down)
{
	if (key_is_down && game_in_progress())
	{
		int32 active_input_user = player_mapping_first_active_input_user();
		if (active_input_user != k_number_of_users)
		{
			int32 unit_by_input_user = player_mapping_get_unit_by_input_user(active_input_user);
			if (unit_by_input_user != NONE)
				unit_debug_ninja_rope(unit_by_input_user);
		}
	}
}

void __cdecl debug_key_play_animation(bool key_is_down)
{
}

void __cdecl debug_key_profile_summary(bool key_is_down)
{
	if (key_is_down)
		profile_summary_cycle();
}

void __cdecl debug_key_profile_summary_off(bool key_is_down)
{
	if (key_is_down)
		profile_summary_enabled = false;
}

void __cdecl debug_key_profile_off(bool key_is_down)
{
}

void __cdecl debug_key_profile_next_thread(bool key_is_down)
{
}

void __cdecl debug_key_profile_prev_thread(bool key_is_down)
{
}

void __cdecl debug_key_profile_next_attribute(bool key_is_down)
{
}

void __cdecl debug_key_profile_prev_attribute(bool key_is_down)
{
}

void __cdecl debug_key_profile_next_sort(bool key_is_down)
{
}

void __cdecl debug_key_profile_prev_sort(bool key_is_down)
{
}

void __cdecl debug_key_profile_next_display(bool key_is_down)
{
}

void __cdecl debug_key_profile_dump_frame(bool key_is_down)
{
}

bool debug_player_input;
void __cdecl player_control_debug_render_toggle()
{
	debug_player_input = !debug_player_input;
}

void __cdecl debug_player_input_toggle(bool key_is_down)
{
	if (key_is_down && game_in_progress())
		player_control_debug_render_toggle();
}

void __cdecl debug_key_toggle_weapons(bool key_is_down)
{
	if (key_is_down && game_in_progress() && !game_in_editor())
		weapons_debug_render_toggle();
}

void __cdecl debug_key_exit_game(bool key_is_down)
{
	if (key_is_down)
		main_exit_game();
}

void __cdecl debug_key_mouse_focus(bool key_is_down)
{
	if (key_is_down)
	{
		console_warning("Unimplemented: " __FUNCTION__);
	}
}

void __cdecl debug_key_clear_screen(bool key_is_down)
{
	if (key_is_down)
		terminal_clear();
}

void __cdecl debug_key_save_camera(bool key_is_down)
{
	if (key_is_down)
		director_save_camera();
}

void __cdecl debug_key_load_camera(bool key_is_down)
{
	if (key_is_down)
		director_load_camera();
}

void __cdecl debug_key_teleport_to_camera(bool key_is_down)
{
	if (key_is_down)
		cheat_teleport_to_camera();
}

void __cdecl debug_key_toggle_pause(bool key_is_down)
{
}

void __cdecl debug_key_print_screen(bool key_is_down)
{
	if (key_is_down)
	{
		c_static_string<128> screenshot_path;
		c_static_string<128> screenshot_filepath;

		for (int32 screenshot_index = 0; screenshot_index < 100; screenshot_index++)
		{
			screenshot_path.clear();
			screenshot_filepath.clear();

			const char* root = "screenshots\\";
			screenshot_path.print("%sscreenshot_%d", root, screenshot_index);
			screenshot_filepath.print("%s.jpg", screenshot_path.get_string());

			s_file_reference screenshot_file;
			file_reference_create_from_path(&screenshot_file, screenshot_filepath.get_string(), false);
			file_create_parent_directories_if_not_present(&screenshot_file);
			if (!file_exists(&screenshot_file))
			{
				break;
			}
		}

		REFERENCE_DECLARE(0x0244DEE8, bool, screenshot_taken);
		REFERENCE_DECLARE(0x0244DFC0, c_static_wchar_string<256>, screenshot_filepath_wide);

		screenshot_filepath_wide.print(L"%hs", screenshot_filepath.get_string());
		screenshot_taken = true;

		console_printf("screenshot taken '%s'", screenshot_path.get_string());
	}
}

void __cdecl debug_key_adjust_game_speed_internal(real32 delta)
{
	real32 v2 = fabsf(delta);
	real32 v3 = debug_game_speed;
	real32 v4 = (debug_game_speed + delta) / v2;

	real32 v5 = 1.0f;
	if (v4 < 0.0f)
		v5 = -1.0f;

	real32 v6 = ((v5 / 2) + v4) * v2;
	if (fmaxf(v6, 0.0f) >= 5.0f)
	{
		v6 = 5.0f;
	}
	else if (v6 <= 0.0f)
	{
		v3 = debug_game_speed;
		if (debug_game_speed > game_tick_length())
		{
			real32 v8 = game_tick_length();
			v3 = debug_game_speed;
			v6 = v8;
		}
	}

	if (v3 == 0.0f)
		v6 = fminf(game_tick_length(), v6);

	if (debug_game_speed == game_tick_length() && delta > 0.0f)
		v6 = delta;

	debug_game_speed = v6;
	console_printf("game speed %.1f", v6);
}

void __cdecl debug_key_increment_game_speed_minor(bool key_is_down)
{
	if (key_is_down && debug_game_speed < 5.0f)
		debug_key_adjust_game_speed_internal(0.1f);
}

void __cdecl debug_key_decrement_game_speed_minor(bool key_is_down)
{
	if (key_is_down && debug_game_speed > 0.0f)
		debug_key_adjust_game_speed_internal(-0.1f);
}

void __cdecl debug_key_increment_game_speed_major(bool key_is_down)
{
	if (key_is_down && debug_game_speed < 5.0f)
		debug_key_adjust_game_speed_internal(0.5f);
}

void __cdecl debug_key_decrement_game_speed_major(bool key_is_down)
{
	if (key_is_down && debug_game_speed > 0.0f)
		debug_key_adjust_game_speed_internal(-0.5f);
}

void __cdecl debug_dump_assert_log(bool key_is_down)
{
}

void __cdecl debug_time_stats_display(bool key_is_down)
{
	if (key_is_down)
	{
		//game_time_toggle_history_render();
		console_warning("Unimplemented: " __FUNCTION__);
	}
}

void __cdecl debug_time_stats_pause(bool key_is_down)
{
	if (key_is_down)
	{
		//game_time_toggle_history_gather();
		console_warning("Unimplemented: " __FUNCTION__);
	}
}

void __cdecl debug_toggle_controls_method(bool key_is_down)
{
	if (key_is_down)
	{
		static int32 controls_method = 0;
		global_preferences_set_controls_method(controls_method = (controls_method + 1) % 2);
		input_abstraction_globals.controls_method = controls_method;

		console_printf("controls method: %s", controls_method == 0 ? "keyboard" : "controller");
	}
}

void __cdecl debug_key_force_respawn(bool key_is_down)
{
	if (key_is_down)
	{
		if (!game_is_authoritative())
			return;

		static bool force_respawn = false;
		force_respawn = !force_respawn;

		c_player_in_game_iterator player_iterator;
		player_iterator.begin();
		while (player_iterator.next())
		{
			player_datum* player = player_iterator.get_datum();

			// set and evaluate `player->respawn_forced`
			if (player->respawn_forced = force_respawn)
			{
				player->respawn_timer = 0;
				game_engine_globals->player_navpoint_data[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_iterator.get_index())].__data14[0] = 0;
			}
		}

		console_printf("Force Respawn = %s", force_respawn ? "ON" : "OFF");
	}
}

void __cdecl debug_button_drop_flag_at_camera(bool button_is_down)
{
	if (button_is_down)
		editor_flag_new_at_camera();
}

void __cdecl debug_button_drop_flag_as_projectile(bool button_is_down)
{
	if (button_is_down)
		editor_flag_new_at_look_at_point();
}

