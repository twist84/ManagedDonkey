#include "networking/logic/life_cycle/life_cycle_handler_post_match.hpp"

c_life_cycle_state_handler_post_match::c_life_cycle_state_handler_post_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_countdown_seconds_timer(),
	m_rematch_squad_host_update_timer(),
	m_ready_for_next_match_timer(),
	m_waiting_for_stats_replication_timer(),
	m_pre_rematch_squad_peer_count()
{
	//DECLFUNC(0x004540E0, void, __thiscall, c_life_cycle_state_handler_post_match*)(this);
}

char const* c_life_cycle_state_handler_post_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004547C0, c_life_cycle_state_handler_post_match, get_state_string);

	return "post-match";
}

void c_life_cycle_state_handler_post_match::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00499D60, c_life_cycle_state_handler_post_match, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_post_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00499D70, c_life_cycle_state_handler_post_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_post_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00499DE0, c_life_cycle_state_handler_post_match, exit, to);
}

//.text:00499E10 ; 
//.text:00499E20 ; 
//.text:00499EB0 ; private: int32 c_life_cycle_state_handler_post_match::get_peer_rematch_team_index(int32)
//.text:00499F60 ; private: int32 c_life_cycle_state_handler_post_match::get_player_rematch_team_index(int32)

void c_life_cycle_state_handler_post_match::get_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE_CLASS_MEMBER(0x00499FB0, c_life_cycle_state_handler_post_match, get_progress, progress_out);
}

//.text:0049A340 ; private: void c_life_cycle_state_handler_post_match::get_state_from_progress(s_replicated_life_cycle_progress_post_match*)

void c_life_cycle_state_handler_post_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x0049A3D0, c_life_cycle_state_handler_post_match, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_post_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x0049A3F0, c_life_cycle_state_handler_post_match, initialize, manager);
}

//.text:0049A420 ; 
//.text:0049A430 ; 
//.text:0049A450 ; 
//.text:0049A460 ; 

bool c_life_cycle_state_handler_post_match::rematch_join_in_progress() const
{
	//return INVOKE_CLASS_MEMBER(0x0049A480, c_life_cycle_state_handler_post_match, rematch_join_in_progress);

	return m_flags.test(_life_cycle_post_match_rematch_joining_bit);
}

//.text:0049A490 ; 
//.text:0049A4C0 ; 
//.text:0049A4E0 ; 
//.text:0049A500 ; private: void c_life_cycle_state_handler_post_match::squad_host_notify_rematch_failure(char const*)
//.text:0049A540 ; private: bool c_life_cycle_state_handler_post_match::squad_host_ready_for_matchmaking_start()
//.text:0049A660 ; 

void c_life_cycle_state_handler_post_match::update()
{
	INVOKE_CLASS_MEMBER(0x0049A680, c_life_cycle_state_handler_post_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_post_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x0049A7C0, c_life_cycle_state_handler_post_match, update_for_state_transition);
}

//.text:0049A890 ; private: void c_life_cycle_state_handler_post_match::update_group_host()
//.text:0049AAC0 ; private: void c_life_cycle_state_handler_post_match::update_leader_idle_controllers()
//.text:0049AB60 ; private: void c_life_cycle_state_handler_post_match::update_progress(s_replicated_life_cycle_progress_post_match const*)
//.text:0049ABC0 ; private: void c_life_cycle_state_handler_post_match::update_squad_host()

