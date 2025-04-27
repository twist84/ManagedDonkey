#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"

enum e_network_session_state
{
	_network_session_state_none = 0,
	_network_session_state_peer_creating,
	_network_session_state_peer_joining,
	_network_session_state_peer_join_abort,
	_network_session_state_peer_established,
	_network_session_state_peer_leaving,
	_network_session_state_host_established,
	_network_session_state_host_disband,

	//_network_session_state_host_handoff,
	//_network_session_state_host_reestablish,
	//_network_session_state_election,

	k_network_session_state_count
};

struct s_network_session_state_peer_creating
{
	// e_transport_platform
	int32 platform;
	bool __unknown4;
	uint64 join_nonce;
	s_network_session_join_request join_request;
};
static_assert(sizeof(s_network_session_state_peer_creating) == 0x320);

struct s_network_session_state_peer_joining
{
	int32 observer_channel;
	s_transport_secure_address join_address;
	transport_address join_usable_address;
	bool use_peer_joining_join_nonce;
	uint64 join_nonce;
	s_network_session_join_request join_request;
	uint32 join_secure_connection_timestamp;
	uint32 __unknown34C;
	uint32 __unknown350;
	uint8 __data[0xC];
};
static_assert(sizeof(s_network_session_state_peer_joining) == 0x360);

struct s_network_session_state_peer_join_abort
{
	s_transport_secure_address join_address;
	transport_address join_usable_address;
	s_transport_secure_address secure_address;
	uint64 join_nonce;
	uint32 join_secure_connection_timestamp;
	uint32 join_abort_secure_connection_timestamp;
	uint8 __data[0x8];
};
static_assert(sizeof(s_network_session_state_peer_join_abort) == 0x50);

struct s_network_session_state_peer_established
{
	uint32 __unknown0;
	uint32 peer_established_secure_connection_timestamp;
};
static_assert(sizeof(s_network_session_state_peer_established) == 0x8);

struct s_network_session_state_peer_leaving
{
	uint8 __data[0x8];
	uint32 __unknown8;
};
static_assert(sizeof(s_network_session_state_peer_leaving) == 0xC);

