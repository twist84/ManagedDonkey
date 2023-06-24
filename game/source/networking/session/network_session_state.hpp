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

	k_network_session_state_count
};

struct s_network_session_state_peer_creating
{
	// e_transport_platform
	long platform;
	bool __unknown4;
	qword join_nonce;
	s_network_session_join_request join_request;
};
static_assert(sizeof(s_network_session_state_peer_creating) == 0x320);

struct s_network_session_state_peer_joining
{
	long observer_channel;
	s_transport_secure_address join_address;
	transport_address join_usable_address;
	bool use_peer_joining_join_nonce;
	qword join_nonce;
	s_network_session_join_request join_request;
	dword join_secure_connection_timestamp;
	dword __unknown34C;
	dword __unknown350;
	byte __data[0xC];
};
static_assert(sizeof(s_network_session_state_peer_joining) == 0x360);

struct s_network_session_state_peer_join_abort
{
	s_transport_secure_address join_address;
	transport_address join_usable_address;
	s_transport_secure_address secure_address;
	qword join_nonce;
	dword join_secure_connection_timestamp;
	dword join_abort_secure_connection_timestamp;
	byte __data[0x8];
};
static_assert(sizeof(s_network_session_state_peer_join_abort) == 0x50);

struct s_network_session_state_peer_established
{
	dword __unknown0;
	dword peer_established_secure_connection_timestamp;
};
static_assert(sizeof(s_network_session_state_peer_established) == 0x8);

struct s_network_session_state_peer_leaving
{
	byte __data[0x8];
	dword __unknown8;
};
static_assert(sizeof(s_network_session_state_peer_leaving) == 0xC);

