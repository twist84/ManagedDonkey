#include "networking/messages/network_messages_simulation_distributed.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"

HOOK_DECLARE(0x004E0EF0, network_message_types_register_simulation_distributed);

bool __cdecl c_network_message_distributed_game_results::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0E70, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_distributed_game_results::encode(c_bitstream* packet, int32 message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0EB0, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_simulation_distributed(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004E0EF0, network_message_types_register_simulation_distributed, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_distributed_game_results,
		"game-results",
		0,
		sizeof(s_network_message_distributed_game_results),
		sizeof(s_network_message_distributed_game_results),
		c_network_message_distributed_game_results::encode,
		c_network_message_distributed_game_results::decode,
		nullptr,
		nullptr);
}

