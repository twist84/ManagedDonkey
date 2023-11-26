#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_none : c_life_cycle_state_handler
{
};
static_assert(sizeof(c_life_cycle_state_handler_none) == 0x28);

