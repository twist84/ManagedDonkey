#include "networking/logic/logic_matchmaking_seeker.hpp"

#include "memory/module.hpp"
#include "networking/network_time.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x004D4560, c_matchmaking_seeker, search_in_progress);

c_matchmaking_seeker::c_matchmaking_seeker() :
	m_flags(0),
	m_current_session_search_index(),
	m_search_stage(),
	m_search_stage_round(),
	m_total_sessions_found(),
	m_total_session_search_failures(),
	m_total_session_searches(),
	m_current_sessions_contacted_count(),
	m_total_session_contacted_count(),
	m_search_start_time(),
	m_current_search_completed_time(),
	m_session_searches(),
	m_session_properties(),
	m_search_preference(),
	m_ranked(),
	m_game_mode(),
	m_game_type()
{
}

bool c_matchmaking_seeker::get_session_to_join(s_suitable_matchmaking_session* session_out)
{
	return INVOKE_CLASS_MEMBER(0x004D41F0, c_matchmaking_seeker, get_session_to_join, session_out);
}

bool c_matchmaking_seeker::search_in_progress()
{
	//return INVOKE_CLASS_MEMBER(0x004D4560, c_matchmaking_seeker, search_in_progress);

	m_search_stage = _matchmaking_search_stage_any;
	m_session_searches[m_current_session_search_index].status.start_timestamp = network_time_get();

	m_flags.set(_matchmaking_seeker_online_search_in_progress_bit, false);
	m_flags.set(_matchmaking_seeker_online_search_results_being_tracked_bit, false);
	m_flags.set(_matchmaking_seeker_search_failed_bit, false);

	bool result;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_matchmaking_seeker, search_in_progress);
	return result;
}

