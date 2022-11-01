#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/network_arbitration.hpp"

struct c_life_cycle_state_handler_matchmaking_arbitration : c_life_cycle_state_handler
{
	byte __data[0x10];
	s_arbitration_registration_result m_registration_result;
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_arbitration) == 0x1C0);

