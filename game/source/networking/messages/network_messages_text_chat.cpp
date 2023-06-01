#include "networking/messages/network_messages_text_chat.hpp"

#include "cseries/cseries.hpp"
#include "main/console.hpp"
#include "memory/bitstream.hpp"
#include "networking/messages/network_message_type_collection.hpp"

bool c_network_message_text_chat::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	ASSERT(message_storage_size == sizeof(s_network_message_text_chat));

	s_network_message_text_chat* message = static_cast<s_network_message_text_chat*>(message_storage);

	packet->read_raw_data("session-id", &message->session_id, 128);

	message->routed_players = packet->read_integer("routed-players", 32);
	message->payload.metadata = packet->read_integer("metadata", 8);

	message->payload.source_is_server = packet->read_bool("source-is-server");
	if (!message->payload.source_is_server)
		packet->read_secure_address("source-player", &message->payload.source_player);

	message->payload.destination_player_count = packet->read_integer("destination-player-count", 8);
	if (!VALID_COUNT(message->payload.destination_player_count, 16))
		return false;

	for (long i = 0; i < message->payload.destination_player_count; i++)
	{
		s_transport_secure_address* destination_player = &message->payload.destination_players[i];
		packet->read_secure_address("destination-player", destination_player);
	}

	packet->read_string_wchar("text", message->payload.text_buffer, NUMBEROF(message->payload.text_buffer));

	if (!message->payload.text.is_empty())
		console_printf_color(global_real_argb_grey, "%s: %ls", "TEST", message->payload.text_buffer);

	return !packet->error_occurred();
}

void c_network_message_text_chat::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	ASSERT(message_storage_size == sizeof(s_network_message_text_chat));

	s_network_message_text_chat const* message = static_cast<s_network_message_text_chat const*>(message_storage);

	packet->write_raw_data("session-id", &message->session_id, 128);

	packet->write_integer("routed-players", message->routed_players, 32);
	packet->write_integer("metadata", message->payload.metadata, 8);

	packet->write_bool("source-is-server", message->payload.source_is_server);
	if (!message->payload.source_is_server)
		packet->write_secure_address("source-player", &message->payload.source_player);

	packet->write_integer("destination-player-count", message->payload.destination_player_count, 8);
	for (long i = 0; i < message->payload.destination_player_count; i++)
	{
		s_transport_secure_address const* destination_player = &message->payload.destination_players[i];
		packet->write_secure_address("destination-player", destination_player);
	}

	packet->write_string_wchar("text", message->payload.text_buffer, NUMBEROF(message->payload.text_buffer));
}

void __cdecl network_message_types_register_text_chat(c_network_message_type_collection* message_collection)
{
	ASSERT(message_collection);

	message_collection->register_message_type(_custom_network_message_text_chat, "text chat", 0, sizeof(s_network_message_text_chat), sizeof(s_network_message_text_chat), c_network_message_text_chat::encode, c_network_message_text_chat::decode, nullptr, nullptr);
}

