#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_leaving : c_life_cycle_state_handler
{
	byte_flags m_flags;
	bool m_disconnected;
	byte __data[0x6];
};
static_assert(sizeof(c_life_cycle_state_handler_leaving) == 0x30);

