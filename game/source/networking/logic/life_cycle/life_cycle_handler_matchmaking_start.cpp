#include "life_cycle_handler_matchmaking_start.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role);

bool net_matchmaking_force_gather = true;
bool net_matchmaking_force_search = false;

long __fastcall c_life_cycle_state_handler_matchmaking_start::determine_matchmaking_find_match_role(c_life_cycle_state_handler_matchmaking_start* _this, void* unused, bool a2)
{
    long result = 1;
    HOOK_INVOKE_CLASS(result =, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, long(__thiscall*)(c_life_cycle_state_handler_matchmaking_start*, bool), _this, a2);

    if (net_matchmaking_force_gather)
    {
        c_console::write_line("networking:logic:life-cycle:matchmaking_start: FORCE GATHER");
        result = 2;
    }
    else if (net_matchmaking_force_search)
    {
        c_console::write_line("networking:logic:life-cycle:matchmaking_start: FORCE SEARCH");
        result = 1;
    }

    return result;
}

