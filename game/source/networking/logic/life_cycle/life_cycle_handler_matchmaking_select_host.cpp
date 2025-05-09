#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_select_host.hpp"

c_life_cycle_state_handler_matchmaking_select_host::c_life_cycle_state_handler_matchmaking_select_host() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_select_host_start_time()
{
	//DECLFUNC(0x00454090, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_select_host::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454790, c_life_cycle_state_handler_matchmaking_select_host, get_state_string);

	return "matchmaking-select-host";
}

void c_life_cycle_state_handler_matchmaking_select_host::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00490800, c_life_cycle_state_handler_matchmaking_select_host, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_select_host::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00490850, c_life_cycle_state_handler_matchmaking_select_host, exit, to);
}

void c_life_cycle_state_handler_matchmaking_select_host::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00490930, c_life_cycle_state_handler_matchmaking_select_host, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_select_host::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00490950, c_life_cycle_state_handler_matchmaking_select_host, initialize, manager);
}

void c_life_cycle_state_handler_matchmaking_select_host::update()
{
	INVOKE_CLASS_MEMBER(0x00490A20, c_life_cycle_state_handler_matchmaking_select_host, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_select_host::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00490AC0, c_life_cycle_state_handler_matchmaking_select_host, update_for_state_transition);
}

