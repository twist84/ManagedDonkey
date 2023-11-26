#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_select_host : c_life_cycle_state_handler
{
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_select_host) == 0x30);

