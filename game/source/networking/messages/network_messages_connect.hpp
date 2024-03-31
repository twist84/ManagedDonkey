#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

enum e_network_connect_refuse_reason
{
	_network_connect_refuse_reason_none = 0,
	_network_connect_refuse_reason_invalid_flags,
	_network_connect_refuse_reason_old_identifier,
	_network_connect_refuse_reason_unknown_stranger,
	_network_connect_refuse_reason_security_failed,
	_network_connect_refuse_reason_too_many_channels,
	_network_connect_refuse_reason_session_failed_to_add,
	_network_connect_refuse_reason_invalid_topology,

	k_network_connect_refuse_reason_count
};

struct s_network_message_connect_request
{
	dword channel_identifier;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse
{
	dword remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish
{
	dword identifier;
	dword remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed
{
	dword remote_identifier;
	dword identifier;
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);

struct c_bitstream;

class c_network_message_connect_request
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_connect_refuse
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_connect_establish
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_connect_closed
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

extern char const* const k_connect_refuse_reason_strings[k_network_connect_refuse_reason_count];

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_connect(c_network_message_type_collection* message_collection);
extern char const* network_message_connect_refuse_get_reason_string(long reason);

