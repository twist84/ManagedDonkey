#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_start.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role);

bool net_matchmaking_force_gather = true;
bool net_matchmaking_force_search = false;

e_life_cycle_matchmaking_find_match_role __thiscall c_life_cycle_state_handler_matchmaking_start::determine_matchmaking_find_match_role(bool force_gather)
{
	e_life_cycle_matchmaking_find_match_role result = _life_cycle_matchmaking_find_match_role_search;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, force_gather);

	if (net_matchmaking_force_gather) // network_globals.matchmaking_force_gather
	{
		event(_event_message, "networking:logic:life-cycle:matchmaking_start: FORCE GATHER");
		result = _life_cycle_matchmaking_find_match_role_search_gather;
	}
	else if (net_matchmaking_force_search) // network_globals.matchmaking_force_search
	{
		event(_event_message, "networking:logic:life-cycle:matchmaking_start: FORCE SEARCH");
		result =  _life_cycle_matchmaking_find_match_role_search;
	}

	return result;
}

