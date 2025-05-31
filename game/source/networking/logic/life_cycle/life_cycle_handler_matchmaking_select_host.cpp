#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_select_host.hpp"

c_life_cycle_state_handler_matchmaking_select_host::c_life_cycle_state_handler_matchmaking_select_host() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_select_host_start_time()
{
	//DECLFUNC(0x00454090, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*)(this);
}

const char* c_life_cycle_state_handler_matchmaking_select_host::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454790, c_life_cycle_state_handler_matchmaking_select_host, get_state_string);

	return "matchmaking-select-host";
}

void c_life_cycle_state_handler_matchmaking_select_host::dispose()
{
	//INVOKE_CLASS_MEMBER(0x004907F0, c_life_cycle_state_handler_matchmaking_select_host, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_matchmaking_select_host::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00490800, c_life_cycle_state_handler_matchmaking_select_host, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_select_host::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00490850, c_life_cycle_state_handler_matchmaking_select_host, exit, to);
}

//.text:00490900 ; 
//.text:00490910 ; 

void c_life_cycle_state_handler_matchmaking_select_host::get_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE_CLASS_MEMBER(0x00490920, c_life_cycle_state_handler_matchmaking_select_host, get_progress, progress_out);
}

void c_life_cycle_state_handler_matchmaking_select_host::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00490930, c_life_cycle_state_handler_matchmaking_select_host, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_select_host::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00490950, c_life_cycle_state_handler_matchmaking_select_host, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_requires_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	parent_flags.set(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_select_host, &parent_flags, 0x0ULL, 0xAE0004E80ULL);
}

//.text:00490980 ; 
//.text:00490990 ; 
//.text:004909B0 ; 
//.text:004909C0 ; 
//.text:004909E0 ; 
//.text:00490A00 ; 

void c_life_cycle_state_handler_matchmaking_select_host::update()
{
	INVOKE_CLASS_MEMBER(0x00490A20, c_life_cycle_state_handler_matchmaking_select_host, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_select_host::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00490AC0, c_life_cycle_state_handler_matchmaking_select_host, update_for_state_transition);
}

//.text:00490BB0 ; private: void c_life_cycle_state_handler_matchmaking_select_host::update_progress()

