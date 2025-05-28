#include "networking/logic/life_cycle/life_cycle_handler_joining.hpp"

c_life_cycle_state_handler_joining::c_life_cycle_state_handler_joining() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_failure_game_mode(),
	m_previous_state()
{
	//DECLFUNC(0x00453F50, void, __thiscall, c_life_cycle_state_handler_joining*)(this);
}

char const* c_life_cycle_state_handler_joining::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454710, c_life_cycle_state_handler_joining, get_state_string);

	return "joining";
}

void c_life_cycle_state_handler_joining::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00493660, c_life_cycle_state_handler_joining, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_joining::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004933E0, c_life_cycle_state_handler_joining, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_joining::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00493460, c_life_cycle_state_handler_joining, exit, to);
}

//.text:00493760 ; 

void c_life_cycle_state_handler_joining::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00493770, c_life_cycle_state_handler_joining, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_joining, &parent_flags, 0ULL, 0ULL);
}

bool c_life_cycle_state_handler_joining::rematch_join_in_progress() const
{
	//return INVOKE_CLASS_MEMBER(0x004937A0, c_life_cycle_state_handler_joining, rematch_join_in_progress);

	return m_previous_state == _life_cycle_state_post_match;
}

//.text:004937B0 ; 
//.text:004937D0 ; 

void c_life_cycle_state_handler_joining::update()
{
	INVOKE_CLASS_MEMBER(0x00493500, c_life_cycle_state_handler_joining, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_joining::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00493610, c_life_cycle_state_handler_joining, update_for_state_transition);
}

