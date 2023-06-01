#include "networking/messages/network_messages_simulation.hpp"

#include "networking/messages/network_message_type_collection.hpp"

bool __cdecl c_network_message_player_acknowledge::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0090, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_view_establishment::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0110, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_player_acknowledge::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E01C0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_view_establishment::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0290, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_simulation(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004E0410, network_message_types_register_simulation, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_view_establishment,
		"view-establishment",
		0,
		sizeof(s_network_message_view_establishment),
		sizeof(s_network_message_view_establishment),
		c_network_message_view_establishment::encode,
		c_network_message_view_establishment::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_acknowledge,
		"player-acknowledge",
		0,
		sizeof(s_network_message_player_acknowledge),
		sizeof(s_network_message_player_acknowledge),
		c_network_message_player_acknowledge::encode,
		c_network_message_player_acknowledge::decode,
		nullptr,
		nullptr);
}

