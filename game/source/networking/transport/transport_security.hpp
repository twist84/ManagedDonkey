#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_address.hpp"

struct s_transport_unique_identifier
{
	dword part0;
	word part4[2];
};
static_assert(sizeof(s_transport_unique_identifier) == 0x8);

struct s_transport_secure_identifier : s_transport_unique_identifier
{
	byte part8[8];
};
static_assert(sizeof(s_transport_secure_identifier) == 0x10);

struct s_transport_secure_address : s_transport_unique_identifier
{
	byte part8[8];
};
static_assert(sizeof(s_transport_secure_address) == 0x10);

struct s_transport_secure_key : s_transport_unique_identifier
{
	byte part8[8];
};
static_assert(sizeof(s_transport_secure_key) == 0x10);

struct s_transport_session_description
{
	s_transport_secure_identifier id;
	s_transport_secure_address address;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_session_description) == 0x30);

#pragma pack(push, 1)
struct s_transport_security_globals
{
	bool initialized;
	bool address_resolved;
	s_transport_secure_address secure_address;
	byte __unknown12;
	byte __unknown13;
	transport_address address;
	s_transport_secure_identifier identifier;
	s_transport_secure_address local_unique_identifier;
	byte __data48[0x190];
};
static_assert(sizeof(s_transport_security_globals) == 0x1D8);
#pragma pack(pop)

extern s_transport_security_globals& transport_security_globals;

extern bool __cdecl transport_secure_address_get_insecure(transport_address* address);
extern void __cdecl transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier);
extern bool __cdecl transport_secure_address_decode(s_transport_session_description const* secure_host_description, s_transport_secure_address const* secure_address, transport_address* usable_address);
extern bool __cdecl transport_secure_address_resolve();
extern bool __cdecl transport_secure_address_retrieve(transport_address const* usable_address, long platform, s_transport_secure_address* secure_address);
extern char* __cdecl transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* _string, long maximum_string_length, bool include_online, bool include_mac);
extern char* __cdecl transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier);
extern bool __cdecl transport_secure_identifier_retrieve(transport_address const* usable_address, long transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address);
extern void __cdecl transport_security_initialize();
extern void __cdecl transport_security_startup();
extern void __cdecl transport_secure_address_reset_for_new_networking_mode(void* callback_data);
extern void __cdecl transport_unique_identifier_resolve();