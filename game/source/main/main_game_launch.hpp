#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"

extern void main_game_launch(const char* map_name);
extern void main_game_launch_campaign();
//extern void main_game_launch_default();
//extern void main_game_launch_default_editor();
extern int32 main_game_launch_get_player_count();
extern int32 main_game_launch_get_last_player();
extern void main_game_launch_initialize();
extern void main_game_launch_legacy(const char* map_name);
extern void main_game_launch_multiplayer();
//extern void main_game_launch_process();
extern void main_game_launch_set_active_primary_skulls(int32 active_primary_skulls);
extern void main_game_launch_set_active_secondary_skulls(int32 active_secondary_skulls);
extern void main_game_launch_set_coop_player_count(int32 coop_player_count);
extern void main_game_launch_set_difficulty(int32 difficulty);
extern void main_game_launch_set_initial_zone_set_index(int32 initial_zone_set_index);
extern void main_game_launch_set_insertion_point(int16 insertion_point);
extern void main_game_launch_set_map_name(const char* map_name);
extern void main_game_launch_set_multiplayer_engine(const char* engine_name);
extern void main_game_launch_set_multiplayer_splitscreen_count(int32 splitscreen_count);
//extern void main_game_launch_set_multiplayer_variant(const char* multiplayer_variant);
extern void main_game_launch_set_tick_rate(int32 tick_rate);
extern void main_game_launch_survival();
extern void main_game_launch_ui_shell();

