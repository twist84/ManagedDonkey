#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_matchmaking_find_and_assemble_match : c_life_cycle_state_handler
{
	byte __data[0x19B38];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_find_and_assemble_match) == 0x19B60);
