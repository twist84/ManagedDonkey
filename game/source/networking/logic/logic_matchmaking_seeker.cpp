#include "networking/logic/logic_matchmaking_seeker.hpp"

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

