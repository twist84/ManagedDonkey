#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_start.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role);

bool net_matchmaking_force_gather = true;
bool net_matchmaking_force_search = false;

c_life_cycle_state_handler_matchmaking_start::c_life_cycle_state_handler_matchmaking_start() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_matchmaking_find_match_role(),
	m_entry_data(),
	m_waiting_to_start_timer(),
	m_last_warning_toast_time()
{
	//DECLFUNC(0x004540B0, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_start::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004547A0, c_life_cycle_state_handler_matchmaking_start, get_state_string);

	return "matchmaking-start";
}

//.text:00493900 ; private: void c_life_cycle_state_handler_matchmaking_start::create_group_session()

e_life_cycle_matchmaking_find_match_role __thiscall c_life_cycle_state_handler_matchmaking_start::determine_matchmaking_find_match_role(bool force_gather)
{
	//return INVOKE_CLASS_MEMBER(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, force_gather);

	e_life_cycle_matchmaking_find_match_role result = _life_cycle_matchmaking_find_match_role_search;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, force_gather);

	if (net_matchmaking_force_gather) // network_globals.matchmaking_force_gather
	{
		event(_event_message, "networking:logic:life-cycle:matchmaking_start: FORCE GATHER");
		result = _life_cycle_matchmaking_find_match_role_search_gather;
	}
	else if (net_matchmaking_force_search) // network_globals.matchmaking_force_search
	{
		event(_event_message, "networking:logic:life-cycle:matchmaking_start: FORCE SEARCH");
		result = _life_cycle_matchmaking_find_match_role_search;
	}

	return result;
}

void c_life_cycle_state_handler_matchmaking_start::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00493A40, c_life_cycle_state_handler_matchmaking_start, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_matchmaking_start::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00493A50, c_life_cycle_state_handler_matchmaking_start, enter, from, entry_data_size, entry_data);
}

//.text:00493C40 ; 

void c_life_cycle_state_handler_matchmaking_start::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00493C60, c_life_cycle_state_handler_matchmaking_start, exit, to);
}

//.text:00493CC0 ; private: void c_life_cycle_state_handler_matchmaking_start::finish_group_session_creation()
//.text:00493E60 ; 

void c_life_cycle_state_handler_matchmaking_start::get_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE_CLASS_MEMBER(0x00493E70, c_life_cycle_state_handler_matchmaking_start, get_progress, progress_out);
}

void c_life_cycle_state_handler_matchmaking_start::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00494010, c_life_cycle_state_handler_matchmaking_start, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_start::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00494030, c_life_cycle_state_handler_matchmaking_start, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_start, &parent_flags, 0x240000000ULL, 0x0ULL);
	m_last_warning_toast_time = -1;
}

//.text:00494070 ; 
//.text:00494080 ; 
//.text:004940A0 ; 
//.text:004940B0 ; 
//.text:004940D0 ; 
//.text:004940F0 ; private: bool c_life_cycle_state_handler_matchmaking_start::squad_allowed_to_start_matchmaking(bool)
//.text:004941B0 ; 

void c_life_cycle_state_handler_matchmaking_start::update()
{
	INVOKE_CLASS_MEMBER(0x004941D0, c_life_cycle_state_handler_matchmaking_start, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_start::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00494440, c_life_cycle_state_handler_matchmaking_start, update_for_state_transition);
}

//.text:00494530 ; private: void c_life_cycle_state_handler_matchmaking_start::update_group_creation()
//.text:00494690 ; private: void c_life_cycle_state_handler_matchmaking_start::update_squad_ready()

