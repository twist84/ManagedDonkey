#include "networking/logic/life_cycle/life_cycle_handler_in_game.hpp"

c_life_cycle_state_handler_in_game::c_life_cycle_state_handler_in_game() :
	c_life_cycle_state_handler(),
	m_flags(0)
{
	//DECLFUNC(0x00453F10, void, __thiscall, c_life_cycle_state_handler_in_game*)(this);
}

char const* c_life_cycle_state_handler_in_game::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546F0, c_life_cycle_state_handler_in_game, get_state_string);

	return "in-game";
}

void c_life_cycle_state_handler_in_game::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004920C0, c_life_cycle_state_handler_in_game, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_game::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00492140, c_life_cycle_state_handler_in_game, exit, to);
}

void c_life_cycle_state_handler_in_game::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00492290, c_life_cycle_state_handler_in_game, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_in_game, parent_flags, 0ULL, 0ULL);
	m_game_instance = NONE;
}

void c_life_cycle_state_handler_in_game::update()
{
	INVOKE_CLASS_MEMBER(0x004923E0, c_life_cycle_state_handler_in_game, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_game::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00492960, c_life_cycle_state_handler_in_game, update_for_state_transition);
}

