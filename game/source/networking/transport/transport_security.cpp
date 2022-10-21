#include "networking/transport/transport_security.hpp"

#include "cseries/console.hpp"
#include "networking/transport/transport.hpp"

#include <string.h>

s_transport_security_globals& transport_security_globals = *reinterpret_cast<s_transport_security_globals*>(0x0199FAB0);

bool __cdecl transport_secure_address_get_insecure(transport_address* address)
{
	FUNCTION_BEGIN(true);

	if (address)
		*address = transport_security_globals.address;

    return transport_security_globals.address_resolved;
}

void __cdecl transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier)
{
	FUNCTION_BEGIN(true);

	memcpy(unique_identifier, secure_address, sizeof(s_transport_unique_identifier));
}

bool __cdecl transport_secure_address_resolve()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430CF0, transport_secure_address_resolve);
}

char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* _string, long maximum_string_length, bool include_online, bool include_mac)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430E20, transport_secure_address_to_string, secure_address, _string, maximum_string_length, include_online, include_mac);
}

char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430ED0, transport_secure_identifier_get_string, secure_identifier);
}

bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, long transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430F30, transport_secure_identifier_retrieve, usable_address, transport_platform, secure_identifier, secure_address);
}


void __cdecl transport_security_initialize()
{
	FUNCTION_BEGIN(true);

	memset(&transport_security_globals, 0, sizeof(transport_security_globals));
	transport_register_transition_functions(
		0,
		transport_secure_address_reset_for_new_networking_mode,
		transport_secure_address_reset_for_new_networking_mode,
		0);
	transport_security_globals.initialized = true;
}

void __cdecl transport_security_startup()
{
	FUNCTION_BEGIN(true);

	transport_unique_identifier_resolve();
	transport_secure_address_resolve();
}

void __cdecl transport_secure_address_reset_for_new_networking_mode(void* callback_data)
{
	FUNCTION_BEGIN(true);

	transport_security_globals.address_resolved = false;
}

void __cdecl transport_unique_identifier_resolve()
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x004312E0, transport_unique_identifier_resolve);
	transport_secure_address_resolve();
	transport_secure_address_extract_identifier(&transport_security_globals.secure_address, &transport_security_globals.local_unique_identifier);
}
