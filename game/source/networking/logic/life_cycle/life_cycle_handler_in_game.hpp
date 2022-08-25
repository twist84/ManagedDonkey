#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_in_game : c_life_cycle_state_handler
{
	byte __data[0x18];
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x40);
