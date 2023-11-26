#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_assemble_match : c_life_cycle_state_handler
{
	byte __data[0x100];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_assemble_match) == 0x128);

