#include "networking/logic/life_cycle/life_cycle_handler_leaving.hpp"

#include "cseries/cseries_events.hpp"
#include "main/main_game.hpp"
#include "networking/session/network_session.hpp"

void c_life_cycle_state_handler_leaving::update()
{
	DECLFUNC(0x00493500, void, __thiscall, c_life_cycle_state_handler_leaving*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_leaving::update_for_state_transition()
{
	return DECLFUNC(0x00493610, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_leaving*)(this);
}

void c_life_cycle_state_handler_leaving::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x004933E0, void, __thiscall, c_life_cycle_state_handler_leaving*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_leaving::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x00493460, void, __thiscall, c_life_cycle_state_handler_leaving*, c_life_cycle_state_handler*)(this, handler);

	c_life_cycle_state_handler::exit(handler);

	if (!main_game_change_in_progress())
		main_menu_launch();
}

char const* c_life_cycle_state_handler_leaving::get_state_string()
{
	//return DECLFUNC(0x00454720, char const*, __thiscall, c_life_cycle_state_handler_leaving*)(this);

	return "leaving";
}

void c_life_cycle_state_handler_leaving::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00493490, void, __thiscall, c_life_cycle_state_handler_leaving*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_leaving, &handler_flags, 0, 0);
}

