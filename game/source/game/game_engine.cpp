#include "game/game_engine.hpp"

#include "cseries/cseries.hpp"
#include "cseries/console.hpp"
#include "game/game_time.hpp"
#include "memory/module.hpp"

//HOOK_DECLARE(0x005521D0, game_engine_get_pre_round_ticks);

long __cdecl game_engine_get_player_place(long absolute_player_index)
{
	return INVOKE(0x005506F0, game_engine_get_player_place, absolute_player_index);
}

long __cdecl game_engine_get_player_score(long absolute_player_index)
{
	return INVOKE(0x00550720, game_engine_get_player_score, absolute_player_index);
}

long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score)
{
	return INVOKE(0x005507D0, game_engine_get_player_score_for_display, absolute_player_index, final_score);
}

long __cdecl game_engine_get_team_place(long team)
{
	return INVOKE(0x00550BF0, game_engine_get_team_place, team);
}

long __cdecl game_engine_get_team_score_for_display(long team, bool final_score)
{
	return INVOKE(0x00550D10, game_engine_get_team_score_for_display, team, final_score);
}

bool __cdecl game_engine_in_round()
{
	return INVOKE(0x00550F90, game_engine_in_round);
}

long __cdecl game_engine_get_pre_round_ticks()
{
	FUNCTION_BEGIN(false);

    return game_seconds_integer_to_ticks(8);
}

