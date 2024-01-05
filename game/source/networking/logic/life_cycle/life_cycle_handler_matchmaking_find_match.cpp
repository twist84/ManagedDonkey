#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match.hpp"

void c_life_cycle_state_handler_matchmaking_find_match::update()
{
	DECLFUNC(0x00495E20, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_find_match::update_for_state_transition()
{
	return DECLFUNC(0x00495EA0, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*)(this);
}

void c_life_cycle_state_handler_matchmaking_find_match::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00495B60, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_find_match::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00495BB0, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_find_match::get_state_string()
{
	return DECLFUNC(0x00454760, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*)(this);
}

void c_life_cycle_state_handler_matchmaking_find_match::handle_missing_required_session_parameter(e_life_cycle_session_type session_type)
{
	DECLFUNC(0x00495D30, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*, e_life_cycle_session_type)(this, session_type);
}

void c_life_cycle_state_handler_matchmaking_find_match::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00495D50, void, __thiscall, c_life_cycle_state_handler_matchmaking_find_match*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_find_match, &handler_flags, 0xA40000000, 0);
}

