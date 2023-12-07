#include "main/main.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "camera/director.hpp"
#include "game/cheats.hpp"
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
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/gui_screens/game_browser/gui_game_browser.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_error_manager.hpp"
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
#include "networking/tools/remote_command.hpp"
#include "physics/havok_component.hpp"
#include "physics/havok_entity_iterator.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/render_structure.hpp"
#include "render/views/render_view.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "simulation/simulation.hpp"
#include "text/font_loading.hpp"
#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE(0x0189D044, bool, g_force_upload_even_if_untracked);
REFERENCE_DECLARE(0x022AE4F0, bool, g_render_thread_user_setting);
REFERENCE_DECLARE(0x022AE4F2, bool, disable_main_loop_throttle);
REFERENCE_DECLARE(0x022B456C, c_interlocked_long, g_render_thread_waiting);
REFERENCE_DECLARE(0x022B46C8, c_interlocked_long, g_render_thread_enabled);
REFERENCE_DECLARE(0x022B471C, c_interlocked_long, g_single_thread_request_flags);
REFERENCE_DECLARE(0x022B473C, bool, g_main_game_exit);

// a static byte array used in `c_network_channel::receive_packet`
// passed to `c_network_message_queue::retrieve_message`
// passed to `c_network_message_handler::handle_channel_message`
REFERENCE_DECLARE_ARRAY(0x019E8D58, byte, message_storage, 0x40000);

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x00505CCD, main_loop_body_mid);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);
HOOK_DECLARE(0x00506080, main_loop_body_single_threaded);
HOOK_DECLARE(0x00506460, main_loop_pregame_show_progress_screen);

void copy_input_states(bool enabled)
{
	if (enabled)
	{
		csmemcpy(input_abstraction_globals.input_states + 1, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		csmemcpy(input_abstraction_globals.input_states + 2, input_abstraction_globals.input_states, sizeof(s_game_input_state));
		csmemcpy(input_abstraction_globals.input_states + 3, input_abstraction_globals.input_states, sizeof(s_game_input_state));
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
	{
		.map_id = 340,
		.radius = 2.0f,
		.position = { 81.6f, -72.4f, 7.2f },
		.message = L"a player took the lake base center man cannon"
	}
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

void __cdecl main_loop_body_main_part()
{
	INVOKE(0x00505C10, main_loop_body_main_part);
}

void __cdecl main_loop_body_begin()
{
	// right control for tests
	if (input_key_frames_down(_key_code_right_control, _input_type_ui) == 1)
	{
		hs_function_table_debug;
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
		g_havok_component_data;
		c_lights_view::g_gel_bitmap_index;
		c_lights_view::g_render_light_intensity;
		c_lights_view::g_debug_clip_planes;
		c_visible_items::m_items;
		c_visible_items::m_marker_indices;
		g_render_structure_globals;
		g_gui_error_manager;
		g_user_interface_globals;
		c_cache_file_tag_resource_runtime_manager* resource_runtime_manager = g_resource_runtime_manager.get();

		s_thread_local_storage* tls = get_tls();

		TLS_DATA_GET_VALUE_REFERENCE(g_objectives);
		TLS_DATA_GET_VALUE_REFERENCE(ai_globals);

		//transport_address local_machine_address{};
		//get_local_machine_address(&local_machine_address);

		TLS_DATA_GET_VALUE_REFERENCE(player_data);
		long player_count = 0;
		{
			c_player_in_game_iterator player_iterator;
			player_iterator.begin();
			while (player_iterator.next())
			{
				player_count++;
			}
		}

		{
			c_console::write_line("players: %i", player_count);
			c_player_in_game_iterator player_iterator;
			player_iterator.begin();
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

		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		game_engine_globals->map_variant.print();
	}

	if (input_key_frames_down(_key_code_keypad_add, _input_type_ui) == 1)
	{
#if defined(ISEXPERIMENTAL)
		console_process_command("net_session_create multiplayer system_link", true);
		console_process_command("net_load_and_use_map_variant \"ed\\0.4.11.2\\maps\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_load_and_use_game_variant \"ed\\0.4.11.2\\variants\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_test_session_mode setup", true);

		//s_file_reference file_reference;
		//file_reference_create_from_path(&file_reference, "C:\\Dev\\Blam\\Halo 3 (March 8 2007)\\saved_films\\riverwo_F9EB78A3.film", false);
		//
		//static char file_buffer[0x200000]{};
		//s_blf_saved_film::s_blf_chunk_saved_film_header_delta const* saved_film_header = nullptr;
		//bool byte_swap = false;
		//
		//find_blf_chunk(&file_reference, file_buffer, &saved_film_header, &byte_swap);
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

		//main_crash("fast");
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

bool cheat_drop_has_tag = false;
long cheat_drop_tag_index = 0;
long cheat_drop_variant_name = 0;
s_model_customization_region_permutation cheat_drop_permutations[16]{};
long cheat_drop_permutation_count = 0;

// we hook `main_time_frame_rate_debug` for access to `shell_seconds_elapsed`
void __cdecl main_loop_body_mid(real shell_seconds_elapsed)
{
	long lock = tag_resources_lock_game();

	if (cheat_drop_has_tag)
		main_cheat_drop_tag_private();
	
	real elapsed_game_dt = shell_seconds_elapsed;
	if (main_time_halted())
		elapsed_game_dt = 0.0f;
	
	terminal_update(elapsed_game_dt);
	console_update(elapsed_game_dt);
	//xbox_connection_update();
	remote_command_process();
	debug_menu_update();
	//cinematic_debug_camera_control_update();
	
	tag_resources_unlock_game(lock);
}

void __cdecl main_loop_body_end()
{
	// change teams buttons
	if (game_is_ui_shell())
	{
		if (input_key_frames_down(_key_code_right_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _button_action_left_bumper);

		if (input_key_frames_down(_key_code_left_parenthesis, _input_type_ui) == 1)
			event_manager_button_pressed(_controller_index0, _button_action_right_bumper);
	}
	else
	{
		if (input_key_frames_down(_key_code_right_parenthesis, _input_type_game) == 1)
			event_manager_button_pressed(_controller_index0, _button_action_left_bumper);

		if (input_key_frames_down(_key_code_left_parenthesis, _input_type_game) == 1)
			event_manager_button_pressed(_controller_index0, _button_action_right_bumper);
	}

	//// home cluster keys
	//if (input_key_frames_down(_key_code_insert, _input_type_ui) == 1)
	//{
	//	console_process_command("load_preferences_from_file preferences.txt", true);
	//}
	//if (input_key_frames_down(_key_code_delete, _input_type_ui) == 1)
	//{
	//	static bool elite_hanger = false;
	//
	//	if (game_is_ui_shell())
	//		user_interface_start_hs_script_by_name((elite_hanger = !elite_hanger) ? "elitehangar" : "humanhangar");
	//}
	//if (input_key_frames_down(_key_code_home, _input_type_ui) == 1)
	//{
	//	//main_game_launch_set_multiplayer_splitscreen_count(2);
	//	main_game_launch_set_coop_player_count(2);
	//	main_game_launch("maps\\riverworld");
	//}
	//if (input_key_frames_down(_key_code_end, _input_type_ui) == 1)
	//{
	//	director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
	//}
	if (input_key_frames_down(_key_code_page_up, _input_type_ui) == 1)
	{
		player_control_toggle_machinima_camera_enabled();
		player_control_toggle_machinima_camera_debug();
	}
	if (input_key_frames_down(_key_code_page_down, _input_type_ui) == 1)
	{
		player_control_toggle_machinima_camera_use_old_controls();
	}
	//
	//if (input_key_frames_down(_key_code_keypad_enter, _input_type_ui) == 1)
	//{
	//}
	//if (input_key_frames_down(_key_code_keypad_decimal, _input_type_ui) == 1)
	//{
	//	load_game_browser(_controller_index0, 0, _browser_type_system_link_games);
	//}
}

void __cdecl main_loop_body_multi_threaded()
{
	//INVOKE(0x00506070, main_loop_body_multi_threaded);

	publish_waiting_gamestate();
	main_loop_body_main_part();
	publish_waiting_gamestate();
}

enum e_main_loop_body_part_flags
{
	// 1: main_pix_add_named_counter
	//  - main_thread_work_ms
	//  - main_thread_constant_overhead_ms
	//  - main_thread_raw_ms
	//  - main_thread_raw_game_ticks_ms
	//  - main_thread_raw_publish_ms
	//  - main_thread_zone_set_switch_raw_ms
	//  - main_thread_game_state_flush_raw_ms

	_main_loop_body_part_flags_count = 3
};

//void __cdecl main_loop_body_single_threaded(c_flags<e_main_loop_body_part_flags, byte, _main_loop_body_part_flags_count> parts_to_run) // debug?
void __cdecl main_loop_body_single_threaded()
{
	//INVOKE(0x00506080, main_loop_body_single_threaded);

	// Halo Online

	main_loop_body_main_part();
	
	TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	
	if (!game_is_multithreaded() || !g_main_gamestate_timing_data->flags.is_empty())
	{
		font_idle();

		if (game_is_multithreaded())
		{
			main_time_mark_publishing_start_time();
			if (restricted_region_publish_to_mirror(k_game_state_shared_region))
			{
				main_time_mark_publishing_end_time();
	
				restricted_region_unlock_primary(k_game_state_shared_region);
				if (restricted_region_lock_mirror(k_game_state_shared_region))
				{
					main_thread_lock_rasterizer_and_resources();
					process_published_game_state(true);
					main_thread_unlock_rasterizer_and_resources();
	
					if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
						restricted_region_unlock_mirror(k_game_state_shared_region);
				}
				restricted_region_lock_primary(k_game_state_shared_region);
			}
		}
		else
		{
			main_time_mark_publishing_start_time();
			main_time_mark_publishing_end_time();
	
			main_thread_lock_rasterizer_and_resources();
			process_published_game_state(!shell_application_is_paused());
			main_thread_unlock_rasterizer_and_resources();
		}
	
		g_main_gamestate_timing_data->reset();
		main_render_purge_pending_messages();
	}

	// Halo 3
	////PROFILER(single_thread_update)
	//
	////main_loop_body()
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	//
	//if (!g_main_gamestate_timing_data->flags.is_empty())
	//{
	//	font_idle();
	//	main_time_mark_publishing_start_time();
	//
	//	if (restricted_region_publish_to_mirror(k_game_state_shared_region))
	//	{
	//		//PROFILER(single_thread_render)
	//
	//		restricted_region_unlock_primary(k_game_state_shared_region);
	//		if (restricted_region_lock_mirror(k_game_state_shared_region))
	//		{
	//			main_thread_lock_rasterizer_and_resources();
	//			process_published_game_state(true);
	//			main_thread_unlock_rasterizer_and_resources();
	//
	//			if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
	//				restricted_region_unlock_mirror(k_game_state_shared_region);
	//		}
	//		restricted_region_lock_primary(k_game_state_shared_region);
	//
	//		g_main_gamestate_timing_data.reset();
	//		main_render_purge_pending_messages();
	//	}
	//}

	// Halo Reach
	//if (parts_to_run.test((e_main_loop_body_part_flags)1))
	//{
	//	//PROFILER(single_thread_update)
	//	main_loop_body_main_part();
	//}
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	//
	//if (!g_main_gamestate_timing_data->flags.is_empty())
	//{
	//	font_idle();
	//	main_time_mark_publishing_start_time();
	//
	//	if (restricted_region_publish_to_mirror(k_game_state_shared_region))
	//	{
	//		generate_event(_event_level_message, "sound:multithreading: === Mirror Published Single");
	//		//bool global_shared_mirror_success = restricted_region_publish_to_mirror_blocking(k_global_shared_data_region);
	//		//ASSERT(global_shared_mirror_success);
	//
	//		main_time_mark_publishing_end_time();
	//
	//		//PROFILER(single_thread_render)
	//
	//		render_debug_update();
	//		sound_render_deferred_wait(true);
	//		render_debug_process_deferred_events();
	//		sound_render_deferred_start(true);
	//
	//		restricted_region_unlock_primary(k_game_state_shared_region);
	//		restricted_region_unlock_primary(k_global_shared_data_region);
	//		if (restricted_region_lock_mirror(k_game_state_shared_region)/* && restricted_region_lock_mirror(k_global_shared_data_region)*/)
	//		{
	//			restricted_region_allow_mirror_aliasing(k_game_state_shared_region);
	//			main_thread_lock_rasterizer_and_resources();
	//			process_published_game_state(true);
	//			main_thread_unlock_rasterizer_and_resources();
	//
	//			if (restricted_region_mirror_locked_for_current_thread(k_global_shared_data_region))
	//				restricted_region_unlock_mirror(k_global_shared_data_region);
	//
	//			if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
	//				restricted_region_unlock_mirror(k_game_state_shared_region);
	//		}
	//		restricted_region_lock_primary(k_game_state_shared_region);
	//		restricted_region_lock_primary(k_global_shared_data_region);
	//	}
	//
	//	g_main_gamestate_timing_data.reset();
	//	main_render_purge_pending_messages();
	//}
}

REFERENCE_DECLARE(0x022B47B8, long, main_globals_main_loop_time);
REFERENCE_DECLARE(0x0244DF07, bool, byte_244DF07);
REFERENCE_DECLARE(0x0244DF08, bool, byte_244DF08);

//void main_loop_body(c_flags<e_main_loop_body_part_flags, byte, _main_loop_body_part_flags_count> parts_to_run, dword* wait_for_render_thread, __int64* vblank_index) // debug?
void main_loop_body(dword* wait_for_render_thread, dword* tick_count)
{
	dword current_tick_count = GetTickCount();
	dword tick_delta = current_tick_count - *tick_count;

	if (disable_main_loop_throttle || tick_delta >= 7)
	{
		*tick_count = current_tick_count;

		bool requested_single_thread = false;
		main_globals_main_loop_time = system_milliseconds();

		main_set_single_thread_request_flag(0, !g_render_thread_user_setting);
		if (game_is_multithreaded() && (render_thread_get_mode() == 1 || render_thread_get_mode() == 2))
		{
			main_thread_process_pending_messages();
			main_loop_body_multi_threaded();
		}
		else
		{
			requested_single_thread = true;
			main_thread_process_pending_messages();
			main_loop_body_single_threaded();
		}

		g_single_thread_request_flags.set_bit(3, byte_244DF08 || byte_244DF07);
		if (game_is_multithreaded())
		{
			if (!g_single_thread_request_flags.peek() != requested_single_thread)
			{
				if (requested_single_thread)
					unlock_resources_and_resume_render_thread(*wait_for_render_thread);
				else
					*wait_for_render_thread = _internal_halt_render_thread_and_lock_resources(__FILE__, __LINE__);
			}
		}
	}
	else
	{
		// main_thread_sleep
		sleep(7 - tick_delta);
	}
}

void __cdecl main_loop_initialize_restricted_regions()
{
	if (game_is_multithreaded())
	{
		restricted_region_unlock_primary(k_global_render_data_region);
		restricted_region_unlock_primary(k_game_state_render_region);
	}
}

void __cdecl main_loop_dispose_restricted_regions()
{
	if (game_is_multithreaded())
	{
		restricted_region_lock_primary(k_game_state_render_region);
		restricted_region_lock_primary(k_global_render_data_region);
	}
}

void __cdecl main_loop()
{
	if (game_is_multithreaded())
	{
		g_render_thread_user_setting = true;
		g_render_thread_enabled.set(true);
	}

	main_loop_enter();
	main_loop_initialize_restricted_regions();

	dword wait_for_render_thread = 0;
	dword tick_count = GetTickCount();
	while (!g_main_game_exit)
	{
		main_loop_body(&wait_for_render_thread, &tick_count);
	}

	render_thread_set_mode(1, 0);
	main_loop_dispose_restricted_regions();
	main_loop_exit();
}
HOOK_DECLARE(0x005059E0, main_loop);

dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line)
{
	return INVOKE(0x00504D20, _internal_halt_render_thread_and_lock_resources, file, line);
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

	//render_thread_set_mode(1, 0);
	//main_loop_dispose_restricted_regions();
	//
	//if (game_is_multithreaded())
	//{
	//	main_render_purge_pending_messages();
	//	wait_for_thread_to_exit(_registered_thread_render, 0xFFFFFFFF);
	//}
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
			static c_static_string<12288> status_message_ascii;
			status_message_ascii.clear();
			status_message_ascii.print("%ls", status_message.get_string());

			main_render_pregame(main_pregame_frame, status_message_ascii.get_string());

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

void __cdecl main_loop_process_global_state_changes()
{
	INVOKE(0x005065B0, main_loop_process_global_state_changes);
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

void __cdecl main_set_single_thread_request_flag(long single_threaded_request_flags, bool set)
{
	INVOKE(0x00506EB0, main_set_single_thread_request_flag, single_threaded_request_flags, set);

	//g_single_thread_request_flags.set_bit(single_threaded_request_flags, set);
}

//main_status

struct s_file_reference;
void __cdecl main_status_dump(s_file_reference* file)
{
	INVOKE(0x00507100, main_status_dump, file);
}

void __cdecl main_switch_zone_set(long zone_set_index)
{
	INVOKE(0x00507210, main_switch_zone_set, zone_set_index);
}

void __cdecl main_thread_lock_rasterizer_and_resources()
{
	//INVOKE(0x00507330, main_thread_lock_rasterizer_and_resources);

	tag_resources_lock_render();
}

void __cdecl main_thread_process_pending_messages()
{
	//INVOKE(0x00507340, main_thread_process_pending_messages);

	process_game_engine_globals_messages();
	game_sound_process_update_messages();
}

void __cdecl main_thread_unlock_rasterizer_and_resources()
{
	INVOKE(0x00507350, main_thread_unlock_rasterizer_and_resources);
}

bool __cdecl main_time_halted()
{
	//return INVOKE(0x00507370, main_time_halted);

	return shell_application_is_paused();
}

void __cdecl process_published_game_state(bool a1)
{
	INVOKE(0x00507450, process_published_game_state, a1);
}

void __cdecl publish_waiting_gamestate()
{
	INVOKE(0x005074D0, publish_waiting_gamestate);
}

//bool __cdecl render_thread_set_mode(e_render_thread_mode mode_a, e_render_thread_mode mode_b)
bool __cdecl render_thread_set_mode(long mode_compare, long mode_exchange)
{
	//return INVOKE(0x005076D0, render_thread_set_mode, mode_compare, mode_exchange);

	return game_is_multithreaded() && g_render_thread_enabled.set_if_equal(mode_exchange, mode_compare) == mode_compare;
}

long __cdecl render_thread_get_mode()
{
	if (game_is_multithreaded())
		return g_render_thread_enabled.peek();

	return 0;
}

bool __cdecl render_thread_enabled()
{
	//return INVOKE(0x00507550, render_thread_enabled);

	if (game_is_multithreaded())
		return g_render_thread_enabled.peek();

	return false;
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

		// dump last accessed tag and resource owner
		c_static_string<1024> last_accessed;
		char tag_group[8]{};
		REFERENCE_DECLARE(0x0190E460, long, last_resource_owner);

		if (g_last_tag_accessed.index != NONE)
		{
			tag_to_string(g_last_tag_accessed.group_tag, tag_group);
			last_accessed.print_line("last accessed tag:\r\n    ['%s', %04X] '%s.%s'",
				tag_group,
				g_last_tag_accessed.index,
				g_last_tag_accessed.get_name(),
				g_last_tag_accessed.get_group_name());
			file_write(&crash_info_output_file, last_accessed.length(), last_accessed.get_string());
		}

		if (last_resource_owner != NONE)
		{
			for (s_cache_file_tag_resource_data* resource : g_cache_file_globals.resource_gestalt->resources)
			{
				if (resource->runtime_data.owner_tag.index != last_resource_owner)
					continue;

				tag_to_string(resource->runtime_data.owner_tag.group_tag, tag_group);
				last_accessed.print_line("last accessed resource owner:\r\n    ['%s', %04X] '%s.%s'",
					tag_group,
					resource->runtime_data.owner_tag.index,
					resource->runtime_data.owner_tag.get_name(),
					resource->runtime_data.owner_tag.get_group_name());

				break;
			}
			file_write(&crash_info_output_file, last_accessed.length(), last_accessed.get_string());
		}

		file_close(&crash_info_output_file);
	}
}

void __cdecl main_crash_just_upload_dammit()
{
	g_catch_exceptions = true;
	g_set_never_a_debugger_present = true;
	g_force_upload_even_if_untracked = true;
	//g_suppress_keyboard_for_minidump = true;
	//g_suppress_upload_debug = false;
	//game_state_set_test_options("default");
}

#define NULL_BELONGS_TO_CHUCKY *(char const**)NULL = "chucky was here!  NULL belongs to me!!!!!"

void __cdecl main_crash(char const* type)
{
	stack_walk(0);

	ASSERT(csstricmp(type, "assert"), "asserting on command");

	if (!csstricmp(type, "now"))
	{
		NULL_BELONGS_TO_CHUCKY;
	}
	else if (!csstricmp(type, "fast"))
	{
		main_crash_just_upload_dammit();
		//g_fake_minidump_creation = true;
		NULL_BELONGS_TO_CHUCKY;
	}
	else if (!csstricmp(type, "gpu") || !csstricmp(type, "halt"))
	{
		main_halt_and_catch_fire();
	}
	else if (!csstricmp(type, "async"))
	{
		//c_synchronized_long ill_never_be_done{};
		//async_queue_simple_callback(main_crash_async, 0, 0, 6, &ill_never_be_done);
	}
	else if (!csstricmp(type, "screen"))
	{
		rasterizer_dump_display_to_bmp("crash_report\\crash_screen.bmp");
	}
	else if (!csstricmp(type, "crash_profiler_thread"))
	{
		signal_thread_to_crash(k_thread_profiler);
	}
	else if (!csstricmp(type, "assert_profiler_thread"))
	{
		signal_thread_to_assert(k_thread_profiler);
	}
	else if (!csstricmp(type, "crash_async_io_thread"))
	{
		signal_thread_to_crash(k_thread_async_io);
	}
	else if (!csstricmp(type, "assert_async_io_thread"))
	{
		signal_thread_to_assert(k_thread_async_io);
	}
	else if (!csstricmp(type, "crash_render_thread"))
	{
		signal_thread_to_crash(k_thread_render);
	}
	else if (!csstricmp(type, "assert_render_thread"))
	{
		signal_thread_to_assert(k_thread_render);
	}
	else if (!csstricmp(type, "crash_netdebug_thread"))
	{
		signal_thread_to_crash(k_thread_net_debug);
	}
	else if (!csstricmp(type, "assert_netdebug_thread"))
	{
		signal_thread_to_assert(k_thread_net_debug);
	}
	else if (!csstricmp(type, "crash_event_logs_thread"))
	{
		signal_thread_to_crash(k_thread_event_logs);
	}
	else if (!csstricmp(type, "assert_event_logs_thread"))
	{
		signal_thread_to_assert(k_thread_event_logs);
	}
	else if (!csstricmp(type, "quit"))
	{
		main_exit_game();
	}
}

void __cdecl main_cheat_drop_tag(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count)
{
	if (tag_index == NONE)
		return;

	cheat_drop_tag_index = tag_index;
	cheat_drop_variant_name = variant_name;
	cheat_drop_has_tag = true;
	cheat_drop_permutation_count = 0;

	if (permutations)
	{
		for (long i = 0; i < permutation_count; i++)
		{
			cheat_drop_permutations[i].region_name = permutations[i].region_name;
			cheat_drop_permutations[i].permutation_name = permutations[i].permutation_name;
			cheat_drop_permutation_count++;
		}
	}
}

void __cdecl main_cheat_drop_tag_private()
{
	cheat_drop_has_tag = false;

	cheat_drop_tag_in_main_event_loop(
		cheat_drop_tag_index,
		cheat_drop_variant_name,
		cheat_drop_permutations,
		cheat_drop_permutation_count);

	cheat_drop_tag_index = NONE;
	cheat_drop_variant_name = NONE;
	cheat_drop_permutation_count = 0;
}

