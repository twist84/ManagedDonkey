#pragma once

#include "cseries/cseries.hpp"

// s_network_http_request_hash
struct s_secure_signature
{
	dword data[5];
};
static_assert(sizeof(s_secure_signature) == 0x14);
