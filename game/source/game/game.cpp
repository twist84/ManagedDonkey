#include "game/game.hpp"

#include "ai/ai.hpp"
#include "cache/cache_files_windows.hpp"
#include "config/version.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/runtime_state.hpp"
#include "cutscene/cinematics.hpp"
#include "cutscene/recorded_animations.hpp"
#include "data_mining/data_mine_management.hpp"
#include "editor/editor_stubs.hpp"
#include "effects/effects.hpp"
#include "error/error_report_render.hpp"
#include "fmod/src/sound_fmod.hpp"
#include "game/campaign_metagame.hpp"
#include "game/cheats.hpp"
#include "game/game_achievements.hpp"
#include "game/game_allegiance.hpp"
#include "game/game_engine.hpp"
#include "game/game_grief.hpp"
#include "game/player_rumble.hpp"
#include "game/player_training.hpp"
#include "game/players.hpp"
#include "game/survival_mode.hpp"
#include "hs/hs.hpp"
#include "interface/chud/chud.hpp"
#include "interface/closed_caption.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface.hpp"
#include "interface/overhead_map.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_hs.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_render.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/network_globals.hpp"
#include "networking/online/online.hpp"
#include "networking/online/online_achievements.hpp"
#include "networking/tools/network_webstats.hpp"
#include "networking/tools/remote_command.hpp"
#include "networking/tools/telnet_console.hpp"
#include "networking/tools/xbox_connection_manager.hpp"
#include "networking/transport/transport.hpp"
#include "objects/object_activation_regions.hpp"
#include "objects/object_broadphase.hpp"
#include "objects/object_early_movers.hpp"
#include "objects/object_placement.hpp"
#include "objects/object_scheduler.hpp"
#include "objects/object_scripting.hpp"
#include "objects/watch_window.hpp"
#include "objects/widgets/widgets.hpp"
#include "physics/breakable_surfaces.hpp"
#include "physics/collision_debug.hpp"
#include "physics/collision_usage.hpp"
#include "physics/havok.hpp"
#include "physics/havok_proxies.hpp"
#include "physics/point_physics.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_hue_saturation.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/render.hpp"
#include "render/render_debug.hpp"
#include "render/render_game_state.hpp"
#include "render/render_structure.hpp"
#include "render/render_visibility.hpp"
#include "render/views/hud_camera_view.hpp"
#include "render/views/render_player_view.hpp"
#include "render_methods/render_method_submit.hpp"
#include "saved_games/autosave_queue.hpp"
#include "saved_games/determinism_debug_manager.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "saved_games/saved_game_files.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_interpolators.hpp"
#include "scenario/scenario_pvs.hpp"
#include "scenario/scenario_sky.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "screenshots/screenshots_loader.hpp"
#include "screenshots/screenshots_uploader.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation.hpp"
#include "sky_atm/atmosphere.hpp"
#include "sound/game_sound.hpp"
#include "sound/game_sound_deterministic.hpp"
#include "sound/game_sound_player_effects.hpp"
#include "sound/sound_classes.hpp"
#include "sound/sound_manager.hpp"
#include "spartan_program/spartan_program_handler.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "structures/structures.hpp"
#include "tag_files/files_windows.hpp"
#include "test/test_functions.hpp"
#include "visibility/visibility_collection.hpp"
#include "visibility/visibility_portal_activation.hpp"

HOOK_DECLARE(0x00530F80, game_finish);
HOOK_DECLARE(0x00530CD0, game_dispose);
HOOK_DECLARE(0x00530D10, game_dispose_from_old_map);
HOOK_DECLARE(0x00530E10, game_dispose_from_old_non_bsp_zone_set);
HOOK_DECLARE(0x00530E70, game_dispose_from_old_structure_bsp);
HOOK_DECLARE(0x005312C0, game_globals_initialize_for_new_map);
HOOK_DECLARE(0x00531530, game_initialize);
HOOK_DECLARE(0x00531600, game_initialize_for_new_map);
HOOK_DECLARE(0x00531790, game_initialize_for_new_non_bsp_zone_set);
HOOK_DECLARE(0x005317F0, game_initialize_for_new_structure_bsp);
HOOK_DECLARE(0x00532AD0, game_prepare_for_non_bsp_zone_set_switch);
HOOK_DECLARE(0x00532B10, game_prepare_to_switch_structure_bsp);
HOOK_DECLARE(0x00533120, game_tick);
HOOK_DECLARE(0x00533B80, game_update_pvs);
HOOK_DECLARE(0x006961B0, game_launch_get_initial_script_name);

bool g_debug_survival_mode = false;

c_static_array<c_static_array<int32, MAXIMUM_CLUSTERS_PER_STRUCTURE>, 16> g_cluster_activation_reason;

const real_argb_color* const k_activation_colors[6]
{
	nullptr,
	global_real_argb_orange,
	global_real_argb_green,
	global_real_argb_pink,
	global_real_argb_purple,
	global_real_argb_yellow
};
int32 k_activation_color_override_index = 0;

real32 const k_game_loss_time = 6.0f;
real32 const k_game_finished_time = 7.0f;

const s_cluster_reference* cluster_reference_set(s_cluster_reference* cluster_reference, int32 bsp_index, int32 cluster_index)
{
	cluster_reference->bsp_index = static_cast<int8>(bsp_index);
	cluster_reference->cluster_index = static_cast<int8>(cluster_index);

	return cluster_reference;
}

bool __cdecl game_is_multithreaded()
{
	return INVOKE(0x0042E2C0, game_is_multithreaded);
}

//.text:005303E0 ; void __cdecl __tls_set_g_game_globals_allocator(void*)

void __cdecl assert_game_options_verify(const game_options* options)
{
	//INVOKE(0x00530440, assert_game_options_verify, options);

	char error_string[512]{};
	if (!game_options_verify(options, error_string, sizeof(error_string)))
		VASSERT(c_string_builder("game_options_verify failed: %s", error_string).get_string());
}

//.text:00530580 ; game_globals_get_primary_skulls
//.text:005305D0 ; game_globals_get_secondary_skulls
//.text:00530620 ; void __cdecl game_react_to_level_completion()
//.text:00530790 ; 
//.text:005307A0 ; 

void __cdecl game_clear_structure_pvs(s_game_cluster_bit_vectors* structure_pvs, uns32 structure_bsp_mask)
{
	INVOKE(0x005307B0, game_clear_structure_pvs, structure_pvs, structure_bsp_mask);
}

void __cdecl game_clusters_and(const s_game_cluster_bit_vectors* a1, const s_game_cluster_bit_vectors* a2, s_game_cluster_bit_vectors* a3)
{
	INVOKE(0x005307F0, game_clusters_and, a1, a2, a3);
}

void __cdecl game_clusters_fill(s_game_cluster_bit_vectors* a1, bool a2)
{
	INVOKE(0x00530840, game_clusters_fill, a1, a2);
}

void __cdecl game_clusters_or(const s_game_cluster_bit_vectors* a1, const s_game_cluster_bit_vectors* a2, s_game_cluster_bit_vectors* a3)
{
	INVOKE(0x00530860, game_clusters_or, a1, a2, a3);
}

void __cdecl game_compute_pvs(s_game_cluster_bit_vectors* a1, bool a2, c_static_array<c_static_array<int32, MAXIMUM_CLUSTERS_PER_STRUCTURE>, 16>* a3)
{
	INVOKE(0x005308B0, game_compute_pvs, a1, a2, a3);
}

bool game_clusters_test(const s_game_cluster_bit_vectors* vector, struct s_cluster_reference structure_reference)
{
	return vector->flags.element(structure_reference.bsp_index).test(structure_reference.cluster_index);
}

//.text:00530A20 ; bool __cdecl game_coop_allow_respawn()
//.text:00530A40 ; int32 __cdecl game_coop_player_count()

void __cdecl game_create_ai(e_game_create_mode mode)
{
	INVOKE(0x00530A70, game_create_ai, mode);

	//int32 lock = game_create_lock_resources(mode);
	//random_seed_allow_use();
	//
	//ai_place(game_mode_get(), true);
	//
	//random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

int32 __cdecl game_create_lock_resources(e_game_create_mode mode)
{
	return INVOKE(0x00530AC0, game_create_lock_resources, mode);

	//return mode == _game_create_from_main ? tag_resources_lock_game() : 0;
}

void __cdecl game_create_missing_objects(e_game_create_mode mode)
{
	INVOKE(0x00530AE0, game_create_missing_objects, mode);

	//assert_game_options_verify(&game_globals->options);
	//ASSERT(game_globals->map_active);
	//
	//int32 lock = game_create_lock_resources(mode);
	//random_seed_allow_use();
	//
	//object_placement_create_global_objects(game_globals->options.game_mode, true);
	//
	//random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

void __cdecl game_create_objects(e_game_create_mode mode)
{
	INVOKE(0x00530B30, game_create_objects, mode);

	//assert_game_options_verify(&game_globals->options);
	//ASSERT(game_globals->map_active);
	//
	//int32 lock = game_create_lock_resources(mode);
	//random_seed_allow_use();
	//
	//c_scenerio_sky_objects_helper::create_sky_objects(global_scenario_get());
	//object_placement_create_global_objects(game_globals->options.game_mode, false);
	//lights_place(game_globals->options.game_mode);
	//
	//random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

void __cdecl game_create_players()
{
	INVOKE(0x00530B90, game_create_players);

	//if (!main_game_reset_in_progress())
	//{
	//	game_options* options = game_options_get();
	//	ASSERT(!simulation_reset_in_progress());
	//
	//	players_set_machines(options->machines.valid_machine_mask, options->machines.machines);
	//	players_set_local_machine(options->machines.local_machine_exists ? &options->machines.local_machine : NULL);
	//	for (int32 i = 0; i < 16; i++)
	//	{
	//		if (options->players[i].player_valid)
	//			player_new(i, &options->players[i], false);
	//	}
	//
	//	players_finish_creation();
	//	simulation_notify_players_created();
	//}

	if (!game_is_splitscreen_deterministic())
		return;

	c_player_in_game_iterator player_iterator;
	player_iterator.begin();
	while (player_iterator.next())
	{
		if (!player_is_local(player_iterator.get_index()))
			continue;

		int16 absolute_index = player_iterator.get_absolute_index();
		player_datum* player = player_iterator.get_datum();

		e_controller_index controller_index = (e_controller_index)absolute_index;
		if (VALID_CONTROLLER(controller_index))
		{
			uns64 player_identifier = online_local_user_get_player_identifier(controller_index);
			player->player_identifier = player_identifier;

			const wchar_t* name = online_local_user_get_name(controller_index);

			if (player->configuration.host.name.is_empty())
				player->configuration.host.name = name;

			if (player->configuration.client.desired_name.is_empty())
				player->configuration.client.desired_name = name;
		}
		else
		{
			if (player->configuration.host.name.is_empty())
				player->configuration.host.name.print(L"player_%d", absolute_index);

			if (player->configuration.client.desired_name.is_empty())
				player->configuration.client.desired_name.print(L"player_%d", absolute_index);
		}
	}
}

void __cdecl game_create_unlock_resources(e_game_create_mode mode, int32& lock)
{
	INVOKE(0x00530C20, game_create_unlock_resources, mode, lock);

	//if (mode == _game_create_from_main)
	//	tag_resources_unlock_game(lock);
}

//.text:00530C50 ; bool __cdecl game_determinism_version_compatible(int32 determinism_version)

e_campaign_difficulty_level __cdecl game_difficulty_level_get()
{
	//return INVOKE(0x00530C70, game_difficulty_level_get);

	return (e_campaign_difficulty_level)game_options_get()->campaign_difficulty;
}

e_campaign_difficulty_level __cdecl game_difficulty_level_get_ignore_easy()
{
	//return INVOKE(0x00530CA0, game_difficulty_level_get);

	if (game_difficulty_level_get() == _campaign_difficulty_level_easy)
		return _campaign_difficulty_level_normal;

	return game_difficulty_level_get();
}

void __cdecl game_dispose()
{
	//INVOKE(0x00530CD0, game_dispose);

	// some bool set to false

	for (int32 system_index = g_game_system_count - 1; system_index >= 0; system_index--)
	{
		ASSERT(g_game_systems[system_index].dispose_proc);
		g_game_systems[system_index].dispose_proc();
	}

	fmod_dispose();
}

void __cdecl game_dispose_from_old_map()
{
	//INVOKE(0x00530D10, game_dispose_from_old_map);

	ASSERT(main_game_loaded_map());
	ASSERT(!game_globals->initializing);
	ASSERT(game_globals->map_active);
	ASSERT(game_globals->active_structure_bsp_mask == 0);

	game_globals->active_designer_zone_mask = 0;
	game_globals->active_cinematic_zone_mask = 0;
	game_globals->active_game_progression_level = _string_id_invalid;
	game_globals->prepare_for_game_progression = false;

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	game_globals->game_in_progress = false;

	fmod_dispose_from_old_map();

	for (int32 system_index = g_game_system_count - 1; system_index >= 0; system_index--)
	{
		if (g_game_systems[system_index].dispose_from_old_map_proc)
			g_game_systems[system_index].dispose_from_old_map_proc();
	}

	random_seed_debug_log_end();
	game_globals_dispose_from_old_map();
	game_globals->map_active = false;
}

void __cdecl game_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set)
{
	//INVOKE(0x00530E10, game_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);

	assert_game_options_verify(&game_globals->options);
	ASSERT(game_globals->map_active);

	game_designer_zone_set_debug_status("disposing", game_globals->active_designer_zone_mask);
	game_cinematic_zone_set_debug_status("disposing", game_globals->active_cinematic_zone_mask);

	havok_can_modify_state_allow();

	for (int32 system_index = g_game_system_count - 1; system_index >= 0; system_index--)
	{
		if (g_game_systems[system_index].dispose_from_old_non_bsp_zone_set_proc)
			g_game_systems[system_index].dispose_from_old_non_bsp_zone_set_proc(old_non_bsp_zone_set);
	}

	objects_purge_deleted_objects();
	havok_can_modify_state_disallow();

	game_designer_zone_set_debug_status("disposed", game_globals->active_designer_zone_mask);
	game_cinematic_zone_set_debug_status("disposed", game_globals->active_cinematic_zone_mask);

	game_globals->active_designer_zone_mask = 0;
	game_globals->active_cinematic_zone_mask = 0;
}

void __cdecl game_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	//INVOKE(0x00530E70, game_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);

	assert_game_options_verify(&game_globals->options);
	ASSERT(game_globals->map_active);
	ASSERT(game_globals->active_structure_bsp_mask != 0);
	ASSERT(global_structure_bsp_active_mask_get() != 0);

	game_clear_structure_pvs(&game_globals->cluster_pvs, deactivating_structure_bsp_mask);
	game_clear_structure_pvs(&game_globals->cluster_pvs_local, deactivating_structure_bsp_mask);
	game_clear_structure_pvs(&game_globals->cluster_activation, deactivating_structure_bsp_mask);

	game_bsp_debug_status("disposing", game_globals->active_structure_bsp_mask);

	havok_can_modify_state_allow();

	for (int32 system_index = g_game_system_count - 1; system_index >= 0; system_index--)
	{
		if (g_game_systems[system_index].dispose_from_old_structure_bsp_proc)
			g_game_systems[system_index].dispose_from_old_structure_bsp_proc(deactivating_structure_bsp_mask);
	}

	objects_purge_deleted_objects();
	havok_can_modify_state_disallow();

	game_bsp_debug_status("disposed", game_globals->active_structure_bsp_mask);

	game_globals->active_structure_bsp_mask = 0;
}

void __cdecl game_finish()
{
	//INVOKE(0x00530F80, game_finish);

	ASSERT(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		game_globals->game_finished = true;
		game_globals->game_finished_timer = game_seconds_to_ticks_round(game_is_campaign_or_survival() ? 1.0f : k_game_finished_time);
	}
}

//.text:00530FF0 ; 

void __cdecl game_finished_update()
{
	//INVOKE(0x00531040, game_finished_update);

	if (game_globals->game_finished && game_globals->game_finished_timer > 0)
		game_globals->game_finished_timer--;
}

void __cdecl game_frame(real32 game_seconds_elapsed)
{
	//INVOKE(0x00531070, game_frame, game_seconds_elapsed);

	if (game_seconds_elapsed > 0.0f)
	{
		widgets_update(game_seconds_elapsed);
		game_sound_update(game_seconds_elapsed);
		scenario_frame_update(game_seconds_elapsed);
		scenario_interpolators_frame_update(game_seconds_elapsed);
		scripted_exposure_update(game_seconds_elapsed);
	}

	player_effect_frame_update(game_seconds_elapsed);
	c_render_globals::advance_frame_time(game_seconds_elapsed);
	game_globals->game_had_an_update_tick_this_frame = false;
}

uns32 __cdecl game_get_active_cinematic_zone_mask()
{
	//return INVOKE(0x00531110, game_get_active_cinematic_zone_mask);

	return game_globals->active_cinematic_zone_mask;
}

uns32 __cdecl game_get_active_designer_zone_mask()
{
	//return INVOKE(0x00531130, game_get_active_designer_zone_mask);

	return game_globals->active_designer_zone_mask;
}

uns32 __cdecl game_get_active_structure_bsp_mask()
{
	//return INVOKE(0x00531150, game_get_active_structure_bsp_mask);

	return game_globals->active_structure_bsp_mask;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_activation()
{
	//return INVOKE(0x00531170, game_get_cluster_activation);

	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_activation;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs()
{
	//return INVOKE(0x00531190, game_get_cluster_pvs);

	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs_local()
{
	//return INVOKE(0x005311B0, game_get_cluster_pvs_local);

	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs_local;
}

//.text:005311D0 ; void __cdecl game_get_determinism_versions(int32* determinism_version, int32* determinism_compatible_version)
//.text:00531200 ; 

e_language __cdecl game_get_master_language()
{
	//return INVOKE(0x00531250, game_get_master_language);

	return game_options_get()->language;
}

//.text:00531270 ; int32 __cdecl game_get_recently_spawned_grace_period_seconds()

void __cdecl game_globals_dispose_from_old_map()
{
	//INVOKE(0x00531290, game_globals_dispose_from_old_map);

	main_status("game_instance", NULL);
	main_status("game_simulation", NULL);
	main_status("game_playback", NULL);
}

void __cdecl game_globals_initialize_for_new_map(const game_options* options)
{
	//INVOKE(0x005312C0, game_globals_initialize_for_new_map, options);

	assert_game_options_verify(options);

	csmemcpy(&game_globals->options, options, sizeof(game_options));
	game_globals->options.load_level_only = true;

	if (game_globals->options.game_mode == _ui_game_mode_multiplayer || options->multiplayer_variant.get_game_engine_index())
	{
		game_globals->options.multiplayer_variant.copy_from_and_validate(&options->multiplayer_variant);
		if (!game_engine_variant_validate(&game_globals->options.multiplayer_variant))
			event(_event_warning, "variant validation failed, about to start playing a default variant");
	}

	random_seed_allow_use();
	set_random_seed(game_globals->options.random_seed);
	random_seed_disallow_use();

	if (game_globals->options.dump_random_seeds)
		random_seed_debug_log_begin(&game_globals->options);

	game_globals->game_lost = false;
	game_globals->game_revert = false;
	game_globals->game_finished = false;
	game_globals->pvs_activation_type = false;

	game_globals->active_primary_skulls = options->campaign_active_primary_skulls;
	game_globals->active_secondary_skulls = options->campaign_active_secondary_skulls;

	ASSERT(options->game_simulation >= 0 && options->game_simulation < k_game_simulation_count);

	main_status("game_instance", "%016I64X", options->game_instance);
	main_status("game_simulation", "%s", k_game_simulation_names[options->game_simulation]);

	ASSERT(options->game_playback >= 0 && options->game_playback < k_game_playback_count);
	main_status("game_playback", "%s", k_game_playback_names[options->game_playback]);

	game_globals->active_game_progression = game_globals->options.campaign_game_progression;
}

bool __cdecl game_had_an_update_tick_this_frame()
{
	//return INVOKE(0x00531410, game_had_an_update_tick_this_frame);

	return game_globals && game_globals->game_had_an_update_tick_this_frame;
}

bool __cdecl game_has_nonlocal_players()
{
	return INVOKE(0x00531430, game_has_nonlocal_players);
}

bool __cdecl game_in_progress()
{
	//return INVOKE(0x005314B0, game_in_progress);

	if (game_globals && game_globals->game_in_progress)
		return !game_globals->initializing && game_globals->map_active;

	return false;
}

bool __cdecl game_in_startup_phase()
{
	//return INVOKE(0x005314F0, game_in_startup_phase);

	if (game_globals)
	{
		if (game_globals->game_in_progress)
		{
			ASSERT(!game_globals->initializing);
			ASSERT(game_globals->map_active);

			return false;
		}

		return game_globals->initializing || game_globals->map_active;
	}
	return false;
}

void __cdecl game_initialize()
{
	//INVOKE(0x00531530, game_initialize);

	game_globals = (game_globals_storage*)g_game_globals_allocator.allocate(sizeof(game_globals_storage), "game globals");
	csmemset(game_globals, 0, sizeof(game_globals_storage));

	game_globals->active_structure_bsp_mask = 0;
	game_globals->active_game_progression_level = _string_id_invalid;
	game_globals->prepare_for_game_progression = false;

	real_math_reset_precision();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		//system_debug_memory_internal(c_string_builder("before initializing %s", g_game_systems[system_index].name).get_string(), __FILE__, __LINE__);
		c_console::write_line("before initializing %s", g_game_systems[system_index].name);

		if (!sub_42E5D0() || g_game_systems[system_index].initialize_proc != c_water_renderer::initialize)
		{
			ASSERT(g_game_systems[system_index].initialize_proc);
			g_game_systems[system_index].initialize_proc();
		}

		//system_debug_memory_internal(c_string_builder("after initializing %s", g_game_systems[system_index].name).get_string(), __FILE__, __LINE__);
		c_console::write_line("after initializing %s", g_game_systems[system_index].name);
	}

	// some bool set to true
}

void __cdecl game_initialize_for_new_map(const game_options* options)
{
	//INVOKE(0x00531600, game_initialize_for_new_map, options);

	ASSERT(options);
	ASSERT(main_game_loaded_map());

	char scenario_path[256]{};
	char loaded_map[256]{};

	cache_file_get_canonical_path(options->scenario_path.get_string()).copy_to(scenario_path, sizeof(scenario_path));
	cache_file_get_canonical_path(main_game_loaded_map_name()).copy_to(loaded_map, sizeof(loaded_map));

	bool scenario_path_and_loaded_map_match = strncmp_debug(loaded_map, scenario_path, 256) == 0;
	ASSERT(scenario_path_and_loaded_map_match);

	ASSERT(!game_globals->initializing);
	ASSERT(!game_globals->map_active);
	ASSERT(!game_globals->game_in_progress);
	ASSERT(game_globals->active_structure_bsp_mask == 0);

	csmemset(&game_globals->cluster_pvs, 0, sizeof(game_globals->cluster_pvs));
	csmemset(&game_globals->cluster_pvs_local, 0, sizeof(game_globals->cluster_pvs_local));
	csmemset(&game_globals->cluster_activation, 0, sizeof(game_globals->cluster_activation));
	g_cluster_activation_reason.clear();

	c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	real_math_reset_precision();
	random_seed_allow_use();

	game_globals->initializing = true;
	game_globals_initialize_for_new_map(options);
	fmod_initialize_for_new_map();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		if (g_game_systems[system_index].initialize_for_new_map_proc)
			g_game_systems[system_index].initialize_for_new_map_proc();
	}

	game_globals->initializing = false;
	game_globals->map_active = true;
	game_globals->active_game_progression_level = _string_id_invalid;
	game_globals->prepare_for_game_progression = false;

	random_seed_disallow_use();
}

void __cdecl game_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set)
{
	//INVOKE(0x00531790, game_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);

	assert_game_options_verify(&game_globals->options);
	ASSERT(game_globals->map_active);
	ASSERT(game_globals->active_designer_zone_mask == 0);
	ASSERT((global_designer_zone_active_mask_get() & new_non_bsp_zone_set->designer_zone_mask) == new_non_bsp_zone_set->designer_zone_mask);

	game_designer_zone_set_debug_status("initializing", global_designer_zone_active_mask_get());
	game_cinematic_zone_set_debug_status("initializing", global_cinematic_zone_active_mask_get());

	game_globals->active_designer_zone_mask = global_designer_zone_active_mask_get();
	game_globals->active_cinematic_zone_mask = global_cinematic_zone_active_mask_get();

	random_seed_allow_use();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		if (g_game_systems[system_index].initialize_for_new_non_bsp_zone_set_proc)
			g_game_systems[system_index].initialize_for_new_non_bsp_zone_set_proc(new_non_bsp_zone_set);
	}

	random_seed_disallow_use();

	game_designer_zone_set_debug_status("initialization done", game_globals->active_designer_zone_mask);
	game_cinematic_zone_set_debug_status("initialization done", game_globals->active_cinematic_zone_mask);
}

void __cdecl game_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	//INVOKE(0x005317F0, game_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	assert_game_options_verify(&game_globals->options);
	ASSERT(game_globals->map_active);
	ASSERT(game_globals->active_structure_bsp_mask == 0);
	ASSERT(global_structure_bsp_active_mask_get() != 0);

	game_bsp_debug_status("initializing", global_structure_bsp_active_mask_get());
	game_globals->active_structure_bsp_mask = global_structure_bsp_active_mask_get();

	random_seed_allow_use();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		if (g_game_systems[system_index].initialize_for_new_structure_bsp_proc)
			g_game_systems[system_index].initialize_for_new_structure_bsp_proc(activating_structure_bsp_mask);
	}

	random_seed_disallow_use();

	game_bsp_debug_status("initialization done", game_globals->active_structure_bsp_mask);
}

//.text:00531840 ; int16 __cdecl game_insertion_point_get()
//.text:00531870 ; void __cdecl game_insertion_point_lock(int16)
//.text:00531900 ; void __cdecl game_insertion_point_unlock(int16)

bool __cdecl game_is_authoritative()
{
	//return INVOKE(0x005319C0, game_is_authoritative);

	return !game_is_predicted();
}

bool __cdecl game_is_authoritative_playback()
{
	return INVOKE(0x005319F0, game_is_authoritative_playback);
}

bool __cdecl game_is_available()
{
	//return INVOKE(0x00531A30, game_is_available);

	return game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask;
}

bool __cdecl game_is_campaign_or_survival()
{
	return game_is_campaign() || game_is_survival();
}

bool __cdecl game_is_campaign()
{
	//return INVOKE(0x00531A60, game_is_campaign);

	return game_mode_get() == _game_mode_campaign;
}

bool __cdecl game_is_cooperative()
{
	return INVOKE(0x00531A80, game_is_cooperative);

	//return game_is_campaign() && players_get_active_and_in_game_count(true) > 1;
}

//.text:00531AC0 ; bool __cdecl game_is_cross_language()

bool __cdecl game_is_distributed()
{
	//return INVOKE(0x00531AF0, game_is_distributed);

	int8 game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_distributed_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

bool __cdecl game_is_finished()
{
	//return INVOKE(0x00531B20, game_is_finished);

	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished;
}

bool __cdecl game_is_finished_immediate()
{
	//return INVOKE(0x00531B40, game_is_finished_immediate);

	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished && !game_globals->game_finished_timer;
}

//.text:00531B70 ; bool __cdecl game_is_in_progress_on_live()

bool __cdecl game_is_lost()
{
	return INVOKE(0x00531BE0, game_is_lost);

	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_lost;
}

// custom like `game_is_finished_immediate`
bool __cdecl game_is_lost_immediate()
{
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_lost && !game_globals->game_loss_timer;
}

bool __cdecl game_is_multiplayer()
{
	//return INVOKE(0x00531C00, game_is_multiplayer);

	return game_mode_get() == _game_mode_multiplayer;
}

bool __cdecl game_is_networked()
{
	//return INVOKE(0x00531C20, game_is_networked);

	int8 game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

bool __cdecl game_is_or_was_cooperative()
{
	return INVOKE(0x00531C50, game_is_or_was_cooperative);
}

bool __cdecl game_is_playback()
{
	//return INVOKE(0x00531D10, game_is_playback);

	return game_playback_get();
}

bool __cdecl game_is_playtest()
{
	//return INVOKE(0x00531D50, game_is_playtest);

	return game_options_get()->playtest_mode;
}

bool __cdecl game_is_predicted()
{
	//return INVOKE(0x00531D70, game_is_predicted);

	return game_options_get()->game_simulation == _game_simulation_distributed_client;
}

bool __cdecl game_is_server()
{
	//return INVOKE(0x00531DA0, game_is_server);

	int8 game_simulation = game_options_get()->game_simulation;
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
	{
		return true;
	}

	return false;
}

bool __cdecl game_is_splitscreen_deterministic()
{
	return INVOKE(0x00531DD0, game_is_splitscreen_deterministic);

	//int32 player_count = 0;
	//
	//c_player_in_game_iterator player_iterator;
	//player_iterator.begin();
	//while (player_iterator.next())
	//{
	//	if (player_is_local(player_iterator.get_index()))
	//		player_count++;
	//}
	//
	//return player_count > 1;
}

bool __cdecl game_is_survival()
{
	//return INVOKE(0x00531E20, game_is_survival);

	if (game_globals && (game_globals->initializing || game_globals->map_active))
		return game_globals->options.game_mode == _game_mode_campaign && game_globals->options.survival_enabled;

	return g_debug_survival_mode;
}

bool __cdecl game_is_synchronous_networking()
{
	//return INVOKE(0x00531E60, game_is_synchronous_networking);

	return IN_RANGE_INCLUSIVE(game_options_get()->game_simulation,
		_game_simulation_synchronous_client, _game_simulation_synchronous_server);
}

bool __cdecl game_is_ui_shell()
{
	//return INVOKE(0x00531E90, game_is_ui_shell);

	return game_mode_get() == _game_mode_ui_shell;
}

void __cdecl game_launch_initial_script()
{
	INVOKE(0x00531EB0, game_launch_initial_script);

	//char script_name[128]{};
	//if (game_launch_get_initial_script_name(script_name))
	//	hs_wake_by_name(script_name);
}

//.text:00531EE0 ; game_level_advance
//.text:00531FF0 ; prepare_game_level
//.text:00532050 ; game_level_prepare

void __cdecl game_loss_update()
{
	INVOKE(0x005320B0, game_loss_update);
}

//.text:005321B0 ; void __cdecl game_lost_for_scripting(bool)

void __cdecl game_lost(bool lost)
{
	//INVOKE(0x005321E0, game_lost, game_revert);

	ASSERT(game_globals && game_globals->map_active);

	game_globals->game_revert = lost;
	if (lost)
	{
		if (!game_globals->game_lost)
		{
			game_globals->game_lost = true;
			game_globals->game_loss_timer = game_seconds_to_ticks_round(k_game_loss_time);
			game_state_prepare_for_revert();
		}
	}
	else if (game_globals->game_lost)
		game_globals->game_lost = false;
}

e_game_mode __cdecl game_mode_get()
{
	//return INVOKE(0x00532260, game_mode_get);

	return (e_game_mode)game_options_get()->game_mode;
}

void __cdecl game_options_clear_game_playback()
{
	//INVOKE(0x00532280, game_options_clear_game_playback);

	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_playback = _game_playback_none;
}

void __cdecl game_options_game_engine_fixup()
{
	INVOKE(0x005322A0, game_options_game_engine_fixup);

	//ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));
	//
	//game_globals->options.game_variant.recreate_variant_vtable_for_game_engine_index(game_globals->options.game_variant.get_game_engine_index());
}

game_options* __cdecl game_options_get()
{
	//return INVOKE(0x005322D0, game_options_get);

	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

void __cdecl game_options_print_game_id()
{
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	console_printf("%I64d", game_options_get()->game_instance);
}

//.text:005322F0 ; bool __cdecl game_options_match(const game_options*, const game_options*)

void __cdecl game_options_new(game_options* options)
{
	INVOKE(0x005323A0, game_options_new, options);
}

void __cdecl game_options_setup_default_players(int32 player_count, game_options* options)
{
	DECLFUNC(0x00532440, void, __cdecl, int32, game_options*)(player_count, options);
}

//.text:00532610 ; void __cdecl game_options_setup_for_playback(e_game_playback_type)
//void __cdecl game_options_setup_for_saved_film(e_game_playback_type)

bool __cdecl game_options_valid()
{
	//return INVOKE(0x00532650, game_options_valid);

	return game_globals && (game_globals->initializing || game_globals->map_active);
}

void __cdecl game_options_validate(game_options* options)
{
	//INVOKE(0x00532680, game_options_validate, options);

	game_engine_variant_validate(&options->multiplayer_variant);
	options->map_variant.validate();
}

//.text:005326B0 ; void __cdecl game_options_validate_for_saved_game(int32)

bool __cdecl game_options_verify(const game_options* options, char* error_string, int32 error_string_length)
{
	return INVOKE(0x005326F0, game_options_verify, options, error_string, error_string_length);

	//ASSERT(options);
	//ASSERT(error_string);
	//
	//if (!VALID_INDEX(options->game_mode, k_game_mode_count))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"game_mode %d outside valid range [0, %d)",
	//		options->game_mode,
	//		k_game_mode_count);
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->game_simulation, k_game_simulation_count))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"game_simulation %d outside valid range [0, %d)",
	//		options->game_simulation,
	//		k_game_simulation_count);
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->game_tick_rate, 300))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"game_tick_rate %d outside valid range [0, %d)",
	//		options->game_tick_rate,
	//		300);
	//	return false;
	//}
	//
	//if (!options->scenario_path.length())
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"scenario_path is empty");
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->initial_zone_set_index, k_maximum_scenario_zone_set_count))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"initial_zone_set_index %d outside valid range [0, %d)",
	//		options->initial_zone_set_index,
	//		k_maximum_scenario_zone_set_count);
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->game_playback, k_game_playback_count))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"game_playback %d outside valid range [0, %d)",
	//		options->game_playback,
	//		k_game_playback_count);
	//	return false;
	//}
	//
	//if (options->record_saved_film && options->game_playback)
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"cannot simultaneously record and play back a saved film");
	//	return false;
	//}
	//
	//if (options->map_variant.m_map_id != NONE && options->map_id != options->map_variant.m_map_id)
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"map_id 0x%08X does not match map variant map_id 0x%08X",
	//		options->map_id,
	//		options->map_variant.m_map_id);
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->campaign_metagame_scoring, 3))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"metagame scoring option %i is invalid",
	//		options->campaign_metagame_scoring);
	//	return false;
	//}
	//
	//if (!VALID_INDEX(options->campaign_insertion_point, 9))
	//{
	//	csnzprintf(error_string, error_string_length,
	//		"game_playback %d outside valid range [0, %d)",
	//		options->campaign_insertion_point);
	//	return false;
	//}
	//
	//int32 player_count = 0;
	//// $TODO `options->machines` checks
	//// $TODO `options->players` checks
	//
	//// $TODO confirm `options->game_mode` checks
	//switch (options->game_mode)
	//{
	//case _game_mode_campaign:
	//{
	//	if (game_in_editor())
	//	{
	//		if (player_count == 0)
	//		{
	//			csnzprintf(error_string, error_string_length,
	//				"must have at least one player to play in the editor");
	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		if (VALID_INDEX(options->campaign_difficulty, k_campaign_difficulty_levels_count))
	//		{
	//			csnzprintf(
	//				error_string,
	//				error_string_length,
	//				"campaign_difficulty %d outside valid range [0, %d)",
	//				options->campaign_difficulty,
	//				k_campaign_difficulty_levels_count);
	//			return false;
	//		}
	//		if (VALID_COUNT(player_count, k_maximum_campaign_players))
	//		{
	//			csnzprintf(
	//				error_string,
	//				error_string_length,
	//				"campaign_difficulty %d outside valid range [0, %d)",
	//				options->campaign_difficulty,
	//				k_campaign_difficulty_levels_count);
	//			return false;
	//		}
	//		if (!options->machines.local_machine_exists)
	//		{
	//			csnzprintf(error_string, error_string_length,
	//				"local machine must exist in a campaign game");
	//			return false;
	//		}
	//	}
	//}
	//break;
	//case _game_mode_multiplayer:
	//{
	//	if (VALID_COUNT(player_count, k_maximum_multiplayer_players))
	//	{
	//		csnzprintf(
	//			error_string,
	//			error_string_length,
	//			"player_count %d outside valid range [1, %d]",
	//			player_count,
	//			k_maximum_multiplayer_players);
	//		return false;
	//	}
	//	if (!options->machines.local_machine_exists)
	//	{
	//		csnzprintf(error_string, error_string_length,
	//			"local machine must exist in a multiplayer game");
	//		return false;
	//	}
	//}
	//break;
	//case _game_mode_ui_shell:
	//{
	//	if (player_count == 0)
	//	{
	//		csnzprintf(error_string, error_string_length,
	//			"must have at least one player to run the UI shell");
	//		return false;
	//	}
	//	if (!options->machines.local_machine_exists)
	//	{
	//		csnzprintf(error_string, error_string_length,
	//			"local machine must exist in the UI shell");
	//		return false;
	//	}
	//}
	//break;
	//default:
	//{
	//	if (options->game_mode != _game_mode_tool)
	//	{
	//		csnzprintf(error_string, error_string_length,
	//			"game_mode %d is unknown",
	//			options->game_mode);
	//		return false;
	//	}
	//}
	//break;
	//}
	//
	//return true;
}

e_game_playback_type __cdecl game_playback_get()
{
	//return INVOKE(0x00532A80, game_playback_get);

	return (e_game_playback_type)game_options_get()->game_playback;
}

//.text:00532AA0 ; void __cdecl game_playback_set(e_game_playback_type playback_type)

void __cdecl game_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* currently_active_tags_registry)
{
	//INVOKE(0x00532AD0, game_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, currently_active_tags_registry);

	assert_game_options_verify(&game_globals->options);
	ASSERT(game_globals->map_active);
	ASSERT(new_non_bsp_zone_set->designer_zone_mask == 0 || VALID_INDEX(highest_bit_set(new_non_bsp_zone_set->designer_zone_mask), global_scenario_get()->designer_zones.count));
	ASSERT((game_globals->active_designer_zone_mask & old_non_bsp_zone_set->designer_zone_mask) == game_globals->active_designer_zone_mask);
	ASSERT((game_globals->active_cinematic_zone_mask & old_non_bsp_zone_set->cinematic_zone_mask) == game_globals->active_cinematic_zone_mask);

	game_designer_zone_set_debug_status("preparing switch from", game_globals->active_structure_bsp_mask);
	game_designer_zone_set_debug_status("preparing switch to", new_non_bsp_zone_set->designer_zone_mask);
	game_cinematic_zone_set_debug_status("preparing switch from", game_globals->active_cinematic_zone_mask);
	game_cinematic_zone_set_debug_status("preparing switch to", new_non_bsp_zone_set->cinematic_zone_mask);

	random_seed_allow_use();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		if (g_game_systems[system_index].prepare_for_non_bsp_zone_set_switch_proc)
			g_game_systems[system_index].prepare_for_non_bsp_zone_set_switch_proc(old_non_bsp_zone_set, new_non_bsp_zone_set, currently_active_tags_registry);
	}

	objects_purge_deleted_objects();

	random_seed_disallow_use();

	game_designer_zone_set_debug_status("switching from", game_globals->active_structure_bsp_mask);
	game_designer_zone_set_debug_status("switching to", new_non_bsp_zone_set->designer_zone_mask);
	game_cinematic_zone_set_debug_status("switching from", game_globals->active_cinematic_zone_mask);
	game_cinematic_zone_set_debug_status("switching to", new_non_bsp_zone_set->cinematic_zone_mask);
}

void __cdecl game_prepare_to_switch_structure_bsp(uns32 old_structure_bsp_mask, uns32 new_structure_bsp_mask)
{
	//INVOKE(0x00532B10, game_prepare_to_switch_structure_bsp, old_structure_bsp_mask, new_structure_bsp_mask);

	ASSERT(game_globals->map_active);
	ASSERT(new_structure_bsp_mask == 0 || VALID_INDEX(highest_bit_set(new_structure_bsp_mask), global_scenario_get()->structure_bsp_references.count));
	ASSERT(game_globals->active_structure_bsp_mask == old_structure_bsp_mask);

	char status_string[1024]{};

	game_bsp_debug_status("preparing switch from", game_globals->active_structure_bsp_mask);
	game_bsp_debug_status("preparing switch to", new_structure_bsp_mask);

	havok_can_modify_state_allow();

	for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
	{
		if (g_game_systems[system_index].prepare_for_new_zone_set_proc)
			g_game_systems[system_index].prepare_for_new_zone_set_proc(old_structure_bsp_mask, new_structure_bsp_mask);
	}

	objects_purge_deleted_objects();
	havok_can_modify_state_disallow();

	game_bsp_debug_status("switching from", game_globals->active_structure_bsp_mask);
	game_bsp_debug_status("switching to", new_structure_bsp_mask);
}

//.text:00532B50 ; void __cdecl skull_primary_enable
//.text:00532B90 ; game_get_game_progression

void __cdecl game_pvs_clear_scripted_camera_pvs()
{
	//INVOKE(0x00532BB0, game_pvs_clear_scripted_camera_pvs);

	if (game_globals)
		game_globals->pvs_use_scripted_camera = false;
}

void __cdecl game_pvs_enable_scripted_camera_pvs()
{
	//INVOKE(0x00532BD0, game_pvs_enable_scripted_camera_pvs);

	if (game_globals)
		game_globals->pvs_use_scripted_camera = true;
}

void __cdecl game_pvs_scripted_clear()
{
	//INVOKE(0x00532BF0, game_pvs_scripted_clear);

	if (game_globals)
		game_globals->pvs_activation_type = 0;
}

s_cluster_reference __cdecl game_pvs_scripted_get_cluster_reference()
{
	return INVOKE(0x00532C10, game_pvs_scripted_get_cluster_reference);
}

//.text:00532CB0 ; void __cdecl game_pvs_scripted_set_camera_point(int16 camera_point_index)

void __cdecl game_pvs_scripted_set_object(int32 object_index)
{
	//INVOKE(0x00532D40, game_pvs_scripted_set_object, object_index);

	if (!game_globals)
		return;

	if (object_index == NONE)
	{
		game_globals->pvs_activation_type = 0;
	}
	else
	{
		game_globals->pvs_activation_type = 1;
		game_globals->pvs_activation.object_index = object_index;
	}
}

//.text:00532D90 ; void __cdecl update_controller_game_progression()

void __cdecl game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable)
{
	//INVOKE(0x00532EE0, game_skull_enable_secondary, secondary_skull, enable);

	if (game_globals)
		SET_BIT(game_globals->active_secondary_skulls, secondary_skull, enable);
}

//.text:00532F20 ; game_set_active_primary_skulls
//.text:00532F50 ; game_set_active_secondary_skulls

bool __cdecl game_skull_is_active_primary(e_campaign_skulls_primary primary_skull)
{
	if (game_globals)
		return game_globals->active_primary_skulls == primary_skull;

	return false;
}

bool __cdecl game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull)
{
	if (game_globals)
		return game_globals->active_secondary_skulls == secondary_skull;

	return false;
}

void __cdecl game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable)
{
	if (game_globals)
		SET_BIT(game_globals->active_primary_skulls, primary_skull, enable);
}

void __cdecl game_set_active_skulls(uns32* active_primary_skulls, uns32* active_secondary_skulls)
{
	if (game_globals)
	{
		if (active_primary_skulls)
			*active_primary_skulls = game_globals->active_primary_skulls;

		if (active_secondary_skulls)
			*active_secondary_skulls = game_globals->active_secondary_skulls;
	}
}

void __cdecl game_set_difficulty(int16 campaign_difficulty)
{
	//INVOKE(0x00532F80, game_set_difficulty, campaign_difficulty);

	if (game_in_progress() &&
		game_is_campaign() &&
		campaign_difficulty >= _campaign_difficulty_level_easy &&
		campaign_difficulty < k_campaign_difficulty_levels_count)
	{
		game_options_get()->campaign_difficulty = campaign_difficulty;
	}
}

e_game_simulation_type __cdecl game_simulation_get()
{
	//return INVOKE(0x00532FD0, game_simulation_get);

	return (e_game_simulation_type)game_options_get()->game_simulation;
}

void __cdecl game_simulation_set(e_game_simulation_type game_simulation)
{
	//INVOKE(0x00532FF0, game_simulation_set, game_simulation);

	ASSERT(game_simulation > _game_simulation_none && game_simulation < k_game_simulation_count);
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_simulation = game_simulation;
	main_status("game_simulation: %s", k_game_simulation_names[game_simulation]);
}

void __cdecl game_start(e_game_create_mode mode)
{
	//INVOKE(0x00533030, game_start, mode);

	ASSERT(game_globals);
	ASSERT(!game_globals->initializing);
	ASSERT(game_globals->map_active);
	ASSERT(game_globals->active_structure_bsp_mask != 0);
	ASSERT(!game_globals->game_in_progress);

	game_globals->game_in_progress = true;
	simulation_start();

	int32 lock = game_create_lock_resources(mode);
	//random_seed_allow_use();

	game_engine_game_starting();
	game_launch_initial_script();

	//random_seed_disallow_use();
	game_create_unlock_resources(mode, lock);
}

bool __cdecl game_survival_allow_respawn()
{
	//return INVOKE(0x005330B0, game_survival_allow_respawn);

	return game_globals && game_globals->options.game_mode == _game_mode_campaign && !game_skull_is_active_primary(_campaign_skull_primary_iron);
}

//.text:005330D0 ; bool __cdecl game_test_cluster_activation(const s_cluster_reference*)

void __cdecl game_tick()
{
	//INVOKE(0x00533120, game_tick);

	PROFILER(game_tick)
	{
		struct simulation_update update = { .flags = 0 };
		s_simulation_update_metadata metadata = { .flags = 0 };

		game_globals->game_had_an_update_tick_this_frame = true;
		main_status(__FUNCTION__, "time %d", game_time_get());

		PROFILER(build_simulation_update)
		{
			real_math_reset_precision();

			simulation_build_update(true, &update, &metadata);
			simulation_record_update(&update);

			game_state_preserve();
		}

		render_debug_notify_game_tick_begin();
		c_rasterizer::notify_game_tick_begin();
		c_water_renderer::game_update();
		damage_acceleration_queue_begin();
		simulation_apply_before_game(&update);
		levels_update();

		if (update.flags.test(_simulation_update_simulation_in_progress_bit))
		{
			chud_game_tick();
			players_update_before_game(&update);
			sound_update();
			game_tick_pulse_random_seed_deterministic(&update);
			ai_update();
			recorded_animations_update();
			game_sound_deterministic_update_timers();
			game_engine_update();
			game_results_update();
			editor_update();
			cinematics_game_tick();

			RENDER_ENABLED(true)
			{
				c_hue_saturation_control::copy_from_gamestate();
			}

			hs_update();

			RENDER_ENABLED(true)
			{
				c_hue_saturation_control::copy_to_gamestate();
			}

			BOT_CLIENT(false)
			{
				game_update_pvs();
			}

			object_scheduler_update();
			object_activation_regions_update();
			objects_update();
			damage_acceleration_queue_end();
			havok_proxies_update();
			havok_update();
			havok_proxies_move();
			objects_move();
			objects_post_update();

			BOT_CLIENT(false)
			{
				impacts_update();
			}

			breakable_surfaces_update();
			effects_update();
			lights_update();
			game_engine_update_after_game();
			simulation_apply_after_game(&update);
			players_update_after_game(&update);

			PROFILER(high_level_game_systems)
			{
				campaign_metagame_update();
				game_allegiance_update();
				game_loss_update();
				game_finished_update();

				// odst achievement function?
				sub_6967B0();

				game_save_update();
				cinematic_update();
				s_depth_of_field::update();
				game_grief_update();
				//achievements_update();
				test_functions_update();
			}

			PROFILER(interface_system_update)
			{
				first_person_weapons_update();
				player_effect_update();
				overhead_map_update();
				observer_game_tick();
				director_game_tick();
			}
		}
		else
		{
			damage_acceleration_queue_end();
		}
		simulation_update_aftermath(&update, &metadata);

		if (update.flags.test(_simulation_update_simulation_in_progress_bit))
			game_time_advance();

		simulation_destroy_update(&update);
		main_status(__FUNCTION__, NULL);
		render_debug_notify_game_tick_end();
	}
}

void __cdecl game_tick_pulse_random_seed_deterministic(const struct simulation_update* update)
{
	INVOKE(0x005333A0, game_tick_pulse_random_seed_deterministic, update);
}

int32 __cdecl game_tick_rate_get()
{
	//return INVOKE(0x00533470, game_tick_rate_get);

	return game_options_get()->game_tick_rate;
}

void __cdecl game_time_get_date_and_time(s_date_and_time* date_and_time)
{
	INVOKE(0x00533490, game_time_get_date_and_time, date_and_time);
}

//.text:00533500 ; e_game_time_holiday __cdecl game_time_get_holiday()
//.text:005335E0 game_progression_get_previous_map_block_index
//.text:00533640 ; 
//.text:00533690 ; game_prepare_for_progression

void __cdecl game_update(int32 tick_count, real32* game_seconds_elapsed)
{
	//INVOKE(0x005336F0, game_update, tick_count, game_seconds_elapsed);

	PROFILER(game_update)
	{
		render_thread_get_mode();

		game_globals->game_had_an_update_tick_this_frame = game_simulation_get() == _game_simulation_synchronous_server && tick_count == 1;

		int32 actual_ticks;
		for (actual_ticks = 0; actual_ticks < tick_count && !main_events_pending() && !simulation_aborted(); actual_ticks++)
		{
			game_tick();
			game_globals->game_had_an_update_tick_this_frame = false;
		}

		if (actual_ticks < tick_count)
			game_time_discard(tick_count, actual_ticks, game_seconds_elapsed);
	}
}

//.text:005339C0 ; 

void __cdecl game_update_pvs()
{
	//INVOKE(0x00533B80, game_update_pvs);

	PROFILER(game_update_pvs)
	{
		s_game_cluster_bit_vectors cluster_pvs = game_globals->cluster_pvs;
		s_game_cluster_bit_vectors cluster_pvs_local = game_globals->cluster_pvs_local;
		s_game_cluster_bit_vectors cluster_activation = game_globals->cluster_activation;
		g_cluster_activation_reason.clear();

		game_compute_pvs(&game_globals->cluster_pvs, false, &g_cluster_activation_reason);
		game_compute_pvs(&game_globals->cluster_pvs_local, true, NULL);

		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			structure_bsp* bsp = global_structure_bsp_get(structure_bsp_index);

			uns32 active_clusters[8]{};
			if (ai_get_active_clusters(structure_bsp_index, active_clusters, bsp->clusters.count))
			{
				for (int32 cluster_index = 0; cluster_index < bsp->clusters.count; cluster_index++)
				{
					if (!game_globals->cluster_pvs.flags[structure_bsp_index].test(cluster_index) && BIT_VECTOR_TEST_FLAG(active_clusters, cluster_index))
					{
						g_cluster_activation_reason[structure_bsp_index][cluster_index] = 2;
					}
				}

				bit_vector_or(
					bsp->clusters.count,
					game_globals->cluster_pvs.flags[structure_bsp_index].get_bits_direct(),
					active_clusters,
					game_globals->cluster_activation.flags[structure_bsp_index].get_writeable_bits_direct());
			}
			else
			{
				csmemcpy(
					game_globals->cluster_activation.flags.element(structure_bsp_index).get_writeable_bits_direct(),
					game_globals->cluster_pvs.flags.element(structure_bsp_index).get_bits_direct(),
					sizeof(game_globals->cluster_pvs.flags.get_element_size())
				);
			}
		}

		s_cluster_reference scripted_cluster = game_pvs_scripted_get_cluster_reference();
		if (scripted_cluster.bsp_index != NONE)
		{
			s_scenario_pvs_row pvs_row{};
			scenario_zone_set_pvs_get_row(global_scenario_index_get(), &pvs_row, scenario_zone_set_index_get(), scripted_cluster, false);

			s_game_cluster_bit_vectors clusters_from_pvs_row{};
			scenario_zone_set_pvs_write_row(&clusters_from_pvs_row, &pvs_row);

			for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
				structure_bsp_index != NONE;
				structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
			{
				structure_bsp* bsp = global_structure_bsp_get(structure_bsp_index);

				ASSERT(VALID_COUNT(bsp->clusters.count, MAXIMUM_CLUSTERS_PER_STRUCTURE));

				for (int32 cluster_index = 0; cluster_index < bsp->clusters.count; cluster_index++)
				{
					s_cluster_reference cluster_reference{};
					cluster_reference_set(&cluster_reference, structure_bsp_index, cluster_index);
					if (!game_clusters_test(&game_globals->cluster_pvs, cluster_reference) && game_clusters_test(&clusters_from_pvs_row, cluster_reference))
					{
						g_cluster_activation_reason[structure_bsp_index][cluster_index] = 3;
					}
				}
			}

			game_clusters_or(&game_globals->cluster_pvs, &clusters_from_pvs_row, &game_globals->cluster_pvs);
		}

		if (csmemcmp(&cluster_pvs, &game_globals->cluster_pvs, sizeof(s_game_cluster_bit_vectors)))
		{
			for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
			{
				if (g_game_systems[system_index].change_pvs_proc)
					g_game_systems[system_index].change_pvs_proc(&cluster_pvs, &game_globals->cluster_pvs, false);
			}
		}

		if (csmemcmp(&cluster_pvs_local, &game_globals->cluster_pvs_local, sizeof(s_game_cluster_bit_vectors)))
		{
			for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
			{
				if (g_game_systems[system_index].change_pvs_proc)
					g_game_systems[system_index].change_pvs_proc(&cluster_pvs_local, &game_globals->cluster_pvs_local, true);
			}
		}

		if (csmemcmp(&cluster_activation, &game_globals->cluster_activation, sizeof(s_game_cluster_bit_vectors)))
		{
			for (int32 system_index = 0; system_index < g_game_system_count; system_index++)
			{
				if (g_game_systems[system_index].activation_proc)
					g_game_systems[system_index].activation_proc(&cluster_activation, &game_globals->cluster_activation);
			}
		}
	}
}

void __cdecl game_won()
{
	INVOKE(0x00533E40, game_won);
}

//.text:00533EF0 ; 
//.text:00533FC0 ; const char* __cdecl get_game_difficulty_name(int16)

void __cdecl game_finish_immediate()
{
	ASSERT(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		game_finish();
		game_globals->game_finished_timer = 0;
	}
}

bool __cdecl game_options_read_launch_settings_from_string(const char* buffer, s_game_options_launch_settings* out_launch_settings)
{
	s_game_options_launch_settings launch_settings{};

	*launch_settings.scenario_path = 0;
	//*launch_settings.insertion_point_name = 0;
	//*launch_settings.zone_set_name = 0;

	if (sscanf_s(buffer, "%d\n%s\n%d\n%d\n%d\n%d\n%hd\n%hd\n%d\n",
		&launch_settings.build_number,
		launch_settings.scenario_path, 256,
		&launch_settings.campaign_difficulty,
		&launch_settings.game_mode,
		&launch_settings.game_engine_index,
		&launch_settings.player_count,
		&launch_settings.insertion_point,
		&launch_settings.zone_set_index,
		&launch_settings.launch_file_flags))
	{
		//launch_settings.insertion_point_name[0] &= (strncmp(launch_settings.insertion_point_name, "-", 128) == 0) - 1;
		//launch_settings.zone_set_name[0] &= (strncmp(launch_settings.zone_set_name, "-", 128); == 0) - 1;
		csmemcpy(out_launch_settings, &launch_settings, sizeof(s_game_options_launch_settings));
		return true;
	}

	return false;
}

bool __cdecl game_launch_get_settings(s_game_options_launch_settings* out_launch_settings)
{
	bool result = false;

	FILE* launch_file;
	if (fopen_s(&launch_file, "launch.txt", "r") == 0 && launch_file)
	{
		char buffer[1072]{};
		if (fread_s(buffer, 1024, 1, 1024, launch_file))
		{
			s_game_options_launch_settings launch_settings{};
			*launch_settings.scenario_path = 0;
			//*launch_settings.insertion_point_name = 0;
			//*launch_settings.zone_set_name = 0;

			if (game_options_read_launch_settings_from_string(buffer, &launch_settings) && launch_settings.build_number == version_get_build_number())
			{
				csmemcpy(out_launch_settings, &launch_settings, sizeof(s_game_options_launch_settings));
				result = true;
			}
		}

		fclose(launch_file);
	}

	return result;
}

bool __cdecl game_launch_get_initial_script_name(const char* script_name)
{
	//return INVOKE(0x006961B0, game_launch_get_initial_script_name, script_name);

	// fix for hanger background not being loaded
	if (game_is_ui_shell())
		user_interface_start_hs_script_by_name("humanhangar");

	return false;
}

bool __cdecl game_options_get_launch_settings(game_options* options, bool change_in_progress)
{
	//return INVOKE(0x006961C0, game_options_get_launch_settings, options, change_in_progress);

	ASSERT(options);

	s_game_options_launch_settings launch_settings{};
	*launch_settings.scenario_path = 0;
	//*launch_settings.insertion_point_name = 0;
	//*launch_settings.zone_set_name = 0;
	launch_settings.player_count = 1;

	if (!game_launch_get_settings(&launch_settings))
		return false;

	game_options_new(options);
	options->scenario_path.set(launch_settings.scenario_path);
	options->game_mode = launch_settings.game_mode;
	options->record_saved_film = saved_film_manager_should_record_film(options);
	options->campaign_difficulty = launch_settings.campaign_difficulty;

	build_default_game_variant(&options->multiplayer_variant, options->game_mode != _game_mode_multiplayer ? _game_engine_type_none : launch_settings.game_engine_index);

	options->campaign_insertion_point = launch_settings.insertion_point;
	options->initial_zone_set_index = launch_settings.zone_set_index;
	game_options_setup_default_players(launch_settings.player_count, options);

	if (TEST_BIT(launch_settings.launch_file_flags, 1))
	{
		s_file_reference to_delete{};
		file_reference_create_from_path(&to_delete, "launch.txt", false);
		file_delete(&to_delete);
	}

	return true;
}

void game_bsp_debug_status(const char* status, uns32 structure_bsp_mask)
{
	ASSERT(status);

	char structure_bsp_string[1024]{};
	scenario_get_structure_bsp_string_from_mask(structure_bsp_mask, structure_bsp_string, sizeof(structure_bsp_string));
	main_status("bsp", "%s 0x%x (%s)", status, structure_bsp_mask, structure_bsp_string);
}

void game_designer_zone_set_debug_status(const char* status, uns32 designer_zone_mask)
{
	ASSERT(status);

	char designer_zone_string[1024]{};
	scenario_get_designer_zone_string_from_mask(designer_zone_mask, designer_zone_string, sizeof(designer_zone_string));
	main_status("designer_zone", "%s 0x%x (%s)", status, designer_zone_mask, designer_zone_string);
}

void game_cinematic_zone_set_debug_status(const char* status, uns32 cinematic_zone_mask)
{
	ASSERT(status);

	char cinematic_zone_string[1024]{};
	scenario_get_cinematic_zone_string_from_mask(cinematic_zone_mask, cinematic_zone_string, sizeof(cinematic_zone_string));
	main_status("cinematic_zone", "%s 0x%x (%s)", status, cinematic_zone_mask, cinematic_zone_string);
}

void __cdecl game_pvs_debug_render()
{
	if (game_in_progress() && game_get_active_structure_bsp_mask())
	{
		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			structure_bsp* bsp = global_structure_bsp_get(structure_bsp_index);
			for (int32 cluster_index = 0; cluster_index < bsp->clusters.count; cluster_index++)
			{
				const real_argb_color* activation_color = nullptr;
				if (debug_pvs_activation)
				{
					ASSERT(g_cluster_activation_reason[structure_bsp_index][cluster_index] >= 0 && g_cluster_activation_reason[structure_bsp_index][cluster_index] < NUMBEROF(k_activation_colors));
					if (k_activation_colors[g_cluster_activation_reason[structure_bsp_index][cluster_index]])
						activation_color = k_activation_colors[g_cluster_activation_reason[structure_bsp_index][cluster_index]];

					if (!activation_color && VALID_INDEX(k_activation_color_override_index, NUMBEROF(k_activation_colors)))
						activation_color = k_activation_colors[k_activation_color_override_index];
				}

				if (activation_color)
				{
					real_rectangle3d* bounds = &bsp->clusters[cluster_index].bounds;
					render_debug_box_outline(true, bounds, activation_color);

					real_point3d point0{};
					real_point3d point1{};

					point0.x = bounds->x0;
					point0.y = bounds->y0;
					point0.z = bounds->z0;
					point1.x = bounds->x1;
					point1.y = bounds->y1;
					point1.z = bounds->z1;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x1;
					point0.y = bounds->y0;
					point0.z = bounds->z0;
					point1.x = bounds->x0;
					point1.y = bounds->y1;
					point1.z = bounds->z1;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x0;
					point0.y = bounds->y1;
					point0.z = bounds->z0;
					point1.x = bounds->x1;
					point1.y = bounds->y0;
					point1.z = bounds->z1;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x1;
					point0.y = bounds->y1;
					point0.z = bounds->z0;
					point1.x = bounds->x0;
					point1.y = bounds->y0;
					point1.z = bounds->z1;
					render_debug_line(true, &point0, &point1, activation_color);
				}
			}
		}
	}
}

const char* const k_game_simulation_names[k_game_simulation_count]
{
	"none",
	"local",
	"sync-client",
	"sync-server",
	"dist-client",
	"dist-server"
};

const char* const k_game_playback_names[k_game_playback_count]
{
	"none",
	"local",
	"network-server",
	"network-client"
};

#define DECLARE_GAME_SYSTEM_NAME(NAME) .name = #NAME,
#define DECLARE_GAME_SYSTEM_MEMBER(SYSTEM, PROC) .PROC##_proc = SYSTEM##_##PROC
#define DECLARE_GAME_SYSTEM_MEMBER2(SYSTEM, PROC) .PROC##_proc = SYSTEM##::##PROC
#define DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(ADDRESS, PROC) .PROC##_proc = reinterpret_cast<decltype(s_game_system::PROC##_proc)>(ADDRESS)

s_game_system const g_game_systems[]
{
	{
		DECLARE_GAME_SYSTEM_NAME(determinism_debug_manager)
		DECLARE_GAME_SYSTEM_MEMBER(determinism_debug_manager, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530270, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(determinism_debug_manager, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530280, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(determinism_debug_manager, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530290, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(determinism_debug_manager, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302A0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(optional_cache)
		DECLARE_GAME_SYSTEM_MEMBER(optional_cache, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00603ED0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(optional_cache, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00603EB0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(screenshots_loader)
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_loader, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00680DF0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_loader, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00680DE0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(transport)
		DECLARE_GAME_SYSTEM_MEMBER(transport, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004306F0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(transport, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00430650, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(runtime_state)
		DECLARE_GAME_SYSTEM_MEMBER(runtime_state, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00509E00, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(runtime_state, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00509DC0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(runtime_state, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00509E10, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(runtime_state, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00509DD0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(c_structure_renderer)
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EF10, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EDA0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EF20, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EDB0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EF70, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_structure_renderer, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2EE00, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(data_mine)
		DECLARE_GAME_SYSTEM_MEMBER(data_mine, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005AC790, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(data_mine, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005AC740, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(overlapped)
		DECLARE_GAME_SYSTEM_MEMBER(overlapped, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005A8DE0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(overlapped, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005A8DD0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(random_math)
		DECLARE_GAME_SYSTEM_MEMBER(random_math, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0051F0B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(random_math, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0051F070, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(network)
		DECLARE_GAME_SYSTEM_MEMBER(network, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0049E1B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(network, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0049E050, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(network_webstats)
		DECLARE_GAME_SYSTEM_MEMBER(network_webstats, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004E4040, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(network_webstats, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004E4010, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(xbox_connection)
		DECLARE_GAME_SYSTEM_MEMBER(xbox_connection, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E2AC0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(xbox_connection, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E2AB0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(remote_command)
		DECLARE_GAME_SYSTEM_MEMBER(remote_command, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E2A70, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(remote_command, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E2A60, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(telnet_console)
		DECLARE_GAME_SYSTEM_MEMBER(telnet_console, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301A0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(telnet_console, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301B0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(console)
		DECLARE_GAME_SYSTEM_MEMBER(console, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00505000, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(console, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00504FF0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(input_abstraction)
		DECLARE_GAME_SYSTEM_MEMBER(input_abstraction, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0060C390, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(input_abstraction, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0060B3A0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(input_abstraction, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0060C430, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(input_abstraction, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0060B3C0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(collision_log)
		DECLARE_GAME_SYSTEM_MEMBER(collision_log, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530300, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(collision_log, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530310, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(levels)
		DECLARE_GAME_SYSTEM_MEMBER(levels, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054C110, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(levels, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054ADF0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(levels, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054C2D0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(levels, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054AEA0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(visibility_collection)
		DECLARE_GAME_SYSTEM_MEMBER(visibility_collection, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006874F0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(visibility_collection, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006874E0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_grief)
		DECLARE_GAME_SYSTEM_MEMBER(game_grief, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678FD0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_grief, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678F90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_grief, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678FF0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_grief, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678FA0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(achievements)
		DECLARE_GAME_SYSTEM_MEMBER(achievements, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E2320, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(achievements, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014E22D0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_state)
		DECLARE_GAME_SYSTEM_MEMBER(game_state, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050FDB0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050F5D0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050FDC0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050F5E0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050FED0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050F600, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, initialize_for_new_non_bsp_zone_set), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050FEB0, initialize_for_new_non_bsp_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(game_state, dispose_from_old_non_bsp_zone_set),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0050F5F0, dispose_from_old_non_bsp_zone_set),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_time)
		DECLARE_GAME_SYSTEM_MEMBER(game_time, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00564ED0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_time, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00564D10, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_time, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00564F30, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_time, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00564D20, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(profiler)
		DECLARE_GAME_SYSTEM_MEMBER(profiler, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530230, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(profiler, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530240, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(profiler, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530250, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(profiler, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530260, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_allegiance)
		DECLARE_GAME_SYSTEM_MEMBER(game_allegiance, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00670220, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_allegiance, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0066FFC0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_allegiance, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00670290, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_allegiance, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0066FFD0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(players)
		DECLARE_GAME_SYSTEM_MEMBER(players, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005422C0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(players, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00541AF0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(players, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00542340, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(players, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00541B30, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(players, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00542460, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(players, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00541B70, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(player_control)
		DECLARE_GAME_SYSTEM_MEMBER(player_control, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D2EE0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(player_control, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D0A40, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(player_control, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D3120, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(player_control, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D0A50, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(player_training)
		DECLARE_GAME_SYSTEM_MEMBER(player_training, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00689B90, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(player_training, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00689A20, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(player_training, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00689BF0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(player_training, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00689A30, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_engine)
		DECLARE_GAME_SYSTEM_MEMBER(game_engine, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00550FF0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_engine, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054E290, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_engine, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005511A0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_engine, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0054E2A0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(simulation)
		DECLARE_GAME_SYSTEM_MEMBER(simulation, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004419C0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(simulation, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00441230, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(simulation, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00441A40, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(simulation, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00441270, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(scenario)
		DECLARE_GAME_SYSTEM_MEMBER(scenario, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004EA340, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(scenario, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004E9F90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(scenario, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004EA350, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004E9FA0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004EA370, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(scenario, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x004E9FB0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(physics_constants)
		DECLARE_GAME_SYSTEM_MEMBER(physics_constants, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006816B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(physics_constants, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681680, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(physics_constants, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681770, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(physics_constants, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006816A0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(collision_debug)
		DECLARE_GAME_SYSTEM_MEMBER(collision_debug, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(collision_debug, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302C0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(objects)
		DECLARE_GAME_SYSTEM_MEMBER(objects, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B35F50, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(objects, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B35380, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(objects, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B36130, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(objects, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B35430, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(objects, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B36310, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(objects, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B35530, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(objects, activation),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B34D60, activation),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_early_movers)
		DECLARE_GAME_SYSTEM_MEMBER(object_early_movers, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B98A20, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_early_movers, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B989E0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_early_movers, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B98A90, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_early_movers, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B98A00, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_scripting)
		DECLARE_GAME_SYSTEM_MEMBER(object_scripting, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B95CD0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_scripting, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B959F0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_scripting, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B95D30, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_scripting, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B95A10, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_scheduler)
		DECLARE_GAME_SYSTEM_MEMBER(object_scheduler, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B99E90, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_scheduler, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B99E60, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_scheduler, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B99EF0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_scheduler, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B99E80, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_activation_regions)
		DECLARE_GAME_SYSTEM_MEMBER(object_activation_regions, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B9A5D0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_activation_regions, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B9A590, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_activation_regions, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B9A610, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_activation_regions, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B9A5B0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(scenario_kill_trigger_volumes)
		DECLARE_GAME_SYSTEM_MEMBER(scenario_kill_trigger_volumes, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C560, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_kill_trigger_volumes, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C520, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_kill_trigger_volumes, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C5C0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_kill_trigger_volumes, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C540, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(scenario_sky_objects)
		DECLARE_GAME_SYSTEM_MEMBER(scenario_sky_objects, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678EF0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_sky_objects, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678ED0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_sky_objects, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678F00, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_sky_objects, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678EE0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(scenario_soft_ceilings)
		DECLARE_GAME_SYSTEM_MEMBER(scenario_soft_ceilings, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00674C50, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_soft_ceilings, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00674960, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_soft_ceilings, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00674CB0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_soft_ceilings, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00674970, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(campaign_metagame)
		DECLARE_GAME_SYSTEM_MEMBER(campaign_metagame, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00607C80, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(campaign_metagame, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00606F90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(campaign_metagame, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00607CE0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(campaign_metagame, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00606FB0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(autosave_queue)
		DECLARE_GAME_SYSTEM_MEMBER(autosave_queue, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067BEC0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(autosave_queue, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067BDA0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(autosave_queue, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067BED0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(autosave_queue, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067BDB0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(saved_game_files)
		DECLARE_GAME_SYSTEM_MEMBER(saved_game_files, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00526840, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(saved_game_files, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00525DA0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(saved_game_files, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00526890, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(saved_game_files, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00525DB0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(survival_mode)
		DECLARE_GAME_SYSTEM_MEMBER(survival_mode, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00689140, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(survival_mode, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00688DE0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(survival_mode, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006891F0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(survival_mode, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00688E10, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(c_rasterizer)
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1FD20, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1F910, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1FEC0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1F950, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1FF40, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_rasterizer, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A1F9A0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(render)
		DECLARE_GAME_SYSTEM_MEMBER(render, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A29900, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(render, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A296F0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(render, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A29970, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(render, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A29730, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(render, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A29980, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(render, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A29750, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(structures)
		DECLARE_GAME_SYSTEM_MEMBER(structures, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006783E0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(structures, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006783B0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(structures, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006783F0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(structures, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006783C0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(structures, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678400, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(structures, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006783D0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(breakable_surfaces)
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056B6E0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056ADC0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056B770, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056ADF0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056B800, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(breakable_surfaces, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0056AE10, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(director)
		DECLARE_GAME_SYSTEM_MEMBER(director, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00591C40, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(director, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005917B0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(director, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00591CE0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(director, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005917C0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(observer)
		DECLARE_GAME_SYSTEM_MEMBER(observer, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006128B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(observer, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00612520, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(observer, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00612A50, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(observer, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00612530, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(observer, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00612AA0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(observer, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00612540, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(s_depth_of_field)
		DECLARE_GAME_SYSTEM_MEMBER2(s_depth_of_field, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3ECD0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(s_depth_of_field, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3EAB0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(s_depth_of_field, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3ED30, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(s_depth_of_field, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3EAC0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(c_water_renderer)
		DECLARE_GAME_SYSTEM_MEMBER2(c_water_renderer, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A35D70, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(c_water_renderer, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A35AE0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(c_water_renderer, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A36220, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_water_renderer, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A35BA0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(render_texture_camera)
		DECLARE_GAME_SYSTEM_MEMBER(render_texture_camera, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3ACD0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(render_texture_camera, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3AB90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(render_texture_camera, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3AD30, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(render_texture_camera, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3ABA0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(render_hud_camera)
		DECLARE_GAME_SYSTEM_MEMBER(render_hud_camera, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2DB50, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(render_hud_camera, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2D880, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(render_hud_camera, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2DBB0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(render_hud_camera, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2D890, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(s_scripted_exposure)
		DECLARE_GAME_SYSTEM_MEMBER2(s_scripted_exposure, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2AA90, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(s_scripted_exposure, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2A7E0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(s_scripted_exposure, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2AAF0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(s_scripted_exposure, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A2A7F0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(s_render_game_state)
		DECLARE_GAME_SYSTEM_MEMBER2(s_render_game_state, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3E750, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(s_render_game_state, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3E6D0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(s_render_game_state, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3E7B0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(s_render_game_state, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3E6E0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(c_decal_system)
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006931F0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00691B00, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006936C0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00691CD0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, prepare_for_new_zone_set),            //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302D0, prepare_for_new_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00693740, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00691D70, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, change_pvs),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068FBB0, change_pvs),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, prepare_for_non_bsp_zone_set_switch), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006941B0, prepare_for_non_bsp_zone_set_switch),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, initialize_for_new_non_bsp_zone_set), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302E0, initialize_for_new_non_bsp_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER2(c_decal_system, dispose_from_old_non_bsp_zone_set),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005302F0, dispose_from_old_non_bsp_zone_set),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(effects)
		DECLARE_GAME_SYSTEM_MEMBER(effects, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BC740, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(effects, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BBE70, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(effects, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BC990, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(effects, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BBF10, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(effects, prepare_for_new_zone_set),            //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530200, prepare_for_new_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(effects, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BCA80, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(effects, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BBFD0, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(effects, prepare_for_non_bsp_zone_set_switch), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005BCE50, prepare_for_non_bsp_zone_set_switch),
		DECLARE_GAME_SYSTEM_MEMBER(effects, initialize_for_new_non_bsp_zone_set), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530210, initialize_for_new_non_bsp_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(effects, dispose_from_old_non_bsp_zone_set),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530220, dispose_from_old_non_bsp_zone_set),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(point_physics)
		DECLARE_GAME_SYSTEM_MEMBER(point_physics, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00695C90, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(point_physics, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00695C70, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(point_physics, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00695CA0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(point_physics, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00695C80, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(c_atmosphere_fog_interface)
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00673820, initialize),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530180, dispose),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00673880, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530170, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006738F0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00530190, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER2(c_atmosphere_fog_interface, change_pvs),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00672230, change_pvs),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(screen_effect)
		DECLARE_GAME_SYSTEM_MEMBER(screen_effect, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00683010, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(screen_effect, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00682EF0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(screen_effect, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00683040, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(screen_effect, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00682F20, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(sound_classes)
		DECLARE_GAME_SYSTEM_MEMBER(sound_classes, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006612C0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(sound_classes, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00661290, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(sound_classes, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00661330, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(sound_classes, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006612B0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(sound)
		DECLARE_GAME_SYSTEM_MEMBER(sound, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00517960, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(sound, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00516A30, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(sound, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00517CF0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(sound, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00516A90, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(sound, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00517F20, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(sound, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00516BD0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_sound_deterministic)
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_deterministic, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068CBE0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_deterministic, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068CA10, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_deterministic, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068CD40, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_deterministic, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068CA40, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_sound)
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D92B0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D86D0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D93D0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D8700, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D9570, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005D8750, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_sound_player_effects)
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_player_effects, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678630, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_player_effects, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678600, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_player_effects, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678690, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_sound_player_effects, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00678610, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(rumble)
		DECLARE_GAME_SYSTEM_MEMBER(rumble, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00615700, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(rumble, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006156E0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(rumble, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00615760, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(rumble, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006156F0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(player_effect)
		DECLARE_GAME_SYSTEM_MEMBER(player_effect, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00684A00, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(player_effect, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00684650, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(player_effect, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00684A60, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(player_effect, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00684660, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(user_interface)
		DECLARE_GAME_SYSTEM_MEMBER(user_interface, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A84580, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(user_interface, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A84270, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(user_interface, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A84650, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(user_interface, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A842A0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(interface)
		DECLARE_GAME_SYSTEM_MEMBER(interface, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AAE610, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(interface, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADC50, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(interface, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AAE620, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(interface, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADC60, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(chud)
		DECLARE_GAME_SYSTEM_MEMBER(chud, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A89790, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(chud, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A88D70, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(chud, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A897A0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(chud, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A88D80, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(overhead_map)
		DECLARE_GAME_SYSTEM_MEMBER(overhead_map, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADC00, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(overhead_map, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADBE0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(overhead_map, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADC10, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(overhead_map, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADBF0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(cheats)
		DECLARE_GAME_SYSTEM_MEMBER(cheats, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301C0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(cheats, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301D0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(cheats, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301E0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(cheats, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005301F0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(cinematic)
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067D2A0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067CE30, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067D300, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067CE40, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, prepare_for_new_zone_set),            //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067DCE0, prepare_for_new_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067D410, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067CE80, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, prepare_for_non_bsp_zone_set_switch), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067DCF0, prepare_for_non_bsp_zone_set_switch),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, initialize_for_new_non_bsp_zone_set), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067D400, initialize_for_new_non_bsp_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(cinematic, dispose_from_old_non_bsp_zone_set),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067CE70, dispose_from_old_non_bsp_zone_set),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(closed_caption)
		DECLARE_GAME_SYSTEM_MEMBER(closed_caption, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A98CF0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(closed_caption, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A98C10, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(closed_caption, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A98D20, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(closed_caption, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A98C40, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(screenshots_uploader)
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_uploader, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619680, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_uploader, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619610, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_uploader, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619690, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(screenshots_uploader, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619620, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(spartan_program_handler)
		DECLARE_GAME_SYSTEM_MEMBER(spartan_program_handler, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619BB0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(spartan_program_handler, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00619BA0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(hs)
		DECLARE_GAME_SYSTEM_MEMBER(hs, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00679330, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(hs, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006791C0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(hs, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00679400, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(hs, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006791E0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(recorded_animations)
		DECLARE_GAME_SYSTEM_MEMBER(recorded_animations, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681F10, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(recorded_animations, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681EE0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(recorded_animations, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681F50, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(recorded_animations, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00681EF0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(debug_menu)
		DECLARE_GAME_SYSTEM_MEMBER(debug_menu, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00671670, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(debug_menu, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006713B0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(debug_menu, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00671680, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(debug_menu, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006713C0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(error_report_render)
		DECLARE_GAME_SYSTEM_MEMBER(error_report_render, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADB80, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(error_report_render, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADB50, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(error_report_render, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADB90, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(error_report_render, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00AADB60, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_placement)
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59E10, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59D10, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59EB0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59D20, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, prepare_for_new_zone_set),            //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B5A310, prepare_for_new_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59FC0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B59D30, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(object_placement, prepare_for_non_bsp_zone_set_switch), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B5A470, prepare_for_non_bsp_zone_set_switch),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(havok)
		DECLARE_GAME_SYSTEM_MEMBER(havok, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C5BB0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(havok, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C5550, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(havok, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C5DB0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(havok, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C55A0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(havok, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C5E00, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(havok, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005C55C0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(object_broadphase)
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96EA0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96E40, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96F20, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96E50, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96F30, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(object_broadphase, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00B96E60, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(havok_proxies)
		DECLARE_GAME_SYSTEM_MEMBER(havok_proxies, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067A4E0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(havok_proxies, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067A370, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(havok_proxies, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067A510, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(havok_proxies, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0067A390, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(player_positions)
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BBC0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BB90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BBD0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BBA0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BBE0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(player_positions, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0053BBB0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(ai)
		DECLARE_GAME_SYSTEM_MEMBER(ai, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x01433EA0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(ai, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x014314D0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(ai, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x01433F70, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(ai, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x01431500, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(ai, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x01434010, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(ai, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x01431570, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(portal_activation)
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9E30, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9CA0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9E90, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9CB0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9EA0, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(portal_activation, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x005F9CC0, dispose_from_old_structure_bsp),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(scenario_interpolators)
		DECLARE_GAME_SYSTEM_MEMBER(scenario_interpolators, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00688880, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_interpolators, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00688600, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_interpolators, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006888E0, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(scenario_interpolators, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00688620, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(game_save)
		DECLARE_GAME_SYSTEM_MEMBER(game_save, initialize),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00682500, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(game_save, dispose),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00682490, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(game_save, initialize_for_new_map), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00682560, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(game_save, dispose_from_old_map),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006824A0, dispose_from_old_map),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(watch_window)
		DECLARE_GAME_SYSTEM_MEMBER(watch_window, initialize), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C8F0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(watch_window, dispose),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0068C8E0, dispose),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(bink_playback)
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, initialize),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616AC0, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, dispose),                             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x006169A0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, initialize_for_new_map),              //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616B70, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, dispose_from_old_map),                //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616A00, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, prepare_for_new_zone_set),            //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616C50, prepare_for_new_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, initialize_for_new_structure_bsp),    //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616B90, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, dispose_from_old_structure_bsp),      //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616A20, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, prepare_for_non_bsp_zone_set_switch), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616C80, prepare_for_non_bsp_zone_set_switch),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, initialize_for_new_non_bsp_zone_set), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616B80, initialize_for_new_non_bsp_zone_set),
		DECLARE_GAME_SYSTEM_MEMBER(bink_playback, dispose_from_old_non_bsp_zone_set),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00616A10, dispose_from_old_non_bsp_zone_set),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(editor)
		DECLARE_GAME_SYSTEM_MEMBER(editor, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042E010, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(editor, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042DF90, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(editor, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042E020, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(editor, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042DFA0, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(editor, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042E030, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(editor, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042DFB0, dispose_from_old_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(editor, change_pvs),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x0042DF80, change_pvs),
	},
	{
		DECLARE_GAME_SYSTEM_NAME(render_state_cache)
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, initialize),                       //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DB20, initialize),
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, dispose),                          //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DAF0, dispose),
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, initialize_for_new_map),           //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DB30, initialize_for_new_map),
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, dispose_from_old_map),             //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DB00, dispose_from_old_map),
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, initialize_for_new_structure_bsp), //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DB40, initialize_for_new_structure_bsp),
		DECLARE_GAME_SYSTEM_MEMBER(render_state_cache, dispose_from_old_structure_bsp),   //DECLARE_GAME_SYSTEM_MEMBER_ADDRESS(0x00A3DB10, dispose_from_old_structure_bsp),
	}
};

#undef DECLARE_GAME_SYSTEM_MEMBER2
#undef DECLARE_GAME_SYSTEM_MEMBER
#undef DECLARE_GAME_SYSTEM_MEMBER_ADDRESS
#undef DECLARE_GAME_SYSTEM_NAME

int32 const g_game_system_count = NUMBEROF(g_game_systems);

