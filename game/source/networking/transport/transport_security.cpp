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

//00430AF0 ; void __cdecl transport_secure_address_build_from_identifier(const s_transport_unique_identifier*, s_transport_secure_address*)

bool __cdecl transport_secure_address_compare(const s_transport_secure_address* a, const s_transport_secure_address* b)
{
	return INVOKE(0x00430B20, transport_secure_address_compare, a, b);
}

// `transport_secure_identifier_compare` and `transport_secure_address_compare` are basically the same function
bool __cdecl transport_secure_identifier_compare(const s_transport_secure_identifier* a, const s_transport_secure_identifier* b)
{
	return INVOKE(0x00430B20, transport_secure_identifier_compare, a, b);
}

bool __cdecl transport_secure_address_decode(const s_transport_session_description* secure_host_description, const s_transport_secure_address* secure_address, transport_address* usable_address)
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

void __cdecl transport_secure_address_extract_identifier(const s_transport_secure_address* secure_address, s_transport_unique_identifier* unique_identifier)
{
	//INVOKE(0x00430B80, transport_secure_address_extract_identifier, secure_address, unique_identifier);

	csmemcpy(unique_identifier, secure_address, sizeof(s_transport_unique_identifier));
}

bool __cdecl transport_secure_address_get(s_transport_secure_address* secure_address)
{
	//INVOKE(0x00430BA0, transport_secure_address_get, secure_address);

	if (secure_address)
	{
		*secure_address = transport_security_globals.local_secure_address;
	}
	return transport_security_globals.local_address_valid;
}

bool __cdecl transport_secure_address_get_insecure(transport_address* address)
{
	//return INVOKE(0x00430BD0, transport_secure_address_get_insecure, address);

	if (address)
	{
		*address = transport_security_globals.local_insecure_address;
	}
	return transport_security_globals.local_address_valid;
}

uns64 __cdecl transport_secure_address_get_local_machine_id()
{
	//return INVOKE(0x00430C10, transport_secure_address_get_local_machine_id);

	uns64 machine_id{};
	s_transport_secure_address local_secure_address{};
	if (transport_secure_address_get(&local_secure_address))
	{
		uns64 local_machine_id{};
		if (transport_secure_address_get_machine_id(&local_secure_address, &local_machine_id))
		{
			machine_id = local_machine_id;
		}
	}
	return machine_id;
}

bool __cdecl transport_secure_address_get_machine_id(const s_transport_secure_address* secure_address, uns64* secure_machine_id)
{
	//return INVOKE(0x00430C30, transport_secure_address_get_machine_id, secure_address, secure_machine_id);

	//return online_is_connected_to_live() && !XNetXnAddrToMachineId(secure_address, secure_machine_id);

	// no idea what `XNetXnAddrToMachineId` actually does, so we make the machine id from the secure and insecure addresses
	//static s_transport_secure_identifier secure_identifier{};
	//static transport_address address{};
	//XNetXnAddrToInAddr(secure_address, &secure_identifier, &address);
	//
	//*secure_machine_id = make_int64(*(int32*)secure_address->data, address.ipv4_address);
	//return online_is_connected_to_live() && *secure_machine_id;

	*secure_machine_id = 0xFA00000000000000 | make_int64(*(int32*)secure_address->data, (*(int32*)secure_address->data) & 0x0000FFFF);
	bool success = online_is_connected_to_live() && *secure_machine_id;
	return success;
}

//00430C40 ; const s_transport_secure_address* __cdecl transport_secure_address_get_safe()
//00430C50 ; char* __cdecl transport_secure_address_get_string(const s_transport_secure_address*)
//00430CC0 ; bool __cdecl transport_secure_address_pending()

//00430CD0 ; XNetAddEntry thunk

void __cdecl transport_secure_address_reset_for_new_networking_mode(void* callback_data)
{
	//INVOKE(0x00430CE0, transport_secure_address_reset_for_new_networking_mode, callback_data);

	transport_security_globals.local_address_valid = false;
}

bool __cdecl transport_secure_address_resolve()
{
	return INVOKE(0x00430CF0, transport_secure_address_resolve);
}

bool __cdecl transport_secure_address_retrieve(const transport_address* usable_address, int32 address_platform, s_transport_secure_address* secure_address)
{
	//return INVOKE(0x00430DF0, transport_secure_address_retrieve, usable_address, address_platform, secure_address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_secure_address_retrieve, usable_address, address_platform, secure_address);
	//return result;
	
	ASSERT(usable_address);
	ASSERT(secure_address);

	// $TODO add IPV6 support

	bool success = false;
	if (usable_address->address_length == IPV4_ADDRESS_LENGTH && usable_address->ipv4_address && address_platform)
	{
		success = _XNetInAddrToXnAddr(usable_address, secure_address);
	}
	return success;
}

char* __cdecl transport_secure_address_to_string(const s_transport_secure_address* secure_address, char* string, int32 length, bool include_online, bool include_mac)
{
	return INVOKE(0x00430E20, transport_secure_address_to_string, secure_address, string, length, include_online, include_mac);
}

//00430E90 ; XNetRemoveEntry thunk

//00430EA0 ; bool __cdecl transport_secure_connection_initiate(const transport_address*)
//00430EB0 ; e_transport_secure_connection __cdecl transport_secure_connection_status(const transport_address*)
//00430EC0 ; void __cdecl transport_secure_connection_terminate(const transport_address*)

char* __cdecl transport_secure_identifier_get_string(const s_transport_secure_identifier* secure_identifier)
{
	//return INVOKE(0x00430ED0, transport_secure_identifier_get_string, secure_identifier);

	char* result = nullptr;
	HOOK_INVOKE(result =, transport_secure_identifier_get_string, secure_identifier);
	return result;
}

bool __cdecl transport_secure_identifier_retrieve(const transport_address* usable_address, int32 platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address)
{
	//return INVOKE(0x00430F30, transport_secure_identifier_retrieve, usable_address, platform, secure_identifier, secure_address);

	//bool result = false;
	//HOOK_INVOKE(result =, transport_secure_identifier_retrieve, usable_address, platform, secure_identifier, secure_address);
	//return result;

	ASSERT(usable_address);
	ASSERT(secure_identifier);
	ASSERT(secure_address);

	// $TODO add IPV6 support

	bool success = false;
	if (usable_address->address_length == IPV4_ADDRESS_LENGTH && usable_address->ipv4_address && platform)
	{
		success = XNetInAddrToXnAddr(usable_address, secure_address, secure_identifier);
	}
	return success;
}

//00430F60 ; bool __cdecl transport_secure_key_create(s_transport_session_description*, e_transport_platform)
//00430FD0 ; bool __cdecl transport_secure_key_register(s_transport_session_description*, e_transport_platform)

bool __cdecl transport_secure_nonce_compare(uns64 nonce1, uns64 nonce2)
{
	//return INVOKE(0x00430FF0, transport_secure_nonce_compare, nonce1, nonce2);

	//ASSERT(nonce1 && nonce2);

	return nonce1 == nonce2;
}

uns64 __cdecl transport_secure_nonce_generate()
{
	return INVOKE(0x00431010, transport_secure_nonce_generate);
}

const char* __cdecl transport_secure_nonce_get_string(uns64 nonce)
{
	//INVOKE(0x00431100, transport_secure_nonce_get_string, nonce);

	union
	{
		uns64 value;
		byte bytes[8];
	};

	value = nonce;

	static c_static_string<64> transport_secure_nonce_string;
	transport_secure_nonce_string.print("%02X%02X%02X%02X-%02X%02X%02X%02X", bytes[7], bytes[6], bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0]);
	return transport_secure_nonce_string.get_string();
}

void __cdecl transport_secure_random(int32 random_length, byte* random_data)
{
	int32 const k_meg = 0x100000;

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

		//uns32 seed = generate_random_seed();
		//for (int32 byte_index = 0; byte_index < random_length; byte_index++)
		//{
		//	random_data[byte_index] = _random_range(&seed, 0, __FILE__, __LINE__, 0, 256);
		//}
	}
}

void __cdecl transport_security_initialize()
{
	//INVOKE(0x004311A0, transport_security_initialize);

	csmemset(&transport_security_globals, 0, sizeof(transport_security_globals));
	transport_register_transition_functions(
		NULL,
		transport_secure_address_reset_for_new_networking_mode,
		transport_secure_address_reset_for_new_networking_mode,
		NULL);
	transport_security_globals.initialized = true;
}

void __cdecl transport_security_startup()
{
	//INVOKE(0x004311D0, transport_security_startup);

	transport_unique_identifier_resolve();
	transport_secure_address_resolve();
}

//00431200 ; const char* __cdecl transport_session_description_get_string(const s_transport_session_description*)
//004312C0 ; const s_transport_unique_identifier* __cdecl transport_unique_identifier_get()
//004312D0 ; const char* __cdecl transport_unique_identifier_get_string(const s_transport_unique_identifier*)

void __cdecl transport_unique_identifier_resolve()
{
	//INVOKE(0x004312E0, transport_unique_identifier_resolve);

	transport_secure_address_resolve();
	transport_secure_address_extract_identifier(&transport_security_globals.local_secure_address, &transport_security_globals.local_unique_identifier);
}

