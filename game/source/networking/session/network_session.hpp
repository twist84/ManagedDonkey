#pragma once

#include "networking/logic/network_join.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/session/network_session_parameters.hpp"

enum e_network_session_disconnection_policy
{
	_network_session_disconnection_allow_waiting_for_establishment = 0,
	_network_session_disconnection_allow,
	_network_session_disconnection_reestablishes_as_host,

	k_network_session_disconnection_policy_count
};

class c_network_channel_owner
{
public:
	virtual bool desire_channel_heartbeat(int32 observer_channel_index) const;
	virtual bool channel_is_load_bearing(int32 observer_channel_index) const;
	virtual bool attempt_channel_reconnection(int32 observer_channel_index, bool preexisting_connection) const;
	virtual void notify_channel_connection(int32 observer_channel_index, uns32 connection_identifier, bool connected);
	virtual void notify_channel_died(int32 observer_channel_index);
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

class c_network_message_gateway;
class c_network_observer;
class c_network_session_manager;
class c_network_channel;
class c_network_session :
	public c_network_channel_owner
{
public:
	bool peer_request_player_desired_properties_update_(int32 player_update_number, e_controller_index controller_index, const s_player_configuration_from_client* player_data_from_client, uns32 player_voice);

public:
	int32 current_local_state() const;
	bool disconnected() const;
	bool established() const;
	bool is_host() const;
	bool is_leader() const;
	bool leaving_session() const;
	bool peer_joining() const;
	bool channel_is_authoritative(c_network_channel* channel);
	void destroy_session();
	void disconnect();
	void force_disconnect();
	bool force_disconnect_peer(const s_transport_secure_address* peer_secure_address);
	const char* get_disconnection_policy_string() const;
	int32 get_peer_observer_channel(int32 peer_index) const;
	const c_network_session_membership* get_session_membership() const;
	c_network_session_membership* get_session_membership_for_update();
	const c_network_session_membership* get_session_membership_unsafe() const;
	int32 get_session_membership_update_number() const;
	c_network_session_parameters* get_session_parameters();
	const c_network_session_parameters* get_session_parameters() const;
	static const char* get_type_string(e_network_session_type session_type);
	static const char* get_class_string(e_network_session_class session_class);
	const char* get_state_string() const;
	const char* get_mode_string() const;
	void handle_disconnection();
	bool handle_leave_internal(int32 peer_index);
	bool has_managed_session_connection() const;
	bool host_assume_leadership();
	bool host_boot_machine(int32 peer_index, e_network_session_boot_reason boot_reason);
	void host_connection_refused(const transport_address* address, e_network_join_refuse_reason refuse_reason);
	bool host_established() const;
	bool host_set_player_current_properties(int32 player_index, const s_player_configuration* player_data);
	void idle();
	bool initialize_session(int32 session_index, e_network_session_type session_type, c_network_message_gateway* message_gateway, c_network_observer* observer, c_network_session_manager* session_manager);
	void initiate_leave_protocol(bool leave_immediately);
	bool is_peer_joining_this_session() const;
	bool join_abort(const transport_address* incoming_address, uns64 join_nonce);
	bool join_abort_in_progress(const transport_address* address) const;
	void join_abort_successful(const transport_address* address);
	void join_accept(const s_network_session_join_request* join_request, const transport_address* address);
	bool join_allowed_by_privacy() const;
	//join_remote_session
	bool leader_request_boot_machine(const s_transport_secure_address* boot_peer_address, e_network_session_boot_reason boot_reason);
	bool leader_request_delegate_leadership(const s_transport_secure_address* leader_address);
	e_network_session_mode session_mode() const;
	e_network_session_type session_type() const;
	s_network_session_player* get_player(int32 player_index);
	void leave_session();
	bool membership_is_locked() const;
	bool peer_request_player_desired_properties_update(int32 player_update_number, e_controller_index controller_index, const s_player_configuration_from_client* player_data_from_client, uns32 player_voice);
	e_network_session_class session_class() const;

private:
	void set_disconnection_policy(e_network_session_disconnection_policy disconnection_policy);

public:
	bool waiting_for_host_connection(const transport_address* address) const;
	void leave_session_and_disconnect();
	bool handle_boot_machine(c_network_channel* channel, const s_network_message_boot_machine* message);
	bool handle_delegate_leadership(c_network_channel* channel, const s_network_message_delegate_leadership* message);
	bool handle_host_decline(c_network_channel* channel, const s_network_message_host_decline* message);
	bool handle_join_request(const transport_address* address, const s_network_message_join_request* message);
	void handle_leave_acknowledgement(const transport_address* address);
	bool handle_leave_request(const transport_address* address);
	bool handle_membership_update(const s_network_message_membership_update* message);
	bool handle_parameters_request(c_network_channel* channel, const s_network_message_parameters_request* message);
	bool handle_parameters_update(const s_network_message_parameters_update* message);
	void handle_peer_connect(const transport_address* address, const s_network_message_peer_connect* message);
	bool handle_peer_establish(c_network_channel* channel, const s_network_message_peer_establish* message);
	bool handle_peer_properties(c_network_channel* channel, const s_network_message_peer_properties* message);
	bool handle_player_add(c_network_channel* channel, const s_network_message_player_add* message);
	bool handle_player_properties(c_network_channel* channel, const s_network_message_player_properties* message);
	bool handle_player_refuse(c_network_channel* channel, const s_network_message_player_refuse* message);
	bool handle_player_remove(c_network_channel* channel, const s_network_message_player_remove* message);
	bool handle_session_boot(const transport_address* address, const s_network_message_session_boot* message);
	bool handle_session_disband(const transport_address* address, const s_network_message_session_disband* message);
	bool handle_time_synchronize(const transport_address* address, const s_network_message_time_synchronize* message);

	struct s_local_state_data_peer_creating
	{
		int32 secure_key_platform; // e_transport_platform
		bool connect_not_join;
		uns64 join_nonce;
		s_network_session_join_request join_request;
	};
	static_assert(sizeof(s_local_state_data_peer_creating) == 0x320);

	struct s_local_state_data_peer_joining
	{
		int32 observer_channel_index;
		s_transport_secure_address join_remote_address;
		transport_address join_usable_address;
		bool connect_not_join;
		uns64 join_nonce;
		s_network_session_join_request join_request;
		uns32 join_initiated_timestamp;
		uns32 join_ping_from_host_timestamp;
		uns32 join_secure_connection_timestamp;
		int32 join_attempt_count;
		uns32 last_join_attempt_timestamp;
	};
	static_assert(sizeof(s_local_state_data_peer_joining) == 0x360);

	struct s_local_state_data_peer_join_abort
	{
		s_transport_secure_address join_remote_address;
		transport_address join_usable_address;
		s_transport_secure_address joining_local_address;
		uns64 join_nonce;
		uns32 join_initiated_timestamp;
		uns32 join_abort_initiated_timestamp;
		uns32 last_join_abort_timestamp;
	};
	static_assert(sizeof(s_local_state_data_peer_join_abort) == 0x50);

	struct s_local_state_data_peer_established
	{
		int32 peer_reestablishment_state; // e_peer_reestablish_state
		uns32 established_timestamp;
	};
	static_assert(sizeof(s_local_state_data_peer_established) == 0x8);

	struct s_local_state_data_peer_leaving
	{
		int32 peer_reestablishment_state; // e_peer_reestablish_state
		uns32 leave_initiated_timestamp;
		uns32 last_leave_attempt_timestamp;
	};
	static_assert(sizeof(s_local_state_data_peer_leaving) == 0xC);

	struct s_local_state_data
	{
		union
		{
			s_local_state_data_peer_creating peer_creating;
			s_local_state_data_peer_joining peer_joining;
			s_local_state_data_peer_join_abort peer_join_abort;
			s_local_state_data_peer_established peer_established;
			s_local_state_data_peer_leaving peer_leaving;
			//s_local_state_data_host_handoff host_handoff;
			//s_local_state_data_host_reestablish host_reestablish;
			//s_local_state_data_election election;
		};
	};

	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
	int32 m_session_index;
	e_network_session_type m_session_type;
	e_network_session_class m_session_class;
	c_network_session_membership m_session_membership;
	c_network_session_parameters m_session_parameters;
	e_network_session_state m_local_state;
	c_network_session::s_local_state_data m_local_state_data;
	uns32 m_local_connection_identifier;
	uns32 m_local_last_time_synchronize_success_timestamp;
	uns32 m_local_last_time_synchronize_sent_timestamp;
	bool m_time_exists;
	int32 m_time_offset;
	int32 m_creation_time;
	int32 m_managed_session_index;
	e_network_join_refuse_reason m_join_refuse_reason;
	uns64 m_host_join_nonce;
	bool m_only_use_squad_session_for_election;
	bool m_enable_speculative_host_migration;
	uns32 m_time_of_last_speculative_host_migration;
	uns32 m_time_of_last_speculative_host_migration_check;
	uns32 m_speculative_host_migration_disallow_mask;
	e_network_session_disconnection_policy m_disconnection_policy;

	struct s_network_session_local_player
	{
		s_player_identifier player_identifier;
		s_transport_secure_address secure_address;
		int32 peer_output_user_index;
	} m_player_we_are_adding;

	struct
	{
		s_player_identifier player_identifier;
		e_network_join_refuse_reason refuse_reason;
		uns32 request_time;
	} m_local_user_player_add;
};
static_assert(sizeof(c_network_session) == 0x25BC40);

extern const char* g_session_disconnection_policy_strings[k_network_session_disconnection_policy_count];

