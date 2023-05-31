#include "networking/messages/network_messages_test.hpp"

#include "networking/messages/network_message_type_collection.hpp"

void __cdecl c_network_message_test::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_test::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl network_message_types_register_test(c_network_message_type_collection* message_collection)
{
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

