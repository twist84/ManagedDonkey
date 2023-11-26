#pragma once

#include "cseries/cseries.hpp"
#include "networking/network_utilities.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"

enum e_life_cycle_state_transition_type
{

};

enum e_life_cycle_state_handler_flags
{

	k_life_cycle_state_handler_flags = 6
};

struct c_life_cycle_state_manager;
struct c_life_cycle_state_handler
{
	virtual void update(void);
	virtual e_life_cycle_state_transition_type update_for_state_transition(long);
	virtual void enter(c_life_cycle_state_handler*, long, void*);
	virtual void exit(c_life_cycle_state_handler*);
	virtual char const* get_state_string(void);
	virtual void handle_missing_required_session_parameter(e_life_cycle_session_type);

	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_state;
	c_life_cycle_state_manager* m_manager;

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> m_handler_flags;
	c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> type_flags0;
	c_flags<e_network_session_parameter_type, qword, k_network_session_parameter_type_count> type_flags1;
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x28);

