#pragma once

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"

enum
{
	_debug_key_no_modifier = 0,
	_debug_key_shift,
	_debug_key_ctrl,
	_debug_key_shift_ctrl,

	k_debug_key_modifier_count,
};

struct debug_key
{
	const char* name;
	int16 key_code;
	int16 modifier;
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
	int16 gamepad_abstract_button;
	void(__cdecl* function)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* variable;
};
static_assert(sizeof(s_debug_button) == 0x14);

extern debug_key global_debug_key_list[];
//extern c_static_flags<ALIGN(NUMBEROF(global_debug_key_list), 4), NUMBEROF(global_debug_key_list)> global_debug_key_down;
extern uns32 g_debug_button_down_flags;
extern s_debug_button g_debug_button_list[];

extern void __cdecl debug_keys_initialize();
extern void __cdecl debug_keys_dispose();
extern void __cdecl debug_keys_update();

extern void __cdecl debug_key_select_this_actor(bool key_is_down);
extern void __cdecl debug_key_select_prev_encounter(bool key_is_down);
extern void __cdecl debug_key_select_next_encounter(bool key_is_down);
extern void __cdecl debug_key_select_next_actor(bool key_is_down);
extern void __cdecl debug_key_select_prev_actor(bool key_is_down);
extern void __cdecl debug_key_render_spray(bool key_is_down);
extern void __cdecl debug_key_erase_all_actors(bool key_is_down);
extern void __cdecl debug_key_rotate_units(bool key_is_down);
extern void __cdecl debug_key_rotate_all_units(bool key_is_down);
extern void __cdecl debug_key_ninja_rope(bool key_is_down);
extern void __cdecl debug_key_play_animation(bool key_is_down);
extern void __cdecl debug_key_profile_summary(bool key_is_down);
extern void __cdecl debug_key_profile_summary_off(bool key_is_down);
extern void __cdecl debug_key_profile_off(bool key_is_down);
extern void __cdecl debug_key_profile_next_thread(bool key_is_down);
extern void __cdecl debug_key_profile_prev_thread(bool key_is_down);
extern void __cdecl debug_key_profile_next_attribute(bool key_is_down);
extern void __cdecl debug_key_profile_prev_attribute(bool key_is_down);
extern void __cdecl debug_key_profile_next_sort(bool key_is_down);
extern void __cdecl debug_key_profile_prev_sort(bool key_is_down);
extern void __cdecl debug_key_profile_next_display(bool key_is_down);
extern void __cdecl debug_key_profile_dump_frame(bool key_is_down);
extern void __cdecl debug_player_input_toggle(bool key_is_down);
extern void __cdecl debug_key_toggle_weapons(bool key_is_down);
extern void __cdecl debug_key_exit_game(bool key_is_down);
extern void __cdecl debug_key_mouse_focus(bool key_is_down);
extern void __cdecl debug_key_clear_screen(bool key_is_down);
extern void __cdecl debug_key_save_camera(bool key_is_down);
extern void __cdecl debug_key_load_camera(bool key_is_down);
extern void __cdecl debug_key_teleport_to_camera(bool key_is_down);
extern void __cdecl debug_key_toggle_pause(bool key_is_down);
extern void __cdecl debug_key_print_screen(bool key_is_down);
extern void __cdecl debug_key_increment_game_speed_minor(bool key_is_down);
extern void __cdecl debug_key_decrement_game_speed_minor(bool key_is_down);
extern void __cdecl debug_key_increment_game_speed_major(bool key_is_down);
extern void __cdecl debug_key_decrement_game_speed_major(bool key_is_down);
extern void __cdecl debug_dump_assert_log(bool key_is_down);
extern void __cdecl debug_time_stats_display(bool key_is_down);
extern void __cdecl debug_time_stats_pause(bool key_is_down);
extern void __cdecl debug_toggle_controls_method(bool key_is_down);
extern void __cdecl debug_key_force_respawn(bool key_is_down);

extern void __cdecl debug_button_drop_flag_at_camera(bool button_is_down);
extern void __cdecl debug_button_drop_flag_as_projectile(bool button_is_down);

