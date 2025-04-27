#include "networking/messages/network_messages_test.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"

//HOOK_DECLARE(0x004E1000, network_message_types_register_test);

bool __cdecl c_network_message_test::decode(c_bitstream* packet, int32 message_storage_size, void* message_storage)
{
	return INVOKE(0x004E0F20, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_test::encode(c_bitstream* packet, int32 message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0F60, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_test(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004E1000, network_message_types_register_test, message_collection);

	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_test,
		"test",
		0,
		sizeof(s_network_message_test),
		sizeof(s_network_message_test),
		c_network_message_test::encode,
		c_network_message_test::decode,
		nullptr,
		nullptr);
}

