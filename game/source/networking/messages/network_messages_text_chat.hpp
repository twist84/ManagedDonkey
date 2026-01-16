#pragma once

#include "networking/transport/transport_security.hpp"
#include "text/unicode.hpp"

struct s_network_message_text_chat_payload
{
	// _text_chat_metadata_broadcast = 1
	int32 metadata;

	bool source_is_server;
	s_transport_secure_address source_player;

	c_static_array<s_transport_secure_address, 16> destination_players;
	int32 destination_player_count;

	union
	{
		wchar_t text_buffer[122];
		c_static_wchar_string<122> text;
	};
};
COMPILE_ASSERT(sizeof(s_network_message_text_chat_payload) == 0x210 /* 0x188 */);

struct s_network_message_text_chat
{
	s_transport_secure_identifier session_id;
	int32 routed_players;
	s_network_message_text_chat_payload payload;
};
COMPILE_ASSERT(sizeof(s_network_message_text_chat) == 0x224 /* 0x194 */);

class c_bitstream;
class c_network_message_text_chat
{
public:
	static void encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_text_chat(c_network_message_type_collection* message_collection);

