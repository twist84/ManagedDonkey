#include "game/game_engine_team.hpp"

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

bool __cdecl game_engine_has_teams()
{
    return INVOKE(0x005565E0, game_engine_has_teams);
}

bool __cdecl game_engine_is_team_ever_active(long team)
{
    return INVOKE(0x005566A0, game_engine_is_team_ever_active, team);
}

