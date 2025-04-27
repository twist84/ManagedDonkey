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
	bool current_state_ready_for_state_transition_or_query() const;
	bool current_state_ready_for_update() const;
	void deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	c_life_cycle_state_handler* get_current_state_handler() const;
	void initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* active_squad_session, c_network_session* target_session, c_network_session* group_session);
	void notify_expect_squad_join();
	void request_leave_sessions(bool disconnect);
	void request_state_change(e_life_cycle_state state, long entry_data_size, void const* entry_data);
	void set_current_state(e_life_cycle_state state, long entry_data_size, void* entry_data);
	void swap_squad_sessions();
	void swap_target_and_group_sessions();
	e_life_cycle_state get_current_state() const;
	void terminate();
	void update();

	c_network_session* get_active_squad_session() const;
	c_network_session* get_target_session() const;
	c_network_session* get_group_session() const;
	c_network_observer* get_observer() const;
	void register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	
//protected:
	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_current_state;
	c_static_array<c_life_cycle_state_handler*, k_life_cycle_state_count> m_handlers;
	c_network_session_manager* m_session_manager;
	c_network_session* m_active_squad_session;
	c_network_session* m_target_session;
	c_network_session* m_group_session;
	c_network_observer* m_observer;

	bool m_pending_state_change;
	uint8 : 8;
	uint8 : 8;
	uint8 : 8;
	c_enum<e_life_cycle_state, long, _life_cycle_state_none, k_life_cycle_state_count> m_pending_state;

	long m_entry_data_size;
	uint8 m_entry_data[k_maximum_state_change_entry_data_size];

	// is this another struct?
	//uint8 __data7C[0x4];
	//c_matchmaking_quality m_matchmaking_quality; // c_matchmaking_quality* c_life_cycle_state_manager::get_matchmaking_quality()
	//uint8 __data9978[0x18];
	uint8 __data7C[0x9914];
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x9990);

