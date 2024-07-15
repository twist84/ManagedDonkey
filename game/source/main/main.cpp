#include "main/main.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
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
#include "interface/closed_caption.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/console.hpp"
#include "main/loading.hpp"
#include "main/main_game.hpp"
#include "main/main_predict.hpp"
#include "main/main_render.hpp"
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
#include "screenshots/screenshots_uploader.hpp"
#include "shell/shell_windows.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"
#include "spartan_program/spartan_program_handler.hpp"
#include "tag_files/tag_files_sync.hpp"
#include "test/test_globals.hpp"
#include "text/font_cache.hpp"
#include "text/font_loading.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189D044, bool, g_force_upload_even_if_untracked);
REFERENCE_DECLARE(0x022AE4F0, bool, g_render_thread_user_setting);
REFERENCE_DECLARE(0x022AE4F2, bool, disable_main_loop_throttle);
REFERENCE_DECLARE_ARRAY(0x022AE4F8, s_main_status_value, g_status_values, 32);
REFERENCE_DECLARE(0x022B456C, c_interlocked_long, g_render_thread_waiting);
REFERENCE_DECLARE(0x022B46C8, c_interlocked_long, g_render_thread_enabled);
REFERENCE_DECLARE(0x022B471C, c_interlocked_long, g_single_thread_request_flags);
REFERENCE_DECLARE(0x022B4738, _main_globals, main_globals);

REFERENCE_DECLARE(0x022B47F0, bool, recursion_lock_triggered);
REFERENCE_DECLARE(0x022B47F1, bool, recursion_lock_triggered_while_exiting);

REFERENCE_DECLARE(0x0244DF07, bool, byte_244DF07);
REFERENCE_DECLARE(0x0244DF08, bool, byte_244DF08);

//HOOK_DECLARE(0x00504D20, _internal_halt_render_thread_and_lock_resources);
HOOK_DECLARE(0x00505530, main_events_pending);
HOOK_DECLARE(0x00505650, main_events_reset);
HOOK_DECLARE(0x005063A0, main_loop_pregame);
HOOK_DECLARE(0x00506460, main_loop_pregame_show_progress_screen);
HOOK_DECLARE(0x005065B0, main_loop_process_global_state_changes);
HOOK_DECLARE(0x00507450, process_published_game_state);

bool g_fake_minidump_creation = true;
bool g_suppress_keyboard_for_minidump = false;
char const* const k_crash_info_output_filename = "crash_report\\crash_info.txt";

bool debug_console_pauses_game = true;
bool debug_no_drawing = false;

bool cheat_drop_tag = false;
long cheat_drop_tag_index = 0;
long cheat_drop_variant_name = 0;
s_model_customization_region_permutation cheat_drop_permutations[16]{};
long cheat_drop_permutation_count = 0;

bool main_loop_suspended = false;

bool main_loop_is_suspended()
{
	return main_loop_suspended;
}

void main_loop_suspend()
{
	if (!main_loop_suspended)
		main_loop_suspended = true;
}

void main_loop_resume()
{
	if (main_loop_suspended)
		main_loop_suspended = false;
}

void __cdecl __tls_set_g_main_gamestate_timing_data_allocator(void* address)
{
	INVOKE(0x00504CE0, __tls_set_g_main_gamestate_timing_data_allocator, address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	//g_main_gamestate_timing_data = (s_game_tick_time_samples*)address;
}

void __cdecl __tls_set_g_main_render_timing_data_allocator(void* address)
{
	INVOKE(0x00504D00, __tls_set_g_main_render_timing_data_allocator, address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	//g_main_render_timing_data = (s_game_tick_time_samples*)address;
}

dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line)
{
	return INVOKE(0x00504D20, _internal_halt_render_thread_and_lock_resources, file, line);

	//if (!game_is_multithreaded())
	//	return 0;
	//
	//dword result = 0;
	//
	//PROFILER(internal_halt_render_thread)
	//{
	//	if (render_thread_get_mode() != 2 && thread_system_initialized() && restricted_region_valid(k_game_state_render_region) && get_current_thread_index() != k_thread_render)
	//	{
	//		result = render_thread_set_mode(1, 0) ? FLAG(1) : 0;
	//
	//		if (!restricted_region_locked_for_current_thread(k_game_state_render_region))
	//		{
	//			restricted_region_lock_primary(k_game_state_render_region);
	//			restricted_region_lock_primary(k_global_render_data_region);
	//			c_rasterizer::rasterizer_device_acquire_thread();
	//
	//			result |= FLAG(0);
	//
	//			if (!thread_has_crashed(k_thread_render))
	//			{
	//				restricted_region_unlock_primary(k_game_state_shared_region);
	//				for (long i = restricted_region_get_mirror_count(k_game_state_shared_region); i; --i)
	//				{
	//					if (restricted_region_lock_mirror(k_game_state_shared_region))
	//					{
	//						{
	//							c_tag_resources_game_lock game_lock{};
	//							process_published_game_state(false);
	//						}
	//
	//						if (restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region))
	//							restricted_region_unlock_mirror(k_game_state_shared_region);
	//					}
	//				}
	//			}
	//		}
	//
	//		if (get_current_thread_index() == k_game_state_update_region && restricted_region_locked_for_current_thread(k_game_state_update_region))
	//		{
	//			main_thread_process_pending_messages();
	//		}
	//	}
	//}
	//
	//return result;
}

dword __cdecl audio_thread_loop(void* thread_parameter)
{
	return INVOKE(0x00504F80, audio_thread_loop, thread_parameter);

	//if (game_is_multithreaded() && !current_thread_should_exit())
	//{
	//	do
	//	{
	//		current_thread_update_test_functions();
	//		sound_render_dispatch();
	//	} while (!current_thread_should_exit());
	//}
	//
	//return 1;
}

//.text:00505170 ; public: bool __cdecl s_scenario_zone_activation::is_empty() const

void __cdecl main_activate_cinematic_tag_private()
{
	INVOKE(0x00505190, main_activate_cinematic_tag_private);

	//main_globals.activate_cinematic_zone_from_tag = false;
}

void __cdecl main_activate_cinematic_zone(long cinematic_zone_index)
{
	INVOKE(0x005051A0, main_activate_cinematic_zone, cinematic_zone_index);
}

void __cdecl main_activate_cinematic_zone_for_debugging(long cinematic_zone_index)
{
	INVOKE(0x00505220, main_activate_cinematic_zone_for_debugging, cinematic_zone_index);
}

void __cdecl main_activate_designer_zone(long designer_zone_index)
{
	INVOKE(0x005052D0, main_activate_designer_zone, designer_zone_index);
}

//.text:00505370 ; void __cdecl sub_505370() // saber function called within `c_rasterizer::cleanup_before_device_reset`

void __cdecl main_clear_global_pending_zone_activation(long game_state_proc_flags)
{
	INVOKE(0x00505380, main_clear_global_pending_zone_activation, game_state_proc_flags);

	//if (main_globals.prepare_to_switch_zone_set)
	//{
	//	main_globals.prepare_to_switch_zone_set = false;
	//	main_globals.prepare_to_switch_zone_set_index = NONE;
	//}
	//
	//if (main_globals.switch_zone_set)
	//{
	//	main_globals.switch_zone_set = false;
	//	main_globals.switch_zone_set_index = NONE;
	//}
	//
	//if (main_globals.non_bsp_zone_activation)
	//{
	//	main_globals.non_bsp_zone_activation = false;
	//	main_globals.scenario_zone_activation.clear();
	//}
}

void __cdecl main_deactivate_cinematic_tag_private()
{
	INVOKE(0x005053F0, main_deactivate_cinematic_tag_private);

	//main_globals.deactivate_cinematic_zone_from_tag = false;
	//scenario_set_and_activate_runtime_cinematic_tag(NONE);
}

void __cdecl main_deactivate_cinematic_zone(long cinematic_zone_index)
{
	INVOKE(0x00505400, main_deactivate_cinematic_zone, cinematic_zone_index);
}

void __cdecl main_deactivate_designer_zone(long designer_zone_index)
{
	INVOKE(0x00505470, main_deactivate_designer_zone, designer_zone_index);
}

void __cdecl main_decompress_gamestate()
{
	INVOKE(0x005054F0, main_decompress_gamestate);

	//if (!main_globals.game_state_decompression)
	//	main_globals.game_state_decompression = true;
}

//.text:00505510
void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool* variable)
{
	ASSERT(VALID_INDEX(reason, k_number_of_main_reset_event_reasons));

	if (*variable == true)
	{
		generate_event(_event_level_warning, "main:events: ignoring %s due to %s", name, k_main_event_reason_description[reason]);
		*variable = false;
	}
}

//.text:00505520
void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool volatile* variable)
{
	ASSERT(VALID_INDEX(reason, k_number_of_main_reset_event_reasons));

	if (*variable == true)
	{
		generate_event(_event_level_warning, "main:events: ignoring %s due to %s", name, k_main_event_reason_description[reason]);
		*variable = false;
	}
}

bool __cdecl main_events_pending()
{
	//return INVOKE(0x00505530, main_events_pending);

	bool result = main_game_change_in_progress();
	if (game_in_editor())
	{
		if (main_globals.reset_zone_resources || main_globals.switch_zone_set || main_globals.map_reset)
			result = true;
	}
	else if (main_globals.skip_cinematic
		|| main_globals.map_reset
		|| main_globals.map_revert
		|| main_globals.deactivate_cinematic_zone_from_tag
		|| main_globals.activate_cinematic_zone_from_tag
		|| main_globals.game_state_decompression
		|| game_state_compressor_lock_pending()
		|| main_globals.reset_zone_resources
		|| main_globals.prepare_to_switch_zone_set
		|| main_globals.switch_zone_set
		|| main_globals.save
		|| main_globals.save_and_exit
		|| main_globals.save_core
		|| main_globals.load_core
		|| main_globals.user_interface_save_files
		|| main_globals.reloading_active_zone_set
		|| main_globals.non_bsp_zone_activation
		|| cache_file_tag_resources_prefetch_update_required()
		|| texture_cache_is_blocking()
		|| geometry_cache_is_blocking()
		|| byte_244DF08 /* sub_6103F0 */)
	{
		result = true;
	}

	return result;
}

char const* const k_main_event_reason_description[k_number_of_main_reset_event_reasons]
{
	"changing the map",
	"xsync in progress"
};

void __cdecl main_events_reset(e_main_reset_events_reason reason)
{
	//INVOKE(0x00505650, main_events_reset, reason);

	main_event_reset_internal("skip cinematic", reason, &main_globals.skip_cinematic);
	main_event_reset_internal("map reset", reason, &main_globals.map_reset);
	main_event_reset_internal("map revert", reason, &main_globals.map_revert);
	main_globals.map_revert_flags.clear();
	main_event_reset_internal("activate cinematic tag", reason, &main_globals.activate_cinematic_tag);
	main_event_reset_internal("game state decompression", reason, &main_globals.game_state_decompression);
	main_event_reset_internal("reset zone resources", reason, &main_globals.reset_zone_resources);
	main_event_reset_internal("prepare to switch zone set", reason, &main_globals.prepare_to_switch_zone_set);
	main_event_reset_internal("switch zone set", reason, &main_globals.switch_zone_set);
	main_event_reset_internal("save", reason, &main_globals.save);
	main_event_reset_internal("save and exit", reason, &main_globals.save_and_exit);
	main_event_reset_internal("reloading active zone set", reason, &main_globals.reloading_active_zone_set);
	main_event_reset_internal("non-bsp zone activation", reason, &main_globals.non_bsp_zone_activation);
	main_globals.scenario_zone_activation.clear();
	//main_event_reset_internal("cheat drop tag", reason, &main_globals.cheat_drop_tag);
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
	//	generate_event(_event_level_error, "main_game: game_state_compressor failed to decompress and load gamestate.");
	//};
	//main_globals.game_state_decompression = false;
}

bool __cdecl main_game_is_exiting()
{
	//return INVOKE(0x00505700, main_game_is_exiting);

	return main_globals.exit_game;
}

void __cdecl main_loop_pregame_halt_and_catch_fire_push()
{
	if (is_main_thread())
		main_globals.main_loop_pregame_entered++;
}

void __cdecl main_loop_pregame_halt_and_catch_fire_pop()
{
	if (is_main_thread())
		main_globals.main_loop_pregame_entered--;
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

#if defined(_DEBUG)
// crash and burn outside of debug
HOOK_DECLARE(0x00505710, main_halt_and_catch_fire);
#endif
void __cdecl main_halt_and_catch_fire()
{
	//INVOKE(0x00505710, main_halt_and_catch_fire);

	static c_static_string<24576> text{};

	main_loop_pregame_halt_and_catch_fire_push();
	if (recursion_lock_triggered)
	{
		if (recursion_lock_triggered_while_exiting)
		{
			generate_event(_event_level_warning, "crash: recursion lock triggered!");
			exit(NONE);
		}
		else
		{
			recursion_lock_triggered_while_exiting = true;
			generate_event(_event_level_critical, "crash: ### CATASTROPHIC ERROR: halt_and_catch_fire: recursion lock triggered while exiting! (Someone probably smashed memory all to bits)");
			while (!is_debugger_present());
			abort();
		}
	}
	else
	{
		main_loop_pregame_halt_and_catch_fire_push();
		release_locks_safe_for_crash_release();

		dword lock_time = system_milliseconds();

		bool upload_debug_started = false;
		bool upload_debug_completed = false;
		bool upload_debug_success = false;
		bool create_fake_minidump = false;

		recursion_lock_triggered = true;

		generate_event(_event_level_warning, "lifecycle: CRASH");
		main_status("system_milliseconds", "time %d", system_milliseconds());
		main_status_dump(NULL);

		font_initialize_emergency();
		online_process_debug_output_queue();
		transport_initialize();
		input_clear_all_rumblers();
		progress_set_default_callbacks(NULL);
		c_online_lsp_manager::get()->go_into_crash_mode();
		network_webstats_force_reset();
		user_interface_networking_set_ui_upload_quota(NONE);
		console_close();

		if (false /* #TODO: add variable */)
			main_crash_just_upload_dammit();

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
			c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
		
			dword this_loop_time = system_milliseconds();
			dword time_delta = this_loop_time - lock_time;
			real shell_seconds_elapsed = time_delta / 1000.0f;
			lock_time = this_loop_time;
		
			c_rasterizer::begin_frame();
			c_rasterizer::setup_targets_simple();
		
			text.set(events_get());

			char upload_debug_output[1024]{};
			if (upload_debug_started && upload_debug_get_output(upload_debug_output, 1024))
				text.append(upload_debug_output);

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
			//			long description_length = 0;
			//			char description[256]{};
			//			wchar_string_to_ascii_string(description_wide, description, 256, &description_length);
			//			if (description_length > 1)
			//				crash_user_input_upload_now(description, description_length);
			//		}
			//	}
			//}
		
			e_main_pregame_frame pregame_frame_type = _main_pregame_frame_minidump_upload_waiting;
			if (upload_to_server)
			{
				if (upload_debug_completed && upload_debug_success)
				{
					pregame_frame_type = _main_pregame_frame_minidump_upload_completed_successfully;
				}
				else if (!upload_debug_completed)
				{
					if (upload_debug_started)
					{
						if ((system_seconds() & 1) != 0)
							text.append("\r\n\r\nDO NOT REBOOT YOUR CONSOLE\r\nWE NEED THE CRASH LOGS!1!!");
						else
							text.append("\r\n\r\nEACH TIME YOU REBOOT A CRASH\r\n  A BABY KITTEN DIES!1!!");
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

			main_render_pregame(pregame_frame_type, text.get_string());
			c_rasterizer::end_frame();
			overlapped_update();
			input_update();
			terminal_update(shell_seconds_elapsed);
			online_lsp_update();
			data_mine_update();
			network_webstats_update();
			transport_global_update();
			online_process_debug_output_queue();

			if (upload_debug_started && !upload_debug_completed)
				upload_debug_completed = upload_debug_complete(&upload_debug_success);
		
			sleep(0);
		}

		// by setting the `emergency_mode` here to `false` we enable fonts again
		{
			c_font_cache_scope_lock scope_lock{};
			g_font_globals.emergency_mode = false;
		}

		recursion_lock_triggered = false;
	}
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

void __cdecl main_load_core_name(char const* core_name)
{
	//INVOKE(0x005059A0, main_load_core_name, core_name);

	main_globals.load_core = true;
	main_globals.core_name.set(core_name);
	director_notify_map_reset();
}

void __cdecl main_load_core_private()
{
	//INVOKE(0x005059C0, main_load_core_private);

	if (game_in_progress())
	{
		game_state_load_core(main_globals.core_name.get_string());
		main_globals.load_core = false;
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

//void main_loop_body(c_flags<e_main_loop_body_part_flags, byte, _main_loop_body_part_flags_count> parts_to_run, dword* wait_for_render_thread, __int64* vblank_index) // debug?
void __cdecl main_loop_body(dword* wait_for_render_thread, dword* time)
{
	dword time_delta = system_milliseconds() - *time;
	if (!disable_main_loop_throttle && time_delta < 7)
		sleep(7 - time_delta);
	*time = system_milliseconds();

	bool requested_single_thread = false;
	main_globals.main_loop_time = system_milliseconds();

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

	g_single_thread_request_flags.set_bit(3, byte_244DF08 /* sub_6103F0 */ || byte_244DF07 /* sub_610530 */);
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

void __cdecl main_loop()
{
	//INVOKE(0x005059E0, main_loop);

	if (game_is_multithreaded())
	{
		g_render_thread_user_setting = true;
		g_render_thread_enabled.set(true);
	}

	main_loop_enter();

	dword wait_for_render_thread = 0;
	dword time = system_milliseconds();
	while (!main_globals.exit_game)
	{
		if (main_loop_is_suspended())
		{
			switch_to_thread();
		}
		else
		{
			main_loop_body(&wait_for_render_thread, &time);
		}
	}

	main_loop_exit();
}

// functions for `main_loop_body_main_part`
void __cdecl sub_5077E0()
{
	INVOKE(0x005077E0, sub_5077E0);
}

void __cdecl sub_641A60(real shell_seconds_elapsed)
{
	INVOKE(0x00641A60, sub_641A60, shell_seconds_elapsed);
}

void __cdecl main_loop_body_main_part()
{
	//INVOKE(0x00505C10, main_loop_body_main_part);

	TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	
	PROFILER(main_loop_body_main_part)
	{
		// we no longer hook calls from `main_loop_body_main_part` for this
		test_main_loop_body_begin();
	
		PROFILER(main, main_loop)
		{
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
			//	main_loop_process_global_state_changes();
			//else
			//	main_events_reset(_main_reset_events_reason_xsync_in_progress);
	
			PROFILER(idle_tag_resources) // main_loop, tag_idle
			{
				tag_resources_main_loop_idle();
			}
	
			// main_loop, main_memory
			//main_verify_memory();
			//main_dump_memory();
	
			if (shell_application_is_paused())
			{
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit1, true);
			}
			else
			{
				PROFILER(main_shell) // main_loop, shell
				{
					real shell_seconds_elapsed = 0.0f;
					real world_seconds_elapsed = 0.0f;
					real game_seconds_elapsed = 0.0f;
					long game_ticks_elapsed = 0;
	
					// main_loop, wait
					shell_seconds_elapsed = main_time_update();
	
					sub_641A60(shell_seconds_elapsed);
					main_time_frame_rate_debug();
	
					// main_loop, unittest
					//unit_test_update();
	
					overlapped_update();
	
					PROFILER(update_console_terminal_and_debug_menu) // main_loop, con
					{
						c_tag_resources_game_lock game_lock{};
	
						if (cheat_drop_tag) // main_globals.cheat_drop_tag
							main_cheat_drop_tag_private();
	
						real seconds_elapsed = shell_seconds_elapsed;
						if (main_time_halted())
							seconds_elapsed = 0.0f;
	
						terminal_update(seconds_elapsed);
						console_update(seconds_elapsed);
						//telnet_console_update();
						//xbox_connection_update();
						remote_command_process();
						debug_menu_update();
						cinematic_debug_camera_control_update();
					}
	
					{
						c_tag_resources_game_lock game_lock{};
	
						if (main_time_halted())
							world_seconds_elapsed = 0.0f;
						else
							world_seconds_elapsed = shell_seconds_elapsed;
	
						g_main_gamestate_timing_data->shell_seconds_elapsed += shell_seconds_elapsed;
						g_main_gamestate_timing_data->world_seconds_elapsed += world_seconds_elapsed;
						g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit6, main_time_halted());
	
						PROFILER(update_ui) // main_loop, ui
						{
							user_interface_update(shell_seconds_elapsed);
							closed_caption_update();
							bink_playback_update();
							screenshots_uploader_update();
							spartan_program_handler_update();
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
								bool game_update_result = false;
								PROFILER(main_simulation_time_update)
								{
									game_update_result = game_time_update(world_seconds_elapsed, &game_seconds_elapsed, &game_ticks_elapsed);
								}
	
								g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit2, !game_update_result);
								g_main_gamestate_timing_data->game_seconds_elapsed += game_seconds_elapsed;
								g_main_gamestate_timing_data->game_ticks_elapsed += game_ticks_elapsed;
								g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit0, g_main_gamestate_timing_data->game_ticks_elapsed);
	
								//if (!g_main_gamestate_timing_data->game_ticks_elapsed)
								//	g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit0, user_interface_is_active());
	
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
										sound_idle();
									else
										sound_render();
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
	
								g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit5, true);
	
								if (main_time_halted())
									sound_idle();
								else
									sound_render();
	
								//static dword main_loop_network_time_since_abort = 0;
								//if (simulation_aborted())
								//{
								//	dword network_time_since_abort = simulation_get_network_time_since_abort();
								//	if (network_time_since_abort > 1000)
								//		network_time_since_abort /= 1000;
								//	else
								//		network_time_since_abort = 0;
								//
								//	if (network_time_since_abort >= 5)
								//	{
								//		if (network_time_since_abort >= main_loop_network_time_since_abort + 5)
								//		{
								//			main_loop_network_time_since_abort = network_time_since_abort;
								//			console_printf("simulation aborted %i seconds ago [%s]. Waiting for instructions...", network_time_since_abort, simulation_get_abort_reason_string());
								//		}
								//	}
								//	else
								//	{
								//		main_loop_network_time_since_abort = 0;
								//	}
								//}
								//else
								//{
								//	main_loop_network_time_since_abort = 0;
								//}
							}
						}

						hf2p_game_update();
					}
				}
			}
	
			if (game_time_get_paused())
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit3, true);
	
			if (game_time_initialized() && fabsf(game_time_get_speed() - 1.0f) > k_real_epsilon)
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit7, true);
	
			if (game_in_progress() && game_is_playback() && !game_is_authoritative_playback())
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit10, true);
	
			if (g_main_gamestate_timing_data->world_seconds_elapsed < 0.06666667f)
			{
				if (user_interface_requests_unlocked_framerate())
				{
					g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit11, true);
				}
				else if (game_in_progress() && game_is_playback())
				{
					if (g_main_gamestate_timing_data->world_seconds_elapsed > 0.0f && g_main_gamestate_timing_data->world_seconds_elapsed < 0.025f // 0.0083333338 in h3ek
						&& !g_main_gamestate_timing_data->flags.test(_game_tick_publishing_unknown_bit8)
						&& !g_main_gamestate_timing_data->flags.test(_game_tick_publishing_unknown_bit9)
						&& !g_main_gamestate_timing_data->flags.test(_game_tick_publishing_unknown_bit0))
					{
						g_main_gamestate_timing_data->flags.clear();
					}
				}
			}
			else
			{
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit11, true);
			}
	
			if (game_is_multithreaded())
				g_main_gamestate_timing_data->flags.set(_game_tick_publishing_unknown_bit8, !main_time_is_throttled());
	
			PROFILER(predict_tag_resources)
			{
				main_render_predict_tag_resources();
			}
		}
	
		exceptions_update();
		collision_log_end_frame();
	
		// we no longer hook calls from `main_loop_body_main_part` for this
		test_main_loop_body_end();
	}
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

	TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);

	PROFILER(single_thread_update)
	{
		main_loop_body_main_part();
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
							restricted_region_unlock_mirror(k_game_state_shared_region);
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

	TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);

	main_globals.suppress_startup_sequence = true;
	main_globals.has_performed_startup_sequence = false;

	main_loading_initialize();
	main_game_initialize();
	main_time_initialize();
	console_initialize();
	game_initialize();

	restricted_allocation_manager_reserve_memory(
		k_game_state_shared_region,
		__tls_set_g_main_gamestate_timing_data_allocator,
		main_thread_combine_timing_data,
		NULL,
		&g_main_gamestate_timing_data_allocator,
		"timing samples",
		"global",
		sizeof(s_game_tick_time_samples),
		0,
		g_main_gamestate_timing_data);

	restricted_allocation_manager_reserve_memory(
		k_game_state_shared_region,
		__tls_set_g_main_render_timing_data_allocator,
		NULL,
		NULL,
		&g_main_render_timing_data_allocator,
		"timing samples",
		"global",
		sizeof(s_game_tick_time_samples),
		0,
		g_main_render_timing_data);

	shell_halt_if_necessary();
	//tag_files_initialize_from_main();
	main_render_predict_initialize();
	console_execute_initial_commands();
	physical_memory_resize_region_initialize();

	g_main_gamestate_timing_data->initialize();
	g_main_render_timing_data->initialize();

	main_render_purge_pending_messages();
	main_time_mark_publishing_start_time();
	main_time_mark_publishing_end_time();

	if (game_is_multithreaded())
	{
		c_rasterizer::rasterizer_device_release_thread();
		start_thread(k_thread_render);
		thread_set_priority(k_thread_render, _thread_priority_above_normal);
		start_thread(k_thread_update);
	}
	start_thread(k_thread_audio);

	main_loop_initialize_restricted_regions();
}

void __cdecl main_loop_exit()
{
	//INVOKE(0x00506360, main_loop_exit);

	REFERENCE_DECLARE(0x02446530, bool, d3d_resource_allocator_dont_release);
	
	render_thread_set_mode(1, 0);
	main_loop_dispose_restricted_regions();
	
	if (game_is_multithreaded())
	{
		main_render_purge_pending_messages();
		wait_for_thread_to_exit(k_thread_render, 1);
	}
	
	d3d_resource_allocator_dont_release = true;
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

		if (!render_thread_get_mode())
			main_loop_pregame_show_progress_screen();

		//if (render_debug_initialized())
		//	render_debug_update();

		sound_idle();
		input_update();
		shell_idle();

		bink_playback_check_for_terminate_no_lock();
	}
}

void __cdecl main_loop_pregame()
{
	//INVOKE(0x005063A0, main_loop_pregame);

	if (is_main_thread())
	{
		dword current_time = system_milliseconds();
		bool bink_active = bink_playback_active();

		if (!main_globals.main_loop_pregame_entered
			&& main_globals.main_loop_time
			&& current_time - main_globals.main_loop_time >= dword(!bink_active ? 24 : 15))
		{
			main_globals.main_loop_pregame_entered++;
			if (bink_active)
				main_globals.main_loop_time = current_time;

			//main_loop_pregame_update_stack_high_water_mark();
			main_loop_pregame_do_work();

			if (!bink_active)
				main_globals.main_loop_time = system_milliseconds();
			main_globals.main_loop_pregame_entered--;
		}

		//main_globals.pregame_progress_screen_shown = 0;
	}
}

void __cdecl main_loop_pregame_disable(bool disable)
{
	INVOKE(0x00506430, main_loop_pregame_disable, disable);
}

void __cdecl main_loop_pregame_show_progress_screen()
{
	//INVOKE(0x00506460, main_loop_pregame_show_progress_screen);

	static c_static_wchar_string<12288> status_message;
	e_main_pregame_frame pregame_frame_type = main_loading_get_loading_status(&status_message);
	if (pregame_frame_type == _main_pregame_frame_none)
	{
		//editor_show_pregame_progress(main_pregame_frame, status_message.get_string());
		return;
	}

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	if (c_rasterizer::begin_frame())
	{
		c_rasterizer::setup_targets_simple();

		if (pregame_frame_type == _main_pregame_frame_loading_screen)
		{
			main_render_pregame_loading_screen();
		}
		else if (pregame_frame_type == _main_pregame_frame_status_message)
		{
			main_render_status_message(status_message.get_string());
		}
		else
		{
			static c_static_string<12288> status_message_ascii;
			status_message_ascii.clear();
			status_message_ascii.print("%ls", status_message.get_string());

			main_render_pregame(pregame_frame_type, status_message_ascii.get_string());

			if (pregame_frame_type == _main_pregame_frame_progress_report)
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
	//INVOKE(0x005065B0, main_loop_process_global_state_changes);

	if (game_in_editor())
	{
		c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	
		main_game_launch_default_editor();
		main_game_change_update();
	
		if (main_globals.skip_cinematic)
			main_globals.skip_cinematic = false;
	
		if (main_globals.map_revert)
			main_globals.map_revert = false;
	
		if (!main_globals.map_revert_flags.is_empty())
			main_globals.map_revert_flags.clear();
	
		if (main_globals.map_reset)
			main_reset_map_private();
	
		if (main_globals.deactivate_cinematic_zone_from_tag)
			main_deactivate_cinematic_tag_private();
	
		if (main_globals.activate_cinematic_zone_from_tag)
			main_activate_cinematic_tag_private();
	
		if (main_globals.reset_zone_resources)
			main_reset_zone_resources_private();
	
		if (main_globals.switch_zone_set)
			main_switch_zone_set_private();
	
		if (main_globals.save)
			main_globals.save = false;
	
		if (main_globals.save_and_exit)
			main_globals.save_and_exit = false;
	
		if (main_globals.save_core)
			main_globals.save_core = false;
	
		if (main_globals.load_core)
			main_globals.load_core = false;
	
		if (main_globals.user_interface_save_files)
			main_globals.user_interface_save_files = false;
	
		if (main_globals.reloading_active_zone_set)
			main_reload_active_zone_set_private();
	
		if (main_globals.non_bsp_zone_activation)
			main_modify_zone_activation_private();
	
		if (main_globals.game_state_decompression)
			main_globals.game_state_decompression = false;
	}
	else
	{
		if (main_globals.run_demos)
			main_run_demos_private();
	
		main_game_launch_default();
		main_game_change_update();
	
		if (main_events_pending())
		{
			c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	
			exceptions_update();
	
			if (main_globals.skip_cinematic)
				main_skip_cinematic_private();
	
			if (main_globals.map_revert)
				main_revert_map_private();
	
			if (main_globals.map_reset)
				main_reset_map_private();
	
			if (main_globals.deactivate_cinematic_zone_from_tag)
				main_deactivate_cinematic_tag_private();
	
			if (main_globals.activate_cinematic_zone_from_tag)
				main_activate_cinematic_tag_private();
	
			if (main_globals.game_state_decompression)
				main_game_gamestate_decompress_and_apply_private();
	
			if (game_state_compressor_lock_pending())
				game_state_compressor_lock_update();
	
			if (main_globals.reset_zone_resources)
				main_reset_zone_resources_private();
	
			if (main_globals.prepare_to_switch_zone_set)
				main_prepare_to_switch_zone_set_private();
	
			if (main_globals.switch_zone_set)
				main_switch_zone_set_private();
	
			if (main_globals.non_bsp_zone_activation)
				main_modify_zone_activation_private();
	
			if (main_globals.save)
				main_save_map_private();
	
			if (main_globals.save_and_exit)
				main_save_map_and_exit_private();
	
			if (main_globals.save_core)
				main_save_core_private();
	
			if (main_globals.load_core)
				main_load_core_private();
	
			if (main_globals.user_interface_save_files)
				main_user_interface_save_files_private();
	
			if (main_globals.reloading_active_zone_set)
				main_reload_active_zone_set_private();
	
			if (cache_file_tag_resources_prefetch_update_required())
				cache_file_tag_resources_update_prefetch_state();
		}
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

bool __cdecl main_menu_has_performed_startup_sequence()
{
	//return INVOKE(0x005069B0, main_menu_has_performed_startup_sequence);

	return main_globals.has_performed_startup_sequence;
}

void __cdecl main_modify_zone_activation_private()
{
	INVOKE(0x005069C0, main_modify_zone_activation_private);

	//main_globals.non_bsp_zone_activation = false;
	//if (game_in_progress())
	//	scenario_modify_active_zones(&main_globals.scenario_zone_activation);
	//main_globals.scenario_zone_activation.clear();
}

void __cdecl main_prepare_for_switch_zone_set(long zone_set_index)
{
	INVOKE(0x00506A10, main_prepare_for_switch_zone_set, zone_set_index);

	//if (global_scenario_try_and_get())
	//{
	//	if (zone_set_index >= 0 && zone_set_index < global_scenario->zone_sets.count())
	//	{
	//		if (zone_set_index == scenario_zone_set_index_get())
	//		{
	//			if (main_globals.prepare_to_switch_zone_set)
	//			{
	//				main_globals.prepare_to_switch_zone_set = false;
	//				main_globals.prepare_to_switch_zone_set_index = 0;
	//			}
	//			else
	//			{
	//				console_warning("tried to prepare to switch to current zone-set %d", zone_set_index);
	//			}
	//		}
	//		else
	//		{
	//			//main_trace_event_internal(__FUNCTION__);
	//			main_globals.prepare_to_switch_zone_set = true;
	//			main_globals.prepare_to_switch_zone_set_index = zone_set_index;
	//		}
	//	}
	//	else
	//	{
	//		console_warning("tried to switch to invalid zone-set %d", zone_set_index);
	//	}
	//}
	//else
	//{
	//	console_warning("tried to switch to a zone-set without a scenario loaded");
	//}
}

void __cdecl main_prepare_to_switch_zone_set_private()
{
	INVOKE(0x00506A70, main_prepare_to_switch_zone_set_private);

	//bool load_failed = false;
	//if (game_in_editor())
	//{
	//	load_failed = true;
	//}
	//else
	//{
	//	load_failed = scenario_prepare_to_switch_zone_set(main_globals.prepare_to_switch_zone_set_index);
	//	chud_messaging_special_load(false);
	//}
	//
	//if (!load_failed)
	//{
	//	generate_event(_event_level_error,
	//		"main_prepare_to_switch_zone_set() failed for '%s' zone set %d, must abort game",
	//		game_options_get()->scenario_path.get_string(),
	//		main_globals.switch_zone_set_index);
	//
	//	main_game_load_panic();
	//}
	//
	//main_globals.prepare_to_switch_zone_set = false;
	//main_globals.prepare_to_switch_zone_set_index = NONE;
}

void __cdecl main_print_version()
{
	INVOKE(0x00506AB0, main_print_version);
}

void __cdecl main_reload_active_zone_set_private()
{
	INVOKE(0x00506AE0, main_reload_active_zone_set_private);
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

	//main_globals.map_reset = true;
	//main_globals.map_reset_random = random;
}

void __cdecl main_reset_map_private()
{
	INVOKE(0x00506B20, main_reset_map_private);

	//if (game_in_progress() && !game_time_get_paused())
	//{
	//	main_clear_global_pending_zone_activation(NONE);
	//	main_game_reset_map(main_globals.map_reset_random);
	//	main_globals.map_reset = false;
	//	main_globals.map_reset_random = false;
	//}
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

	//main_globals.map_revert = true;
	//main_globals.map_revert_flags.set(_game_state_revert_bit_user, user);
}

void __cdecl main_revert_map_private()
{
	INVOKE(0x00506BD0, main_revert_map_private);
}

void __cdecl main_revert_map_scripting()
{
	INVOKE(0x00506CA0, main_revert_map_scripting);

	////main_trace_event_internal(__FUNCTION__);
	//main_globals.map_revert = true;
	//main_globals.map_revert_flags.clear();
	//main_globals.map_revert_flags.set(_game_state_revert_bit_scripting, true);
	//main_globals.map_revert_flags.set(_game_state_revert_bit_scripting_cinematic, true);
}

//.text:00506CC0

void __cdecl main_run_demos_private()
{
	INVOKE(0x00506CD0, main_run_demos_private);

	//main_globals.run_demos = false;
}

void __cdecl main_save_and_exit_campaign()
{
	INVOKE(0x00506CE0, main_save_and_exit_campaign);

	//if (game_in_progress() && game_is_campaign())
	//	user_interface_interactive_session_request_campaign_quit(2);
	//else
	//	main_save_and_exit_campaign_immediately();
}

void __cdecl main_save_and_exit_campaign_immediately()
{
	//INVOKE(0x00506D10, main_save_and_exit_campaign_immediately);

	main_globals.save_and_exit = true;
}

void __cdecl main_save_core()
{
	//INVOKE(0x00506D20, main_save_core);

	main_save_core_name("core");
}

void __cdecl main_save_core_name(char const* core_name)
{
	//INVOKE(0x00506D40, main_save_core_name, core_name);

	main_globals.save_core = true;
	main_globals.core_name.set(core_name);
}

void __cdecl main_save_core_private()
{
	//INVOKE(0x00506D60, main_save_core_private);

	if (game_in_progress())
	{
		game_state_save_core(main_globals.core_name.get_string());
		main_globals.save_core = false;
	}
}

void __cdecl main_save_map()
{
	INVOKE(0x00506D80, main_save_map);

	//main_globals.save = true;
	//chud_messaging_special_autosave(true);
}

void __cdecl main_save_map_and_exit_private()
{
	//INVOKE(0x00506D90, main_save_map_and_exit_private);

	if (!game_is_playback())
	{
		game_state_save_to_persistent_storage_blocking();
		game_finish();
	}
	main_globals.save_and_exit = false;
}

bool __cdecl main_save_map_pending()
{
	//return INVOKE(0x00506DB0, main_save_map_pending);

	return main_globals.save;
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
	//main_globals.save = false;
}

void __cdecl main_set_active_designer_zone_mask(dword a1)
{
	INVOKE(0x00506E50, main_set_active_designer_zone_mask, a1);
}

void __cdecl main_set_single_thread_request_flag(long single_threaded_request_flags, bool set)
{
	//INVOKE(0x00506EB0, main_set_single_thread_request_flag, single_threaded_request_flags, set);

	g_single_thread_request_flags.set_bit(single_threaded_request_flags, set);
}

void __cdecl main_skip_cinematic()
{
	INVOKE(0x00506ED0, main_skip_cinematic);
}

void __cdecl main_skip_cinematic_private()
{
	INVOKE(0x00506EF0, main_skip_cinematic_private);
}

bool __cdecl main_startup_sequence()
{
	return INVOKE(0x00506F10, main_startup_sequence);
}

// main_status in main.hpp
//.text:00506FB0 ; void __cdecl main_status(char const* status_type, char const* format, ...)

struct s_file_reference;
void __cdecl main_status_dump(s_file_reference* file)
{
	INVOKE(0x00507100, main_status_dump, file);
}

void __cdecl main_status_print()
{
	//INVOKE(0x005071C0, main_status_print);

	for (long i = 0; i < NUMBEROF(g_status_values); i++)
	{
		if (csstrnlen(g_status_values[i].status_type, NUMBEROF(g_status_values[i].status_type)))
			console_printf("%s: %s", g_status_values[i].status_type, g_status_values[i].status_value);
	}
}

void __cdecl main_suppress_startup_sequence()
{
	INVOKE(0x005071F0, main_suppress_startup_sequence);

	//main_globals.suppress_startup_sequence = false;
}

void __cdecl main_switch_bsp(long zone_set_index)
{
	INVOKE(0x00507200, main_switch_bsp, zone_set_index);

	//generate_event(_event_level_error, "switch bsp is a deprecated function. Use switch zone set instead.");
	//main_switch_zone_set(zone_set_index);
}

void __cdecl main_switch_zone_set(long zone_set_index)
{
	INVOKE(0x00507210, main_switch_zone_set, zone_set_index);
}

void __cdecl main_switch_zone_set_private()
{
	INVOKE(0x00507280, main_switch_zone_set_private);
}

void __cdecl main_thread_combine_timing_data(void* address)
{
	INVOKE(0x005072D0, main_thread_combine_timing_data, address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
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
		time_halted = true;

	return time_halted;
}

void __cdecl main_user_interface_save_files()
{
	INVOKE(0x00507380, main_user_interface_save_files);

	//if (!main_globals.user_interface_save_files)
	//{
	//	main_globals.user_interface_save_files = true;
	//	if (game_in_progress())
	//		chud_messaging_special_saving(true);
	//}
}

void __cdecl main_user_interface_save_files_private()
{
	INVOKE(0x005073B0, main_user_interface_save_files_private);

	//main_globals.user_interface_save_files = false;
	//if (game_in_progress())
	//	chud_messaging_special_saving(false);
}

void __cdecl process_published_game_state(bool a1)
{
	//INVOKE(0x00507450, process_published_game_state, a1);

	TLS_DATA_GET_VALUE_REFERENCE(g_main_gamestate_timing_data);
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	
	PROFILER(process_published_game_state)
	{
		g_main_render_timing_data->accum(g_main_gamestate_timing_data);
	
		if (a1 && c_rasterizer::rasterizer_thread_owns_device())
		{
			main_render();
			sub_604A20();
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
}

bool __cdecl render_thread_enabled()
{
	//return INVOKE(0x00507550, render_thread_enabled);

	if (game_is_multithreaded())
		return g_render_thread_enabled.peek();

	return false;
}

long __cdecl render_thread_get_mode()
{
	if (game_is_multithreaded())
		return g_render_thread_enabled.peek();

	return 0;
}

//bool __cdecl render_thread_set_mode(e_render_thread_mode mode_a, e_render_thread_mode mode_b)
bool __cdecl render_thread_set_mode(long mode_compare, long mode_exchange)
{
	//return INVOKE(0x005076D0, render_thread_set_mode, mode_compare, mode_exchange);

	return game_is_multithreaded() && g_render_thread_enabled.set_if_equal(mode_exchange, mode_compare) == mode_compare;
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
	file_reference_create_from_path(&crash_info_output_file, k_crash_info_output_filename, false);
	file_create_parent_directories_if_not_present(&crash_info_output_file);

	if (file_exists(&crash_info_output_file))
		file_delete(&crash_info_output_file);

	dword error = 0;
	if (file_create(&crash_info_output_file) && file_open(&crash_info_output_file, FLAG(_file_open_flag_desired_access_write), &error))
	{
		char const* _string = "stack:\r\n";
		file_write(&crash_info_output_file, strlen(_string), _string);

		if (context)
			stack_walk_with_context(&crash_info_output_file, 1, static_cast<_CONTEXT*>(context));
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

		if (last_resource_owner != NONE && g_cache_file_globals.resource_gestalt != NULL)
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

c_tag_resources_game_lock::c_tag_resources_game_lock() :
	lock(tag_resources_lock_game())
{
}

c_tag_resources_game_lock::~c_tag_resources_game_lock()
{
	tag_resources_unlock_game(lock);
}

#define NULL_BELONGS_TO_CHUCKY *(char const**)NULL = "chucky was here!  NULL belongs to me!!!!!"

static c_synchronized_long ill_never_be_done{};
e_async_completion main_crash_async(s_async_task* task, void* data, long data_size)
{
	NULL_BELONGS_TO_CHUCKY;
	return _async_completion_done;
}

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
		g_fake_minidump_creation = true;
		NULL_BELONGS_TO_CHUCKY;
	}
	else if (!csstricmp(type, "gpu") || !csstricmp(type, "halt"))
	{
		main_halt_and_catch_fire();
	}
	else if (!csstricmp(type, "async"))
	{
		async_queue_simple_callback(main_crash_async, NULL, 0, _async_priority_blocking_generic, &ill_never_be_done);
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
	cheat_drop_tag = true;
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
	cheat_drop_tag = false;

	cheat_drop_tag_in_main_event_loop(
		cheat_drop_tag_index,
		cheat_drop_variant_name,
		cheat_drop_permutations,
		cheat_drop_permutation_count);

	cheat_drop_tag_index = NONE;
	cheat_drop_variant_name = NONE;
	cheat_drop_permutation_count = 0;
}

