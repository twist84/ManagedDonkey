#include "networking/messages/network_messages_connect.hpp"

#include "networking/messages/network_message_type_collection.hpp"

bool __cdecl c_network_message_connect_closed::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    return INVOKE(0x004DC470, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_connect_establish::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    return INVOKE(0x004DC4D0, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_connect_refuse::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    return INVOKE(0x004DC510, decode, packet, message_storage_size, message_storage);
}

bool __cdecl c_network_message_connect_request::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
    return INVOKE(0x004DC550, decode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_connect_closed::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    INVOKE(0x004DC590, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_connect_establish::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    INVOKE(0x004DC670, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_connect_refuse::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    INVOKE(0x004DC710, encode, packet, message_storage_size, message_storage);
}

void __cdecl c_network_message_connect_request::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
    INVOKE(0x004DC7B0, encode, packet, message_storage_size, message_storage);
}

void __cdecl network_message_types_register_connect(c_network_message_type_collection* message_collection)
{
    //INVOKE(0x004DC850, network_message_types_register_connect, message_collection);

    ASSERT(message_collection);

    message_collection->register_message_type(
        _network_message_connect_request,
        "connect-request",
        0,
        sizeof(s_network_message_connect_request),
        sizeof(s_network_message_connect_request),
        c_network_message_connect_request::encode,
        c_network_message_connect_request::decode,
        nullptr,
        nullptr);

    message_collection->register_message_type(
        _network_message_connect_refuse,
        "connect-refuse",
        0,
        sizeof(s_network_message_connect_refuse),
        sizeof(s_network_message_connect_refuse),
        c_network_message_connect_refuse::encode,
        c_network_message_connect_refuse::decode,
        nullptr,
        nullptr);

    message_collection->register_message_type(
        _network_message_connect_establish,
        "connect-establish",
        0,
        sizeof(s_network_message_connect_establish),
        sizeof(s_network_message_connect_establish),
        c_network_message_connect_establish::encode,
        c_network_message_connect_establish::decode,
        nullptr,
        nullptr);

    message_collection->register_message_type(
        _network_message_connect_closed,
        "connect-closed",
        0,
        sizeof(s_network_message_connect_closed),
        sizeof(s_network_message_connect_closed),
        c_network_message_connect_closed::encode,
        c_network_message_connect_closed::decode,
        nullptr,
        nullptr);
}

