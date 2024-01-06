#include "networking/logic/life_cycle/life_cycle_handler_start_game.hpp"

void c_life_cycle_state_handler_start_game::update()
{
	DECLFUNC(0x00490FB0, void, __thiscall, c_life_cycle_state_handler_start_game*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_start_game::update_for_state_transition()
{
	return DECLFUNC(0x00491010, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_start_game*)(this);
}

void c_life_cycle_state_handler_start_game::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00490C30, void, __thiscall, c_life_cycle_state_handler_start_game*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_start_game::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00490CA0, void, __thiscall, c_life_cycle_state_handler_start_game*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_start_game::get_state_string()
{
	//return DECLFUNC(0x004547E0, char const*, __thiscall, c_life_cycle_state_handler_start_game*)(this);

	return "start-game";
}

void c_life_cycle_state_handler_start_game::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00490CB0, void, __thiscall, c_life_cycle_state_handler_start_game*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_start_game, &handler_flags, 0, 0);
}

