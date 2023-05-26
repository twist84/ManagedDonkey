#pragma once

#include "networking/transport/transport_security.hpp"
#include "text/unicode.hpp"

struct s_network_message_text_chat
{
	s_transport_secure_identifier session_id;
	long routed_players;
	long metadata;

	bool source_is_server;
	s_transport_secure_address source_player;

	s_transport_secure_address destination_players[16];
	long destination_player_count;

	union
	{
		wchar_t text_buffer[122];
		c_static_wchar_string<122> text;
	};
};
static_assert(sizeof(s_network_message_text_chat) == 0x224 /* 0x194 */);

struct c_bitstream;
class c_network_message_text_chat
{
public:
	static void encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_text_chat(c_network_message_type_collection* message_collection);

