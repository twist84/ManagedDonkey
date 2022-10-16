#include "networking/transport/transport_security.hpp"

s_transport_security_globals& transport_security_globals = *reinterpret_cast<s_transport_security_globals*>(0x0199FAB0);

bool transport_secure_address_get_insecure(transport_address* address)
{
	if (address)
		*address = transport_security_globals.address;

    return transport_security_globals.address_resolved;
}
