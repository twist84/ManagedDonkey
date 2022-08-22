#include "game/game_options.hpp"

#include "game/game_globals.hpp"

#include <cassert>
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

bool game_options_verify(game_options const* options, char* error_string, long error_string_length)
{
	bool(__cdecl * game_options_verify_ptr)(game_options const* options) = reinterpret_cast<decltype(game_options_verify_ptr)>(0x005326F0);

	return game_options_verify_ptr(options);
}

void assert_game_options_verify(game_options const* options)
{
	void(__cdecl * assert_game_options_verify_ptr)(game_options const* options) = reinterpret_cast<decltype(assert_game_options_verify_ptr)>(0x00530440);

	char error_string[512]{};
	assert(game_options_verify(options, error_string, sizeof(error_string)));
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

// bool game_had_an_update_tick_this_frame()

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
	void(__cdecl * game_options_setup_default_players_ptr)(long player_count, game_options*) = reinterpret_cast<decltype(game_options_setup_default_players_ptr)>(0x00532440);

	return game_options_setup_default_players_ptr(player_count, options);
}

// bool game_is_finished_waiting_for_level_advance()