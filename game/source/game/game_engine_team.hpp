#pragma once

class c_game_variant;

extern uns16 __cdecl game_engine_get_available_teams();
extern bool __cdecl game_engine_has_teams();
extern bool __cdecl game_engine_is_team_ever_active(int32 team);
extern bool __cdecl game_engine_has_player_switching_enabled();
extern int32 __cdecl game_engine_variant_get_maximum_team_count(const c_game_variant* variant, e_map_id multiplayer_map_id);
extern bool __cdecl game_engine_variant_is_observer_allowed(const c_game_variant* variant);

