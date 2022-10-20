#include "networking/transport/transport_security.hpp"

s_transport_security_globals& transport_security_globals = *reinterpret_cast<s_transport_security_globals*>(0x0199FAB0);

bool __cdecl transport_secure_address_get_insecure(transport_address* address)
{
	if (address)
		*address = transport_security_globals.address;

    return transport_security_globals.address_resolved;
}

char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* _string, long maximum_string_length, bool include_online, bool include_mac)
{
	return INVOKE(0x00430E20, transport_secure_address_to_string, secure_address, _string, maximum_string_length, include_online, include_mac);
}

char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
	return INVOKE(0x00430ED0, transport_secure_identifier_get_string, secure_identifier);
}

