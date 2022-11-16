#include "main/main.hpp"

#include "camera/director.hpp"
#include "cseries/console.hpp"
#include "game/game.hpp"
#include "game/player_control.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "input/input_abstraction.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_hs.hpp"
#include "main/global_preferences.hpp"
#include "main/main_game_launch.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

#include <assert.h>

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);

void copy_input_states(bool enabled)
{
	if (enabled)
	{
		memcpy(input_abstraction_globals.input_states + 1, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		memcpy(input_abstraction_globals.input_states + 2, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		memcpy(input_abstraction_globals.input_states + 3, input_abstraction_globals.input_states, sizeof(s_game_input_state));
	}
}

void __cdecl main_loop_body_begin()
{
	FUNCTION_BEGIN(false);

	// right control for tests
	if (GetKeyState(VK_RCONTROL) & 0x8000)
	{
		hs_function_table;
		hs_external_globals;

		printf("");
	}

	copy_input_states(true);

	if (GetKeyState(VK_PAUSE) & 0x8000)
	{
		static long controls_method = 0;
		global_preferences_set_controls_method(controls_method = !controls_method);
		input_abstraction_globals.controls_method = controls_method;
		Sleep(75);
	}
}

void __cdecl main_loop_body_end()
{
	FUNCTION_BEGIN(false);

	bool key_pressed = false;

	// home cluster keys
	if (GetKeyState(VK_INSERT) & 0x8000)
	{
		global_preferences_set_shadow_quality(_quality_setting_high);
		global_preferences_set_texture_resolution_quality(_quality_setting_high);
		global_preferences_set_texture_filtering_quality(_quality_setting_high);
		global_preferences_set_lighting_quality(_quality_setting_high);
		global_preferences_set_effects_quality(_quality_setting_high);
		global_preferences_set_details_quality(_quality_setting_high);
		global_preferences_set_postprocessing_quality(_quality_setting_high);
		global_preferences_set_vsync(true);
		global_preferences_set_motion_blur(false);
		global_preferences_set_antialiasing(true);
		global_preferences_set_brightness(50);
		global_preferences_set_contrast(50);

		global_preferences_set_fullscreen(true);
		sub_79BA30(1920, 1080);
		key_pressed = true;
	}
	else if (GetKeyState(VK_DELETE) & 0x8000)
	{
		static bool elite_hanger = false;

		if (game_is_ui_shell())
			user_interface_start_hs_script_by_name((elite_hanger = !elite_hanger) ? "elitehangar" : "humanhangar");

		key_pressed = true;
	}
	else if (GetKeyState(VK_HOME) & 0x8000)
	{
		//main_game_launch_set_multiplayer_splitscreen_count(1);
		main_game_launch_set_coop_player_count(2);
		main_game_launch("maps\\riverworld");
		key_pressed = true;
	}
	else if (GetKeyState(VK_END) & 0x8000)
	{
		director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
		key_pressed = true;
	}
	else if (GetKeyState(VK_PRIOR) & 0x8000)
	{
		player_control_toggle_machinima_camera_enabled();
		player_control_toggle_machinima_camera_debug();
		key_pressed = true;
	}
	else if (GetKeyState(VK_NEXT) & 0x8000)
	{
		player_control_toggle_machinima_camera_use_old_controls();
		key_pressed = true;
	}

	if (key_pressed)
		Sleep(75);
}

