#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_and_assemble_match.hpp"

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update()
{
	DECLFUNC(0x00496860, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_and_assemble_match::update_for_state_transition()
{
	return DECLFUNC(0x00496910, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*)(this);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x004963D0, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00496490, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_find_and_assemble_match::get_state_string()
{
	//return DECLFUNC(0x00454750, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*)(this);

	return "matchmaking-find-and-assemble-match";
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	DECLFUNC(0x00496740, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*, e_network_session_type)(this, session_type);

	//generate_event(_event_level_warning, "networking:logic:life_cycle:matchmaking_find_and_assemble_match: became host without required session parameters [session type %d], aborting matchmaking", session_type);
	//switch (session_type)
	//{
	//case _network_session_type_squad:
	//	squad_session_host_abort_matchmaking();
	//	break;
	//case _network_session_type_group:
	//	disband_group_session();
	//	break;
	//}
}

void c_life_cycle_state_handler_matchmaking_find_and_assemble_match::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00496780, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_and_assemble_match*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_find_and_assemble_match, &handler_flags, 0xA40000000, 0xA60004C80);
}

