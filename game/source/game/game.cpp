#include "game/game.hpp"

#include "config/version.hpp"
#include "cseries/cseries.hpp"
#include "editor/editor_stubs.hpp"
#include "effects/effects.hpp"
#include "game/game_achievements.hpp"
#include "game/game_grief.hpp"
#include "hs/hs.hpp"
#include "interface/overhead_map.hpp"
#include "interface/user_interface_hs.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/widgets/widgets.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer_hue_saturation.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/render.hpp"
#include "render/render_debug.hpp"
#include "render/render_visibility.hpp"
#include "scenario/scenario_interpolators.hpp"
#include "simulation/simulation.hpp"
#include "sound/game_sound.hpp"
#include "sound/sound_manager.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "tag_files/files_windows.hpp"
#include "test/test_functions.hpp"

HOOK_DECLARE(0x00530F80, game_finish);
HOOK_DECLARE(0x00533120, game_tick);
HOOK_DECLARE(0x006961B0, game_launch_has_initial_script);

bool g_debug_survival_mode = false;

char const* const k_game_simulation_names[k_game_simulation_count]
{
	"none",
	"local",
	"sync-client",
	"sync-server",
	"dist-client",
	"dist-server"
};

c_static_array<c_static_array<long, 256>, 16> g_cluster_activation_reason;

real_argb_color const* const k_activation_colors[6]
{
	nullptr,
	global_real_argb_orange,
	global_real_argb_green,
	global_real_argb_pink,
	global_real_argb_purple,
	global_real_argb_yellow
};
long k_activation_color_override_index = 1;

game_globals_storage* game_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	return game_globals;
}

bool __cdecl game_is_multithreaded()
{
	return INVOKE(0x0042E2C0, game_is_multithreaded);
}

//.text:005303E0 ; void __cdecl __tls_set_g_game_globals_allocator(void*)

void __cdecl assert_game_options_verify(game_options const* options)
{
	//INVOKE(0x00530440, assert_game_options_verify, options);

	char error_string[512]{};
	ASSERT(game_options_verify(options, error_string, sizeof(error_string))); // ("game_options_verify failed: %s", error_string)
}

//.text:00530580 ; game_globals_get_primary_skulls
//.text:005305D0 ; game_globals_get_secondary_skulls
//.text:00530620 ; void __cdecl game_react_to_level_completion()
//.text:005307B0 ; 
//.text:005307F0 ; void __cdecl game_clusters_and
//.text:00530840 ; game_clusters_fill
//.text:00530860 ; game_clusters_or
//.text:005308B0 ; game_compute_pvs
//.text:00530A20 ; bool __cdecl game_coop_allow_respawn()
//.text:00530A40 ; long __cdecl game_coop_player_count()

void __cdecl game_create_ai(e_game_create_mode mode)
{
	INVOKE(0x00530A70, game_create_ai, mode);

	//long lock = game_create_lock_resources(mode);
	////random_seed_allow_use();
	//
	//ai_place(game_mode_get(), true);
	//
	////random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

long __cdecl game_create_lock_resources(e_game_create_mode mode)
{
	return INVOKE(0x00530AC0, game_create_lock_resources, mode);

	//return mode == _game_create_mode_lock ? tag_resources_lock_game() : 0;
}

void __cdecl game_create_missing_objects(e_game_create_mode mode)
{
	INVOKE(0x00530AE0, game_create_missing_objects, mode);

	//game_globals_storage* game_globals = game_globals_get();
	//assert_game_options_verify(&game_globals->options);
	//ASSERT(game_globals->map_active);
	//
	//long lock = game_create_lock_resources(mode);
	////random_seed_allow_use();
	//
	//object_placement_create_global_objects(game_globals->options.game_mode, true);
	//
	////random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

void __cdecl game_create_objects(e_game_create_mode mode)
{
	INVOKE(0x00530B30, game_create_objects, mode);

	//game_globals_storage* game_globals = game_globals_get();
	//assert_game_options_verify(&game_globals->options);
	//ASSERT(game_globals->map_active);
	//
	//long lock = game_create_lock_resources(mode);
	////random_seed_allow_use();
	//
	//c_scenerio_sky_objects_helper::create_sky_objects(global_scenario_get());
	//object_placement_create_global_objects(game_globals->options.game_mode, false);
	//lights_place(game_globals->options.game_mode);
	//
	////random_seed_disallow_use();
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
	//	players_set_machines(options->machine_options.valid_machine_mask, options->machine_options.machines);
	//	players_set_local_machine(options->machine_options.local_machine_exists ? &options->machine_options.local_machine : NULL);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (options->players[i].player_valid)
	//			player_new(i, &options->players[i], false);
	//	}
	//
	//	players_finish_creation();
	//	simulation_notify_players_created();
	//}
}

void __cdecl game_create_unlock_resources(e_game_create_mode mode, long& lock)
{
	INVOKE(0x00530C20, game_create_unlock_resources, mode, lock);

	//if (mode == _game_create_mode_lock)
	//	tag_resources_unlock_game(lock);
}

//.text:00530C50 ; bool __cdecl game_determinism_version_compatible(long determinism_version)

e_campaign_difficulty_level __cdecl game_difficulty_level_get()
{
	//return INVOKE(0x00530C70, game_difficulty_level_get);

	return game_options_get()->campaign_difficulty;
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
	INVOKE(0x00530CD0, game_dispose);
}

void __cdecl game_dispose_from_old_map()
{
	INVOKE(0x00530D10, game_dispose_from_old_map);
}

//.text:00530E10 ; void __cdecl game_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* non_bsp_zone_set)
//.text:00530E70 ; void __cdecl game_dispose_from_old_structure_bsp(dword)

void __cdecl game_finish()
{
	//INVOKE(0x00530F80, game_finish);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		// seconds_to_wait:
		// - halo 3: 7.0f
		// - halo reach: game_is_campaign_or_survival() ? 1.0f : 7.0f
		// - halo online: 40.0f
		real seconds_to_wait = 7.0f;

		game_globals->game_finished = true;
		game_globals->game_finished_wait_time = game_seconds_to_ticks_round(seconds_to_wait);

		// halo online
		//if (!game_is_playback())
		//	game_sound_disable_at_game_finish();
	}
}

//.text:00530FF0 ; 

void __cdecl game_finished_update()
{
	//INVOKE(0x00531040, game_finished_update);

	game_globals_storage* game_globals = game_globals_get();

	if (game_globals->game_finished && game_globals->game_finished_wait_time > 0)
		game_globals->game_finished_wait_time--;
}

void __cdecl game_frame(real game_seconds_elapsed)
{
	//INVOKE(0x00531070, game_frame, game_seconds_elapsed);

	game_globals_storage* game_globals = game_globals_get();
	
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
	game_globals->update_tick_this_frame = false;
}

dword __cdecl game_get_active_cinematic_zone_mask()
{
	//return INVOKE(0x00531110, game_get_active_cinematic_zone_mask);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals->active_cinematic_zone_mask;
}

dword __cdecl game_get_active_designer_zone_mask()
{
	//return INVOKE(0x00531130, game_get_active_designer_zone_mask);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals->active_designer_zone_mask;
}

dword __cdecl game_get_active_structure_bsp_mask()
{
	//return INVOKE(0x00531150, game_get_active_structure_bsp_mask);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals->active_structure_bsp_mask;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_activation()
{
	//return INVOKE(0x00531170, game_get_cluster_activation);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_activation;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs()
{
	//return INVOKE(0x00531190, game_get_cluster_pvs);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs_local()
{
	//return INVOKE(0x005311B0, game_get_cluster_pvs_local);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs_local;
}

//.text:005311D0 ; void __cdecl game_get_determinism_versions(long* determinism_version, long* determinism_compatible_version)
//.text:00531200 ; 

e_language __cdecl game_get_master_language()
{
	//return INVOKE(0x00531250, game_get_master_language);

	return game_options_get()->language;
}

//.text:00531270 ; long __cdecl game_get_recently_spawned_grace_period_seconds()
//.text:00531290 ; void __cdecl game_globals_dispose_from_old_map()
//.text:005312C0 ; void __cdecl game_globals_initialize_for_new_map(game_options const* options)

bool __cdecl game_had_an_update_tick_this_frame()
{
	//return INVOKE(0x00531410, game_had_an_update_tick_this_frame);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals && game_globals->update_tick_this_frame;
}

bool __cdecl game_has_nonlocal_players()
{
	return INVOKE(0x00531430, game_has_nonlocal_players);
}

bool __cdecl game_in_progress()
{
	//return INVOKE(0x005314B0, game_in_progress);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && game_globals->game_in_progress)
		return !game_globals->initializing && game_globals->map_active;

	return false;
}

bool __cdecl game_in_startup_phase()
{
	//return INVOKE(0x005314F0, game_in_startup_phase);

	game_globals_storage* game_globals = game_globals_get();
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
	INVOKE(0x00531530, game_initialize);
}

void __cdecl game_initialize_for_new_map(game_options const* options)
{
	INVOKE(0x00531600, game_initialize_for_new_map, options);
}

//.text:00531790 ; void __cdecl game_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* non_bsp_zone_set)
//.text:005317F0 ; void __cdecl game_initialize_for_new_structure_bsp(dword)
//.text:00531840 ; short __cdecl game_insertion_point_get()
//.text:00531870 ; void __cdecl game_insertion_point_lock(short)
//.text:00531900 ; void __cdecl game_insertion_point_unlock(short)

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

	game_globals_storage* game_globals = game_globals_get();
	return game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask;
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

	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_distributed_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

bool __cdecl game_is_finished()
{
	//return INVOKE(0x00531B20, game_is_finished);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished;
}

bool __cdecl game_is_finished_immediate()
{
	//return INVOKE(0x00531B40, game_is_finished_immediate);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished && !game_globals->game_finished_wait_time;
}

//.text:00531B70 ; bool __cdecl game_is_in_progress_on_live()

bool __cdecl game_is_lost()
{
	return INVOKE(0x00531BE0, game_is_lost);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_lost;
}

// custom like `game_is_finished_immediate`
bool __cdecl game_is_lost_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_lost && !game_globals->game_lost_wait_time;
}

bool __cdecl game_is_multiplayer()
{
	//return INVOKE(0x00531C00, game_is_multiplayer);

	return game_mode_get() == _game_mode_multiplayer;
}

bool __cdecl game_is_networked()
{
	//return INVOKE(0x00531C20, game_is_networked);

	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
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

	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
		return true;

	return false;
}

//.text:00531DD0 ; bool __cdecl game_is_splitscreen_deterministic()

bool __cdecl game_is_survival()
{
	//return INVOKE(0x00531E20, game_is_survival);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && (game_globals->initializing || game_globals->map_active))
		return game_globals->options.game_mode == _game_mode_campaign && game_globals->options.survival_enabled;

	return g_debug_survival_mode;
}

bool __cdecl game_is_synchronous_networking()
{
	//return INVOKE(0x00531E60, game_is_synchronous_networking);

	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_synchronous_server)
		return true;

	return false;
}

bool __cdecl game_is_ui_shell()
{
	//return INVOKE(0x00531E90, game_is_ui_shell);

	return game_mode_get() == _game_mode_mainmenu;
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

void __cdecl game_lost(bool game_revert)
{
	//INVOKE(0x005321E0, game_lost, game_revert);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	game_globals->game_revert = game_revert;
	if (game_revert)
	{
		if (!game_globals->game_lost)
		{
			// seconds_to_wait:
			// - halo 3: 5.0f
			// - halo reach: 6.0f
			// - halo online: 6.0f
			real seconds_to_wait = 5.0f;

			game_globals->game_lost = true;
			game_globals->game_lost_wait_time = game_seconds_to_ticks_round(seconds_to_wait);
			game_state_prepare_for_revert();
		}
	}
	else
	{
		if (game_globals->game_lost && !game_globals->game_revert)
			game_globals->game_lost = false;
	}
}

e_game_mode __cdecl game_mode_get()
{
	//return INVOKE(0x00532260, game_mode_get);

	return game_options_get()->game_mode;
}

void __cdecl game_options_clear_game_playback()
{
	//INVOKE(0x00532280, game_options_clear_game_playback);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_playback = _game_playback_none;
}

void __cdecl game_options_game_engine_fixup()
{
	INVOKE(0x005322A0, game_options_game_engine_fixup);

	//game_globals_storage* game_globals = game_globals_get();
	//ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));
	//
	//game_globals->options.game_variant.recreate_variant_vtable_for_game_engine_index(game_globals->options.game_variant.get_game_engine_index());
}

game_options* __cdecl game_options_get()
{
	//return INVOKE(0x005322D0, game_options_get);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

void __cdecl game_options_print_game_id()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	console_printf("%I64d", game_options_get()->game_instance);
}

//.text:005322F0 ; bool __cdecl game_options_match(game_options const*, game_options const*)

void __cdecl game_options_new(game_options* options)
{
	INVOKE(0x005323A0, game_options_new, options);
}

void __cdecl game_options_setup_default_players(long player_count, game_options* options)
{
	DECLFUNC(0x00532440, void, __cdecl, long, game_options*)(player_count, options);
}

//.text:00532610 ; void __cdecl game_options_setup_for_playback(e_game_playback_type)
//void __cdecl game_options_setup_for_saved_film(e_game_playback_type)

bool __cdecl game_options_valid()
{
	//return INVOKE(0x00532650, game_options_valid);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals && (game_globals->initializing || game_globals->map_active);
}

void __cdecl game_options_validate(game_options* options)
{
	INVOKE(0x00532680, game_options_validate, options);
}

//.text:005326B0 ; void __cdecl game_options_validate_for_saved_game(long)

bool __cdecl game_options_verify(game_options const* options, char* error_string, long error_string_length)
{
	return INVOKE(0x005326F0, game_options_verify, options, error_string, error_string_length);
}

e_game_playback_type __cdecl game_playback_get()
{
	//return INVOKE(0x00532A80, game_playback_get);

	return game_options_get()->game_playback;
}

//.text:00532AA0 ; void __cdecl game_playback_set(e_game_playback_type playback_type)
//.text:00532AD0 ; void __cdecl game_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const*, s_game_non_bsp_zone_set const*, c_scenario_resource_registry*)
//.text:00532B10 ; void __cdecl game_prepare_to_switch_structure_bsp(dword, dword)
//.text:00532B50 ; void __cdecl skull_primary_enable
//.text:00532B90 ; game_get_game_progression

void __cdecl game_pvs_clear_scripted_camera_pvs()
{
	//INVOKE(0x00532BB0, game_pvs_clear_scripted_camera_pvs);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = false;
}

void __cdecl game_pvs_enable_scripted_camera_pvs()
{
	//INVOKE(0x00532BD0, game_pvs_enable_scripted_camera_pvs);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = true;
}

void __cdecl game_pvs_scripted_clear()
{
	//INVOKE(0x00532BF0, game_pvs_scripted_clear);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted = 0;
}

//.text:00532C10 ; struct s_cluster_reference __cdecl game_pvs_scripted_get_cluster_reference()
//.text:00532CB0 ; void __cdecl game_pvs_scripted_set_camera_point(short camera_point_index)

void __cdecl game_pvs_scripted_set_object(long object_index)
{
	INVOKE(0x00532D40, game_pvs_scripted_set_object, object_index);

	game_globals_storage* game_globals = game_globals_get();
	if (!game_globals)
		return;

	if (object_index != -1)
	{
		game_globals->scripted = 1;
		game_globals->scripted_object_index = object_index;
	}
	game_globals->scripted = 0;
}

//.text:00532D90 ; void __cdecl update_controller_game_progression()

//.text: ; skull_secondary_enable
void __cdecl game_skull_enable_secondary(e_secondary_skulls secondary_skull, bool enable)
{
	//INVOKE(0x00532EE0, game_skull_enable_secondary, secondary_skull, enable);

	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_secondary_skulls.set(secondary_skull, enable);
}

//.text:00532F20 ; game_set_active_primary_skulls
//.text:00532F50 ; game_set_active_secondary_skulls

bool __cdecl game_skull_is_active_primary(e_primary_skulls primary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_primary_skulls == primary_skull;

	return false;
}

bool __cdecl game_skull_is_active_secondary(e_secondary_skulls secondary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_secondary_skulls == secondary_skull;

	return false;
}

void __cdecl game_skull_enable_primary(e_primary_skulls primary_skull, bool enable)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_primary_skulls.set(primary_skull, enable);
}

void __cdecl game_set_active_skulls(dword* active_primary_skulls, dword* active_secondary_skulls)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
	{
		if (active_primary_skulls)
			*active_primary_skulls = game_globals->active_primary_skulls;

		if (active_secondary_skulls)
			*active_secondary_skulls = game_globals->active_secondary_skulls;
	}
}

void __cdecl game_set_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	//INVOKE(0x00532F80, game_set_difficulty, campaign_difficulty);

	if (game_in_progress() &&
		game_is_campaign() &&
		campaign_difficulty >= _campaign_difficulty_level_easy &&
		campaign_difficulty < k_number_of_campaign_difficulty_levels)
	{
		game_options_get()->campaign_difficulty = campaign_difficulty;
	}
}

e_game_simulation_type __cdecl game_simulation_get()
{
	//return INVOKE(0x00532FD0, game_simulation_get);

	return game_options_get()->game_simulation;
}

void __cdecl game_simulation_set(e_game_simulation_type game_simulation)
{
	//INVOKE(0x00532FF0, game_simulation_set, game_simulation);

	ASSERT(game_simulation > _game_simulation_none && game_simulation < k_game_simulation_count);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_simulation = game_simulation;
	main_status("game_simulation: %s", k_game_simulation_names[game_simulation]);
}

void __cdecl game_start(e_game_create_mode mode)
{
	INVOKE(0x00533030, game_start, mode);

	//game_globals_storage* game_globals = game_globals_get();
	//ASSERT(game_globals);
	//ASSERT(!game_globals->initializing);
	//ASSERT(game_globals->map_active);
	//ASSERT(game_globals->active_structure_bsp_mask != 0);
	//ASSERT(!game_globals->game_in_progress);
	//
	//game_globals->game_in_progress = true;
	//simulation_start();
	//
	//long lock = game_create_lock_resources(mode);
	////random_seed_allow_use();
	//
	//game_engine_game_starting();
	//game_launch_initial_script();
	//
	////random_seed_disallow_use();
	//game_create_unlock_resources(mode, lock);
}

bool __cdecl game_survival_allow_respawn()
{
	//return INVOKE(0x005330B0, game_survival_allow_respawn);

	game_globals_storage* game_globals = game_globals_get();
	return game_globals && game_globals->options.game_mode == _game_mode_campaign && !game_skull_is_active_primary(_campaign_skull_iron);
}

//.text:005330D0 ; bool __cdecl game_test_cluster_activation(s_cluster_reference const*)

void __cdecl game_tick()
{
	//INVOKE(0x00533120, game_tick);

	PROFILER(game_tick)
	{
		struct simulation_update update = { .high_level_flags = 0 };
		s_simulation_update_metadata metadata = { .flags = 0 };

		game_globals_get()->update_tick_this_frame = true;
		main_status(__FUNCTION__, "time %d", game_time_get());

		PROFILER(build_simulation_update)
		{
			real_math_reset_precision();

			//saved_film_history_before_update_built();
			simulation_build_update(true, &update, &metadata);
			//saved_film_history_after_update_built(&update, metadata);
			simulation_record_update(&update);

			game_state_preserve();
		}

		c_rasterizer::notify_game_tick_begin();
		c_water_renderer::game_update();
		damage_acceleration_queue_begin();
		simulation_apply_before_game(&update);
		levels_update();

		if (update.high_level_flags.test(_simulation_update_high_level_simulation_in_progress_bit))
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

		if (update.high_level_flags.test(_simulation_update_high_level_simulation_in_progress_bit))
			game_time_advance();

		simulation_destroy_update(&update);
		main_status(__FUNCTION__, NULL);
	}
}

void __cdecl game_tick_pulse_random_seed_deterministic(struct simulation_update const* update)
{
	INVOKE(0x005333A0, game_tick_pulse_random_seed_deterministic, update);
}

long __cdecl game_tick_rate_get()
{
	return INVOKE(0x00533470, game_tick_rate_get);

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

void __cdecl game_update(long tick_count, real* game_seconds_elapsed)
{
	//INVOKE(0x005336F0, game_update, tick_count, game_seconds_elapsed);

	PROFILER(game_update)
	{
		render_thread_get_mode();

		game_globals_storage* game_globals = game_globals_get();
		game_globals->update_tick_this_frame = game_simulation_get() == _game_simulation_synchronous_server && tick_count == 1;

		long actual_ticks;
		for (actual_ticks = 0; actual_ticks < tick_count && !main_events_pending() && !simulation_aborted(); actual_ticks++)
		{
			game_tick();
			game_globals->update_tick_this_frame = false;
		}

		if (actual_ticks < tick_count)
			game_time_discard(tick_count, actual_ticks, game_seconds_elapsed);
	}
}

//.text:005339C0 ; 

void __cdecl game_update_pvs()
{
	INVOKE(0x00533B80, game_update_pvs);
}

void __cdecl game_won()
{
	INVOKE(0x00533E40, game_won);
}

//.text:00533EF0 ; 
//.text:00533FC0 ; char const* __cdecl get_game_difficulty_name(short)

void __cdecl game_finish_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		game_finish();
		game_globals->game_finished_wait_time = 0;
	}
}

bool __cdecl game_options_read_launch_settings_from_string(char const* buffer, s_game_options_launch_settings* out_launch_settings)
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

bool __cdecl game_launch_has_initial_script(char const* script_name)
{
	// fix for hanger background not being loaded
	if (game_is_ui_shell())
		user_interface_start_hs_script_by_name("humanhangar");

	return false;
}

bool __cdecl game_options_get_launch_settings(game_options* options, bool change_in_progress)
{
	ASSERT(options);

	//bool result = false;
	//HOOK_INVOKE(result =, game_options_get_launch_settings, options, change_in_progress);
	//return result;

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
	options->record_saved_film = true;//saved_film_manager_should_record_film(options);
	options->campaign_difficulty = launch_settings.campaign_difficulty;

	build_default_game_variant(&options->game_variant, options->game_mode != _game_mode_multiplayer ? _game_engine_type_none : launch_settings.game_engine_index);

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

void __cdecl game_pvs_debug_render()
{
	if (game_in_progress() && game_get_active_structure_bsp_mask())
	{
		for (long structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			structure_bsp* bsp = global_structure_bsp_get(structure_bsp_index);
			for (long cluster_index = 0; cluster_index < bsp->clusters.count(); cluster_index++)
			{
				real_argb_color const* activation_color = nullptr;
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

					point0.x = bounds->x.lower;
					point0.y = bounds->y.lower;
					point0.z = bounds->z.lower;
					point1.x = bounds->x.upper;
					point1.y = bounds->y.upper;
					point1.z = bounds->z.upper;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x.upper;
					point0.y = bounds->y.lower;
					point0.z = bounds->z.lower;
					point1.x = bounds->x.lower;
					point1.y = bounds->y.upper;
					point1.z = bounds->z.upper;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x.lower;
					point0.y = bounds->y.upper;
					point0.z = bounds->z.lower;
					point1.x = bounds->x.upper;
					point1.y = bounds->y.lower;
					point1.z = bounds->z.upper;
					render_debug_line(true, &point0, &point1, activation_color);

					point0.x = bounds->x.upper;
					point0.y = bounds->y.upper;
					point0.z = bounds->z.lower;
					point1.x = bounds->x.lower;
					point1.y = bounds->y.lower;
					point1.z = bounds->z.upper;
					render_debug_line(true, &point0, &point1, activation_color);
				}
			}
		}
	}
}

