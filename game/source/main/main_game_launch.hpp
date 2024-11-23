#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"

//extern void main_game_launch_default();
//extern void main_game_launch_default_editor();
extern void main_game_launch_initialize();
extern void main_game_launch_set_map_name(char const* map_name);
extern void main_game_launch_set_difficulty(e_campaign_difficulty_level difficulty);
extern void main_game_launch_campaign();
extern void main_game_launch_survival();
extern void main_game_launch_multiplayer();
extern void main_game_launch_ui_shell();
extern void main_game_launch_set_coop_player_count(long coop_player_count);
extern void main_game_launch_set_active_primary_skulls(long active_primary_skulls);
extern void main_game_launch_set_active_secondary_skulls(long active_secondary_skulls);
extern void main_game_launch_set_multiplayer_splitscreen_count(long multiplayer_splitscreen_count);
extern void main_game_launch_set_multiplayer_engine(char const* multiplayer_engine);
//extern void main_game_launch_set_multiplayer_variant(char const*);
extern void main_game_launch_set_initial_zone_set_index(long initial_zone_set_index);
extern void main_game_launch_set_insertion_point(short insertion_point);
extern void main_game_launch_set_tick_rate(long tick_rate);
extern void main_game_launch_legacy(char const* map_name);
extern void main_game_launch(char const* map_name);
//extern void main_game_launch_process();
extern void main_game_launch_set_active_skulls_primary(long active_skulls_primary);
extern void main_game_launch_set_active_skulls_secondary(long active_skulls_secondary);

extern long main_game_launch_get_player_count();
extern long main_game_launch_get_last_player();