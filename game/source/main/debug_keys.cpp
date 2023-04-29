#include "main/debug_keys.hpp"

#include "cseries/console.hpp"
#include "game/game.hpp"
#include "memory/thread_local.hpp"
#include "units/units.hpp"

debug_key global_debug_key_list[]
{
	//{
	//	.name = "Select This Actor",
	//	.key_code = _key_code_f1,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_select_this_actor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Select Prev Encounter",
	//	.key_code = _key_code_f2,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_select_prev_encounter(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Select Next Encounter",
	//	.key_code = _key_code_f3,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_select_next_encounter(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Select Next Actor",
	//	.key_code = _key_code_f4,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_select_next_actor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Select Prev Actor",
	//	.key_code = _key_code_f4,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_select_prev_actor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Show Actor Spray",
	//	.key_code = _key_code_f5,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_render_spray(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Erase All Actors",
	//	.key_code = _key_code_f6,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_erase_all_actors(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
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
	//{
	//	.name = "Ninja Rope",
	//	.key_code = _key_code_left_parenthesis,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_ninja_rope(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
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
	//{
	//	.name = "Play Animation",
	//	.key_code = _key_code_k,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_play_animation(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Summary",
	//	.key_code = _key_code_f10,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_profile_summary(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr // #TODO
	//},
	//{
	//	.name = "Profile Summary Off",
	//	.key_code = _key_code_f10,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_profile_summary_off(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Off",
	//	.key_code = _key_code_f7,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_profile_off(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Next Thread",
	//	.key_code = _key_code_f7,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_profile_next_thread(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Prev Thread",
	//	.key_code = _key_code_f7,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_profile_prev_thread(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Next Attrib",
	//	.key_code = _key_code_f8,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_profile_next_attribute(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Prev Attrib",
	//	.key_code = _key_code_f8,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_profile_prev_attribute(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Next Sort",
	//	.key_code = _key_code_f9,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_profile_next_sort(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Prev Sort",
	//	.key_code = _key_code_f9,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_profile_prev_sort(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Disp Type",
	//	.key_code = _key_code_f9,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_profile_next_display(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Profile Dump Frame",
	//	.key_code = _key_code_f11,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_profile_dump_frame(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Input debug",
	//	.key_code = _key_code_i,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_player_input_toggle(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = true,
	//	.variable = nullptr
	//},
	//{
	//	.name = "Weapon Debug",
	//	.key_code = _key_code_w,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_toggle_weapons(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = true,
	//	.toggle_variable = true,
	//	.variable = nullptr
	//},
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
	//{
	//	.name = "exit game",
	//	.key_code = _key_code_escape,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_exit_game(bool)`
	//	.allow_out_of_game = true,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "toggle mouse focus",
	//	.key_code = _key_code_m,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_mouse_focus(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "clear screen",
	//	.key_code = _key_code_enter,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_clear_screen(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr
	//},
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
	//{
	//	.name = "display framerate",
	//	.key_code = _key_code_f,
	//	.modifier = 2,
	//	.callback = nullptr,
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = true,
	//	.variable = nullptr // #TODO `bool display_framerate`
	//},
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
	//{
	//	.name = "save camera",
	//	.key_code = _key_code_j,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_save_camera(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "load camera",
	//	.key_code = _key_code_k,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_load_camera(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "teleport to Camera",
	//	.key_code = _key_code_l,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_key_teleport_to_camera(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
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
	//{
	//	.name = "toggle pause",
	//	.key_code = _key_code_f15,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_toggle_pause(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "print screen",
	//	.key_code = _key_code_print_screen,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_print_screen(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed minor +",
	//	.key_code = _key_code_plus,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_increment_game_speed_minor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed minor -",
	//	.key_code = _key_code_minus,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_decrement_game_speed_minor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed major +",
	//	.key_code = _key_code_plus,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_increment_game_speed_major(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed major -",
	//	.key_code = _key_code_minus,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_decrement_game_speed_major(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed minor +",
	//	.key_code = _key_code_keypad_add,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_increment_game_speed_minor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed minor -",
	//	.key_code = _key_code_keypad_subtract,
	//	.modifier = 0,
	//	.callback = nullptr, // #TODO `void debug_key_decrement_game_speed_minor(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed major +",
	//	.key_code = _key_code_keypad_add,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_increment_game_speed_major(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "game speed major -",
	//	.key_code = _key_code_keypad_subtract,
	//	.modifier = 1,
	//	.callback = nullptr, // #TODO `void debug_key_decrement_game_speed_major(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "dump asserts",
	//	.key_code = _key_code_a,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_dump_assert_log(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "time stats display",
	//	.key_code = _key_code_t,
	//	.modifier = 2,
	//	.callback = nullptr, // #TODO `void debug_time_stats_display(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
	//{
	//	.name = "time stats pause",
	//	.key_code = _key_code_t,
	//	.modifier = 4,
	//	.callback = nullptr, // #TODO `void debug_time_stats_pause(bool)`
	//	.allow_out_of_game = false,
	//	.allow_in_editor = false,
	//	.toggle_variable = false,
	//	.variable = nullptr
	//},
};

long* global_debug_key_down;
dword_flags g_debug_button_down_flags;

s_debug_button g_debug_button_list[]
{
	{
		.name = "drop flag at camera",
		.button = _controller_button_y,
		.callback = nullptr, // #TODO` callback for `void debug_button_drop_flag_at_camera(bool)`
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	},
	{
		.name = "drop flag as bullet",
		.button = _controller_button_x,
		.callback = nullptr, // #TODO callback for `void debug_button_drop_flag_as_projectile(bool)`
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = nullptr
	}
};

void __cdecl debug_keys_initialize()
{
	long key_down_count = 0;
	for (debug_key& key : global_debug_key_list)
	{
		if (key.variable)
			*key.variable = false;
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
		//ASSERT(VALID_INDEX(key.modifier, NUMBEROF(modifier_down))); // NUMBEROF(modifier_down) == 6
		//bool key_down = frames_down && modifier_down[key.modifier];

		bool key_down = (input_key_frames_down(static_cast<e_key_code>(key.key_code), _input_type_game) == 1);

		// Extra logic

		if (key.variable)
			*key.variable = key_down;

		// Other extra logic

		if (key.variable)
		{
			bool toggled = *key.variable;

			c_console::write_line("%s = %s", key.name, toggled ? "ON" : "OFF");
			*key.variable = !toggled;
		}

		// Other other extra logic

		if (key.callback)
			key.callback(key_down);
	}

	//for (s_debug_button& button : g_debug_button_list)
	//{
	//	bool button_down = ?;
	//
	//	// Extra logic
	//
	//	if (button.callback)
	//		button.callback(button_down);
	//}
}

void __cdecl debug_key_rotate_units(bool enable)
{
	if (enable)
	{
		if (!game_in_progress() || game_is_ui_shell())
			return;

		TLS_REFERENCE(player_data);

		long active_user = players_first_active_user();
		if (active_user != NONE)
		{
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

void __cdecl debug_key_rotate_all_units(bool enable)
{
	if (enable)
	{
		if (!game_in_progress() || game_is_ui_shell())
			return;

		TLS_REFERENCE(player_data);

		long active_user = players_first_active_user();
		if (active_user != NONE)
		{
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

