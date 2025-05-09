#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match.hpp"

#include "cseries/cseries_events.hpp"

c_life_cycle_state_handler_matchmaking_find_match::c_life_cycle_state_handler_matchmaking_find_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_matchmaking_seeker(),
	m_current_session_join_attempt(),
	m_desparation_start_time()
{
	//DECLFUNC(0x00454010, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_find_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454760, c_life_cycle_state_handler_matchmaking_find_match, get_state_string);

	return "matchmaking-find-match";
}

void c_life_cycle_state_handler_matchmaking_find_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00495B60, c_life_cycle_state_handler_matchmaking_find_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00495BB0, c_life_cycle_state_handler_matchmaking_find_match, exit, to);
}

void c_life_cycle_state_handler_matchmaking_find_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00495D30, c_life_cycle_state_handler_matchmaking_find_match, handle_missing_required_session_parameter, session_type);

	//ASSERT(session_type == _network_session_type_squad);
	//event(_event_warning, "networking:logic:life_cycle:matchmaking_find_match: became host without required session parameters, aborting matchmaking");
	//squad_session_host_abort_matchmaking();
}

void c_life_cycle_state_handler_matchmaking_find_match::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00495D50, c_life_cycle_state_handler_matchmaking_find_match, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_find_match, &parent_flags, 0xA40000000ULL, 0ULL);
}

void c_life_cycle_state_handler_matchmaking_find_match::update()
{
	INVOKE_CLASS_MEMBER(0x00495E20, c_life_cycle_state_handler_matchmaking_find_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00495EA0, c_life_cycle_state_handler_matchmaking_find_match, update_for_state_transition);
}

