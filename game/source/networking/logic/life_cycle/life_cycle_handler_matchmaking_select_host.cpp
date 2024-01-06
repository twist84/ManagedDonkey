#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_select_host.hpp"

#include "cseries/cseries_events.hpp"

void c_life_cycle_state_handler_matchmaking_select_host::update()
{
	DECLFUNC(0x00490A20, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_select_host::update_for_state_transition()
{
	return DECLFUNC(0x00490AC0, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*)(this);
}

void c_life_cycle_state_handler_matchmaking_select_host::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00490800, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_select_host::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00490850, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_select_host::get_state_string()
{
	//return DECLFUNC(0x00454790, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*)(this);

	return "matchmaking-select-host";
}

void c_life_cycle_state_handler_matchmaking_select_host::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	DECLFUNC(0x00490930, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*, e_network_session_type)(this, session_type);

	//ASSERT(session_type == _network_session_type_group);
	//generate_event(_event_level_error, "networking:logic:life_cycle:matchmaking_select_host: became host without required session parameters, restarting matchmaking");
	//disband_group_session();
}

void c_life_cycle_state_handler_matchmaking_select_host::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00490950, void, __thiscall, c_life_cycle_state_handler_matchmaking_select_host*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_select_host, &handler_flags, 0, 0xAE0004E80);
}

