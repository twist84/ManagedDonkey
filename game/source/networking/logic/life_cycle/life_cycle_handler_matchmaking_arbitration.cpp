#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

void c_life_cycle_state_handler_matchmaking_arbitration::update()
{
	DECLFUNC(0x00494EF0, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_arbitration::update_for_state_transition()
{
	return DECLFUNC(0x00495450, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*)(this);
}

void c_life_cycle_state_handler_matchmaking_arbitration::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x004946E0, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_arbitration::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x004947E0, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_arbitration::get_state_string()
{
	return DECLFUNC(0x00454730, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*)(this);
}

void c_life_cycle_state_handler_matchmaking_arbitration::handle_missing_required_session_parameter(e_life_cycle_session_type session_type)
{
	DECLFUNC(0x00494960, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*, e_life_cycle_session_type)(this, session_type);
}

void c_life_cycle_state_handler_matchmaking_arbitration::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x004949D0, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_arbitration, &handler_flags, 0, 0xAE0004C80);
}

