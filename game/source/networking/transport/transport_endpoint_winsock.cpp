#include "networking/transport/transport_endpoint_winsock.hpp"

#include "cseries/console.hpp"

long __cdecl get_platform_socket_option(e_transport_endpoint_option option)
{
    switch (option)
    {
    case _transport_endpoint_option_reuse_address:
        return SO_REUSEADDR;
    case _transport_endpoint_option_linger:
        return SO_LINGER;
    case _transport_endpoint_option_broardcast:
        return SO_BROADCAST;
    case _transport_endpoint_option_send_buffer_size:
        return SO_SNDBUF;
    case _transport_endpoint_option_receive_buffer_size:
        return SO_RCVBUF;
    case _transport_endpoint_option_unknown5:
        return 0x4001;
    default:
        c_console::write_line("networking:transport:endpoint: option %d unknown", option);
        break;
    }

    return -1;
}
