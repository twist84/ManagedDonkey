#pragma once

struct s_network_http_request_hash
{
	byte hash[0x14];
};
static_assert(sizeof(s_network_http_request_hash) == 0x14);

struct s_rsa_signature
{
	byte signature[0x100];
};
static_assert(sizeof(s_rsa_signature) == 0x100);

