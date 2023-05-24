#include "networking/network_memory.hpp"

bool __cdecl c_network_message_gateway::send_message_broadcast(e_network_message_type message_type, long data_size, void const* data, word port)
{
	//return DECLFUNC(0x00484080, bool, __thiscall, c_network_message_gateway*, e_network_message_type, long, void const*, word)(this, message_type, data_size, data, port);

	transport_address outgoing_address{};
	transport_get_broadcast_address(&outgoing_address, port);
	return send_message_directed(&outgoing_address, message_type, data_size, data);
}

bool c_network_message_gateway::send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long data_size, void const* data)
{
	return DECLFUNC(0x004840C0, bool, __thiscall, c_network_message_gateway*, transport_address const*, e_network_message_type, long, void const*)(this, outgoing_address, message_type, data_size, data);
}

