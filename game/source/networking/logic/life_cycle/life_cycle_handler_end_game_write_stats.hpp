#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_end_game_write_stats : c_life_cycle_state_handler
{
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x30);
