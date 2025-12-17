#include "networking/messages/network_messages_out_of_band.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_protocol.hpp" // k_network_protocol_version
#include "networking/network_game_definitions.hpp"

HOOK_DECLARE(0x004DC3D0, network_message_types_register_out_of_band);

bool __cdecl c_network_message_directed_search::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	return c_network_message_broadcast_search::decode(packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_broadcast_reply::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	//return INVOKE(0x004DC000, decode, packet, message_storage_size, message_storage);

	s_network_message_broadcast_reply* message = (s_network_message_broadcast_reply*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));

	message->protocol_version = (uns16)packet->read_integer("protocol", 16);
	packet->read_raw_data("search-nonce", &message->search_nonce, 64);

	return squad_status_decode(packet, &message->status_data) && !packet->error_occurred();
}

bool __cdecl c_network_message_broadcast_search::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	//return INVOKE(0x004DC050, decode, packet, message_storage_size, message_storage);

	s_network_message_broadcast_search* message = (s_network_message_broadcast_search*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));

	message->protocol_version = (uns16)packet->read_integer("protocol", 16);
	packet->read_raw_data("nonce", &message->nonce, 64);

	return !packet->error_occurred();
}

bool __cdecl c_network_message_ping::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	//return INVOKE(0x004DC090, decode, packet, message_storage_size, message_storage);

	s_network_message_ping* message = (s_network_message_ping*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_ping));

	message->unique_identifier = (uns16)packet->read_integer("id", 16);
	message->origin_timestamp = packet->read_integer("timestamp", 32);
	message->request_permission_to_qos = packet->read_bool("request_qos");

	return !packet->error_occurred();
}

bool __cdecl c_network_message_pong::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	//return INVOKE(0x004DC0E0, decode, packet, message_storage_size, message_storage);

	s_network_message_pong* message = (s_network_message_pong*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_pong));

	message->unique_identifier = (uns16)packet->read_integer("id", 16);
	message->origin_timestamp = packet->read_integer("timestamp", 32);
	message->qos_response = (e_network_qos_response)packet->read_integer("qos_response", k_network_qos_response_bits);

	return !packet->error_occurred() && VALID_INDEX(message->qos_response, k_network_qos_response_count);
}

void __cdecl c_network_message_directed_search::encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage)
{
	c_network_message_broadcast_search::encode(packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_broadcast_reply::encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage)
{
	//INVOKE(0x004DC140, encode, packet, message_storage_size, message_storage);

	const s_network_message_broadcast_reply* message = (const s_network_message_broadcast_reply*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
	ASSERT(message->protocol_version == k_network_protocol_version);

	packet->write_integer("protocol", message->protocol_version, 16);
	packet->write_raw_data("search-nonce", &message->search_nonce, 64);
	squad_status_encode(packet, &message->status_data);
}

void __cdecl c_network_message_broadcast_search::encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage)
{
	//INVOKE(0x004DC1B0, encode, packet, message_storage_size, message_storage);

	const s_network_message_broadcast_search* message = (const s_network_message_broadcast_search*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
	ASSERT(message->protocol_version == k_network_protocol_version);

	packet->write_integer("protocol", message->protocol_version, 16);
	packet->write_raw_data("nonce", &message->nonce, 64);
}

void __cdecl c_network_message_ping::encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage)
{
	//INVOKE(0x004DC220, encode, packet, message_storage_size, message_storage);

	const s_network_message_ping* message = (const s_network_message_ping*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_ping));

	packet->write_integer("id", message->unique_identifier, 16);
	packet->write_integer("timestamp", message->origin_timestamp, 32);
	packet->write_bool("request_qos", message->request_permission_to_qos);
}

void __cdecl c_network_message_pong::encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage)
{
	//INVOKE(0x004DC300, encode, packet, message_storage_size, message_storage);

	const s_network_message_pong* message = (const s_network_message_pong*)message_storage;

	ASSERT(message_storage_size == sizeof(s_network_message_pong));

	packet->write_integer("id", message->unique_identifier, 16);
	packet->write_integer("timestamp", message->origin_timestamp, 32);
	packet->write_integer("qos_response", message->qos_response, k_network_qos_response_bits);
}

void __cdecl network_message_types_register_out_of_band(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004DC3D0, network_message_types_register_out_of_band, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_ping,
		"ping",
		0,
		sizeof(s_network_message_ping),
		sizeof(s_network_message_ping),
		c_network_message_ping::encode,
		c_network_message_ping::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_pong,
		"pong",
		0,
		sizeof(s_network_message_pong),
		sizeof(s_network_message_pong),
		c_network_message_pong::encode,
		c_network_message_pong::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_broadcast_search,
		"broadcast-search",
		0,
		sizeof(s_network_message_broadcast_search),
		sizeof(s_network_message_broadcast_search),
		c_network_message_broadcast_search::encode,
		c_network_message_broadcast_search::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_custom_network_message_directed_search,
		"directed-search",
		0,
		sizeof(s_network_message_directed_search),
		sizeof(s_network_message_directed_search),
		c_network_message_directed_search::encode,
		c_network_message_directed_search::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_broadcast_reply,
		"broadcast-reply",
		0,
		sizeof(s_network_message_broadcast_reply),
		sizeof(s_network_message_broadcast_reply),
		c_network_message_broadcast_reply::encode,
		c_network_message_broadcast_reply::decode,
		nullptr,
		nullptr);
}

