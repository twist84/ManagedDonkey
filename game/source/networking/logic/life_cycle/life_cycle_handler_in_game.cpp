#include "networking/logic/life_cycle/life_cycle_handler_in_game.hpp"

void c_life_cycle_state_handler_in_game::update()
{
	DECLFUNC(0x004923E0, void, __thiscall, c_life_cycle_state_handler_in_game*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_game::update_for_state_transition()
{
	return DECLFUNC(0x00492960, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_in_game*)(this);
}

void c_life_cycle_state_handler_in_game::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x004920C0, void, __thiscall, c_life_cycle_state_handler_in_game*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_game::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00492140, void, __thiscall, c_life_cycle_state_handler_in_game*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_in_game::get_state_string()
{
	return DECLFUNC(0x004546F0, char const*, __thiscall, c_life_cycle_state_handler_in_game*)(this);
}

void c_life_cycle_state_handler_in_game::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00492290, void, __thiscall, c_life_cycle_state_handler_in_game*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_in_game, &handler_flags, 0, 0);
	m_game_instance = 0xFFFFFFFFFFFFFFFF;
}

