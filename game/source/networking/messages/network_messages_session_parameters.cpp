#include "networking/messages/network_messages_session_parameters.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"

HOOK_DECLARE(0x004E0030, network_message_types_register_session_parameters);

bool __cdecl c_network_message_parameters_request::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DFFB0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_parameters_update::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return INVOKE(0x004DFFD0, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_parameters_request::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004DFFF0, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_parameters_update::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
	INVOKE(0x004E0010, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_session_parameters(c_network_message_type_collection* message_collection)
{
	//INVOKE(0x004E0030, network_message_types_register_session_parameters, message_collection);

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

