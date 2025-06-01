#pragma once

struct c_game_variant;

extern bool __cdecl game_engine_has_teams();
extern bool __cdecl game_engine_is_team_ever_active(int32 team);
extern int32 __cdecl game_engine_variant_get_maximum_team_count(const c_game_variant* variant, e_map_id multiplayer_map_id);

