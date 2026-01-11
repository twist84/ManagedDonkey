#include "test/test_globals.hpp"

#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/physical_memory_map.hpp"
#include "cache/restricted_memory.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/progress.hpp"
#include "cseries/runtime_state.hpp"
#include "hf2p/hf2p.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_globals_external.hpp"
#include "input/input_windows.hpp"
#include "interface/chud/chud_draw.hpp"
#include "interface/chud/chud_messaging.hpp"
#include "interface/gui_screens/game_browser/gui_game_browser.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_text.hpp"
#include "main/console.hpp"
#include "main/global_preferences.hpp"
#include "main/levels.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "main/main_game_launch.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/logic/logic_session_tracker.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/logic/network_banhammer.hpp"
#include "networking/logic/network_leaderboard.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_memory.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/online/online_service_record.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/tools/network_blf.hpp"
#include "physics/havok.hpp"
#include "physics/havok_component.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "rasterizer/rasterizer_hue_saturation.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/render_structure.hpp"
#include "render/views/render_view.hpp"
#include "saved_games/game_state.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "simulation/simulation.hpp"
#include "text/font_loading.hpp"
#include "units/bipeds.hpp"
#include "visibility/visibility_collection.hpp"

// a static byte array used in `c_network_channel::receive_packet`
// passed to `c_network_message_queue::retrieve_message`
// passed to `c_network_message_handler::handle_channel_message`
REFERENCE_DECLARE_ARRAY(0x019E8D58, byte, message_storage, 0x40000);

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
	e_map_id map_id;
	real32 radius;
	real_point3d position;
	const wchar_t* message;
};

static s_location_message location_messages[] =
{
	{ (e_map_id)340, 2.0f, { 81.6f, -72.4f, 7.2f }, L"a player took the lake base center man cannon" }
};

void show_location_messages()
{
	static real32 last_message_time = game_time_get_safe_in_seconds();

	if (!game_in_progress())
		return;

	e_map_id map_id = game_options_get()->map_id;

	s_location_message* found_location_message = nullptr;
	for (int32 i = 0; i < NUMBEROF(location_messages); i++)
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

void __cdecl test_main_loop_body_begin()
{
	// $TODO make this better
	if (input_key_frames_down(_key_right_shift, _input_type_ui) >= 1 && input_key_frames_down(_key_right_control, _input_type_ui) >= 1)
	{
		c_player_in_game_iterator player_iterator{};
		player_iterator.begin();
		while (player_iterator.next())
		{
			int32 index = player_iterator.get_index();
			int16 absolute_index = player_iterator.get_absolute_index();
			player_datum* player = player_iterator.get_datum();

			biped_datum* biped = BIPED_GET(player->unit_index);
			biped->biped.lean = real_pin(biped->biped.lean + 0.1f, -1.0f, 1.0f);
			break;
		}
	}
	// $TODO make this better
	if (input_key_frames_down(_key_right_shift, _input_type_ui) >= 1 && input_key_frames_down(_key_right_alt, _input_type_ui) >= 1)
	{
		c_player_in_game_iterator player_iterator{};
		player_iterator.begin();
		while (player_iterator.next())
		{
			int32 index = player_iterator.get_index();
			int16 absolute_index = player_iterator.get_absolute_index();
			player_datum* player = player_iterator.get_datum();

			biped_datum* biped = BIPED_GET(player->unit_index);
			biped->biped.lean = real_pin(biped->biped.lean - 0.1f, -1.0f, 1.0f);
			break;
		}
	}

	// right control for tests
	if (input_key_frames_down(_key_right_control, _input_type_ui) == 1 && input_key_frames_down(_key_right_shift, _input_type_ui) == 0 && input_key_frames_down(_key_right_alt, _input_type_ui) == 0)
	{
		async_globals;
		cache_file_copy_globals;
		cache_file_table_of_contents;
		g_actual_runtime_state_allocation;
		g_additional_raw_servers;
		g_cache_file_globals;
		g_cache_file_io_arena;
		g_chud_draw_globals;
		g_copy_decompressor;
		g_current_game_results;
		g_font_globals;
		g_font_package_cache;
		g_game_results_globals;
		g_global_preferences_internal;
		g_gui_error_manager;
		g_havok_component_data;
		g_havok_globals;
		g_hue_saturation_control_default;
		g_hue_saturation_control_exposed_to_hs;
		g_level_globals;
		g_network_banhammer_globals;
		g_network_banhammer_initialized;
		g_network_configuration;
		g_network_configuration_initialized;
		g_online_lsp_manager;
		g_online_service_record_manager;
		g_online_url;
		g_parse_text_table;
		g_rasterizer_timing_globals;
		g_render_structure_globals;
		g_restricted_region_names;
		g_restricted_regions;
		g_runtime_state_allocation;
		g_runtime_state_globals;
		g_screenshots_uploader;
		g_server_descriptions;
		g_synch_globals;
		g_user_interface_globals;
		game_engines;
		game_state_globals;
		hs_external_globals;
		k_hs_external_global_count;
		hs_function_table;
		hs_function_table_count;
		input_globals;
		loading_globals;
		main_globals;
		message_storage;
		network_bandwidth_globals;
		network_base_memory_globals;
		network_leaderboard_globals;
		network_shared_memory_globals;
		online_session_manager_globals;
		physical_memory_globals;
		progress_globals;
		rasterizer_globals;
		render_object_globals;
		session_tracker_globals;
		simulation_globals;

		c_lights_view::g_debug_clip_planes;
		c_lights_view::g_gel_bitmap_index;
		c_lights_view::g_render_light_intensity;
		c_player_view::x_current_player_view;
		c_player_view::x_global_player_views;
		c_visible_items::m_items;
		c_visible_items::m_marker_indices;

		c_cache_file_tag_resource_runtime_manager* resource_runtime_manager = g_resource_runtime_manager.get();

		c_player_in_game_iterator player_iterator;
		{
			player_iterator.begin();
			int32 player_count = 0;
			while (player_iterator.next())
				player_count++;
			c_console::write_line("players: %i", player_count);

			player_iterator.begin();
			while (player_iterator.next())
			{
				int32 index = player_iterator.get_index();
				int16 absolute_index = player_iterator.get_absolute_index();
				player_datum* player = player_iterator.get_datum();

				c_console::write_line(L"    0x%08X, #%hi, %s",
					index,
					absolute_index,
					player->configuration.host.name);
			}
		}

		game_engine_globals->runtime_map_variant.print();
	}

	if (input_key_frames_down(_keypad_add, _input_type_ui) == 1)
	{
#if defined(ISEXPERIMENTAL)
		console_process_command("net_session_create multiplayer system_link", true);
		console_process_command("net_load_and_use_map_variant \"ed\\0.4.11.2\\maps\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_load_and_use_game_variant \"ed\\0.4.11.2\\variants\\Highlander - Air Ball.bin\"", true);
		console_process_command("net_test_session_mode setup", true);

		players_globals->sprint_inhibited = !players_globals->sprint_inhibited;
#else
		//shell_halt_with_message("FUCK");
#endif // ISEXPERIMENTAL

		//main_crash("fast");
	}

	if (input_key_frames_down(_keypad_divide, _input_type_ui) == 1/* || GetKeyState(VK_PAUSE) & 0x8000*/)
	{
		//{
		//	LOCAL_TAG_RESOURCE_SCOPE_LOCK();
		//
		//	//console_process_command("player_ragdoll", true);
		//	//console_process_command("player_drop_weapon", true);
		//}
	}

	copy_input_states(false);
	show_location_messages();
}

void __cdecl test_main_loop_body_end()
{
	// change teams buttons
	if (game_is_ui_shell())
	{
		if (input_key_frames_down(_key_right_bracket, _input_type_ui) == 1)
			event_manager_button_pressed(_controller0, _button_left_bumper);

		if (input_key_frames_down(_key_left_bracket, _input_type_ui) == 1)
			event_manager_button_pressed(_controller0, _button_right_bumper);
	}
	else
	{
		if (input_key_frames_down(_key_right_arrow, _input_type_game) == 1)
			event_manager_button_pressed(_controller0, _button_left_bumper);

		if (input_key_frames_down(_key_left_arrow, _input_type_game) == 1)
			event_manager_button_pressed(_controller0, _button_right_bumper);
	}

	//// home cluster keys
	//if (input_key_frames_down(_key_insert, _input_type_ui) == 1)
	//{
	//	console_process_command("load_preferences_from_file preferences.txt", true);
	//}
	//if (input_key_frames_down(_key_delete, _input_type_ui) == 1)
	//{
	//	static bool elite_hanger = false;
	//
	//	if (game_is_ui_shell())
	//		user_interface_start_hs_script_by_name((elite_hanger = !elite_hanger) ? "elitehangar" : "humanhangar");
	//}
	//if (input_key_frames_down(_key_home, _input_type_ui) == 1)
	//{
	//	//main_game_launch_set_multiplayer_splitscreen_count(2);
	//	main_game_launch_set_coop_player_count(2);
	//	main_game_launch("maps\\riverworld");
	//}
	//if (input_key_frames_down(_key_end, _input_type_ui) == 1)
	//{
	//	director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
	//}
	if (input_key_frames_down(_key_page_up, _input_type_ui) == 1)
	{
		player_control_globals->machinima_camera_enabled = !player_control_globals->machinima_camera_enabled;
		player_control_globals->machinima_camera_debug = !player_control_globals->machinima_camera_debug;
	}
	if (input_key_frames_down(_key_page_down, _input_type_ui) == 1)
	{
		player_control_globals->machinima_camera_old_controls = !player_control_globals->machinima_camera_old_controls;
	}
	//
	//if (input_key_frames_down(_keypad_enter, _input_type_ui) == 1)
	//{
	//}
	//if (input_key_frames_down(_keypad_decimal, _input_type_ui) == 1)
	//{
	//	load_game_browser(_controller0, 0, _browse_system_link);
	//}
}

