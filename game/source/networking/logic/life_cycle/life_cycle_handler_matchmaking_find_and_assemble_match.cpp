#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_and_assemble_match.hpp"

#include "cseries/cseries_events.hpp"

c_life_cycle_state_handler_matchmaking_find_and_assemble_match::c_life_cycle_state_handler_matchmaking_find_and_assemble_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_matchmaking_seeker(),
	m_current_session_join_attempt(),
	m_gather_start_time(),
	m_desparation_start_time(),
	m_force_gather_in_matchmaking_start(),
	m_advertised_session_properties(),
	m_enter_find_and_assemble_data(),
	m_qos_listener_data()
{
	//DECLFUNC(0x00453FD0, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*)(this);

	zero_array(pad);
}

char const* c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454750, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, get_state_string);

	return "matchmaking-find-and-assemble-match";
}

//.text:004963C0 ; public: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::dispose()

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004963D0, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00496490, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, exit, to);
}

//.text:00496530 ; 
//.text:00496540 ; 
//.text:00496580 ; public: bool c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_gather_party_properties(s_matchmaking_gather_party_properties*)
//.text:00496670 ; public: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_progress(s_life_cycle_matchmaking_progress*)
//.text:00496680 ; public: bool c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_search_party_properties(s_matchmaking_search_party_properties*)

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

//.text:004967B0 ; 
//.text:004967C0 ; 
//.text:004967E0 ; 
//.text:004967F0 ; 

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::notify_qos_reply_block_set()
{
	INVOKE_CLASS_MEMBER(0x00496810, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, notify_qos_reply_block_set);
}

//.text:00496820 ; 
//.text:00496840 ; 

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update()
{
	INVOKE_CLASS_MEMBER(0x00496860, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00496910, c_life_cycle_state_handler_matchmaking_find_and_assemble_match, update_for_state_transition);
}

//.text:00496A00 ; private: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_gathering()
//.text:00496BA0 ; private: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_joining()
//.text:00496F80 ; private: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_leaving()
//.text:00497040 ; private: void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_progress()

