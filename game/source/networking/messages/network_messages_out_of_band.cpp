#include "networking/messages/network_messages_out_of_band.hpp"

#include "memory/module.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_protocol.hpp" // k_network_protocol_version

void c_network_message_broadcast_search::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    s_network_message_broadcast_search const* message = static_cast<s_network_message_broadcast_search const*>(message_storage);

    ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
    ASSERT(message->protocol_version == k_network_protocol_version);

    INVOKE(0x004DC1B0, c_network_message_broadcast_search::encode, packet, message_storage_size, message_storage);
}

bool c_network_message_broadcast_search::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));

    return INVOKE(0x004DC050, c_network_message_broadcast_search::decode, packet, message_storage_size, message_storage);
}

void c_network_message_directed_search::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    c_network_message_broadcast_search::encode(packet, message_storage_size, message_storage);
}

bool c_network_message_directed_search::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    return c_network_message_broadcast_search::decode(packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_out_of_band(c_network_message_type_collection* message_collection)
{
    ASSERT(message_collection);

    INVOKE(0x004DC3D0, network_message_types_register_out_of_band, message_collection);

    message_collection->register_message_type(_custom_network_message_directed_search, "directed-search", 0, sizeof(s_network_message_directed_search), sizeof(s_network_message_directed_search), c_network_message_directed_search::encode, c_network_message_directed_search::decode, nullptr, nullptr);
}
HOOK_DECLARE_CALL(0x0049E231, network_message_types_register_out_of_band);

