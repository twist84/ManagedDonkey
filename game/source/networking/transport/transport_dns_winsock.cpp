#include "networking/transport/transport_dns_winsock.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "networking/transport/transport.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

bool __cdecl transport_dns_address_to_name(dns_result* result)
{
	return INVOKE(0x0043B330, transport_dns_address_to_name, result);

#if 0
	bool success = false;
	if (transport_available())
	{
		ASSERT(result != nullptr);

		union
		{
			uns32 ipv4_address;
			uns16 ipv6_address[8];
		} ip_address{};

		uns32 length = 0;
		int family = AF_UNSPEC;

		switch (result->address[0].address_length)
		{
		case IPV4_ADDRESS_LENGTH:
		{
			ip_address.ipv4_address = bswap_uns32(result->address[0].ipv4_address);

			length = IPV4_ADDRESS_LENGTH;
			family = AF_INET;
		}
		break;
		case IPV6_ADDRESS_LENGTH:
		{
			ip_address.ipv6_address[0] = bswap_uns16(result->address[0].ipv6_address[0]);
			ip_address.ipv6_address[1] = bswap_uns16(result->address[0].ipv6_address[1]);
			ip_address.ipv6_address[2] = bswap_uns16(result->address[0].ipv6_address[2]);
			ip_address.ipv6_address[3] = bswap_uns16(result->address[0].ipv6_address[3]);
			ip_address.ipv6_address[4] = bswap_uns16(result->address[0].ipv6_address[4]);
			ip_address.ipv6_address[5] = bswap_uns16(result->address[0].ipv6_address[5]);
			ip_address.ipv6_address[6] = bswap_uns16(result->address[0].ipv6_address[6]);
			ip_address.ipv6_address[7] = bswap_uns16(result->address[0].ipv6_address[7]);

			length = IPV6_ADDRESS_LENGTH;
			family = AF_INET6;
		}
		break;
		default:
		{
			event(_event_error, "networking:transport:dns: reverse dns failed: bad address type");
		}
		break;
		}

		char* address = (char*)&ip_address;
		if (family != AF_UNSPEC)
		{
			hostent* host = gethostbyaddr(address, length, family);
			if (host && *host->h_aliases)
			{
				csstrnzcpy(result->name, *host->h_aliases, sizeof(result->name));
				result->name[255] = 0;
				success = true;
			}
		}
	}
	return success;
#endif
}

bool __cdecl transport_dns_name_to_address(dns_result* result)
{
	//return INVOKE(0x0043B460, transport_dns_name_to_address, result);

	bool success = false;
	if (transport_available())
	{
		ASSERT(result != nullptr);

		hostent* host = gethostbyname(result->name);
		if (host != nullptr)
		{
			for (int16 i = 0; i < NUMBEROF(result->address); i++)
			{
				if (!host->h_addr_list[i])
				{
					break;
				}

				switch (host->h_addrtype)
				{
				case AF_INET:
				{
					uns32* ip = (uns32*)host->h_addr_list[i];
					result->address[i].ipv4_address = bswap_uns32(*ip);
					result->address[i].address_length = IPV4_ADDRESS_LENGTH;
				}
				break;
				case AF_INET6:
				{
					uns16* ip = (uns16*)host->h_addr_list[i];
					result->address[i].ipv6_address[0] = bswap_uns16(ip[0]);
					result->address[i].ipv6_address[1] = bswap_uns16(ip[1]);
					result->address[i].ipv6_address[2] = bswap_uns16(ip[2]);
					result->address[i].ipv6_address[3] = bswap_uns16(ip[3]);
					result->address[i].ipv6_address[4] = bswap_uns16(ip[4]);
					result->address[i].ipv6_address[5] = bswap_uns16(ip[5]);
					result->address[i].ipv6_address[6] = bswap_uns16(ip[6]);
					result->address[i].ipv6_address[7] = bswap_uns16(ip[7]);
					result->address[i].address_length = IPV6_ADDRESS_LENGTH;
				}
				break;
				}

				if (transport_address_valid(&result->address[i]))
				{
					success = true;
				}
			}
		}
	}
	return success;
}

