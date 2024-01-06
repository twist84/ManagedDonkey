#include "networking/logic/life_cycle/life_cycle_handler_post_match.hpp"

#include "cseries/cseries_events.hpp"

void c_life_cycle_state_handler_post_match::update()
{
	DECLFUNC(0x0049A680, void, __thiscall, c_life_cycle_state_handler_post_match*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_post_match::update_for_state_transition()
{
	return DECLFUNC(0x0049A7C0, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_post_match*)(this);
}

void c_life_cycle_state_handler_post_match::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00499D70, void, __thiscall, c_life_cycle_state_handler_post_match*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_post_match::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00499DE0, void, __thiscall, c_life_cycle_state_handler_post_match*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_post_match::get_state_string()
{
	//return DECLFUNC(0x004547C0, char const*, __thiscall, c_life_cycle_state_handler_post_match*)(this);

	return "post-match";
}

void c_life_cycle_state_handler_post_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	DECLFUNC(0x0049A3D0, void, __thiscall, c_life_cycle_state_handler_post_match*, e_network_session_type)(this, session_type);

	//ASSERT(session_type == _network_session_type_group);
	//generate_event(_event_level_error, "networking:logic:life_cycle:post_match: became host without required session parameters, restarting matchmaking");
	//disband_group_session();
}

void c_life_cycle_state_handler_post_match::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x0049A3F0, void, __thiscall, c_life_cycle_state_handler_post_match*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_recreates_group_bit, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_post_match, &handler_flags, 0, 0x240000000);
}

