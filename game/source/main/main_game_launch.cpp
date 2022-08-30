#include "main_game_launch.hpp"

#include "game/game.hpp"
#include "main/main_game.hpp"
#include "networking/logic/network_life_cycle.hpp"

// memcpy, timeGetTime
#include <windows.h>

#include <cassert> // assert
#include <stdio.h> //printf
#include <string>

s_main_game_launch_globals g_launch_globals = { .player_count = 1, .options = *reinterpret_cast<game_options*>(0x023B6348) };

// void main_game_launch_default()
// void main_game_launch_default_editor()

void main_game_launch_initialize()
{
	//game_options_new(&g_launch_globals.options);
}

void main_game_launch_set_map_name(char const* map_name)
{
	assert(map_name);
	strncpy_s(g_launch_globals.options.scenario_path, map_name, 260);
}

void main_game_launch_set_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
	g_launch_globals.options.campaign_difficulty = campaign_difficulty;
	game_set_difficulty(campaign_difficulty);
}

void main_game_launch_campaign()
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
}

void main_game_launch_survival()
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
	g_launch_globals.options.survival_enabled = true;
}

void main_game_launch_multiplayer()
{
	g_launch_globals.options.game_mode = _game_mode_multiplayer;
}

void main_game_launch_ui_shell()
{
	g_launch_globals.options.game_mode = _game_mode_mainmenu;
}

void main_game_launch_set_coop_player_count(long player_count)
{
	if (player_count > 0 && player_count <= 4)
	{
		g_launch_globals.options.game_mode = _game_mode_campaign;
		g_launch_globals.player_count = player_count;
	}
	else
	{
		printf("main_game_launch_set_coop_player_count: invalid player count %d (must be from 1-%d)", player_count, 4);
	}
}

void main_game_launch_set_multiplayer_splitscreen_count(long player_count)
{
	if (player_count > 0 && player_count <= 4)
	{
		g_launch_globals.options.game_mode = _game_mode_multiplayer;
		g_launch_globals.player_count = player_count;
		if (g_launch_globals.options.game_variant.m_game_engine_index == _game_engine_base_variant)
			g_launch_globals.options.game_variant.m_game_engine_index = _game_engine_slayer_variant;

		//if (g_launch_globals.options.game_variant.m_game_engine_index == _game_engine_base_variant)
		//    build_default_game_variant(&g_launch_globals.options.game_variant, _game_engine_slayer_variant);
	}
	else
	{
		printf("main_game_launch_set_multiplayer_splitscreen_count: invalid player count %d (must be from 1-%d)", player_count, 4);
	}
}

void main_game_launch_set_multiplayer_engine(char const* engine_name)
{
	e_game_engine_variant game_engine_index;

	for (long i = _game_engine_base_variant; i < k_game_engine_variant_count; i++)
	{
		if (strcmp(engine_name, game_engine_variant_get_name(i)) != 0)
			continue;

		game_engine_index = e_game_engine_variant(i);
	}

	g_launch_globals.options.game_variant.m_game_engine_index = game_engine_index;
}

// void main_game_launch_set_multiplayer_variant(char const*)
// void main_game_launch_set_initial_zone_set_index(long)
// void main_game_launch_set_insertion_point(short)
// void main_game_launch_set_tick_rate(long)
// void main_game_launch_legacy(char const*)

void main_game_launch(const char* map_name)
{
	main_game_globals;
	debug_load_panic_to_main_menu;

	main_game_launch_set_map_name(map_name);
	if (network_life_cycle_get_state())
		network_life_cycle_end();

	if (g_launch_globals.options.game_mode == _game_mode_campaign)
	{
		if (g_launch_globals.options.campaign_difficulty.get() <= _campaign_difficulty_level_easy)
			g_launch_globals.options.campaign_difficulty = _campaign_difficulty_level_easy;
		if (g_launch_globals.options.campaign_difficulty.get() > k_campaign_difficulty_levels_count)
			g_launch_globals.options.campaign_difficulty = _campaign_difficulty_level_legendary;
	}
	else if (g_launch_globals.options.game_mode != _game_mode_multiplayer)
	{
		printf("main_game_launch: unknown game mode %d!", g_launch_globals.options.game_mode.get());
		return;
	}

	g_launch_globals.options.record_saved_film = false;

	if (g_launch_globals.player_count < 1)
		g_launch_globals.player_count = 1;
	if (g_launch_globals.player_count > 4)
		g_launch_globals.player_count = 4;

	game_options_setup_default_players(g_launch_globals.player_count, &g_launch_globals.options);
	//game_options_validate(&g_launch_globals.options);
	main_game_change(&g_launch_globals.options);
}

// void main_game_launch_process()

void main_game_launch_set_active_skulls_primary(long primary_skull)
{
	g_launch_globals.options.skulls_primary = primary_skull;
}

void main_game_launch_set_active_skulls_secondary(long secondary_skull)
{
	g_launch_globals.options.skulls_secondary = secondary_skull;
}

long main_game_launch_get_player_count()
{
	return g_launch_globals.player_count;
}

// the last player controls the game visuals, hud and other stuff
long main_game_launch_get_last_player()
{
	return g_launch_globals.player_count - 1;
}
