#include "main/main.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "camera/director.hpp"
#include "game/game.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/game_results.hpp"
#include "game/game_state.hpp"
#include "game/player_control.hpp"
#include "hf2p/hf2p.hpp"
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
#include "multithreading/synchronization.hpp"
#include "multithreading/threads.hpp"
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
#include "text/font_loading.hpp"
#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE(0x0189D044, bool, g_force_upload_even_if_untracked);
REFERENCE_DECLARE(0x022B46C8, c_interlocked_long, g_render_thread_enabled);
REFERENCE_DECLARE(0x022B473C, bool, g_main_game_exit);

// a static byte array used in `c_network_channel::receive_packet`
// passed to `c_network_message_queue::retrieve_message`
// passed to `c_network_message_handler::handle_channel_message`
REFERENCE_DECLARE_ARRAY(0x019E8D58, byte, message_storage, 0x40000);

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);
HOOK_DECLARE(0x00506460, main_loop_pregame_show_progress_screen);

#if defined(DEDICATED_SERVER)

byte const nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
DATA_PATCH_DECLARE(0x004370F9, dedicated_server_patch, nop);	// unskip peer request player add status

byte const jump[] = { 0xEB };
//DATA_PATCH_DECLARE(0x0045B596, dedicated_server_patch, jump);	// skip can accept player join request
DATA_PATCH_DECLARE(0x0045A8BB, dedicated_server_patch, jump);
DATA_PATCH_DECLARE(0x0052D62E, dedicated_server_patch, jump);
DATA_PATCH_DECLARE(0x0052D67A, dedicated_server_patch, jump);

byte const return0[] = { 0x32, 0xC0, 0xC3 };
byte const return1[] = { 0xB0, 0x01, 0xC3 };
DATA_PATCH_DECLARE(0x0042E5D0, sub_42E5D0, return1); // disable render resources
DATA_PATCH_DECLARE(0x0042E5E0, sub_42E5E0, return1); // disable audio resources
DATA_PATCH_DECLARE(0x0042E600, game_is_dedicated_server, return1);
DATA_PATCH_DECLARE(0x0042E610, game_is_client, return0);

byte const bool_true[] = { 0x1 };
DATA_PATCH_DECLARE(0x0244F970, byte_244F970, bool_true); // enable dedicated mode
DATA_PATCH_DECLARE(0x0244F971, byte_244F971, bool_true); // disable audio

byte const _return[] = { 0xC3 };
DATA_PATCH_DECLARE(0x00504F80, dedicated_server_patch, _return); // audio_thread_loop
DATA_PATCH_DECLARE(0x005075A0, dedicated_server_patch, _return); // render_thread_loop
DATA_PATCH_DECLARE(0x0050C830, dedicated_server_patch, _return); // global_preferences_init
DATA_PATCH_DECLARE(0x00512690, dedicated_server_patch, _return); // input_update
DATA_PATCH_DECLARE(0x00551780, dedicated_server_patch, _return); // game_engine_interface_update
DATA_PATCH_DECLARE(0x005926C0, dedicated_server_patch, _return); // director_update
DATA_PATCH_DECLARE(0x005D4990, dedicated_server_patch, _return); // player_control_update
DATA_PATCH_DECLARE(0x0060D880, dedicated_server_patch, _return); // input_abstraction_update
DATA_PATCH_DECLARE(0x00613A60, dedicated_server_patch, _return); // observer_update
DATA_PATCH_DECLARE(0x00615BA0, dedicated_server_patch, _return); // rumble_update
DATA_PATCH_DECLARE(0x006173A0, dedicated_server_patch, _return); // bink_playback_update
DATA_PATCH_DECLARE(0x00619770, dedicated_server_patch, _return); // fileshare ui
DATA_PATCH_DECLARE(0x00619C00, dedicated_server_patch, _return); // spartan rank/milestone ui
DATA_PATCH_DECLARE(0x0064E190, dedicated_server_patch, _return); // fmod_initialize
DATA_PATCH_DECLARE(0x00A223F0, dedicated_server_patch, _return); // c_rasterizer::initialize_window
DATA_PATCH_DECLARE(0x00A7FE50, dedicated_server_patch, _return); // user_interface_networking_update
DATA_PATCH_DECLARE(0x00A8AAE0, dedicated_server_patch, _return); // chud_update
DATA_PATCH_DECLARE(0x00A9EDF0, dedicated_server_patch, _return); // first_person_weapons_update_camera_estimates
DATA_PATCH_DECLARE(0x00B26710, dedicated_server_patch, _return); // saber_update

#endif // DEDICATED_SERVER


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
	long map_id;
	real radius;
	real_point3d position;
	wchar_t const* message;
};

static s_location_message location_messages[] =
{
	{ .map_id = 340, .radius = 2.0f, .position = { 80.8f, -72.4f, 6.7f }, .message = L"a player took the lake base center man cannon" }
};

void show_location_messages()
{
	static real last_message_time = game_time_get_safe_in_seconds();

	if (!game_in_progress())
		return;

	long map_id = game_options_get()->map_id;

	s_location_message* found_location_message = nullptr;
	for (long i = 0; i < NUMBEROF(location_messages); i++)
	{
		s_location_message& location_message = location_messages[i];
		if (location_message.map_id != map_id)
			continue;

		if (!game_engine_player_is_nearby(&location_message.position, location_message.radius))
			continue;

		found_location_message = &location_message;
		break;
	}

	if (found_location_message == nullptr)
		return;

	if ((last_message_time + 1.0f) < game_time_get_safe_in_seconds())
	{
		chud_messaging_post(player_mapping_first_active_output_user(), found_location_message->message, _chud_message_context_self);
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

	g_main_game_exit = true;
}

void __cdecl main_halt_and_catch_fire()
{
	INVOKE(0x00505710, main_halt_and_catch_fire);

	//REFERENCE_DECLARE(0x022B47B4, long, main_globals_main_loop_pregame_entered);
	//REFERENCE_DECLARE(0x022B47F0, bool, byte_22B47F0);
	//REFERENCE_DECLARE(0x022B47F1, bool, byte_22B47F1);
	//
	//if (is_main_thread())
	//	main_globals_main_loop_pregame_entered++;
	//
	//if (byte_22B47F0)
	//{
	//	if (!byte_22B47F1)
	//	{
	//		byte_22B47F1 = true;
	//
	//		c_console::write_line("crash: recursion lock triggered!");
	//
	//		exit(-1);
	//	}
	//
	//	c_console::write_line("crash: ### CATASTROPHIC ERROR: halt_and_catch_fire: recursion lock triggered while exiting! (Someone probably smashed memory all to bits)");
	//
	//	abort();
	//}
	//
	//if (is_main_thread())
	//	main_globals_main_loop_pregame_entered++;
	//
	//release_locks_safe_for_crash_release();
	//byte_22B47F0 = true;
	//c_console::write_line("lifecycle: CRASH");
	//main_status("system_milliseconds", "time %d", system_milliseconds());
	//main_status_dump(nullptr);
	//font_initialize_emergency();
	//online_process_debug_output_queue();
	//transport_initialize();
	//input_clear_all_rumblers();
	//progress_set_default_callbacks(nullptr);
	//c_online_lsp_manager::get()->go_into_crash_mode();
	//network_webstats_force_reset();
	//user_interface_networking_set_ui_upload_quota(NONE);
	//
	//char Dst[256]{};
	//game_state_debug_server_file_uploading_enabled(Dst);
	//
	//byte_22B47F0 = false;
	//
	//if (is_main_thread())
	//	main_globals_main_loop_pregame_entered--;
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
		message_storage;
		g_font_globals;
		g_font_package_cache;
		g_copy_decompressor;
		cache_file_table_of_contents;
		cache_file_copy_globals;
		g_service_client;

		TLS_DATA_GET_VALUE_REFERENCE(g_objectives);
		TLS_DATA_GET_VALUE_REFERENCE(ai_globals);

		//transport_address local_machine_address{};
		//get_local_machine_address(&local_machine_address);

		c_lights_view::g_gel_bitmap_index;
		c_lights_view::g_render_light_intensity;
		c_lights_view::g_debug_clip_planes;
		c_visible_items::m_items;
		c_visible_items::m_markers;

		TLS_DATA_GET_VALUE_REFERENCE(player_data);
		long player_count = 0;
		{
			c_player_in_game_iterator player_iterator(player_data);
			while (player_iterator.next())
			{
				player_count++;
			}
		}

		c_console::write_line("players: %i", player_count);
		c_player_with_unit_iterator player_iterator(player_data);
		while (player_iterator.next())
		{
			long index = player_iterator.get_index();
			short absolute_index = player_iterator.get_absolute_index();
			player_datum* player = player_iterator.get_datum();

			c_console::write_line(L"    0x%08X, #%hi, %s",
				index,
				absolute_index,
				player->configuration.host.name.get_string());
		}
	}

	if (input_key_frames_down(_key_code_keypad_add, _input_type_ui) == 1)
	{
#if defined(ISEXPERIMENTAL)
		console_process_command("net_session_create multiplayer system_link", true);
		console_process_command("net_load_and_use_map_variant \"ed\\0.4.11.2\\maps\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_load_and_use_game_variant \"ed\\0.4.11.2\\variants\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_test_session_mode setup", true);

		printf("");

		//s_file_reference file_reference;
		//file_reference_create_from_path(&file_reference, "C:\\Dev\\Blam\\Halo 3 (March 8 2007)\\saved_films\\riverwo_F9EB78A3.film", false);
		//
		//static char file_buffer[0x200000]{};
		//s_blf_saved_film::s_blf_chunk_saved_film_header_delta const* saved_film_header = nullptr;
		//bool byte_swap = false;
		//
		//find_blf_chunk(&file_reference, file_buffer, &saved_film_header, &byte_swap);
		//
		//printf("");
		//
		//static s_blf_saved_film blf_saved_film;
		//static c_game_variant game_variant;
		//static c_map_variant map_variant;
		//bool is_valid = false;
		//if (!blf_saved_film.copy_to_and_validate(&game_variant, &map_variant, &is_valid) && is_valid)
		//	c_console::write_line("ui: unable to load variants from saved film file, copy_to_and_validate() failed!");
#else
		//shell_halt_with_message("FUCK");
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
}

void __cdecl main_loop_body_mid(real shell_seconds_elapsed)
{
	terminal_update(shell_seconds_elapsed);
	console_update(shell_seconds_elapsed);
}
HOOK_DECLARE_CALL(0x00505CCD, main_loop_body_mid);

void __cdecl main_loop_body_end()
{
	// change teams buttons
	if (game_is_ui_shell())
	{
		if (input_key_frames_down(_key_code_right_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_left_bumper);

		if (input_key_frames_down(_key_code_left_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_right_bumper);
	}
	else
	{
		if (input_key_frames_down(_key_code_right_parenthesis, _input_type_game) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_left_bumper);

		if (input_key_frames_down(_key_code_left_parenthesis, _input_type_game) == 1)
			event_manager_button_pressed(_controller_index0, _gamepad_button_right_bumper);
	}

	// home cluster keys
	if (input_key_frames_down(_key_code_insert, _input_type_ui) == 1)
	{
		console_process_command("load_preferences_from_file preferences.txt", true);
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

	if (input_key_frames_down(_key_code_keypad_enter, _input_type_ui) == 1)
	{
	}
	if (input_key_frames_down(_key_code_keypad_decimal, _input_type_ui) == 1)
	{
		load_game_browser(_controller_index0, 0, _browser_type_system_link_games);
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

//main_status

struct s_file_reference;
void __cdecl main_status_dump(s_file_reference* file)
{
	INVOKE(0x00507100, main_status_dump, file);
}

void __cdecl main_write_stack_to_crash_info_status_file(char const* crash_info, void* context)
{
	//INVOKE(0x0066D180, main_write_stack_to_crash_info_status_file, crash_info, context);

	s_file_reference crash_info_output_file{};
	file_reference_create_from_path(&crash_info_output_file, "crash_report\\crash_info.txt", false);
	file_create_parent_directories_if_not_present(&crash_info_output_file);

	if (file_exists(&crash_info_output_file))
		file_delete(&crash_info_output_file);

	dword error = 0;
	if (file_create(&crash_info_output_file) && file_open(&crash_info_output_file, FLAG(_file_open_flag_desired_access_write), &error))
	{
		char const* _string = "stack:\r\n";
		file_write(&crash_info_output_file, strlen(_string), _string);

		if (context)
			stack_walk_with_context(&crash_info_output_file, 1, static_cast<CONTEXT*>(context));
		else
			stack_walk_to_file(&crash_info_output_file, 3);

		if (crash_info)
			file_write(&crash_info_output_file, strlen(crash_info), crash_info);

		main_status("system_milliseconds", "time %d", system_milliseconds());
		main_status_dump(&crash_info_output_file);

		file_close(&crash_info_output_file);
	}
}

