#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match_client.hpp"

c_life_cycle_state_handler_matchmaking_find_match_client::c_life_cycle_state_handler_matchmaking_find_match_client() :
	c_life_cycle_state_handler(),
	m_flags(0)
{
	//DECLFUNC(0x00454050, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_find_match_client::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454770, c_life_cycle_state_handler_matchmaking_find_match_client, get_state_string);

	return "matchmaking-find-match-client";
}

void c_life_cycle_state_handler_matchmaking_find_match_client::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00495A00, c_life_cycle_state_handler_matchmaking_find_match_client, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_matchmaking_find_match_client::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00495A10, c_life_cycle_state_handler_matchmaking_find_match_client, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00495A40, c_life_cycle_state_handler_matchmaking_find_match_client, exit, to);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::get_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE_CLASS_MEMBER(0x00495A50, c_life_cycle_state_handler_matchmaking_find_match_client, get_progress, progress_out);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00495A60, c_life_cycle_state_handler_matchmaking_find_match_client, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00495A80, c_life_cycle_state_handler_matchmaking_find_match_client, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_find_match_client, &parent_flags, 0xA40000000ULL, 0ULL);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::update()
{
	INVOKE_CLASS_MEMBER(0x00495AB0, c_life_cycle_state_handler_matchmaking_find_match_client, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_match_client::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00495AC0, c_life_cycle_state_handler_matchmaking_find_match_client, update_for_state_transition);
}

