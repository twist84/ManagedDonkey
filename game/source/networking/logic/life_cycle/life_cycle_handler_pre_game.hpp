#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

extern bool net_skip_countdown;

struct c_life_cycle_state_handler_pre_game : c_life_cycle_state_handler
{
	dword m_time;
	long m_countdown_timer;
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_pre_game) == 0x38);

