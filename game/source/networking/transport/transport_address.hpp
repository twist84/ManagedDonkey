#pragma once

#include "cseries/cseries.hpp"

struct transport_address
{
	union
	{
		// in_addr
		union
		{
			byte bytes[4];
			word words[2];
			dword addr;
		} ip4;

		// in6_addr
		union
		{
			byte bytes[16];
			word words[8];
		} ip6;
	};

	short port;
	short address_length;
};
static_assert(sizeof(transport_address) == 0x14);
