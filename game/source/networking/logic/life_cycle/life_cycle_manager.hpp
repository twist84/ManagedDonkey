#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"

const int32 k_maximum_state_change_entry_data_size = 16;

class c_life_cycle_state_handler;
class c_network_session_manager;
class c_network_session;
class c_network_observer;
class c_life_cycle_state_manager
{
public:
	c_life_cycle_state_manager();
	bool current_state_ready_for_state_transition_or_query() const;
	bool current_state_ready_for_update() const;
	void deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	void display_state_strings();
	static void dispose();
	c_network_session* get_active_squad_session() const;
	c_life_cycle_state_handler* get_current_state_handler() const;
	c_network_session* get_group_session() const;
	c_matchmaking_quality* get_matchmaking_quality();
	c_network_observer* get_observer() const;
	c_network_session* get_target_session() const;
	void initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session);
	void notify_expect_squad_join();
	void notify_lost_connection();
	void notify_session_disbandment_and_host_assumption(const c_network_session* session);
	void register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	void request_leave_sessions(bool leave_and_disconnect);
	void request_state_change(e_life_cycle_state state, int32 entry_data_size, const void* entry_data);
	void set_current_state(e_life_cycle_state state, int32 entry_data_size, void* entry_data);
	void set_pause_state(const char* state_string, bool enabled);
	void swap_squad_sessions();
	void swap_target_and_group_sessions();
	e_life_cycle_state get_current_state() const;
	void terminate();
	void update();

private:
	void update_handle_session_parameters();
	void update_handle_session_states();
	void update_messaging();

public:
//protected:
	e_life_cycle_state m_current_state;
	c_static_array<c_life_cycle_state_handler*, k_life_cycle_state_count> m_handlers;
	c_network_session_manager* m_session_manager;
	c_network_session* m_active_squad_session;
	c_network_session* m_target_session;
	c_network_session* m_group_session;
	c_network_observer* m_observer;
	bool m_pending_state_change;
	e_life_cycle_state m_pending_state;
	int32 m_pending_state_change_entry_data_size;
	uns8 m_pending_state_change_entry_data[k_maximum_state_change_entry_data_size];
	c_matchmaking_quality m_matchmaking_quality;
	bool m_matchmaking_messaging_session_identifier_valid;
	s_transport_secure_identifier m_matchmaking_messaging_session_identifier;
	int32 m_matchmaking_messaging_current_cookie;

	// >= play builds
	//bool m_pause_state_enabled;
	//e_life_cycle_state m_pause_state;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_manager) == 0x9990);
COMPILE_ASSERT(0x0000 == OFFSETOF(c_life_cycle_state_manager, m_current_state));
COMPILE_ASSERT(0x0004 == OFFSETOF(c_life_cycle_state_manager, m_handlers));
COMPILE_ASSERT(0x004C == OFFSETOF(c_life_cycle_state_manager, m_session_manager));
COMPILE_ASSERT(0x0050 == OFFSETOF(c_life_cycle_state_manager, m_active_squad_session));
COMPILE_ASSERT(0x0054 == OFFSETOF(c_life_cycle_state_manager, m_target_session));
COMPILE_ASSERT(0x0058 == OFFSETOF(c_life_cycle_state_manager, m_group_session));
COMPILE_ASSERT(0x005C == OFFSETOF(c_life_cycle_state_manager, m_observer));
COMPILE_ASSERT(0x0060 == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change));
COMPILE_ASSERT(0x0064 == OFFSETOF(c_life_cycle_state_manager, m_pending_state));
COMPILE_ASSERT(0x0068 == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change_entry_data_size));
COMPILE_ASSERT(0x006C == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change_entry_data));
COMPILE_ASSERT(0x0080 == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_quality));
COMPILE_ASSERT(0x9978 == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_session_identifier_valid));
COMPILE_ASSERT(0x9979 == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_session_identifier));
COMPILE_ASSERT(0x998C == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_current_cookie));


