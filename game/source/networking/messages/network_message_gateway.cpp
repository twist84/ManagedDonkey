#include "networking/network_memory.hpp"

bool __cdecl c_network_message_gateway::send_message_broadcast(e_network_message_type message_type, long data_size, void const* data, word port)
{
	return DECLFUNC(0x00484080, bool, __thiscall, c_network_message_gateway*, e_network_message_type, long, void const*, word)(this, message_type, data_size, data, port);
}

