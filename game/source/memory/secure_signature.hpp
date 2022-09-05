#pragma once

#include "cseries/cseries.hpp"


struct s_network_http_request_hash
{
	dword data[5];
};
static_assert(sizeof(s_network_http_request_hash) == 0x14);

struct s_rsa_signature
{
	qword data[32];
};
static_assert(sizeof(s_rsa_signature) == 0x100);