#pragma once

#include "cseries/cseries.hpp"
#include "life_cycle_matchmaking_quality.hpp"

const int32 k_maximum_state_change_entry_data_size = 16;

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
	void request_state_change(e_life_cycle_state state, int32 entry_data_size, void const* entry_data);
	void set_current_state(e_life_cycle_state state, int32 entry_data_size, void* entry_data);
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
	byte pad[3];
	s_transport_secure_identifier m_matchmaking_messaging_session_identifier;
	int32 m_matchmaking_messaging_current_cookie;
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x9990);
static_assert(0x0000 == OFFSETOF(c_life_cycle_state_manager, m_current_state));
static_assert(0x0004 == OFFSETOF(c_life_cycle_state_manager, m_handlers));
static_assert(0x004C == OFFSETOF(c_life_cycle_state_manager, m_session_manager));
static_assert(0x0050 == OFFSETOF(c_life_cycle_state_manager, m_active_squad_session));
static_assert(0x0054 == OFFSETOF(c_life_cycle_state_manager, m_target_session));
static_assert(0x0058 == OFFSETOF(c_life_cycle_state_manager, m_group_session));
static_assert(0x005C == OFFSETOF(c_life_cycle_state_manager, m_observer));
static_assert(0x0060 == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change));
static_assert(0x0064 == OFFSETOF(c_life_cycle_state_manager, m_pending_state));
static_assert(0x0068 == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change_entry_data_size));
static_assert(0x006C == OFFSETOF(c_life_cycle_state_manager, m_pending_state_change_entry_data));
static_assert(0x0080 == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_quality));
static_assert(0x9978 == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_session_identifier_valid));
static_assert(0x9979 == OFFSETOF(c_life_cycle_state_manager, pad));
static_assert(0x997C == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_session_identifier));
static_assert(0x998C == OFFSETOF(c_life_cycle_state_manager, m_matchmaking_messaging_current_cookie));


