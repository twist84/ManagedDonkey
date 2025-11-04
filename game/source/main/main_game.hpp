#pragma once

#include "cseries/cseries.hpp"
#include "game/game_progression.hpp"
#include "game/game_options.hpp"

struct s_configure_memory
{
	void(__cdecl* dispose_memory_proc)();
	void(__cdecl* configure_memory_proc)(e_map_memory_configuration);
};

struct s_main_game_globals
{
	int32 map_memory_configuration;
	c_enum<e_game_loaded_status, int32, _game_loaded_status_none, k_game_loaded_status_count> game_loaded_status;
	char game_loaded_scenario_path[260];

	bool reset_in_progress;
	bool map_advance_pending;

	// ODST, game_progression_levels_block: "type" and block index
	c_enum<e_game_progression_level, int32, _game_progression_level_none, k_game_progression_level_count> gp_level_advance_type;
	int32 gp_level_block_index;

	bool map_change_pending;
	bool map_change_pending_unload;
	uns32 map_change_initiate_time;
	uns32 map_change_timer;
	game_options pending_game_options;

	// missing in release builds
	inline static bool load_core_on_game_launch = false;
	inline static c_static_string<256> core_name = "";
	inline static int32 launch_player_count = 1;

	game_options launch_game_options;
};
static_assert(sizeof(s_main_game_globals) == 0x497B8);

extern const s_configure_memory(&g_configure_memory_procs)[8];
//extern const s_configure_memory g_configure_memory_procs[8];
extern s_main_game_globals& main_game_globals;

// main_game_load_panic
extern bool& load_panic_recursion_lock;
extern bool debug_load_panic_to_main_menu;

extern e_map_memory_configuration __cdecl compute_desired_map_memory_configuration(const game_options* options);
//extern void __cdecl data_mine_insert_single_player_game_options(const char* event_name);
extern bool __cdecl sub_566CC0();
extern void __cdecl main_game_campaign_loaded(const game_options* options);
extern void __cdecl main_game_change(const game_options* options);
extern void __cdecl main_game_change_abort();
extern bool __cdecl main_game_change_immediate(const game_options* options);
extern bool __cdecl main_game_change_in_progress();
extern void __cdecl main_game_change_update();
extern void __cdecl main_game_configure_map_memory(const game_options* options);
extern void __cdecl main_game_configure_map_memory_pop();
extern void __cdecl main_game_configure_map_memory_push(e_map_memory_configuration desired_memory_configuration);
extern bool __cdecl main_game_goto_next_level();
extern void __cdecl main_game_initialize();
extern void __cdecl main_game_internal_close_caches();
extern void __cdecl main_game_internal_map_load_abort(bool reload_map);
extern void __cdecl main_game_internal_map_load_begin(bool reload_map);
extern bool __cdecl main_game_internal_map_load_complete(bool reload_map, const game_options* options);
extern void __cdecl main_game_internal_map_unload_begin();
extern void __cdecl main_game_internal_map_unload_complete();
extern bool __cdecl main_game_internal_open_caches(const game_options* options);
extern void __cdecl main_game_pregame_blocking_load();
extern void __cdecl main_game_internal_pregame_blocking_unload();
extern void __cdecl main_game_internal_pregame_load();
extern void __cdecl main_game_internal_pregame_unload();
extern void __cdecl main_game_launch_default();
extern void __cdecl main_game_launch_default_editor();
extern bool __cdecl main_game_load_blocking(const char* scenario_path);
extern bool __cdecl main_game_load_map(const game_options* options);
extern void __cdecl main_game_load_panic();
extern void __cdecl main_game_load_from_core_name(const char* core_name);
extern void __cdecl main_game_load_from_core();
extern bool __cdecl main_game_loaded_map();
extern const char* __cdecl main_game_loaded_map_name();
extern bool __cdecl main_game_loaded_pregame();
extern void __cdecl main_game_reload_map(int32 initial_zone_set_index);
extern void __cdecl main_game_reload_map_by_name(const char* map_name, int32 initial_zone_set_index);
extern void __cdecl main_game_request_level_advance_spoke(int32 level_index);
extern void __cdecl main_game_request_level_advance();
extern void __cdecl main_game_request_level_advance_hub(int32 level_index);
extern void __cdecl main_game_request_level_advance_normal(int32 level_index);
extern bool __cdecl main_game_reset_in_progress();
extern void __cdecl main_game_reset_map(bool reset_map_random);
extern bool __cdecl main_game_start(const game_options* options);
extern void __cdecl main_game_unload_and_prepare_for_next_game(const game_options* options);
extern void __cdecl main_menu_build_game_options(game_options* options);
extern void __cdecl main_menu_launch();
extern void __cdecl main_menu_launch_force();
extern bool __cdecl map_memory_configuration_is_campaign(e_map_memory_configuration configuration);
extern bool __cdecl map_memory_configuration_is_campaign_epilogue(e_map_memory_configuration configuration);
extern bool __cdecl map_memory_configuration_is_main_menu(e_map_memory_configuration configuration);
extern bool __cdecl map_memory_configuration_is_multiplayer(e_map_memory_configuration configuration);
extern bool __cdecl map_memory_configuration_is_saved_film(e_map_memory_configuration configuration);

