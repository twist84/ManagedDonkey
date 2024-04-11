#pragma once

extern long __cdecl game_engine_active_player_count_by_team(long team);
extern void __cdecl game_engine_ai_scripting_allegiance(short campaign_team, short team);
extern void __cdecl game_engine_finish_game();
extern void __cdecl game_engine_game_won(short team);
extern void __cdecl game_engine_give_object_ownership_to_team(long object_index, short team);
extern void __cdecl game_engine_give_weapon_ownership_to_team(long weapon_index, short team);
extern void __cdecl game_engine_mp_team_allegiance(short team_a, short team_b);
extern long __cdecl game_engine_players_by_team(long team);
extern void __cdecl game_engine_respawn_override_timers(short team);
extern void __cdecl game_engine_scripting_dispose();
extern void __cdecl game_engine_scripting_dispose_from_old_map();
extern void __cdecl game_engine_scripting_initialize();
extern void __cdecl game_engine_scripting_initialize_for_new_map();

