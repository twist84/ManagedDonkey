#pragma once

#include "networking/logic/life_cycle/life_cycle_manager.hpp"

struct c_life_cycle_state_handler_matchmaking_start : c_life_cycle_state_handler
{
	byte_flags m_flags;
	long m_find_match_role;
	long m_start_mode;
	long m_initial_desperation_sessions_found_count;
	long m_initial_session_search_count;
	dword __time3C;
	dword __time40;
	byte __data[0x4];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_start) == 0x48);
