#pragma once

#include "networking/transport/transport_address.hpp"

struct s_transport_unique_identifier
{
	long parts[2];
};

struct s_transport_secure_identifier
{
	long parts[4];
};

struct s_transport_secure_address
{
	long parts[4];
};

struct s_transport_secure_key
{
	long parts[4];
};

#pragma pack(push, 1)
struct s_transport_security_globals
{
	bool initialized;
	bool address_resolved;
	s_transport_secure_address secure_address;
	byte __unknown12;
	byte __unknown13;
	transport_address address;
	s_transport_secure_identifier identifier;
	s_transport_secure_address local_unique_identifier;
	byte __data48[0x190];
};
static_assert(sizeof(s_transport_security_globals) == 0x1D8);
#pragma pack(pop)

extern s_transport_security_globals& transport_security_globals;

extern bool transport_secure_address_get_insecure(transport_address* address);