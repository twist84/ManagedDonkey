#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/session/network_session_parameters.hpp"
#include "networking/session/network_session_state.hpp"
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

struct s_network_message_boot_machine;
struct s_network_message_delegate_leadership;
struct s_network_message_host_decline;
struct s_network_message_join_request;
struct s_network_message_membership_update;
struct s_network_message_parameters_request;
struct s_network_message_parameters_update;
struct s_network_message_peer_connect;
struct s_network_message_peer_establish;
struct s_network_message_peer_properties;
struct s_network_message_player_add;
struct s_network_message_player_properties;
struct s_network_message_player_refuse;
struct s_network_message_player_remove;
struct s_network_message_session_boot;
struct s_network_message_session_disband;
struct s_network_message_time_synchronize;

struct c_network_message_gateway;
struct c_network_observer;
struct c_network_session_manager;
struct c_network_channel;
struct c_network_session :
	public c_network_channel_owner
{
	long current_local_state() const;
	bool disconnected() const;
	bool established() const;
	bool is_host() const;
	bool is_leader() const;
	bool leaving_session() const;
	bool peer_joining() const;
	bool channel_is_authoritative(c_network_channel* channel);
	void destroy_session();
	void force_disconnect();
	bool force_disconnect_peer(s_transport_secure_address const* peer_secure_address);
	long get_peer_observer_channel(long peer_index) const;
	c_network_session_membership const* get_session_membership() const;
	c_network_session_membership* get_session_membership_for_update();
	c_network_session_membership const* get_session_membership_unsafe() const;
	long get_session_membership_update_number() const;
	c_network_session_parameters* get_session_parameters();
	c_network_session_parameters const* get_session_parameters() const;
	static char const* get_type_string(e_network_session_type session_type);
	static char const* get_class_string(e_network_session_class session_class);
	char const* get_state_string() const;
	char const* get_mode_string() const;
	void handle_disconnection();
	bool handle_leave_internal(long peer_index);
	bool has_managed_session_connection() const;
	bool host_assume_leadership();
	bool host_boot_machine(long peer_index, e_network_session_boot_reason boot_reason);
	void host_connection_refused(transport_address const* address, e_network_join_refuse_reason refuse_reason);
	bool host_established() const;
	bool host_set_player_current_properties(long player_index, s_player_configuration const* player_data);
	void idle();
	bool initialize_session(long session_index, e_network_session_type session_type, c_network_message_gateway* message_gateway, c_network_observer* observer, c_network_session_manager* session_manager);
	void initiate_leave_protocol(bool leave_immediately);
	bool is_peer_joining_this_session() const;
	bool join_abort(transport_address const* incoming_address, qword join_nonce);
	bool join_abort_in_progress(transport_address const* address) const;
	void join_abort_successful(transport_address const* address);
	void join_accept(s_network_session_join_request const* join_request, transport_address const* address);
	bool join_allowed_by_privacy() const;
	//join_remote_session
	bool leader_request_boot_machine(s_transport_secure_address const* boot_peer_address, e_network_session_boot_reason boot_reason);
	bool leader_request_delegate_leadership(s_transport_secure_address const* leader_address);
	e_network_session_mode session_mode() const;
	s_network_session_player* get_player(long player_index);
	bool peer_request_player_desired_properties_update(long player_update_number, e_controller_index controller_index, s_player_configuration_from_client const* player_data_from_client, dword player_voice);
	e_network_session_class session_class() const;
	bool waiting_for_host_connection(transport_address const* address) const;
	bool handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message);
	bool handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message);
	bool handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message);
	bool handle_join_request(transport_address const* address, s_network_message_join_request const* message);
	void handle_leave_acknowledgement(transport_address const* address);
	bool handle_leave_request(transport_address const* address);
	bool handle_membership_update(s_network_message_membership_update const* message);
	bool handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	bool handle_parameters_update(s_network_message_parameters_update const* message);
	void handle_peer_connect(transport_address const* address, s_network_message_peer_connect const* message);
	bool handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	bool handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message);
	bool handle_player_add(c_network_channel* channel, s_network_message_player_add const* message);
	bool handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	bool handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	bool handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message);
	bool handle_session_boot(transport_address const* address, s_network_message_session_boot const* message);
	bool handle_session_disband(transport_address const* address, s_network_message_session_disband const* message);
	bool handle_time_synchronize(transport_address const* address, s_network_message_time_synchronize const* message);

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	long m_session_index;
	c_enum<e_network_session_type, long, _network_session_type_none, k_network_session_type_count> m_session_type;
	c_enum<e_network_session_class, long, _network_session_class_offline, k_network_session_class_count> m_session_class;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;

	c_enum<e_network_session_state, long, _network_session_state_none, k_network_session_state_count> m_local_state;
	union
	{
		s_network_session_state_peer_creating peer_creating;
		s_network_session_state_peer_joining peer_joining;
		s_network_session_state_peer_join_abort peer_join_abort;
		s_network_session_state_peer_established peer_established;
		s_network_session_state_peer_leaving peer_leaving;
	} m_local_state_data;

	dword m_connection_identifier;
	dword m_time_synchronization_end_time;
	dword m_time_synchronization_start_time;
	bool m_time_exists;
	dword m_time;
	long __unknown25BBEC;
	long m_managed_session_index;
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

