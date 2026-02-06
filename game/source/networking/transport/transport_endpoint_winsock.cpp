#include "networking/transport/transport_endpoint_winsock.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include <WinSock2.h>
#include <ws2ipdef.h>

const int32 k_socket_address_size = sizeof(sockaddr_in6); // 0x1C

HOOK_DECLARE(0x0043F980, get_platform_socket_option);
HOOK_DECLARE(0x0043FA30, transport_endpoint_accept);
HOOK_DECLARE(0x0043FAC0, transport_endpoint_async_connect);
HOOK_DECLARE(0x0043FB60, transport_endpoint_async_is_connected);
HOOK_DECLARE(0x0043FCE0, transport_endpoint_bind);
HOOK_DECLARE(0x0043FD50, transport_endpoint_blocking);
HOOK_DECLARE(0x0043FD70, transport_endpoint_connect);
HOOK_DECLARE(0x0043FDE0, transport_endpoint_connected);
HOOK_DECLARE(0x0043FDF0, transport_endpoint_create);
HOOK_DECLARE(0x0043FE50, transport_endpoint_create_socket);
HOOK_DECLARE(0x0043FED0, transport_endpoint_delete);
HOOK_DECLARE(0x0043FF30, transport_endpoint_disconnect);
HOOK_DECLARE(0x0043FF80, transport_endpoint_get_option_value);
HOOK_DECLARE(0x00440000, transport_endpoint_get_socket_address);
HOOK_DECLARE(0x00440120, transport_endpoint_get_transport_address);
HOOK_DECLARE(0x00440230, transport_endpoint_get_type);
HOOK_DECLARE(0x00440240, transport_endpoint_listen);
HOOK_DECLARE(0x00440280, transport_endpoint_listening);
HOOK_DECLARE(0x00440290, transport_endpoint_read);
HOOK_DECLARE(0x004402F0, transport_endpoint_read_from);
HOOK_DECLARE(0x00440390, transport_endpoint_readable);
HOOK_DECLARE(0x00440440, transport_endpoint_reject);
HOOK_DECLARE(0x00440470, transport_endpoint_set_blocking);
HOOK_DECLARE(0x00440520, transport_endpoint_set_option_value);
HOOK_DECLARE(0x00440590, transport_endpoint_setup);
HOOK_DECLARE(0x004405C0, transport_endpoint_test);
HOOK_DECLARE(0x00440660, transport_endpoint_write);
HOOK_DECLARE(0x004406C0, transport_endpoint_write_to);
HOOK_DECLARE(0x00440740, transport_endpoint_writeable);
HOOK_DECLARE(0x004407D0, transport_get_endpoint_address);

#define wsa_error_level() error_level(WSAGetLastError())
#define wsa_error_string() winsock_error_to_string(WSAGetLastError())

bool known_error_code(int32 error_code)
{
	bool known = false;
	switch (error_code)
	{
	case WSAENETUNREACH:
	case WSAECONNRESET:
	case WSAETIMEDOUT:
	case WSAECONNREFUSED:
	case WSAEHOSTUNREACH:
		known = true;
		break;
	}
	return known;
}

e_event_level error_level(int32 error_code)
{
	return e_event_level(!known_error_code(error_code) + _event_message);
}

int32 __cdecl get_platform_socket_option(e_transport_endpoint_option option)
{
	//return INVOKE(0x0043F980, get_platform_socket_option, option);

	int32 option_out = NONE;
	switch (option)
	{
	case _transport_endpoint_option_reuse_address:
		option_out = SO_REUSEADDR;
		break;
	case _transport_endpoint_option_linger:
		option_out = SO_LINGER;
		break;
	case _transport_endpoint_option_broadcast:
		option_out = SO_BROADCAST;
		break;
	case _transport_endpoint_option_send_buffer_size:
		option_out = SO_SNDBUF;
		break;
	case _transport_endpoint_option_recv_buffer_size:
		option_out = SO_RCVBUF;
		break;
	case _transport_endpoint_option_alpha:
		option_out = 0x4001;
		break;
	default:
		event(_event_warning, "networking:transport:endpoint: option %d unknown", option);
		break;
	}
	return option_out;
}

transport_endpoint* __cdecl transport_endpoint_accept(transport_endpoint* listening_endpoint)
{
	//return INVOKE(0x0043FA30, transport_endpoint_accept, listening_endpoint);

	byte address_buffer[k_socket_address_size]{};
	int32 address_length = k_socket_address_size;

	transport_endpoint* endpoint = nullptr;
	if (transport_available())
	{
		ASSERT(listening_endpoint != nullptr);
		ASSERT(listening_endpoint->socket != INVALID_SOCKET);
		ASSERT(TEST_BIT(listening_endpoint->flags, _transport_endpoint_listening_bit));

		SOCKET new_socket = ::accept(listening_endpoint->socket, (sockaddr*)address_buffer, (int*)&address_length);
		if (new_socket == INVALID_SOCKET)
		{
			event(wsa_error_level(), "transport:endpoint: accept() failed: error= %s",
				wsa_error_string());
		}
		else
		{
			endpoint = transport_endpoint_create(listening_endpoint->type);
			if (endpoint)
			{
				endpoint->socket = new_socket;
				SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, true);
			}
		}
	}
	return endpoint;
}

bool __cdecl transport_endpoint_async_connect(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FAC0, transport_endpoint_async_connect, endpoint, address);

	ASSERT(endpoint != nullptr);
	ASSERT(address != nullptr);

	byte address_buffer[k_socket_address_size];
	int32 address_length;

	bool success = false;
	if (transport_available() &&
		transport_endpoint_get_socket_address(address, &address_length, address_buffer) &&
		transport_endpoint_create_socket(endpoint, address))
	{
		if (transport_endpoint_set_blocking(endpoint, false))
		{
			int result = ::connect(endpoint->socket, (const sockaddr*)address_buffer, address_length);
			if (result)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					success = true;
				}
				else
				{
					event(wsa_error_level(), "transport:endpoint: connect() failed: error= %s",
						wsa_error_string());
				}
			}
			else
			{
				SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, true);
				SET_BIT(endpoint->flags, _transport_endpoint_clientside_bit, true);
				success = true;
			}
		}
		else
		{
			event(wsa_error_level(), "transport:endpoint: transport_endpoint_set_blocking() failed: error= %s",
				wsa_error_string());
		}
	}
	return success;
}

bool __cdecl transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected)
{
	//return INVOKE(0x0043FB60, transport_endpoint_async_is_connected, endpoint, is_connected);

	ASSERT(endpoint != nullptr);
	ASSERT(is_connected);

	*is_connected = false;

	fd_set writeable_set;
	fd_set except_set;
	timeval timeout;

	bool success = false;
	if (transport_available())
	{
		if (endpoint->socket == NONE)
		{
			event(_event_error, "networking:transport:endpoint: transport_endpoint_async_is_connected().  endpoint was invalid.");
		}
		else
		{
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;

			FD_ZERO(&writeable_set);
			FD_SET(endpoint->socket, &writeable_set);

			FD_ZERO(&except_set);
			FD_SET(endpoint->socket, &except_set);

			if (::select(0, nullptr, &writeable_set, &except_set, &timeout) == -1)
			{
				event(wsa_error_level(), "transport:endpoint: transport_endpoint_async_is_connected() failed in select(): error= %s",
					wsa_error_string());
			}
			else if (FD_ISSET(endpoint->socket, &except_set))
			{
				event(_event_error, "networking:transport:endpoint: transport_endpoint_async_is_connected() failed to connect to host.");
			}
			else
			{
				success = true;
				if (FD_ISSET(endpoint->socket, &writeable_set))
				{
					SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, true);
					SET_BIT(endpoint->flags, _transport_endpoint_clientside_bit, true);

					*is_connected = true;
				}
			}
		}
	}
	return success;
}

bool __cdecl transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address)
{
	//return INVOKE(0x0043FCE0, transport_endpoint_bind, endpoint, address);

	ASSERT(endpoint != nullptr);
	ASSERT(address != nullptr);

	byte address_buffer[k_socket_address_size];
	int32 address_length;

	bool success = false;
	if (transport_available() &&
		transport_endpoint_get_socket_address(address, &address_length, address_buffer) &&
		transport_endpoint_create_socket(endpoint, address))
	{
		if (::bind(endpoint->socket, (const sockaddr*)&address_buffer, address_length) != 0)
		{
			event(wsa_error_level(), "transport:endpoint: bind() failed: error= %s",
				wsa_error_string());
		}
		else
		{
			success = true;
		}
	}
	return success;
}

bool __cdecl transport_endpoint_blocking(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FD50, transport_endpoint_blocking, endpoint);

	ASSERT(endpoint != nullptr);
	return TEST_BIT(endpoint->flags, _transport_endpoint_blocking_bit);
}

bool __cdecl transport_endpoint_connect(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FD70, transport_endpoint_connect, endpoint, address);

	ASSERT(endpoint != nullptr);
	ASSERT(address != nullptr);

	byte address_buffer[k_socket_address_size];
	int32 address_length;

	bool success = false;

	if (transport_available() &&
		transport_endpoint_get_socket_address(address, &address_length, address_buffer) &&
		transport_endpoint_create_socket(endpoint, address))
	{
		if (::connect(endpoint->socket, (const sockaddr*)address_buffer, address_length) != 0)
		{
			event(wsa_error_level(), "transport:endpoint: connect() failed: error= %s",
				wsa_error_string());
		}
		else
		{
			SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, true);
			SET_BIT(endpoint->flags, _transport_endpoint_clientside_bit, true);
			success = true;
		}
	}

	return success;
}

bool __cdecl transport_endpoint_connected(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FDE0, transport_endpoint_connected, endpoint);

	ASSERT(endpoint != nullptr);
	return TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit);
}

transport_endpoint* __cdecl transport_endpoint_create(e_transport_type type)
{
	//return INVOKE(0x0043FDF0, transport_endpoint_create, type);

	transport_endpoint* endpoint = nullptr;
	if (transport_available())
	{
		endpoint = (transport_endpoint*)system_malloc(sizeof(transport_endpoint));
		if (endpoint)
		{
			transport_endpoint_setup(endpoint, type);
		}
		else
		{
			event(_event_error, "networking:transport:endpoint: OUT OF MEMORY - transport_endpoint_create failed");
		}
	}
	else
	{
		event(_event_warning, "networking:transport:endpoint: transport_endpoint_create() called when transport_available()==false");
	}
	return endpoint;
}

bool __cdecl transport_endpoint_create_socket(transport_endpoint* endpoint, const transport_address* address)
{
	//return INVOKE(0x0043FE50, transport_endpoint_create_socket, endpoint, address);

	ASSERT(endpoint);
	ASSERT(address);

	bool success = false;

	if (endpoint->socket == INVALID_SOCKET)
	{
		int32 socket_type = 0;
		int32 socket_protocol = 0;
		int32 family = NONE;

		switch (endpoint->type)
		{
		case _transport_type_udp:
			socket_type = SOCK_DGRAM;
			socket_protocol = IPPROTO_UDP;
			break;
		case _transport_type_vdp:
			//socket_type = SOCK_DGRAM;
			//socket_protocol = 254;
			break;
		case _transport_type_tcp:
			socket_type = SOCK_STREAM;
			socket_protocol = IPPROTO_TCP;
			break;
		case _transport_type_unix:
			break;
		default:
			UNREACHABLE("unknown endpoint type");
			break;
		};

		switch (address->address_length)
		{
		case IPV4_ADDRESS_LENGTH:
			family = AF_INET;
			break;
		case IPV6_ADDRESS_LENGTH:
			family = AF_INET6;
			break;
		}

		endpoint->socket = ::socket(family, socket_type, socket_protocol);
	}

	if (endpoint->socket == INVALID_SOCKET)
	{
		event(wsa_error_level(), "transport:endpoint: socket() failed: error= %s",
			wsa_error_string());
	}
	else
	{
		SET_BIT(endpoint->flags, _transport_endpoint_blocking_bit, true);
		success = true;

#ifdef USE_IPV6
		int v6only = 0;
		::setsockopt(endpoint->socket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&v6only, sizeof(v6only));
#endif
	}

	return success;
}

void __cdecl transport_endpoint_delete(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FED0, transport_endpoint_delete, endpoint);

	ASSERT(endpoint != nullptr);

	transport_endpoint_disconnect(endpoint);
	system_free(endpoint);
}

void __cdecl transport_endpoint_disconnect(transport_endpoint* endpoint)
{
	//return INVOKE(0x0043FF30, transport_endpoint_disconnect, endpoint);

	ASSERT(endpoint != nullptr);

	if (endpoint->socket != INVALID_SOCKET)
	{
		if (transport_available())
		{
			if (TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit) && shutdown(endpoint->socket, SD_BOTH))
			{
				event(wsa_error_level(), "transport:endpoint: shutdown() failed: error= %s",
					wsa_error_string());
			}
			if (::closesocket(endpoint->socket) != 0)
			{
				event(wsa_error_level(), "transport:endpoint: closesocket() failed: error= %s",
					wsa_error_string());
			}
		}
		else
		{
			event(_event_error, "networking:transport:endpoint: unable to disconnect endpoint, transport is unavailable (we probably leaked a socket and might crash)");
		}
	}

	endpoint->socket = INVALID_SOCKET;
	endpoint->flags = 0;
}

int32 __cdecl transport_endpoint_get_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option)
{
	//return INVOKE(0x0043FF80, transport_endpoint_get_option_value, endpoint, option);

	ASSERT(endpoint != nullptr);

	int32 result = 0;
	if (transport_available() && endpoint->socket != INVALID_SOCKET)
	{
		int32 platform_option = get_platform_socket_option(option);
		if (platform_option == NONE)
		{
			event(_event_error, "networking:transport:endpoint: unsupported endpoint option %d", option);
		}
		else
		{
			int32 option_value = 0;
			int32 option_length = sizeof(option_value);
			if (::getsockopt(endpoint->socket, SOL_SOCKET, platform_option, (char*)&option_value, (int*)&option_length) != 0)
			{
				event(wsa_error_level(), "transport:endpoint: getsockopt(%d, %d) failed with %s",
					option,
					platform_option,
					wsa_error_string());
			}
			result = option_value;
		}
	}
	return result;
}

bool __cdecl transport_endpoint_get_socket_address(const transport_address* address, int32* socket_address_size, uns8* const socket_address)
{
	//return INVOKE(0x00440000, transport_endpoint_get_socket_address, address, socket_address_size, socket_address);

	ASSERT(address);
	ASSERT(socket_address_size);

	bool success = false;
	switch (address->address_length)
	{
	case IPV4_ADDRESS_LENGTH:
	{
		sockaddr_in* ipv4_address = (sockaddr_in*)socket_address;

		ipv4_address->sin_family = AF_INET;
		ipv4_address->sin_port = bswap_uns16(address->port);
		ipv4_address->sin_addr.s_addr = bswap_uns32(address->ipv4_address);

		*socket_address_size = sizeof(sockaddr_in);
		success = true;
	}
	break;
	case IPV6_ADDRESS_LENGTH:
	{
		sockaddr_in6* ipv6_address = (sockaddr_in6*)socket_address;

		ipv6_address->sin6_family = AF_INET6;
		ipv6_address->sin6_port = bswap_uns16(address->port);
		ipv6_address->sin6_flowinfo = 0;
		ipv6_address->sin6_addr.s6_words[0] = bswap_uns16(address->ina6.words[0]);
		ipv6_address->sin6_addr.s6_words[1] = bswap_uns16(address->ina6.words[1]);
		ipv6_address->sin6_addr.s6_words[2] = bswap_uns16(address->ina6.words[2]);
		ipv6_address->sin6_addr.s6_words[3] = bswap_uns16(address->ina6.words[3]);
		ipv6_address->sin6_addr.s6_words[4] = bswap_uns16(address->ina6.words[4]);
		ipv6_address->sin6_addr.s6_words[5] = bswap_uns16(address->ina6.words[5]);
		ipv6_address->sin6_addr.s6_words[6] = bswap_uns16(address->ina6.words[6]);
		ipv6_address->sin6_addr.s6_words[7] = bswap_uns16(address->ina6.words[7]);
		ipv6_address->sin6_scope_id = 0;

		*socket_address_size = sizeof(sockaddr_in6);
		success = true;
	}
	break;
	default:
	{
		*socket_address_size = 0;
		event(_event_error, "networking:transport:endpoint: get_socket_address() failed: unknown address type %d",
			address->address_length);
	}
	break;
	}
	return success;
}

bool __cdecl transport_endpoint_get_transport_address(int32 socket_address_length, const uns8* socket_address, transport_address* address)
{
	//return INVOKE(0x00440120, transport_endpoint_get_transport_address, socket_address_length, socket_address, address);

	ASSERT(socket_address_length >= 0 && socket_address_length <= k_socket_address_size);

	bool success = false;
	switch (socket_address_length)
	{
	case sizeof(sockaddr_in):
	{
		const sockaddr_in* ipv4_address = (const sockaddr_in*)socket_address;
		ASSERT(ipv4_address->sin_family == AF_INET);

		address->port = bswap_uns16(ipv4_address->sin_port);
		address->ipv4_address = bswap_uns32(ipv4_address->sin_addr.s_addr);
		address->address_length = IPV4_ADDRESS_LENGTH;

		success = true;
	}
	break;
	case sizeof(sockaddr_in6):
	{
		const sockaddr_in6* ipv6_address = (const sockaddr_in6*)socket_address;
		ASSERT(ipv6_address->sin6_family == AF_INET6);

		address->ina6.words[0] = bswap_uns16(ipv6_address->sin6_addr.s6_words[0]);
		address->ina6.words[1] = bswap_uns16(ipv6_address->sin6_addr.s6_words[1]);
		address->ina6.words[2] = bswap_uns16(ipv6_address->sin6_addr.s6_words[2]);
		address->ina6.words[3] = bswap_uns16(ipv6_address->sin6_addr.s6_words[3]);
		address->ina6.words[4] = bswap_uns16(ipv6_address->sin6_addr.s6_words[4]);
		address->ina6.words[5] = bswap_uns16(ipv6_address->sin6_addr.s6_words[5]);
		address->ina6.words[6] = bswap_uns16(ipv6_address->sin6_addr.s6_words[6]);
		address->ina6.words[7] = bswap_uns16(ipv6_address->sin6_addr.s6_words[7]);
		address->port = bswap_uns16(ipv6_address->sin6_port);
		address->address_length = IPV6_ADDRESS_LENGTH;

		success = true;
	}
	break;
	default:
	{
		event(_event_error, "networking:transport:endpoint: get_transport_address() failed: unknown address type %d",
			socket_address_length);

		csmemset(address, 0, sizeof(*address));
	}
	break;
	}
	return success;
}

e_transport_type __cdecl transport_endpoint_get_type(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440230, transport_endpoint_get_type, endpoint);

	ASSERT(endpoint != nullptr);

	return endpoint->type;
}

bool __cdecl transport_endpoint_listen(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440240, transport_endpoint_listen, endpoint);

	ASSERT(endpoint != nullptr);
	ASSERT(endpoint->socket != INVALID_SOCKET);

	bool success = false;
	if (transport_available())
	{
		if (::listen(endpoint->socket, k_maximum_outstanding_connect_requests) != 0)
		{
			event(wsa_error_level(), "transport:endpoint: listen() failed: error= %s",
				wsa_error_string());
		}
		else
		{
			SET_BIT(endpoint->flags, _transport_endpoint_listening_bit, true);
			success = true;
		}
	}
	return success;
}

bool __cdecl transport_endpoint_listening(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440280, transport_endpoint_listening, endpoint);

	ASSERT(endpoint != nullptr);

	return TEST_BIT(endpoint->flags, _transport_endpoint_listening_bit);
}

int16 __cdecl transport_endpoint_read(transport_endpoint* endpoint, void* buffer, int16 length)
{
	//return INVOKE(0x00440290, transport_endpoint_read, endpoint, buffer, length);

	ASSERT(endpoint != nullptr);
	ASSERT(buffer != nullptr);
	ASSERT(length > 0);

	ASSERT(endpoint->socket != INVALID_SOCKET);

	int16 bytes_read = 0;
	if (transport_available() && TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit))
	{
		bytes_read = ::recv(endpoint->socket, (char*)buffer, length, 0);
		if (bytes_read == -1i16)
		{
			int32 wsa_error = WSAGetLastError();
			if (wsa_error == WSAEWOULDBLOCK)
			{
				bytes_read = _transport_readwrite_error_would_block;
			}
			else
			{
				event(error_level(wsa_error), "transport:read: recv() failed w/ unknown error '%s'",
					winsock_error_to_string(wsa_error));
				bytes_read = _transport_readwrite_error_unknown;
			}
		}
		else if (bytes_read)
		{
			ASSERT(bytes_read > 0);
		}
		else
		{
			SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, false);
			SET_BIT(endpoint->flags, _transport_endpoint_readable_bit, false);
			SET_BIT(endpoint->flags, _transport_endpoint_clientside_bit, false);
		}
	}
	return bytes_read;
}

int16 __cdecl transport_endpoint_read_from(transport_endpoint* endpoint, void* buffer, int16 length, transport_address* source)
{
	//return INVOKE(0x004402F0, transport_endpoint_read_from, endpoint, buffer, length, source);

	ASSERT(endpoint != nullptr);
	ASSERT(buffer != nullptr);
	ASSERT(length > 0);
	ASSERT(source != nullptr);
	ASSERT(endpoint->socket != INVALID_SOCKET);
	ASSERT(endpoint->type != _transport_type_unix);

	byte address_buffer[k_socket_address_size]{};
	int32 address_length = k_socket_address_size;

	int16 bytes_read = -3i16;
	if (transport_available())
	{
		bytes_read = ::recvfrom(endpoint->socket, (char*)buffer, length, 0, (sockaddr*)&address_buffer, (int*)&address_length);
		if (bytes_read == -1i16)
		{
			int wsa_error = WSAGetLastError();
			if (wsa_error == WSAEWOULDBLOCK)
			{
				bytes_read = _transport_readwrite_error_would_block;
			}
			else
			{
				event(error_level(wsa_error), "transport:read: recvfrom() failed w/ unknown error '%s'",
					winsock_error_to_string(wsa_error));
				bytes_read = _transport_readwrite_error_unknown;
			}
		}
		else if (bytes_read)
		{
			ASSERT(bytes_read > 0);
			transport_endpoint_get_transport_address(address_length, address_buffer, source);
		}
	}
	return bytes_read;
}

bool __cdecl transport_endpoint_readable(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440390, transport_endpoint_readable, endpoint);

	ASSERT(endpoint != nullptr);

	bool readable = false;
	if (transport_available() && endpoint->socket != INVALID_SOCKET)
	{
		if (TEST_BIT(endpoint->flags, _transport_endpoint_readable_bit))
		{
			readable = TEST_BIT(endpoint->flags, _transport_endpoint_in_set_bit);
		}
		else
		{
			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;

			fd_set readable_set;
			FD_ZERO(&readable_set);
			FD_SET(endpoint->socket, &readable_set);

			readable = ::select(1, &readable_set, nullptr, nullptr, &timeout) == 1 && FD_ISSET(endpoint->socket, &readable_set);
		}
	}
	return readable;
}

bool __cdecl transport_endpoint_reject(transport_endpoint* listening_endpoint)
{
	//return INVOKE(0x00440440, transport_endpoint_reject, listening_endpoint);

	if (transport_available())
	{
		transport_endpoint* junk = transport_endpoint_accept(listening_endpoint);
		if (junk)
		{
			transport_endpoint_delete(junk);
		}
	}
	return true;
}

bool __cdecl transport_endpoint_set_blocking(transport_endpoint* endpoint, bool blocking)
{
	//return INVOKE(0x00440470, transport_endpoint_set_blocking, endpoint, blocking);

	ASSERT(endpoint != nullptr);

	bool success = true;
	if (transport_available())
	{
		bool currently_blocking = TEST_BIT(endpoint->flags, _transport_endpoint_blocking_bit);
		if (endpoint->socket == INVALID_SOCKET)
		{
			event(_event_error, "networking:transport:endpoint: invalid endpoint in transport_endpoint_set_blocking()");
			success = false;
		}
		else if (currently_blocking)
		{
			if (!blocking)
			{
				uns32 flags = 1;
				if (::ioctlsocket(endpoint->socket, FIONBIO, &flags) != 0)
				{
					event(wsa_error_level(), "transport:endpoint: failed to set endpoint non-blocking; error= %s",
						wsa_error_string());

					success = false;
				}
				else
				{
					SET_BIT(endpoint->flags, _transport_endpoint_blocking_bit, false);
				}
			}
		}
		else if (blocking)
		{
			uns32 flags = 0;
			if (::ioctlsocket(endpoint->socket, FIONBIO, &flags) != 0)
			{
				event(wsa_error_level(), "transport:endpoint: failed to set endpoint blocking; error= %s",
					wsa_error_string());

				success = false;
			}
			else
			{
				SET_BIT(endpoint->flags, _transport_endpoint_blocking_bit, true);
			}
		}
	}
	return success;
}

bool __cdecl transport_endpoint_set_option_value(transport_endpoint* endpoint, e_transport_endpoint_option option, int32 value)
{
	//return INVOKE(0x00440520, transport_endpoint_set_option_value, endpoint, option, value);

	ASSERT(endpoint != nullptr);

	int32 result = value;
	int32 option_length = sizeof(result);

	bool success = false;
	if (transport_available() && endpoint->socket != INVALID_SOCKET)
	{
		int32 platform_option = get_platform_socket_option(option);
		if (platform_option != NONE)	
		{
			const char* setsockopt_value = (const char*)&result;
			if (::setsockopt(endpoint->socket, SOL_SOCKET, platform_option, setsockopt_value, option_length) != 0)
			{
				event(wsa_error_level(), "transport:endpoint: setsockopt(%d, %d) failed with %s",
					option,
					platform_option,
					wsa_error_string());
			}
			else
			{
				success = true;
			}
		}
		else
		{
			event(_event_error, "networking:transport:endpoint: unsupported endpoint option %d",
				option);
		}
	}
	return success;
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

	ASSERT(endpoint != nullptr);
	ASSERT(address != nullptr);

	byte address_buffer[k_socket_address_size];
	int32 address_length;

	bool success = false;
	if (transport_available()
		&& transport_endpoint_get_socket_address(address, &address_length, address_buffer)
		&& transport_endpoint_create_socket(endpoint, address)
		&& ::connect(endpoint->socket, (const sockaddr*)address_buffer, (int)address_length) == 0)
	{
		SET_BIT(endpoint->flags, _transport_endpoint_connected_bit, true);
		SET_BIT(endpoint->flags, _transport_endpoint_clientside_bit, true);
		success = true;
	}
	transport_endpoint_disconnect(endpoint);
	return success;
}

int16 __cdecl transport_endpoint_write(transport_endpoint* endpoint, const void* buffer, int16 length)
{
	//return INVOKE(0x00440660, transport_endpoint_write, endpoint, buffer, length);

	ASSERT(endpoint != nullptr);
	ASSERT(buffer != nullptr);
	ASSERT(length > 0);
	ASSERT(endpoint->socket != INVALID_SOCKET);

	int16 bytes_written = 0;
	if (transport_available() && TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit))
	{
		bytes_written = ::send(endpoint->socket, (const char*)buffer, (int)length, 0);
		if (bytes_written == -1i16)
		{
			int32 wsa_error = WSAGetLastError();
			if (wsa_error == WSAEWOULDBLOCK)
			{
				return _transport_readwrite_error_would_block;
			}
			else if (wsa_error == WSAEHOSTUNREACH)
			{
				return _transport_write_error_unreachable;
			}
			else
			{
				event(error_level(wsa_error), "transport:write: send() failed w/ unknown error '%s'",
					winsock_error_to_string(wsa_error));

				return _transport_readwrite_error_unknown;
			}
		}
		else
		{
			ASSERT(bytes_written > 0);
		}
	}
	return bytes_written;
}

int16 __cdecl transport_endpoint_write_to(transport_endpoint* endpoint, const void* buffer, int16 length, const transport_address* destination)
{
	//return INVOKE(0x004406C0, transport_endpoint_write_to, endpoint, buffer, length, destination);

	ASSERT(endpoint != nullptr);
	ASSERT(buffer != nullptr);
	ASSERT(length > 0);
	ASSERT(destination != nullptr);
	ASSERT(endpoint->socket != INVALID_SOCKET);
	ASSERT(endpoint->type != _transport_type_unix);

	byte address_buffer[k_socket_address_size];
	int32 address_length;

	int16 bytes_written = _transport_readwrite_error_unknown;
	if (transport_available() &&
		transport_endpoint_get_socket_address(destination, &address_length, address_buffer))
	{
		bytes_written = ::sendto(endpoint->socket, (const char*)buffer, length, 0, (const sockaddr*)address_buffer, (int)address_length);
		if (bytes_written == -1i16)
		{
			int32 wsa_error = WSAGetLastError();
			if (wsa_error == WSAEWOULDBLOCK)
			{
				bytes_written = _transport_readwrite_error_would_block;
			}
			else if (wsa_error == WSAEHOSTUNREACH)
			{
				bytes_written = _transport_write_error_unreachable;
			}
			else
			{
				event(error_level(wsa_error), "transport:write: sendto() '%s' failed w/ unknown error '%s'",
					transport_address_get_string(destination),
					winsock_error_to_string(wsa_error));

				bytes_written = _transport_readwrite_error_unknown;
			}
		}
		else
		{
			ASSERT(bytes_written >= 0);
		}
	}
	return bytes_written;
}

bool __cdecl transport_endpoint_writeable(transport_endpoint* endpoint)
{
	//return INVOKE(0x00440740, transport_endpoint_writeable, endpoint);

	ASSERT(endpoint != nullptr);

	bool writeable = false;
	if (transport_available() && endpoint->socket != INVALID_SOCKET)
	{
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		fd_set writeable_set;
		FD_ZERO(&writeable_set);
		FD_SET(endpoint->socket, &writeable_set);

		writeable = ::select(1, nullptr, &writeable_set, nullptr, &timeout) == 1 && FD_ISSET(endpoint->socket, &writeable_set);
	}
	return writeable;
}

bool __cdecl transport_get_endpoint_address(transport_endpoint* endpoint, transport_address* address)
{
	//return INVOKE(0x004407D0, transport_get_endpoint_address, endpoint, address);

	ASSERT(address != nullptr);

	bool success = false;
	if (transport_available())
	{
		transport_get_listen_address(address, 0);

		if (endpoint && endpoint->socket != INVALID_SOCKET)
		{
			byte address_buffer[k_socket_address_size];
			int32 length = k_socket_address_size;
			if (!TEST_BIT(endpoint->flags, _transport_endpoint_connected_bit) ||
				TEST_BIT(endpoint->flags, _transport_endpoint_listening_bit) ||
				::getpeername(endpoint->socket, (sockaddr*)address_buffer, (int*)&length) != 0 &&
				::getsockname(endpoint->socket, (sockaddr*)address_buffer, (int*)&length) != 0)
			{
				event(wsa_error_level(), "transport:endpoint: transport_get_endpoint_address() failed to retrieve an address; error= %s",
					wsa_error_string());
			}
			else
			{
				success = transport_endpoint_get_transport_address(length, address_buffer, address);
			}
		}
	}
	return success;
}

