#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/transport/transport_security.hpp"
#include "shell/shell.hpp"

#define k_network_protocol_version 9

struct s_network_message_join_request
{
	word protocol;
	word executable_type;
	dword minimum_network_version;
	dword current_network_version;
	s_transport_secure_identifier session_id;
	s_network_session_join_request join_request;
};
static_assert(sizeof(s_network_message_join_request) == 0x330);

struct s_network_message_peer_connect
{
	word protocol;
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_peer_connect) == 0x20);

struct s_network_message_join_abort
{
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_join_abort) == 0x18);

struct s_network_message_join_refuse
{
	s_transport_secure_identifier session_id;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> reason;
};
static_assert(sizeof(s_network_message_join_refuse) == 0x14);

struct s_network_message_leave_session
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_session) == 0x10);

struct s_network_message_leave_acknowledge
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_acknowledge) == 0x10);

struct s_network_message_session_disband
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_session_disband) == 0x10);

struct s_network_message_session_boot
{
	s_transport_secure_identifier session_id;
	c_enum<e_network_session_boot_reason, long, _network_session_boot_reason_player_booted_player, k_network_session_boot_reason_count> reason;
};
static_assert(sizeof(s_network_message_session_boot) == 0x14);

struct s_network_message_host_decline
{
	s_transport_secure_identifier session_id;
	bool session_exists;
	bool peer_exists;
	bool host_exists;
	byte __pad13[0x1];
	s_transport_secure_address host_address;
};
static_assert(sizeof(s_network_message_host_decline) == 0x24);

struct s_network_message_peer_establish
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_peer_establish) == 0x10);

struct s_network_message_time_synchronize
{
	s_transport_secure_identifier session_id;
	dword client_timestamp[2];
	dword authority_timestamp[2];
	dword synchronization_stage;
};
static_assert(sizeof(s_network_message_time_synchronize) == 0x24);

struct c_bitstream;

class c_network_message_join_request
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_peer_connect
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_join_abort
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_join_refuse
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_leave_session
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_leave_acknowledge
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_session_disband
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_session_boot
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_host_decline
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_peer_establish
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_time_synchronize
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
	static bool __cdecl compare(long, void*, void*);
	static void __cdecl dispose(long, void*);
};

extern char const* network_message_join_refuse_get_reason_string(long reason);

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_session_protocol(c_network_message_type_collection* message_collection);

