#include "game/game.hpp"

#include "config/version.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_globals.hpp"
#include "game/game_options.hpp"
#include "game/game_state.hpp"
#include "game/game_time.hpp"
#include "interface/user_interface_hs.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/files_windows.hpp"

#include <string.h>

HOOK_DECLARE(0x005336F0, game_update);
HOOK_DECLARE(0x006961B0, game_launch_has_initial_script);
HOOK_DECLARE(0x006961C0, game_options_get_launch_settings);

bool __cdecl game_in_startup_phase()
{
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

bool __cdecl game_in_progress()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && game_globals->game_in_progress)
		return !game_globals->initializing && game_globals->map_active;

	return false;
}

// long __cdecl game_create_lock_resources(e_game_create_mode)
// void __cdecl game_create_unlock_resources(e_game_create_mode, long&)
// void __cdecl game_start(enum e_game_create_mode)

bool __cdecl game_options_valid()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && (game_globals->initializing || game_globals->map_active);
}

bool __cdecl game_options_verify(game_options const* options, char* error_string, long error_string_length)
{
	return INVOKE(0x005326F0, game_options_verify, options, error_string, error_string_length);
}

void __cdecl assert_game_options_verify(game_options const* options)
{
	char error_string[512]{};
	ASSERT(game_options_verify(options, error_string, sizeof(error_string))); // ("game_options_verify failed: %s", error_string)
	//INVOKE(0x00530440, assert_game_options_verify, options);
}

game_options* __cdecl game_options_get()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

void __cdecl game_options_print_game_id()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	c_console::write_line("%I64d", game_options_get()->game_instance);
}

// void __cdecl game_options_setup_for_saved_film(e_game_playback_type playback_type)

void __cdecl game_options_clear_game_playback()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_playback = _game_playback_none;
}

// void __cdecl game_options_game_engine_fixup()

e_campaign_difficulty_level __cdecl game_difficulty_level_get()
{
	return game_options_get()->campaign_difficulty;
}

e_campaign_difficulty_level __cdecl game_difficulty_level_get_ignore_easy()
{
	if (game_difficulty_level_get() == _campaign_difficulty_level_easy)
		return _campaign_difficulty_level_normal;

	return game_difficulty_level_get();
}

e_game_mode __cdecl game_mode_get()
{
	return game_options_get()->game_mode;
}

bool __cdecl game_is_ui_shell()
{
	return game_mode_get() == _game_mode_mainmenu;
}

bool __cdecl game_is_multiplayer()
{
	return game_mode_get() == _game_mode_multiplayer;
}

bool __cdecl game_is_campaign()
{
	return game_mode_get() == _game_mode_campaign;
}

static bool g_debug_survival_mode = false;

bool __cdecl game_is_survival()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && (game_globals->initializing || game_globals->map_active))
		return game_globals->options.game_mode == _game_mode_campaign && game_globals->options.survival_enabled;

	return g_debug_survival_mode;
}

bool __cdecl game_is_campaign_or_survival()
{
	return game_is_campaign() || game_is_survival();
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
	if (game_in_progress() &&
		game_is_campaign() &&
		campaign_difficulty >= _campaign_difficulty_level_easy &&
		campaign_difficulty < k_campaign_difficulty_levels_count)
	{
		game_options_get()->campaign_difficulty = campaign_difficulty;
	}
}

// bool __cdecl game_is_cooperative()
// long __cdecl game_coop_player_count()

bool __cdecl game_is_playtest()
{
	return game_options_get()->playtest_mode;
}

bool __cdecl game_had_an_update_tick_this_frame()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->update_tick_this_frame;
}

e_game_simulation_type __cdecl game_simulation_get()
{
	return game_options_get()->game_simulation;
}

bool __cdecl game_is_playback()
{
	return game_playback_get();
}

e_game_playback_type __cdecl game_playback_get()
{
	return game_options_get()->game_playback;
}

// void __cdecl game_playback_set(e_game_playback_type playback_type)

void __cdecl game_simulation_set(e_game_simulation_type game_simulation)
{
	char const* k_game_simulation_names[k_game_simulation_count]
	{
		"none",
		"local",
		"sync-client",
		"sync-server",
		"dist-client",
		"dist-server"
	};

	ASSERT(game_simulation > _game_simulation_none && game_simulation < k_game_simulation_count);

	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_simulation = game_simulation;
	c_console::write_line("game_simulation: %s", k_game_simulation_names[game_simulation]);
}

bool __cdecl game_is_synchronous_networking()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_synchronous_server)
		return true;

	return false;
}

bool __cdecl game_is_networked()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

// bool __cdecl game_is_in_progress_on_live()

bool __cdecl game_is_server()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
		return true;

	return false;
}

bool __cdecl game_is_authoritative()
{
	return !game_is_predicted();
}

bool __cdecl game_is_predicted()
{
	return game_options_get()->game_simulation == _game_simulation_distributed_client;
}

bool __cdecl game_is_distributed()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_distributed_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

long __cdecl game_tick_rate_get()
{
	return game_options_get()->game_tick_rate;
}

bool __cdecl game_skull_is_active_primary(e_campaign_skulls_primary primary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_primary_skulls == primary_skull;

	return false;
}

bool __cdecl game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_secondary_skulls == secondary_skull;

	return false;
}

void __cdecl game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_primary_skulls.set(primary_skull, enable);
}

void __cdecl game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_secondary_skulls.set(secondary_skull, enable);
}

bool __cdecl game_coop_allow_respawn()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->options.game_mode == _game_mode_campaign && !game_skull_is_active_primary(_campaign_skull_iron);
}

// bool __cdecl game_survival_allow_respawn(long)

e_language __cdecl game_get_master_language()
{
	return game_options_get()->language;
}

// bool __cdecl game_is_language_neutral()

void __cdecl game_options_setup_default_players(long player_count, game_options* options)
{
	DECLFUNC(0x00532440, void, __cdecl, long, game_options*)(player_count, options);
}

// bool __cdecl game_is_finished_waiting_for_level_advance()

bool __cdecl game_is_available()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_pvs_local()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs_local;
}

s_game_cluster_bit_vectors* __cdecl game_get_cluster_activation()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_activation;
}

// bool __cdecl game_test_cluster_activation(s_cluster_reference* cluster_reference)

void __cdecl game_pvs_enable_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = true;
}

void __cdecl game_pvs_clear_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = false;
}

void __cdecl game_pvs_scripted_set_object(datum_index object_index)
{
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

// void __cdecl game_pvs_scripted_set_camera_point(short camera_point_index)

void __cdecl game_pvs_scripted_clear()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted = 0;
}

#define BOT_CLIENT(true_false) if (game_is_bot_client() == true_false)

void __cdecl game_tick()
{
	INVOKE(0x00533120, game_tick);

	//simulation_update update = { .flags = 0 };
	//s_simulation_update_metadata metadata = { .flags = 0 };
	//
	//game_globals_get()->update_tick_this_frame = true;
	//main_status("game_tick", "time %d", game_time_get());
	//
	//real_math_reset_precision();
	//simulation_build_update(true, &update, &metadata);
	//simulation_record_update(&update);
	//game_state_preserve();
	//c_rasterizer::notify_game_tick_begin();
	//c_water_renderer::game_update();
	//damage_acceleration_queue_begin();
	//simulation_apply_before_game(&update);
	//levels_update();
	//
	//if (TEST_BIT(update.flags, 0))
	//{
	//	chud_game_tick();
	//	players_update_before_game(&update);
	//	sound_update();
	//	game_tick_pulse_random_seed_deterministic(&update);
	//	ai_update();
	//	recorded_animations_update();
	//	game_sound_deterministic_update_timers();
	//	game_engine_update();
	//	game_results_update();
	//	editor_update();
	//	cinematics_game_tick();
	//	c_hue_saturation_control::copy_from_gamestate();
	//	hs_update();
	//	c_hue_saturation_control::copy_to_gamestate();
	//
	//	BOT_CLIENT(FALSE)
	//	{
	//		game_update_pvs();
	//	}
	//
	//	object_scheduler_update();
	//	object_activation_regions_update();
	//	objects_update();
	//	damage_acceleration_queue_end();
	//	havok_proxies_update();
	//	havok_update();
	//	havok_proxies_move();
	//	objects_move();
	//	objects_post_update();
	//
	//	BOT_CLIENT(FALSE)
	//	{
	//		impacts_update();
	//	}
	//
	//	breakable_surfaces_update();
	//	effects_update();
	//	lights_update();
	//	game_engine_update_after_game();
	//	simulation_apply_after_game(&update);
	//	players_update_after_game(&update);
	//	campaign_metagame_update();
	//	game_allegiance_update();
	//	game_loss_update();
	//	game_decrement_end_match_wait_time();
	//	game_finished_update();
	//	game_save_update();
	//	cinematic_update();
	//	s_depth_of_field::update();
	//	game_grief_update();
	//	first_person_weapons_update();
	//	player_effect_update();
	//	overhead_map_update();
	//	observer_game_tick();
	//	director_game_tick();
	//}
	//else
	//{
	//	damage_acceleration_queue_end();
	//}
	//simulation_update_aftermath(&update, &metadata);
	//
	//if (TEST_BIT(update.flags, 0))
	//	game_time_advance();
	//
	//simulation_destroy_update(&update);
	//main_status("game_tick", 0);
}

void __cdecl game_update(long tick_count, real* game_seconds_elapsed)
{
	render_thread_enabled();

	// reach
	if (game_options_get()->game_simulation == _game_simulation_synchronous_server && tick_count == 1)
		game_globals_get()->update_tick_this_frame = true;
	else
		game_globals_get()->update_tick_this_frame = false;

	long actual_ticks = 0;
	if (tick_count > 0)
	{
		while (!main_events_pending() && !simulation_aborted())
		{
			game_tick();

			// reach
			game_globals_get()->update_tick_this_frame = false;

			if (++actual_ticks >= tick_count)
				return;
		}
	}

	if (actual_ticks < tick_count)
		game_time_discard(tick_count, actual_ticks, game_seconds_elapsed);
}

// void __cdecl game_update_pvs()
// void __cdecl game_won()
// bool __cdecl game_is_won()

void __cdecl game_lost(bool game_revert)
{
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

bool __cdecl game_is_lost()
{
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

void __cdecl game_finish()
{
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

bool __cdecl game_is_finished()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished;
}

bool __cdecl game_is_finished_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	ASSERT(game_globals && game_globals->map_active);

	return game_globals->game_finished && !game_globals->game_finished_wait_time;
}

void __cdecl game_options_new(game_options* options)
{
	INVOKE(0x005323A0, game_options_new, options);
}

struct s_game_options_launch_settings
{
	// 106708
	long build_number;

	char scenario_path[256];

	e_campaign_difficulty_level campaign_difficulty;

	e_game_mode game_mode;
	e_game_engine_type game_engine_index;

	long player_count;

	short insertion_point;
	short zone_set_index;
	dword_flags launch_file_flags; // bit 1, delete after read

	//char insertion_point_name[128]; // name speculation, never actually saw this used
	//char zone_set_name[128];
};
//static_assert(sizeof(s_game_options_launch_settings) == 0x210);
static_assert(sizeof(s_game_options_launch_settings) == 0x11C);

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
		memcpy(out_launch_settings, &launch_settings, sizeof(s_game_options_launch_settings));
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
				memcpy(out_launch_settings, &launch_settings, sizeof(s_game_options_launch_settings));
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

	build_default_game_variant(&options->game_variant, options->game_mode != _game_mode_multiplayer ? _game_engine_base_variant : launch_settings.game_engine_index);

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
