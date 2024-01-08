#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_in_game : c_life_cycle_state_handler
{
	byte_flags m_flags;
	bool m_handled_simulation_abort;
	long m_time;
	dword m_simulation_aborted_timestamp;
	qword m_game_instance;
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x40);

