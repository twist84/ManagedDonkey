#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_end_match_write_stats :
	public c_life_cycle_state_handler
{
public:

//protected:
	uint16 m_flags;
	long __time2C;
};
static_assert(sizeof(c_life_cycle_state_handler_end_match_write_stats) == 0x30);

