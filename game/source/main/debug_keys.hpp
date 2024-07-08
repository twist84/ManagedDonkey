#pragma once

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"

struct debug_key
{
	const char* name;
	//c_enum<e_key_code, short, _key_code_escape, k_total_key_code_count> key_code;
	short key_code;
	short modifier;
	void(__cdecl* function)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* variable;
};
static_assert(sizeof(debug_key) == 0x14);

struct s_debug_button
{
	const char* name;
	//c_enum<e_controller_button, short, _controller_button_left_trigger, k_controller_button_count> button;
	short button;
	void(__cdecl* function)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* variable;
};
static_assert(sizeof(s_debug_button) == 0x14);

extern debug_key global_debug_key_list[];
//extern c_static_flags<ALIGN(NUMBEROF(global_debug_key_list), 4), NUMBEROF(global_debug_key_list)> global_debug_key_down;
extern dword_flags g_debug_button_down_flags;
extern s_debug_button g_debug_button_list[];

extern void __cdecl debug_keys_initialize();
extern void __cdecl debug_keys_dispose();
extern void __cdecl debug_keys_update();

extern void __cdecl debug_key_select_this_actor(bool enabled);
extern void __cdecl debug_key_select_prev_encounter(bool enabled);
extern void __cdecl debug_key_select_next_encounter(bool enabled);
extern void __cdecl debug_key_select_next_actor(bool enabled);
extern void __cdecl debug_key_select_prev_actor(bool enabled);
extern void __cdecl debug_key_render_spray(bool enabled);
extern void __cdecl debug_key_erase_all_actors(bool enabled);
extern void __cdecl debug_key_rotate_units(bool enabled);
extern void __cdecl debug_key_rotate_all_units(bool enabled);
extern void __cdecl debug_key_ninja_rope(bool enabled);
extern void __cdecl debug_key_play_animation(bool enabled);
extern void __cdecl debug_key_profile_summary(bool enabled);
extern void __cdecl debug_key_profile_summary_off(bool enabled);
extern void __cdecl debug_key_profile_off(bool enabled);
extern void __cdecl debug_key_profile_next_thread(bool enabled);
extern void __cdecl debug_key_profile_prev_thread(bool enabled);
extern void __cdecl debug_key_profile_next_attribute(bool enabled);
extern void __cdecl debug_key_profile_prev_attribute(bool enabled);
extern void __cdecl debug_key_profile_next_sort(bool enabled);
extern void __cdecl debug_key_profile_prev_sort(bool enabled);
extern void __cdecl debug_key_profile_next_display(bool enabled);
extern void __cdecl debug_key_profile_dump_frame(bool enabled);
extern void __cdecl debug_player_input_toggle(bool enabled);
extern void __cdecl debug_key_toggle_weapons(bool enabled);
extern void __cdecl debug_key_exit_game(bool enabled);
extern void __cdecl debug_key_mouse_focus(bool enabled);
extern void __cdecl debug_key_clear_screen(bool enabled);
extern void __cdecl debug_key_save_camera(bool enabled);
extern void __cdecl debug_key_load_camera(bool enabled);
extern void __cdecl debug_key_teleport_to_camera(bool enabled);
extern void __cdecl debug_key_toggle_pause(bool enabled);
extern void __cdecl debug_key_print_screen(bool enabled);
extern void __cdecl debug_key_increment_game_speed_minor(bool enabled);
extern void __cdecl debug_key_decrement_game_speed_minor(bool enabled);
extern void __cdecl debug_key_increment_game_speed_major(bool enabled);
extern void __cdecl debug_key_decrement_game_speed_major(bool enabled);
extern void __cdecl debug_dump_assert_log(bool enabled);
extern void __cdecl debug_time_stats_display(bool enabled);
extern void __cdecl debug_time_stats_pause(bool enabled);
extern void __cdecl debug_key_force_respawn(bool enabled);

extern void __cdecl debug_button_drop_flag_at_camera(bool enabled);
extern void __cdecl debug_button_drop_flag_as_projectile(bool enabled);

