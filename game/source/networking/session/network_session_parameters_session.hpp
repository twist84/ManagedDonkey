#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"
#include "networking/transport/transport_security.hpp"
#include "shell/shell.hpp"

struct s_network_session_parameter_session_mode
{
	long session_mode_sequence;
	c_enum<e_network_session_mode, long, k_network_session_mode_count> session_mode;
	dword session_mode_timestamp;
};
static_assert(sizeof(s_network_session_parameter_session_mode) == 0xC);

struct c_network_session_parameter_session_mode :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_session_mode>
{
	e_network_session_mode get() const;
	bool request_change(e_network_session_mode session_mode);
	bool set(e_network_session_mode session_mode);
};
static_assert(sizeof(c_network_session_parameter_session_mode) == 0x48);

struct s_network_session_parameter_session_size
{
	long maximum_peer_count;
	long maximum_player_count;
};
static_assert(sizeof(s_network_session_parameter_session_size) == 0x8);

struct c_network_session_parameter_session_size
	: public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_session_size>
{
};
static_assert(sizeof(c_network_session_parameter_session_size) == 0x40);

struct s_network_session_remote_session_join_data
{
	c_enum<e_join_remote_state, long, k_join_remote_state_count> join_state;
	c_enum<e_join_type, long, k_join_type_count> join_from;
	c_enum<e_join_type, long, k_join_type_count> join_to;
	c_enum<e_networking_join_destination_squad, long, k_join_destination_count> join_target;
	qword join_nonce;
	c_enum<e_transport_platform, long, k_transport_platform_count> platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key session_key;
	s_transport_secure_address host_secure_address;
	c_enum<e_network_session_class, long, k_network_session_class_count> session_class;
	c_enum<e_life_cycle_join_result, long, k_life_cycle_join_result_count> join_result;
	bool join_to_public_slots;
};
static_assert(sizeof(s_network_session_remote_session_join_data) == 0x58);

struct c_network_session_parameter_requested_remote_join_data :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
static_assert(sizeof(c_network_session_parameter_requested_remote_join_data) == 0xE0);

struct c_network_session_parameter_remote_join_data :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
static_assert(sizeof(c_network_session_parameter_remote_join_data) == 0xE0);

struct s_network_session_parameter_lobby_vote_set
{
	struct s_lobby_vote
	{
		byte __unknown0;
		byte __unknown1;
		byte __unknown2;
	};

	c_static_array<s_lobby_vote, 2> __unknown0;
	byte __unknown6;
	byte : 8;
};
static_assert(sizeof(s_network_session_parameter_lobby_vote_set) == 0x8);

struct c_network_session_parameter_lobby_vote_set :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_lobby_vote_set>
{
};
static_assert(sizeof(c_network_session_parameter_lobby_vote_set) == 0x40);
