#pragma once

struct s_network_http_request_hash
{
	byte hash[0x14];
};
COMPILE_ASSERT(sizeof(s_network_http_request_hash) == 0x14);

struct s_rsa_signature
{
	byte signature[0x100];
};
COMPILE_ASSERT(sizeof(s_rsa_signature) == 0x100);

