#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

c_life_cycle_state_handler_matchmaking_arbitration::c_life_cycle_state_handler_matchmaking_arbitration() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_arbitration_start_time(),
	m_arbitration_waiting_for_completion_start_time(),
	m_registration_result()
{
	//DECLFUNC(0x00453F90, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_arbitration::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454730, c_life_cycle_state_handler_matchmaking_arbitration, get_state_string);

	return "matchmaking-arbitration";
}

void c_life_cycle_state_handler_matchmaking_arbitration::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004946E0, c_life_cycle_state_handler_matchmaking_arbitration, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_arbitration::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x004947E0, c_life_cycle_state_handler_matchmaking_arbitration, exit, to);
}

void c_life_cycle_state_handler_matchmaking_arbitration::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00494960, c_life_cycle_state_handler_matchmaking_arbitration, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_arbitration::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x004949D0, c_life_cycle_state_handler_matchmaking_arbitration, initialize, manager);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update()
{
	INVOKE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_arbitration::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x004952F0, c_life_cycle_state_handler_matchmaking_arbitration, update_for_state_transition);
}

