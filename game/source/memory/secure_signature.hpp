#pragma once

#include "cseries/cseries.hpp"


struct s_network_http_request_hash
{
	dword data[5];
};
static_assert(sizeof(s_network_http_request_hash) == 0x14);

struct s_rsa_signature
{
	byte signature[0x100];
};
static_assert(sizeof(s_rsa_signature) == 0x100);