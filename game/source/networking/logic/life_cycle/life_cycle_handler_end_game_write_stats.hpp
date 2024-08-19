#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_end_game_write_stats :
	public c_life_cycle_state_handler
{
public:
	
//protected:
	long __time28;
	word_flags m_flags;
};
static_assert(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x30);

