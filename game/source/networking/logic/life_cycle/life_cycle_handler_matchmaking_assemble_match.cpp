#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_assemble_match.hpp"

c_life_cycle_state_handler_matchmaking_assemble_match::c_life_cycle_state_handler_matchmaking_assemble_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_gather_start_time(),
	m_ready_to_start_time(),
	m_advertised_session_properties(),
	m_qos_listener_stats_query_time(),
	m_last_qos_listener_data_replies_sent(),
	m_qos_listener_data()
{
	//DECLFUNC(0x00453FB0, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_assemble_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454740, c_life_cycle_state_handler_matchmaking_assemble_match, get_state_string);

	return "matchmaking-assemble-match";
}

void c_life_cycle_state_handler_matchmaking_assemble_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00497280, c_life_cycle_state_handler_matchmaking_assemble_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00497370, c_life_cycle_state_handler_matchmaking_assemble_match, exit, to);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00497920, c_life_cycle_state_handler_matchmaking_assemble_match, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00497960, c_life_cycle_state_handler_matchmaking_assemble_match, initialize, manager);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::update()
{
	INVOKE_CLASS_MEMBER(0x00497D80, c_life_cycle_state_handler_matchmaking_assemble_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_assemble_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00498130, c_life_cycle_state_handler_matchmaking_assemble_match, update_for_state_transition);
}

