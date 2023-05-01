#include "main/main.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "camera/director.hpp"
#include "cseries/cseries_console.hpp"
#include "cseries/cseries_windows.hpp"
#include "dialogs/show_direct_connect.hpp"
#include "game/game.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/game_results.hpp"
#include "game/game_state.hpp"
#include "game/player_control.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "input/input.hpp"
#include "input/input_abstraction.hpp"
#include "interface/chud/chud_messaging.hpp"
#include "interface/gui_screens/game_browser/gui_game_browser.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_text.hpp"
#include "main/console.hpp"
#include "main/debug_keys.hpp"
#include "main/global_preferences.hpp"
#include "main/loading.hpp"
#include "main/main_game_launch.hpp"
#include "main/main_render.hpp"
#include "memory/data.hpp"
#include "memory/thread_local.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_memory.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/tools/network_blf.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/views/render_view.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "simulation/simulation.hpp"
#include "visibility/visibility_collection.hpp"
#include "xbox/xbox.hpp"
#include "xbox/xnet.hpp"

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

bool __cdecl main_events_pending()
{
	return INVOKE(0x00505530, main_events_pending);
}

void __cdecl main_exit_game()
{
	//INVOKE(0x005056D0, main_exit_game);

	static REFERENCE_DECLARE(0x022B473C, bool, main_game_exit);
	main_game_exit = true;
}

void __cdecl game_dispose_hook_for_console_dispose()
{
	game_dispose();
	console_dispose();
}
HOOK_DECLARE_CALL(0x00505BF5, game_dispose_hook_for_console_dispose);

void __cdecl main_loop_body_begin()
{
	// right control for tests
	if (input_key_frames_down(_key_code_right_control, _input_type_ui) == 1)
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
		online_session_manager_globals;
		g_cache_file_io_arena;
		g_parse_text_table;
		simulation_globals;
		g_network_configuration_initialized;
		g_network_configuration;
		network_shared_memory_globals;
		network_base_memory_globals;
		g_game_results_globals;
		g_current_game_results;
		game_state_globals;
		g_server_descriptions;
		g_additional_raw_servers;
		g_online_lsp_manager;
		g_global_preferences;
		g_screenshots_uploader;

		TLS_REFERENCE(g_objectives);
		TLS_REFERENCE(ai_globals);

		//transport_address local_machine_address{};
		//get_local_machine_address(&local_machine_address);

		c_lights_view::g_gel_bitmap_index;
		c_lights_view::g_render_light_intensity;
		c_lights_view::g_debug_clip_planes;
		c_visible_items::m_items;
		c_visible_items::m_markers;

		TLS_REFERENCE(player_data);
		long player_count = 0;
		{
			c_player_in_game_iterator player_iterator(player_data);
			while (player_iterator.next())
				player_count++;
		}

		c_console::write_line("players: %i", player_count);
		c_player_with_unit_iterator player_iterator(player_data);
		while (player_iterator.next())
		{
			long index = player_iterator.get_index();
			short absolute_index = player_iterator.get_absolute_index();
			player_datum* player = player_iterator.get_datum();

			c_console::write_line(L"    0x%08X, #%hi, %s", index, absolute_index, player->configuration.host.name.get_string());
		}

		ASSERT(g_cache_file_globals.resource_data->loaded_resources.count() <=
			g_cache_file_globals.resource_file_counts_mapping[0] +
			g_cache_file_globals.resource_file_counts_mapping[1] + 
			g_cache_file_globals.resource_file_counts_mapping[2] + 
			g_cache_file_globals.resource_file_counts_mapping[3] + 
			g_cache_file_globals.resource_file_counts_mapping[4]);

		for (cache_file_resource_instance* resource_instance : g_cache_file_globals.resource_data->loaded_resources)
		{
			// are these ever non-zero?
			if (resource_instance->file_location.__unknown18 || resource_instance->file_location.__unknown1C || resource_instance->file_location.__unknown20)
				throw; // throw if they are!

			// can "flags" be "has optional data"
			if (resource_instance->runtime_data.flags.test(_cache_file_resource_data_flags_has_optional_data))
				throw; // throw if it can

			char group_string[8]{};
			tag_to_string(resource_instance->runtime_data.owner_tag.group_tag, group_string);

			//0: scenario_structure_bsp
			//1: bitmap
			ASSERT(resource_instance->runtime_data.resource_type_index != 2);
			//3: sound
			//4: model_animation_graph
			//5: render_model
			//6: bink
			//7: scenario_structure_bsp
			c_console::write_line("resources: ['%s', %08X], type: %d", group_string, resource_instance->runtime_data.owner_tag.index, resource_instance->runtime_data.resource_type_index);
		}

		if (game_in_progress() && !game_is_ui_shell())
		{
			TLS_REFERENCE(game_engine_globals);

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
#ifndef ISEXPERIMENTAL
		s_file_reference file_reference;
		file_reference_create_from_path(&file_reference, "C:\\Dev\\Blam\\Halo 3 (March 8 2007)\\saved_films\\riverwo_F9EB78A3.film", false);

		static char file_buffer[0x200000]{};
		s_blf_saved_film::s_blf_chunk_saved_film_header_delta const* saved_film_header = nullptr;
		bool byte_swap = false;

		find_blf_chunk(&file_reference, file_buffer, &saved_film_header, &byte_swap);

		printf("");

		//static s_blf_saved_film blf_saved_film;
		//static c_game_variant game_variant;
		//static c_map_variant map_variant;
		//bool is_valid = false;
		//if (!blf_saved_film.copy_to_and_validate(&game_variant, &map_variant, &is_valid) && is_valid)
		//	c_console::write_line("ui: unable to load variants from saved film file, copy_to_and_validate() failed!");
#else
		shell_halt_with_message("FUCK");
#endif // ISEXPERIMENTAL
	}

	if (input_key_frames_down(_key_code_keypad_subtract, _input_type_ui) == 1/* || GetKeyState(VK_PAUSE) & 0x8000*/)
	{
		static long controls_method = 0;
		global_preferences_set_controls_method(controls_method = (controls_method + 1) % 2);
		input_abstraction_globals.controls_method = controls_method;
	}

	copy_input_states(false);
	show_location_messages();
	show_direct_connect_dialog();
}

void __cdecl main_loop_body_end()
{
	debug_keys_update();

	// change teams buttons
	if (game_is_ui_shell())
	{
		if (input_key_frames_down(_key_code_right_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_left_bumper);

		if (input_key_frames_down(_key_code_left_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_right_bumper);
	}

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

	static long browser_type = _browser_type_system_link_games;
	static char const* browser_type_names[k_browser_type_count]
	{
		"system link games",
		"friends games",
		"xbox live games"
	};

	if (input_key_frames_down(_key_code_keypad_enter, _input_type_ui) == 1)
	{
		browser_type = (browser_type + 1) % k_browser_type_count;

		c_console::write_line("setting browser type: %s", browser_type_names[browser_type]);
	}

	if (input_key_frames_down(_key_code_keypad_decimal, _input_type_ui) == 1)
	{
		load_game_browser(_controller_index0, 0, (e_browser_type)browser_type);

		c_console::write_line("browser type: %s", browser_type_names[browser_type]);
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

bool __cdecl render_thread_enabled()
{
	return INVOKE(0x00507550, render_thread_enabled);
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

void __cdecl main_loop_enter()
{
	INVOKE(0x00506200, main_loop_enter);
}

void __cdecl game_initialize_hook_for_console_initialize()
{
	console_initialize();
	game_initialize();
}
HOOK_DECLARE_CALL(0x00506219, game_initialize_hook_for_console_initialize);

void __cdecl main_loop_exit()
{
	INVOKE(0x00506360, main_loop_exit);
}

void __cdecl main_loop_pregame()
{
	INVOKE(0x005063A0, main_loop_pregame);
}

void __cdecl main_loop_pregame_disable(bool disable)
{
	INVOKE(0x00506430, main_loop_pregame_disable, disable);
}

void __cdecl main_loop_pregame_show_progress_screen()
{
	//INVOKE(0x00506460, main_loop_pregame_show_progress_screen);

	static c_static_wchar_string<12288> status_message;
	long main_pregame_frame = main_loading_get_loading_status(&status_message);
	if (!main_pregame_frame)
	{
		//editor_show_pregame_progress(main_pregame_frame, status_message.get_string());
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
			main_render_status_message(status_message.get_string());
		}
		else
		{
			static char status_message_ascii[12288];
			status_message_ascii[0] = 0;

			wchar_string_to_ascii_string(status_message.get_string(), status_message_ascii, 12288, nullptr);
			main_render_pregame(main_pregame_frame, status_message_ascii);

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

void __cdecl main_loop_status_message(wchar_t const* status_message)
{
	//INVOKE(0x00506900, main_loop_status_message, status_message);

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	c_rasterizer::begin_frame();
	c_rasterizer::setup_targets_simple();
	main_render_status_message(status_message);
	main_time_throttle(0);
	c_rasterizer::end_frame();
}

