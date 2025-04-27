#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"

struct c_matchmaking_seeker
{
	uint8 m_flags;
	uint8 __pad1[0x3];
	int32 m_current_session_search_index;
	int32 m_search_stage;
	int32 m_search_stage_round;
	int32 m_total_sessions_found;
	int32 m_total_session_search_failures;
	int32 m_total_session_searches;
	int32 m_current_sessions_contacted_count;
	int32 m_total_session_contacted_count;
	uint32 m_search_start_time;
	uint32 m_current_search_completed_time;
	uint8 __data28[0x4];
	c_static_array<s_matchmaking_session_search, 16> m_session_searches;
	s_matchmaking_session_properties m_session_properties;
	int32 m_current_search_preference;
	uint8 __data34F8[0x10];
};
static_assert(sizeof(c_matchmaking_seeker) == 0x3508);

