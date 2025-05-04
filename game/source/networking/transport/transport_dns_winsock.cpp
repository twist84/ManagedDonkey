#include "networking/transport/transport_dns_winsock.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "networking/transport/transport.hpp"

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>

bool __cdecl transport_dns_address_to_name(dns_result* result)
{
	return INVOKE(0x0043B330, transport_dns_address_to_name, result);

	//if (!transport_available())
	//	return false;
	//
	//ASSERT(result != NULL);
	//
	//transport_address* address = &result->address[0];
	//
	//union
	//{
	//	uns32 ipv4_address;
	//	uns16 ipv6_address[8];
	//} ip_address;
	//
	//int32 type = 0;
	//int32 address_length = 0;
	//switch (address->address_length)
	//{
	//case 4:
	//{
	//	type = AF_INET;
	//	address_length = 4;
	//
	//	ip_address.ipv4_address = bswap_uns32(result->address[0].ipv4_address);
	//}
	//break;
	//case 16:
	//{
	//	type = AF_INET6;
	//	address_length = 16;
	//
	//	ip_address.ipv6_address[0] = bswap_uns16(address->ina6.words[0]);
	//	ip_address.ipv6_address[1] = bswap_uns16(address->ina6.words[1]);
	//	ip_address.ipv6_address[2] = bswap_uns16(address->ina6.words[2]);
	//	ip_address.ipv6_address[3] = bswap_uns16(address->ina6.words[3]);
	//	ip_address.ipv6_address[4] = bswap_uns16(address->ina6.words[4]);
	//	ip_address.ipv6_address[5] = bswap_uns16(address->ina6.words[5]);
	//	ip_address.ipv6_address[6] = bswap_uns16(address->ina6.words[6]);
	//	ip_address.ipv6_address[7] = bswap_uns16(address->ina6.words[7]);
	//}
	//break;
	//default:
	//{
	//	event(_event_error, "networking:transport:dns: reverse dns failed: bad address type");
	//	return false;
	//}
	//break;
	//}
	//
	//hostent* host = gethostbyaddr((const char*)&ip_address, address_length, type);
	//if (!host || !*host->h_aliases)
	//{
	//	return false;
	//}
	//
	//csstrnzcpy(result->name, *host->h_aliases, sizeof(result->name));
	//result->name[255] = 0;
	//return true;
}

bool __cdecl transport_dns_name_to_address(dns_result* result)
{
	return INVOKE(0x0043B460, transport_dns_name_to_address, result);
}

