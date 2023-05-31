#include "networking/messages/network_messages_session_parameters.hpp"

#include "networking/messages/network_message_type_collection.hpp"

void __cdecl c_network_message_parameters_update::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_parameters_update::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_parameters_request::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_parameters_request::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl network_message_types_register_session_parameters(c_network_message_type_collection* message_collection)
{
	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_parameters_update,
		"parameters-update",
		0,
		sizeof(s_network_message_parameters_update),
		sizeof(s_network_message_parameters_update),
		c_network_message_parameters_update::encode,
		c_network_message_parameters_update::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_parameters_request,
		"parameters-request",
		0,
		sizeof(s_network_message_parameters_request),
		sizeof(s_network_message_parameters_request),
		c_network_message_parameters_request::encode,
		c_network_message_parameters_request::decode,
		nullptr,
		nullptr);
}

