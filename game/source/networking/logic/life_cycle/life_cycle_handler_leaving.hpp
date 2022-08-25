#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_leaving : c_life_cycle_state_handler
{
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_leaving) == 0x30);
