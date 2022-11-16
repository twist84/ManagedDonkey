#pragma once

extern long __cdecl game_engine_get_player_place(long absolute_player_index);
extern long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score);
extern long __cdecl game_engine_get_team_place(long team);
extern long __cdecl game_engine_get_team_score_for_display(long team, bool final_score);
extern bool __cdecl game_engine_in_round();
extern void __cdecl game_engine_interface_update(float world_seconds_elapsed);
extern long __cdecl game_engine_get_pre_round_ticks();