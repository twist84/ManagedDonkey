#include "main/main_game.hpp"

#include "bink/bink_playback.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_sound_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "camera/director.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "editor/editor_stubs.hpp"
#include "effects/screen_effect.hpp"
#include "game/campaign_metagame.hpp"
#include "game/game.hpp"
#include "game/game_progression.hpp"
#include "input/input_windows.hpp"
#include "interface/damaged_media.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "main/global_preferences.hpp"
#include "main/levels.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "main/main_game_launch.hpp"
#include "main/main_predict.hpp"
#include "main/main_render.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/network_memory.hpp"
#include "networking/online/online_files.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation.hpp"

REFERENCE_DECLARE_ARRAY(0x016587D8, const s_configure_memory, g_configure_memory_procs, 8);
REFERENCE_DECLARE(0x023916D8, s_main_game_globals, main_game_globals);
REFERENCE_DECLARE(0x023DAE90, bool, load_panic_recursion_lock);

HOOK_DECLARE(0x00566EF0, main_game_change_immediate);
//HOOK_DECLARE(0x00567230, main_game_configure_map_memory_pop);
//HOOK_DECLARE(0x00567270, main_game_configure_map_memory_push);
HOOK_DECLARE(0x00567670, main_game_pregame_blocking_load);
HOOK_DECLARE(0x00567AD0, main_game_load_panic);
HOOK_DECLARE(0x00567C10, main_game_request_level_advance_spoke);
HOOK_DECLARE(0x00567C30, main_game_request_level_advance);
HOOK_DECLARE(0x00567C50, main_game_request_level_advance_hub);
HOOK_DECLARE(0x00567C70, main_game_request_level_advance_normal);
HOOK_DECLARE(0x00567E40, main_game_start);

bool debug_load_panic_to_main_menu = true;

//const s_configure_memory g_configure_memory_procs[8]
//{
//	{ optional_cache_memory_dispose, optional_cache_memory_initialize },
//	{ network_memory_shared_dispose, network_memory_shared_initialize },
//	{ user_interface_memory_dispose, user_interface_memory_initialize },
//	{ user_interface_networking_memory_dispose, user_interface_networking_memory_initialize },
//	{ online_files_memory_dispose, online_files_memory_initialize },
//	{ bink_playback_memory_dispose, bink_playback_memory_initialize },
//	{ NULL, NULL /*saved_film_manager_memory_dispose, saved_film_manager_memory_initialize*/ },
//	{ screenshots_uploader_memory_dispose, screenshots_uploader_memory_initialize },
//};

//.text:00566A80 ; unknown destructor
//.text:00566AD0 ; unknown destructor

e_map_memory_configuration __cdecl compute_desired_map_memory_configuration(const game_options* options)
{
	return INVOKE(0x00566B30, compute_desired_map_memory_configuration, options);

	//if (options)
	//{
	//	e_game_mode game_mode = (e_game_mode)options->game_mode;
	//	switch (game_mode)
	//	{
	//	case _game_mode_campaign:
	//	{
	//		if (options->game_playback)
	//		{
	//			return _map_memory_configuration_campaign_saved_film;
	//		}
	//		else if (options->map_id == 4200) // ODST: C200, COASTAL HIGHWAY
	//		{
	//			return _map_memory_configuration_campaign_epilogue;
	//		}
	//
	//		return _map_memory_configuration_campaign;
	//	}
	//	break;
	//	case _game_mode_multiplayer:
	//	{
	//		if (options->game_playback)
	//		{
	//			return _map_memory_configuration_multiplayer_saved_film;
	//		}
	//
	//		return _map_memory_configuration_multiplayer;
	//	}
	//	case _game_mode_ui_shell:
	//	{
	//		return _map_memory_configuration_main_menu;
	//	}
	//	break;
	//	default:
	//	{
	//		return _map_memory_configuration_none;
	//	}
	//	break;
	//	}
	//}
	//
	//return _map_memory_configuration_none;
}

//.text:00566B90 ; void __cdecl data_mine_insert_single_player_game_options(const char* event_name)

bool __cdecl sub_566CC0()
{
	// $TODO find name
	// used in `c_tag_resource_cache_controller::mark_necessary_resources`

	return INVOKE(0x00566CC0, sub_566CC0);

	//if (game_options_valid() && game_is_ui_shell())
	//{
	//	return true;
	//}
	//
	//if (!main_game_globals.map_change_pending || main_game_globals.map_change_pending_unload)
	//{
	//	return false;
	//}
	//
	//return main_game_globals.pending_game_options.game_mode == _game_mode_ui_shell;
}

void __cdecl main_game_campaign_loaded(const game_options* options)
{
	//INVOKE(0x00566D00, main_game_campaign_loaded, options);

	ASSERT(options);
	ASSERT(game_in_progress() && game_is_campaign());

	if (options->game_mode == _game_mode_campaign)
	{
		if (options->map_id != _map_id_none)
		{
			user_interface_set_last_campaign_level(options->campaign_id, options->map_id);
		}
	}

	if (options->game_playback == _game_playback_none && !game_is_or_was_cooperative())
	{
		e_controller_index controller_index = user_interface_get_reload_from_persistent_storage();
		if (controller_index != k_no_controller)
		{
			game_state_try_and_load_from_persistent_storage(controller_index);
			user_interface_set_reload_from_persistent_storage(k_no_controller);
		}
	}
}

void __cdecl main_game_change(const game_options* options)
{
	//INVOKE(0x00566E70, main_game_change, options);

	if (options)
	{
		assert_game_options_verify(options);
		csmemcpy(&main_game_globals.pending_game_options, options, sizeof(game_options));
	}

	main_game_globals.map_change_pending = true;
	main_game_globals.map_change_pending_unload = options == NULL;
	main_game_globals.map_change_initiate_time = system_milliseconds();

	if (!options)
	{
		if (network_life_cycle_get_state() != _life_cycle_state_none)
		{
			network_life_cycle_request_leave(false);
		}
		else
		{
			network_life_cycle_end();
		}
	}
}

void __cdecl main_game_change_abort()
{
	INVOKE(0x00566EE0, main_game_change_abort);

	//main_game_globals.map_change_pending = false;
}

bool __cdecl main_game_change_immediate(const game_options* options)
{
	//return INVOKE(0x00566EF0, main_game_change_immediate, options);

	bool result = false;
	RENDER_THREAD_LOCK;

	main_render_purge_pending_messages();
	main_render_assert_no_pending_messages();
	main_game_unload_and_prepare_for_next_game(options);
	main_render_assert_no_pending_messages();
	main_events_reset(_main_reset_events_map_change);
	main_render_assert_no_pending_messages();
	main_status("map", NULL);

	if (options)
	{
		events_clear();
		input_flush();

		main_status("map", "loading %s", options->scenario_path.get_string());

		assert_game_options_verify(options);
		//determinism_debug_manager_game_start();

		main_render_assert_no_pending_messages();
		if (main_game_load_map(options) && (main_render_assert_no_pending_messages(), main_game_start(options)))
		{
			main_status("map", "loaded %s", options->scenario_path.get_string());
			main_status("minor_version", "%i", get_map_minor_version());

			event(_event_message, "lifecycle: MAP-LOADED %s", options->scenario_path.get_string());

			//c_datamine datamine(0, "map l oaded", 2, "main", "game");
			//data_mine_usability_add_basic_information(&datamine);
			//datamine.add_string("map", options->scenario_path.get_string());

			global_preferences_flush();

			switch (options->game_mode)
			{
			case _game_mode_campaign:
			{
				//data_mine_insert_single_player_game_options("game start");
				main_game_campaign_loaded(options);
			}
			break;
			case _game_mode_ui_shell:
			{
				//data_mine_upload();
				if (!main_startup_sequence())
				{
					user_interface_enter_game_shell();
				}

				online_guide_delay_toasts(0);
			}
			break;
			case _game_mode_multiplayer:
			{
				const char* game_engine_name = game_engine_type_get_string(options->multiplayer_variant.get_game_engine_index());
				const char* game_variant_name = options->multiplayer_variant.get_active_variant()->get_name();

				event(_event_message, "lifecycle: MULTIPLAYER-GAME %s", game_engine_name);
				event(_event_message, "lifecycle: MULTIPLAYER-VARIANT %s", game_variant_name);
			}
			break;
			}

			if (main_game_globals.load_core_on_game_launch)
			{
				main_load_core_name(main_game_globals.core_name.get_string());
				main_game_globals.load_core_on_game_launch = false;
			}

			result = true;
		}
		else
		{
			main_status("map", "load-failed %s", options->scenario_path.get_string());

			event(_event_critical, "main_game_change_immediate() failed for '%s', cannot load game", options->scenario_path.get_string());

			main_game_load_panic();
		}
	}
	else
	{
		main_game_internal_pregame_load();
		result = true;
	}

	//if (!success)
	//	main_game_cleanup_loading_screen();

	ASSERT(main_game_loaded_map() || main_game_loaded_pregame());

	// I do not like this, I do not!
	{
		apply_player_representation_fixup();
		apply_global_screen_effect();
	}

	return result;
}

bool __cdecl main_game_change_in_progress()
{
	//return INVOKE(0x005670E0, main_game_change_in_progress);

	return main_game_globals.map_change_pending;
}

void __cdecl main_game_change_update()
{
	//INVOKE(0x005670F0, main_game_change_update);

	//main_game_editor_world_controller_update();

	if (main_game_globals.map_advance_pending && !metagame_postgame_in_progress())
	{
		bool session_is_leader = false;
		if (game_is_playback())
		{
			metagame_postgame_dismiss_ui();
		}
		else if ((!game_in_progress() || !game_is_campaign() || (main_game_goto_next_level())
			&& (!network_squad_session_controls_coop_game_options(&session_is_leader) || session_is_leader)))
		{
			event(_event_message, "networking:main_game: congratulations, you won the game! or, the next map failed to load");

			if (game_in_progress() && game_is_ui_shell())
			{
				event(_event_message, "networking:main_game: already in the ui, not ending the simulation");
			}
			else
			{
				simulation_end(_simulation_abort_reason_failed_to_find_next_level);
			}

			if (!user_interface_reset_networking_to_pregame())
			{
				main_menu_launch();
			}
		}

		main_game_globals.map_advance_pending = false;
	}

	if (main_game_globals.map_change_pending)
	{
		game_options* pending_game_options = &main_game_globals.pending_game_options;
		if (main_game_globals.map_change_pending_unload && !main_game_loaded_pregame())
		{
			pending_game_options = NULL;
		}
		main_game_change_immediate(pending_game_options);

		sub_5129B0();

		main_game_globals.map_change_pending = false;
		main_game_globals.map_change_pending_unload = false;
		main_game_globals.map_change_timer = 0;
	}
}

void __cdecl main_game_configure_map_memory(const game_options* options)
{
	//INVOKE(0x00567200, main_game_configure_map_memory, options);

	e_map_memory_configuration desired_memory_configuration = compute_desired_map_memory_configuration(options);
	ASSERT(VALID_INDEX(desired_memory_configuration, k_map_memory_configuration_count));

	if (main_game_globals.map_memory_configuration == _game_loaded_status_map_reloading)
	{
		ASSERT(options->load_level_only);
		ASSERT(desired_memory_configuration == main_game_globals.map_memory_configuration);
	}
	else if (main_game_globals.map_memory_configuration != desired_memory_configuration)
	{
		main_game_configure_map_memory_pop();
		main_game_configure_map_memory_push(desired_memory_configuration);
	}
}

void __cdecl main_game_configure_map_memory_pop()
{
	INVOKE(0x00567230, main_game_configure_map_memory_pop);

	//if (main_game_globals.map_memory_configuration <= _map_memory_configuration_none)
	//{
	//	return;
	//}
	//
	//for (int32 i = NUMBEROF(g_configure_memory_procs); i >= 0; i--)
	//{
	//	const s_configure_memory* configure_memory = &g_configure_memory_procs[i];
	//	if (configure_memory->dispose_memory_proc)
	//	{
	//		configure_memory->dispose_memory_proc();
	//	}
	//}
}

void __cdecl main_game_configure_map_memory_push(e_map_memory_configuration desired_memory_configuration)
{
	INVOKE(0x00567270, main_game_configure_map_memory_push, desired_memory_configuration);

	//if (desired_memory_configuration <= _map_memory_configuration_none)
	//{
	//	return;
	//}
	//
	//physical_memory_stage_push(_memory_stage_map_configuration);
	//main_game_globals.map_memory_configuration = desired_memory_configuration;
	//
	//for (int32 i = 0; i < NUMBEROF(g_configure_memory_procs); i++)
	//{
	//	const s_configure_memory* configure_memory = &g_configure_memory_procs[i];
	//	if (configure_memory->configure_memory_proc)
	//	{
	//		configure_memory->configure_memory_proc(desired_memory_configuration);
	//	}
	//}
}

bool __cdecl main_game_goto_next_level()
{
	return INVOKE(0x005672B0, main_game_goto_next_level);
}

//.text:00567440 ; network_squad_session_set_coop_game_options_internal

void __cdecl main_game_initialize()
{
	//INVOKE(0x005674B0, main_game_initialize);

	main_game_launch_initialize();
	main_game_globals.map_advance_pending = false;
}

void __cdecl main_game_internal_close_caches()
{
	//INVOKE(0x005674D0, main_game_internal_close_caches);

	tag_resources_prepare_for_new_map();
	cache_file_tag_resources_dispose_from_old_map();
	sound_cache_close();
	texture_cache_close();
	geometry_cache_close();
	scenario_language_pack_unload();
}

void __cdecl main_game_internal_map_load_abort(bool reload_map)
{
	//INVOKE(0x005674F0, main_game_internal_map_load_abort, reload_map);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_map_loading
		|| main_game_globals.game_loaded_status == _game_loaded_status_map_loaded_failure
		|| (reload_map && main_game_globals.game_loaded_status == _game_loaded_status_map_reloading));

	if (reload_map || main_game_globals.game_loaded_status == _game_loaded_status_map_loaded_failure)
	{
		main_game_internal_close_caches();
		scenario_unload();
	}

	physical_memory_stage_pop(_memory_stage_level_initialize);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

void __cdecl main_game_internal_map_load_begin(bool reload_map)
{
	//INVOKE(0x00567540, main_game_internal_map_load_begin, reload_map);

	ASSERT(main_game_globals.game_loaded_status == (reload_map ? _game_loaded_status_map_reloading : _game_loaded_status_none));

	if (!reload_map)
	{
		physical_memory_stage_push(_memory_stage_level_initialize);
	}

	main_game_globals.game_loaded_status = _game_loaded_status_map_loading;
}

bool __cdecl main_game_internal_map_load_complete(bool reload_map, const game_options* options)
{
	//return INVOKE(0x00567560, main_game_internal_map_load_complete, reload_map, options);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_map_loading);
	ASSERT(options);

	bool result = true;
	if (!reload_map)
	{
		result = main_game_internal_open_caches(options);
	}

	main_game_globals.game_loaded_status = _game_loaded_status_map_loaded;
	options->scenario_path.copy_to(main_game_globals.game_loaded_scenario_path, 260);

	return result;
}

void __cdecl main_game_internal_map_unload_begin()
{
	//INVOKE(0x005675D0, main_game_internal_map_unload_begin);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_map_loaded);
	ASSERT(!bink_playback_active());

	main_game_internal_close_caches();

	main_game_globals.game_loaded_status = _game_loaded_status_map_unloading;
	csstrnzcpy(main_game_globals.game_loaded_scenario_path, "", 260);
}

void __cdecl main_game_internal_map_unload_complete()
{
	//INVOKE(0x00567610, main_game_internal_map_unload_complete);

	physical_memory_stage_pop(_memory_stage_level_initialize);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

bool __cdecl main_game_internal_open_caches(const game_options* options)
{
	//return INVOKE(0x00567630, main_game_internal_open_caches, options);

	bool result = scenario_language_pack_load();
	geometry_cache_open();
	texture_cache_open();
	sound_cache_open();
	cache_file_tag_resources_initialize_for_new_map((e_game_mode)options->game_mode);
	main_predict_reset_for_map_reload();

	return result;
}

void __cdecl main_game_pregame_blocking_load()
{
	//INVOKE(0x00567670, main_game_pregame_blocking_load);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_none);

	main_game_configure_map_memory_push(_map_memory_configuration_main_menu);
	physical_memory_stage_push(_memory_stage_level_initialize);
	texture_cache_open_pregame();
	main_game_globals.game_loaded_status = _game_loaded_status_pregame;
}

void __cdecl main_game_internal_pregame_blocking_unload()
{
	//INVOKE(0x005676D0, main_game_internal_pregame_blocking_unload);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_pregame);

	texture_cache_close_pregame();
	physical_memory_stage_pop(_memory_stage_level_initialize);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

void __cdecl main_game_internal_pregame_load()
{
	//INVOKE(0x005676F0, main_game_internal_pregame_load);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_none);

	physical_memory_stage_push(_memory_stage_map_configuration);
	physical_memory_stage_push(_memory_stage_level_initialize);
	texture_cache_open_pregame();
	main_game_globals.game_loaded_status = _game_loaded_status_pregame;
}

void __cdecl main_game_internal_pregame_unload()
{
	//INVOKE(0x00567720, main_game_internal_pregame_unload);

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_pregame);

	texture_cache_close_pregame();
	physical_memory_stage_pop(_memory_stage_level_initialize);
	physical_memory_stage_pop(_memory_stage_map_configuration);
	main_game_globals.game_loaded_status = _game_loaded_status_none;
}

void __cdecl main_game_launch_default()
{
	//INVOKE(0x00567750, main_game_launch_default);

	if (!main_game_loaded_map() && !main_game_loaded_pregame())
	{
		{
			game_options options{};
			if (game_options_get_launch_settings(&options, main_game_globals.map_change_pending))
			{
				game_options_validate(&options);
				main_game_change(&options);
			}
		}

		if (main_game_globals.map_change_pending)
		{
			main_suppress_startup_sequence();
		}
		else if (game_in_editor())
		{
			VASSERT(0, "editor did not specify a level to load!");
		}
		else
		{
			main_menu_launch();
		}
	}
}

void __cdecl main_game_launch_default_editor()
{
	//INVOKE(0x00567820, main_game_launch_default_editor);

	if (!main_game_loaded_map() && !main_game_loaded_pregame())
	{
		const char* map_name = editor_get_map_name();
		main_game_launch(map_name);
	}
}

bool __cdecl main_game_load_blocking(const char* scenario_path)
{
	//return INVOKE(0x00567830, main_game_load_blocking, scenario_path);

	return main_load_map(scenario_path, 2);
}

void __cdecl main_tag_load_begin(int32 game_mode, int32 a2, const char* map_or_zone_name)
{
	ASSERT(map_or_zone_name);
	ASSERT(!loading_globals.tag_load_in_progress);

	//tag_load_missing_tags_reset();

	loading_globals.tag_load_in_progress = true;
	loading_globals.tag_load_time = system_milliseconds();
}

void __cdecl main_tag_load_end()
{
	ASSERT(loading_globals.tag_load_in_progress);

	if (shell_application_type() == _shell_application_tool)
	{
		tag_load_missing_tags_report();
	}

	loading_globals.tag_load_in_progress = false;
}

bool __cdecl main_game_load_map(const game_options* options)
{
	//return INVOKE(0x00567850, main_game_load_map, options);

	assert_game_options_verify(options);
	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_none);

	bool success = false;
	bool reload_map = false;
	bool map_loaded = false;

	const char* scenario_path = options->scenario_path.get_string();
	int32 map_load_status = main_load_map_status(scenario_path);

	if (map_load_status == 0 || map_load_status == 1)
	{
		main_game_unload_and_prepare_for_next_game(NULL);
		main_game_pregame_blocking_load();
	}

	if (!main_kick_startup_masking_sequence(true) || !bink_playback_active())
	{
		loading_basic_progress_enable(scenario_path, options->campaign_insertion_point);
	}

	if (levels_path_is_dlc(scenario_path))
	{
		levels_open_dlc(scenario_path, true);
	}

	if (map_load_status != _map_load_status_loaded)
	{
		if (map_load_status == 0 || map_load_status == 1)
		{
			map_loaded = main_game_load_blocking(scenario_path);
			main_game_internal_pregame_blocking_unload();
			main_game_unload_and_prepare_for_next_game(options);
		}
	}

	main_tag_load_begin(options->game_mode, NONE, scenario_path);
	main_game_internal_map_load_begin(reload_map);
	if (scenario_load(options->campaign_id, options->map_id, scenario_path))
	{
		if (main_game_internal_map_load_complete(reload_map, options))
		{
			success = scenario_preload_initial_zone_set(options->initial_zone_set_index);
		}
		else
		{
			main_game_globals.game_loaded_status = _game_loaded_status_map_loaded_failure;
		}
	}

	if (!success)
	{
		main_game_internal_map_load_abort(reload_map);
	}

	if ((map_load_status != _map_load_status_loaded && !map_loaded) || !success)
	{
		damaged_media_exception();
	}

	main_tag_load_end();

	bink_notify_load_masking_complete();

	bool resume_copy = false;
	while (bink_playback_active())
	{
		if (main_game_is_exiting())
		{
			break;
		}

		input_update();
		main_loop_pregame();
		cache_files_copy_do_work();
		if (bink_playback_using_io_during_map_load_masking() && !resume_copy)
		{
			cache_files_copy_pause();
			resume_copy = true;
		}
	}

	if (resume_copy)
		cache_files_copy_resume();

	// main_game_cleanup_loading_screen
	if (loading_basic_progress_enabled())
	{
		loading_basic_progress_complete();
		//if (c_rasterizer_loading_screen::active()) // function returns false, skipping for now
		//{
		//	bool keep_pumping = true;
		//	while (keep_pumping)
		//	{
		//		int32 map_load_progress = 0;
		//		e_session_game_start_status game_start_status{};
		//		if (user_interface_squad_get_machine_count() <= 1
		//			|| !user_interface_get_session_precaching_progress(&map_load_progress, &game_start_status)
		//			|| game_start_status == _session_game_start_status_error
		//			|| map_load_progress >= 100)
		//		{
		//			keep_pumping = !loading_basic_progress_keep_pumping();
		//		}
		//
		//		main_loop_pregame();
		//	}
		//}
		loading_basic_progress_disable();
	}

	ASSERT(main_game_globals.game_loaded_status == (success ? _game_loaded_status_map_loaded : _game_loaded_status_none));
	return success;
}

void __cdecl main_game_load_panic()
{
	//INVOKE(0x00567AD0, main_game_load_panic);

	RENDER_THREAD_LOCK;

	main_render_purge_pending_messages();
	main_game_unload_and_prepare_for_next_game(NULL);
	ASSERT(!main_game_loaded_map() && !main_game_loaded_pregame());

	bool successfully_loaded = false;
	if (debug_load_panic_to_main_menu)
	{
		if (load_panic_recursion_lock)
		{
			event(_event_critical, "main_game_load_panic: recursion lock triggered (we must have failed to load the main menu from a panic state)");
		}
		else
		{
			game_options options{};
			load_panic_recursion_lock = true;
			main_menu_build_game_options(&options);
			main_game_change_immediate(&options);
			ASSERT(main_game_loaded_map() || main_game_loaded_pregame());
			load_panic_recursion_lock = false;
			successfully_loaded = true;
		}
	}

	if (!successfully_loaded)
	{
		event(_event_critical, "main game load failed, unable to recover, aborting to pregame");

		main_game_internal_pregame_load();
		main_halt_and_display_errors();
	}

	ASSERT(main_game_loaded_map() || main_game_loaded_pregame());
}

void __cdecl main_game_load_from_core_name(const char* core_name)
{
	game_options options{};
	if (game_state_get_game_options_from_core(core_name, &options))
	{
		main_game_globals.load_core_on_game_launch = true;
		main_game_globals.core_name.set(core_name);

		game_options_validate(&options);
		main_game_change(&options);
	}
	else
	{
		event(_event_warning, "Failed to get game options from core (so I can't load it!)");
	}
}

void __cdecl main_game_load_from_core()
{
	main_game_load_from_core_name("core");
}

bool __cdecl main_game_loaded_map()
{
	//return INVOKE(0x00567BC0, main_game_loaded_map);

	return main_game_globals.game_loaded_status == _game_loaded_status_map_loaded;
}

const char* __cdecl main_game_loaded_map_name()
{
	//return INVOKE(0x00567BD0, main_game_loaded_map_name);

	return main_game_globals.game_loaded_scenario_path;
}

bool __cdecl main_game_loaded_pregame()
{
	//return INVOKE(0x00567BE0, main_game_loaded_pregame);

	return main_game_globals.game_loaded_status == _game_loaded_status_pregame;
}

void __cdecl main_game_reload_map(int32 initial_zone_set_index)
{
	//INVOKE(0x00567BF0, main_game_reload_map, initial_zone_set_index);

	director_notify_map_reset();
	if (main_game_globals.game_loaded_status == _game_loaded_status_map_loaded)
	{
		main_game_reload_map_by_name(main_game_globals.game_loaded_scenario_path, initial_zone_set_index);
	}
	else
	{
		event(_event_warning, "main_reload_map: no game is loaded!");
	}
}

void __cdecl main_game_reload_map_by_name(const char* map_name, int32 initial_zone_set_index)
{
	//INVOKE(0x00567C00, main_game_reload_map_by_name, map_name, initial_zone_set_index);

	if (game_in_progress())
	{
		game_options options{};
		csmemcpy(&options, game_options_get(), sizeof(game_options));
		options.scenario_path.set(map_name);
		if (initial_zone_set_index != NONE)
		{
			options.initial_zone_set_index = (int16)initial_zone_set_index;
		}
		options.game_simulation = _game_simulation_local;
		options.game_playback = _game_playback_none;
		csstrnzcpy(main_game_globals.game_loaded_scenario_path, map_name, NUMBEROF(main_game_globals.game_loaded_scenario_path));
		options.load_level_only = true;
		options.random_seed = generate_random_seed();
		game_options_validate(&options);
		main_game_change(&options);
	}
}

void __cdecl main_game_request_level_advance_spoke(int32 gp_level_index)
{
	//INVOKE(0x00567C10, main_game_request_level_advance_spoke, gp_level_index);

	ASSERT(game_is_campaign());

	main_game_globals.gp_level_advance_type = _game_progression_level_spoke_and_level_is_spoke;
	main_game_globals.gp_level_block_index = gp_level_index;
	main_game_globals.map_advance_pending = true;
}

void __cdecl main_game_request_level_advance()
{
	//INVOKE(0x00567C30, main_game_request_level_advance);

	ASSERT(game_is_campaign());

	main_game_globals.gp_level_advance_type = _game_progression_level_none;
	main_game_globals.map_advance_pending = true;
}

void __cdecl main_game_request_level_advance_hub(int32 gp_level_index)
{
	//INVOKE(0x00567C50, main_game_request_level_advance_hub, gp_level_index);

	ASSERT(game_is_campaign());

	main_game_globals.gp_level_advance_type = _game_progression_level_hub_and_level_is_hub;
	main_game_globals.gp_level_block_index = gp_level_index;
	main_game_globals.map_advance_pending = true;
}

void __cdecl main_game_request_level_advance_normal(int32 gp_level_index)
{
	//INVOKE(0x00567C70, main_game_request_level_advance_normal, gp_level_index);

	ASSERT(game_is_campaign());

	main_game_globals.gp_level_advance_type = _game_progression_level_normal;
	main_game_globals.gp_level_block_index = gp_level_index;
	main_game_globals.map_advance_pending = true;
}


bool __cdecl main_game_reset_in_progress()
{
	//return INVOKE(0x00567C90, main_game_reset_in_progress);

	return main_game_globals.reset_in_progress;
}

void __cdecl main_game_reset_map(bool reset_map_random)
{
	//INVOKE(0x00567CA0, main_game_reset_map, reset_map_random);

	//profiler_pause_reporting();
	RENDER_THREAD_LOCK;

	main_game_globals.reset_in_progress = true;

	simulation_stop();
	director_notify_map_reset();
	players_detach_from_map();
	//data_mine_insert_single_player_game_options("map reset");

	game_options options{};
	csmemcpy(&options, game_options_get(), sizeof(game_options));

	if (!game_is_survival())
	{
		int32 last_level = game_progression_get_last_level();
		if (last_level != NONE && game_progression_level_has_gameplay(last_level) && !game_progression_level_is_hub(last_level))
		{
			options.campaign_insertion_point = 0;
		}
	}

	if (reset_map_random)
	{
		options.random_seed = generate_random_seed();
	}

	int16 zone_set_index = options.initial_zone_set_index;
	if (game_in_editor())
	{
		zone_set_index = (int16)scenario_zone_set_index_get();
		if (VALID_INDEX(zone_set_index, global_scenario->zone_sets.count))
		{
			options.initial_zone_set_index = zone_set_index;
		}
		else
		{
			options.initial_zone_set_index = 0;
		}
	}
	scenario_prepare_for_map_reset(zone_set_index);

	{
		LOCAL_TAG_RESOURCE_SCOPE_LOCK;
		game_dispose_from_old_structure_bsp(global_structure_bsp_active_mask_get());
	}

	game_dispose_from_old_map();
	events_clear();
	input_flush();
	main_render_purge_pending_messages();

	bool success = main_game_start(&options);
	main_game_globals.reset_in_progress = false;
	if (!success)
	{
		event(_event_critical, "main_game_reset_map couldn't restart the game!");
		main_game_load_panic();
	}
}

bool __cdecl main_game_start(const game_options* options)
{
	//return INVOKE(0x00567E40, main_game_start, options);

	int32 zone_set_index = 0;
	if (options->initial_zone_set_index > 0)
	{
		zone_set_index = options->initial_zone_set_index;
	}

	if (zone_set_index > global_scenario->zone_sets.count - 1)
	{
		zone_set_index = global_scenario->zone_sets.count - 1;
	}

	RENDER_THREAD_LOCK;

	game_initialize_for_new_map(options);
	scenario_activate_initial_designer_zones(zone_set_index);
	game_create_objects(_game_create_from_main);
	game_create_players();

	bool success = false;

	if (scenario_activate_initial_zone_set(zone_set_index))
	{
		game_start(_game_create_from_main);
		game_create_ai(_game_create_from_main);

		success = true;
	}

	if (success)
	{
		if (!game_is_ui_shell())
		{
			//c_datamine datamine(0, "game start", 1, "main");
			//data_mine_usability_add_basic_information(&datamine);
		}
	}
	else
	{
		game_dispose_from_old_map();
	}

	return success;
}

void __cdecl main_game_unload_and_prepare_for_next_game(const game_options* options)
{
	//INVOKE(0x00567F40, main_game_unload_and_prepare_for_next_game, options);

	RENDER_THREAD_LOCK;
	
	if (game_in_progress())
	{
		//data_mine_insert_single_player_game_options("game finish");
		game_engine_game_ending();
		simulation_stop();
		scenario_switch_to_null_zone_set();
		game_dispose_from_old_map();
	}
	
	e_game_loaded_status game_loaded_status = main_game_globals.game_loaded_status;
	switch (game_loaded_status)
	{
	case _game_loaded_status_none:
		break;
	case _game_loaded_status_map_loading:
	case _game_loaded_status_map_loaded_failure:
	case _game_loaded_status_map_unloading:
		return;
	case _game_loaded_status_map_loaded:
	case _game_loaded_status_map_reloading:
	{
		main_game_internal_map_unload_begin();
		scenario_unload();
		main_game_internal_map_unload_complete();
	}
	break;
	case _game_loaded_status_pregame:
	{
		main_game_internal_pregame_unload();
	}
	break;
	}

	ASSERT(main_game_globals.game_loaded_status == _game_loaded_status_none);
	main_game_configure_map_memory(options);
}

void __cdecl main_menu_build_game_options(game_options* options)
{
	INVOKE(0x00568080, main_menu_build_game_options, options);

	//game_options_new(options);
	//options->game_mode = _game_mode_ui_shell;
	//s_level_datum level{};
	//if (levels_try_and_get_main_menu_map(&level))
	//{
	//	options->scenario_path.set(level.scenario_file);
	//}
	//else
	//{
	//	options->scenario_path.set(k_main_menu_scenario_tag);
	//}
	//game_options_setup_default_players(1, options);
}

void __cdecl main_menu_launch()
{
	INVOKE(0x005680E0, main_menu_launch);

	//if (!main_halted_with_errors())
	//{
	//	if (game_in_progress() && game_is_ui_shell())
	//	{
	//		user_interface_enter_game_shell();
	//	}
	//	else
	//	{
	//		game_options options{};
	//		main_menu_build_game_options(&options);
	//		game_options_validate(&options);
	//		main_game_change(&options);
	//	}
	//}
}

void __cdecl main_menu_launch_force()
{
	//INVOKE(0x00568190, main_menu_launch_force);

	game_options options{};
	main_menu_build_game_options(&options);
	game_options_validate(&options);
	main_game_change(&options);
}

bool __cdecl map_memory_configuration_is_campaign(e_map_memory_configuration configuration)
{
	//return INVOKE(0x00568260, map_memory_configuration_is_campaign, configuration);

	return configuration == _map_memory_configuration_campaign
		|| configuration == _map_memory_configuration_campaign_saved_film
		|| configuration == _map_memory_configuration_campaign_epilogue;
}

bool __cdecl map_memory_configuration_is_campaign_epilogue(e_map_memory_configuration configuration)
{
	//return INVOKE(0x00568280, map_memory_configuration_is_campaign_epilogue, configuration);

	return configuration == _map_memory_configuration_campaign_epilogue;
}

bool __cdecl map_memory_configuration_is_main_menu(e_map_memory_configuration configuration)
{
	//return INVOKE(0x00568290, map_memory_configuration_is_main_menu, configuration);

	return configuration == _map_memory_configuration_main_menu;
}

bool __cdecl map_memory_configuration_is_multiplayer(e_map_memory_configuration configuration)
{
	//return INVOKE(0x005682A0, map_memory_configuration_is_multiplayer, configuration);

	return configuration == _map_memory_configuration_multiplayer
		|| configuration == _map_memory_configuration_multiplayer_saved_film;
}

bool __cdecl map_memory_configuration_is_saved_film(e_map_memory_configuration configuration)
{
	//return INVOKE(0x005682C0, map_memory_configuration_is_saved_film, configuration);

	return configuration == _map_memory_configuration_campaign_saved_film
		|| configuration == _map_memory_configuration_multiplayer_saved_film;
}

