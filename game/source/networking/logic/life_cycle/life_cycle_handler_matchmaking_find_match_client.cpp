#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match_client.hpp"

#include "cseries/cseries_events.hpp"

void c_life_cycle_state_handler_matchmaking_find_match_client::update()
{
	DECLFUNC(0x00495AB0, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_match_client::update_for_state_transition()
{
	return DECLFUNC(0x00495AC0, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*)(this);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00495A10, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_match_client::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00495A40, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_find_match_client::get_state_string()
{
	//return DECLFUNC(0x00454770, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*)(this);

	return "matchmaking-find-match-client";
}

void c_life_cycle_state_handler_matchmaking_find_match_client::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	DECLFUNC(0x00495A60, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*, e_network_session_type)(this, session_type);

	//ASSERT(session_type == _network_session_type_squad);
	//generate_event(_event_level_warning, "networking:logic:life_cycle:matchmaking_find_match_client: became host without required session parameters, aborting matchmaking");
	//squad_session_host_abort_matchmaking();
}

void c_life_cycle_state_handler_matchmaking_find_match_client::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00495A80, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match_client*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_find_match_client, &handler_flags, 0xA40000000, 0);
}

