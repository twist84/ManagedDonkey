#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"

struct c_matchmaking_seeker
{
	byte_flags m_flags;
	byte __pad1[0x3];
	long m_current_session_search_index;
	long m_session_search_stage;
	long __unknownC;
	long __unknown10;
	long m_total_failure_count;
	long m_total_session_search_count;
	long m_total_sessions_contacted_count1;
	long m_total_sessions_contacted_count2;
	long m_initial_search_start_time;
	byte __data28[0x8];
	s_matchmaking_session_search m_session_search_data[16];
	s_matchmaking_session_properties m_session_properties;
	long m_current_search_preference;
	byte __data34F8[0x10];
};
static_assert(sizeof(c_matchmaking_seeker) == 0x3508);

