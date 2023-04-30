#include "networking/delivery/network_link.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"
#include "networking/network_memory.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"

REFERENCE_DECLARE(0x0224A494, c_network_link*, g_network_link);

bool c_network_link::create_endpoint(e_transport_type type, word port, bool set_broadcast_option, transport_endpoint** out_endpoint)
{
    return DECLFUNC(0x0043B6F0, bool, __cdecl, e_transport_type, word, bool, transport_endpoint**)(type, port, set_broadcast_option, out_endpoint);

    //transport_endpoint* endpoint = transport_endpoint_create(type);
    //if (!endpoint)
    //{
    //    c_console::write_line("networking:link: create_endpoint[%d] - unable to create endpoint!", type);
    //    return false;
    //}
    //
    //transport_address address;
    //transport_get_listen_address(&address, port);
    //
    //if (!transport_endpoint_bind(endpoint, &address) || !transport_endpoint_set_blocking(endpoint, false) || (set_broadcast_option && !transport_endpoint_set_option_value(endpoint, _transport_endpoint_option_broadcast, true)))
    //{
    //    c_console::write_line("networking:link: create_endpoint(%d) - unable to setup endpoint!", type);
    //    transport_endpoint_delete(endpoint);
    //    return false;
    //}
    //
    //*out_endpoint = endpoint;
    //return true;
}

bool __cdecl c_network_link::create_endpoints()
{
    return DECLFUNC(0x0043B7C0, bool, __thiscall, c_network_link*)(this);
    
    // Halo 3
    //if (create_endpoint(_transport_type_vdp, 1000, false, &m_endpoints[0]) && create_endpoint(_transport_type_udp, 1001, true, &m_endpoints[1]))
    //{
    //    m_endpoints_created = true;
    //}
    //else
    //{
    //    destroy_endpoints();
    //}
    //return m_endpoints_created;

    // Halo Online
    //bool result = false;
    //
    //word broadcast_port = k_broadcast_port;
    //while (true)
    //{
    //    g_broadcast_port = broadcast_port;
    //
    //    result = create_endpoint(_transport_type_udp, broadcast_port, true, &m_endpoint);
    //    if (result)
    //        break;
    //
    //    destroy_endpoints();
    //
    //    if (++broadcast_port - k_broadcast_port >= 1000)
    //    {
    //        g_broadcast_port = k_broadcast_port;
    //        return result;
    //    }
    //}
    //
    //return result;
}

bool c_network_link::decode_packet(long data_buffer_size, byte const* data_buffer, s_link_packet* packet) const
{
    return DECLFUNC(0x0043B820, bool, __thiscall, c_network_link const*, long, byte const*, s_link_packet*)(this, data_buffer_size, data_buffer, packet);
}

void __cdecl c_network_link::destroy_endpoints()
{
    //DECLFUNC(0x0043B940, void, __thiscall, c_network_link*)(this);

    if (m_endpoint)
    {
        transport_endpoint_delete(m_endpoint);
        m_endpoint = 0;
    }
}

bool __cdecl c_network_link::physical_link_available()
{
    //INVOKE(0x0043BBB0, c_network_link::physical_link_available);

    return transport_available();
}

