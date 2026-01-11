#include "main/main.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "config/version.hpp"
#include "cseries/async.hpp"
#include "cseries/async_helpers.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/progress.hpp"
#include "cseries/upload_debug.hpp"
#include "data_mining/data_mine_management.hpp"
#include "editor/editor_stubs.hpp"
#include "game/cheats.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "hf2p/hf2p.hpp"
#include "input/input_abstraction.hpp"
#include "interface/c_controller.hpp"
#include "interface/chud/chud_messaging.hpp"
#include "interface/closed_caption.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/console.hpp"
#include "main/loading.hpp"
#include "main/main_game.hpp"
#include "main/main_game_launch.hpp"
#include "main/main_predict.hpp"
#include "main/main_render.hpp"
#include "main/main_screenshot.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/network_globals.hpp"
#include "networking/online/online.hpp"
#include "networking/online/online_achievements.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/tools/remote_command.hpp"
#include "networking/transport/transport.hpp"
#include "objects/objects.hpp"
#include "physics/collision_usage.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_debug.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "shell/shell.hpp"
#include "shell/shell_windows.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"
#include "spartan_program/spartan_program_handler.hpp"
#include "tag_files/tag_files_sync.hpp"
#include "test/test_globals.hpp"
#include "text/font_cache.hpp"
#include "text/font_loading.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189D044, bool, g_force_upload_even_if_untracked) = true;
REFERENCE_DECLARE(0x022AE4F0, bool, g_render_thread_user_setting) = false;
REFERENCE_DECLARE(0x022AE4F2, bool, disable_main_loop_throttle) = false;
REFERENCE_DECLARE_ARRAY(0x022AE4F8, s_main_status_value, g_status_values, 32);
REFERENCE_DECLARE(0x022B456C, c_interlocked_long, g_render_thread_waiting);
REFERENCE_DECLARE(0x022B46C8, c_interlocked_long, g_render_thread_enabled);
REFERENCE_DECLARE(0x022B471C, c_interlocked_long, g_single_thread_request_flags);
REFERENCE_DECLARE(0x022B4738, _main_globals, main_globals);

//REFERENCE_DECLARE(0x022B47F0, bool, x_recursion_lock);
//REFERENCE_DECLARE(0x022B47F1, bool, x_fatal_recursion_lock);

//HOOK_DECLARE(0x00504D20, _internal_halt_render_thread_and_lock_resources);
HOOK_DECLARE(0x00504F80, audio_thread_loop);
HOOK_DECLARE(0x00505530, main_events_pending);
HOOK_DECLARE(0x00505650, main_events_reset);
HOOK_DECLARE(0x005063A0, main_loop_pregame);
HOOK_DECLARE(0x00506430, main_loop_pregame_disable);
HOOK_DECLARE(0x00506460, main_loop_pregame_show_progress_screen);
HOOK_DECLARE(0x005065B0, main_loop_process_global_state_changes);
HOOK_DECLARE(0x00506A10, main_prepare_for_switch_zone_set);
HOOK_DECLARE(0x00507210, main_switch_zone_set);
HOOK_DECLARE(0x00507450, process_published_game_state);

bool g_toggle_game_scripts = false;

#define NULL_BELONGS_TO_CHUCKY *(const char**)NULL = "chucky was here!  NULL belongs to me!!!!!"

static c_synchronized_long ill_never_be_done{};

const char* const k_main_event_reason_description[k_number_of_main_reset_event_reasons]
{
	"changing the map",
	"xsync in progress"
};

bool g_fake_minidump_creation = true;
bool g_suppress_keyboard_for_minidump = false;
const char* const k_crash_info_output_filename = "crash_report\\crash_info.txt";

bool debug_console_pauses_game = true;
bool debug_no_drawing = false;

int32 cheat_drop_tag_index = 0;
int32 cheat_drop_variant_name = 0;
s_model_customization_region_permutation cheat_drop_permutations[16]{};
int32 cheat_drop_permutation_count = 0;

bool main_loop_suspended = false;

void __cdecl __tls_set_g_main_gamestate_timing_data_allocator(void* new_address)
{
	INVOKE(0x00504CE0, __tls_set_g_main_gamestate_timing_data_allocator, new_address);

	//g_main_gamestate_timing_data = (s_game_tick_time_samples*)new_address;
}

void __cdecl __tls_set_g_main_render_timing_data_allocator(void* new_address)
{
	INVOKE(0x00504D00, __tls_set_g_main_render_timing_data_allocator, new_address);

	//g_main_render_timing_data = (s_game_tick_time_samples*)new_address;
}

uns32 __cdecl _internal_halt_render_thread_and_lock_resources(const char* file, int32 line)
{
	return INVOKE(0x00504D20, _internal_halt_render_thread_and_lock_resources, file, line);

	//PROFILER(internal_halt_render_thread)
	//{
	//	if (!game_is_multithreaded() || render_thread_get_mode() == _render_thread_mode_loading_screen)
	//	{
	//		return 0;
	//	}
	//
	//	if (!thread_system_initialized())
	//	{
	//		return 0;
	//	}
	//
	//	if (!restricted_region_valid(k_game_state_render_region))
	//	{
	//		return 0;
	//	}
	//
	//	if (get_current_thread_index() == k_thread_render)
	//	{
	//		return 0;
	//	}
	//
	//	uns32 returned_token = render_thread_set_mode(_render_thread_mode_enabled, _render_thread_mode_disabled) ? FLAG(1) : 0;
	//	if (!restricted_region_locked_for_current_thread(k_game_state_render_region))
	//	{
	//		restricted_region_lock_primary(k_game_state_render_region);
	//		restricted_region_lock_primary(k_global_render_data_region);
	//
	//		c_rasterizer::rasterizer_device_acquire_thread();
	//
	//		returned_token |= FLAG(0);
	//
	//		if (!thread_has_crashed(k_thread_render))
	//		{
	//			restricted_region_unlock_primary(k_game_state_shared_region);
	//			for (int32 mirror_index = restricted_region_get_mirror_count(k_game_state_shared_region); mirror_index; mirror_index--)
	//			{
	//				if (restricted_region_lock_mirror(k_game_state_shared_region))
	//				{
	//					{
	//						TAG_RESOURCES_GAME_LOCK();
	//						process_published_game_state(false);
	//					}
	//
	//					if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
	//					{
	//						restricted_region_unlock_mirror(k_game_state_shared_region);
	//					}
	//				}
	//			}
	//			restricted_region_lock_primary(k_game_state_shared_region);
	//		}
	//	}
	//
	//	if (get_current_thread_index() == k_game_state_update_region && restricted_region_locked_for_current_thread(k_game_state_update_region))
	//	{
	//		main_thread_process_pending_messages();
	//	}
	//
	//	return returned_token;
	//}
}

uns32 __cdecl audio_thread_loop(void* blah)
{
	//return INVOKE(0x00504F80, audio_thread_loop, blah);

	if (game_is_multithreaded())
	{
		while (!current_thread_should_exit())
		{
			current_thread_update_test_functions();
			sound_render_dispatch();
		}
	}
	
	return 1;
}

//.text:00504FB0 ; 
//.text:00504FC0 ; 
//.text:00504FD0 ; 
//.text:00504FF0 ; void __cdecl console_dispose()
//.text:00505000 ; void __cdecl console_initialize()
//.text:00505010 ; 
//.text:00505040 ; 
//.text:00505070 ; 
//.text:00505080 ; 
//.text:00505090 ; 
//.text:005050C0 ; 
//.text:005050F0 ; 
//.text:00505100 ; 
//.text:00505130 ; 
//.text:00505150 ; 
//.text:00505160 ; 
//.text:00505170 ; public: bool __cdecl s_scenario_zone_activation::is_empty() const

void __cdecl main_activate_cinematic_tag_private()
{
	INVOKE(0x00505190, main_activate_cinematic_tag_private);

	//main_globals.activate_cinematic_zone_from_tag = false;
}

void __cdecl main_activate_cinematic_zone(int32 cinematic_zone_index)
{
	INVOKE(0x005051A0, main_activate_cinematic_zone, cinematic_zone_index);
}

void __cdecl main_activate_cinematic_zone_for_debugging(int32 cinematic_zone_index)
{
	INVOKE(0x00505220, main_activate_cinematic_zone_for_debugging, cinematic_zone_index);
}

void __cdecl main_activate_designer_zone(int32 designer_zone_index)
{
	INVOKE(0x005052D0, main_activate_designer_zone, designer_zone_index);
}

//.text:00505370 ; void __cdecl sub_505370() // saber function called within `c_rasterizer::cleanup_before_device_reset`

void __cdecl main_cheat_drop_tag(int32 tag_index, int32 variant_name, const s_model_customization_region_permutation* permutations, int32 permutation_count)
{
	if (tag_index != NONE)
	{
		cheat_drop_tag_index = tag_index;
		cheat_drop_variant_name = variant_name;
		main_globals.drop_cheat_tag = true;
		cheat_drop_permutation_count = 0;

		if (permutations)
		{
			for (int32 permutation_index = 0; permutation_index < permutation_count; permutation_index++)
			{
				cheat_drop_permutations[permutation_index].region_name = permutations[permutation_index].region_name;
				cheat_drop_permutations[permutation_index].permutation_name = permutations[permutation_index].permutation_name;
				cheat_drop_permutation_count++;
			}
		}
	}
}

void __cdecl main_cheat_drop_tag_private()
{
	main_globals.drop_cheat_tag = false;

	cheat_drop_tag_in_main_event_loop(
		cheat_drop_tag_index,
		cheat_drop_variant_name,
		cheat_drop_permutations,
		cheat_drop_permutation_count);

	cheat_drop_tag_index = NONE;
	cheat_drop_variant_name = NONE;
	cheat_drop_permutation_count = 0;
}

void __cdecl main_clear_global_pending_zone_activation(int32 game_state_proc_flags)
{
	INVOKE(0x00505380, main_clear_global_pending_zone_activation, game_state_proc_flags);

	//if (main_globals.prepare_to_switch_zone_set)
	//{
	//	main_globals.prepare_to_switch_zone_set = false;
	//	main_globals.prepare_to_switch_to_zone_set_index = NONE;
	//}
	//
	//if (main_globals.switch_zone_set)
	//{
	//	main_globals.switch_zone_set = false;
	//	main_globals.switch_to_zone_set_index = NONE;
	//}
	//
	//if (main_globals.modify_zone_activation)
	//{
	//	main_globals.modify_zone_activation = false;
	//	main_globals.pending_zone_activation.clear();
	//}
}

void __cdecl main_crash(const char* type)
{
	stack_walk(0);

	switch (string_hash(type))
	{
	case "assert"_hash:
	{
		VASSERT(0, "asserting on command");
	}
	break;
	case "now"_hash:
	{
		NULL_BELONGS_TO_CHUCKY;
	}
	break;
	case "fast"_hash:
	{
		main_crash_just_upload_dammit();
		g_fake_minidump_creation = true;
		NULL_BELONGS_TO_CHUCKY;
	}
	break;
	case "gpu"_hash:
	case "halt"_hash:
	{
		main_halt_and_catch_fire();
	}
	break;
	case "async"_hash:
	{
		async_queue_simple_callback(main_crash_async, NULL, 0, _async_priority_blocking_generic, &ill_never_be_done);
	}
	break;
	case "screen"_hash:
	{
		rasterizer_dump_display_to_bmp("crash_report\\crash_screen.bmp");
	}
	break;
	case "crash_profiler_thread"_hash:
	{
		signal_thread_to_crash(k_thread_profiler);
	}
	break;
	case "assert_profiler_thread"_hash:
	{
		signal_thread_to_assert(k_thread_profiler);
	}
	break;
	case "crash_async_io_thread"_hash:
	{
		signal_thread_to_crash(k_thread_async_io);
	}
	break;
	case "assert_async_io_thread"_hash:
	{
		signal_thread_to_assert(k_thread_async_io);
	}
	break;
	case "crash_render_thread"_hash:
	{
		signal_thread_to_crash(k_thread_render);
	}
	break;
	case "assert_render_thread"_hash:
	{
		signal_thread_to_assert(k_thread_render);
	}
	break;
	case "crash_netdebug_thread"_hash:
	{
		signal_thread_to_crash(k_thread_netdebug);
	}
	break;
	case "assert_netdebug_thread"_hash:
	{
		signal_thread_to_assert(k_thread_netdebug);
	}
	break;
	case "crash_event_logs_thread"_hash:
	{
		signal_thread_to_crash(k_thread_event_logs);
	}
	break;
	case "assert_event_logs_thread"_hash:
	{
		signal_thread_to_assert(k_thread_event_logs);
	}
	break;
	case "quit"_hash:
	{
		main_exit_game();
	}
	break;
	}
}

e_async_completion __cdecl main_crash_async(s_async_task* task, void* data, int32 data_size)
{
	NULL_BELONGS_TO_CHUCKY;
	return _async_completion_done;
}

void __cdecl main_crash_just_upload_dammit()
{
	g_catch_exceptions = true;
	g_set_never_a_debugger_present = true;
	g_force_upload_even_if_untracked = true;
	g_suppress_keyboard_for_minidump = true;
	g_suppress_upload_debug = false;
	game_state_set_test_options("default");
}

void __cdecl main_deactivate_cinematic_tag_private()
{
	INVOKE(0x005053F0, main_deactivate_cinematic_tag_private);

	//main_globals.deactivate_cinematic_zone_from_tag = false;
	//scenario_set_and_activate_runtime_cinematic_tag(NONE);
}

void __cdecl main_deactivate_cinematic_zone(int32 cinematic_zone_index)
{
	INVOKE(0x00505400, main_deactivate_cinematic_zone, cinematic_zone_index);
}

void __cdecl main_deactivate_designer_zone(int32 designer_zone_index)
{
	INVOKE(0x00505470, main_deactivate_designer_zone, designer_zone_index);
}

void __cdecl main_decompress_gamestate()
{
	INVOKE(0x005054F0, main_decompress_gamestate);

	//if (!main_globals.gamestate_decompression_pending)
	//{
	//	main_globals.gamestate_decompression_pending = true;
	//}
}

//.text:00505510
void __cdecl main_event_reset_internal(const char* description, e_main_reset_events_reason reason, bool* event_flag)
{
	ASSERT(VALID_INDEX(reason, k_number_of_main_reset_event_reasons));

	if (*event_flag == true)
	{
		event(_event_warning, "main:events: ignoring %s due to %s",
			description,
			k_main_event_reason_description[reason]);
		*event_flag = false;
	}
}

//.text:00505520
void __cdecl main_event_reset_internal(const char* description, e_main_reset_events_reason reason, bool volatile* event_flag)
{
	ASSERT(VALID_INDEX(reason, k_number_of_main_reset_event_reasons));

	if (*event_flag == true)
	{
		event(_event_warning, "main:events: ignoring %s due to %s",
			description,
			k_main_event_reason_description[reason]);
		*event_flag = false;
	}
}

bool __cdecl main_events_pending()
{
	//return INVOKE(0x00505530, main_events_pending);

	bool result = main_game_change_in_progress();
	if (game_in_editor())
	{
		if (main_globals.reset_zone_resources || main_globals.switch_zone_set || main_globals.reset_map)
		{
			result = true;
		}
	}
	else if (main_globals.skip_cinematic
		|| main_globals.reset_map
		|| main_globals.revert_map
		|| main_globals.deactivate_cinematic_zone_from_tag
		|| main_globals.activate_cinematic_zone_from_tag
		|| saved_film_manager_has_pending_global_state_change()
		|| main_globals.gamestate_decompression_pending
		|| game_state_compressor_lock_pending()
		|| main_globals.reset_zone_resources
		|| main_globals.prepare_to_switch_zone_set
		|| main_globals.switch_zone_set
		|| main_globals.save_map
		|| main_globals.save_map_and_exit
		|| main_globals.save_core
		|| main_globals.load_core
		|| main_globals.ui_saving_files
		|| main_globals.reload_active_zone_set
		|| main_globals.modify_zone_activation
		|| cache_file_tag_resources_prefetch_update_required()
		|| texture_cache_is_blocking()
		|| geometry_cache_is_blocking()
		|| screenshot_globals.take_screenshot2 /* is_taking_screenshot */)
	{
		result = true;
	}

	return result;
}

void __cdecl main_events_reset(e_main_reset_events_reason reason)
{
	//INVOKE(0x00505650, main_events_reset, reason);

	main_event_reset_internal("skip cinematic", reason, &main_globals.skip_cinematic);
	main_event_reset_internal("map reset", reason, &main_globals.reset_map);
	main_event_reset_internal("map revert", reason, &main_globals.revert_map);
	main_globals.revert_map_flags.clear();
	main_event_reset_internal("activate cinematic tag", reason, &main_globals.activate_cinematic_tag);
	main_event_reset_internal("game state decompression", reason, &main_globals.gamestate_decompression_pending);
	main_event_reset_internal("reset zone resources", reason, &main_globals.reset_zone_resources);
	main_event_reset_internal("prepare to switch zone set", reason, &main_globals.prepare_to_switch_zone_set);
	main_event_reset_internal("switch zone set", reason, &main_globals.switch_zone_set);
	main_event_reset_internal("save", reason, &main_globals.save_map);
	main_event_reset_internal("save and exit", reason, &main_globals.save_map_and_exit);
	main_event_reset_internal("reloading active zone set", reason, &main_globals.reload_active_zone_set);
	main_event_reset_internal("non-bsp zone activation", reason, &main_globals.modify_zone_activation);
	main_globals.pending_zone_activation.clear();
	main_event_reset_internal("cheat drop tag", reason, &main_globals.drop_cheat_tag);
}

void __cdecl main_exit_game()
{
	//INVOKE(0x005056D0, main_exit_game);

	main_globals.exit_game = true;
}

void __cdecl main_game_gamestate_decompress_and_apply_private()
{
	INVOKE(0x005056E0, main_game_gamestate_decompress_and_apply_private);

	//if (!game_state_get_compressor()->game_state_decompress_buffer(true, true, false))
	//{
	//	event(_event_error, "main_game: game_state_compressor failed to decompress and load gamestate.");
	//}
	//main_globals.gamestate_decompression_pending = false;
}

bool __cdecl main_game_is_exiting()
{
	//return INVOKE(0x00505700, main_game_is_exiting);

	return main_globals.exit_game;
}

#if defined(_DEBUG)
// crash and burn outside of debug
HOOK_DECLARE(0x00505710, main_halt_and_catch_fire);
#endif
void __cdecl main_halt_and_catch_fire()
{
	//INVOKE(0x00505710, main_halt_and_catch_fire);

	static c_static_string<24576> text{};
	static bool x_recursion_lock = false;
	static bool x_fatal_recursion_lock = false;
	static bool x_just_upload_dammit = false;

	main_loop_pregame_halt_and_catch_fire_push();
	if (x_recursion_lock)
	{
		if (!x_fatal_recursion_lock)
		{
			x_fatal_recursion_lock = true;
			event(_event_warning, "crash: recursion lock triggered!");
			exit(NONE);
		}

		event(_event_critical, "crash: ### CATASTROPHIC ERROR: halt_and_catch_fire: recursion lock triggered while exiting! (Someone probably smashed memory all to bits)");
		while (!is_debugger_present());
		abort();
	}

	main_loop_pregame_halt_and_catch_fire_push();
	release_locks_safe_for_crash_release();
	//crash_report_folder_create_if_not_present();
	bool crash_ui_enable = true;//network_configuration_is_crash_ui_enabled() || minidump_force_regular_minidump_with_ui;

	uns32 lock_time = system_milliseconds();

	bool upload_debug_started = false;
	bool upload_debug_completed = false;
	bool upload_debug_success = false;
	bool create_fake_minidump = false;

	x_recursion_lock = true;

	event(_event_warning, "lifecycle: CRASH");
	main_status("system_milliseconds", "time %d", system_milliseconds());
	main_status_dump(NULL);

	font_initialize_emergency();
	online_process_debug_output_queue();
	transport_initialize();
	input_clear_all_rumblers();
	progress_set_default_callbacks(NULL);
	saved_film_manager_close();
	c_online_lsp_manager::get()->go_into_crash_mode();
	network_webstats_force_reset();
	user_interface_networking_set_ui_upload_quota(NONE);
	console_close();

	if (x_just_upload_dammit)
	{
		main_crash_just_upload_dammit();
	}

	c_static_string<256> reason;
	bool upload_to_server = game_state_debug_server_file_uploading_enabled(&reason);
	if (upload_to_server)
	{
		//c_datamine datamine(1, "crash", 0);
		//datamine.commit();
		//data_mine_upload();
		upload_debug_started = upload_debug_start();
		create_fake_minidump = false;
		if (g_fake_minidump_creation)
		{
			create_fake_minidump = true;
		}
		else if (!g_suppress_upload_debug && !g_suppress_keyboard_for_minidump)
		{
			//create_fake_minidump = online_guide_show_virtual_keyboard_ui(...) == 997;
		}
	}

	while (!is_debugger_present())
	{
		RENDER_THREAD_LOCK;

		uns32 this_loop_time = system_milliseconds();
		uns32 time_delta = this_loop_time - lock_time;
		real32 shell_seconds_elapsed = time_delta / 1000.0f;
		lock_time = this_loop_time;

		if (crash_ui_enable)
		{
			c_rasterizer::begin_frame();
			c_rasterizer::setup_targets_simple();
		}

		text.set(events_get());

		char upload_debug_output[1024]{};
		if (upload_debug_started && upload_debug_get_output(upload_debug_output, 1024))
		{
			text.append(upload_debug_output);
		}

		//if (create_fake_minidump)
		//{
		//	if (g_fake_minidump_creation)
		//	{
		//		create_fake_minidump = false;
		//		c_static_string<256> description = "crash fast";
		//		crash_user_input_upload_now(description.get_string(), description.length() + 1);
		//	}
		//	else if()
		//	{
		//		create_fake_minidump = false;
		//		if (!)
		//		{
		//			int32 description_length = 0;
		//			char description[256]{};
		//			wchar_string_to_ascii_string(description_wide, description, 256, &description_length);
		//			if (description_length > 1)
		//				crash_user_input_upload_now(description, description_length);
		//		}
		//	}
		//}

		e_main_pregame_frame pregame_frame_type = _main_pregame_frame_crash_uploading;
		if (upload_to_server)
		{
			if (upload_debug_completed && upload_debug_success)
			{
				pregame_frame_type = _main_pregame_frame_crash_done;
			}
			else if (!upload_debug_completed)
			{
				if (upload_debug_started)
				{
					if ((system_seconds() & 1) != 0)
					{
						text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE\r\nWE NEED THE CRASH LOGS!1!!");
					}
					else
					{
						text.append("\r\n\r\nEACH TIME YOU REBOOT A CRASH\r\n  A BABY KITTEN DIES!1!!");
					}
				}
			}
			else if ((system_seconds() & 1) != 0)
			{
				text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE!!!\r\nUPLOAD FAILED TO START!!!");
			}
			else
			{
				text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE!!!\r\nGET A TESTER!!!");
			}
		}
		else
		{
			reason.copy_to(text.get_buffer(), text.element_count);
			upload_debug_completed = true;
			upload_debug_success = true;
		}

		if (crash_ui_enable)
		{
			main_render_pregame(pregame_frame_type, text.get_string());
			c_rasterizer::end_frame();
		}

		overlapped_update();
		input_update();
		terminal_update(shell_seconds_elapsed);
		online_lsp_update();
		data_mine_update();
		network_webstats_update();
		transport_global_update();
		online_process_debug_output_queue();

		if (upload_debug_started && !upload_debug_completed)
		{
			upload_debug_completed = upload_debug_complete(&upload_debug_success);
		}

		sleep(0);
	}

	// by setting the `emergency_mode` to `false` we enable fonts again?
	{
		c_font_cache_scope_lock scope_lock{};
		g_font_globals.emergency_mode = false;
	}

	//while (!return_to_application)
	//{
	//	__debugbreak();
	//}

	x_recursion_lock = false;
	
	main_loop_pregame_halt_and_catch_fire_pop();
}

void __cdecl main_halt_and_display_errors()
{
	INVOKE(0x00505840, main_halt_and_display_errors);

	//main_globals.halted_with_errors = true;
	////damaged_media_halt_and_display_error();
	//main_game_change(NULL);
}

bool __cdecl main_halted_with_errors()
{
	return INVOKE(0x00505860, main_halted_with_errors);

	//return main_globals.halted_with_errors;
}

bool __cdecl main_is_in_main_loop_pregame()
{
	return INVOKE(0x005058A0, main_is_in_main_loop_pregame);

	//return main_globals.main_loop_pregame_entered != 0;
}

bool __cdecl main_is_switching_zone_sets()
{
	return INVOKE(0x005058B0, main_is_switching_zone_sets);

	//return main_globals.switch_zone_set;
}

bool __cdecl main_kick_startup_masking_sequence(bool kick_input)
{
	return INVOKE(0x005058C0, main_kick_startup_masking_sequence, kick_input);
}

void __cdecl main_load_core()
{
	//INVOKE(0x00505980, main_load_core);

	main_load_core_name("core");
}

void __cdecl main_load_core_name(const char* core_name)
{
	//INVOKE(0x005059A0, main_load_core_name, core_name);

	main_globals.load_core = true;
	main_globals.core_file_name.set(core_name);
	director_notify_map_reset();
}

void __cdecl main_load_core_private()
{
	//INVOKE(0x005059C0, main_load_core_private);

	if (game_in_progress())
	{
		game_state_load_core(main_globals.core_file_name.get_string());
		main_globals.load_core = false;
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

void __cdecl main_loop_initialize_restricted_regions()
{
	if (game_is_multithreaded())
	{
		restricted_region_unlock_primary(k_global_render_data_region);
		restricted_region_unlock_primary(k_game_state_render_region);
	}
}

void __cdecl main_loop()
{
	//INVOKE(0x005059E0, main_loop);

	if (game_is_multithreaded())
	{
		g_render_thread_user_setting = true;
		g_render_thread_enabled.set(_render_thread_mode_enabled);
	}

	main_loop_enter();
	main_loop_initialize_restricted_regions();

	uns32 render_thread_lock_token = 0;
	uns32 previous_loop_time = system_milliseconds();

	while (!main_globals.exit_game)
	{
		if (main_loop_is_suspended())
		{
			switch_to_thread();
		}
		else
		{
			uns32 current_loop_time = system_milliseconds();
			uns32 loop_time_advance = current_loop_time - previous_loop_time;
			previous_loop_time = current_loop_time;
			if (!disable_main_loop_throttle && loop_time_advance < 7)
			{
				uns32 sleepy_time = 7 - loop_time_advance;
				sleep(sleepy_time);
			}
			main_globals.main_loop_pregame_last_time = current_loop_time;


			main_set_single_thread_request_flag(_single_thread_for_user_request, !g_render_thread_user_setting);

			bool single_threaded_mode_active = false;

			e_render_thread_mode render_thread_mode = render_thread_get_mode();
			if (render_thread_mode == _render_thread_mode_enabled || render_thread_mode == _render_thread_mode_loading_screen)
			{
				main_thread_process_pending_messages();
				main_loop_body_multi_threaded();
			}
			else
			{
				single_threaded_mode_active = true;
				main_thread_process_pending_messages();
				main_loop_body_single_threaded();
			}

			main_set_single_thread_request_flag(_single_thread_for_unknown3, screenshot_globals.take_screenshot2 && screenshot_globals.take_screenshot);

			bool single_threaded_desired = g_single_thread_request_flags.peek() == _single_thread_for_user_request;
			if (single_threaded_desired != single_threaded_mode_active)
			{
				if (single_threaded_mode_active)
				{
					unlock_resources_and_resume_render_thread(render_thread_lock_token);
				}
				else
				{
					render_thread_lock_token = _internal_halt_render_thread_and_lock_resources(__FILE__, __LINE__);
				}
			}
		}
	}

	render_thread_set_mode(_render_thread_mode_enabled, _render_thread_mode_disabled);

	main_loop_dispose_restricted_regions();
	main_loop_exit();
}

// functions for `main_loop_body`
void __cdecl sub_5077E0()
{
	INVOKE(0x005077E0, sub_5077E0);

	//if (input_key_frames_down(_key_print_screen, _input_type_ui) == 1)
	//{
	//	s_key_state key{};
	//	input_get_key(&key, _input_type_ui);
	//	take_screenshot(); // 0060F420
	//}
}

void __cdecl sub_641A60(real32 shell_seconds_elapsed)
{
	//INVOKE(0x00641A60, sub_641A60, shell_seconds_elapsed);

	if (game_is_ui_shell())
	{
		c_controller_interface* controller = controller_get(_controller0);
		c_player_profile_interface* player_profile = controller->get_player_profile_interface();

		static e_player_model_choice x_player_model_choice = _player_model_choice_spartan;
		if (x_player_model_choice != player_profile->get_player_model_choice())
		{
			switch (x_player_model_choice = player_profile->get_player_model_choice())
			{
			case _player_model_choice_spartan:
			{
				user_interface_start_hs_script_by_name("humanhangar");
			}
			break;
			case _player_model_choice_elite:
			{
				user_interface_start_hs_script_by_name("elitehangar");
			}
			break;
			}
		}
	}
}

void __cdecl main_loop_body()
{
	//INVOKE(0x00505C10, main_loop_body);

	PROFILER(main_loop_body)
	{
		// we no longer hook calls from `main_loop_body` for this
		test_main_loop_body_begin();

		PROFILER(main, main_loop)
		{
			render_debug_reset_cache_to_game_tick_entires();
			collision_log_begin_frame();

			PROFILER(input_update) // main_loop, input
			{
				input_update();
				input_abstraction_update();
				sub_5077E0(); // take_screenshot hotkey wrapper
				global_preferences_update();
			}

			PROFILER(idle_font_async_shell_and_errors) // main_loop, async
			{
				async_idle();
				shell_idle();
			}

			PROFILER(idle_background_tasks) // main_loop, bckgrnd
			{
				tag_files_sync_update();
				cache_files_copy_do_work();
				main_loading_idle();
				//async_statistics_display();
				optional_cache_main_loop_idle();
			}

			//g_cubemap_render_manager.tick();
			main_loop_process_global_state_changes();
			//if (tag_files_can_load_tags())
			//{
			//	main_loop_process_global_state_changes();
			//}
			//else
			//{
			//	main_events_reset(_main_reset_events_pending_xsync);
			//}

			PROFILER(idle_tag_resources) // main_loop, tag_idle
			{
				tag_resources_main_loop_idle();
			}

			// main_loop, main_memory
			//main_verify_memory();
			//main_dump_memory();

			if (shell_application_is_paused())
			{
				g_main_gamestate_timing_data->flags.set(_game_published_shell_paused, true);
			}
			else
			{
				PROFILER(main_shell) // main_loop, shell
				{
					real32 shell_seconds_elapsed = 0.0f;
					real32 world_seconds_elapsed = 0.0f;
					real32 game_seconds_elapsed = 0.0f;
					int32 game_ticks_elapsed = 0;

					// main_loop, wait
					shell_seconds_elapsed = main_time_update();

					sub_641A60(shell_seconds_elapsed);
					main_time_frame_rate_debug();

					// main_loop, unittest
					//unit_test_update();

					overlapped_update();

					PROFILER(update_console_terminal_and_debug_menu) // main_loop, con
					{
						TAG_RESOURCES_GAME_LOCK();

						if (main_globals.drop_cheat_tag)
						{
							main_cheat_drop_tag_private();
						}

						if (g_toggle_game_scripts)
						{
							if (g_run_game_scripts = !g_run_game_scripts)
							{
								console_printf("game scripts enabled");
							}
							else
							{
								console_printf("game scripts disabled");
							}
							g_toggle_game_scripts = false;
						}

						real32 terminal_update_time = main_time_halted() ? 0.0f : shell_seconds_elapsed;

						terminal_update(terminal_update_time);
						console_update(terminal_update_time);
						//telnet_console_update();
						//xbox_connection_update();
						remote_command_process();
						debug_menu_update();
						cinematic_debug_camera_control_update();
					}

					{
						TAG_RESOURCES_GAME_LOCK();

						world_seconds_elapsed = main_time_halted() ? 0.0f : shell_seconds_elapsed;

						g_main_gamestate_timing_data->shell_dt += shell_seconds_elapsed;
						g_main_gamestate_timing_data->world_dt += world_seconds_elapsed;
						g_main_gamestate_timing_data->flags.set(_game_published_main_time_halted, main_time_halted());

						PROFILER(update_ui) // main_loop, ui
						{
							user_interface_update(shell_seconds_elapsed);
							closed_caption_update();
							bink_playback_update();
							screenshots_uploader_update();
							spartan_program_handler_update();
							saved_film_manager_update();
						}

						PROFILER(network_io) // main_loop, netio
						{
							network_receive();
							network_update();
						}

						PROFILER(simulation_update)
						{
							simulation_update();
						}

						if (simulation_in_progress())
						{
							PROFILER(main_simulation) // main_loop, main_sim
							{
								bool game_time_advancing = false;
								PROFILER(main_simulation_time_update)
								{
									game_time_advancing = game_time_update(world_seconds_elapsed, &game_seconds_elapsed, &game_ticks_elapsed);
								}

								g_main_gamestate_timing_data->flags.set(_game_published_game_time_unchanged, !game_time_advancing);
								g_main_gamestate_timing_data->game_dt += game_seconds_elapsed;
								g_main_gamestate_timing_data->elapsed_game_ticks += game_ticks_elapsed;
								g_main_gamestate_timing_data->flags.set(_game_published_new_game_tick, g_main_gamestate_timing_data->elapsed_game_ticks);

								//if (!g_main_gamestate_timing_data->elapsed_game_ticks)
								//{
								//	g_main_gamestate_timing_data->flags.set(_game_published_new_game_tick, user_interface_is_active());
								//}

								PROFILER(main_simulation_update)
								{
									player_control_update(world_seconds_elapsed, game_seconds_elapsed);
									game_update(game_ticks_elapsed, &game_seconds_elapsed);
									network_send();
								}

								PROFILER(main_simulation_post_update)
								{
									game_frame(game_seconds_elapsed);
									director_update(world_seconds_elapsed);
									observer_update(world_seconds_elapsed);
									game_engine_interface_update(world_seconds_elapsed);
									chud_update(world_seconds_elapsed);
									rumble_update(world_seconds_elapsed);
									achievements_update(world_seconds_elapsed);
									first_person_weapons_update_camera_estimates();

									if (main_time_halted())
									{
										sound_idle();
									}
									else
									{
										sound_render();
									}
								}

								//render_debug_game_thread();
							}
						}
						else
						{
							PROFILER(main_simulation_inactive)
							{
								simulation_update_pregame();
								network_send();
								rumble_clear_all_now();

								g_main_gamestate_timing_data->flags.set(_game_published_pregame, true);

								if (main_time_halted())
								{
									sound_idle();
								}
								else
								{
									sound_render();
								}

#if 0
								static uns32 main_loop_network_time_since_abort = 0;
								if (simulation_aborted())
								{
									uns32 network_time_since_abort = simulation_get_network_time_since_abort();
									if (network_time_since_abort > 1000)
									{
										network_time_since_abort /= 1000;
									}
									else
									{
										network_time_since_abort = 0;
									}
								
									if (network_time_since_abort >= 5)
									{
										if (network_time_since_abort >= main_loop_network_time_since_abort + 5)
										{
											main_loop_network_time_since_abort = network_time_since_abort;
											console_printf("simulation aborted %i seconds ago [%s]. Waiting for instructions...",
												network_time_since_abort,
												simulation_get_abort_reason_string());
										}
									}
									else
									{
										main_loop_network_time_since_abort = 0;
									}
								}
								else
								{
									main_loop_network_time_since_abort = 0;
								}
#endif
							}
						}

						hf2p_game_update();
					}
				}
			}

			if (game_time_get_paused())
			{
				g_main_gamestate_timing_data->flags.set(_game_published_game_time_paused, true);
			}

			if (game_time_initialized() && fabsf(game_time_get_speed() - 1.0f) > k_real_epsilon)
			{
				g_main_gamestate_timing_data->flags.set(_game_published_game_speed_slowed, true);
			}

			if (game_in_progress() && game_is_playback() && !game_is_authoritative_playback())
			{
				g_main_gamestate_timing_data->flags.set(_game_published_network_playback_client, true);
			}

			constexpr real32 k_maintain_minimal_framerate = 15.0f;
			constexpr real32 k_minimum_playable_framerate = 40.0f; // 120.0f in h3ek
			if (g_main_gamestate_timing_data->world_dt < 1.0f / k_maintain_minimal_framerate)
			{
				if (user_interface_requests_unlocked_framerate())
				{
					g_main_gamestate_timing_data->flags.set(_game_published_ui_request, true);
				}
				else if (game_in_progress() && game_is_playback())
				{
					if (IN_RANGE(g_main_gamestate_timing_data->world_dt, 0.0f, 1.0f / k_minimum_playable_framerate)
						&& !g_main_gamestate_timing_data->flags.test(_game_published_framerate_infinite)
						&& !g_main_gamestate_timing_data->flags.test(_game_published_ui_request)
						&& !g_main_gamestate_timing_data->flags.test(_game_published_new_game_tick))
					{
						g_main_gamestate_timing_data->flags.clear();
					}
				}
			}
			else
			{
				g_main_gamestate_timing_data->flags.set(_game_published_maintain_minimal_framerate, true);
			}

			if (game_is_multithreaded())
			{
				g_main_gamestate_timing_data->flags.set(_game_published_framerate_infinite, !main_time_is_throttled());
			}

			PROFILER(predict_tag_resources)
			{
				main_render_predict_tag_resources();
			}
		}

		exceptions_update();
		collision_log_end_frame();

		// we no longer hook calls from `main_loop_body` for this
		test_main_loop_body_end();
	}
}

void __cdecl main_loop_body_multi_threaded()
{
	//INVOKE(0x00506070, main_loop_body_multi_threaded);

	publish_waiting_gamestate();
	main_loop_body();
	publish_waiting_gamestate();
}

void __cdecl main_loop_body_single_threaded()
{
	//INVOKE(0x00506080, main_loop_body_single_threaded);

	PROFILER(single_thread_update)
	{
		main_loop_body();
	}

	if (!game_is_multithreaded() || !g_main_gamestate_timing_data->flags.is_empty())
	{
		font_idle();

		if (game_is_multithreaded())
		{
			main_time_mark_publishing_start_time();
			if (restricted_region_publish_to_mirror(k_game_state_shared_region))
			{
				PROFILER(single_thread_render)
				{
					main_time_mark_publishing_end_time();

					restricted_region_unlock_primary(k_game_state_shared_region);
					if (restricted_region_lock_mirror(k_game_state_shared_region))
					{
						main_thread_lock_rasterizer_and_resources();
						process_published_game_state(true);
						main_thread_unlock_rasterizer_and_resources();

						if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
						{
							restricted_region_unlock_mirror(k_game_state_shared_region);
						}
					}
					restricted_region_lock_primary(k_game_state_shared_region);
				}
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
}

void __cdecl main_loop_enter()
{
	//INVOKE(0x00506200, main_loop_enter);

	main_globals.startup_sequence = true;
	main_globals.startup_sequence_performed = false;

	main_loading_initialize();
	main_game_initialize();
	main_time_initialize();
	console_initialize();
	game_initialize();

	g_main_gamestate_timing_data = (s_game_tick_time_samples*)g_main_gamestate_timing_data_allocator.allocate(sizeof(s_game_tick_time_samples), "timing samples");
	g_main_render_timing_data = (s_game_tick_time_samples*)g_main_render_timing_data_allocator.allocate(sizeof(s_game_tick_time_samples), "timing samples");

	shell_halt_if_necessary();
	tag_files_initialize_from_main();
	main_render_predict_initialize();
	console_execute_initial_commands();
	physical_memory_resize_region_initialize();

	g_main_gamestate_timing_data->initialize();
	g_main_render_timing_data->initialize();

	main_render_purge_pending_messages();
	main_time_mark_publishing_start_time();
	main_time_mark_publishing_end_time();

	//main_loop_present_first_frame();

	if (game_is_multithreaded())
	{
		c_rasterizer::rasterizer_device_release_thread();
		start_thread(k_thread_render);
		thread_set_priority(k_thread_render, _thread_priority_above_normal);
		start_thread(k_thread_update);
	}
	start_thread(k_thread_audio);
}

void __cdecl main_loop_exit()
{
	//INVOKE(0x00506360, main_loop_exit);

	if (game_is_multithreaded())
	{
		main_render_purge_pending_messages();
		wait_for_thread_to_exit(k_thread_render, 1);
	}

	main_render_sub_604AD0();
	main_game_unload_and_prepare_for_next_game(NULL);
	physical_memory_resize_region_dispose();
	game_dispose();
	console_dispose();
	main_loading_dispose();
}

void __cdecl main_loop_pregame_do_work()
{
	//if (main_globals.main_loop_halt_and_catch_fire_entered_count)
	//{
	//	overlapped_update();
	//}
	//else
	{
		main_thread_process_pending_messages();
		overlapped_update();
		network_idle();
		//hf2p_idle();

		if (render_thread_get_mode() == _render_thread_mode_disabled)
		{
			main_loop_pregame_show_progress_screen();
		}

		//if (render_debug_initialized())
		//{
		//	render_debug_update();
		//}

		sound_idle();
		input_update();
		shell_idle();

		bink_playback_check_for_terminate_no_lock();
	}
}

bool __cdecl main_loop_is_suspended()
{
	return main_loop_suspended;
}

void __cdecl main_loop_pregame()
{
	//INVOKE(0x005063A0, main_loop_pregame);

	if (is_main_thread())
	{
		uns32 current_time = system_milliseconds();
		bool bink_active = bink_playback_active();

		if (!main_globals.main_loop_pregame_entered
			&& main_globals.main_loop_pregame_last_time
			&& current_time - main_globals.main_loop_pregame_last_time >= uns32(!bink_active ? 24 : 15))
		{
			main_globals.main_loop_pregame_entered++;
			if (bink_active)
			{
				main_globals.main_loop_pregame_last_time = current_time;
			}

			//main_loop_pregame_update_stack_high_water_mark();
			main_loop_pregame_do_work();

			if (!bink_active)
			{
				main_globals.main_loop_pregame_last_time = system_milliseconds();
			}
			main_globals.main_loop_pregame_entered--;
		}

		//main_globals.pregame_progress_screen_shown = 0;
	}
}

void __cdecl main_loop_pregame_disable(bool disable)
{
	//INVOKE(0x00506430, main_loop_pregame_disable, disable);

	if (is_main_thread())
	{
		if (disable)
		{
			main_globals.main_loop_pregame_entered++;
		}
		else
		{
			main_globals.main_loop_pregame_entered--;
		}
	}
}

void __cdecl main_loop_pregame_halt_and_catch_fire_pop()
{
	if (is_main_thread())
	{
		main_globals.main_loop_pregame_entered--;
	}
}

void __cdecl main_loop_pregame_halt_and_catch_fire_push()
{
	if (is_main_thread())
	{
		main_globals.main_loop_pregame_entered++;
	}
}

void __cdecl main_loop_pregame_show_progress_screen()
{
	//INVOKE(0x00506460, main_loop_pregame_show_progress_screen);

	c_static_wchar_string<12288> pregame_frame_text;
	e_main_pregame_frame pregame_frame_type = main_loading_get_loading_status(&pregame_frame_text);
	if (pregame_frame_type == _main_pregame_frame_none)
	{
		//editor_show_pregame_progress(main_pregame_frame, pregame_frame_text.get_string());
	}
	else
	{
		RENDER_THREAD_LOCK;
		if (c_rasterizer::begin_frame())
		{
			c_rasterizer::setup_targets_simple();

			if (pregame_frame_type == _main_pregame_frame_loading_screen)
			{
				main_render_pregame_loading_screen();
			}
			else if (pregame_frame_type == _main_pregame_frame_cache_loading)
			{
				main_render_status_message(pregame_frame_text.get_string());
			}
			else
			{
				char char_string[12288]{};
				int32 char_string_length = 0;
				wchar_string_to_ascii_string(pregame_frame_text.get_string(), char_string, NUMBEROF(char_string), &char_string_length);
				char_string[char_string_length] = 0;
				main_render_pregame(pregame_frame_type, char_string);
			}

			if (pregame_frame_type != _main_pregame_frame_loading_debug)
			{
				main_time_throttle(0);
			}
			c_rasterizer::end_frame();
		}
	}
}

void __cdecl main_loop_process_global_state_changes()
{
	//INVOKE(0x005065B0, main_loop_process_global_state_changes);

	if (game_in_editor())
	{
		RENDER_THREAD_LOCK;

		main_game_launch_default_editor();
		main_game_change_update();

		if (main_globals.skip_cinematic)
		{
			main_globals.skip_cinematic = false;
		}

		if (main_globals.revert_map)
		{
			main_globals.revert_map = false;
		}

		if (!main_globals.revert_map_flags.is_empty())
		{
			main_globals.revert_map_flags.clear();
		}

		if (main_globals.reset_map)
		{
			main_reset_map_private();
		}

		if (main_globals.deactivate_cinematic_zone_from_tag)
		{
			main_deactivate_cinematic_tag_private();
		}

		if (main_globals.activate_cinematic_zone_from_tag)
		{
			main_activate_cinematic_tag_private();
		}

		if (main_globals.reset_zone_resources)
		{
			main_reset_zone_resources_private();
		}

		if (main_globals.switch_zone_set)
		{
			main_switch_zone_set_private();
		}

		if (main_globals.save_map)
		{
			main_globals.save_map = false;
		}

		if (main_globals.save_map_and_exit)
		{
			main_globals.save_map_and_exit = false;
		}

		if (main_globals.save_core)
		{
			main_globals.save_core = false;
		}

		if (main_globals.load_core)
		{
			main_globals.load_core = false;
		}

		if (main_globals.ui_saving_files)
		{
			main_globals.ui_saving_files = false;
		}

		if (main_globals.reload_active_zone_set)
		{
			main_reload_active_zone_set_private();
		}

		if (main_globals.modify_zone_activation)
		{
			main_modify_zone_activation_private();
		}

		if (main_globals.gamestate_decompression_pending)
		{
			main_globals.gamestate_decompression_pending = false;
		}
	}
	else
	{
		if (main_globals.run_xdemos)
		{
			main_run_demos_private();
		}

		main_game_launch_default();
		main_game_change_update();

		if (main_events_pending())
		{
			RENDER_THREAD_LOCK;

			exceptions_update();

			if (main_globals.skip_cinematic)
			{
				main_skip_cinematic_private();
			}

			if (main_globals.revert_map)
			{
				main_revert_map_private();
			}

			if (main_globals.reset_map)
			{
				main_reset_map_private();
			}

			if (main_globals.deactivate_cinematic_zone_from_tag)
			{
				main_deactivate_cinematic_tag_private();
			}

			if (main_globals.activate_cinematic_zone_from_tag)
			{
				main_activate_cinematic_tag_private();
			}

			if (saved_film_manager_has_pending_global_state_change())
			{
				saved_film_manager_perform_global_state_change();
			}

			if (main_globals.gamestate_decompression_pending)
			{
				main_game_gamestate_decompress_and_apply_private();
			}

			if (game_state_compressor_lock_pending())
			{
				game_state_compressor_lock_update();
			}

			if (main_globals.reset_zone_resources)
			{
				main_reset_zone_resources_private();
			}

			if (main_globals.prepare_to_switch_zone_set)
			{
				main_prepare_to_switch_zone_set_private();
			}

			if (main_globals.switch_zone_set)
			{
				main_switch_zone_set_private();
			}

			if (main_globals.modify_zone_activation)
			{
				main_modify_zone_activation_private();
			}

			if (main_globals.save_map)
			{
				main_save_map_private();
			}

			if (main_globals.save_map_and_exit)
			{
				main_save_map_and_exit_private();
			}

			if (main_globals.save_core)
			{
				main_save_core_private();
			}

			if (main_globals.load_core)
			{
				main_load_core_private();
			}

			if (main_globals.ui_saving_files)
			{
				main_user_interface_save_files_private();
			}

			if (main_globals.reload_active_zone_set)
			{
				main_reload_active_zone_set_private();
			}

			if (cache_file_tag_resources_prefetch_update_required())
			{
				cache_file_tag_resources_update_prefetch_state();
			}
		}
	}
}

void __cdecl main_loop_resume()
{
	if (main_loop_suspended)
	{
		main_loop_suspended = false;
	}
}

void __cdecl main_loop_suspend()
{
	if (!main_loop_suspended)
	{
		main_loop_suspended = true;
	}
}

void __cdecl main_loop_status_message(const wchar_t* status_message)
{
	//INVOKE(0x00506900, main_loop_status_message, status_message);

	RENDER_THREAD_LOCK;
	c_rasterizer::begin_frame();
	c_rasterizer::setup_targets_simple();
	main_render_status_message(status_message);
	main_time_throttle(0);
	c_rasterizer::end_frame();
}

bool __cdecl main_menu_has_performed_startup_sequence()
{
	//return INVOKE(0x005069B0, main_menu_has_performed_startup_sequence);

	return main_globals.startup_sequence_performed;
}

void __cdecl main_modify_zone_activation_private()
{
	INVOKE(0x005069C0, main_modify_zone_activation_private);

	//main_globals.modify_zone_activation = false;
	//if (game_in_progress())
	//{
	//	scenario_modify_active_zones(&main_globals.pending_zone_activation);
	//}
	//main_globals.pending_zone_activation.clear();
}

void __cdecl main_prepare_for_switch_zone_set(int32 zone_set_index)
{
	//INVOKE(0x00506A10, main_prepare_for_switch_zone_set, zone_set_index);

	struct scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
	{
		console_warning("tried to switch to a zone-set without a scenario loaded");
	}
	else if (!VALID_INDEX(zone_set_index, global_scenario->zone_sets.count))
	{
		console_warning("tried to switch to invalid zone-set %d", zone_set_index);
	}
	else if (zone_set_index != scenario_zone_set_index_get())
	{
		main_trace_event_internal(__FUNCTION__);
		main_globals.prepare_to_switch_zone_set = true;
		main_globals.prepare_to_switch_to_zone_set_index = zone_set_index;
		//chud_messaging_special_load(true);
	}
	else if (!main_globals.prepare_to_switch_zone_set)
	{
		console_warning("tried to prepare to switch to current zone-set %d", zone_set_index);
	}
	else
	{
		main_globals.prepare_to_switch_zone_set = false;
		main_globals.prepare_to_switch_to_zone_set_index = 0;
		//chud_messaging_special_load(false);
	}
}

void __cdecl main_prepare_to_switch_zone_set_private()
{
	INVOKE(0x00506A70, main_prepare_to_switch_zone_set_private);

	//if (!game_in_editor() && !scenario_prepare_to_switch_zone_set(main_globals.prepare_to_switch_to_zone_set_index))
	//{
	//	event(_event_error, "main_prepare_to_switch_zone_set() failed for '%s' zone set %d, must abort game",
	//		game_options_get()->scenario_path.get_string(),
	//		main_globals.switch_to_zone_set_index);
	//
	//	main_game_load_panic();
	//}
	//
	//main_globals.prepare_to_switch_zone_set = false;
	//main_globals.prepare_to_switch_to_zone_set_index = NONE;
}

void __cdecl main_print_version()
{
	//INVOKE(0x00506AB0, main_print_version);

	console_printf(version_get_full_string() /*shell_get_version()*/);
	console_printf("minor version %d cache file language %s",
		get_map_minor_version(),
		get_language_display_name(get_map_language()));
}

void __cdecl main_reload_active_zone_set_private()
{
	INVOKE(0x00506AE0, main_reload_active_zone_set_private);

	//main_globals.reload_active_zone_set = false;
}

void __cdecl main_reset_map()
{
	INVOKE(0x00506AF0, main_reset_map);

	////main_trace_event_internal(__FUNCTION__);
	//main_reset_map_internal(false);
}

void __cdecl main_reset_map_internal(bool random)
{
	INVOKE(0x00506B00, main_reset_map_internal, random);

	//main_globals.reset_map = true;
	//main_globals.reset_map_random_seed = random;
}

void __cdecl main_reset_map_private()
{
	//INVOKE(0x00506B20, main_reset_map_private);

	if (game_in_progress() && !game_time_get_paused())
	{
		main_clear_global_pending_zone_activation(NONE);
		main_game_reset_map(main_globals.reset_map_random_seed);
		main_globals.reset_map = false;
		main_globals.reset_map_random_seed = false;
	}
}

void __cdecl main_reset_map_random()
{
	INVOKE(0x00506B60, main_reset_map_random);

	////main_trace_event_internal(__FUNCTION__);
	//main_reset_map_internal(true);
}

void __cdecl main_reset_zone_resources()
{
	INVOKE(0x00506B70, main_reset_zone_resources);

	//if (global_scenario_try_and_get())
	//{
	//	//main_trace_event_internal(__FUNCTION__);
	//	main_globals.reset_zone_resources = true;
	//}
	//else
	//{
	//	console_warning("tried to switch to a zone-set without a scenario loaded");
	//}
}

void __cdecl main_reset_zone_resources_private()
{
	INVOKE(0x00506B90, main_reset_zone_resources_private);

	//main_globals.reset_zone_resources = false;
	//scenario_reset_zone_resources_from_main();
}

void __cdecl main_revert_map(bool user)
{
	INVOKE(0x00506BA0, main_revert_map, user);

	//main_globals.revert_map = true;
	//main_globals.revert_map_flags.set(_game_state_revert_user_bit, user);
}

void __cdecl main_revert_map_private()
{
	INVOKE(0x00506BD0, main_revert_map_private);
}

void __cdecl main_revert_map_scripting()
{
	INVOKE(0x00506CA0, main_revert_map_scripting);

	////main_trace_event_internal(__FUNCTION__);
	//main_globals.revert_map = true;
	//main_globals.revert_map_flags.clear();
	//main_globals.revert_map_flags.set(_game_state_revert_by_scripting_bit, true);
	//main_globals.revert_map_flags.set(_game_state_revert_keep_playing_cinematic_outros_bit, true);
}

//.text:00506CC0

void __cdecl main_run_demos_private()
{
	INVOKE(0x00506CD0, main_run_demos_private);

	//main_globals.run_xdemos = false;
}

void __cdecl main_save_and_exit_campaign()
{
	INVOKE(0x00506CE0, main_save_and_exit_campaign);

	//if (game_in_progress() && game_is_campaign())
	//{
	//	user_interface_interactive_session_request_campaign_quit(_campaign_quit_reason_quit_from_command);
	//}
	//else
	//{
	//	main_save_and_exit_campaign_immediately();
	//}
}

void __cdecl main_save_and_exit_campaign_immediately()
{
	//INVOKE(0x00506D10, main_save_and_exit_campaign_immediately);

	main_globals.save_map_and_exit = true;
}

void __cdecl main_save_core()
{
	//INVOKE(0x00506D20, main_save_core);

	main_save_core_name("core");
}

void __cdecl main_save_core_name(const char* core_name)
{
	//INVOKE(0x00506D40, main_save_core_name, core_name);

	main_globals.save_core = true;
	main_globals.core_file_name.set(core_name);
}

void __cdecl main_save_core_private()
{
	//INVOKE(0x00506D60, main_save_core_private);

	if (game_in_progress())
	{
		game_state_save_core(main_globals.core_file_name.get_string());
		main_globals.save_core = false;
	}
}

void __cdecl main_save_map()
{
	INVOKE(0x00506D80, main_save_map);

	//main_globals.save_map = true;
	//chud_messaging_special_autosave(true);
}

void __cdecl main_save_map_and_exit_private()
{
	//INVOKE(0x00506D90, main_save_map_and_exit_private);

	if (!game_is_playback())
	{
		saved_film_manager_close();
		game_state_save_to_persistent_storage_blocking();
		game_finish();
	}
	main_globals.save_map_and_exit = false;
}

bool __cdecl main_save_map_pending()
{
	//return INVOKE(0x00506DB0, main_save_map_pending);

	return main_globals.save_map;
}

void __cdecl main_save_map_private()
{
	INVOKE(0x00506DC0, main_save_map_private);

	//chud_messaging_special_autosave(false);
	//if (game_in_progress())
	//{
	//	game_state_save();
	//
	//	c_datamine datamine(0, "checkpoint save", 1, "main");
	//	data_mine_usability_add_basic_information(&datamine);
	//	data_mine_usability_add_all_players_status(&datamine, 4);
	//}
	//main_globals.save_map = false;
}

void __cdecl main_set_active_designer_zone_mask(uns32 designer_zone_mask)
{
	INVOKE(0x00506E50, main_set_active_designer_zone_mask, designer_zone_mask);

	//uns32 designer_zone_active_mask = global_designer_zone_active_mask_get();
	//main_globals.pending_zone_activation.activating_designer_zone_mask = designer_zone_mask & ~designer_zone_active_mask;
	//main_globals.pending_zone_activation.deactivating_designer_zone_mask = designer_zone_active_mask & ~designer_zone_mask;
	//main_globals.modify_zone_activation =
	//	TEST_MASK(designer_zone_active_mask, ~designer_zone_mask) ||
	//	TEST_MASK(designer_zone_mask, ~designer_zone_active_mask) ||
	//	main_globals.pending_zone_activation.deactivating_cinematic_zone_mask || 
	//	main_globals.pending_zone_activation.activating_cinematic_zone_mask;
}

void __cdecl main_set_single_thread_request_flag(e_single_threaded_request_flags flag, bool setting)
{
	//INVOKE(0x00506EB0, main_set_single_thread_request_flag, flag, set);

	g_single_thread_request_flags.set_bit(flag, setting);
}

void __cdecl main_skip_cinematic()
{
	INVOKE(0x00506ED0, main_skip_cinematic);

	//main_globals.switch_zone_set = false;
	//main_globals.save_map = false;
	//main_globals.skip_cinematic = true;
}

void __cdecl main_skip_cinematic_private()
{
	INVOKE(0x00506EF0, main_skip_cinematic_private);

	//main_globals.revert_map = true;
	//main_globals.revert_map_flags.clear();
	//main_globals.revert_map_flags.set(_game_state_revert_by_scripting_bit, true);
}

bool __cdecl main_startup_sequence()
{
	return INVOKE(0x00506F10, main_startup_sequence);
}

// main_status in main.hpp
//.text:00506FB0 ; void __cdecl main_status(const char* status_type, const char* format, ...)

struct s_file_reference;
void __cdecl main_status_dump(s_file_reference* file)
{
	INVOKE(0x00507100, main_status_dump, file);
}

void __cdecl main_status_print()
{
	//INVOKE(0x005071C0, main_status_print);

	for (int32 status_value_index = 0; status_value_index < NUMBEROF(g_status_values); status_value_index++)
	{
		const s_main_status_value* status_value = &g_status_values[status_value_index];
		if (csstrnlen(status_value->status_type, NUMBEROF(status_value->status_type)))
		{
			console_printf("%s: %s", status_value->status_type, status_value->status_data);
		}
	}
}

void __cdecl main_suppress_startup_sequence()
{
	INVOKE(0x005071F0, main_suppress_startup_sequence);

	//main_globals.startup_sequence = false;
}

void __cdecl main_switch_bsp(int32 zone_set_index)
{
	INVOKE(0x00507200, main_switch_bsp, zone_set_index);

	//event(_event_error, "switch bsp is a deprecated function. Use switch zone set instead.");
	//main_switch_zone_set(zone_set_index);
}

void __cdecl main_switch_scenario_and_zone_set(const char* map_name, int32 initial_zone_set)
{
	main_game_globals.launch_game_options.initial_zone_set_index = (int16)initial_zone_set;
	main_game_launch(map_name);
}

void __cdecl main_switch_zone_set(int32 zone_set_index)
{
	//INVOKE(0x00507210, main_switch_zone_set, zone_set_index);

	struct scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
	{
		console_warning("tried to switch to a zone-set without a scenario loaded");
	}
	else if (!VALID_INDEX(zone_set_index, scenario->zone_sets.count))
	{
		console_warning("tried to switch to invalid zone-set %d", zone_set_index);
	}
	else if (!scenario_zone_set_is_fully_active(zone_set_index))
	{
		main_trace_event_internal(__FUNCTION__);
		main_globals.switch_to_zone_set_index = zone_set_index;
		main_globals.switch_zone_set = true;
		chud_messaging_special_load(true);
	}
	else if (!main_globals.switch_zone_set)
	{
		console_warning("tried to switch to current zone-set %d", zone_set_index);
	}
	else
	{
		main_globals.switch_to_zone_set_index = zone_set_index;
		main_globals.switch_zone_set = false;
		chud_messaging_special_load(false);
	}
}

void __cdecl main_switch_zone_set_private()
{
	INVOKE(0x00507280, main_switch_zone_set_private);

	//bool load_succeeded = false;
	//if (game_in_editor())
	//{
	//	load_succeeded = editor_switch_zone_set(main_globals.switch_to_zone_set_index);
	//}
	//else
	//{
	//	load_succeeded = scenario_switch_zone_set(main_globals.switch_to_zone_set_index);
	//	chud_messaging_special_load(false);
	//}
	//
	//if (!load_succeeded)
	//{
	//	event(_event_error, "main_switch_structure_bsp() failed for '%s' bsp %d, must abort game",
	//		game_options_get()->scenario_path.get_string(),
	//		main_globals.switch_to_zone_set_index);
	//	main_game_load_panic();
	//}
	//
	//main_globals.switch_zone_set = false;
	//main_globals.switch_to_zone_set_index = NONE;
}

void __cdecl main_thread_combine_timing_data(void* address)
{
	INVOKE(0x005072D0, main_thread_combine_timing_data, address);

	//g_main_gamestate_timing_data->accum((s_game_tick_time_samples*)address);
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

	//c_rasterizer::cleanup_d3d_states(false);
	//tag_resources_unlock_render_gpu_usage(tag_resources_unlock_render_access());
}

bool __cdecl main_time_halted()
{
	//return INVOKE(0x00507370, main_time_halted);

	bool time_halted = shell_application_is_paused();

	if ((debug_console_pauses_game && debugging_system_has_focus())
		&& (!game_in_progress() || !game_has_nonlocal_players() || game_is_authoritative_playback()))
	{
		time_halted = true;
	}

	if (main_globals.reset_zone_resources)
	{
		time_halted = true;
	}

	return time_halted;
}

void __cdecl main_user_interface_save_files()
{
	INVOKE(0x00507380, main_user_interface_save_files);

	//if (!main_globals.ui_saving_files)
	//{
	//	main_globals.ui_saving_files = true;
	//	if (game_in_progress())
	//	{
	//		chud_messaging_special_saving(true);
	//	}
	//}
}

void __cdecl main_user_interface_save_files_private()
{
	INVOKE(0x005073B0, main_user_interface_save_files_private);

	//main_globals.ui_saving_files = false;
	//if (game_in_progress())
	//{
	//	chud_messaging_special_saving(false);
	//}
}

void __cdecl process_published_game_state(bool render)
{
	//INVOKE(0x00507450, process_published_game_staterender);

	PROFILER(process_published_game_state)
	{
		g_main_render_timing_data->accum(g_main_gamestate_timing_data);

		if (render && c_rasterizer::rasterizer_thread_owns_device())
		{
			main_render();
		}
		else
		{
			main_render_process_messages();
		}
	}
}

void __cdecl publish_waiting_gamestate()
{
	INVOKE(0x005074D0, publish_waiting_gamestate);

	//if (render_thread_get_mode() == _render_thread_mode_enabled)
	//{
	//	if (TEST_MASK(g_main_gamestate_timing_data->flags.get_unsafe(), MASK(k_game_tick_publishing_flag_count)))
	//	{
	//		main_time_mark_publishing_start_time();
	//		if (restricted_region_publish_to_mirror(k_game_state_shared_region))
	//		{
	//			g_main_gamestate_timing_data->reset();
	//			main_render_purge_pending_messages();
	//			main_time_mark_publishing_end_time();
	//		}
	//	}
	//}
}

e_render_thread_mode __cdecl render_thread_get_mode()
{
	//return INVOKE(0x00507550, render_thread_get_mode);

	e_render_thread_mode mode = _render_thread_mode_disabled;
	if (game_is_multithreaded())
	{
		mode = (e_render_thread_mode)g_render_thread_enabled.peek();
	}
	return mode;
}

//.text:00507570 ; bool __cdecl render_thread_is_halted_waiting_for_lock()
//.text:00507590 ; void __cdecl render_thread_lock_rasterizer_and_resources()
//.text:005075A0 ; uns32 __cdecl render_thread_loop(void*)

bool __cdecl render_thread_set_mode(e_render_thread_mode old_setting, e_render_thread_mode setting)
{
	//return INVOKE(0x005076D0, render_thread_set_mode, old_setting, setting);

	return game_is_multithreaded() && g_render_thread_enabled.set_if_equal(setting, old_setting) == old_setting;
}

//.text:00507700 ; void __cdecl render_thread_unlock_rasterizer_and_resources()

void __cdecl unlock_resources_and_resume_render_thread(uns32 token)
{
	INVOKE(0x00507940, unlock_resources_and_resume_render_thread, token);

	//if (game_is_multithreaded())
	//{
	//	if (TEST_BIT(token, _unlock_render_section))
	//	{
	//		c_rasterizer::rasterizer_device_release_thread();
	//		restricted_region_unlock_primary(k_global_render_data_region);
	//		restricted_region_unlock_primary(k_game_state_render_region);
	//	}
	//
	//	if (TEST_BIT(token, _resume_render_thread) && game_is_multithreaded())
	//	{
	//		render_thread_set_mode(_render_thread_mode_disabled, _render_thread_mode_enabled);
	//	}
	//}
}

void __cdecl main_write_stack_to_crash_info_status_file(const char* crash_info, void* context)
{
	//INVOKE(0x0066D180, main_write_stack_to_crash_info_status_file, crash_info, context);

	s_file_reference crash_info_output_file{};
	file_reference_create_from_path(&crash_info_output_file, k_crash_info_output_filename, false);
	file_create_parent_directories_if_not_present(&crash_info_output_file);

	if (file_exists(&crash_info_output_file))
	{
		file_delete(&crash_info_output_file);
	}

	uns32 error = 0;
	if (file_create(&crash_info_output_file) && file_open(&crash_info_output_file, FLAG(_file_open_flag_desired_access_write), &error))
	{
		const char* string = "stack:\r\n";
		file_write(&crash_info_output_file, strlen_debug(string), string);

		if (context)
		{
			stack_walk_with_context(&crash_info_output_file, 1, static_cast<_CONTEXT*>(context));
		}
		else
		{
			stack_walk_to_file(&crash_info_output_file, 3);
		}

		if (crash_info)
		{
			file_write(&crash_info_output_file, strlen_debug(crash_info), crash_info);
		}

		main_status("system_milliseconds", "time %d", system_milliseconds());
		main_status_dump(&crash_info_output_file);

		// dump last accessed tag and resource owner
		c_static_string<1024> last_accessed;
		char tag_group[8]{};
		REFERENCE_DECLARE(0x0190E460, int32, last_resource_owner);

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

		if (last_resource_owner != NONE && g_cache_file_globals.resource_gestalt != NULL)
		{
			for (s_cache_file_tag_resource_data* resource : g_cache_file_globals.resource_gestalt->resources)
			{
				if (resource->runtime_data.owner_tag.index == last_resource_owner)
				{
					tag_to_string(resource->runtime_data.owner_tag.group_tag, tag_group);
					last_accessed.print_line("last accessed resource owner:\r\n    ['%s', %04X] '%s.%s'",
						tag_group,
						resource->runtime_data.owner_tag.index,
						resource->runtime_data.owner_tag.get_name(),
						resource->runtime_data.owner_tag.get_group_name());

					break;
				}
			}
			file_write(&crash_info_output_file, last_accessed.length(), last_accessed.get_string());
		}

		file_close(&crash_info_output_file);
	}
}

c_tag_resources_game_lock::c_tag_resources_game_lock() :
	m_resource_key(tag_resources_lock_game())
{
}

c_tag_resources_game_lock::~c_tag_resources_game_lock()
{
	tag_resources_unlock_game(m_resource_key);
}

bool debug_trace_main_events = false;

void __cdecl main_trace_event_internal(const char* function_name)
{
	if (debug_trace_main_events)
	{
		c_event_context local_event_context("", false, new c_event_context_string_builder("%s", function_name));
		stack_walk(1);
	}
}

