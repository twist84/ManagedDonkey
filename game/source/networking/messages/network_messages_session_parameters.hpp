#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_base.hpp"
#include "networking/transport/transport_security.hpp"

int32 const k_network_session_parameters_update_payload_size = 0x20000;
int32 const k_network_session_parameters_request_payload_size = 0x20000;

struct s_network_message_parameters_update
{
	s_transport_secure_identifier session_id;
	bool initial_update;
	uint64 cleared_parameters;
	uint64 updated_parameters;
	uint8 payload[k_network_session_parameters_update_payload_size];
};
static_assert(sizeof(s_network_message_parameters_update) == 0x20028);

struct s_network_message_parameters_request
{
	s_transport_secure_identifier session_id;
	uint64 change_request_parameters;
	uint8 payload[k_network_session_parameters_request_payload_size];
};
static_assert(sizeof(s_network_message_parameters_request) == 0x20018);

struct c_bitstream;

class c_network_message_parameters_update
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_parameters_request
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_session_parameters(c_network_message_type_collection* message_collection);

