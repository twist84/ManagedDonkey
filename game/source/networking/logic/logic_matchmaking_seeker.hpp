#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"

//typedef c_flags<e_matchmaking_seeker_flags, uns8, k_matchmaking_seeker_search_flags_count> c_matchmaking_seeker_flags;
typedef uns8 c_matchmaking_seeker_flags;

struct __declspec(align(8)) c_matchmaking_seeker
{
	c_matchmaking_seeker_flags m_flags;
	int32 m_current_session_search_index;
	int32 m_search_stage;
	int32 m_search_stage_round;
	int32 m_total_sessions_found;
	int32 m_total_session_search_failures;
	int32 m_total_session_searches;
	int32 m_current_sessions_contacted_count;
	int32 m_total_session_contacted_count;
	uns32 m_search_start_time;
	uns32 m_current_search_completed_time;
	c_static_array<s_matchmaking_session_search, 16> m_session_searches;
	s_matchmaking_session_properties m_session_properties;
	e_matchmaking_search_preference m_search_preference;
	bool m_ranked;
	int32 m_game_mode;
	int32 m_game_type;
};
static_assert(sizeof(c_matchmaking_seeker) == 0x3508);
static_assert(0x0000 == OFFSETOF(c_matchmaking_seeker, m_flags));
static_assert(0x0004 == OFFSETOF(c_matchmaking_seeker, m_current_session_search_index));
static_assert(0x0008 == OFFSETOF(c_matchmaking_seeker, m_search_stage));
static_assert(0x000C == OFFSETOF(c_matchmaking_seeker, m_search_stage_round));
static_assert(0x0010 == OFFSETOF(c_matchmaking_seeker, m_total_sessions_found));
static_assert(0x0014 == OFFSETOF(c_matchmaking_seeker, m_total_session_search_failures));
static_assert(0x0018 == OFFSETOF(c_matchmaking_seeker, m_total_session_searches));
static_assert(0x001C == OFFSETOF(c_matchmaking_seeker, m_current_sessions_contacted_count));
static_assert(0x0020 == OFFSETOF(c_matchmaking_seeker, m_total_session_contacted_count));
static_assert(0x0024 == OFFSETOF(c_matchmaking_seeker, m_search_start_time));
static_assert(0x0028 == OFFSETOF(c_matchmaking_seeker, m_current_search_completed_time));
static_assert(0x0030 == OFFSETOF(c_matchmaking_seeker, m_session_searches));
static_assert(0x3430 == OFFSETOF(c_matchmaking_seeker, m_session_properties));
static_assert(0x34F4 == OFFSETOF(c_matchmaking_seeker, m_search_preference));
static_assert(0x34F8 == OFFSETOF(c_matchmaking_seeker, m_ranked));
static_assert(0x34FC == OFFSETOF(c_matchmaking_seeker, m_game_mode));
static_assert(0x3500 == OFFSETOF(c_matchmaking_seeker, m_game_type));


