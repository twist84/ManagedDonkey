#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_in_game :
	public c_life_cycle_state_handler
{
public:
	
//protected:
	uns8 m_flags;
	bool m_handled_simulation_abort;
	int32 m_time;
	uns32 m_simulation_aborted_timestamp;
	uns64 m_game_instance;
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x40);

