#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/logic_matchmaking_seeker.hpp"
#include "networking/logic/logic_session_tracker.hpp"

struct c_life_cycle_state_handler_matchmaking_find_match :
	public c_life_cycle_state_handler
{
public:
	
//protected:
	uns8 m_flags;
	byte __data29[0x3];
	byte __data2C[0x4];
	c_matchmaking_seeker m_seeker;
	s_network_session_tracker_session_data m_session;
	byte __data19A5C[0xC];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_find_match) == 0x19A68);

