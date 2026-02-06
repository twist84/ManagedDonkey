#include "game/game_engine_team.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x005568E0, game_engine_has_player_switching_enabled);
HOOK_DECLARE(0x00556ED0, game_engine_variant_is_observer_allowed);

//.text:00556200 ; void __cdecl game_engine_build_initial_teams(void)
//.text:00556360 ; void __cdecl game_engine_build_teams(void)
//.text:005563B0 ; void __cdecl game_engine_build_valid_team_mapping(void)
//.text:00556500 ; int32 __cdecl game_engine_get_any_player_from_team(enum e_game_team)

uns16 __cdecl game_engine_get_available_teams()
{
	return INVOKE(0x00556590, game_engine_get_available_teams);
}

bool __cdecl game_engine_has_teams()
{
	return INVOKE(0x005565E0, game_engine_has_teams);
}

//.text:00556610 ; bool __cdecl game_engine_is_team_active(enum e_game_team)
//.text:00556660 ; 

bool __cdecl game_engine_is_team_ever_active(int32 team)
{
	return INVOKE(0x005566A0, game_engine_is_team_ever_active, team);
}

//.text:005566F0 ; 
//.text:00556730 ; enum e_game_team __cdecl game_engine_next_active_team(enum e_game_team)
//.text:00556750 ; enum e_game_team __cdecl game_engine_next_inactive_team(enum e_game_team)
//.text:00556770 ; enum e_game_team __cdecl game_engine_next_team_internal(enum e_game_team, bool)
//.text:005567D0 ; void __cdecl game_engine_recompute_active_teams(void)

bool __cdecl game_engine_has_player_switching_enabled()
{
	if (game_is_playback())
	{
		return true;
	}

	const c_game_variant* game_variant = current_game_variant();
	if (!game_variant)
	{
		return false;
	}

	return game_variant->get_active_variant()->get_miscellaneous_options()->get_director_allow_player_switching();
}

//.text:00556900 ; bool __cdecl game_engine_team_designator_is_enemy(enum e_multiplayer_team_designator, enum e_multiplayer_team_designator)
//.text:00556920 ; bool __cdecl game_engine_is_team_designator_valid(enum e_multiplayer_team_designator)
//.text:00556950 ; enum e_game_team __cdecl game_engine_team_designator_to_team_index(enum e_multiplayer_team_designator)
//.text:00556980 ; enum e_multiplayer_team_designator __cdecl game_engine_team_index_to_team_designator(enum e_game_team)
//.text:005569E0 ; bool __cdecl game_engine_team_is_enemy(enum e_game_team, enum e_game_team)
//.text:00556A10 ; bool __cdecl game_engine_team_is_out_of_lives(enum e_game_team)
//.text:00556AC0 ; int32 __cdecl game_engine_teams_get_number_of_players_alive(enum e_game_team)
//.text:00556B60 ; 
//.text:00556C30 ; int32 __cdecl game_engine_teams_get_number_of_shared_lives(void)
//.text:00556C40 ; bool __cdecl game_engine_teams_player_can_change_teams(int32)
//.text:00556DD0 ; bool __cdecl game_engine_teams_use_one_shared_life(enum e_game_team)

int32 __cdecl game_engine_variant_get_maximum_team_count(const c_game_variant* variant, e_map_id multiplayer_map_id)
{
	return INVOKE(0x00556E60, game_engine_variant_get_maximum_team_count, variant, multiplayer_map_id);
}

//.text:00556EB0 ; bool __cdecl game_engine_variant_has_teams(const c_game_variant*)

bool __cdecl game_engine_variant_is_observer_allowed(const c_game_variant* variant)
{
	//INVOKE(0x00556ED0, game_engine_variant_is_observer_allowed, variant);

#if 0
	// Testing
	return true;
#else

#if 1
	// Halo Online
	return false;
#else
	// Halo 3
	ASSERT(variant != nullptr);
	return variant->get_active_variant()->get_social_options()->get_observers_enabled();
#endif

#endif
}

