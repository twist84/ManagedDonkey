#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_end_game_write_stats :
	public c_life_cycle_state_handler
{
public:
	
//protected:
	int32 __time28;
	uns16 m_flags;
};
static_assert(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x30);

