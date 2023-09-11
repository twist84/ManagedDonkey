#include "network_observer.hpp"

c_network_observer::s_channel_observer const* c_network_observer::find_observer_by_channel(c_network_channel const* observer) const
{
    ASSERT(observer >= &m_channel_observers[0] && observer < &m_channel_observers[k_network_maximum_observers]);
    ASSERT(((byte*)observer - (byte*)&m_channel_observers[0]) % sizeof(s_channel_observer) == 0);

    return static_cast<c_network_observer::s_channel_observer const*>(observer);
}

long c_network_observer::observer_channel_find_by_network_channel(long owner_type, c_network_channel* channel) const
{
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
    INVOKE_CLASS_MEMBER(0x004474F0, c_network_observer::observer_channel_send_message, owner_type, observer_index, a3, message_type, data_size, data);
}

