#include "networking/logic/life_cycle/life_cycle_handler_leaving.hpp"

c_life_cycle_state_handler_leaving::c_life_cycle_state_handler_leaving() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_leave_and_disconnect(),
	m_failure_game_mode()
{
	//DECLFUNC(0x00453F70, void, __thiscall, c_life_cycle_state_handler_leaving*)(this);
}

const char* c_life_cycle_state_handler_leaving::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454720, c_life_cycle_state_handler_leaving, get_state_string);

	return "leaving";
}

void c_life_cycle_state_handler_leaving::dispose()
{
	//INVOKE_CLASS_MEMBER(0x004933D0, c_life_cycle_state_handler_leaving, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_leaving::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004933E0, c_life_cycle_state_handler_leaving, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_leaving::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00493460, c_life_cycle_state_handler_leaving, exit, to);
}

//.text:00493480 ; 

void c_life_cycle_state_handler_leaving::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00493490, c_life_cycle_state_handler_leaving, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_leaving, &parent_flags, 0x0ULL, 0x0ULL);
}

//.text:004934C0 ; 
//.text:004934E0 ; 

void c_life_cycle_state_handler_leaving::update()
{
	INVOKE_CLASS_MEMBER(0x00493500, c_life_cycle_state_handler_leaving, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_leaving::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00493610, c_life_cycle_state_handler_leaving, update_for_state_transition);
}

