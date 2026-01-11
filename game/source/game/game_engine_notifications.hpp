#pragma once

extern void __cdecl game_engine_player_changed_handicap_levels(int32 player_index, int16 old_handicap_level, int16 new_handicap_level);
extern void __cdecl game_engine_player_changed_indices(int32 player_old_index, int32 player_new_index);
extern void __cdecl game_engine_player_changed_names(int32 player_index, const wchar_t* old_name, const wchar_t* new_name);
extern void __cdecl game_engine_player_changed_teams(int32 player_index, e_game_team old_team_index, e_game_team new_team_index);
extern void __cdecl game_engine_player_deleted(int32 player_index);
extern void __cdecl game_engine_player_nearby_item(int32 player_index, int32 item_index);
extern void __cdecl game_engine_player_prepare_to_change_team(int32 player_index, e_game_team team_index);

