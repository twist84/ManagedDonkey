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
//HOOK_DECLARE(0x004402F0, transport_endpoint_read_from);
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
    transport_endpoint* result = nullptr;
    HOOK_INVOKE(result =, transport_endpoint_accept, listening_endpoint);
    return result;
}

bool __cdecl transport_endpoint_async_connect(transport_endpoint* endpoint, transport_address const* address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_async_connect, endpoint, address);
    return result;
}

bool __cdecl transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_async_is_connected, endpoint, is_connected);
    return result;
}

bool __cdecl transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_bind, endpoint, address);
    return result;
}

bool __cdecl transport_endpoint_connect(transport_endpoint* endpoint, transport_address const* address)
{
    assert(endpoint != NULL);
    assert(address != NULL);

    return INVOKE(0x0043FD70, transport_endpoint_connect, endpoint, address);
}

transport_endpoint* __cdecl transport_endpoint_create(e_transport_type type)
{
    transport_endpoint* result = nullptr;
    HOOK_INVOKE(result =, transport_endpoint_create, type);
    return result;
}

bool __cdecl transport_endpoint_create_socket(transport_endpoint* endpoint, transport_address const* address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_create_socket, endpoint, address);
    return result;
}

void __cdecl transport_endpoint_delete(transport_endpoint* endpoint)
{
    HOOK_INVOKE(, transport_endpoint_delete, endpoint);
}

void __cdecl transport_endpoint_disconnect(transport_endpoint* endpoint)
{
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
    long result = 0;
    HOOK_INVOKE(result =, transport_endpoint_get_option_value, endpoint, option);
    return result;
}

bool __cdecl transport_endpoint_get_socket_address(transport_address const* address, long* socket_address_size, byte* const socket_address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_get_socket_address, address, socket_address_size, socket_address);
    return result;
}

bool __cdecl transport_endpoint_get_transport_address(long socket_address_length, byte const* const socket_address, transport_address* address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_get_transport_address, socket_address_length, socket_address, address);
    return result;
}

e_transport_type __cdecl transport_endpoint_get_type(transport_endpoint* endpoint)
{
    assert(endpoint != NULL);

    return endpoint->type;
}

bool __cdecl transport_endpoint_listen(transport_endpoint* endpoint)
{
    assert(endpoint != NULL);
    assert(endpoint->socket != INVALID_SOCKET);

    if (!transport_available())
        return false;

    if (listen(endpoint->socket, 16))
    {
        c_console::write_line("transport:endpoint: listen() failed: error= %s", winsock_error_to_string(WSAGetLastError()));

        return false;
    }

    endpoint->flags |= FLAG(1);
    return true;
}

short __cdecl transport_endpoint_read(transport_endpoint* endpoint, void* buffer, short length)
{
    assert(buffer != NULL);
    assert(length > 0);
    assert(endpoint->socket != INVALID_SOCKET);

    short result;
    if (transport_available() && TEST_BIT(endpoint->flags, 0))
    {
        word bytes_read = recv(endpoint->socket, static_cast<char*>(buffer), length, 0);
        if (bytes_read == 0xFFFF)
        {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK)
            {
                result = -2;
            }
            else
            {
                c_console::write_line("transport:read: recv() failed w/ unknown error '%s'", winsock_error_to_string(error));
                result = -3;
            }
        }
        else if (bytes_read)
        {
            assert(bytes_read > 0);
        }
        else
        {
            endpoint->flags &= ~FLAG(0);
            endpoint->flags &= ~FLAG(2);
            endpoint->flags &= ~FLAG(5);
        }
    }

    return result;
}

short __cdecl transport_endpoint_read_from(transport_endpoint* endpoint, void* buffer, short length, transport_address* source)
{
    //short result = 0;
    //HOOK_INVOKE(result =, transport_endpoint_read_from, endpoint, buffer, length, source);
    //return result;

    return INVOKE(0x004402F0, transport_endpoint_read_from, endpoint, buffer, length, source);
}

bool __cdecl transport_endpoint_readable(transport_endpoint* endpoint)
{
    assert(endpoint != NULL);

    return INVOKE(0x00440390, transport_endpoint_readable, endpoint);
}

bool __cdecl transport_endpoint_reject(transport_endpoint* listening_endpoint)
{
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
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_set_blocking, endpoint, blocking);
    return result;
}

bool __cdecl transport_endpoint_set_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option, long value)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_set_option_value, endpoint, option, value);
    return result;
}

void __cdecl transport_endpoint_setup(transport_endpoint* endpoint, e_transport_type type)
{
    assert(endpoint);
    assert((type == _transport_type_udp) || (type == _transport_type_vdp) || (type == _transport_type_tcp));

    endpoint->socket = INVALID_SOCKET;
    endpoint->flags = 0;
    endpoint->type = type;
}

bool __cdecl transport_endpoint_test(transport_endpoint* endpoint, transport_address const* address)
{
    assert(endpoint != NULL);
    assert(address != NULL);

    return INVOKE(0x004405C0, transport_endpoint_test, endpoint, address);
}

short __cdecl transport_endpoint_write(transport_endpoint* endpoint, void const* buffer, short length)
{
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
    short result = 0;
    HOOK_INVOKE(result =, transport_endpoint_write_to, endpoint, buffer, length, destination);
    return result;
}

bool __cdecl transport_endpoint_writeable(transport_endpoint* endpoint)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_endpoint_writeable, endpoint);
    return result;
}

bool __cdecl transport_get_endpoint_address(transport_endpoint* endpoint, transport_address* address)
{
    bool result = false;
    HOOK_INVOKE(result =, transport_get_endpoint_address, endpoint, address);
    return result;
}

bool __cdecl transport_endpoint_connected(transport_endpoint* endpoint)
{
    assert(endpoint != NULL);

    // ((endpoint->flags & 1) & 32) == 0
    return TEST_BIT(endpoint->flags, 8);
}

