#include "networking/messages/network_message_queue.hpp"

#include "networking/delivery/network_channel.hpp"
#include "networking/messages/network_message_type_collection.hpp"

void c_network_message_queue::send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload)
{
	DECLFUNC(0x0047D5C0, void, __thiscall, c_network_message_queue*, e_network_message_type, long, void const*)(this, message_type, raw_message_size, raw_message_payload);
}

bool c_network_message_queue::has_channel_been_used() const
{
	return m_has_channel_been_used;
}

