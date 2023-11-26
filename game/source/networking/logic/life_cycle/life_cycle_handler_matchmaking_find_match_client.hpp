#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_find_match_client : c_life_cycle_state_handler
{
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_find_match_client) == 0x30);

