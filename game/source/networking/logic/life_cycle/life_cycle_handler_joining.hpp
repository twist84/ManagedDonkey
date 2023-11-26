#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_joining : c_life_cycle_state_handler
{
	byte __data[0x10];
};
static_assert(sizeof(c_life_cycle_state_handler_joining) == 0x38);

