#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_search.hpp"

struct c_network_link;
struct c_network_message_gateway;
struct s_available_session;

struct s_network_broadcast_search_globals
{
	bool initialized;
	c_network_link* link;
	c_network_message_gateway* message_gateway;
	bool search_active;
	bool sessions_updated;
	uint32 last_broadcast_message_sent;
	uint64 search_nonce;
	long maximum_session_count;
	s_available_session* available_sessions;
};
static_assert(sizeof(s_network_broadcast_search_globals) == 0x28);

extern s_network_broadcast_search_globals& g_broadcast_search_globals;
long const k_network_broadcast_search_interval = 2000;

struct transport_address;
struct s_network_message_broadcast_reply;

extern void(__cdecl* network_broadcast_search_update_callback)(transport_address* outgoing_address);

extern bool __cdecl network_broadcast_search_active(uint64* search_nonce);
extern bool __cdecl network_broadcast_search_begin(long controller_index, long maximum_session_count, s_available_session* session_storage);
extern void __cdecl network_broadcast_search_dispose();
extern void __cdecl network_broadcast_search_end();
extern void __cdecl network_broadcast_search_handle_reply(transport_address const* address, s_network_message_broadcast_reply const* message);
extern bool __cdecl network_broadcast_search_initialize(c_network_link* link, c_network_message_gateway* message_gateway);
extern void __cdecl network_broadcast_search_update();

