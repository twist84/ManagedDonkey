#include "networking/messages/network_message_queue.hpp"

#include "networking/delivery/network_channel.hpp"
#include "networking/messages/network_message_type_collection.hpp"

bool c_network_message_queue::has_channel_been_used() const
{
	return m_second_fragment_sent;
}

void c_network_message_queue::send_message(e_network_message_type message_type, int32 raw_message_size, const void* raw_message_payload)
{
	INVOKE_CLASS_MEMBER(0x0047D5C0, c_network_message_queue, send_message, message_type, raw_message_size, raw_message_payload);
}

