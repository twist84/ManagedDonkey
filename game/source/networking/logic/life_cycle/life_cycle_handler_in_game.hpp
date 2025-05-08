#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_life_cycle_state_handler_in_game_flags, uns8, k_life_cycle_state_handler_in_game_flags_count> c_life_cycle_state_handler_in_game_flags;
typedef uns8 c_life_cycle_state_handler_in_game_flags;

struct c_life_cycle_state_handler_in_game :
	public c_life_cycle_state_handler
{
public:
	
//protected:
	c_life_cycle_state_handler_in_game_flags m_flags;
	bool m_handled_simulation_abort;
	int32 m_time_of_handled_campaign_quit;
	uns32 m_simulation_aborted_timestamp;
	uns64 m_game_instance;
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x40);
static_assert(0x28 == OFFSETOF(c_life_cycle_state_handler_in_game, m_flags));
static_assert(0x29 == OFFSETOF(c_life_cycle_state_handler_in_game, m_handled_simulation_abort));
static_assert(0x2C == OFFSETOF(c_life_cycle_state_handler_in_game, m_time_of_handled_campaign_quit));
static_assert(0x30 == OFFSETOF(c_life_cycle_state_handler_in_game, m_simulation_aborted_timestamp));
static_assert(0x38 == OFFSETOF(c_life_cycle_state_handler_in_game, m_game_instance));

