#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct s_transport_address_mapping
{
	bool initialized;
	byte __pad1[0x3];

	transport_address address;
	s_transport_secure_address secure_address;
	s_transport_secure_identifier secure_identifier;
};
static_assert(sizeof(s_transport_address_mapping) == 0x38);

extern s_transport_address_mapping(&g_transport_address_mapping)[51];

extern void __cdecl XNetCreateKey(s_transport_secure_identifier* out_secure_identifier);
extern int32 __cdecl XNetFindEntry(const transport_address* address, const s_transport_secure_address* secure_address, bool ignore_invalid);
extern void __cdecl XNetAddEntry(const transport_address* address, const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier);
extern bool __cdecl XNetXnAddrToInAddr(const s_transport_secure_address* secure_address, const s_transport_secure_identifier* secure_identifier, transport_address* out_address);
extern bool __cdecl _XNetInAddrToXnAddr(const transport_address* address, s_transport_secure_address* out_secure_address);
extern bool __cdecl XNetInAddrToXnAddr(const transport_address* address, s_transport_secure_address* out_secure_address, s_transport_secure_identifier* out_secure_identifier);
extern void __cdecl XNetRemoveEntry(const transport_address* address);

extern uns32 get_external_ip();

