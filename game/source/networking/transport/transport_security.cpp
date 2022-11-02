#include "networking/transport/transport_security.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"

#include <assert.h>
#include <string.h>

HOOK_DECLARE(0x00430B60, transport_secure_address_decode);
HOOK_DECLARE(0x00430DF0, transport_secure_address_retrieve);
HOOK_DECLARE(0x00430ED0, transport_secure_identifier_get_string);
HOOK_DECLARE(0x00431100, transport_secure_nonce_get_string);

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

bool __cdecl transport_secure_address_decode(s_transport_session_description const* secure_host_description, s_transport_secure_address const* secure_address, transport_address* usable_address)
{
	assert(secure_host_description);
	assert(secure_address);
	assert(usable_address);

	bool result = false;
	HOOK_INVOKE(result =, transport_secure_address_decode, secure_host_description, secure_address, usable_address);
	return result;
}

bool __cdecl transport_secure_address_resolve()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430CF0, transport_secure_address_resolve);
}

bool __cdecl transport_secure_address_retrieve(transport_address const* usable_address, long platform, s_transport_secure_address* secure_address)
{
	bool result = false;
	HOOK_INVOKE(result =, transport_secure_address_retrieve, usable_address, platform, secure_address);
	return result;
}

char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* _string, long maximum_string_length, bool include_online, bool include_mac)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430E20, transport_secure_address_to_string, secure_address, _string, maximum_string_length, include_online, include_mac);
}

char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
	FUNCTION_BEGIN(true);

	char* result = nullptr;
	HOOK_INVOKE(result =, transport_secure_identifier_get_string, secure_identifier);
	return result;
}

bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, long transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00430F30, transport_secure_identifier_retrieve, usable_address, transport_platform, secure_identifier, secure_address);
}

char const* __cdecl transport_secure_nonce_get_string(qword nonce)
{
	union
	{
		qword value;
		byte bytes[8];
	};

	value = nonce;

	static c_static_string<64> transport_secure_nonce_string;
	transport_secure_nonce_string.print("%02X%02X%02X%02X-%02X%02X%02X%02X", bytes[7], bytes[6], bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0]);
	return transport_secure_nonce_string.get_string();
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

void network_debug_print(const char* format, ...)
{
	switch ((long)format)
	{
	case 0x01612028:
	case 0x01616488:
		break;
	default:
	{
		va_list list;
		va_start(list, format);
		c_console::write_line(format, list);
		va_end(list);
	}
	break;
	}
}

HOOK_DECLARE(0x000D858D0, network_debug_print);

