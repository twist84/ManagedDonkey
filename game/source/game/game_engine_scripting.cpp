#include "game/game_engine_scripting.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_team.hpp"
#include "memory/thread_local.hpp"

// mp_active_player_count_by_team
// given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes
int32 __cdecl game_engine_active_player_count_by_team(int32 team)
{
	return INVOKE(0x006E5590, game_engine_active_player_count_by_team, team);

	//if (!VALID_INDEX(team, k_multiplayer_team_count))
	//{
	//	event(_event_warning, "invalid team index #%ld passed to game_engine_active_player_count_by_team", team);
	//	return 0;
	//}
	//
	//int32 player_count = 0;
	//
	//TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//for (int32 i = data_next_index(*player_data, NONE); i != NONE; i = data_next_index(*player_data, i))
	//{
	//	player_datum* player = &player_data[i];
	//	if (player->configuration.host.team_index == team && (player->unit_index != NONE || player->multiplayer.lives_remaining > 0))
	//		player_count++;
	//}
	//
	//return player_count;
}

// mp_ai_allegiance
// causes an allegiance to be formed between an AI squad team and a multiplayer team\r\nNETWORK SAFE: Yes
void __cdecl game_engine_ai_scripting_allegiance(int16 campaign_team, int16 team)
{
	INVOKE(0x006E5620, game_engine_ai_scripting_allegiance, campaign_team, team);

	//if (!VALID_INDEX(campaign_team, k_campaign_team_count))
	//{
	//	event(_event_warning, "mp_ai_allegiance: invalid campaign team #%d!", campaign_team);
	//	return;
	//}
	//
	//if (!VALID_INDEX(team, k_multiplayer_team_count))
	//{
	//	event(_event_warning, "mp_ai_allegiance: invalid multiplayer team #%d!", team);
	//	return;
	//}
	//
	//if (game_is_authoritative())
	//	ai_scripting_allegiance(campaign_team, team);
}

// deterministic_end_game
// end game deterministically, by inserting a simulation queue event\r\nNETWORK SAFE: Yes
void __cdecl game_engine_finish_game()
{
	INVOKE(0x006E5650, game_engine_finish_game);

	//if (game_in_progress() && game_is_multiplayer() && game_is_authoritative())
	//	simulation_queue_game_global_event_insert(2);
}

// used in `game_engine_ai_scripting_allegiance` and `game_engine_mp_team_allegiance`
void __cdecl game_engine_form_team_allegiance(int16 campaign_team, int16 team)
{
	INVOKE(0x006E5680, game_engine_form_team_allegiance, campaign_team, team);
}

// mp_game_won
// given a team index, declares the game a victory for that team and a loss for all others\r\nNETWORK SAFE: Yes
void __cdecl game_engine_game_won(int16 team)
{
	//INVOKE(0x006E5690, game_engine_game_won, team);

	if (!game_is_authoritative())
		return;

	if (!game_engine_has_teams())
	{
		event(_event_warning, "game_engine_game_won: not a team game!");
		return;
	}

	if (!VALID_INDEX(team, k_multiplayer_team_count))
	{
		event(_event_warning, "invalid team index #%ld passed to game_engine_game_won");
		return;
	}

	game_engine_end_round_with_winner((e_game_team)team, true, _game_engine_game_end_scripting);
}

// mp_object_belongs_to_team
// causes specified object to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes
void __cdecl game_engine_give_object_ownership_to_team(int32 object_index, int16 team)
{
	INVOKE(0x006E56C0, game_engine_give_object_ownership_to_team, object_index, team);
}

// mp_weapon_belongs_to_team
// causes specified weapon to belong to the given team, so that only that team can pick it up\r\nNETWORK SAFE: Yes
void __cdecl game_engine_give_weapon_ownership_to_team(int32 weapon_index, int16 team)
{
	INVOKE(0x006E5730, game_engine_give_weapon_ownership_to_team, weapon_index, team);
}

// mp_allegiance
// create an allegiance between two multiplayer teams\r\nNETWORK SAFE: Yes
void __cdecl game_engine_mp_team_allegiance(int16 team_a, int16 team_b)
{
	INVOKE(0x006E5740, game_engine_mp_team_allegiance, team_a, team_b);
}

// mp_players_by_team
// given a team index, returns an object list containing all living player objects belonging to that team\r\nNETWORK SAFE: Yes
int32 __cdecl game_engine_players_by_team(int32 team)
{
	return INVOKE(0x006E5770, game_engine_players_by_team, team);
}

// mp_respawn_override_timers
// causes all players on the specified team waiting to respawn (due to timer) to respawn immediately\r\nNETWORK SAFE: Yes
void __cdecl game_engine_respawn_override_timers(int16 team)
{
	INVOKE(0x006E5820, game_engine_respawn_override_timers, team);

	//if (!game_is_authoritative())
	//	return;
	//
	//if (!VALID_INDEX(team, k_multiplayer_team_count))
	//	return;
	//
	//TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	//
	//c_player_in_game_iterator player_iterator;
	//player_iterator.begin();
	//while (player_iterator.next())
	//{
	//	player_datum* player = player_iterator.get_datum();
	//	if (player->configuration.host.team_index == (int32)team)
	//	{
	//		player->respawn_timer = 0;
	//		game_engine_globals->player_navpoint_data[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_iterator.get_index())].__data[0] = 0;
	//
	//		// no call to `simulation_action_game_engine_player_update` in Halo Online
	//	}
	//}
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

