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

void c_life_cycle_state_handler_post_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00499D70, c_life_cycle_state_handler_post_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_post_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00499DE0, c_life_cycle_state_handler_post_match, exit, to);
}

void c_life_cycle_state_handler_post_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x0049A3D0, c_life_cycle_state_handler_post_match, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_post_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x0049A3F0, c_life_cycle_state_handler_post_match, initialize, manager);
}

void c_life_cycle_state_handler_post_match::update()
{
	INVOKE_CLASS_MEMBER(0x0049A680, c_life_cycle_state_handler_post_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_post_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x0049A7C0, c_life_cycle_state_handler_post_match, update_for_state_transition);
}

