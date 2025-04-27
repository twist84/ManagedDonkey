#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct s_xnet_entry
{
	bool initialized;
	uint8 __pad1[0x3];

	transport_address address;
	s_transport_secure_address secure_address;
	s_transport_secure_identifier secure_identifier;
};
static_assert(sizeof(s_xnet_entry) == 0x38);

extern s_xnet_entry(&xnet_mapping)[51];

extern void __cdecl XNetCreateKey(s_transport_secure_identifier* out_secure_identifier);
extern long __cdecl XNetFindEntry(transport_address const* address, s_transport_secure_address const* secure_address, bool ignore_invalid);
extern void __cdecl XNetAddEntry(transport_address const* address, s_transport_secure_address const* secure_address, s_transport_secure_identifier  const* secure_identifier);
extern bool __cdecl XNetXnAddrToInAddr(s_transport_secure_address const* secure_address, s_transport_secure_identifier const* secure_identifier, transport_address* out_address);
extern bool __cdecl _XNetInAddrToXnAddr(transport_address const* address, s_transport_secure_address* out_secure_address);
extern bool __cdecl XNetInAddrToXnAddr(transport_address const* address, s_transport_secure_address* out_secure_address, s_transport_secure_identifier* out_secure_identifier);
extern void __cdecl XNetRemoveEntry(transport_address const* address);

extern uint32 get_external_ip();

