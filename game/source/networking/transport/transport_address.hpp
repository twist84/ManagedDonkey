#pragma once

#include "cseries/cseries.hpp"

struct s_player_identifier;
struct s_transport_secure_address;

struct transport_address
{
	transport_address();
	transport_address(uint32 _ipv4_address, uint16 _port, int16 _address_length);
	transport_address(s_player_identifier const* player_identifier);

	union
	{
		uint32 ipv4_address;

		// in_addr
		// address_length == 0x4
		union
		{
			uint8 bytes[4];
			uint16 words[2];
		} ina;

		// in6_addr
		// address_length == 0x10
		union
		{
			uint8 bytes[16];
			uint16 words[8];
		} ina6;

		// address_length == 0xFFFF
		char const* str;
	};

	uint16 port;
	int16 address_length;
};
static_assert(sizeof(transport_address) == 0x14);

struct dns_result
{
	transport_address address[8];
	char name[256];
};
static_assert(sizeof(dns_result) == 0x1A0);

extern bool __cdecl transport_address_equivalent(transport_address const* a, transport_address const* b);
extern char const* __cdecl transport_address_get_string(transport_address const* address);
extern void __cdecl transport_address_ipv4_build(transport_address* address, uint32 ip_address, uint16 port);
extern uint32 __cdecl transport_address_ipv4_extract(transport_address const* address);
extern bool __cdecl transport_address_is_loopback(transport_address const* address);
extern char* __cdecl transport_address_to_string(transport_address const* address, s_transport_secure_address const* secure_address, char* string, int16 maximum_string_length, bool include_port, bool include_extra);
extern bool __cdecl transport_address_valid(transport_address const* address);
extern void __cdecl transport_get_broadcast_address(transport_address* address, uint16 port);
extern void __cdecl transport_get_listen_address(transport_address* address, uint16 port);
extern void __cdecl transport_get_loopback_address(transport_address* address, uint16 port);

extern void transport_address_from_string(wchar_t const* str, transport_address& address);
extern void transport_address_from_string(char const* str, transport_address& address);
extern void transport_address_from_host(char const* name, transport_address& address);

