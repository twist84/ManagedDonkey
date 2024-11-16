#include "networking/session/network_observer.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/messages/network_messages_connect.hpp"

c_network_observer::s_channel_observer const* c_network_observer::find_observer_by_channel(c_network_channel const* observer) const
{
    //ASSERT(observer >= &m_channel_observers[0] && observer < &m_channel_observers[k_network_maximum_observers]);
    ASSERT((void*)observer >= &m_channel_observers[0] && (void*)observer < &m_channel_observers[k_network_maximum_observers]);

    ASSERT(((byte*)observer - (byte*)&m_channel_observers[0]) % sizeof(s_channel_observer) == 0);

    return (c_network_observer::s_channel_observer const*)observer;
}

void c_network_observer::destroy_observer()
{
    INVOKE_CLASS_MEMBER(0x00446190, c_network_observer, destroy_observer);
}

//.text:004465D0 ; e_transport_secure_connection c_network_observer::get_secure_connection_status(long) const
//.text:00446610 ; bool c_network_observer::get_simple_status(long, long*, real*, long*, long*)

void c_network_observer::handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* connect_request)
{
    INVOKE_CLASS_MEMBER(0x004466A0, c_network_observer, handle_connect_request, incoming_address, connect_request);
}

//.text:00446A10 ; void c_network_observer::handle_packet_event(c_network_channel*, e_network_packet_event, long, long, long)

bool c_network_observer::initialize_observer(c_network_link* link, c_network_message_type_collection* message_types, c_network_message_gateway* message_gateway, c_network_message_handler* message_handler, s_observer_configuration const* configuration)
{
    return INVOKE_CLASS_MEMBER(0x00446C00, c_network_observer, initialize_observer, link, message_types, message_gateway, message_handler, configuration);

    //ASSERT(link);
    //ASSERT(message_types);
    //ASSERT(configuration);
    //ASSERT(m_link == NULL);
    //ASSERT(m_message_types == NULL);
    //ASSERT(m_configuration == NULL);
}

//.text:00446D90 ; bool c_network_observer::is_bandwidth_stable()
//.text:00446DA0 ; bool c_network_channel::is_empty() const

void c_network_observer::monitor()
{
    INVOKE_CLASS_MEMBER(0x00446DD0, c_network_observer, monitor);
}

//.text:00446ED0 ; 
//.text:00446EF0 ; void c_network_observer::notify_channel_packet_sent(c_network_channel*, long, bool, long, long, long)
//.text:00446F60 ; bool c_network_observer::observer_channel_backlogged(e_network_observer_owner, long, e_network_message_type) const
//.text:00446FE0 ; bool c_network_observer::observer_channel_currently_alive(e_network_observer_owner, long) const
//.text:00447070 ; long c_network_observer::observer_channel_find_by_machine_identifier(e_network_observer_owner, s_transport_unique_identifier const*) const
//.text:004470F0 ; long c_network_observer::observer_channel_find_by_network_address(e_network_observer_owner, s_transport_secure_address const*)

long c_network_observer::observer_channel_find_by_network_channel(long owner_type, c_network_channel* channel) const
{
    //return INVOKE_CLASS_MEMBER(0x00447150, c_network_observer, observer_channel_find_by_network_channel, owner_type, channel);

    //ASSERT(owner_type >= 0 && owner_type < k_network_observer_owner_count);
    ASSERT(channel != NULL);

    s_channel_observer const* observer = find_observer_by_channel(channel);
    ASSERT(observer != NULL);

    if (observer->state && TEST_BIT(observer->owner_flags, owner_type))
        return observer - m_channel_observers;

    return NONE;
}

//.text:004471A0 ; void c_network_observer::observer_channel_get_bandwidth_events(e_network_observer_owner, long, long, long*) const
//.text:004471E0 ; bool c_network_observer::observer_channel_get_last_receive_activity(e_network_observer_owner, long, dword*) const
//.text:00447220 ; c_network_channel* c_network_observer::observer_channel_get_network_channel(e_network_observer_owner, long)
//.text:00447240 ; c_network_observer::s_channel_observer const* c_network_observer::get_observer(e_network_observer_owner, long) const
//.text:00447260 ; void c_network_observer::observer_channel_get_secure_address(e_network_observer_owner, long, s_transport_secure_address*) const
//.text:00447290 ; 
//.text:004472E0 ; void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner, long)
//.text:00447340 ; 
//.text:004473A0 ; void c_network_observer::observer_channel_release(e_network_observer_owner, long)
//.text:004473D0 ; long c_network_observer::observer_channel_request(e_network_observer_owner, s_transport_secure_address const*)
//.text:004473E0 ; void c_network_observer::observer_channel_reset(e_network_observer_owner, long)
//.text:00447420 ; void c_network_observer::observer_channel_reset_security(e_network_observer_owner, long)
//.text:004474B0 ; bool c_network_observer::observer_channel_secure_connection_established(e_network_observer_owner, long) const

void c_network_observer::observer_channel_send_message(long owner_type, long observer_index, bool a3, e_network_message_type message_type, long data_size, void const* data)
{
    INVOKE_CLASS_MEMBER(0x004474F0, c_network_observer, observer_channel_send_message, owner_type, observer_index, a3, message_type, data_size, data);
}

//.text:00447660 ; void c_network_observer::observer_channel_set_observer_is_bad_client(e_network_observer_owner, long, bool)
//.text:00447690 ; 
//.text:004476C0 ; void c_network_observer::observer_channel_set_waiting_on_backlog(e_network_observer_owner, long, e_network_message_type)
//.text:00447740 ; 
//.text:00447760 ; bool c_network_observer::observer_desires_heartbeat(long) const

void c_network_observer::observer_prioritize_upload_bandwidth(bool prioritize_upload_bandwidth)
{
    //INVOKE_CLASS_MEMBER(0x004477E0, c_network_observer, observer_prioritize_upload_bandwidth, prioritize_upload_bandwidth);

    if (m_prioritize_upload_bandwidth != prioritize_upload_bandwidth)
    {
        m_prioritize_upload_bandwidth = prioritize_upload_bandwidth;
        __unknown23F00 = true;
        __unknown23F01 = true;
        generate_event(_event_message, "networking:observer:stream: rebalance stream for upload bandwidth prioritzation=%d", prioritize_upload_bandwidth);
    }
}

//.text:00447820 ; 
//.text:004478E0 ; 
//.text:00447AA0 ; 
//.text:00447B10 ; bool c_network_observer::quality_statistics_are_set() const
//.text:00447B20 ; c_network_oberserver::quality_statistics_report_badness
//.text:00447B60 ; s_network_quality_statistics* c_network_observer::quality_statistics_get()
//.text:00447B70 ; void c_network_observer::quality_statistics_get_ratings(long*, long*, long*)
//.text:00447CC0 ; void c_network_observer::quality_statistics_notify_established_connectivity(e_network_observer_owner, long, bool)
//.text:00447D90 ; void c_network_observer::quality_statistics_notify_peer_left_gracefully(e_network_observer_owner, long)
//.text:00447DE0 ; void c_network_observer::quality_statistics_report_badness(long, bool, char const*)
//.text:00447F60 ; void c_network_observer::quality_statistics_set(s_network_quality_statistics const*)
//.text:00447F90 ; 
//.text:00447FE0 ; void c_network_observer::record_throughput_bandwidth(long, long, long, long)
//.text:004481C0 ; void c_network_observer::recreate_channels()
//.text:00448250 ; void c_network_observer::register_owner(e_network_observer_owner, c_network_channel_owner*)
//.text:00448270 ; void c_network_observer::release_observer(long)
//.text:00448370 ; void c_network_observer::reset_bandwidth()
//.text:004484E0 ; 
//.text:004484F0 ; void c_network_channel::set_connection_packet_timeout(long)
//.text:00448500 ; void c_network_observer::set_estimated_bandwidth(long, bool, bool)

void c_network_observer::set_online_network_environment(bool online_network_environment)
{
    //INVOKE_CLASS_MEMBER(0x00448590, c_network_observer, set_online_network_environment, online_network_environment);

    m_online_network_environment = online_network_environment;
}

//.text:004485A0 ; void c_network_connection::set_packet_timeout(long)
//.text:004485B0 ; bool c_network_channel::simulation_attached() const
//.text:004485C0 ; bool c_network_channel::simulation_is_authority() const
//.text:004485E0 ; c_network_observer::stream_balance_all_stream_bandwidth
//.text:00448D40 ; c_network_observer::stream_set_bandwidth
//.text:00448F00 ; bool c_network_observer::stream_check_stream_channel_connections(bool*)
//.text:004492D0 ; void c_network_observer::stream_dispose(long)
//.text:00449330 ; long c_network_observer::stream_get_current_latency_threshold(long, bool)
//.text:00449370 ; c_network_observer::stream_set_bandwidth
//.text:004495F0 ; 
//.text:00449610 ; 
//.text:00449650 ; 
//.text:004496C0 ; 
//.text:00449770 ; void c_network_observer::stream_update()
//.text:0044BE90 ; void c_network_observer::update_observer_connection_initiation(long)
//.text:0044C2A0 ; void c_network_observer::update_observer_connection_state(long)
//.text:0044C540 ; bool c_network_observer::update_observer_existence(long)
//.text:0044C600 ; void c_network_observer::update_observer_qos(long)
//.text:0044C830 ; void c_network_observer::update_observer_secure_state(long)
//.text:0044C980 ; void c_network_observer::update_observer_timeout(long)
//.text:0044CAE0 ; void c_network_observer::update_owner(e_network_observer_owner, long)
//.text:0044CB00 ; bool c_network_observer::update_undesired_connection_timers(long, e_network_channel_closure_reason)

