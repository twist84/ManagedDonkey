#pragma once

#include "cseries/cseries.hpp"
#include "game/game_progression.hpp"
#include "game/game_options.hpp"

enum e_game_loaded_status
{
	_game_loaded_status_none = 0,
	_game_loaded_status_map_loading,
	_game_loaded_status_map_loaded,
	_game_loaded_status_map_loaded_failure,
	_game_loaded_status_map_unloading,
	_game_loaded_status_map_reloading,
	_game_loaded_status_pregame,

	k_game_loaded_status_count
};

struct s_main_game_globals
{
	long map_memory_configuration;
	c_enum<e_game_loaded_status, long, k_game_loaded_status_count> game_loaded_status;
	char game_loaded_scenario_path[260];

	bool reset_in_progress;
	bool request_level_advance;
	byte : 8;
	byte : 8;

	// ODST, game_progression_levels_block: "type" and block index
	c_enum<e_game_progression_level, long, k_game_progression_level_count> gp_level_advance_type;
	long gp_level_block_index;

	bool change_in_progress;
	bool game_load_pending;
	byte : 8;
	byte : 8;

	dword game_loaded_time;

	dword __unknown120;

	game_options game_loaded_options;
};

extern s_main_game_globals& main_game_globals;

// main_game_load_panic
extern bool& debug_load_panic_to_main_menu;

//extern void main_game_initialize();
//extern void main_game_launch_initialize();
//extern void main_game_launch_default();
//extern void main_game_launch_default_editor();
//extern void main_game_reset_map(bool reset_map);
//extern bool main_game_reset_in_progress();
//extern bool main_game_change_in_progress();
extern void main_game_change(game_options const* options);
//extern void main_game_change_abort();
//extern void main_game_change_update();
//extern bool main_game_change_immediate(game_options const* options);
//extern void main_game_goto_next_level();
//extern bool main_game_load_map(game_options const* options);
//extern void main_game_unload_and_prepare_for_next_game(game_options const* options);
//extern bool main_game_loaded_pregame();
//extern bool main_game_loaded_map();
//extern char const* main_game_loaded_map_name();
//extern void main_game_load_panic();
//extern void main_game_load_from_core_name(char const* core_name);
//extern void main_game_load_from_core();