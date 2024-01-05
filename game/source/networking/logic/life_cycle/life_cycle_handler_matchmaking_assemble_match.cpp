#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_assemble_match.hpp"

void c_life_cycle_state_handler_matchmaking_assemble_match::update()
{
	DECLFUNC(0x00497D80, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_assemble_match::update_for_state_transition()
{
	return DECLFUNC(0x00498130, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*)(this);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00497280, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00497370, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_assemble_match::get_state_string()
{
	return DECLFUNC(0x00454740, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*)(this);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::handle_missing_required_session_parameter(e_life_cycle_session_type session_type)
{
	DECLFUNC(0x00497920, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*, e_life_cycle_session_type)(this, session_type);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00497960, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_assemble_match, &handler_flags, 0xA40000000, 0xA60004C80);
}

