#include "main_game_launch.hpp"

#include "cache/cache_files_windows.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "main/main_game.hpp"
#include "networking/logic/network_life_cycle.hpp"

// void main_game_launch_default()
// void main_game_launch_default_editor()

void main_game_launch_initialize()
{
	game_options_new(&main_game_globals.launch_game_options);
}

void main_game_launch_set_map_name(char const* map_name)
{
	ASSERT(map_name);
	main_game_globals.launch_game_options.scenario_path.set(map_name);
}

void main_game_launch_set_difficulty(e_campaign_difficulty_level difficulty)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_difficulty = difficulty;
	game_set_difficulty(difficulty);
}

void main_game_launch_campaign()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
}

void main_game_launch_survival()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.survival_enabled = true;
}

void main_game_launch_multiplayer()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
}

void main_game_launch_ui_shell()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_ui_shell;
}

void main_game_launch_set_active_primary_skulls(long active_primary_skulls)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_active_primary_skulls = active_primary_skulls;
}

void main_game_launch_set_active_secondary_skulls(long active_secondary_skulls)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_active_secondary_skulls = active_secondary_skulls;
}

void main_game_launch_set_coop_player_count(long coop_player_count)
{
	if (!IN_RANGE_INCLUSIVE(coop_player_count, 1, 4))
	{
		generate_event(_event_warning, "main_game_launch_set_coop_player_count: invalid player count %d (must be from 1-%d)", coop_player_count, 4);
	}
	else
	{
		main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
		main_game_globals.launch_player_count = coop_player_count;
	}
}

void main_game_launch_set_multiplayer_splitscreen_count(long multiplayer_splitscreen_count)
{
	if (!IN_RANGE_INCLUSIVE(multiplayer_splitscreen_count, 1, 4))
	{
		generate_event(_event_warning, "main_game_launch_set_multiplayer_splitscreen_count: invalid player count %d (must be from 1-%d)", multiplayer_splitscreen_count, 4);
	}
	else
	{
		main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
		main_game_globals.launch_player_count = multiplayer_splitscreen_count;

		if (main_game_globals.launch_game_options.multiplayer_variant.get_game_engine_index() == _game_engine_type_none)
			build_default_game_variant(&main_game_globals.launch_game_options.multiplayer_variant, _game_engine_type_slayer);
	}
}

void main_game_launch_set_multiplayer_engine(char const* engine_name)
{
	for (long i = _game_engine_type_none; i < k_game_engine_type_count; i++)
	{
		if (csstricmp(engine_name, game_engine_type_get_string(i)) != 0)
			continue;

		e_game_engine_type game_engine_index = e_game_engine_type(i);
		main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
		build_default_game_variant(&main_game_globals.launch_game_options.multiplayer_variant, game_engine_index);

		return;
	}

	generate_event(_event_error, "launch: failed to find multiplayer game engine '%s'", engine_name);
}

// void main_game_launch_set_multiplayer_variant(char const* multiplayer_variant)

void main_game_launch_set_initial_zone_set_index(long initial_zone_set_index)
{
	main_game_globals.launch_game_options.initial_zone_set_index = static_cast<short>(initial_zone_set_index);
}

void main_game_launch_set_insertion_point(short insertion_point)
{
	main_game_globals.launch_game_options.campaign_insertion_point = insertion_point;
}

void main_game_launch_set_tick_rate(long tick_rate)
{
	main_game_globals.launch_game_options.game_tick_rate = static_cast<short>(tick_rate);
}

void main_game_launch_legacy(char const* map_name)
{
	//damaged_media_clear_error();
	main_game_launch(map_name);
}

void main_game_launch(char const* map_name)
{
	cache_file_map_clear_all_failures();
	main_game_launch_set_map_name(map_name);

	long launch_player_count = 1;

	if (!is_debugger_present())
	{
		if (main_game_globals.launch_game_options.game_mode == _game_mode_campaign && csstrstr(map_name, "levels\\multi\\"))
		{
			generate_event(_event_warning, "switching you from a campaign game to multiplayer, b/c it looks like you are trying to load a multiplayer map!");

			char const* string = game_engine_type_get_string(_game_engine_type_slayer);
			main_game_launch_set_multiplayer_engine(string);
		}

		if (main_game_globals.launch_game_options.game_mode == _game_mode_multiplayer && csstrstr(map_name, "levels\\solo\\"))
		{
			generate_event(_event_warning, "switching you from a multiplayer game to campaign, b/c it looks like you are trying to load a campaign map!");

			main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
		}
	}

	if (network_life_cycle_get_state())
		network_life_cycle_end();

	if (main_game_globals.launch_game_options.game_mode == _game_mode_campaign)
	{
		main_game_globals.launch_game_options.campaign_difficulty = int_pin(main_game_globals.launch_game_options.campaign_difficulty, _campaign_difficulty_level_easy, _campaign_difficulty_level_legendary);
	}
	else if (main_game_globals.launch_game_options.game_mode != _game_mode_multiplayer)
	{
		generate_event(_event_warning, "main_game_launch: unknown game mode %d!", main_game_globals.launch_game_options.game_mode);
		return;
	}

	main_game_globals.launch_player_count = int_pin(launch_player_count, 1, 4);
	main_game_globals.launch_game_options.record_saved_film = false; // saved_film_manager_should_record_film(&main_game_globals.launch_game_options);
	game_options_setup_default_players(main_game_globals.launch_player_count, &main_game_globals.launch_game_options);
	game_options_validate(&main_game_globals.launch_game_options);
	main_game_change(&main_game_globals.launch_game_options);
}

// void main_game_launch_process()

long main_game_launch_get_player_count()
{
	return main_game_globals.launch_player_count;
}

// the last player controls the game visuals, hud and other stuff
long main_game_launch_get_last_player()
{
	return main_game_globals.launch_player_count - 1;
}

