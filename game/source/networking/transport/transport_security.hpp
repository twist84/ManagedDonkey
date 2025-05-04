#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_address.hpp"

struct s_transport_unique_identifier
{
	byte data[0x8];
};
static_assert(sizeof(s_transport_unique_identifier) == 0x8);

struct s_transport_secure_identifier
{
	byte data[0x10];
};
static_assert(sizeof(s_transport_secure_identifier) == 0x10);

struct s_transport_secure_address
{
	byte data[0x10];
};
static_assert(sizeof(s_transport_secure_address) == 0x10);

struct s_transport_secure_key
{
	byte data[0x10];
};
static_assert(sizeof(s_transport_secure_key) == 0x10);

struct s_transport_session_description
{
	s_transport_secure_identifier id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_session_description) == 0x30);

struct s_transport_security_key
{
	bool active;
	bool generated_locally;
	int32 key_platform;
	s_transport_secure_identifier key_id;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_security_key) == 0x28);

struct s_transport_security_globals
{
	bool initialized;
	bool local_address_valid;
	s_transport_secure_address local_secure_address;
	transport_address local_insecure_address;
	bool local_machine_nonce_valid;
	uns64 local_machine_nonce;
	s_transport_unique_identifier local_unique_identifier;
	byte pad[0x8];
	s_transport_security_key keys[10];
};
static_assert(sizeof(s_transport_security_globals) == 0x1D8);

extern s_transport_security_globals& transport_security_globals;

//extern void __cdecl transport_secure_address_build_from_identifier(s_transport_unique_identifier const*, s_transport_secure_address*);
extern bool __cdecl transport_secure_address_compare(s_transport_secure_address const* a, s_transport_secure_address const* b);
extern bool __cdecl transport_secure_identifier_compare(s_transport_secure_identifier const* a, s_transport_secure_identifier const* b);
extern bool __cdecl transport_secure_address_decode(s_transport_session_description const* secure_host_description, s_transport_secure_address const* secure_address, transport_address* usable_address);
extern void __cdecl transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier);
extern bool __cdecl transport_secure_address_get(s_transport_secure_address*);
extern bool __cdecl transport_secure_address_get_insecure(transport_address* address);
//extern bool __cdecl transport_secure_address_get_secure_machine_id(s_transport_secure_address const*, uns64*);
extern uns64 __cdecl transport_secure_address_get_local_machine_id();
extern bool __cdecl transport_secure_address_get_machine_id(s_transport_secure_address const* secure_address, uns64* secure_machine_id);
//extern s_transport_secure_address const* __cdecl transport_secure_address_get_safe();
//extern char* __cdecl transport_secure_address_get_string(s_transport_secure_address const*);
extern void __cdecl transport_secure_address_reset_for_new_networking_mode(void* callback_data);
extern bool __cdecl transport_secure_address_resolve();
extern bool __cdecl transport_secure_address_retrieve(transport_address const* usable_address, int32 platform, s_transport_secure_address* secure_address);
extern char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* string, int32 maximum_string_length, bool include_online, bool include_mac);
//extern bool __cdecl transport_secure_connection_initiate(transport_address const*);
//extern e_transport_secure_connection __cdecl transport_secure_connection_status(transport_address const*);
//extern void __cdecl transport_secure_connection_terminate(transport_address const*);
extern char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier);
extern bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, int32 transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address);
//extern bool __cdecl transport_secure_key_create(s_transport_session_description*, e_transport_platform);
//extern bool __cdecl transport_secure_key_register(s_transport_session_description*, e_transport_platform);
extern bool __cdecl transport_secure_nonce_compare(uns64 nonce1, uns64 nonce2);
extern uns64 __cdecl transport_secure_nonce_generate();
extern char const* __cdecl transport_secure_nonce_get_string(uns64 nonce);
extern void __cdecl transport_secure_random(int32 random_length, uns8* random_data);
extern void __cdecl transport_security_initialize();
extern void __cdecl transport_security_startup();
//extern char const* __cdecl transport_session_description_get_string(s_transport_session_description const*);
//extern s_transport_unique_identifier const* __cdecl transport_unique_identifier_get();
//extern char const* __cdecl transport_unique_identifier_get_string(s_transport_unique_identifier const*);
extern void __cdecl transport_unique_identifier_resolve();

