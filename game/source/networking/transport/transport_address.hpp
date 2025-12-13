#pragma once

#include "cseries/cseries.hpp"

struct s_player_identifier;
struct s_transport_secure_address;

struct transport_address
{
	transport_address();
	transport_address(uns32 _ipv4_address, uns16 _port, int16 _address_length);
	transport_address(const s_player_identifier* player_identifier);

	union
	{
		uns32 ipv4_address;

		// in_addr
		// address_length == 0x4
		union
		{
			uns8 bytes[4];
			uns16 words[2];
		} ina;

		// in6_addr
		// address_length == 0x10
		union
		{
			uns8 bytes[16];
			uns16 words[8];
		} ina6;

		// address_length == 0xFFFF
		const char* str;
	};

	uns16 port;
	int16 address_length;
};
static_assert(sizeof(transport_address) == 0x14);

extern bool __cdecl transport_address_equivalent(const transport_address* a, const transport_address* b);
extern const char* __cdecl transport_address_get_string(const transport_address* address);
extern void __cdecl transport_address_ipv4_build(transport_address* address, uns32 ip_address, uns16 port);
extern uns32 __cdecl transport_address_ipv4_extract(const transport_address* address);
extern bool __cdecl transport_address_is_loopback(const transport_address* address);
extern char* __cdecl transport_address_to_string(const transport_address* address, const s_transport_secure_address* secure_address, char* string, int16 maximum_string_length, bool include_port, bool include_extra);
extern bool __cdecl transport_address_valid(const transport_address* address);
extern void __cdecl transport_get_broadcast_address(transport_address* address, uns16 port);
extern void __cdecl transport_get_listen_address(transport_address* address, uns16 port);
extern void __cdecl transport_get_listen_address_ipv6(transport_address* address, uns16 port);
extern void __cdecl transport_get_loopback_address(transport_address* address, uns16 port);

extern void transport_address_from_string(const wchar_t* str, transport_address& address);
extern void transport_address_from_string(const char* str, transport_address& address);
extern void transport_address_from_host(const char* name, transport_address& address);

