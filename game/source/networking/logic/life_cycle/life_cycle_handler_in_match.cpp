#include "networking/logic/life_cycle/life_cycle_handler_in_match.hpp"

c_life_cycle_state_handler_in_match::c_life_cycle_state_handler_in_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_match_start_time(),
	m_match_game_instance(),
	m_handled_simulation_abort(),
	m_simulation_aborted_timestamp()
{
	//DECLFUNC(0x00453F30, void, __thiscall, c_life_cycle_state_handler_in_match*)(this);
}

char const* c_life_cycle_state_handler_in_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454700, c_life_cycle_state_handler_in_match, get_state_string);

	return "in-match";
}

//.text:00491070 ; public: void c_life_cycle_state_handler_in_match::dispose()

void c_life_cycle_state_handler_in_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00491080, c_life_cycle_state_handler_in_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x004910E0, c_life_cycle_state_handler_in_match, exit, to);
}

//.text:004911A0 ; private: bool c_life_cycle_state_handler_in_match::group_has_required_session_parameters()

void c_life_cycle_state_handler_in_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x004911C0, c_life_cycle_state_handler_in_match, initialize, manager);
}

bool c_life_cycle_state_handler_in_match::is_map_load_pending() const
{
	return m_flags.test(_life_cycle_state_handler_in_match_map_load);
}

//.text:004911F0 ; network_time_difference_msec
//.text:00491200 ; network_time_get
//.text:00491220 ; network_time_get_exact
//.text:00491230 ; network_time_since
//.text:00491250 ; public: void c_flags_no_init<e_life_cycle_state_handler_in_match_flags, 1>::set(e_life_cycle_state_handler_in_match_flags, bool)

void c_life_cycle_state_handler_in_match::update()
{
	INVOKE_CLASS_MEMBER(0x00491270, c_life_cycle_state_handler_in_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00491480, c_life_cycle_state_handler_in_match, update_for_state_transition);
}

