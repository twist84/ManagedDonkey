#include "networking/transport/transport_address.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport_security.hpp"

#include <string.h>

HOOK_DECLARE(0x0043F660, transport_address_equivalent);
HOOK_DECLARE(0x0043F6F0, transport_address_get_string);
HOOK_DECLARE(0x0043F720, transport_address_ipv4_extract);
HOOK_DECLARE(0x0043F730, transport_address_is_loopback);
HOOK_DECLARE(0x0043F750, transport_address_to_string);
HOOK_DECLARE(0x0043F860, transport_address_valid);
HOOK_DECLARE(0x0043F8D0, transport_get_broadcast_address);
HOOK_DECLARE(0x0043F8F0, transport_get_listen_address);

bool __cdecl transport_address_equivalent(transport_address const* a, transport_address const* b)
{
	ASSERT(a != NULL);
	ASSERT(b != NULL);

	short address_length = a->address_length <= b->address_length ? a->address_length : b->address_length;
	return a->address_length > 0 && a->address_length == b->address_length && memcmp(a, b, address_length) == 0 && a->port == b->port;
}

char const* __cdecl transport_address_get_string(transport_address const* address)
{
	static char _string[256]{};
	transport_address_to_string(address, nullptr, _string, 256, false, true);

	return _string;
}

void __cdecl transport_address_ipv4_build(transport_address* address, dword ip_address, word port)
{
	ASSERT(address);

	address->address_length = sizeof(dword);
	address->ipv4_address = ip_address;
	address->port = port;
}

dword __cdecl transport_address_ipv4_extract(transport_address const* address)
{
	ASSERT(address);

	return address->ipv4_address;
}

bool __cdecl transport_address_is_loopback(transport_address const* address)
{
	ASSERT(address);

	return address->address_length == sizeof(dword) && address->ipv4_address == 0x7F000001;
}

char* __cdecl transport_address_to_string(transport_address const* address, s_transport_secure_address const* secure_address, char* _string, short maximum_string_length, bool include_port, bool include_extra)
{
	ASSERT(address);
	ASSERT(_string);
	ASSERT(maximum_string_length > 0);

	char secure_address_string[256]{};
	s_transport_secure_identifier secure_identifier_from_address;
	s_transport_secure_address secure_address_from_address;

	csstrnzcpy(_string, "", maximum_string_length);
	switch (address->address_length)
	{
	case 4:
		csnzprintf(_string, maximum_string_length, "%hd.%hd.%hd.%hd",
			address->ina.bytes[3],
			address->ina.bytes[2],
			address->ina.bytes[1],
			address->ina.bytes[0]);

		if (include_port)
			csnzappendf(_string, maximum_string_length, ":%hd", address->port);

		if (include_extra)
		{
			if (secure_address)
			{
				transport_secure_address_to_string(secure_address, secure_address_string, 256, false, false);
				csstrnzcat(_string, secure_address_string, maximum_string_length);
			}
			else if (transport_secure_identifier_retrieve(address, 0, &secure_identifier_from_address, &secure_address_from_address))
			{
				transport_secure_address_to_string(&secure_address_from_address, secure_address_string, 256, false, false);
				csstrnzcat(_string, secure_address_string, maximum_string_length);
				csnzappendf(_string, maximum_string_length, "[%s]", transport_secure_identifier_get_string(&secure_identifier_from_address));
			}
			else
			{
				csnzappendf(_string, maximum_string_length, " (insecure)");
			}
		}
		break;
	case 16:
		csnzprintf(_string, maximum_string_length, "%04X.%04X.%04X.%04X.%04X.%04X.%04X.%04X",
			address->ina6.words[0],
			address->ina6.words[1],
			address->ina6.words[2],
			address->ina6.words[3],
			address->ina6.words[4],
			address->ina6.words[5],
			address->ina6.words[6],
			address->ina6.words[7]);

		if (include_port)
			csnzappendf(_string, maximum_string_length, ":%hd", address->port);
		break;
	case 0xFFFF:
		csstrnzcpy(_string, address->str, maximum_string_length);
		break;
	case 0:
		csstrnzcpy(_string, "NULL_ADDRESS", maximum_string_length);
		break;
	}

	return _string;
}

bool __cdecl transport_address_valid(transport_address const* address)
{
	bool result = false;
	if (address)
	{
		switch (address->address_length)
		{
		case 0xFFFFFFFF:
		{
			result = address->ipv4_address != 0;
		}
		break;
		case 4:
		{
			result = address->ipv4_address != 0;
			if (!result)
				c_console::write_line("networking:transport:transport_address_valid: the IPV4 address is NOT valid");
		}
		break;
		case 16:
		{
			for (long i = 0; i < 8; ++i)
			{
				if (address->ina6.words[i])
				{
					result = true;
					break;
				}
			}
			if (!result)
				c_console::write_line("networking:transport:transport_address_valid: the IPV6 address is NOT valid");
		}
		break;
		}
	}

	return result;
}

void __cdecl transport_get_broadcast_address(transport_address* address, word port)
{
	ASSERT(address);

	address->address_length = sizeof(dword);
	address->ipv4_address = 0xFFFFFFFF;
	address->port = port;
}

void __cdecl transport_get_listen_address(transport_address* address, word port)
{
	ASSERT(address);

	address->address_length = sizeof(dword);
	address->ipv4_address = 0;
	address->port = port;
}

void __cdecl transport_get_loopback_address(transport_address* address, word port)
{
	ASSERT(address);

	address->address_length = sizeof(dword);
	address->ipv4_address = 0x7F000001;
	address->port = port;
}

void transport_address_from_string(wchar_t const* str, transport_address& address)
{
	word ip_addr[4]{};
	if (swscanf_s(str, L"%hd.%hd.%hd.%hd:%hd", &ip_addr[3], &ip_addr[2], &ip_addr[1], &ip_addr[0], &address.port))
	{
		address.ina.bytes[3] = static_cast<byte>(ip_addr[3]);
		address.ina.bytes[2] = static_cast<byte>(ip_addr[2]);
		address.ina.bytes[1] = static_cast<byte>(ip_addr[1]);
		address.ina.bytes[0] = static_cast<byte>(ip_addr[0]);

		address.address_length = sizeof(address.ina);
	}
}

