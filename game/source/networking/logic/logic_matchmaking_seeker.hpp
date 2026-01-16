#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"

struct s_suitable_matchmaking_session;

enum e_matchmaking_seeker_flags
{
	_matchmaking_seeker_search_in_progress_bit = 0,
	_matchmaking_seeker_expect_local_machine_in_search_results_bit,
	_matchmaking_seeker_online_search_in_progress_bit,
	_matchmaking_seeker_online_search_failed_bit,
	_matchmaking_seeker_online_search_results_being_tracked_bit,
	_matchmaking_seeker_online_search_blocked_bit,
	_matchmaking_seeker_search_failed_bit,

	k_matchmaking_seeker_search_flags_count,
};
typedef c_flags<e_matchmaking_seeker_flags, uns8, k_matchmaking_seeker_search_flags_count> c_matchmaking_seeker_flags;

enum e_matchmaking_search_stage
{
	_matchmaking_search_stage_strict_skill = 0,
	_matchmaking_search_stage_skill,
	_matchmaking_search_stage_any,
	_matchmaking_search_stage_desparation,

	k_matchmaking_search_stage_count,
	k_matchmaking_search_stage_bits = 2,
};

struct __declspec(align(8)) c_matchmaking_seeker
{
	c_matchmaking_seeker();

	bool get_session_to_join(s_suitable_matchmaking_session* session_out);
	bool search_in_progress();

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
COMPILE_ASSERT(sizeof(c_matchmaking_seeker) == 0x3508);
COMPILE_ASSERT(0x0000 == OFFSETOF(c_matchmaking_seeker, m_flags));
COMPILE_ASSERT(0x0004 == OFFSETOF(c_matchmaking_seeker, m_current_session_search_index));
COMPILE_ASSERT(0x0008 == OFFSETOF(c_matchmaking_seeker, m_search_stage));
COMPILE_ASSERT(0x000C == OFFSETOF(c_matchmaking_seeker, m_search_stage_round));
COMPILE_ASSERT(0x0010 == OFFSETOF(c_matchmaking_seeker, m_total_sessions_found));
COMPILE_ASSERT(0x0014 == OFFSETOF(c_matchmaking_seeker, m_total_session_search_failures));
COMPILE_ASSERT(0x0018 == OFFSETOF(c_matchmaking_seeker, m_total_session_searches));
COMPILE_ASSERT(0x001C == OFFSETOF(c_matchmaking_seeker, m_current_sessions_contacted_count));
COMPILE_ASSERT(0x0020 == OFFSETOF(c_matchmaking_seeker, m_total_session_contacted_count));
COMPILE_ASSERT(0x0024 == OFFSETOF(c_matchmaking_seeker, m_search_start_time));
COMPILE_ASSERT(0x0028 == OFFSETOF(c_matchmaking_seeker, m_current_search_completed_time));
COMPILE_ASSERT(0x0030 == OFFSETOF(c_matchmaking_seeker, m_session_searches));
COMPILE_ASSERT(0x3430 == OFFSETOF(c_matchmaking_seeker, m_session_properties));
COMPILE_ASSERT(0x34F4 == OFFSETOF(c_matchmaking_seeker, m_search_preference));
COMPILE_ASSERT(0x34F8 == OFFSETOF(c_matchmaking_seeker, m_ranked));
COMPILE_ASSERT(0x34FC == OFFSETOF(c_matchmaking_seeker, m_game_mode));
COMPILE_ASSERT(0x3500 == OFFSETOF(c_matchmaking_seeker, m_game_type));


