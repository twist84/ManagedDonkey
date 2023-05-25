#include "networking/messages/network_message_handler.hpp"

void __cdecl c_network_message_handler::handle_out_of_band_message(transport_address const* incoming_address, e_network_message_type message_typ, long message_storage_size, void const* message_storage)
{
	DECLFUNC(0x0049D2C0, void, __thiscall, c_network_message_handler*, transport_address const*, e_network_message_type, long, void const*)(this, incoming_address, message_typ, message_storage_size, message_storage);
}
