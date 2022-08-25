#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_matchmaking_start : c_life_cycle_state_handler
{
	byte __data[0x20];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_start) == 0x48);
