#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_prepare_map.hpp"

c_life_cycle_state_handler_matchmaking_prepare_map::c_life_cycle_state_handler_matchmaking_prepare_map() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_prepare_map_start_time(),
	m_countdown_start_time(),
	m_loading_start_time()
{
	//DECLFUNC(0x00454070, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_prepare_map::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454780, c_life_cycle_state_handler_matchmaking_prepare_map, get_state_string);

	return "matchmaking-prepare-map";
}

void c_life_cycle_state_handler_matchmaking_prepare_map::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004983C0, c_life_cycle_state_handler_matchmaking_prepare_map, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00498450, c_life_cycle_state_handler_matchmaking_prepare_map, exit, to);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00498800, c_life_cycle_state_handler_matchmaking_prepare_map, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00498820, c_life_cycle_state_handler_matchmaking_prepare_map, initialize, manager);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::update()
{
	INVOKE_CLASS_MEMBER(0x00498A20, c_life_cycle_state_handler_matchmaking_prepare_map, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_prepare_map::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00498B30, c_life_cycle_state_handler_matchmaking_prepare_map, update_for_state_transition);
}

