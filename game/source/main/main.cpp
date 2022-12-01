#include "main/main.hpp"

#include "cache/cache_files.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "camera/director.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "game/game.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/player_control.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "input/input.hpp"
#include "input/input_abstraction.hpp"
#include "interface/chud/chud_messaging.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_hs.hpp"
#include "main/global_preferences.hpp"
#include "main/loading.hpp"
#include "main/main_game_launch.hpp"
#include "main/main_render.hpp"
#include "memory/data.hpp"
#include "memory/thread_local.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/views/render_view.hpp"
#include "simulation/simulation.hpp"

#include <assert.h>

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);
HOOK_DECLARE(0x00506460, main_loop_pregame_show_progress_screen);

REFERENCE_DECLARE(0x022B46C8, c_interlocked_long, g_render_thread_enabled);

void copy_input_states(bool enabled)
{
	if (enabled)
	{
		memcpy(input_abstraction_globals.input_states + 1, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		memcpy(input_abstraction_globals.input_states + 2, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		memcpy(input_abstraction_globals.input_states + 3, input_abstraction_globals.input_states, sizeof(s_game_input_state));
	}
}

struct s_location_message
{
	real_point3d position;
	real radius;
	wchar_t const* message;
};

static s_location_message location_messages[] =
{
	// valhalla
	{ { 80.8f, -72.4f, 6.7f }, 2.0f, L"a player took the lake base center man cannon" }
};

void show_location_messages()
{
	static real last_message_time = game_time_get_safe_in_seconds();

	long location_message_index = -1;
	for (long i = 0; i < NUMBEROF(location_messages); i++)
	{
		if (!game_engine_player_is_nearby(&location_messages[i].position, location_messages[i].radius))
			continue;

		location_message_index = i;
		break;
	}

	if (location_message_index == -1)
		return;

	if ((last_message_time + 1.0f) < game_time_get_safe_in_seconds())
	{
		chud_messaging_post(player_mapping_first_active_output_user(), location_messages[location_message_index].message, _chud_message_context_self);
		last_message_time = game_time_get_safe_in_seconds();
	}
}

void __cdecl main_loop_body_begin()
{
	// right control for tests
	if (input_key_frames_down(_key_code_right_control, _input_type_ui))
	{
		hs_function_table;
		hs_external_globals;
		c_player_view::x_current_player_view;
		c_player_view::x_global_player_views;
		render_object_globals;
		g_cache_file_globals;
		g_restricted_region_names;
		g_restricted_regions;
		input_globals;

		long player_count = 0;
		{
			c_player_in_game_iterator player_iterator(get_tls()->player_data);
			while (player_iterator.next())
				player_count++;
		}

		c_console::write_line("players: %i", player_count);
		c_player_with_unit_iterator player_iterator(get_tls()->player_data);
		while (player_iterator.next())
		{
			long index = player_iterator.get_index();
			short absolute_index = player_iterator.get_absolute_index();
			player_datum* player = player_iterator.get_datum();

			c_console::write_line(L"    0x%08X, #%hi, %s", index, absolute_index, player->configuration.host.player_name.get_string());
		}

		assert(g_cache_file_globals.loaded_resources->resource_loaded_count <=
			g_cache_file_globals.resource_file_counts_mapping[0] +
			g_cache_file_globals.resource_file_counts_mapping[1] + 
			g_cache_file_globals.resource_file_counts_mapping[2] + 
			g_cache_file_globals.resource_file_counts_mapping[3] + 
			g_cache_file_globals.resource_file_counts_mapping[4]);

		if (game_in_progress() && !game_is_ui_shell())
		{
			s_game_engine_globals* game_engine_globals = get_tls()->game_engine_globals;

			long output_user_index = player_mapping_first_active_output_user();
			long unit_index = player_mapping_get_unit_by_output_user(output_user_index);
			if (unit_index != 0xFFFFFFFF)
			{
				unit_add_grenade_type_to_inventory(unit_index, _grenade_type_frag, 8);
				unit_add_grenade_type_to_inventory(unit_index, _grenade_type_plasma, 8);
				unit_add_grenade_type_to_inventory(unit_index, _grenade_type_claymore, 8);
				unit_add_grenade_type_to_inventory(unit_index, _grenade_type_firebomb, 8);
			}

			printf("");
		}
	}

	if (input_key_frames_down(_key_code_keypad_add, _input_type_ui) == 1)
	{
		shell_halt_with_message("FUCK");
	}

	if (input_key_frames_down(_key_code_keypad_subtract, _input_type_ui) == 1/* || GetKeyState(VK_PAUSE) & 0x8000*/)
	{
		static long controls_method = 0;
		global_preferences_set_controls_method(controls_method = (controls_method + 1) % 2);
		input_abstraction_globals.controls_method = controls_method;
	}

	copy_input_states(true);
	show_location_messages();
}

void __cdecl main_loop_body_end()
{
	// home cluster keys
	if (input_key_frames_down(_key_code_insert, _input_type_ui) == 1)
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
	}
	if (input_key_frames_down(_key_code_delete, _input_type_ui) == 1)
	{
		static bool elite_hanger = false;

		if (game_is_ui_shell())
			user_interface_start_hs_script_by_name((elite_hanger = !elite_hanger) ? "elitehangar" : "humanhangar");
	}
	if (input_key_frames_down(_key_code_home, _input_type_ui) == 1)
	{
		//main_game_launch_set_multiplayer_splitscreen_count(2);
		main_game_launch_set_coop_player_count(2);
		main_game_launch("maps\\riverworld");
	}
	if (input_key_frames_down(_key_code_end, _input_type_ui) == 1)
	{
		director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
	}
	if (input_key_frames_down(_key_code_page_up, _input_type_ui) == 1)
	{
		player_control_toggle_machinima_camera_enabled();
		player_control_toggle_machinima_camera_debug();
	}
	if (input_key_frames_down(_key_code_page_down, _input_type_ui) == 1)
	{
		player_control_toggle_machinima_camera_use_old_controls();
	}
}

bool game_is_multithreaded()
{
	return INVOKE(0x0042E2C0, game_is_multithreaded);
}

dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line)
{
	return INVOKE(0x00504D20, _internal_halt_render_thread_and_lock_resources, file, line);
}

void __cdecl unlock_resources_and_resume_render_thread(dword flags)
{
	INVOKE(0x00507940, unlock_resources_and_resume_render_thread, flags);

	//if (game_is_multithreaded())
	//{
	//	if (TEST_BIT(flags, 0))
	//	{
	//		c_rasterizer::rasterizer_device_release_thread();
	//		restricted_region_unlock_primary(4);
	//		restricted_region_unlock_primary(2);
	//	}
	//
	//	if (TEST_BIT(flags, 1) && game_is_multithreaded())
	//		g_render_thread_enabled.set_if_equal(true, false);
	//}
}

void __cdecl main_loop_pregame_show_progress_screen()
{
	static c_static_wchar_string<12288> loading_status;
	long main_pregame_frame = main_loading_get_loading_status(&loading_status);
	if (!main_pregame_frame)
	{
		//editor_show_pregame_progress(main_pregame_frame, loading_status.get_string());
		return;
	}

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	if (c_rasterizer::begin_frame())
	{
		c_rasterizer::setup_targets_simple();

		if (main_pregame_frame == 8)
		{
			main_render_pregame_loading_screen();
		}
		else if (main_pregame_frame == 3)
		{
			main_render_status_message(loading_status.get_string());
		}
		else
		{
			static char loading_status_ascii[12288];
			loading_status_ascii[0] = 0;

			wchar_string_to_ascii_string(loading_status.get_string(), loading_status_ascii, 12288, nullptr);
			main_render_pregame(main_pregame_frame, loading_status_ascii);

			if (main_pregame_frame == 2)
			{
				c_rasterizer::end_frame();
				return;
			}
		}

		main_time_throttle(0);
		c_rasterizer::end_frame();
	}
}

