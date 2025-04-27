#include "networking/transport/transport_security.hpp"

#include "memory/module.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/online/online.hpp"
#include "networking/transport/transport.hpp"
#include "xbox/xnet.hpp"

REFERENCE_DECLARE(0x0199FAB0, s_transport_security_globals, transport_security_globals);

HOOK_DECLARE(0x00430B60, transport_secure_address_decode);
HOOK_DECLARE(0x00430C10, transport_secure_address_get_local_machine_id);
HOOK_DECLARE(0x00430C30, transport_secure_address_get_machine_id);
HOOK_DECLARE(0x00430DF0, transport_secure_address_retrieve);
HOOK_DECLARE(0x00430ED0, transport_secure_identifier_get_string);
HOOK_DECLARE(0x00430F30, transport_secure_identifier_retrieve);
HOOK_DECLARE(0x00431100, transport_secure_nonce_get_string);

//00430AF0 ; void __cdecl transport_secure_address_build_from_identifier(s_transport_unique_identifier const*, s_transport_secure_address*)

bool __cdecl transport_secure_address_compare(s_transport_secure_address const* a, s_transport_secure_address const* b)
{
	return INVOKE(0x00430B20, transport_secure_address_compare, a, b);
}

// `transport_secure_identifier_compare` and `transport_secure_address_compare` are basically the same function
bool __cdecl transport_secure_identifier_compare(s_transport_secure_identifier const* a, s_transport_secure_identifier const* b)
{
	return INVOKE(0x00430B20, transport_secure_identifier_compare, a, b);
}

bool __cdecl transport_secure_address_decode(s_transport_session_description const* secure_host_description, s_transport_secure_address const* secure_address, transport_address* usable_address)
{
	ASSERT(secure_host_description);
	ASSERT(secure_address);
	ASSERT(usable_address);

	//INVOKE(0x00430B60, transport_secure_address_decode, secure_host_description, secure_address, usable_address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_secure_address_decode, secure_host_description, secure_address, usable_address);
	//return result;

	return XNetXnAddrToInAddr(secure_address, &secure_host_description->id, usable_address);
}

void __cdecl transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier)
{
	//INVOKE(0x00430B80, transport_secure_address_extract_identifier, secure_address, unique_identifier);

	csmemcpy(unique_identifier, secure_address, sizeof(s_transport_unique_identifier));
}

bool __cdecl transport_secure_address_get(s_transport_secure_address* secure_address)
{
	//INVOKE(0x00430BA0, transport_secure_address_get, secure_address);

	if (secure_address)
		*secure_address = transport_security_globals.secure_address;

	return transport_security_globals.address_resolved;
}

bool __cdecl transport_secure_address_get_insecure(transport_address* address)
{
	//return INVOKE(0x00430BD0, transport_secure_address_get_insecure, address);

	if (address)
		*address = transport_security_globals.address;

	return transport_security_globals.address_resolved;
}

uint64 __cdecl transport_secure_address_get_local_machine_id()
{
	//return INVOKE(0x00430C10, transport_secure_address_get_local_machine_id);

	s_transport_secure_address secure_address{};
	if (transport_secure_address_get(&secure_address))
	{
		uint64 machine_id{};
		if (transport_secure_address_get_machine_id(&secure_address, &machine_id))
			return machine_id;
	}

	return 0;
}

bool __cdecl transport_secure_address_get_machine_id(s_transport_secure_address const* secure_address, uint64* secure_machine_id)
{
	//return INVOKE(0x00430C30, transport_secure_address_get_machine_id, secure_address, secure_machine_id);

	//return online_is_connected_to_live() && !XNetXnAddrToMachineId(secure_address, secure_machine_id);

	// no idea what `XNetXnAddrToMachineId` actually does, so we make the machine id from the secure and insecure addresses
	static s_transport_secure_identifier secure_identifier{};
	static transport_address address{};
	XNetXnAddrToInAddr(secure_address, &secure_identifier, &address);

	*secure_machine_id = make_int64(secure_address->part0, address.ipv4_address);
	return online_is_connected_to_live() && *secure_machine_id;
}

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
	//return INVOKE(0x00430DF0, transport_secure_address_retrieve, usable_address, platform, secure_address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_secure_address_retrieve, usable_address, platform, secure_address);
	//return result;

	if (usable_address->address_length == sizeof(uint32) && usable_address->ipv4_address && platform)
		return _XNetInAddrToXnAddr(usable_address, secure_address);

	return false;
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

bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, long platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
	//return INVOKE(0x00430F30, transport_secure_identifier_retrieve, usable_address, platform, secure_identifier, secure_address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_secure_identifier_retrieve, usable_address, platform, secure_identifier, secure_address);
	//return result;

	if (usable_address->address_length == sizeof(uint32) && usable_address->ipv4_address && platform)
		return XNetInAddrToXnAddr(usable_address, secure_address, secure_identifier);

	return false;
}

//00430F60 ; bool __cdecl transport_secure_key_create(s_transport_session_description*, e_transport_platform)
//00430FD0 ; bool __cdecl transport_secure_key_register(s_transport_session_description*, e_transport_platform)

bool __cdecl transport_secure_nonce_compare(uint64 nonce1, uint64 nonce2)
{
	//return INVOKE(0x00430FF0, transport_secure_nonce_compare, nonce1, nonce2);

	//ASSERT(nonce1 && nonce2);

	return nonce1 == nonce2;
}

uint64 __cdecl transport_secure_nonce_generate()
{
	return INVOKE(0x00431010, transport_secure_nonce_generate);
}

char const* __cdecl transport_secure_nonce_get_string(uint64 nonce)
{
	//INVOKE(0x00431100, transport_secure_nonce_get_string, nonce);

	union
	{
		uint64 value;
		uint8 bytes[8];
	};

	value = nonce;

	static c_static_string<64> transport_secure_nonce_string;
	transport_secure_nonce_string.print("%02X%02X%02X%02X-%02X%02X%02X%02X", bytes[7], bytes[6], bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0]);
	return transport_secure_nonce_string.get_string();
}

void __cdecl transport_secure_random(long random_length, uint8* random_data)
{
	long const k_meg = 0x100000;

	ASSERT(random_data);
	ASSERT(random_length > 0 && random_length < k_meg);

	bool used_xnet_random = false;
	if (transport_security_globals.initialized)
	{
		//random_length = XNetRandom(random_data, random_length);
		//used_xnet_random = true;
	}
	
	if (!used_xnet_random)
	{
		INVOKE(0x00431130, transport_secure_random, random_length, random_data);

		//long random_seed = generate_random_seed(random_length);
		//for (long i = 0; i < random_length; ++i)
		//	random_data[i] = _random_range(random_seed, 0, __FILE__, __LINE__, 0, 256);
	}
}

void __cdecl transport_security_initialize()
{
	//INVOKE(0x004311A0, transport_security_initialize);

	csmemset(&transport_security_globals, 0, sizeof(transport_security_globals));
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

void transport_secure_identifier_from_string(wchar_t const* str, s_transport_secure_identifier& secure_identifier)
{
	swscanf_s(str, L"%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&secure_identifier.part0,
		&secure_identifier.part4[0],
		&secure_identifier.part4[1],
		&secure_identifier.part8[0],
		&secure_identifier.part8[1],
		&secure_identifier.part8[2],
		&secure_identifier.part8[3],
		&secure_identifier.part8[4],
		&secure_identifier.part8[5],
		&secure_identifier.part8[6],
		&secure_identifier.part8[7]);
}

void transport_secure_identifier_from_string(char const* str, s_transport_secure_identifier& secure_identifier)
{
	sscanf_s(str, "%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&secure_identifier.part0,
		&secure_identifier.part4[0],
		&secure_identifier.part4[1],
		&secure_identifier.part8[0],
		&secure_identifier.part8[1],
		&secure_identifier.part8[2],
		&secure_identifier.part8[3],
		&secure_identifier.part8[4],
		&secure_identifier.part8[5],
		&secure_identifier.part8[6],
		&secure_identifier.part8[7]);
}

void transport_secure_address_from_string(wchar_t const* str, s_transport_secure_address& secure_address)
{
	swscanf_s(str, L"%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&secure_address.part0,
		&secure_address.part4[0],
		&secure_address.part4[1],
		&secure_address.part8[0],
		&secure_address.part8[1],
		&secure_address.part8[2],
		&secure_address.part8[3],
		&secure_address.part8[4],
		&secure_address.part8[5],
		&secure_address.part8[6],
		&secure_address.part8[7]);
}

void transport_secure_address_from_string(char const* str, s_transport_secure_address& secure_address)
{
	sscanf_s(str, "%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&secure_address.part0,
		&secure_address.part4[0],
		&secure_address.part4[1],
		&secure_address.part8[0],
		&secure_address.part8[1],
		&secure_address.part8[2],
		&secure_address.part8[3],
		&secure_address.part8[4],
		&secure_address.part8[5],
		&secure_address.part8[6],
		&secure_address.part8[7]);
}

