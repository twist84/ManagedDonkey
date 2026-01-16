#pragma once

#include "networking/transport/transport_address.hpp"

struct dns_result
{
	transport_address address[8];
	char name[256];
};
COMPILE_ASSERT(sizeof(dns_result) == 0x1A0);

extern bool __cdecl transport_dns_address_to_name(dns_result* result);
extern bool __cdecl transport_dns_name_to_address(dns_result* result);

