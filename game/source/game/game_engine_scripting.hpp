#pragma once

extern int32 __cdecl game_engine_active_player_count_by_team(int32 team);
extern void __cdecl game_engine_ai_scripting_allegiance(int16 campaign_team, int16 team);
extern void __cdecl game_engine_finish_game();
extern void __cdecl game_engine_game_won(int16 team);
extern void __cdecl game_engine_give_object_ownership_to_team(int32 object_index, int16 team);
extern void __cdecl game_engine_give_weapon_ownership_to_team(int32 weapon_index, int16 team);
extern void __cdecl game_engine_mp_team_allegiance(int16 team_a, int16 team_b);
extern int32 __cdecl game_engine_players_by_team(int32 team);
extern void __cdecl game_engine_respawn_override_timers(int16 team);
extern void __cdecl game_engine_scripting_dispose();
extern void __cdecl game_engine_scripting_dispose_from_old_map();
extern void __cdecl game_engine_scripting_initialize();
extern void __cdecl game_engine_scripting_initialize_for_new_map();

