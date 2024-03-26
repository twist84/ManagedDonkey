#include "network_observer.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/messages/network_messages_connect.hpp"

c_network_observer::s_channel_observer const* c_network_observer::find_observer_by_channel(c_network_channel const* observer) const
{
    ASSERT(observer >= &m_channel_observers[0] && observer < &m_channel_observers[k_network_maximum_observers]);
    ASSERT(((byte*)observer - (byte*)&m_channel_observers[0]) % sizeof(s_channel_observer) == 0);

    return static_cast<c_network_observer::s_channel_observer const*>(observer);
}

void c_network_observer::handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* connect_request)
{
    DECLFUNC(0x004466A0, long, __thiscall, c_network_observer*, transport_address const*, s_network_message_connect_request const*)(this, incoming_address, connect_request);
}

long c_network_observer::observer_channel_find_by_network_channel(long owner_type, c_network_channel* channel) const
{
    //return DECLFUNC(0x00447150, long, __thiscall, c_network_observer const*, long, c_network_channel*)(this, owner_type, channel);

    //ASSERT(owner_type >= 0 && owner_type < k_network_observer_owner_count);
    ASSERT(channel != NULL);

    s_channel_observer const* observer = find_observer_by_channel(channel);
    ASSERT(observer != NULL);

    if (observer->state && TEST_BIT(observer->owner_flags, owner_type))
        return observer - m_channel_observers;

    return NONE;
}

void c_network_observer::observer_channel_send_message(long owner_type, long observer_index, bool a3, e_network_message_type message_type, long data_size, void const* data)
{
    DECLFUNC(0x004474F0, void, __thiscall, c_network_observer*, long, long, bool, e_network_message_type, long, void const*)(this, owner_type, observer_index, a3, message_type, data_size, data);
}

void c_network_observer::observer_prioritize_upload_bandwidth(bool prioritize_upload_bandwidth)
{
    //DECLFUNC(0x004477E0, void, __thiscall, c_network_observer*, bool)(this, prioritize_upload_bandwidth);

    if (m_prioritize_upload_bandwidth != prioritize_upload_bandwidth)
    {
        m_prioritize_upload_bandwidth = prioritize_upload_bandwidth;
        __unknown23F00 = true;
        __unknown23F01 = true;
        generate_event(_event_level_message, "networking:observer:stream: rebalance stream for upload bandwidth prioritzation=%d", prioritize_upload_bandwidth);
    }
}

