#include "networking/transport/transport_endpoint_winsock.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include <assert.h>

HOOK_DECLARE(0x0043F980, get_platform_socket_option);
HOOK_DECLARE(0x0043FA30, transport_endpoint_accept);
HOOK_DECLARE(0x0043FAC0, transport_endpoint_async_connect);
HOOK_DECLARE(0x0043FB60, transport_endpoint_async_is_connected);
HOOK_DECLARE(0x0043FCE0, transport_endpoint_bind);
HOOK_DECLARE(0x0043FDF0, transport_endpoint_create);
HOOK_DECLARE(0x0043FE50, transport_endpoint_create_socket);
HOOK_DECLARE(0x0043FED0, transport_endpoint_delete);
HOOK_DECLARE(0x0043FF30, transport_endpoint_disconnect);
HOOK_DECLARE(0x0043FF80, transport_endpoint_get_option_value);
HOOK_DECLARE(0x00440000, transport_endpoint_get_socket_address);
HOOK_DECLARE(0x00440120, transport_endpoint_get_transport_address);
HOOK_DECLARE(0x00440290, transport_endpoint_read);
HOOK_DECLARE(0x004402F0, transport_endpoint_read_from);
HOOK_DECLARE(0x00440440, transport_endpoint_reject);
HOOK_DECLARE(0x00440470, transport_endpoint_set_blocking);
HOOK_DECLARE(0x00440520, transport_endpoint_set_option_value);
HOOK_DECLARE(0x00440590, transport_endpoint_setup);
HOOK_DECLARE(0x00440660, transport_endpoint_write);
HOOK_DECLARE(0x004406C0, transport_endpoint_write_to);
HOOK_DECLARE(0x00440740, transport_endpoint_writeable);
HOOK_DECLARE(0x004407D0, transport_get_endpoint_address);

long __cdecl get_platform_socket_option(e_transport_endpoint_option option)
{
    FUNCTION_BEGIN(true);

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

transport_endpoint* __cdecl transport_endpoint_accept(transport_endpoint* listening_endpoint)
{
    FUNCTION_BEGIN(true);

    transport_endpoint* result = nullptr;
    HOOK_INVOKE(result =, transport_endpoint_accept, listening_endpoint);
    return result;
}

bool __cdecl transport_endpoint_async_connect(transport_endpoint* endpoint, transport_address const* address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_async_connect, endpoint, address);
    return result;
}

bool __cdecl transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_async_is_connected, endpoint, is_connected);
    return result;
}

bool __cdecl transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_bind, endpoint, address);
    return result;
}

transport_endpoint* __cdecl transport_endpoint_create(e_transport_type type)
{
    FUNCTION_BEGIN(true);

    transport_endpoint* result = nullptr;
    HOOK_INVOKE(result =, transport_endpoint_create, type);
    return result;
}

bool __cdecl transport_endpoint_create_socket(transport_endpoint* endpoint, transport_address const* address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_create_socket, endpoint, address);
    return result;
}

void __cdecl transport_endpoint_delete(transport_endpoint* endpoint)
{
    FUNCTION_BEGIN(true);

    HOOK_INVOKE(, transport_endpoint_delete, endpoint);
}

void __cdecl transport_endpoint_disconnect(transport_endpoint* endpoint)
{
    FUNCTION_BEGIN(true);

    assert(endpoint != NULL);

    if (endpoint->socket != INVALID_SOCKET)
    {
        if (transport_available())
        {
            if (TEST_BIT(endpoint->flags, 0))
            {
                if (shutdown(endpoint->socket, 2 /* SD_BOTH */))
                {
                    char const* winsock_error_string = winsock_error_to_string(WSAGetLastError());
                    c_console::write_line("transport:endpoint: shutdown() failed: error= %s", winsock_error_string);
                }
            }
            if (closesocket(endpoint->socket))
            {
                char const* winsock_error_string = winsock_error_to_string(WSAGetLastError());
                c_console::write_line("transport:endpoint: closesocket() failed: error= %s", winsock_error_string);
            }
        }
        else
        {
            c_console::write_line("networking:transport:endpoint: unable to disconnect endpoint, transport is unavailable (we probably leaked a socket and might crash)");
        }
    }
    endpoint->socket = INVALID_SOCKET;
    endpoint->flags = 0;
}

long __cdecl transport_endpoint_get_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option)
{
    FUNCTION_BEGIN(true);

    long result = 0;
    HOOK_INVOKE(result =, transport_endpoint_get_option_value, endpoint, option);
    return result;
}

bool __cdecl transport_endpoint_get_socket_address(transport_address const* address, long* socket_address_size, byte* const socket_address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_get_socket_address, address, socket_address_size, socket_address);
    return result;
}

bool __cdecl transport_endpoint_get_transport_address(long socket_address_length, byte const* const socket_address, transport_address* address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_get_transport_address, socket_address_length, socket_address, address);
    return result;
}

short __cdecl transport_endpoint_read(transport_endpoint* endpoint, void* buffer, short length)
{
    FUNCTION_BEGIN(true);

    short result = 0;
    HOOK_INVOKE(result =, transport_endpoint_read, endpoint, buffer, length);
    return result;
}

short __cdecl transport_endpoint_read_from(transport_endpoint* endpoint, void* buffer, short length, transport_address* source)
{
    FUNCTION_BEGIN(false);

    short result = 0;
    HOOK_INVOKE(result =, transport_endpoint_read_from, endpoint, buffer, length, source);
    return result;
}

bool __cdecl transport_endpoint_reject(transport_endpoint* listening_endpoint)
{
    FUNCTION_BEGIN(true);

    if (transport_available())
    {
        transport_endpoint* endpoint = transport_endpoint_accept(listening_endpoint);
        if (endpoint)
            transport_endpoint_delete(endpoint);
    }
    return true;
}

bool __cdecl transport_endpoint_set_blocking(transport_endpoint* endpoint, bool blocking)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_set_blocking, endpoint, blocking);
    return result;
}

bool __cdecl transport_endpoint_set_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option, long value)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_set_option_value, endpoint, option, value);
    return result;
}

void __cdecl transport_endpoint_setup(transport_endpoint* endpoint, e_transport_type type)
{
    FUNCTION_BEGIN(true);

    assert(endpoint);
    assert((type == _transport_type_udp) || (type == _transport_type_vdp) || (type == _transport_type_tcp));

    endpoint->socket = INVALID_SOCKET;
    endpoint->flags = 0;
    endpoint->type = type;
}

short __cdecl transport_endpoint_write(transport_endpoint* endpoint, void const* buffer, short length)
{
    FUNCTION_BEGIN(true);

    assert(endpoint != NULL);
    assert(buffer != NULL);
    assert(length > 0);
    assert(endpoint->socket != INVALID_SOCKET);

    short result = 0;
    if (transport_available() && TEST_BIT(endpoint->flags, 0))
    {
        short bytes_written = send(endpoint->socket, static_cast<const char*>(buffer), length, 0);
        if (bytes_written == -1)
        {
            int error = WSAGetLastError();

            if (error == WSAEWOULDBLOCK)
            {
                result = -2;
            }
            else if (error == WSAEHOSTUNREACH)
            {
                result = -1;
            }
            else
            {
                c_console::write_line("transport:write: send() failed w/ unknown error '%s'", winsock_error_to_string(error));
                result = -1;
            }
        }
        else assert(bytes_written > 0);
    }
    return result;
}

short __cdecl transport_endpoint_write_to(transport_endpoint* endpoint, void const* buffer, short length, transport_address const* destination)
{
    FUNCTION_BEGIN(true);

    short result = 0;
    HOOK_INVOKE(result =, transport_endpoint_write_to, endpoint, buffer, length, destination);
    return result;
}

bool __cdecl transport_endpoint_writeable(transport_endpoint* endpoint)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_writeable, endpoint);
    return result;
}

bool __cdecl transport_get_endpoint_address(transport_endpoint* endpoint, transport_address* address)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, transport_get_endpoint_address, endpoint, address);
    return result;
}
