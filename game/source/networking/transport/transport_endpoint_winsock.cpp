#include "networking/transport/transport_endpoint_winsock.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include <WinSock2.h>
#include <ws2ipdef.h>

HOOK_DECLARE(0x0043F980, get_platform_socket_option);
HOOK_DECLARE(0x0043FA30, transport_endpoint_accept);
HOOK_DECLARE(0x0043FAC0, transport_endpoint_async_connect);
//HOOK_DECLARE(0x0043FB60, transport_endpoint_async_is_connected);
HOOK_DECLARE(0x0043FCE0, transport_endpoint_bind);
//HOOK_DECLARE(0x0043FD50, transport_endpoint_blocking);
HOOK_DECLARE(0x0043FD70, transport_endpoint_connect);
//HOOK_DECLARE(0x0043FDE0, transport_endpoint_connected);
HOOK_DECLARE(0x0043FDF0, transport_endpoint_create);
HOOK_DECLARE(0x0043FE50, transport_endpoint_create_socket);
HOOK_DECLARE(0x0043FED0, transport_endpoint_delete);
HOOK_DECLARE(0x0043FF30, transport_endpoint_disconnect);
HOOK_DECLARE(0x0043FF80, transport_endpoint_get_option_value);
HOOK_DECLARE(0x00440000, transport_endpoint_get_socket_address);
HOOK_DECLARE(0x00440120, transport_endpoint_get_transport_address);
//HOOK_DECLARE(0x00440230, transport_endpoint_get_type);
//HOOK_DECLARE(0x00440240, transport_endpoint_listen);
//HOOK_DECLARE(0x00440280, transport_endpoint_listening);
HOOK_DECLARE(0x00440290, transport_endpoint_read);
//HOOK_DECLARE(0x004402F0, transport_endpoint_read_from);
//HOOK_DECLARE(0x00440390, transport_endpoint_readable);
HOOK_DECLARE(0x00440440, transport_endpoint_reject);
HOOK_DECLARE(0x00440470, transport_endpoint_set_blocking);
HOOK_DECLARE(0x00440520, transport_endpoint_set_option_value);
HOOK_DECLARE(0x00440590, transport_endpoint_setup);
//HOOK_DECLARE(0x004405C0, transport_endpoint_test);
HOOK_DECLARE(0x00440660, transport_endpoint_write);
HOOK_DECLARE(0x004406C0, transport_endpoint_write_to);
HOOK_DECLARE(0x00440740, transport_endpoint_writeable);
HOOK_DECLARE(0x004407D0, transport_get_endpoint_address);

int32 __cdecl get_platform_socket_option(e_transport_endpoint_option option)
{
	//return INVOKE(0x0043F980, get_platform_socket_option, option);

	switch (option)
	{
	case _transport_endpoint_option_reuse_address:
		return SO_REUSEADDR;
	case _transport_endpoint_option_linger:
		return SO_LINGER;
	case _transport_endpoint_option_broadcast:
		return SO_BROADCAST;
	case _transport_endpoint_option_send_buffer_size:
		return SO_SNDBUF;
	case _transport_endpoint_option_recv_buffer_size:
		return SO_RCVBUF;
	case _transport_endpoint_option_alpha:
		return 0x4001;
	default:
		event(_event_warning, "networking:transport:endpoint: option %d unknown", option);
		break;
	}

	return NONE;
}

transport_endpoint* __cdecl transport_endpoint_accept(transport_endpoint* listening_endpoint)
{
	//return INVOKE(0x0043FA30, transport_endpoint_accept, listening_endpoint);

	ASSERT(listening_endpoint != NULL);
	ASSERT(listening_endpoint->socket != INVALID_SOCKET);
	ASSERT(TEST_BIT(listening_endpoint->flags, _transport_endpoint_listening_bit));

	if (!transport_available())
		return NULL;

	byte socket_address[0x1C];
	int socket_address_size = sizeof(socket_address);
	csmemset(socket_address, 0, socket_address_size);

	SOCKET listening_socket = accept(listening_endpoint->socket, (sockaddr*)socket_address, &socket_address_size);
	if (listening_socket == INVALID_SOCKET)
	{
		event(_event_warning, "transport:endpoint: accept() failed: error= %s",
			winsock_error_to_string(WSAGetLastError()));
		return NULL;
	}
	else
	{
		transport_endpoint* endpoint = transport_endpoint_create(listening_endpoint->type);
		if (endpoint)
		{
			endpoint->flags |= FLAG(_transport_endpoint_connected_bit);
			endpoint->socket = listening_socket;
		}
		return endpoint;
	}

	return NULL;
}

bool __cdecl transport_endpoint_async_connect(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FAC0, transport_endpoint_async_connect, endpoint, );

	ASSERT(endpoint != NULL);
	ASSERT(address != NULL);

	if (!transport_available())
		return false;

	byte socket_address[0x1C];
	int32 socket_address_size = sizeof(socket_address);
	csmemset(socket_address, 0, socket_address_size);

	if (transport_endpoint_get_socket_address(address, &socket_address_size, socket_address) && transport_endpoint_create_socket(endpoint, address))
	{
		if (transport_endpoint_set_blocking(endpoint, false))
		{
			if (connect(endpoint->socket, (const sockaddr*)socket_address, socket_address_size))
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					return true;
				}
				else
				{
					event(_event_warning, "transport:endpoint: connect() failed: error= %s",
						winsock_error_to_string(WSAGetLastError()));
				}
			}
			else
			{
				endpoint->flags |= FLAG(_transport_endpoint_connected_bit);
				endpoint->flags |= FLAG(_transport_endpoint_clientside_bit);
				return true;
			}
		}
		else
		{
			event(_event_warning, "transport:endpoint: transport_endpoint_set_blocking() failed: error= %s",
				winsock_error_to_string(WSAGetLastError()));
		}
	}
	return false;
}

bool __cdecl transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected)
{
	return INVOKE(0x0043FB60, transport_endpoint_async_is_connected, endpoint, is_connected);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_endpoint_async_is_connected, endpoint, is_connected);
	//return result;

	// $IMPLEMENT
}

bool __cdecl transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address)
{
	//return INVOKE(0x0043FCE0, transport_endpoint_bind, endpoint, address);

	ASSERT(endpoint != NULL);
	ASSERT(address != NULL);

	if (!transport_available())
		return false;

	byte socket_address[0x1C];
	int32 socket_address_size = sizeof(socket_address);
	csmemset(socket_address, 0, socket_address_size);

	if (transport_endpoint_get_socket_address(address, &socket_address_size, socket_address) && transport_endpoint_create_socket(endpoint, address))
	{
		if (bind(endpoint->socket, (const sockaddr*)&socket_address, socket_address_size))
		{
			event(_event_warning, "transport:endpoint: bind() failed: error= %s", winsock_error_to_string(WSAGetLastError()));
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool __cdecl transport_endpoint_blocking(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FD50, transport_endpoint_blocking, endpoint);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_endpoint_blocking, endpoint);
	//return result;

	ASSERT(endpoint != NULL);

	return TEST_BIT(endpoint->flags, _transport_endpoint_blocking_bit);
}

bool __cdecl transport_endpoint_connect(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FD70, transport_endpoint_connect, endpoint, address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_endpoint_connect, endpoint, address);
	//return result;

	ASSERT(endpoint != NULL);
	ASSERT(address != NULL);

	if (!transport_available())
		return false;

	byte socket_address[0x1C];
	int32 socket_address_size = sizeof(socket_address);
	csmemset(socket_address, 0, socket_address_size);

	if (transport_endpoint_get_socket_address(address, &socket_address_size, socket_address) && transport_endpoint_create_socket(endpoint, address))
	{
		if (connect(endpoint->socket, (const sockaddr*)socket_address, socket_address_size))
		{
			event(_event_warning, "transport:endpoint: connect() failed: error= %s",
				winsock_error_to_string(WSAGetLastError()));
		}
		else
		{
			endpoint->flags |= FLAG(_transport_endpoint_connected_bit);
			endpoint->flags |= FLAG(_transport_endpoint_clientside_bit);
			return true;
		}
	}
	return false;
}

bool __cdecl transport_endpoint_connected(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FDE0, transport_endpoint_connected, endpoint);

	ASSERT(endpoint != NULL);

	return TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit);
}

transport_endpoint* __cdecl transport_endpoint_create(e_transport_type type)
{
	//return INVOKE(0x0043FDF0, transport_endpoint_create, type);

	if (transport_available() == false)
	{
		event(_event_warning, "networking:transport:endpoint: transport_endpoint_create() called when transport_available()==false");
		return NULL;
	}

	transport_endpoint* endpoint = (transport_endpoint*)system_malloc(sizeof(transport_endpoint));
	if (!endpoint)
	{
		event(_event_error, "networking:transport:endpoint: OUT OF MEMORY - transport_endpoint_create failed");
		return NULL;
	}

	transport_endpoint_setup(endpoint, type);
	return endpoint;
}

bool __cdecl transport_endpoint_create_socket(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FE50, transport_endpoint_create_socket, endpoint, address);

	ASSERT(endpoint);
	ASSERT(address);

	bool result = false;
	HOOK_INVOKE(result =, transport_endpoint_create_socket, endpoint, address);
	return result;

	// $IMPLEMENT
}

void __cdecl transport_endpoint_delete(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FED0, transport_endpoint_delete, endpoint);

	ASSERT(endpoint != NULL);

	transport_endpoint_disconnect(endpoint);
	system_free(endpoint);
}

void __cdecl transport_endpoint_disconnect(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FF30, transport_endpoint_disconnect, endpoint);

	ASSERT(endpoint != NULL);

	if (endpoint->socket == INVALID_SOCKET)
	{
		endpoint->socket = INVALID_SOCKET;
		endpoint->flags = 0;
		return;
	}

	if (!transport_available())
	{
		event(_event_error, "networking:transport:endpoint: unable to disconnect endpoint, transport is unavailable (we probably leaked a socket and might crash)");
		endpoint->socket = INVALID_SOCKET;
		endpoint->flags = 0;
		return;
	}

	if (TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit) && shutdown(endpoint->socket, 2 /* SD_BOTH */))
	{
		event(_event_warning, "transport:endpoint: shutdown() failed: error= %s",
			winsock_error_to_string(WSAGetLastError()));
	}
	if (closesocket(endpoint->socket))
	{
		event(_event_warning, "transport:endpoint: closesocket() failed: error= %s",
			winsock_error_to_string(WSAGetLastError()));
	}

	endpoint->socket = INVALID_SOCKET;
	endpoint->flags = 0;
}

int32 __cdecl transport_endpoint_get_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option)
{
	//return INVOKE(0x0043FF80, transport_endpoint_get_option_value, endpoint, option);

	ASSERT(endpoint != NULL);

	if (transport_available() == false || endpoint->socket == INVALID_SOCKET)
	{
		return 0;
	}

	int32 platform_socket_option = get_platform_socket_option(option);
	if (platform_socket_option == NONE)
	{
		event(_event_error, "networking:transport:endpoint: unsupported endpoint option %d", option);
		return 0;
	}

	int32 option_value = 0;
	int option_length = 4;
	if (getsockopt(endpoint->socket, 0xFFFF, platform_socket_option, (char*)&option_value, &option_length))
	{
		event(_event_warning, "transport:endpoint: getsockopt(%d, %d) failed with %s",
			option,
			platform_socket_option,
			winsock_error_to_string(WSAGetLastError()));
	}
	return option_value;
}

bool __cdecl transport_endpoint_get_socket_address(const transport_address* address, int32* socket_address_size, uns8* const socket_address)
{
	//return INVOKE(0x00440000, transport_endpoint_get_socket_address, address, socket_address_size, socket_address);

	ASSERT(address);
	ASSERT(socket_address_size);

	*socket_address_size = 0;
	if (address->address_length == 4)
	{
		sockaddr_in* sock = (sockaddr_in*)socket_address;
		sock->sin_family = AF_INET;
		sock->sin_port = bswap_uns16(address->port);
		sock->sin_addr.s_addr = bswap_uns32(address->ipv4_address);
		*socket_address_size = sizeof(sockaddr_in);
		return true;
	}

	if (address->address_length == 16)
	{
		sockaddr_in6* sock = (sockaddr_in6*)socket_address;
		sock->sin6_family = AF_INET6;
		sock->sin6_port = bswap_uns16(address->port);
		sock->sin6_addr.u.Word[0] = bswap_uns16(address->ina6.words[0]);
		sock->sin6_addr.u.Word[1] = bswap_uns16(address->ina6.words[1]);
		sock->sin6_addr.u.Word[2] = bswap_uns16(address->ina6.words[2]);
		sock->sin6_addr.u.Word[3] = bswap_uns16(address->ina6.words[3]);
		sock->sin6_addr.u.Word[4] = bswap_uns16(address->ina6.words[4]);
		sock->sin6_addr.u.Word[5] = bswap_uns16(address->ina6.words[5]);
		sock->sin6_addr.u.Word[6] = bswap_uns16(address->ina6.words[6]);
		sock->sin6_addr.u.Word[7] = bswap_uns16(address->ina6.words[7]);
		sock->sin6_scope_id = 0;
		*socket_address_size = sizeof(sockaddr_in6);
		return true;
	}

	event(_event_error, "networking:transport:endpoint: get_socket_address() failed: unknown address type %d",
		address->address_length);
	return false;
}

bool __cdecl transport_endpoint_get_transport_address(int32 socket_address_length, const uns8* const socket_address, transport_address* address)
{
	//return INVOKE(0x00440120, transport_endpoint_get_transport_address, socket_address_length, socket_address, address);

	bool result = false;
	HOOK_INVOKE(result =, transport_endpoint_get_transport_address, socket_address_length, socket_address, address);
	return result;

	// $IMPLEMENT
}

e_transport_type __cdecl transport_endpoint_get_type(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440230, transport_endpoint_get_type, endpoint);

	ASSERT(endpoint != NULL);

	return endpoint->type;
}

bool __cdecl transport_endpoint_listen(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440240, transport_endpoint_listen, endpoint);

	ASSERT(endpoint != NULL);
	ASSERT(endpoint->socket != INVALID_SOCKET);

	if (!transport_available())
		return false;

	if (listen(endpoint->socket, 16))
	{
		event(_event_warning, "transport:endpoint: listen() failed: error= %s",
			winsock_error_to_string(WSAGetLastError()));

		return false;
	}

	endpoint->flags |= FLAG(_transport_endpoint_listening_bit);
	return true;
}

bool __cdecl transport_endpoint_listening(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440280, transport_endpoint_listening, endpoint);

	ASSERT(endpoint != NULL);

	return TEST_BIT(endpoint->flags, _transport_endpoint_listening_bit);
}

int16 __cdecl transport_endpoint_read(transport_endpoint* endpoint, void* buffer, int16 length)
{
	//return INVOKE(0x00440290, transport_endpoint_read, endpoint, buffer, length);

	ASSERT(buffer != NULL);
	ASSERT(length > 0);
	ASSERT(endpoint->socket != INVALID_SOCKET);

	int16 bytes_read = 0;
	if (transport_available() && TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit))
	{
		bytes_read = recv(endpoint->socket, static_cast<char*>(buffer), length, 0);
		if (bytes_read == int16(0xFFFF))
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				bytes_read = int16(0xFFFE);
			}
			else
			{
				event(_event_warning, "transport:read: recv() failed w/ unknown error '%s'",
					winsock_error_to_string(error));
				bytes_read = int16(0xFFFD);
			}
		}
		else if (bytes_read)
		{
			ASSERT(bytes_read > 0);
		}
		else
		{
			endpoint->flags &= ~FLAG(_transport_endpoint_connected_bit);
			endpoint->flags &= ~FLAG(_transport_endpoint_readable_bit);
			endpoint->flags &= ~FLAG(_transport_endpoint_clientside_bit);
		}
	}

	return bytes_read;
}

int16 __cdecl transport_endpoint_read_from(transport_endpoint* endpoint, void* buffer, int16 length, transport_address* source)
{
	return INVOKE(0x004402F0, transport_endpoint_read_from, endpoint, buffer, length, source);

	ASSERT(endpoint != NULL);
	ASSERT(buffer != NULL);
	ASSERT(length > 0);
	ASSERT(source != NULL);
	ASSERT(endpoint->socket != INVALID_SOCKET);
	ASSERT(endpoint->type != _transport_type_unix);

	// $IMPLEMENT
}

bool __cdecl transport_endpoint_readable(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440390, transport_endpoint_readable, endpoint);

	ASSERT(endpoint != NULL);

	if (!transport_available())
		return false;

	if (endpoint->socket == INVALID_SOCKET)
		return false;

	if (TEST_BIT(endpoint->flags, _transport_endpoint_readable_bit))
		return TEST_BIT(endpoint->flags, _transport_endpoint_in_set_bit);


	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(endpoint->socket, &readfds);

	return select(1, &readfds, NULL, NULL, &timeout) == 1 && FD_ISSET(endpoint->socket, &readfds);
}

bool __cdecl transport_endpoint_reject(transport_endpoint* listening_endpoint)
{
	//return INVOKE(0x00440440, transport_endpoint_reject, listening_endpoint);

	if (transport_available())
	{
		if (transport_endpoint* endpoint = transport_endpoint_accept(listening_endpoint))
			transport_endpoint_delete(endpoint);
	}
	return true;
}

bool __cdecl transport_endpoint_set_blocking(transport_endpoint* endpoint, bool blocking)
{
	//return INVOKE(0x00440470, transport_endpoint_set_blocking, endpoint, blocking);

	bool result = false;
	HOOK_INVOKE(result =, transport_endpoint_set_blocking, endpoint, blocking);
	return result;

	ASSERT(endpoint != NULL);

	// $IMPLEMENT
}

bool __cdecl transport_endpoint_set_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option, int32 value)
{
	//return INVOKE(0x00440520, transport_endpoint_set_option_value, endpoint, option, value);

	bool result = false;
	HOOK_INVOKE(result =, transport_endpoint_set_option_value, endpoint, option, value);
	return result;

	ASSERT(endpoint != NULL);

	// $IMPLEMENT
}

void __cdecl transport_endpoint_setup(transport_endpoint* endpoint, e_transport_type type)
{
	//INVOKE(0x00440590, transport_endpoint_setup, endpoint, type);

	ASSERT(endpoint);
	ASSERT((type == _transport_type_udp) || (type == _transport_type_vdp) || (type == _transport_type_tcp));

	endpoint->socket = INVALID_SOCKET;
	endpoint->flags = 0;
	endpoint->type = type;
}

bool __cdecl transport_endpoint_test(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x004405C0, transport_endpoint_test, endpoint, address);

	ASSERT(endpoint != NULL);
	ASSERT(address != NULL);

	bool result = transport_endpoint_connect(endpoint, address); // inline
	transport_endpoint_disconnect(endpoint);
	return result;
}

int16 __cdecl transport_endpoint_write(transport_endpoint* endpoint, const void* buffer, int16 length)
{
	//return INVOKE(0x00440660, transport_endpoint_write, endpoint, buffer, length);

	ASSERT(endpoint != NULL);
	ASSERT(buffer != NULL);
	ASSERT(length > 0);
	ASSERT(endpoint->socket != INVALID_SOCKET);

	int16 result = 0;
	if (transport_available() && TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit))
	{
		int16 bytes_written = send(endpoint->socket, static_cast<const char*>(buffer), length, 0);
		if (bytes_written == int16(0xFFFF))
		{
			int error = WSAGetLastError();

			if (error == WSAEWOULDBLOCK)
			{
				return int16(0xFFFE);
			}
			else if (error == WSAEHOSTUNREACH)
			{
				return int16(0xFFFF);
			}
			else
			{
				event(_event_warning, "transport:write: send() failed w/ unknown error '%s'",
					winsock_error_to_string(error));

				return int16(0xFFFF);
			}
		}
		else
		{
			ASSERT(bytes_written > 0);
			return bytes_written;
		}
	}
	return result;
}

int16 __cdecl transport_endpoint_write_to(transport_endpoint* endpoint, const void* buffer, int16 length, const transport_address* destination)
{
	//return INVOKE(0x004406C0, transport_endpoint_write_to, endpoint, buffer, length, destination);

	int16 result = 0;
	HOOK_INVOKE(result =, transport_endpoint_write_to, endpoint, buffer, length, destination);
	return result;

	ASSERT(endpoint != NULL);
	ASSERT(buffer != NULL);
	ASSERT(length > 0);
	ASSERT(destination != NULL);
	ASSERT(endpoint->socket != INVALID_SOCKET);
	ASSERT(endpoint->type != _transport_type_unix);

	// $IMPLEMENT
}

bool __cdecl transport_endpoint_writeable(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440740, transport_endpoint_writeable, endpoint);

	ASSERT(endpoint != NULL);

	if (!transport_available())
		return false;

	if (endpoint->socket == INVALID_SOCKET)
		return false;

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(endpoint->socket, &writefds);

	return select(1, NULL, &writefds, NULL, &timeout) == 1 && FD_ISSET(endpoint->socket, &writefds);
}

bool __cdecl transport_get_endpoint_address(transport_endpoint* endpoint, transport_address* address)
{
	//return INVOKE(0x004407D0, transport_get_endpoint_address, endpoint, address);

	bool result = false;
	HOOK_INVOKE(result =, transport_get_endpoint_address, endpoint, address);
	return result;

	ASSERT(address != NULL);

	// $IMPLEMENT
}

