#pragma once

#include "cseries/cseries.hpp"

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
	uns32 channel_identifier;
	uns32 flags;
};
COMPILE_ASSERT(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse
{
	uns32 remote_identifier;
	int32 reason;
};
COMPILE_ASSERT(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish
{
	uns32 identifier;
	uns32 remote_identifier;
};
COMPILE_ASSERT(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed
{
	uns32 remote_identifier;
	uns32 identifier;
	c_enum<e_network_channel_closure_reason, int32, _network_channel_reason_none, k_network_channel_reason_count> reason;
};
COMPILE_ASSERT(sizeof(s_network_message_connect_closed) == 0xC);

class c_bitstream;

class c_network_message_connect_request
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_connect_refuse
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_connect_establish
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_connect_closed
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

extern const char* const k_connect_refuse_reason_strings[k_network_connect_refuse_reason_count];

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_connect(c_network_message_type_collection* message_collection);
extern const char* network_message_connect_refuse_get_reason_string(int32 reason);

