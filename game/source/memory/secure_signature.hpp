#pragma once

#include "cseries/cseries.hpp"

struct s_network_http_request_hash
{
	uint8 hash[0x14];
};
static_assert(sizeof(s_network_http_request_hash) == 0x14);

struct s_rsa_signature
{
	uint8 signature[0x100];
};
static_assert(sizeof(s_rsa_signature) == 0x100);