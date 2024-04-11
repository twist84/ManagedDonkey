#include "game/game_engine_scripting.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_team.hpp"

// mp_active_player_count_by_team
// given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes
long __cdecl game_engine_active_player_count_by_team(long team)
{
	return INVOKE(0x006E5590, game_engine_active_player_count_by_team, team);
}

// mp_ai_allegiance
// causes an allegiance to be formed between an AI squad team and a multiplayer team\r\nNETWORK SAFE: Yes
void __cdecl game_engine_ai_scripting_allegiance(short campaign_team, short team)
{
	INVOKE(0x006E5620, game_engine_ai_scripting_allegiance, campaign_team, team);
}

// deterministic_end_game
// end game deterministically, by inserting a simulation queue event\r\nNETWORK SAFE: Yes
void __cdecl game_engine_finish_game()
{
	INVOKE(0x006E5650, game_engine_finish_game);
}

// used in `game_engine_ai_scripting_allegiance` and `game_engine_mp_team_allegiance`
void __cdecl game_engine_form_team_allegiance(short campaign_team, short team)
{
	INVOKE(0x006E5680, game_engine_form_team_allegiance, campaign_team, team);
}

// mp_game_won
// given a team index, declares the game a victory for that team and a loss for all others\r\nNETWORK SAFE: Yes
void __cdecl game_engine_game_won(short team)
{
	//INVOKE(0x006E5690, game_engine_game_won, team);

	if (!game_is_authoritative())
		return;

	if (game_engine_has_teams())
	{
		if (VALID_INDEX(team, 8))
		{
			game_engine_end_round_with_winner(team, true, _game_engine_end_condition_game_end_scripting);
		}
		else
		{
			generate_event(_event_level_warning, "invalid team index #%ld passed to game_engine_game_won");
		}
	}
	else
	{
		generate_event(_event_level_warning, "game_engine_game_won: not a team game!");
	}
}

// mp_object_belongs_to_team
// causes specified object to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes
void __cdecl game_engine_give_object_ownership_to_team(long object_index, short team)
{
	INVOKE(0x006E56C0, game_engine_give_object_ownership_to_team, object_index, team);
}

// mp_weapon_belongs_to_team
// causes specified weapon to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes
void __cdecl game_engine_give_weapon_ownership_to_team(long weapon_index, short team)
{
	INVOKE(0x006E5730, game_engine_give_weapon_ownership_to_team, weapon_index, team);
}

// mp_allegiance
// create an allegiance between two multiplayer teams\r\nNETWORK SAFE: Yes
void __cdecl game_engine_mp_team_allegiance(short team_a, short team_b)
{
	INVOKE(0x006E5740, game_engine_mp_team_allegiance, team_a, team_b);
}

// mp_players_by_team
// given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes
long __cdecl game_engine_players_by_team(long team)
{
	return INVOKE(0x006E5770, game_engine_players_by_team, team);
}

// mp_respawn_override_timers
// causes all players on the specified team waiting to respawn (due to timer) to respawn immediately\r\nNETWORK SAFE: Yes
void __cdecl game_engine_respawn_override_timers(short team)
{
	INVOKE(0x006E5820, game_engine_respawn_override_timers, team);
}

void __cdecl game_engine_scripting_dispose()
{
	INVOKE(0x006E58D0, game_engine_scripting_dispose);
}

void __cdecl game_engine_scripting_dispose_from_old_map()
{
	INVOKE(0x006E58E0, game_engine_scripting_dispose_from_old_map);
}

void __cdecl game_engine_scripting_initialize()
{
	INVOKE(0x006E58F0, game_engine_scripting_initialize);
}

void __cdecl game_engine_scripting_initialize_for_new_map()
{
	INVOKE(0x006E5900, game_engine_scripting_initialize_for_new_map);
}

