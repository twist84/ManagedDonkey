#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

const long k_maximum_state_change_entry_data_size = 16;

struct c_life_cycle_state_handler;
struct c_network_session_manager;
struct c_network_session;
struct c_network_observer;
struct c_life_cycle_state_manager
{
public:
	void request_state_change(e_life_cycle_state state, long entry_data_size, void* entry_data);
	void request_leave_sessions(bool disconnect);
	void set_current_state(e_life_cycle_state state, long entry_data_size, void* entry_data);
	e_life_cycle_state get_current_state() const;
	c_life_cycle_state_handler* get_current_state_handler() const;
	void terminate();
	c_network_session* get_active_squad_session() const;
	c_network_session* get_target_session() const;
	c_network_session* get_group_session() const;
	c_network_observer* get_observer() const;
	void register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	void deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	
//protected:
	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_current_state;
	c_static_array<c_life_cycle_state_handler*, k_life_cycle_state_count> m_handlers;
	c_network_session_manager* m_session_manager;
	c_network_session* m_active_squad_session;
	c_network_session* m_target_session;
	c_network_session* m_group_session;
	c_network_observer* m_observer;

	bool m_pending_state_change;
	byte : 8;
	byte : 8;
	byte : 8;
	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_pending_state;

	long m_entry_data_size;
	byte m_entry_data[k_maximum_state_change_entry_data_size];

	// is this another struct?
	//byte __data7C[0x4];
	//c_matchmaking_quality m_matchmaking_quality; // c_matchmaking_quality* c_life_cycle_state_manager::get_matchmaking_quality()
	//byte __data9978[0x18];
	byte __data7C[0x9914];
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x9990);

