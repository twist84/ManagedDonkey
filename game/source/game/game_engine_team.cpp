#include "game/game_engine_team.hpp"

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

//.text:00556200 ; void __cdecl game_engine_build_initial_teams(void)
//.text:00556360 ; void __cdecl game_engine_build_teams(void)
//.text:005563B0 ; void __cdecl game_engine_build_valid_team_mapping(void)
//.text:00556500 ; long __cdecl game_engine_get_any_player_from_team(enum e_multiplayer_team)
//.text:00556590 ; unsigned short __cdecl game_engine_get_available_teams(void)

bool __cdecl game_engine_has_teams()
{
	return INVOKE(0x005565E0, game_engine_has_teams);
}

//.text:00556610 ; bool __cdecl game_engine_is_team_active(enum e_multiplayer_team)
//.text:00556660 ; 

bool __cdecl game_engine_is_team_ever_active(long team)
{
	return INVOKE(0x005566A0, game_engine_is_team_ever_active, team);
}

//.text:005566F0 ; 
//.text:00556730 ; enum e_multiplayer_team __cdecl game_engine_next_active_team(enum e_multiplayer_team)
//.text:00556750 ; enum e_multiplayer_team __cdecl game_engine_next_inactive_team(enum e_multiplayer_team)
//.text:00556770 ; enum e_multiplayer_team __cdecl game_engine_next_team_internal(enum e_multiplayer_team, bool)
//.text:005567D0 ; void __cdecl game_engine_recompute_active_teams(void)
//.text:005568E0 ; 
//.text:00556900 ; bool __cdecl game_engine_team_designator_is_enemy(enum e_multiplayer_team_designator, enum e_multiplayer_team_designator)
//.text:00556920 ; bool __cdecl game_engine_is_team_designator_valid(enum e_multiplayer_team_designator)
//.text:00556950 ; enum e_multiplayer_team __cdecl game_engine_team_designator_to_team_index(enum e_multiplayer_team_designator)
//.text:00556980 ; enum e_multiplayer_team_designator __cdecl game_engine_team_index_to_team_designator(enum e_multiplayer_team)
//.text:005569E0 ; bool __cdecl game_engine_team_is_enemy(enum e_multiplayer_team, enum e_multiplayer_team)
//.text:00556A10 ; bool __cdecl game_engine_team_is_out_of_lives(enum e_multiplayer_team)
//.text:00556AC0 ; long __cdecl game_engine_teams_get_number_of_players_alive(enum e_multiplayer_team)
//.text:00556B60 ; 
//.text:00556C30 ; long __cdecl game_engine_teams_get_number_of_shared_lives(void)
//.text:00556C40 ; bool __cdecl game_engine_teams_player_can_change_teams(long)
//.text:00556DD0 ; bool __cdecl game_engine_teams_use_one_shared_life(enum e_multiplayer_team)
//.text:00556E60 ; long __cdecl game_engine_variant_get_maximum_team_count(class c_game_variant const *, enum e_map_id)
//.text:00556EB0 ; bool __cdecl game_engine_variant_has_teams(class c_game_variant const *)
//.text:00556ED0 ; bool __cdecl game_engine_variant_is_observer_allowed(class c_game_variant const *)

