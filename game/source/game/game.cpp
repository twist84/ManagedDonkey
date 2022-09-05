#include "game/game.hpp"

#include "game/game_globals.hpp"
#include "game/game_options.hpp"
#include "cseries/cseries.hpp"
#include "game/game_state.hpp"
#include "game/game_time.hpp"

#include <assert.h>
#include <stdio.h>

bool game_in_startup_phase()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
	{
		if (game_globals->game_in_progress)
		{
			assert(!game_globals->initializing);
			assert(game_globals->map_active);

			return false;
		}

		return game_globals->initializing || game_globals->map_active;
	}
	return false;
}

bool game_in_progress()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && game_globals->game_in_progress)
		return !game_globals->initializing && game_globals->map_active;

	return false;
}

// long game_create_lock_resources(e_game_create_mode)
// void game_create_unlock_resources(e_game_create_mode, long&)
// void game_start(enum e_game_create_mode)

bool game_options_valid()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && (game_globals->initializing || game_globals->map_active);
}

bool __cdecl game_options_verify(game_options const* options, char* error_string, long error_string_length)
{
	return DECLTHUNK(0x005326F0, game_options_verify, options, error_string, error_string_length);
}

void __cdecl assert_game_options_verify(game_options const* options)
{
	char error_string[512]{};
	assert(game_options_verify(options, error_string, sizeof(error_string))); // ("game_options_verify failed: %s", error_string)
	//DECLTHUNK(0x00530440, assert_game_options_verify, options);
}

game_options* game_options_get()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

void game_options_print_game_id()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	printf("%I64d\n", game_options_get()->game_instance);
}

// void game_options_setup_for_saved_film(e_game_playback_type playback_type)

void game_options_clear_game_playback()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_playback = _game_playback_none;
}

// void game_options_game_engine_fixup()

e_campaign_difficulty_level game_difficulty_level_get()
{
	return game_options_get()->campaign_difficulty;
}

e_campaign_difficulty_level game_difficulty_level_get_ignore_easy()
{
	if (game_difficulty_level_get() == _campaign_difficulty_level_easy)
		return _campaign_difficulty_level_normal;

	return game_difficulty_level_get();
}

e_game_mode game_mode_get()
{
	return game_options_get()->game_mode;
}

bool game_is_ui_shell()
{
	return game_mode_get() == _game_mode_mainmenu;
}

bool game_is_multiplayer()
{
	return game_mode_get() == _game_mode_multiplayer;
}

bool game_is_campaign()
{
	return game_mode_get() == _game_mode_campaign;
}

static bool g_debug_survival_mode = false;

bool game_is_survival()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals && (game_globals->initializing || game_globals->map_active))
		return game_globals->options.game_mode == _game_mode_campaign && game_globals->options.survival_enabled;

	return g_debug_survival_mode;
}

bool game_is_campaign_or_survival()
{
	return game_is_campaign() || game_is_survival();
}

void game_set_active_skulls(dword* active_primary_skulls, dword* active_secondary_skulls)
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

void game_set_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	if (game_in_progress() &&
		game_is_campaign() &&
		campaign_difficulty >= _campaign_difficulty_level_easy &&
		campaign_difficulty < k_campaign_difficulty_levels_count)
	{
		game_options_get()->campaign_difficulty = campaign_difficulty;
	}
}

// bool game_is_cooperative()
// long game_coop_player_count()

bool game_is_playtest()
{
	return game_options_get()->playtest_mode;
}

bool game_had_an_update_tick_this_frame()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->update_tick_this_frame;
}

e_game_simulation_type game_simulation_get()
{
	return game_options_get()->game_simulation;
}

bool game_is_playback()
{
	return game_playback_get();
}

e_game_playback_type game_playback_get()
{
	return game_options_get()->game_playback;
}

// void game_playback_set(e_game_playback_type playback_type)

void game_simulation_set(e_game_simulation_type game_simulation)
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

	assert(game_simulation > _game_simulation_none && game_simulation < k_game_simulation_count);

	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && (game_globals->initializing || game_globals->map_active));

	game_globals->options.game_simulation = game_simulation;
	printf("game_simulation: %s\n", k_game_simulation_names[game_simulation]);
}

bool game_is_synchronous_networking()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_synchronous_server)
		return true;

	return false;
}

bool game_is_networked()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_synchronous_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

// bool game_is_in_progress_on_live()

bool game_is_server()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation == _game_simulation_synchronous_server ||
		game_simulation == _game_simulation_distributed_server)
		return true;

	return false;
}

bool game_is_authoritative()
{
	return !game_is_predicted();
}

bool game_is_predicted()
{
	return game_options_get()->game_simulation == _game_simulation_distributed_client;
}

bool game_is_distributed()
{
	e_game_simulation_type game_simulation = game_options_get()->game_simulation;
	if (game_simulation >= _game_simulation_distributed_client &&
		game_simulation <= _game_simulation_distributed_server)
		return true;

	return false;
}

long game_tick_rate_get()
{
	return game_options_get()->game_tick_rate;
}

bool game_skull_is_active_primary(e_campaign_skulls_primary primary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_primary_skulls == primary_skull;

	return false;
}

bool game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		return game_globals->active_secondary_skulls == secondary_skull;

	return false;
}

void game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_primary_skulls.set(primary_skull, enable);
}

void game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable)
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->active_secondary_skulls.set(secondary_skull, enable);
}

bool game_coop_allow_respawn()
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->options.game_mode == _game_mode_campaign && !game_skull_is_active_primary(_campaign_skull_iron);
}

// bool game_survival_allow_respawn(long)

e_language game_get_master_language()
{
	return game_options_get()->language;
}

// bool game_is_language_neutral()

void game_options_setup_default_players(long player_count, game_options* options)
{
	DECLFUNC(0x00532440, void, __cdecl, long, game_options*)(player_count, options);
}

// bool game_is_finished_waiting_for_level_advance()

bool game_is_available(void)
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask;
}

s_game_cluster_bit_vectors* game_get_cluster_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs;
}

s_game_cluster_bit_vectors* game_get_cluster_pvs_local()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs_local;
}

s_game_cluster_bit_vectors* game_get_cluster_activation()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_activation;
}

// bool game_test_cluster_activation(s_cluster_reference* cluster_reference)

void game_pvs_enable_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = true;
}

void game_pvs_clear_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = false;
}

void game_pvs_scripted_set_object(datum_index object_index)
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

// void game_pvs_scripted_set_camera_point(short camera_point_index)

void game_pvs_scripted_clear()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted = 0;
}

// void game_update_pvs()
// void game_won()
// bool game_is_won()

void game_lost(bool game_revert)
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

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

bool game_is_lost()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_lost;
}

// custom like `game_is_finished_immediate`
bool game_is_lost_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_lost && !game_globals->game_lost_wait_time;
}

void game_finish()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

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

void game_finish_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		game_finish();
		game_globals->game_finished_wait_time = 0;
	}
}

bool game_is_finished()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_finished;
}

bool game_is_finished_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_finished && !game_globals->game_finished_wait_time;
}