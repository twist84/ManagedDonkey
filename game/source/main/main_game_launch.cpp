#include "main_game_launch.hpp"

#include "cache/cache_files_windows.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "main/main_game.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "saved_games/saved_film_manager.hpp"

void main_game_launch(const char* map_name)
{
	cache_file_map_clear_all_failures();
	main_game_launch_set_map_name(map_name);

	if (!is_debugger_present())
	{
		if (main_game_globals.launch_game_options.game_mode == _game_mode_campaign && csstrstr(map_name, "levels\\multi\\"))
		{
			event(_event_warning, "switching you from a campaign game to multiplayer, b/c it looks like you are trying to load a multiplayer map!");

			const char* string = game_engine_type_get_string(_game_engine_type_slayer);
			main_game_launch_set_multiplayer_engine(string);
		}

		if (main_game_globals.launch_game_options.game_mode == _game_mode_multiplayer && csstrstr(map_name, "levels\\solo\\"))
		{
			event(_event_warning, "switching you from a multiplayer game to campaign, b/c it looks like you are trying to load a campaign map!");

			main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
		}
	}

	if (network_life_cycle_get_state())
	{
		network_life_cycle_end();
	}

	if (main_game_globals.launch_game_options.game_mode == _game_mode_campaign)
	{
		main_game_globals.launch_game_options.campaign_difficulty = int_pin(main_game_globals.launch_game_options.campaign_difficulty, _campaign_difficulty_level_easy, _campaign_difficulty_level_impossible);
	}
	else if (main_game_globals.launch_game_options.game_mode != _game_mode_multiplayer)
	{
		event(_event_warning, "main_game_launch: unknown game mode %d!", main_game_globals.launch_game_options.game_mode);
		return;
	}

	main_game_globals.launch_player_count = int_pin(main_game_globals.launch_player_count, 1, 4);
	main_game_globals.launch_game_options.record_saved_film = saved_film_manager_should_record_film(&main_game_globals.launch_game_options);
	game_options_setup_default_players(main_game_globals.launch_player_count, &main_game_globals.launch_game_options);
	game_options_validate(&main_game_globals.launch_game_options);
	main_game_change(&main_game_globals.launch_game_options);
}

void main_game_launch_campaign()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
}

//void main_game_launch_default()
//void main_game_launch_default_editor()

int32 main_game_launch_get_player_count()
{
	return main_game_globals.launch_player_count;
}

// the last player controls the game visuals, hud and other stuff
int32 main_game_launch_get_last_player()
{
	return main_game_globals.launch_player_count - 1;
}

void main_game_launch_initialize()
{
	game_options_new(&main_game_globals.launch_game_options);
}

void main_game_launch_legacy(const char* map_name)
{
	//damaged_media_clear_error();
	main_game_launch(map_name);
}

void main_game_launch_multiplayer()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
}

//void main_game_launch_process()

void main_game_launch_set_active_primary_skulls(int32 active_primary_skulls)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_active_primary_skulls = active_primary_skulls;
}

void main_game_launch_set_active_secondary_skulls(int32 active_secondary_skulls)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_active_secondary_skulls = active_secondary_skulls;
}

void main_game_launch_set_coop_player_count(int32 coop_player_count)
{
	if (!IN_RANGE_INCLUSIVE(coop_player_count, 1, 4))
	{
		event(_event_warning, "main_game_launch_set_coop_player_count: invalid player count %d (must be from 1-%d)", coop_player_count, 4);
	}
	else
	{
		main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
		main_game_globals.launch_player_count = coop_player_count;
	}
}

void main_game_launch_set_difficulty(int32 difficulty)
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.campaign_difficulty = (int16)difficulty;
	game_set_difficulty((int16)difficulty);
}

void main_game_launch_set_initial_zone_set_index(int32 initial_zone_set_index)
{
	main_game_globals.launch_game_options.initial_zone_set_index = (int16)initial_zone_set_index;
}

void main_game_launch_set_insertion_point(int16 insertion_point)
{
	main_game_globals.launch_game_options.campaign_insertion_point = insertion_point;
}

void main_game_launch_set_map_name(const char* map_name)
{
	ASSERT(map_name);
	main_game_globals.launch_game_options.scenario_path.set(map_name);
}

void main_game_launch_set_multiplayer_engine(const char* engine_name)
{
	for (int32 i = _game_engine_type_none; i < k_game_engine_type_count; i++)
	{
		if (csstricmp(engine_name, game_engine_type_get_string(i)) != 0)
			continue;

		e_game_engine_type game_engine_index = e_game_engine_type(i);
		main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
		build_default_game_variant(&main_game_globals.launch_game_options.multiplayer_variant, game_engine_index);

		return;
	}

	event(_event_error, "launch: failed to find multiplayer game engine '%s'", engine_name);
}

void main_game_launch_set_multiplayer_splitscreen_count(int32 splitscreen_count)
{
	if (!IN_RANGE_INCLUSIVE(splitscreen_count, 1, 4))
	{
		event(_event_warning, "main_game_launch_set_multiplayer_splitscreen_count: invalid player count %d (must be from 1-%d)",
			splitscreen_count,
			4);
	}
	else
	{
		main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
		main_game_globals.launch_player_count = splitscreen_count;

		if (main_game_globals.launch_game_options.multiplayer_variant.get_game_engine_index() == _game_engine_type_none)
			build_default_game_variant(&main_game_globals.launch_game_options.multiplayer_variant, _game_engine_type_slayer);
	}
}

//void main_game_launch_set_multiplayer_variant(const char* variant_name)
//{
//	c_game_variant variant{};
//	if (game_engine_get_built_in_variant(variant_name, &variant))
//	{
//		main_game_globals.launch_game_options.game_mode = _game_mode_multiplayer;
//		csmemcpy(&main_game_globals.launch_game_options.multiplayer_variant, &variant, sizeof(c_game_variant));
//	}
//	else
//	{
//		event(_event_error, "launch: failed to find multiplayer variant '%s'", variant_name);
//	}
//}

void main_game_launch_set_tick_rate(int32 tick_rate)
{
	main_game_globals.launch_game_options.game_tick_rate = (int16)tick_rate;
}

void main_game_launch_survival()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_campaign;
	main_game_globals.launch_game_options.survival_enabled = true;
}

void main_game_launch_ui_shell()
{
	main_game_globals.launch_game_options.game_mode = _game_mode_ui_shell;
}

