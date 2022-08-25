#pragma once

#include "cseries/cseries.hpp"

enum e_life_cycle_state_transition_type
{

};

enum e_life_cycle_session_type
{

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

	long m_state;
	c_life_cycle_state_manager* m_manager;

	byte m_handler_flags;
	byte : 8;
	byte : 8;
	byte : 8;

	char __data14[0x14];
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x28);

struct c_life_cycle_state_manager
{
	long m_current_state;
	struct c_life_cycle_state_handler* m_handlers[18];
	struct c_network_session_manager* m_session_manager;
	struct c_network_session* m_active_squad_session;
	struct c_network_session* m_target_session;
	struct c_network_session* m_group_session;
	struct c_network_observer* m_observer;
	byte __data[0x9930];
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x9990);