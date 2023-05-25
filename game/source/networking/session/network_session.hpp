#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/session/network_session_parameters.hpp"
#include "shell/shell.hpp"

enum e_network_session_disconnection_policy
{
	_network_session_disconnection_waiting_for_establishment = 0,
	_network_session_disconnection_allowed,
	_network_session_disconnection_reestablish_as_host,

	k_network_session_disconnection_policy_count
};

struct s_network_session_local_player
{
	s_player_identifier player_identifier;
	s_transport_secure_address secure_address;
	long peer_output_user_index;
};
static_assert(sizeof(s_network_session_local_player) == 0x1C);

struct s_local_player_add_queue_entry
{
	s_player_identifier identifier;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> refuse_reason;
	dword add_time;
};
static_assert(sizeof(s_local_player_add_queue_entry) == 0x10);

struct c_network_channel_owner
{
public:
	virtual bool desire_channel_heartbeat(long observer_channel_index) const;
	virtual bool channel_is_load_bearing(long observer_channel_index) const;
	virtual bool attempt_channel_reconnection(long observer_channel_index, bool a2) const;
	virtual void notify_channel_connection(long observer_channel_index, unsigned long a2, bool a3);
	virtual void notify_channel_died(long observer_channel_index);
};
static_assert(sizeof(c_network_channel_owner) == 0x4);

struct c_network_message_gateway;
struct c_network_observer;
struct c_network_session_manager;
struct c_network_session : c_network_channel_owner
{
	c_network_session_membership const* get_session_membership() const;
	c_network_session_membership* get_session_membership_for_update();
	c_network_session_membership const* get_session_membership_unsafe() const;
	c_network_session_parameters const* get_session_parameters() const;
	c_network_session_parameters* get_session_parameters();
	long current_local_state() const;
	bool disconnected() const;
	bool established() const;
	bool is_host() const;
	bool is_leader();
	e_network_session_mode session_mode() const;

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index;
	c_enum<e_network_session_type, long, _network_session_type_none, k_network_session_type_count> m_session_type;
	c_enum<e_network_session_class, long, _network_session_class_offline, k_network_session_class_count> m_session_class;
	long __unknown1C;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	long m_local_state;
	long __unknown74;
	byte m_local_state_data[864];
	dword m_connection_identifier;
	dword m_time_synchronization_end_time;
	dword m_time_synchronization_start_time;
	bool m_time_exists;
	dword m_time;
	long __unknown25BBEC;
	int m_managed_session_index;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> m_join_refuse_reason;
	qword m_host_join_nonce;
	bool m_close_session;
	bool m_close_session_for_matchmaking;
	bool __unknown25BC02;
	bool __unknown25BC03;
	dword m_matchmaking_start_time;
	dword m_matchmaking_establish_time;
	dword __unknown25BC0C;
	c_enum<e_network_session_disconnection_policy, long, _network_session_disconnection_waiting_for_establishment, k_network_session_disconnection_policy_count> m_disconnection_policy;
	s_network_session_local_player m_player_we_are_adding;
	s_local_player_add_queue_entry m_local_user_player_add;
};
static_assert(sizeof(c_network_session) == 0x25BC40);

