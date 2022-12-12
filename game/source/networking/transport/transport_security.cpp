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

REFERENCE_DECLARE(0x0199FAB0, s_transport_security_globals, transport_security_globals);

//00430AF0 ; void __cdecl transport_secure_address_build_from_identifier(s_transport_unique_identifier const*, s_transport_secure_address*)

bool __cdecl transport_secure_address_compare(s_transport_secure_address const* a, s_transport_secure_address const* b)
{
	return INVOKE(0x00430B20, transport_secure_address_compare, a, b);
}

bool __cdecl transport_secure_address_decode(s_transport_session_description const* secure_host_description, s_transport_secure_address const* secure_address, transport_address* usable_address)
{
	assert(secure_host_description);
	assert(secure_address);
	assert(usable_address);

	//INVOKE(0x00430B60, transport_secure_address_decode, secure_host_description, secure_address, usable_address);

	bool result = false;
	HOOK_INVOKE(result =, transport_secure_address_decode, secure_host_description, secure_address, usable_address);
	return result;
}

void __cdecl transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier)
{
	//INVOKE(0x00430B80, transport_secure_address_extract_identifier, secure_address, unique_identifier);

	memcpy(unique_identifier, secure_address, sizeof(s_transport_unique_identifier));
}

//00430BA0 ; bool __cdecl transport_secure_address_get(s_transport_secure_address*)

bool __cdecl transport_secure_address_get_insecure(transport_address* address)
{
	//return INVOKE(0x00430BD0, transport_secure_address_get_insecure, address);

	if (address)
		*address = transport_security_globals.address;

	return transport_security_globals.address_resolved;
}

//00430C30 ; bool __cdecl transport_secure_address_get_secure_machine_id(s_transport_secure_address const*, qword*)
//00430C40 ; s_transport_secure_address const* __cdecl transport_secure_address_get_safe()
//00430C50 ; char* __cdecl transport_secure_address_get_string(s_transport_secure_address const*)
//00430CC0 ; bool __cdecl transport_secure_address_pending()

//00430CD0 ; XNetAddEntry thunk

void __cdecl transport_secure_address_reset_for_new_networking_mode(void* callback_data)
{
	//INVOKE(0x00430CE0, transport_secure_address_reset_for_new_networking_mode, callback_data);

	transport_security_globals.address_resolved = false;
}

bool __cdecl transport_secure_address_resolve()
{
	return INVOKE(0x00430CF0, transport_secure_address_resolve);
}

bool __cdecl transport_secure_address_retrieve(transport_address const* usable_address, long platform, s_transport_secure_address* secure_address)
{
	//return INVOKE(0x00430E20, transport_secure_address_retrieve, usable_address, platform, secure_address);

	bool result = false;
	HOOK_INVOKE(result =, transport_secure_address_retrieve, usable_address, platform, secure_address);
	return result;
}

char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* _string, long maximum_string_length, bool include_online, bool include_mac)
{
	return INVOKE(0x00430E20, transport_secure_address_to_string, secure_address, _string, maximum_string_length, include_online, include_mac);
}

//00430E90 ; XNetRemoveEntry thunk

//00430EA0 ; bool __cdecl transport_secure_connection_initiate(transport_address const*)
//00430EB0 ; e_transport_secure_connection __cdecl transport_secure_connection_status(transport_address const*)
//00430EC0 ; void __cdecl transport_secure_connection_terminate(transport_address const*)

char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier)
{
	//return INVOKE(0x00430ED0, transport_secure_identifier_get_string, secure_identifier);

	char* result = nullptr;
	HOOK_INVOKE(result =, transport_secure_identifier_get_string, secure_identifier);
	return result;
}

bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, long transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
	return INVOKE(0x00430F30, transport_secure_identifier_retrieve, usable_address, transport_platform, secure_identifier, secure_address);
}

//00430F60 ; bool __cdecl transport_secure_key_create(s_transport_session_description*, e_transport_platform)
//00430FD0 ; bool __cdecl transport_secure_key_register(s_transport_session_description*, e_transport_platform)
//00430FF0 ; bool __cdecl transport_secure_nonce_compare(qword, qword)

qword __cdecl transport_secure_nonce_generate()
{
	return INVOKE(0x00431010, transport_secure_nonce_generate);
}

char const* __cdecl transport_secure_nonce_get_string(qword nonce)
{
	//INVOKE(0x00431100, transport_secure_nonce_get_string, nonce);

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

//00431130 ; void __cdecl transport_secure_random(long, byte*)

void __cdecl transport_security_initialize()
{
	//INVOKE(0x004311A0, transport_security_initialize);

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
	//INVOKE(0x004311D0, transport_security_startup);

	transport_unique_identifier_resolve();
	transport_secure_address_resolve();
}

//00431200 ; char const* __cdecl transport_session_description_get_string(s_transport_session_description const*)
//004312C0 ; s_transport_unique_identifier const* __cdecl transport_unique_identifier_get()
//004312D0 ; char const* __cdecl transport_unique_identifier_get_string(s_transport_unique_identifier const*)

void __cdecl transport_unique_identifier_resolve()
{
	//INVOKE(0x004312E0, transport_unique_identifier_resolve);

	transport_secure_address_resolve();
	transport_secure_address_extract_identifier(&transport_security_globals.secure_address, &transport_security_globals.local_unique_identifier);
}

// used inplace of `c_event::generate`
// net::REMOTE_BINLOGGER
void network_debug_print(const char* format, ...)
{
	switch ((long)format)
	{
	case 0x01612028: // "MP/NET/LINK,RCV: c_network_link::read_data_immediate: Read %d-byte packet from invalid address '%s'."
	case 0x01616488: // "MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties accepted for peer #%d [%s], name=%S, session name=%S, map=%d, map status=%s, map progress=%d, game instance=0x%LX, ready hopper=%d, start error=%s, hdd=%d, upstream=%d bps (reliable %d), downstream=%d bps, nat=%d, ratings (conn, host, client)=%d, %d, %d, connectivity=%04X (complete=%04X), flags=%X"
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

