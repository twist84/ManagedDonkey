#include "networking/messages/network_messages_simulation_synchronous.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"

HOOK_DECLARE(0x004E0D70, network_message_types_register_simulation_synchronous);

bool __cdecl c_network_message_synchronous_actions::compare(long a1, void* a2, void* a3)
{
	return INVOKE(0x004E0490, compare, a1, a2, a3);
}

bool __cdecl c_network_message_synchronous_update::compare(long a1, void* a2, void* a3)
{
	return INVOKE(0x004E0500, compare, a1, a2, a3);
}

bool __cdecl c_network_message_synchronous_acknowledge::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E05B0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_synchronous_actions::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E05E0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_synchronous_gamestate::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0690, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_synchronous_playback_control::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0730, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_synchronous_update::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004E07A0, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_synchronous_actions::dispose(long a1, void* a2)
{
	INVOKE(0x004E0870, dispose, a1, a2);
}

void __cdecl c_network_message_synchronous_update::dispose(long a1, void* a2)
{
	INVOKE(0x004E0880, dispose, a1, a2);
}

void __cdecl c_network_message_synchronous_acknowledge::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0890, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_synchronous_actions::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E08E0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_synchronous_gamestate::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0A10, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_synchronous_playback_control::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0B40, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_synchronous_update::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0C60, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_simulation_synchronous(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004E0D70, network_message_types_register_simulation_synchronous, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_synchronous_update,
		"synchronous-update",
		0,
		sizeof(s_network_message_synchronous_update),
		sizeof(s_network_message_synchronous_update),
		c_network_message_synchronous_update::encode,
		c_network_message_synchronous_update::decode,
		c_network_message_synchronous_update::compare,
		c_network_message_synchronous_update::dispose);

	message_collection->register_message_type(
		_network_message_synchronous_playback_control,
		"synchronous-playback-control",
		0,
		sizeof(s_network_message_synchronous_playback_control),
		sizeof(s_network_message_synchronous_playback_control),
		c_network_message_synchronous_playback_control::encode,
		c_network_message_synchronous_playback_control::decode,
		0,
		0);

	message_collection->register_message_type(
		_network_message_synchronous_actions,
		"synchronous-actions",
		0,
		sizeof(s_network_message_synchronous_actions),
		sizeof(s_network_message_synchronous_actions),
		c_network_message_synchronous_actions::encode,
		c_network_message_synchronous_actions::decode,
		c_network_message_synchronous_actions::compare,
		c_network_message_synchronous_actions::dispose);

	message_collection->register_message_type(
		_network_message_synchronous_acknowledge,
		"synchronous-acknowledge",
		0,
		sizeof(s_network_message_synchronous_acknowledge),
		sizeof(s_network_message_synchronous_acknowledge),
		c_network_message_synchronous_acknowledge::encode,
		c_network_message_synchronous_acknowledge::decode,
		0,
		0);

	message_collection->register_message_type(
		_network_message_synchronous_gamestate,
		"synchronous-gamestate",
		FLAG(0),
		sizeof(s_network_message_synchronous_gamestate),
		k_network_message_maximum_size,
		c_network_message_synchronous_gamestate::encode,
		c_network_message_synchronous_gamestate::decode,
		0,
		0);

	message_collection->register_message_type(
		_network_message_synchronous_client_ready,
		"synchronous-client-ready",
		FLAG(1),
		0,
		0,
		nullptr,
		nullptr,
		nullptr,
		nullptr);
}

