#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_prepare_map.hpp"

#include "cseries/cseries_events.hpp"

void c_life_cycle_state_handler_matchmaking_prepare_map::update()
{
	DECLFUNC(0x00498A20, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_prepare_map::update_for_state_transition()
{
	return DECLFUNC(0x00498C70, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*)(this);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x004983C0, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00498450, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_prepare_map::get_state_string()
{
	//return DECLFUNC(0x00454780, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*)(this);

	return "matchmaking-prepare-map";
}

void c_life_cycle_state_handler_matchmaking_prepare_map::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	DECLFUNC(0x00498800, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*, e_network_session_type)(this, session_type);

	//ASSERT(session_type == _network_session_type_group);
	//generate_event(_event_level_error, "networking:logic:life_cycle:matchmaking_prepare_map: became host without required session parameters, restarting matchmaking");
	//disband_group_session();
}

void c_life_cycle_state_handler_matchmaking_prepare_map::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00498820, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_prepare_map, &handler_flags, 0, 0xAE0004E80);
}

