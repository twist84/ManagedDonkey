#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_and_assemble_match.hpp"

#include "cseries/cseries_events.hpp"

c_life_cycle_state_handler_matchmaking_find_and_assemble_match::c_life_cycle_state_handler_matchmaking_find_and_assemble_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_matchmaking_seeker(),
	m_current_session_join_attempt()
{
	DECLFUNC(0x00453FD0, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454750, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, get_state_string);

	return "matchmaking-find-and-assemble-match";
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004963D0, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00496490, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, exit, to);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00496740, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, handle_missing_required_session_parameter, session_type);

	//event(_event_warning, "networking:logic:life_cycle:matchmaking_find_and_assemble_match: became host without required session parameters [session type %d], aborting matchmaking",
	//	session_type);
	//
	//if (session_type == _network_session_type_squad)
	//{
	//	squad_session_host_abort_matchmaking();
	//}
	//else if (session_type == _network_session_type_group)
	//{
	//	disband_group_session();
	//}
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00496780, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, initialize, manager);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update()
{
	INVOKE_CLASS_MEMBER(0x00496860, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00496910, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, update_for_state_transition);
}

