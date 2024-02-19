#include "main/main_game.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "input/input_windows.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/global_preferences.hpp"
#include "main/main.hpp"
#include "main/main_game_launch.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "simulation/simulation.hpp"

REFERENCE_DECLARE(0x023916D8, s_main_game_globals, main_game_globals);
REFERENCE_DECLARE(0x023DAE90, bool, load_panic_recursion_lock);

bool debug_load_panic_to_main_menu = true;

HOOK_DECLARE(0x00566EF0, main_game_change_immediate);
HOOK_DECLARE(0x00567AD0, main_game_load_panic);
HOOK_DECLARE(0x00567E40, main_game_start);

//.text:00566A80 ; unknown destructor
//.text:00566AD0 ; unknown destructor
//.text:00566B30 ; e_map_memory_configuration __cdecl compute_desired_map_memory_configuration(game_options const* options)
//.text:00566B90 ; void __cdecl data_mine_insert_single_player_game_options(char const* event_name)

bool __cdecl sub_566CC0()
{
	// #TODO: find name
	// used in `c_tag_resource_cache_controller::mark_necessary_resources`

	return INVOKE(0x00566CC0, sub_566CC0);

	//if (game_options_valid() && game_is_ui_shell())
	//	return true;
	//
	//if (!main_game_globals.change_in_progress || main_game_globals.game_load_pending)
	//	return false;
	//
	//return main_game_globals.game_loaded_options.game_mode == _game_mode_mainmenu;
}

void __cdecl main_game_campaign_loaded(game_options const* options)
{
	INVOKE(0x00566D00, main_game_campaign_loaded, options);
}

void __cdecl main_game_change(game_options const* options)
{
	//INVOKE(0x00566E70, main_game_change, options);

	if (options)
	{
		assert_game_options_verify(options);
		csmemcpy(&main_game_globals.game_loaded_options, options, sizeof(game_options));
	}

	main_game_globals.change_in_progress = true;
	main_game_globals.game_load_pending = options == nullptr;
	main_game_globals.game_loaded_time = system_milliseconds();

	if (!options)
	{
		if (network_life_cycle_get_state() != _life_cycle_state_none)
			network_life_cycle_request_leave(false);
		else
			network_life_cycle_end();
	}
}

void __cdecl main_game_change_abort()
{
	INVOKE(0x00566EE0, main_game_change_abort);

	//main_game_globals.change_in_progress = false;
}

bool __cdecl main_game_change_immediate(game_options const* options)
{
	//return INVOKE(0x00566EF0, main_game_change_immediate, options);

	bool result = false;
	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);

	main_render_purge_pending_messages();
	main_render_assert_no_pending_messages();
	main_game_unload_and_prepare_for_next_game(options);
	main_render_assert_no_pending_messages();
	main_events_reset(_main_reset_events_reason_changing_the_map);
	main_render_assert_no_pending_messages();
	main_status("map", NULL);

	if (options)
	{
		//events_clear();
		input_flush();

		main_status("map", "loading %s", options->scenario_path.get_string());

		assert_game_options_verify(options);
		//determinism_debug_manager_game_start();

		main_render_assert_no_pending_messages();
		if (main_game_load_map(options) && (main_render_assert_no_pending_messages(), main_game_start(options)))
		{
			main_status("map", "loaded %s", options->scenario_path.get_string());
			main_status("minor_version", "%i", get_map_minor_version());

			//generate_event(_event_level_message, "lifecycle: MAP-LOADED %s", options->scenario_path.get_string());
			c_console::write_line("lifecycle: MAP-LOADED %s", options->scenario_path.get_string());

			//c_datamine datamine(0, "map l oaded", 2, "main", "game");
			//data_mine_usability_add_basic_information(&datamine);
			//datamine.add_string("map", options->scenario_path.get_string());

			global_preferences_flush();

			switch (e_game_mode game_mode = options->game_mode)
			{
			case _game_mode_campaign:
			{
				//data_mine_insert_single_player_game_options("game start");
				main_game_campaign_loaded(options);
			}
			break;
			case _game_mode_mainmenu:
			{
				//data_mine_upload();
				if (!main_startup_sequence())
					user_interface_enter_game_shell();

				online_guide_delay_toasts(0);
			}
			break;
			case _game_mode_multiplayer:
			{
				char const* game_engine_name = game_engine_type_get_string(options->game_variant.get_game_engine_index());
				char const* game_variant_name = options->game_variant.get_active_variant()->get_name();

				//generate_event(_event_level_message, "lifecycle: MULTIPLAYER-GAME %s", game_engine_name);
				c_console::write_line("lifecycle: MULTIPLAYER-GAME %s", game_engine_name);

				//generate_event(_event_level_message, "lifecycle: MULTIPLAYER-VARIANT %s", game_variant_name);
				c_console::write_line("lifecycle: MULTIPLAYER-VARIANT %s", game_variant_name);
			}
			break;
			}

			if (!g_launch_globals.core_name.is_empty())
			{
				main_load_core_name(g_launch_globals.core_name.get_string());
				g_launch_globals.core_name.clear();
			}

			result = true;
		}
		else
		{
			main_status("map", "load-failed %s", options->scenario_path.get_string());

			//generate_event(_event_level_critical, "main_game_change_immediate() failed for '%s', cannot load game", options->scenario_path.get_string());
			c_console::write_line("main_game_change_immediate() failed for '%s', cannot load game", options->scenario_path.get_string());

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

	return result;
}

bool __cdecl main_game_change_in_progress()
{
	return INVOKE(0x005670E0, main_game_change_in_progress);

	return main_game_globals.change_in_progress;
}

//// functions for `main_game_change_update`
//
//bool __cdecl campaign_metagame_sub_60B080()
//{
//	return INVOKE(0x0060B080, campaign_metagame_sub_60B080);
//}
//
//void __cdecl carnage_report_transition_out()
//{
//	INVOKE(0x0060AFD0, carnage_report_transition_out);
//}

void __cdecl main_game_change_update()
{
	INVOKE(0x005670F0, main_game_change_update);

	////main_game_editor_world_controller_update();
	//
	//if (main_game_globals.request_level_advance && !campaign_metagame_sub_60B080())
	//{
	//	bool is_leader = false;
	//
	//	if (game_is_playback())
	//	{
	//		carnage_report_transition_out();
	//	}
	//	else if ((!game_in_progress()
	//		|| !game_is_campaign()
	//		|| game_is_survival()
	//		|| main_game_globals.gp_level_advance_type
	//		|| !main_game_goto_next_level())
	//		&& (!network_squad_session_controls_coop_game_options(&is_leader) || is_leader))
	//	{
	//		if (!game_in_progress() || !game_is_ui_shell())
	//			simulation_end(_simulation_abort_reason_failed_to_find_next_map);
	//
	//		if (!user_interface_reset_networking_to_pregame())
	//			main_menu_launch();
	//	}
	//
	//	main_game_globals.request_level_advance = false;
	//}
	//
	//if (main_game_globals.change_in_progress)
	//{
	//	if (main_game_globals.game_load_pending)
	//	{
	//		if (main_game_globals.game_loaded_status != _game_loaded_status_pregame)
	//			main_game_change_immediate(NULL);
	//	}
	//	else
	//	{
	//		main_game_change_immediate(&main_game_globals.game_loaded_options);
	//	}
	//
	//	sub_5129B0();
	//
	//	main_game_globals.change_in_progress = false;
	//	main_game_globals.game_load_pending = false;
	//	main_game_globals.__unknown120 = 0;
	//}
}

//.text:00567200 ; void __cdecl main_game_configure_map_memory(game_options const* options)
//.text:00567230 ; void __cdecl main_game_configure_map_memory_pop()
//.text:00567270 ; void __cdecl main_game_configure_map_memory_push(e_map_memory_configuration configuration)

bool __cdecl main_game_goto_next_level()
{
	return INVOKE(0x005672B0, main_game_goto_next_level);
}

void __cdecl main_game_initialize()
{
	//INVOKE(0x005674B0, main_game_initialize);

	main_game_launch_initialize();
	main_game_globals.request_level_advance = false;
}

void __cdecl main_game_internal_close_caches()
{
	INVOKE(0x005674D0, main_game_internal_close_caches);
}

void __cdecl main_game_internal_map_load_abort(bool reload_map)
{
	INVOKE(0x005674F0, main_game_internal_map_load_abort, reload_map);
}

void __cdecl main_game_internal_map_load_begin(bool reload_map)
{
	INVOKE(0x00567540, main_game_internal_map_load_begin, reload_map);
}

bool __cdecl main_game_internal_map_load_complete(bool a1, game_options const* options)
{
	return INVOKE(0x00567560, main_game_internal_map_load_complete, a1, options);
}

void __cdecl main_game_internal_map_unload_begin()
{
	INVOKE(0x005675D0, main_game_internal_map_unload_begin);
}

void __cdecl main_game_internal_map_unload_complete()
{
	INVOKE(0x00567610, main_game_internal_map_unload_complete);
}

bool __cdecl main_game_internal_open_caches(game_options const* options)
{
	return INVOKE(0x00567630, main_game_internal_open_caches, options);
}

void __cdecl main_game_pregame_blocking_load()
{
	INVOKE(0x00567670, main_game_pregame_blocking_load);
}

void __cdecl main_game_internal_pregame_blocking_unload()
{
	INVOKE(0x005676D0, main_game_internal_pregame_blocking_unload);
}

void __cdecl main_game_internal_pregame_load()
{
	INVOKE(0x005676F0, main_game_internal_pregame_load);
}

void __cdecl main_game_internal_pregame_unload()
{
	INVOKE(0x00567720, main_game_internal_pregame_unload);
}

void __cdecl main_game_launch_default()
{
	INVOKE(0x00567750, main_game_launch_default);
}

void __cdecl main_game_launch_default_editor()
{
	INVOKE(0x00567820, main_game_launch_default_editor);
}

bool __cdecl main_game_load_blocking(char const* scenario_path)
{
	return INVOKE(0x00567830, main_game_load_blocking, scenario_path);
}

bool __cdecl main_game_load_map(game_options const* options)
{
	return INVOKE(0x00567850, main_game_load_map, options);
}

void __cdecl main_game_load_panic()
{
	//INVOKE(0x00567AD0, main_game_load_panic);

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);

	main_render_purge_pending_messages();
	main_game_unload_and_prepare_for_next_game(NULL);
	ASSERT(!main_game_loaded_map() && !main_game_loaded_pregame());

	bool successfully_loaded = false;
	if (debug_load_panic_to_main_menu)
	{
		if (load_panic_recursion_lock)
		{
			//generate_event(_event_level_critical, "main_game_load_panic: recursion lock triggered (we must have failed to load the main menu from a panic state)");
			c_console::write_line("main_game_load_panic: recursion lock triggered (we must have failed to load the main menu from a panic state)");
		}
		else
		{
			game_options options = game_options();
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
		//generate_event(_event_level_critical, "main game load failed, unable to recover, aborting to pregame");
		c_console::write_line("main game load failed, unable to recover, aborting to pregame");

		main_game_internal_pregame_load();
		main_halt_and_display_errors();
	}

	ASSERT(main_game_loaded_map() || main_game_loaded_pregame());
}

void __cdecl main_game_load_from_core_name(char const* core_name)
{
}

void __cdecl main_game_load_from_core()
{
}

bool __cdecl main_game_loaded_map()
{
	//return INVOKE(0x00567BC0, main_game_loaded_map);

	return main_game_globals.game_loaded_status == _game_loaded_status_map_loaded;
}

char const* __cdecl main_game_loaded_map_name()
{
	//return INVOKE(0x00567BD0, main_game_loaded_map_name);

	return main_game_globals.game_loaded_scenario_path;
}

bool __cdecl main_game_loaded_pregame()
{
	//return INVOKE(0x00567BE0, main_game_loaded_pregame);

	return main_game_globals.game_loaded_status == _game_loaded_status_pregame;
}

void __cdecl main_game_notify_language_change(e_language language)
{
	INVOKE(0x00567BF0, main_game_notify_language_change, language);
}

//.text:00567C10 ; unknown main_game_progression
//.text:00567C30 ; unknown main_game_progression
//.text:00567C50 ; unknown main_game_progression
//.text:00567C70 ; unknown main_game_progression

bool __cdecl main_game_reset_in_progress()
{
	//return INVOKE(0x00567C90, main_game_reset_in_progress);

	return main_game_globals.reset_in_progress;
}

void __cdecl main_game_reset_map(bool reset_map)
{
	INVOKE(0x00567CA0, main_game_reset_map, reset_map);
}

bool __cdecl main_game_start(game_options const* options)
{
	//return INVOKE(0x00567E40, main_game_start, options);

	long zoneset_index = 0;
	if (options->initial_zone_set_index > 0)
		zoneset_index = options->initial_zone_set_index;
	
	if (zoneset_index > global_scenario->zone_sets.count() - 1)
		zoneset_index = global_scenario->zone_sets.count() - 1;
	
	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	
	game_initialize_for_new_map(options);
	scenario_activate_initial_designer_zones(zoneset_index);
	game_create_objects(_game_create_mode_lock);
	game_create_players();

	bool success = false;

	if (scenario_activate_initial_zone_set(zoneset_index))
	{
		game_start(_game_create_mode_lock);
		game_create_ai(_game_create_mode_lock);
	
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

void __cdecl main_game_unload_and_prepare_for_next_game(game_options const* options)
{
	INVOKE(0x00567F40, main_game_unload_and_prepare_for_next_game, options);
}

void __cdecl main_menu_build_game_options(game_options* options)
{
	INVOKE(0x00568080, main_menu_build_game_options, options);

	//game_options_new(options);
	//options->game_mode = _game_mode_mainmenu;
	//s_level_datum level{};
	//if (levels_try_and_get_main_menu_map(&level))
	//	options->scenario_path.set(level.scenario_path);
	//else
	//	options->scenario_path.set("levels\\ui\\mainmenu\\mainmenu" /* k_main_menu_scenario_tag */);
	//game_options_setup_default_players(1, options);
}

void __cdecl main_menu_launch()
{
	INVOKE(0x005680E0, main_menu_launch);
}

void __cdecl main_menu_launch_force()
{
	INVOKE(0x00568190, main_menu_launch_force);
}

//.text:00568260 ; bool __cdecl map_memory_configuration_is_campaign(e_map_memory_configuration configuration)
//.text:00568280 ; bool __cdecl map_memory_configuration_is_campaign_epilogue(e_map_memory_configuration configuration)
//.text:00568290 ; bool __cdecl map_memory_configuration_is_main_menu(e_map_memory_configuration configuration)
//.text:005682A0 ; bool __cdecl map_memory_configuration_is_multiplayer(e_map_memory_configuration configuration)
//.text:005682C0 ; bool __cdecl map_memory_configuration_is_saved_film(e_map_memory_configuration configuration)

