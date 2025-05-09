#include "networking/logic/life_cycle/life_cycle_handler_in_match.hpp"

c_life_cycle_state_handler_in_match::c_life_cycle_state_handler_in_match() :
	c_life_cycle_state_handler(),
	m_flags(0)
{
	//DECLFUNC(0x00453F30, void, __thiscall, c_life_cycle_state_handler_in_match*)(this);
}

char const* c_life_cycle_state_handler_in_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454700, c_life_cycle_state_handler_in_match, get_state_string);

	return "in-match";
}

void c_life_cycle_state_handler_in_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00491080, c_life_cycle_state_handler_in_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x004910E0, c_life_cycle_state_handler_in_match, exit, to);
}

void c_life_cycle_state_handler_in_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x004911C0, c_life_cycle_state_handler_in_match, initialize, manager);
}

void c_life_cycle_state_handler_in_match::update()
{
	INVOKE_CLASS_MEMBER(0x00491270, c_life_cycle_state_handler_in_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00491480, c_life_cycle_state_handler_in_match, update_for_state_transition);
}

